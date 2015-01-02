# debug
#CFLAGS= -std=c++11 -march=native -stdlib=libc++ -g # 7.8 GF/s

# opt
CFLAGS= -std=c++11 -Ofast
DUMPFLAGS= -S $(CFLAGS)

all: sio

sio: sio.cpp Makefile
	clang++ $(CFLAGS) $< -o $@
	clang++ $(DUMPFLAGS) $< 

clean:
	rm $(TARGETS)

