
CC     =$(RISCV)/bin/riscv64-unknown-elf-gcc
OBJDUMP=$(RISCV)/bin/riscv64-unknown-elf-objdump

RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf

hal_dir    =$(emulator_dir)/hal

INC_DIRS  =-I$(hal_dir)
SRCS = test.c boring.c chacha_ise_v4.S \
       $(wildcard $(hal_dir)/*.c) \
       $(wildcard $(hal_dir)/*.S) \
	
OUTPUT  = $(work_dir)/emu_enc.elf
DISASM  = $(work_dir)/emu_enc.disasm

all: $(OUTPUT) $(DISASM)

CCFLAGS+=-static -nostdlib -nostartfiles -lm -lgcc -T $(hal_dir)/test.ld 

$(OUTPUT) : $(SRCS)
	mkdir -p $(work_dir)
	$(CC) $(RISCV_GCC_OPTS) $(CCFLAGS) $(INC_DIRS) -o $@ $^

$(DISASM) : $(OUTPUT)
	$(OBJDUMP) -D -t $< > $@

clean:
	rm -rf $(work_dir)
.PHONY: clean
