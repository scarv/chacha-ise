
work/synth_schacha20.v : chacha_ise.v synth.ys
	@mkdir -p work/
	verilator --lint-only chacha_ise.v
	yosys -s synth.ys

synthesise: work/synth_schacha20.v


work/reference.exe : test.c chacha20_ref.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-reference: work/reference.exe
	./work/reference.exe | tee work/reference.log

work/ise.exe : test.c chacha20_ise.c ise.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise: work/ise.exe
	./work/ise.exe | tee work/ise.log

work/ise_v2.exe : test.c chacha20_ise_v2.c ise_v2.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise_v2: work/ise_v2.exe
	./work/ise_v2.exe | tee work/ise_v2.log

work/ise_v3.exe : test.c chacha20_ise_v3.c ise_v3.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise_v3: work/ise_v3.exe
	./work/ise_v3.exe | tee work/ise_v3.log

work/ise_v4.exe : test.c chacha20_ise_v4.c ise_v4.c
	@mkdir -p work/
	$(CC) -Wall -o $@ $^

run-ise_v4: work/ise_v4.exe
	./work/ise_v4.exe | tee work/ise_v4.log
