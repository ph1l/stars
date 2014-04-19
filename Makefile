CFLAGS=-Wall -std=gnu99 -g

all: stars

stars: stars.c stars.h
	gcc -o stars stars.c -lSDL $(CFLAGS)

clean:
	rm -f stars
