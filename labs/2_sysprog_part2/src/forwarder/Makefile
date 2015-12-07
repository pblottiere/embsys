GCC = gcc
BIN = forwarder

all:
	$(GCC) opts.c handler.c forwarder.c -o $(BIN) -I. \
		-I.. -lrt -lpthread -lm -g

clean:
	rm -f $(BIN)
