GCC = gcc
BIN = shm_reader

all:
	$(GCC) opts.c handler.c $(BIN).c -o $(BIN) -I. -I.. -lrt -lpthread

clean:
	rm -f $(BIN)
