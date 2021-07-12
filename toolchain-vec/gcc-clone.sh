#!/bin/bash

# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

source ${CCI_HOME}/toolchain-vec/share.sh

# =============================================================================

if [ ! -d ${GCC_REPO} ] ; then
  git clone https://github.com/riscv/riscv-gnu-toolchain.git ${GCC_REPO}
fi

cd ${GCC_REPO}
git fetch origin ${GCC_COMMIT}:${BRANCH}
git checkout ${BRANCH}
git submodule update --init --recursive

# =============================================================================
