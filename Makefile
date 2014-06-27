CFLAGS=-Wall -std=gnu99 #-g -DDEBUG

all: sstars

sstars: sstars.c
	gcc -o sstars sstars.c -lSDL $(CFLAGS)


clean:
	rm -f *.o sstars
