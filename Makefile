ifndef CCI_HOME
    $(error "Please run 'source ./bin/source.me.sh' to setup the project workspace")
endif

ifndef RISCV
    $(error "The RISCV environment variable must be set to a valid toolchain.")
endif
export LD_LIBRARY_PATH = $(RISCV)/lib

work_dir ?= $(CCI_HOME)/work

cmodel_dir   ?= $(CCI_HOME)/csrc/cmodel_test
emulator_dir ?= $(CCI_HOME)/emulator
synthesis_dir?= $(CCI_HOME)/synth_yosys

# verifying the chacha ise cmodels
cmodel_verify:
	$(MAKE) -C $(cmodel_dir) all                   work_dir=$(work_dir)

# verifying and evaluating the chacha ise implementations with the emulator 
emu_verify:	
	$(MAKE) -C $(CCI_HOME)/csrc/emu_verify all     work_dir=$(work_dir)/emu_verify emulator_dir=$(emulator_dir)
	$(emulator_dir)/emulator -c -m 700000 $(work_dir)/emu_verify/emu_verify-rocket-chachaise.elf

emu_eval:	
	$(MAKE) -C $(CCI_HOME)/csrc/emu_eval all       work_dir=$(work_dir)/emu_eval   emulator_dir=$(emulator_dir)
	$(emulator_dir)/emulator -c -m 400000 $(work_dir)/emu_eval/test_chacha.elf


