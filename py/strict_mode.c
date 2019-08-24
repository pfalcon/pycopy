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

#include <stdbool.h>

#include "py/runtime.h"
#include "py/strict_mode.h"

bool mp_strict_mode;
bool mp_strict_runtime;

NORETURN void mp_strict_mode_error(void) {
    mp_raise_msg(&mp_type_RuntimeError,
        MP_ERROR_TEXT("strict mode: can't create/del namespace attrs at runtime"));
}

bool mp_handle_store_ns_strict(mp_map_t *map, mp_obj_t attr, mp_obj_t val, bool is_const) {
    (void)is_const; // TODO
    if (mp_strict_runtime) {
        mp_map_elem_t *elem = mp_map_lookup(map, attr, MP_MAP_LOOKUP);
        if (elem == NULL) {
            mp_strict_mode_error();
        }
        elem->value = val;
        return true;
    }

    return false;
}

void mp_strict_update_main_name(void) {
    if (mp_strict_mode) {
        mp_obj_t this_name = mp_obj_dict_get(MP_OBJ_FROM_PTR(mp_globals_get()), MP_OBJ_NEW_QSTR(MP_QSTR___name__));
        if (this_name == MP_OBJ_NEW_QSTR(MP_QSTR___main__)) {
            // Ideally, __name__ should be const slot, but we first put
            // __main__ there as non-const, then potentially update here,
            // so currently it's not marked as constant.
            mp_store_global(MP_QSTR___name__, MP_OBJ_NEW_QSTR(MP_QSTR___main__s), false);
        }
    }
}
