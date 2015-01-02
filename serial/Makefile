# debug
#CFLAGS= -std=c++11 -march=native -stdlib=libc++ -g # 7.8 GF/s

# opt
CFLAGS= -std=c++11 -march=native -Ofast -mllvm -force-vector-unroll=8
DUMPFLAGS= -S $(CFLAGS) -mllvm --x86-asm-syntax=intel

all: sio

sio: sio.cpp Makefile
	clang++ $(CFLAGS) $< -o $@
	clang++ $(DUMPFLAGS) $< 

clean:
	rm $(TARGETS)

