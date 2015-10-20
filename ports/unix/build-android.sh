#!/bin/sh
# Requires standalone toolchain (as made by
# make-standalone-toolchain.sh from NDK).
# See docs/Programmers_Guide/html/md_3__key__topics__building__s_t_a_n_d_a_l_o_n_e-_t_o_o_l_c_h_a_i_n.html
# under SDK dir ("Invoking the compiler (the easy way)" subsection).

#
# Additional flags can be passed via environment vars CFLAGS_EXTRA, LDFLAGS_EXTRA
#

set -e

if [ -z "$1" ]; then
    PROG=micropython-android
else
    PROG=$1
    shift
fi

# Change for other architectures or toolchains
export CROSS_COMPILE=arm-linux-androideabi-
make deplibs BUILD=build-$PROG

# Override LDFLAGS_ARCH= to suppress map file generation, or it
# causes android-ndk-r10e gcc to fault
# Android doesn't have -lpthread, pthread functions are in libc
make BUILD=build-$PROG MICROPY_STANDALONE=1 MICROPY_PY_JNI=1 LIBPTHREAD= LDFLAGS_ARCH=-Wl,--gc-sections V=1 PROG=$PROG "$@"
make BUILD=build-$PROG MICROPY_STANDALONE=1 MICROPY_PY_JNI=1 LIBPTHREAD= LDFLAGS_ARCH=-Wl,--gc-sections V=1 lib

echo
echo "To install on device:"
echo "adb push $PROG /data/local/tmp/micropython"
