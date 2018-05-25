GCC = gcc
BIN = gps
TOP = ../..

all:
	$(GCC) $(BIN).c -g -o $(BIN) -I $(TOP)/include -L $(TOP)/lib/ -lptmx -lnmea
	mv $(BIN) $(TOP)/bin
