#!/bin/bash

# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

source ${CCI_HOME}/toolchain-rvb/share.sh

TARGET_ARCH=riscv64-unknown-elf
ARCH_STRING=rv64imac
ABI_STRING=lp64

# =============================================================================

if [ -d ${RISCV} ] ; then
    rm --force --recursive ${RISCV}
fi

mkdir --parents ${RISCV}

# ------ Binutils ----------------------------------------------------------
if [ -d ${BINUTILS_BUILD} ] ; then
    rm --force --recursive ${BINUTILS_BUILD}
fi
mkdir --parents ${BINUTILS_BUILD}

cd   ${BINUTILS_BUILD}
${BINUTILS_REPO}/configure \
    --prefix=${RISCV} \
    --target=$TARGET_ARCH \
    --with-arch=$ARCH_STRING --with-abi=$ABI_STRING
make -j$(nproc)
make install

# ------ GCC ---------------------------------------------------------------
if [ -d ${GCC_BUILD} ] ; then
    rm --force --recursive ${GCC_BUILD}
fi
mkdir --parents ${GCC_BUILD}

cd ${GCC_BUILD}
${GCC_REPO}/configure \
    --prefix=${RISCV} \
    --enable-languages=c \
    --disable-libssp \
    --disable-float --disable-atomic \
    --target=$TARGET_ARCH \
    --with-arch=$ARCH_STRING --with-abi=$ABI_STRING
make -j$(nproc)
make install

# ------ NewLib ------------------------------------------------------------
if [ -d ${NEWLIB_BUILD} ] ; then
    rm --force --recursive ${NEWLIB_BUILD}
fi
mkdir --parents ${NEWLIB_BUILD}

cd ${NEWLIB_BUILD}
export PATH="${RISCV}/bin:$PATH"
${NEWLIB_REPO}/configure \
    --prefix=${RISCV} \
    --target=$TARGET_ARCH \
    --with-arch=$ARCH_STRING --with-abi=$ABI_STRING
make -j$(nproc)
make install
# =============================================================================

