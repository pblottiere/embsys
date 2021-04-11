SONAME = libhook.so
GCC = gcc

all : hook

hook:
	@echo "TODO : create a shared library with hook.c"
	$(GCC) -g -c -fPIC hook.c -o hook.o
	$(GCC) -g -shared -Wl,-soname,$(SONAME) -o $(SONAME) hook.o
# 	cp ptmx.h ../../../include
	mv $(SONAME) ../gps/lib