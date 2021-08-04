#!/bin/bash

# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

source ${CCI_HOME}/toolchain-vec/share.sh

# =============================================================================

if [ -d ${RISCV_VEC} ] ; then
    rm --force --recursive ${RISCV_VEC}
fi

mkdir --parents ${RISCV_VEC}

cd ${GCC_REPO}
make clean
./configure --prefix="${RISCV_VEC}" --enable-multilib --disable-linux --with-arch=rv64gcv --with-abi=lp64d
make -j$(nproc)

# =============================================================================

