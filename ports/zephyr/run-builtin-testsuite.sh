#!/bin/sh
#
# Build and Zephyr executable with embedded testsuite. Build happens
# in a separate set of dirs.
#

set -ex

./make-bin-testsuite BOARD=mps2_an385 BUILD=build-mps2_an385-testsuite OUTDIR_PREFIX=mps2_an385-testsuite

# TODO: Not using Zephyr's "run" target because somehow -semihosting flag
# doesn't want to get passed in.
$ZEPHYR_SDK_INSTALL_DIR/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-arm \
    -cpu cortex-m3 -machine mps2-an385 \
    -nographic -vga none -net none \
    -pidfile qemu.pid -serial mon:stdio \
    -semihosting \
    -kernel outdir/mps2_an385-testsuite/zephyr/zephyr.elf
