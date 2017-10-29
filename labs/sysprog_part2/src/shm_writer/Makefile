GCC = gcc
BIN = shm_writer

all:
	$(GCC) util.c handler.c decoder.c shm_writer.c -o $(BIN) \
		-I. -I../../../gps/include -lrt -lpthread

clean:
	rm -f $(BIN)
