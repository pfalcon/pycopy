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
