#!/usr/bin/env bash

sudo apt-get -y install gcc-arm-none-eabi

source tools/ci.sh && ci_unix_minimal_build
source tools/ci.sh && ci_unix_standard_build

export MAKEOPTS="-j$(nproc)"

make ${MAKEOPTS} -C mpy-cross
make ${MAKEOPTS} -C ports/stm32 submodules
git submodule update --init lib/btstack
make ${MAKEOPTS} -C ports/stm32 BOARD=NUCLEO_F091RC
make ${MAKEOPTS} -C ports/stm32 BOARD=PYBV11 MICROPY_PY_WIZNET5K=5200 MICROPY_PY_CC3K=1
make ${MAKEOPTS} -C ports/stm32 BOARD=PYBD_SF2
make ${MAKEOPTS} -C ports/stm32 BOARD=PYBD_SF6 NANBOX=1 MICROPY_BLUETOOTH_NIMBLE=0 MICROPY_BLUETOOTH_BTSTACK=1
make ${MAKEOPTS} -C ports/stm32 BOARD=NUCLEO_H743ZI CFLAGS_EXTRA='-DMICROPY_PY_THREAD=1'
make ${MAKEOPTS} -C ports/stm32 BOARD=STM32L476DISC
make ${MAKEOPTS} -C ports/stm32 BOARD=NUCLEO_WB55
make ${MAKEOPTS} -C ports/stm32/mboot BOARD=PYBV10 CFLAGS_EXTRA='-DMBOOT_FSLOAD=1 -DMBOOT_VFS_LFS2=1'
make ${MAKEOPTS} -C ports/stm32/mboot BOARD=PYBD_SF6
make ${MAKEOPTS} -C ports/stm32/mboot BOARD=NUCLEO_WB55

make ${MAKEOPTS} -C ports/nrf submodules
make ${MAKEOPTS} -C ports/nrf BOARD=pca10040
make ${MAKEOPTS} -C ports/nrf BOARD=microbit
make ${MAKEOPTS} -C ports/nrf BOARD=pca10056 SD=s140
make ${MAKEOPTS} -C ports/nrf BOARD=pca10090

make ${MAKEOPTS} -C ports/bare-arm
make ${MAKEOPTS} -C ports/minimal CROSS=1 build/firmware.bin
