#!/bin/bash

# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

source ${CCI_HOME}/toolchain-vec/share.sh

# =============================================================================

mkdir --parents ${RISCV_VEC}

if [ -d ${PK_BUILD} ] ; then
    rm --force --recursive ${PK_BUILD}
fi

mkdir --parents ${PK_BUILD}

export PATH="${RISCV_VEC}/bin:${PATH}"

cd ${PK_BUILD}
${PK_REPO}/configure --prefix="${RISCV_VEC}" --host="riscv64-unknown-elf" --with-arch="rv64gc" --with-abi="lp64"
make clean
make
make install

# =============================================================================
