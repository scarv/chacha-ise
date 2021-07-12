# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

work_dir ?= $(CCI_HOME)/work/synth_yosys

$(work_dir)/synth_v1.ys: $(CCI_HOME)/rtl/chacha_ise_v1.v synth.ys
	@mkdir -p $(work_dir)
	@echo "read_verilog" $(CCI_HOME)/rtl/chacha_ise_v1.v  | tee $(work_dir)/synth_v1.ys
	@cat synth.ys | tee -a $(work_dir)/synth_v1.ys
	yosys -s $(work_dir)/synth_v1.ys

$(work_dir)/synth_v2.ys: $(CCI_HOME)/rtl/chacha_ise_v2.v synth.ys
	@mkdir -p $(work_dir)
	@echo "read_verilog" $(CCI_HOME)/rtl/chacha_ise_v2.v  | tee $(work_dir)/synth_v2.ys
	@cat synth.ys | tee -a $(work_dir)/synth_v2.ys
	yosys -s $(work_dir)/synth_v2.ys

$(work_dir)/synth_v3.ys: $(CCI_HOME)/rtl/chacha_ise_v3.v synth.ys
	@mkdir -p $(work_dir)
	@echo "read_verilog" $(CCI_HOME)/rtl/chacha_ise_v3.v  | tee $(work_dir)/synth_v3.ys
	@cat synth.ys | tee -a $(work_dir)/synth_v3.ys
	yosys -s $(work_dir)/synth_v3.ys

all: $(work_dir)/synth_v1.ys $(work_dir)/synth_v2.ys $(work_dir)/synth_v3.ys 

