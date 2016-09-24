/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 * Copyright (c) 2016 Linaro Limited
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

#include "py/mpconfig.h"
#if MICROPY_PY_UTIME

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include <zephyr.h>

#include "py/runtime.h"
#include "py/smallint.h"
#include "py/mphal.h"

#define MP_CLOCKS_PER_SEC CLOCKS_PER_SEC
#define CLOCK_DIV (MP_CLOCKS_PER_SEC / 1000.0F)

STATIC mp_obj_t mod_time_time(void) {
    /* the absense of f.p. support is deliberate. the zephyr port uses
     * single precision floats so the fraction component will start to
     * lose precision on devices with a long uptime.
     */
    return mp_obj_new_int(sys_tick_get() / sys_clock_ticks_per_sec);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_time_time_obj, mod_time_time);

STATIC mp_obj_t mod_time_ticks_us(void) {
    mp_int_t us = sys_tick_get() * sys_clock_us_per_tick;
    return MP_OBJ_NEW_SMALL_INT(us & MP_SMALL_INT_POSITIVE_MASK);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_time_ticks_us_obj, mod_time_ticks_us);

STATIC mp_obj_t mod_time_ticks_ms(void) {
    int64_t us = sys_tick_get() * sys_clock_us_per_tick;
    mp_int_t ms = us / 1000;
    return MP_OBJ_NEW_SMALL_INT(ms & MP_SMALL_INT_POSITIVE_MASK);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_time_ticks_ms_obj, mod_time_ticks_ms);

STATIC mp_obj_t mod_time_ticks_cycles(void) {
    return MP_OBJ_NEW_SMALL_INT(sys_cycle_get_32());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_time_ticks_cycles_obj, mod_time_ticks_cycles);

STATIC mp_obj_t mod_time_cycles_to_ns(mp_obj_t arg) {
    mp_int_t cycles = mp_obj_get_int(arg);
    return mp_obj_new_int(SYS_CLOCK_HW_CYCLES_TO_NS(cycles));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_time_cycles_to_ns_obj, mod_time_cycles_to_ns);

STATIC mp_obj_t mod_time_ticks_diff(mp_obj_t oldval, mp_obj_t newval) {
    mp_uint_t old = MP_OBJ_SMALL_INT_VALUE(oldval);
    mp_uint_t new = MP_OBJ_SMALL_INT_VALUE(newval);
    return MP_OBJ_NEW_SMALL_INT((new - old) & MP_SMALL_INT_POSITIVE_MASK);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_time_ticks_diff_obj, mod_time_ticks_diff);

STATIC mp_obj_t mod_time_sleep(mp_obj_t arg) {
    MP_THREAD_GIL_EXIT();
#if MICROPY_PY_BUILTINS_FLOAT
    if (MP_OBJ_IS_INT(arg)) {
#endif
        task_sleep(SECONDS(mp_obj_get_int(arg)));
#if MICROPY_PY_BUILTINS_FLOAT
    } else {
	task_sleep(MSEC(1000 * mp_obj_get_float(arg)));
    }
#endif
    MP_THREAD_GIL_ENTER();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_time_sleep_obj, mod_time_sleep);

STATIC mp_obj_t mod_time_sleep_ms(mp_obj_t arg) {
    MP_THREAD_GIL_EXIT();
    task_sleep(MSEC(mp_obj_get_int(arg)));
    MP_THREAD_GIL_ENTER();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_time_sleep_ms_obj, mod_time_sleep_ms);

STATIC mp_obj_t mod_time_sleep_us(mp_obj_t arg) {
    MP_THREAD_GIL_EXIT();
    task_sleep(USEC(mp_obj_get_int(arg)));
    MP_THREAD_GIL_ENTER();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_time_sleep_us_obj, mod_time_sleep_us);


STATIC const mp_rom_map_elem_t mp_module_time_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },
    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&mod_time_sleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&mod_time_sleep_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&mod_time_sleep_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&mod_time_time_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_ms), MP_ROM_PTR(&mod_time_ticks_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_us), MP_ROM_PTR(&mod_time_ticks_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_cycles), MP_ROM_PTR(&mod_time_ticks_cycles_obj) },
    { MP_ROM_QSTR(MP_QSTR_cycles_to_ns), MP_ROM_PTR(&mod_time_cycles_to_ns_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_diff), MP_ROM_PTR(&mod_time_ticks_diff_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_time_globals, mp_module_time_globals_table);

const mp_obj_module_t mp_module_time = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_time_globals,
};

#endif // MICROPY_PY_UTIME
