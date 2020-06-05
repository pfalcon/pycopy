/*
 * This file is part of the Pycopy project, https://github.com/pfalcon/pycopy
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Paul Sokolovsky
 *
 * See below for the full license text.
 */
/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2020 Paul Sokolovsky
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

#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/stream.h"

#if MICROPY_PY_USSL && MICROPY_SSL_AXTLS

#include "ssl.h"

typedef struct _mp_obj_ssl_context_t {
    mp_obj_base_t base;
    SSL_CTX *ssl_ctx;
} mp_obj_ssl_context_t;

typedef struct _mp_obj_ssl_socket_t {
    mp_obj_base_t base;
    mp_obj_t sock;
    SSL *ssl_sock;
    byte *buf;
    uint32_t bytes_left;
    bool blocking;
    // We need to store context pointer if for nothing else then to preclude
    // it to be GCed.
    mp_obj_t ctx;
} mp_obj_ssl_socket_t;

struct ssl_sock_args {
    mp_arg_val_t server_side;
    mp_arg_val_t server_hostname;
    mp_arg_val_t do_handshake;
};

STATIC const mp_obj_type_t ussl_socket_type;

STATIC NORETURN void ussl_raise_error(int code) {
    mp_obj_t args[2] = {MP_OBJ_NEW_SMALL_INT(code), MP_OBJ_NEW_QSTR(MP_QSTR_axTLS)};
    nlr_raise(mp_obj_new_exception_args(&mp_type_OSError, 2, args));
}

STATIC mp_obj_t ussl_context_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    // Args are: protocol (CPy compat, ignored so far).
    (void)args;
    mp_arg_check_num(n_args, n_kw, 0, 1, false);

    mp_obj_ssl_context_t *o = m_new_obj(mp_obj_ssl_context_t);
    o->base.type = type;

    uint32_t options = SSL_SERVER_VERIFY_LATER | SSL_NO_DEFAULT_KEY;

    if ((o->ssl_ctx = ssl_ctx_new(options, SSL_DEFAULT_CLNT_SESS)) == NULL) {
        mp_raise_OSError(MP_EINVAL);
    }

    return MP_OBJ_FROM_PTR(o);
}

STATIC mp_obj_t ussl_context_set_cert_key(mp_obj_t self_in, mp_obj_t cert, mp_obj_t key) {
    mp_obj_ssl_context_t *self = MP_OBJ_TO_PTR(self_in);
    mp_buffer_info_t bufinfo;

    mp_get_buffer_raise(key, &bufinfo, MP_BUFFER_READ);
    int res = ssl_obj_memory_load(self->ssl_ctx, SSL_OBJ_RSA_KEY, bufinfo.buf, bufinfo.len, NULL);
    if (res != SSL_OK) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid key"));
    }

    mp_get_buffer_raise(cert, &bufinfo, MP_BUFFER_READ);
    res = ssl_obj_memory_load(self->ssl_ctx, SSL_OBJ_X509_CERT, bufinfo.buf, bufinfo.len, NULL);
    if (res != SSL_OK) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid cert"));
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(ussl_context_set_cert_key_obj, ussl_context_set_cert_key);

