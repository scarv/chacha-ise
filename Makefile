
synthesise_v1: chacha_ise_v1.v synth_v1.ys
	@mkdir -p work/
	yosys -s synth_v1.ys

synthesise_v2: chacha_ise_v2.v synth_v2.ys
	@mkdir -p work/
	yosys -s synth_v2.ys

synthesise_v3: chacha_ise_v3.v synth_v3.ys
	@mkdir -p work/
	yosys -s synth_v3.ys

synthesise_v4: chacha_ise_v4.v synth_v4.ys
	@mkdir -p work/
	yosys -s synth_v4.ys

work/reference.exe : test.c chacha20_ref.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-reference: work/reference.exe
	./work/reference.exe | tee work/reference.log

work/ise_v1.exe : test.c chacha20_ise_v1.c ise_v1.c ise_pack.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise_v1: work/ise_v1.exe
	./work/ise_v1.exe | tee work/ise_v1.log

work/ise_v2.exe : test.c chacha20_ise_v2.c ise_v2.c ise_pack.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise_v2: work/ise_v2.exe
	./work/ise_v2.exe | tee work/ise_v2.log

work/ise_v3.exe : test.c chacha20_ise_v3.c ise_v3.c ise_pack.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise_v3: work/ise_v3.exe
	./work/ise_v3.exe | tee work/ise_v3.log

work/ise_v4.exe : test.c chacha20_ise_v4.c ise_v4.c ise_pack.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise_v4: work/ise_v4.exe
	./work/ise_v4.exe | tee work/ise_v4.log
