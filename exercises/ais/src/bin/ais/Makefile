GCC = gcc
BIN = ais
TOP = ../../..

all:
	$(GCC) ais.c -o $(BIN) -I $(TOP)/include -L $(TOP)/lib/ -lptmx -lnmea
	mv $(BIN) $(TOP)/bin
