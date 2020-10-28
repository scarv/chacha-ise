CC     =$(RISCV)/bin/riscv64-unknown-elf-gcc
OBJDUMP=$(RISCV)/bin/riscv64-unknown-elf-objdump
SIM    =$(RISCV)/bin/spike
ISA    =rv64gcv

VLEN ?=128
SRCS = test.c boring.c chacha_vec_v1.s chacha_vec_v2.s \

RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf

CCFLAGS += -march=$(ISA) -static -lm -lgcc
	
OUTPUT  = $(work_dir)/sim_vec.elf
DISASM  = $(work_dir)/sim_vec.disasm

all: $(OUTPUT) $(DISASM)
	@$(SIM) --isa=$(ISA) --varch=vlen:$(VLEN),elen:64,slen:$(VLEN) pk $(OUTPUT)

$(OUTPUT) : $(SRCS)
	mkdir -p $(work_dir)
	$(CC) $(RISCV_GCC_OPTS) $(CCFLAGS) -o $@ $^

$(DISASM) : $(OUTPUT)
	$(OBJDUMP) -d -t $< > $@

clean:
	rm -rf $(work_dir)
.PHONY: clean
