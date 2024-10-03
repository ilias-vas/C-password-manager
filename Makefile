cflags := -Wall -Werror -ansi -lm

source_files := $(shell find . -name '*.c')

all: pman

run: pman
	./build/pman

pman:
	cc $(cflags) -o ./build/pman $(source_files)
