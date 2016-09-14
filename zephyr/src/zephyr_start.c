#include <zephyr.h>
#include "zephyr_getchar.h"

int real_main(void);

void main(void) {
    if (net_init() < 0) {
        printf("Error in net_init()\n");
    }
    zephyr_getchar_init();
    real_main();
}

// This gets pulled in when mbedTLS is enabled in Zephyr config.
// It doesn't seem to be used by mbedTLS directly directly, but
// rather pulled in for some other libc dependency. All in all,
// this doesn't seem to be called, so included to avoid link
// error.
int gettimeofday(struct timeval *tv, struct timezone *tz) {
    printf("! dummy gettimeofday called\n");
}
