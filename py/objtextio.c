/*
 * This file is part of the Pycopy project, https://github.com/pfalcon/pycopy
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Paul Sokolovsky
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
#include "py/mperrno.h"
#include "py/objtextio.h"

#define MICROPY_PY_IO_TEXTIOBASE (1)
#define MICROPY_PY_IO_TEXTIOBASE_FILENO (1)
#define MICROPY_PY_IO_TEXTIOBASE_WRITE (0)

#if MICROPY_PY_IO_TEXTIOBASE

STATIC mp_obj_t textio_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 1, false);
    mp_get_stream_raise(args[0], MP_STREAM_OP_READ | MP_STREAM_OP_WRITE | MP_STREAM_OP_IOCTL);
    mp_obj_textio_t *o = m_new_obj(mp_obj_textio_t);
    o->base.type = type;
    o->stream = args[0];
    return MP_OBJ_FROM_PTR(o);
}

STATIC mp_obj_t textio_read(size_t n_args, const mp_obj_t *args) {
    mp_obj_textio_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t num;

    if (n_args == 1 || ((num = mp_obj_get_int(args[1])) == -1)) {
        return mp_stream_readall(self->stream, &mp_type_str);
    }

    return mp_stream_read_utf8(self->stream, (mp_uint_t)num);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(textio_read_obj, 1, 2, textio_read);

STATIC mp_obj_t textio_readline(size_t n_args, const mp_obj_t *args) {
    mp_obj_textio_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_obj_t newargs[3] = {self->stream, args[1], args[2]};
    return mp_stream_readline(n_args, newargs, &mp_type_str);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(textio_readline_obj, 1, 3, textio_readline);

#if MICROPY_PY_IO_TEXTIOBASE_WRITE
mp_obj_t textio_write(size_t n_args, const mp_obj_t *args) {
    mp_obj_textio_t *self = MP_OBJ_TO_PTR(args[0]);
    // TODO: Hack, overrides const data
    ((mp_obj_t *)args)[0] = self->stream;
    // TODO: Should return number of written characters, not bytes
    return mp_stream_write_method(n_args, args);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(textio_write_obj, 2, 4, textio_write);
#endif

#if MICROPY_PY_IO_TEXTIOBASE_FILENO
STATIC mp_obj_t textio_fileno(mp_obj_t self_in) {
    mp_obj_textio_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_call_method_by_name_n_kw(self->stream, MP_QSTR_fileno, 0, 0, NULL);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(textio_fileno_obj, textio_fileno);
#endif

STATIC const mp_rom_map_elem_t textio_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&textio_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_readline), MP_ROM_PTR(&textio_readline_obj) },
    #if MICROPY_PY_IO_TEXTIOBASE_WRITE
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&textio_write_obj) },
    #endif
    #if MICROPY_PY_IO_TEXTIOBASE_FILENO
    { MP_ROM_QSTR(MP_QSTR_fileno), MP_ROM_PTR(&textio_fileno_obj) },
    #endif
};

STATIC MP_DEFINE_CONST_DICT(textio_locals_dict, textio_locals_dict_table);

#if 0
STATIC const mp_stream_p_t textiobase_stream_p = {
    .write = textio_write,
    .is_text = true,
};
#endif

const mp_obj_type_t mp_type_textiobase = {
    { &mp_type_type },
    .name = MP_QSTR_TextIOBase,
    .make_new = textio_make_new,
    .locals_dict = (void *)&textio_locals_dict,
};

#endif // MICROPY_PY_IO_TEXTIOBASE
