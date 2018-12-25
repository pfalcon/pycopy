/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Paul Sokolovsky
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

#include <stdlib.h>

#include "py/runtime.h"

typedef struct _mp_obj_roproxy_t {
    mp_obj_base_t base;
    mp_obj_t obj;
} mp_obj_roproxy_t;

STATIC void roproxy_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_obj_roproxy_t *self = MP_OBJ_TO_PTR(self_in);

    if (kind == PRINT_REPR) {
        mp_print_str(print, "<roproxy ");
        mp_obj_print_helper(print, self->obj, PRINT_REPR);
        mp_print_str(print, ">");
    } else {
        mp_obj_print_helper(print, self->obj, kind);
    }
}

STATIC mp_obj_t roproxy_make_new(const mp_obj_type_t *type_in, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    (void)type_in;
    mp_arg_check_num(n_args, n_kw, 1, 1, false);

    mp_obj_roproxy_t *self = m_new_obj(mp_obj_roproxy_t);
    self->base.type = &mp_type_roproxy;
    self->obj = args[0];

    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t roproxy_unary_op(mp_unary_op_t op, mp_obj_t o_in) {
    mp_obj_roproxy_t *self = MP_OBJ_TO_PTR(o_in);
    return mp_unary_op(op, self->obj);
}

STATIC mp_obj_t roproxy_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in) {
    mp_obj_roproxy_t *self = MP_OBJ_TO_PTR(lhs_in);
    return mp_binary_op(op, self->obj, rhs_in);
}

STATIC mp_obj_t roproxy_subscr(mp_obj_t self_in, mp_obj_t index, mp_obj_t value) {
    if (value == MP_OBJ_SENTINEL) {
        mp_obj_roproxy_t *self = MP_OBJ_TO_PTR(self_in);
        return mp_obj_subscr(self->obj, index, value);
    }

    return MP_OBJ_NULL;
}

STATIC void roproxy_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    if (dest[0] == MP_OBJ_NULL) {
        // load attribute
        mp_obj_roproxy_t *self = MP_OBJ_TO_PTR(self_in);
        mp_load_method(self->obj, attr, dest);
    }
}

const mp_obj_type_t mp_type_roproxy = {
    { &mp_type_type },
    .name = MP_QSTR_roproxy,
    .print = roproxy_print,
    .make_new = roproxy_make_new,
    .unary_op = roproxy_unary_op,
    .binary_op = roproxy_binary_op,
    .subscr = roproxy_subscr,
    .attr  = roproxy_attr,
};
