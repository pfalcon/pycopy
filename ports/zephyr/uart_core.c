/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
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
#include <unistd.h>
#include "py/mpconfig.h"
#include "src/zephyr_getchar.h"
// Zephyr headers
#include <drivers/uart.h>
#include <console/console.h>

/*
 * Core UART functions to implement for a port
 */

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    #ifdef CONFIG_CONSOLE_SUBSYS
    return console_getchar();
    #else
    return zephyr_getchar();
    #endif
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    #ifdef CONFIG_CONSOLE_SUBSYS
    while (len--) {
        char c = *str++;
        while (console_putchar(c) == -1) {
            k_sleep(K_MSEC(1));
        }
    }
    #else
    static const struct device *uart_console_dev;
    if (uart_console_dev == NULL) {
        uart_console_dev = device_get_binding(DT_LABEL(DT_CHOSEN(zephyr_console)));
    }

    while (len--) {
        uart_poll_out(uart_console_dev, *str++);
    }
    #endif
}
