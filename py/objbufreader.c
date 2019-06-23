/*
 * This file is part of the Pycopy project, https://github.com/pfalcon/pycopy
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Paul Sokolovsky
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

#include <string.h>

#include "py/runtime.h"
#include "py/stream.h"

#if MICROPY_PY_IO_BUFFEREDREADER
typedef struct _mp_obj_bufreader_t {
    mp_obj_base_t base;
    mp_obj_t stream;
    size_t alloc;
    size_t len;
    size_t pos;
    byte buf[0];
} mp_obj_bufreader_t;

STATIC mp_obj_t bufreader_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 2, 2, false);
    size_t alloc = mp_obj_get_int(args[1]);
    mp_obj_bufreader_t *o = m_new_obj_var(mp_obj_bufreader_t, byte, alloc);
    o->base.type = type;
    o->stream = args[0];
    o->alloc = alloc;
    o->len = 0;
    o->pos = 0;
    return o;
}

STATIC mp_uint_t bufreader_read(mp_obj_t self_in, void *buf, mp_uint_t size, int *errcode) {
    mp_obj_bufreader_t *self = MP_OBJ_TO_PTR(self_in);

    mp_uint_t ret_sz = 0;

    while (size != 0) {
        if (self->len == 0) {
            mp_uint_t in_sz = mp_stream_read_exactly(self->stream, self->buf, self->alloc, errcode);
            if (in_sz == 0 || in_sz == MP_STREAM_ERROR) {
                if (ret_sz != 0) {
                    return ret_sz;
                }
                return in_sz;
            }

            self->pos = 0;
            self->len = in_sz;
        }

        mp_uint_t buf_sz = MIN(size, self->len);
        memcpy(buf, self->buf + self->pos, buf_sz);
        self->pos += buf_sz;
        self->len -= buf_sz;
        size -= buf_sz;
        ret_sz += buf_sz;
        buf = (byte *)buf + buf_sz;
    }

    return ret_sz;
}

STATIC const mp_rom_map_elem_t bufreader_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&mp_stream_read_obj) },
};
STATIC MP_DEFINE_CONST_DICT(bufreader_locals_dict, bufreader_locals_dict_table);

STATIC const mp_stream_p_t bufreader_stream_p = {
    .read = bufreader_read,
};

const mp_obj_type_t mp_type_bufreader = {
    { &mp_type_type },
    .name = MP_QSTR_BufferedReader,
    .make_new = bufreader_make_new,
    .protocol = &bufreader_stream_p,
    .locals_dict = (mp_obj_dict_t *)&bufreader_locals_dict,
};

#endif // MICROPY_PY_IO_BUFFEREDREADER
