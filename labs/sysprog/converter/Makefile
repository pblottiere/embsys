GCC = gcc
BIN = converter

all:
	$(GCC) opts.c handler.c $(BIN).c -o $(BIN) -I. -I.. -lrt -lpthread -lm

clean:
	rm -f $(BIN)
