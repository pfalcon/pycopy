/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Damien P. George
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
#ifndef MICROPY_INCLUDED_PY_OBJSTRINGIO_H
#define MICROPY_INCLUDED_PY_OBJSTRINGIO_H

#include "py/obj.h"

typedef struct _mp_obj_stringio_t {
    mp_obj_base_t base;
    vstr_t *vstr;
    // StringIO has single pointer used for both reading and writing
    mp_uint_t pos;
    // Underlying object buffered by this StringIO
    mp_obj_t ref_obj;
} mp_obj_stringio_t;

// This is "finalizer" function for a case when mp_get_buffer() is called on
// BytesIO object. If mp_get_buffer() returns bufinfo->typecode == BYTESIO_TYPECODE,
// this function should be called with the number of bytes actually written into
// BytesIO's buffer.
static inline void mp_objstringio_update_len(mp_obj_t o_in, size_t increment) {
    mp_obj_stringio_t *o = MP_OBJ_TO_PTR(o_in);
    size_t new_len = increment + o->pos;
    if (new_len > o->vstr->len) {
        o->vstr->len = new_len;
    }
    o->pos += increment;
}

#endif // MICROPY_INCLUDED_PY_OBJSTRINGIO_H
