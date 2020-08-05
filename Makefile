
work/synth_schacha20.v : schacha20.v synth.ys
	@mkdir -p work/
	verilator --lint-only schacha20.v
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
