SONAME = libhook.so
GCC = gcc


all : hook

hook:
	@echo "create a shared library with hook.c"
	$(GCC) -g -c -fPIC hook.c
	$(GCC) -g -shared -Wl,-soname,$(SONAME) -o $(SONAME) hook.o
	mv $(SONAME) ./lib
