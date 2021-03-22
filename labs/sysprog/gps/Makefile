all: init libptmx libnmea libhook gps

init:
	mkdir -p bin lib include

gps:
	cd src/app && make

libptmx: init
	cd src/lib/ptmx && make

libnmea: init
	cd src/lib/nmea && make

libhook: init
	cd src/lib/hook && make

clean:
	find . -name *.o | xargs rm -f
	rm -rf bin
	rm -rf lib
	rm -rf include

ok: init libptmx libnmea_ok libhook gps

libnmea_ok:
	cd src/lib/nmea && make ok
