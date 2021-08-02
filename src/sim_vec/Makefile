# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

CC     =$(RISCV)/bin/riscv64-unknown-elf-gcc
OBJDUMP=$(RISCV)/bin/riscv64-unknown-elf-objdump
SIM    =$(RISCV)/bin/spike
ISA    =rv64gcv

VLEN ?=128
SRCS = test.c chacha_openssl.c chacha_isa.S chacha_vec_v1.S chacha_vec_v2.S \

RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf

CCFLAGS += -march=$(ISA) -lm -lgcc
	
OUTPUT  = $(work_dir)/sim_vec.elf
DISASM  = $(work_dir)/sim_vec.disasm

all: $(OUTPUT) $(DISASM)
	@$(SIM) --isa=$(ISA) --varch=vlen:$(VLEN),elen:64,slen:$(VLEN) pk $(OUTPUT)

$(OUTPUT) : $(SRCS)
	mkdir -p $(work_dir)
	$(CC) $(RISCV_GCC_OPTS) $(CCFLAGS) -DVLEN$(VLEN) -o $@ $^

$(DISASM) : $(OUTPUT)
	$(OBJDUMP) -d -t $< > $@

clean:
	rm -rf $(work_dir)
.PHONY: clean
