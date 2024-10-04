cflags := -Wall -Werror -ansi -lm

source_files := $(shell find . -name '*.c')

all: pman

run: pman
	./build/pman

debug:
	cc $(cflags) -g -o ./build/pman $(source_files)
	gdb ./build/pman

pman:
	cc $(cflags) -o ./build/pman $(source_files)
