
CC     =$(RISCV)/bin/riscv64-unknown-elf-gcc
OBJDUMP=$(RISCV)/bin/riscv64-unknown-elf-objdump

RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf

hal_dir    =$(emulator_dir)/hal
ref_dir    = ../chacha_ref

INC_DIRS  =-I$(hal_dir) -I$(ref_dir)
SRCS = test.c chacha20_ise_v1.c chacha20_ise_v2.c chacha20_ise_v3.c chacha20_ise_v4.c\
       $(wildcard $(ref_dir)/*.c) \
       $(wildcard $(hal_dir)/*.c) \
       $(wildcard $(hal_dir)/*.S) \
	
OUTPUT  = $(work_dir)/test_chacha.elf
DISASM  = $(work_dir)/test_chacha.disasm

all: $(OUTPUT) $(DISASM)

CCFLAGS+=-static -nostdlib -nostartfiles -lm -lgcc -T $(hal_dir)/test.ld

$(OUTPUT) : $(SRCS)
	mkdir -p $(work_dir)
	$(CC) $(RISCV_GCC_OPTS) $(CCFLAGS) $(INC_DIRS) -o $@ $^

$(DISASM) : $(OUTPUT)
	$(OBJDUMP) -dC $< > $@

clean:
	rm -rf $(work_dir)
.PHONY: clean
