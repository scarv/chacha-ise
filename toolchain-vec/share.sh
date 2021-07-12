#!/bin/bash

# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

export TOOL_REPO=${CCI_HOME}/work/toolchain-vec

export GCC_REPO="${TOOL_REPO}/riscv-gnu-toolchain-vec"
export GCC_COMMIT="5842fde8ee5bb3371643b60ed34906eff7a5fa31"

export PK_REPO="${TOOL_REPO}/riscv-pk"
export PK_BUILD="${PK_REPO}/build"
export PK_COMMIT="2ebf3bacce4631390361a72b99b86f3b2c8dc041"

export SPIKE_REPO="${TOOL_REPO}/riscv-isa-sim"
export SPIKE_BUILD="${SPIKE_REPO}/build"
export SPIKE_COMMIT="759f4eba829d299eb34cd1568d3f4694e0d198cb"

export BRANCH="scarv/chacha-ise"

# =============================================================================
