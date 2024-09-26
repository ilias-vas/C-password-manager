cflags := -Wall -Werror -ansi -lm

all: pman

run: pman
	./build/pman

pman:
	cc $(cflags) -o ./build/pman $(wildcard ./src/*.c)