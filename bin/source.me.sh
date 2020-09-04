
#!/bin/sh

echo "-------------------------[Setting Up Project]--------------------------"

# Top level environment variables
export CCI_HOME=`pwd`
export YOSYS_ROOT=$CCI_HOME/external/yosys


if [ -z ${VIVADO_TOOL_DIR} ]; then
    echo "ERROR: VIVADO_TOOL_DIR not set"
else 
	source $VIVADO_TOOL_DIR/settings64.sh
fi

if [ -z ${RISCV} ]; then
    echo "[ERROR] No 'RISCV' environment variable defined"
fi	

export CORE=rocket-chachaise
export TARGET=sakura-x

echo "CCI_HOME          = $CCI_HOME"
echo "RISCV             = $RISCV"
echo "VIVADO_TOOL_DIR   = $VIVADO_TOOL_DIR"
echo "YOSYS_ROOT        = $YOSYS_ROOT"
echo
echo "CORE              = $CORE"
echo "TARGET            = $TARGET"

echo "------------------------------[Finished]-------------------------------"
