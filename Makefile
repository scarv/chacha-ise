# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

ifndef CCI_HOME
    $(error "Please run 'source ./bin/source.me.sh' to setup the project workspace")
endif

ifndef RISCV
    $(error "The RISCV environment variable must be set to a valid toolchain.")
endif
export LD_LIBRARY_PATH = $(CCI_HOME)/emulator

work_dir ?= $(CCI_HOME)/work

emulator_dir ?= $(CCI_HOME)/emulator
synthesis_dir?= $(CCI_HOME)/synth_yosys

# verifying the chacha ise cmodels
cmodel_test:
	$(MAKE) -C $(CCI_HOME)/src/cmodel_test all   work_dir=$(work_dir)

# evaluating the ISE variants on the RocketChip emulator.
emu_eval:	
	$(MAKE) -C $(CCI_HOME)/src/emu_eval all      work_dir=$(work_dir)/emu_eval   emulator_dir=$(emulator_dir)
	$(emulator_dir)/emulator -c -m 400000 $(work_dir)/emu_eval/test_chacha.elf

# evaluating the ISE-assisted software of the ChaCha encryption on the RocketChip emulator.
emu_ise:	
	$(MAKE) -C $(CCI_HOME)/src/emu_ise all       work_dir=$(work_dir)/emu_ise   emulator_dir=$(emulator_dir)
	$(emulator_dir)/emulator -c -m 1700000 $(work_dir)/emu_ise/emu_ise.elf

# evaluating the optimised software of the ChaCha encryption on RV64IB spike simulator.
sim_rvb:	
	$(MAKE) -C $(CCI_HOME)/src/sim_rvb all       work_dir=$(work_dir)/sim_rvb

# evaluating the optimised software and the vectorised software of the ChaCha encryption on RV64GVC spike simulator.
sim_vec:	
	$(MAKE) -C $(CCI_HOME)/src/sim_vec all       work_dir=$(work_dir)/sim_vec

synth_yosys: 
	$(MAKE) -C $(synthesis_dir) all                work_dir=$(work_dir)/synth_yosys

