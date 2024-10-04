cflags := -Wall -Werror -ansi -lm

source_files := $(shell find ./src -name '*.c' -not -name 'main.c')

.PHONY: tests
all: pman

run: pman
	./build/pman

tests:
	cc $(cflags) -g -o ./build/tests ./tests/tests.c $(source_files)
	./build/test

debug: cflags += -g
debug: pman
	gdb ./build/pman

pman:
	cc $(cflags) -o ./build/pman ./src/main.c $(source_files)
