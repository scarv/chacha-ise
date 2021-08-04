#!/bin/bash

# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

export TOOL_REPO=${CCI_HOME}/work/toolchain-rvb

export BINUTILS_REPO="${TOOL_REPO}/riscv-binutils"
export BINUTILS_BUILD="${TOOL_REPO}/riscv-binutils-build"
export BINUTILS_COMMIT="c870418800cd390bb2ae531226efd8a8ce1b741d"
export GCC_REPO="${TOOL_REPO}/riscv-gcc"
export GCC_BUILD="${TOOL_REPO}/riscv-gcc-build"
export GCC_COMMIT="7aad2f362811fb07d75eea90aaebd16fca714d4c"
export NEWLIB_REPO="${TOOL_REPO}/riscv-newlib"
export NEWLIB_BUILD="${TOOL_REPO}/riscv-newlib-build"
export NEWLIB_COMMIT="f289cef6be67da67b2d97a47d6576fa7e6b4c858"

export PK_REPO="${TOOL_REPO}/riscv-pk"
export PK_BUILD="${PK_REPO}-build"
export PK_COMMIT="0011fdf21192bd97b96c078c56cea07059c64f04"

export SPIKE_REPO="${TOOL_REPO}/riscv-isa-sim"
export SPIKE_BUILD="${SPIKE_REPO}/build"
export SPIKE_COMMIT="57023895458bc5206fe59fa229e0be6b05aa2f25"

export BRANCH="scarv/chacha-ise"

# =============================================================================
