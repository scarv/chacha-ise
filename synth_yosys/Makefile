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

$(work_dir)/synth_v4.ys: $(CCI_HOME)/rtl/chacha_ise_v4.v synth.ys
	@mkdir -p $(work_dir)
	@echo "read_verilog" $(CCI_HOME)/rtl/chacha_ise_v4.v  | tee $(work_dir)/synth_v4.ys
	@cat synth.ys | tee -a $(work_dir)/synth_v4.ys
	yosys -s $(work_dir)/synth_v4.ys

all: $(work_dir)/synth_v1.ys $(work_dir)/synth_v2.ys $(work_dir)/synth_v3.ys $(work_dir)/synth_v4.ys
