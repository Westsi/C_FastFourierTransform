CFILES:=$(shell find -L . -type f -name '*.c' | grep -v "./python/")
.PHONY: all clean

all: clean
	clang $(CFILES) -o fft -Iinclude/

clean:
	rm -rf ./fft

asm:
	clang -S $(CFILES) -Iinclude/