GCC = gcc

all: libptmx libnmea gps reader

init:
	mkdir -p bin lib include

gps: init
	cd src/bin/gps && make

reader: init
	cd src/bin/reader && make

libptmx: init
	cd src/lib/ptmx && make

libnmea: init
	cd src/lib/nmea && make

clean:
	find . -name *.o | xargs rm -f
	rm -rf bin
	rm -rf lib
	rm -rf include
