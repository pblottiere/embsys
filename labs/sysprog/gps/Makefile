all: init libptmx libnmea gps

init:
	mkdir -p bin lib include

gps:
	cd src/bin/gps && make

libptmx: init
	cd src/lib/ptmx && make

libnmea: init
	cd src/lib/nmea && make

clean:
	find . -name *.o | xargs rm -f
	rm -rf bin
	rm -rf lib
	rm -rf include

ok: init libptmx libnmea_ok gps

libnmea_ok:
	cd src/lib/nmea && make ok
