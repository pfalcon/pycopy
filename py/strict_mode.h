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

extern bool mp_strict_mode;
extern bool mp_strict_runtime;

#define MP_HANDLE_DEL_NS_STRICT() \
    { if (mp_strict_runtime) mp_strict_mode_error(); }

NORETURN void mp_strict_mode_error(void);
bool mp_handle_store_ns_strict(mp_map_t *map, mp_obj_t attr, mp_obj_t val, bool is_const);
void mp_strict_update_main_name(void);


#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A

#define MP_MAP_QSTR_KEY_UNCONST(key) ((mp_obj_t)((mp_int_t)(key) & ~1))
#define MP_MAP_QSTR_KEY_CONST(key) ((mp_obj_t)((mp_int_t)(key) | 1))
#define MP_MAP_QSTR_KEY_IS_CONST(key) ((mp_int_t)(key) & 1)
#define MP_MAP_GET_KEY_MASK(map) ((map)->all_keys_are_qstrs ? (mp_uint_t) ~1 : (mp_uint_t) ~0)
#define MP_MAP_KEY_APPLY_MASK(key, mask) ((mp_obj_t)((mp_uint_t)(key) & (mask)))

#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C

#define MP_MAP_QSTR_KEY_UNCONST(key) ((mp_obj_t)((mp_int_t)(key) & ~1))
#define MP_MAP_QSTR_KEY_CONST(key) ((mp_obj_t)((mp_int_t)(key) | 1))
#define MP_MAP_QSTR_KEY_IS_CONST(key) ((mp_int_t)(key) & 1)
#define MP_MAP_GET_KEY_MASK(map) ((map)->all_keys_are_qstrs ? (mp_uint_t) ~1 : (mp_uint_t) ~0)
#define MP_MAP_KEY_APPLY_MASK(key, mask) ((mp_obj_t)((mp_uint_t)(key) & (mask)))

#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D

#define MP_MAP_QSTR_KEY_UNCONST(key) ((mp_obj_t)((uint64_t)(key) | 1))
#define MP_MAP_QSTR_KEY_CONST(key) ((mp_obj_t)((uint64_t)(key) & ~1))
#define MP_MAP_QSTR_KEY_IS_CONST(key) (((uint64_t)(key) & 1) == 0)
#define MP_MAP_GET_KEY_MASK(map) ((map)->all_keys_are_qstrs ? (mp_uint_t)1 : (mp_uint_t)0)
#define MP_MAP_KEY_APPLY_MASK(key, mask) ((mp_obj_t)((mp_uint_t)(key) | (mask)))

#else

#error Constant namespace slots are not supported by this object representation

#endif

// Abstracts away MP_MAP_QSTR_KEY_UNCONST(), in case more flag bits are
// added in the future.
#define MP_MAP_NS_KEY(key) MP_MAP_QSTR_KEY_UNCONST(key)
