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
#ifndef PYCOPY_INCLUDED_PY_OBJTEXTIO_H
#define PYCOPY_INCLUDED_PY_OBJTEXTIO_H

#include <stdio.h>
#include <string.h>

#include "py/runtime.h"

typedef struct _mp_obj_textio_t {
    mp_obj_base_t base;
    union {
        mp_obj_t stream;
        mp_rom_obj_t rom_stream;
    };
} mp_obj_textio_t;

extern const mp_obj_type_t mp_type_textiobase;

#endif // PYCOPY_INCLUDED_PY_OBJTEXTIO_H
