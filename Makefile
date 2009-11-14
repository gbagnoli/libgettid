#CFLAGS+= -O3 -march=core2 -floop-interchange -floop-strip-mine -floop-block
CFLAGS= -g
LDFLAGS+= -lpthread 

all: main libgettid.o libgettid.so

main: main.o
	gcc $(CFLAGS) $(LDFLAGS) -o main main.o

libgettid.so: 
	gcc -fPIC  -shared -o libgettid.so libgettid.c -ldl

.PHONY:clean
clean:
	rm -f ./main
	rm -f ./*.o ./*.so

