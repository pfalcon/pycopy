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
    //DEBUG_printf("mp_handle_store_ns_strict(0x%p, %o, %o, %d)\n", map, attr, val, is_const);
    if (mp_strict_mode) {
        if (!map->all_keys_are_qstrs || map->is_fixed) {
            mp_raise_TypeError(NULL);
        }

        mp_map_elem_t *elem = mp_map_lookup(map, attr,
            mp_strict_runtime ? MP_MAP_LOOKUP : MP_MAP_LOOKUP_ADD_IF_NOT_FOUND);

        // If it was runtime, and no attr was found, it's error
        if (elem == NULL) {
            mp_strict_mode_error();
        }

        if (elem->value != MP_OBJ_NULL) {
            if (MP_LIKELY(mp_strict_runtime)) {
                if (MP_MAP_QSTR_KEY_IS_CONST(elem->key)) {
                    mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("strict mode: cannot override const name"));
                } else if (is_const) {
                    mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("strict mode: name already defined as variable"));
                }
                // Otherwise, overriding non-const slot with non-const value
            } else {
                bool dest_is_mod = mp_obj_is_type(elem->value, &mp_type_module);
                if (MP_MAP_QSTR_KEY_IS_CONST(elem->key)) {
                    if (is_const && elem->value == val) {
                        // Can happen during import, e.g. imports of submodules of the same package:
                        //      import pkg.foo
                        //      import pkg.bar
                        // We clearly don't want to throw warning here.
                        // !dest_is_mod case can happen with multiple "from ... import *",
                        // note that these multiples can be spread around different modules
                        // (i.e. m2 does "from m1 import *", then m3 has both "from m1 import *"
                        // and "from m2 import *", the m2 case will re-import m1's symbols again
                        // (as stored in m2 namespace)). __all__ is a good means to prevent
                        // sych re-imports, but we should work reasonably without it too.
                    } else {
                        mp_warning(MP_WARN_CAT(RuntimeWarning), "strict mode: overriding (monkey-patching) const name '%o'", attr);
                    }
                } else if (is_const) {
                    if (dest_is_mod) {
                        // Can happen during import:
                        //      from .foo import foo
                        // - will override submodule "foo" with symbol from it
                        // "foo". At first it seems that warning is warranted,
                        // we want to encourage users to choose different names
                        // for modules vs their contents in such cases. But that
                        // may adversely affect external API. E.g., in pycopy-lib
                        // we have individual "collections" submodules, which can
                        // be used like:
                        //      from collections.defaultdict import defaultdict
                        // Clearly, we can't easily rename submodule to e.g.,
                        // _defaultdict, without making API ugly. So, don't issue
                        // any warnings.
                        //mp_warning(MP_WARN_CAT(RuntimeWarning), "strict mode: overriding (sub)module");
                    } else {
                        mp_warning(MP_WARN_CAT(RuntimeWarning), "strict mode: overriding var with func");
                    }
                }
            }
        }

        if (is_const) {
            elem->key = MP_MAP_QSTR_KEY_CONST(attr);
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
