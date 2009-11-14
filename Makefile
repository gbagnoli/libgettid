#CFLAGS+= -O3 -march=core2 -floop-interchange -floop-strip-mine -floop-block
CFLAGS= -g -I.
LDFLAGS+= -lpthread 

all: libgettid.so 

test: libgettid.so test.o
	gcc $(CFLAGS) -L. -l gettid $(LDFLAGS) -o test test.o

libgettid.so: 
	gcc -I. -fPIC  -shared -o libgettid.so libgettid.c -ldl

.PHONY:clean
clean:
	rm -f ./test
	rm -f ./*.o ./*.so

