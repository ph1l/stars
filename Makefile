CFLAGS=-Wall -std=gnu99 #-g -DDEBUG

all: stars

stars: stars.c
	gcc -o stars stars.c -lSDL $(CFLAGS)

clean:
	rm -f stars
