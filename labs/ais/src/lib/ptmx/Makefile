SONAME = libptmx.so
GCC = gcc

all:
	$(GCC) -c -fPIC ptmx.c -o ptmx.o
	$(GCC) -shared -Wl,-soname,$(SONAME) -o $(SONAME) ptmx.o
	cp ptmx.h ../../../include
	mv $(SONAME) ../../../lib
