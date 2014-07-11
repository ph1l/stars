CFLAGS=-Wall -std=gnu99 #-g -DDEBUG

all: sstars xstars

sstars: starslib.o sstars.o
	gcc -o sstars starslib.o sstars.o -lSDL $(CFLAGS)

xstars: starslib.o xstars.o
	gcc -o xstars starslib.o xstars.o -lX11 $(CFLAGS)

test-starslib: starslib.o test-starslib.o
	gcc -o test-starslib starslib.o test-starslib.o $(CFLAGS)

clean:
	rm -f *.o sstars xstars test-starslib
