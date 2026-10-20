CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: tictactoe

tictactoe: tictactoe.c
	$(CC) $(CFLAGS) tictactoe.c -o tictactoe

clean:
	rm -f tictactoe tictactoe.js tictactoe.wasm
