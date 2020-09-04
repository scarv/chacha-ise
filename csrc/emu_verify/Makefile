
CC     =$(RISCV)/bin/riscv64-unknown-elf-gcc
OBJDUMP=$(RISCV)/bin/riscv64-unknown-elf-objdump

RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf

hal_dir    =$(emulator_dir)/hal
cmodel_dir =../cmodel_ise

INC_DIRS  =-I$(hal_dir) -I$(cmodel_dir)
SRCS = chacha_ise.c chacha_instrs.S \
       $(wildcard $(hal_dir)/*.c) \
       $(wildcard $(hal_dir)/*.S) \
       $(wildcard $(cmodel_dir)/*.c) \

OUTPUT  = $(work_dir)/emu_verify-$(CORE).elf
DISASM  = $(work_dir)/emu_verify-$(CORE).disasm

all: $(OUTHEX) $(DISASM)

CCFLAGS+=-static -nostdlib -nostartfiles -lm -lgcc -T $(hal_dir)/test.ld

$(OUTPUT) : $(SRCS)
	mkdir -p $(work_dir)
	$(CC) $(RISCV_GCC_OPTS) $(CCFLAGS) $(INC_DIRS) -o $@ $^
	
$(DISASM) : $(OUTPUT)
	$(OBJDUMP) -Dt $< > $@

clean:
	rm -rf $(work_dir)
.PHONY: clean
