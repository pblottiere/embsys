# TP 1: System programming: part 1

* Question 1: The makefile is used to specify the steps used to build a project, a make system, with the correct order and linkings between libraries, sources, etc. Make is a build tool used for the aforementioned steps, building executables. 

* Question 2: The gcc compiler is used here.

* Question 3: A shared library is a library that may be used by multiple executables at the same time. When linked, they are not copied into the executable, but references to the functions and their locations are specified instead.

* Question 4: An example of an executable binary is the gps generated in *bin/gps*, which could be linked with something such as *gcc gps.c -o gps*

* Question 5: An example of shared library is the one generated in *lib/libnmea.so*. Its creation requires some more steps, being first that the object file has to be generated with position independent code, so that the memory addresses are not overlapped by other code being used simultaneously, and can be done for example with *gcc -c -fpic nmea.c -o nmea.o*. Later, the objects have to be linked in order to create the shared library, which also has to be specified, such as with *gcc -shared libnmea.so nmea.so* (in this case only one object is linked).
