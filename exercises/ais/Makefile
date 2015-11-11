GCC = gcc

all: libptmx ais reader

ais:
	cd src/bin/ais && make

reader:
	cd src/bin/reader && make

libptmx:
	cd src/lib/ptmx && make

clean:
	find . -name *.o | xargs rm -f
	rm -f bin/*
	rm -f lib/*
	rm -f include/*
