GCC = gcc
BIN = gps_reader

all:
	$(GCC) ../../src/gps_reader/util.c reader.c -o $(BIN) \
		-I../../src/gps_reader -I../../../gps/include

clean:
	rm -f $(BIN)
