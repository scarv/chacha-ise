work_dir        ?= $(CCI_HOME)/work
chacha_ref_dir  ?= $(CCI_HOME)/csrc/chacha_ref
cmodel_ise_dir  ?= $(CCI_HOME)/csrc/cmodel_ise

INC_DIRS  =-I$(chacha_ref_dir) -I$(cmodel_ise_dir)

ise_v1_src = chacha20_ise_v1.c $(cmodel_ise_dir)/ise_v1.c $(cmodel_ise_dir)/ise_pack.c
ise_v2_src = chacha20_ise_v2.c $(cmodel_ise_dir)/ise_v2.c $(cmodel_ise_dir)/ise_pack.c
ise_v3_src = chacha20_ise_v3.c $(cmodel_ise_dir)/ise_v3.c $(cmodel_ise_dir)/ise_pack.c
ise_v4_src = chacha20_ise_v4.c $(cmodel_ise_dir)/ise_v4.c $(cmodel_ise_dir)/ise_pack.c


all: run-reference run-ise_v1 run-ise_v2 run-ise_v3 run-ise_v4
$(work_dir)/cmodel_test/reference.exe : test.c $(chacha_ref_dir)/chacha20.c
	@mkdir -p $(work_dir)/cmodel_test
	$(CC) $(INC_DIRS) -Wall -o $@ $^

run-reference: $(work_dir)/cmodel_test/reference.exe
	@echo "\n\nTest the ChaCha's reference implementation"
	@$(work_dir)/cmodel_test/reference.exe | tee $(work_dir)/cmodel_test/reference.log

$(work_dir)/cmodel_test/ise_v1.exe : test.c $(ise_v1_src)
	@mkdir -p $(work_dir)/cmodel_test/
	$(CC) $(INC_DIRS) -Wall -o $@ $^

run-ise_v1: $(work_dir)/cmodel_test/ise_v1.exe
	@echo "\n\nTest the chacha_ise_v1"
	@$(work_dir)/cmodel_test/ise_v1.exe | tee $(work_dir)/cmodel_test/ise_v1.log

$(work_dir)/cmodel_test/ise_v2.exe : test.c $(ise_v2_src)
	@mkdir -p $(work_dir)/cmodel_test/
	$(CC) $(INC_DIRS) -Wall -o $@ $^

run-ise_v2: $(work_dir)/cmodel_test/ise_v2.exe
	@echo "\n\nTest the chacha_ise_v2"
	@$(work_dir)/cmodel_test/ise_v2.exe | tee $(work_dir)/cmodel_test/ise_v2.log

$(work_dir)/cmodel_test/ise_v3.exe : test.c $(ise_v3_src)
	@mkdir -p $(work_dir)/cmodel_test/
	$(CC) $(INC_DIRS) -Wall -o $@ $^

run-ise_v3: $(work_dir)/cmodel_test/ise_v3.exe
	@echo "\n\nTest the chacha_ise_v3"
	@$(work_dir)/cmodel_test/ise_v3.exe | tee $(work_dir)/cmodel_test/ise_v3.log

$(work_dir)/cmodel_test/ise_v4.exe : test.c $(ise_v4_src)
	@mkdir -p $(work_dir)/cmodel_test/
	$(CC) $(INC_DIRS) -Wall -o $@ $^

run-ise_v4: $(work_dir)/cmodel_test/ise_v4.exe
	@echo "\n\nTest the chacha_ise_v4"
	@$(work_dir)/cmodel_test/ise_v4.exe | tee $(work_dir)/cmodel_test/ise_v4.log
