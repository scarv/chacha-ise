# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

ifndef RV64IB
    $(error "The RISCV environment variable must be set to a valid toolchain supporting Bitmanip extension.")
endif

CC     =$(RV64IB)/bin/riscv64-unknown-elf-gcc
OBJDUMP=$(RV64IB)/bin/riscv64-unknown-elf-objdump
SIM    =$(RV64IB)/bin/spike
ISA    =rv64imacb

SRCS = test.c chacha_openssl.c chacha_rv64ib.S\

RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf

CCFLAGS += -march=$(ISA) -lm -lgcc
	
OUTPUT  = $(work_dir)/sim_rvb.elf
DISASM  = $(work_dir)/sim_rvb.disasm

all: $(OUTPUT) $(DISASM)
	@$(SIM) --isa=$(ISA) pk $(OUTPUT)

$(OUTPUT) : $(SRCS)
	mkdir -p $(work_dir)
	$(CC) $(RISCV_GCC_OPTS) $(CCFLAGS) -o $@ $^

$(DISASM) : $(OUTPUT)
	$(OBJDUMP) -d -t $< > $@

clean:
	rm -rf $(work_dir)
.PHONY: clean
