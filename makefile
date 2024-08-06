MAKEFLAGS += --no-print-directory
all:
	@make all -C build/
test:
	make all
	@build/tests/Emulator_tst
run:
	make all
	@build/src/Emulator_run
