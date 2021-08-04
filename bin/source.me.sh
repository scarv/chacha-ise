# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

#!/bin/sh

echo "-------------------------[Setting Up Project]--------------------------"

# Top level environment variables
export CCI_HOME=`pwd`

if [ -z ${RISCV} ]; then
    echo "[ERROR] No 'RISCV' environment variable defined"
fi	

if [ -z ${RISCV_VEC} ]; then
    echo "[ERROR] No 'RISCV_VEC' environment variable defined"
fi	

export CORE=rocket-chachaise
export TARGET=sakura-x

echo "CCI_HOME          = $CCI_HOME"
echo "RISCV             = $RISCV"
echo "RISCV_VEC         = $RISCV_VEC"
echo
echo "CORE              = $CORE"
echo "TARGET            = $TARGET"

echo "------------------------------[Finished]-------------------------------"
