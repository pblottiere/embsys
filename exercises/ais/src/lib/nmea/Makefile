SONAME = libnmea.so
GCC = gcc

all:
	$(GCC) -c -fPIC nmea.c -o nmea.o
	$(GCC) -shared -Wl,-soname,$(SONAME) -o $(SONAME) nmea.o -lm
	cp nmea.h ../../../include
	mv $(SONAME) ../../../lib
