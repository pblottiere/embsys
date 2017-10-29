GCC = gcc
BIN = shm_reader

all:
	$(GCC) opts.c handler.c $(BIN).c -o $(BIN) -I. -I.. -lpthread

clean:
	rm -f $(BIN)
