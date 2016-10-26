#ifndef __MICROPY_INCLUDED_ZEPHYR_MODPYB_H__
#define __MICROPY_INCLUDED_ZEPHYR_MODPYB_H__

#include "py/obj.h"

extern const mp_obj_type_t pyb_pin_type;

MP_DECLARE_CONST_FUN_OBJ_0(pyb_info_obj);

typedef struct _pyb_pin_obj_t {
    mp_obj_base_t base;
    struct device *port;
    uint32_t pin;
} pyb_pin_obj_t;

#endif // __MICROPY_INCLUDED_ZEPHYR_MODPYB_H__
