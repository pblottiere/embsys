all: init libptmx libnmea gps main libtest

init:
	mkdir -p bin lib include

gps:
	cd src/app && make

main:
	cd src && make

libptmx: init
	cd src/lib/ptmx && make

libnmea: init
	cd src/lib/nmea && make
libtest: init
	cd src/lib/test && make

clean:
	find . -name *.o | xargs rm -f
	rm -rf bin
	rm -rf lib
	rm -rf include

ok: init libptmx libnmea_ok gps main

libnmea_ok:
	cd src/lib/nmea && make ok
