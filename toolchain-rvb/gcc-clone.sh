#!/bin/bash

# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

source ${CCI_HOME}/toolchain-rvb/share.sh

# =============================================================================

# ------ Binutils ----------------------------------------------------------
if [ ! -d ${BINUTILS_REPO} ]; then
    git clone https://github.com/riscv/riscv-binutils-gdb.git ${BINUTILS_REPO}
    cd ${BINUTILS_REPO}
    git fetch origin ${BINUTILS_COMMIT}:${BRANCH}
    git checkout ${BRANCH}
fi

# ------ GCC ---------------------------------------------------------------
if [ ! -d ${GCC_REPO} ] ; then
  git clone https://github.com/riscv/riscv-gcc.git ${GCC_REPO}
  cd ${GCC_REPO}
  git fetch origin ${GCC_COMMIT}:${BRANCH}
  git checkout ${BRANCH}
  git submodule update --init --recursive
fi

# ------ NewLib ------------------------------------------------------------
if [ ! -d ${NEWLIB_REPO} ]; then
    git clone https://github.com/riscv/riscv-newlib.git ${NEWLIB_REPO}
    cd ${NEWLIB_REPO}
    git fetch origin ${NEWLIB_COMMIT}:${BRANCH}
    git checkout ${BRANCH}
fi

# =============================================================================
