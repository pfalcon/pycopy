/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Josef Gajdusek
 * Copyright (c) 2015 Paul Sokolovsky
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

mp_obj_t mp_call_function_1_protected(mp_obj_t fun, mp_obj_t arg) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_obj_t ret = mp_call_function_1(fun, arg);
        nlr_pop();
        return ret;
    } else {
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
        return MP_OBJ_NULL;
    }
}

mp_obj_t mp_call_function_2_protected(mp_obj_t fun, mp_obj_t arg1, mp_obj_t arg2) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_obj_t ret = mp_call_function_2(fun, arg1, arg2);
        nlr_pop();
        return ret;
    } else {
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
        return MP_OBJ_NULL;
    }
}

mp_obj_t mp_call_method_by_name_n_kw(mp_obj_t obj, qstr meth_name, size_t n_args, size_t n_kw, const mp_obj_t *meth_args) {
    mp_obj_t dest[2 + n_args + 2 * n_kw];
    mp_load_method(obj, meth_name, dest);
    memcpy(dest + 2, meth_args, (n_args + 2 * n_kw) * sizeof(*meth_args));
    return mp_call_method_n_kw(n_args, n_kw, dest);
}
