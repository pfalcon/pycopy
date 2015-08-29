#!/bin/sh

OPENWRT_SDK=~/toolchain/OpenWrt-SDK-atheros-for-linux-x86_64-gcc-4.8-linaro_uClibc-0.9.33.2

GCC_PATH=$OPENWRT_SDK/staging_dir/toolchain-mips_mips32_gcc-4.8-linaro_uClibc-0.9.33.2/bin
export STAGING_DIR=$OPENWRT_SDK/staging_dir/target-mips_mips32_uClibc-0.9.33.2
export PATH=$GCC_PATH:$PATH

# Don't even generate .pdr
#    CFLAGS_EXTRA="-Wa,-mno-pdr"
#-march=24kc -DNDEBUG -mips16 

make CROSS_COMPILE=mips-openwrt-linux- libffi

make CROSS_COMPILE=mips-openwrt-linux- CWARN="-Wall" \
    COPT="-Os" \
    CFLAGS_EXTRA="-DMICROPY_GCREGS_SETJMP=1 -DMICROPY_NLR_SETJMP=1 -DMICROPY_EMIT_X64=0" \
    LDFLAGS_EXTRA="-lcurses -static-libgcc" \
    STRIPFLAGS_EXTRA="--remove-section=.pdr -R .comment -R .gnu.version -R .gnu.version_r \
                      -R .gnu.attributes -R .reginfo -R .mdebug.abi32" \
    MICROPY_STANDALONE=1 \
    V=1 -B