STATIC mp_obj_ssl_socket_t *ussl_socket_new(mp_obj_t sock, mp_obj_t ssl_ctx_obj, struct ssl_sock_args *args) {
    #if MICROPY_PY_USSL_FINALISER
    mp_obj_ssl_socket_t *o = m_new_obj_with_finaliser(mp_obj_ssl_socket_t);
    #else
    mp_obj_ssl_socket_t *o = m_new_obj(mp_obj_ssl_socket_t);
    #endif
    o->base.type = &ussl_socket_type;
    o->ctx = ssl_ctx_obj;
    o->buf = NULL;
    o->bytes_left = 0;
    o->sock = sock;
    o->blocking = true;

    mp_obj_ssl_context_t *ssl_ctx = MP_OBJ_TO_PTR(ssl_ctx_obj);

    // axTLS doesn't allow to set SSL_CONNECT_IN_PARTS on connection object,
    // as makes sense, but copies this flag from ssl_ctx to the connection.
    // So, we patch it in ssl_ctx every time.
    if (!args->do_handshake.u_bool) {
        ssl_ctx->ssl_ctx->options |= SSL_CONNECT_IN_PARTS;
    } else {
        ssl_ctx->ssl_ctx->options &= ~SSL_CONNECT_IN_PARTS;
    }

    if (args->server_side.u_bool) {
        o->ssl_sock = ssl_server_new(ssl_ctx->ssl_ctx, (long)sock);
    } else {
        SSL_EXTENSIONS *ext = ssl_ext_new();

        if (args->server_hostname.u_obj != mp_const_none) {
            ext->host_name = (char *)mp_obj_str_get_str(args->server_hostname.u_obj);
        }

        o->ssl_sock = ssl_client_new(ssl_ctx->ssl_ctx, (long)sock, NULL, 0, ext);

        if (args->do_handshake.u_bool) {
            int res = ssl_handshake_status(o->ssl_sock);

            if (res != SSL_OK) {
                ussl_raise_error(res);
            }
        }

    }

    return o;
}

STATIC void ussl_socket_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    mp_obj_ssl_socket_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<_SSLSocket %p>", self->ssl_sock);
}

STATIC mp_uint_t ussl_socket_read(mp_obj_t o_in, void *buf, mp_uint_t size, int *errcode) {
    mp_obj_ssl_socket_t *o = MP_OBJ_TO_PTR(o_in);

    if (o->ssl_sock == NULL) {
        *errcode = EBADF;
        return MP_STREAM_ERROR;
    }

    while (o->bytes_left == 0) {
        mp_int_t r = ssl_read(o->ssl_sock, &o->buf);
        if (r == SSL_OK) {
            // SSL_OK from ssl_read() means "everything is ok, but there's
            // no user data yet". It may happen e.g. if handshake is not
            // finished yet. The best way we can treat it is by returning
            // EAGAIN. This may be a bit unexpected in blocking mode, but
            // default is to perform complete handshake in constructor, so
            // this should not happen in blocking mode. On the other hand,
            // in nonblocking mode EAGAIN (comparing to the alternative of
            // looping) is really preferrable.
            if (o->blocking) {
                continue;
            } else {
                goto eagain;
            }
        }
        if (r < 0) {
            if (r == SSL_CLOSE_NOTIFY || r == SSL_ERROR_CONN_LOST) {
                // EOF
                return 0;
            }
            if (r == SSL_EAGAIN) {
            eagain:
                r = MP_EAGAIN;
            }
            *errcode = r;
            return MP_STREAM_ERROR;
        }
        o->bytes_left = r;
    }

    if (size > o->bytes_left) {
        size = o->bytes_left;
    }
    memcpy(buf, o->buf, size);
    o->buf += size;
    o->bytes_left -= size;
    return size;
}

STATIC mp_uint_t ussl_socket_write(mp_obj_t o_in, const void *buf, mp_uint_t size, int *errcode) {
    mp_obj_ssl_socket_t *o = MP_OBJ_TO_PTR(o_in);

    if (o->ssl_sock == NULL) {
        *errcode = EBADF;
        return MP_STREAM_ERROR;
    }

    mp_int_t r = ssl_write(o->ssl_sock, buf, size);
    if (r < 0) {
        *errcode = r;
        return MP_STREAM_ERROR;
    }
    return r;
}

STATIC mp_uint_t ussl_socket_ioctl(mp_obj_t o_in, mp_uint_t request, uintptr_t arg, int *errcode) {
    mp_obj_ssl_socket_t *self = MP_OBJ_TO_PTR(o_in);
    if (request == MP_STREAM_CLOSE && self->ssl_sock != NULL) {
        ssl_free(self->ssl_sock);
        self->ssl_sock = NULL;
    }
    // Pass all requests down to the underlying socket
    return mp_get_stream(self->sock)->ioctl(self->sock, request, arg, errcode);
}

