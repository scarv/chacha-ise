ifndef CCI_HOME
    $(error "Please run 'source ./bin/source.me.sh' to setup the project workspace")
endif

ifndef RISCV
    $(error "The RISCV environment variable must be set to a valid toolchain.")
endif

work_dir ?= $(CCI_HOME)/work

cmodel_dir   ?= $(CCI_HOME)/csrc/cmodel_test
synthesis_dir?= $(CCI_HOME)/synth_yosys

# verifying the chacha ise cmodels
cmodel_verify:
	$(MAKE) -C $(cmodel_dir) all                   work_dir=$(work_dir)

