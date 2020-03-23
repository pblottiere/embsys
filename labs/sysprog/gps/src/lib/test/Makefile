SONAME = libtest.so
GCC = gcc

all:
	$(GCC) -g -c -fPIC test_lib.c -o test_lib.o
	$(GCC) -g -shared -Wl,-soname,$(SONAME) -o $(SONAME) test_lib.o
	cp test_lib.h ../../../include
	mv $(SONAME) ../../../lib