STATIC mp_obj_t ussl_socket_setblocking(mp_obj_t self_in, mp_obj_t flag_in) {
    mp_obj_ssl_socket_t *o = MP_OBJ_TO_PTR(self_in);
    mp_obj_t sock = o->sock;
    mp_obj_t dest[3];
    mp_load_method(sock, MP_QSTR_setblocking, dest);
    dest[2] = flag_in;
    mp_obj_t res = mp_call_method_n_kw(1, 0, dest);
    o->blocking = mp_obj_is_true(flag_in);
    return res;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(ussl_socket_setblocking_obj, ussl_socket_setblocking);

STATIC const mp_rom_map_elem_t ussl_socket_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&mp_stream_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_readinto), MP_ROM_PTR(&mp_stream_readinto_obj) },
    { MP_ROM_QSTR(MP_QSTR_readline), MP_ROM_PTR(&mp_stream_unbuffered_readline_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&mp_stream_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_setblocking), MP_ROM_PTR(&ussl_socket_setblocking_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&mp_stream_close_obj) },
    #if MICROPY_PY_USSL_FINALISER
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&mp_stream_close_obj) },
    #endif
};

STATIC MP_DEFINE_CONST_DICT(ussl_socket_locals_dict, ussl_socket_locals_dict_table);

STATIC const mp_stream_p_t ussl_socket_stream_p = {
    .read = ussl_socket_read,
    .write = ussl_socket_write,
    .ioctl = ussl_socket_ioctl,
};

STATIC const mp_obj_type_t ussl_socket_type = {
    { &mp_type_type },
    // Save on qstr's, reuse same as for module
    .name = MP_QSTR_ussl,
    .print = ussl_socket_print,
    .getiter = NULL,
    .iternext = NULL,
    .protocol = &ussl_socket_stream_p,
    .locals_dict = (void *)&ussl_socket_locals_dict,
};

STATIC mp_obj_t ussl_context_wrap_socket(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // TODO: Implement more args
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_server_side, MP_ARG_KW_ONLY | MP_ARG_BOOL, {.u_bool = false} },
        { MP_QSTR_server_hostname, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_do_handshake, MP_ARG_KW_ONLY | MP_ARG_BOOL, {.u_bool = true} },
    };

    mp_obj_t ssl_ctx = pos_args[0];
    mp_obj_t sock = pos_args[1];

    struct ssl_sock_args args;
    mp_arg_parse_all(n_args - 2, pos_args + 2, kw_args,
        MP_ARRAY_SIZE(allowed_args), allowed_args, (mp_arg_val_t *)&args);

    return MP_OBJ_FROM_PTR(ussl_socket_new(sock, ssl_ctx, &args));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(ussl_context_wrap_socket_obj, 2, ussl_context_wrap_socket);

STATIC const mp_rom_map_elem_t ussl_context_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_set_cert_key), MP_ROM_PTR(&ussl_context_set_cert_key_obj) },
    { MP_ROM_QSTR(MP_QSTR_wrap_socket), MP_ROM_PTR(&ussl_context_wrap_socket_obj) },
};

STATIC MP_DEFINE_CONST_DICT(ussl_context_locals_dict, ussl_context_locals_dict_table);

STATIC const mp_obj_type_t ussl_context_type = {
    { &mp_type_type },
    .name = MP_QSTR_SSLContext,
    .make_new = ussl_context_make_new,
    .locals_dict = (void *)&ussl_context_locals_dict,
};

STATIC const mp_rom_map_elem_t mp_module_ssl_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ussl) },
    { MP_ROM_QSTR(MP_QSTR_SSLContext), MP_ROM_PTR(&ussl_context_type) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_ssl_globals, mp_module_ssl_globals_table);

const mp_obj_module_t mp_module_ussl = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_ssl_globals,
};

#endif // MICROPY_PY_USSL
