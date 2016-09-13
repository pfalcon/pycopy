/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Paul Sokolovsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mpconfig.h"
#if 1 // MICROPY_PY_

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "py/nlr.h"
#include "py/obj.h"
#include "lib/netutils/netutils.h"

// Zephyr includes
#include <zephyr.h>
#include <net/net_core.h>
#include <net/net_socket.h>
#include <net/ip_buf.h>
#define PACK_ALIAS_STRUCT __attribute__((__packed__,__may_alias__))
#include <net/ip/contiki/ip/uipaddr.h>


#define IPADDR {{192, 0, 2, 2}}
#define MY_IPADDR {IPADDR}

#define WAIT_TICKS TICKS_UNLIMITED

// These are uIP public interface variables wich specify this host address/netmask.
uip_ipaddr_t uip_hostaddr = IPADDR;
uip_ipaddr_t uip_netmask = { { 255, 255, 255, 0 } };

static struct net_addr my_addr = {
    .family = AF_INET,
    { .in_addr = MY_IPADDR },
};

typedef struct _socket_obj_t {
    mp_obj_base_t base;
    struct net_context *sock;
    struct net_addr peer_addr;

    #define STATE_NEW 0
    #define STATE_CONNECTING 1
    #define STATE_CONNECTED 2
    #define STATE_PEER_CLOSED 3
    #define STATE_CLOSED 4
    byte state;
} socket_obj_t;

STATIC void socket_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    socket_obj_t *self = self_in;
    if (self->sock != NULL) {
        mp_printf(print, "<socket %p: state=%d Zstatus=%d uip_conn=%p>",
            self->sock,
            self->state,
            net_context_get_connection_status(self->sock),
            net_context_get_internal_connection(self->sock));
    } else {
        mp_printf(print, "<socket %p: state=%d>", self->sock, self->state);
    }
}

STATIC mp_obj_t socket_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    assert(n_args == 0);
#if 0
    // Net initialization on demand. Has been moved to main(), to let
    // the app respond to pings imemdiately after startup.
    if (net_init() < 0) {
        printf("Error in net_init()\n");
        return mp_const_none;
    }
#endif
    socket_obj_t *o = m_new_obj(socket_obj_t);
    o->base.type = type;
    // We don't know if this will be client or server socket, so it's
    // instantiated lazily
    o->sock = NULL;
    o->state = STATE_NEW;
    return o;
}

STATIC mp_obj_t socket_connect(mp_obj_t self_in, mp_obj_t addr_in) {
    socket_obj_t *self = self_in;

    // Get address
    uint8_t ip[NETUTILS_IPV4ADDR_BUFSIZE];
    mp_uint_t port = netutils_parse_inet_addr(addr_in, ip, NETUTILS_BIG);
    printf("resolved: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

    struct in_addr in4addr_peer = {{{ip[0], ip[1], ip[2], ip[3]}}};
    self->peer_addr.in_addr = in4addr_peer;
    self->peer_addr.family = AF_INET;

    int proto = IPPROTO_TCP;
    self->sock = net_context_get(proto, &self->peer_addr, port, &my_addr, 0);

    int ret = net_context_tcp_init(self->sock, NULL, NET_TCP_TYPE_CLIENT);
    printf("ret=%d\n", ret);
    // Blocking wait until actually connected
    while (net_context_get_connection_status(self->sock) == -EINPROGRESS) {
        printf("st=%d\n", net_context_get_connection_status(self->sock));
        task_sleep(sys_clock_ticks_per_sec / 10);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_connect_obj, socket_connect);

STATIC mp_obj_t socket_send(mp_obj_t self_in, mp_obj_t buf_in) {
    socket_obj_t *self = self_in;

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);

    struct net_buf *netbuf = ip_buf_get_tx(self->sock);
    uint8_t *ptr = net_buf_add(netbuf, bufinfo.len);
    memcpy(ptr, bufinfo.buf, bufinfo.len);
    ip_buf_appdatalen(netbuf) = bufinfo.len;

    int ret = net_send(netbuf);

    return mp_obj_new_int(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_send_obj, socket_send);


STATIC mp_obj_t socket_recv(mp_obj_t self_in, mp_obj_t len_in) {
    socket_obj_t *self = self_in;

    if (self->state == STATE_PEER_CLOSED) {
        return mp_const_empty_bytes;
    }

    struct net_buf *buf = net_receive(self->sock, WAIT_TICKS);
    mp_obj_t ret = mp_obj_new_bytes(ip_buf_appdata(buf), ip_buf_appdatalen(buf));
    if (uip_closed(buf)) {
//printf("uip_closed() == true\n");
        self->state = STATE_PEER_CLOSED;
    }
    ip_buf_unref(buf);
    return ret;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_recv_obj, socket_recv);

STATIC mp_obj_t socket_close(mp_obj_t self_in) {
    socket_obj_t *self = self_in;
    net_context_put(self->sock);
    self->sock = NULL;
    self->state = STATE_CLOSED;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(socket_close_obj, socket_close);

STATIC const mp_map_elem_t socket_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_connect), (mp_obj_t)&socket_connect_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send), (mp_obj_t)&socket_send_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv), (mp_obj_t)&socket_recv_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_close), (mp_obj_t)&socket_close_obj },
};
STATIC MP_DEFINE_CONST_DICT(socket_locals_dict, socket_locals_dict_table);

STATIC const mp_obj_type_t socket_type = {
    { &mp_type_type },
    .name = MP_QSTR_socket,
    .print = socket_print,
    .make_new = socket_make_new,
    .locals_dict = (mp_obj_t)&socket_locals_dict,
};

STATIC const mp_map_elem_t machine_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_usocket) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_socket), (mp_obj_t)&socket_type },
};

STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);

const mp_obj_module_t mp_module_socket = {
    .base = { &mp_type_module },
    .name = MP_QSTR_usocket,
    .globals = (mp_obj_dict_t*)&machine_module_globals,
};

#endif // MICROPY_PY_
