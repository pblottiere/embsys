SONAME = libptmx.so
GCC = gcc

all:
	$(GCC) -g -c -fPIC ptmx.c -o ptmx.o
	$(GCC) -g -shared -Wl,-soname,$(SONAME) -o $(SONAME) ptmx.o
	cp ptmx.h ../../../include
	mv $(SONAME) ../../../lib
