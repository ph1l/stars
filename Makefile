CFLAGS=-Wall -std=gnu99 #-g -DDEBUG
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

all: sstars xstars nstars

sstars: starslib.o sstars.o
	gcc -o sstars starslib.o sstars.o -lSDL $(CFLAGS)

xstars: starslib.o xstars.o
	gcc -o xstars starslib.o xstars.o -lX11 $(CFLAGS)

nstars: starslib.o nstars.o
	gcc -o nstars starslib.o nstars.o -lcurses $(CFLAGS)

test-starslib: starslib.o test-starslib.o
	gcc -o test-starslib starslib.o test-starslib.o $(CFLAGS)

clean:
	rm -f *.o sstars xstars test-starslib

install:
	install -d $(DESTDIR)/$(BINDIR)
	install -m 755 nstars $(DESTDIR)/$(BINDIR)
	install -m 755 sstars $(DESTDIR)/$(BINDIR)
	install -m 755 xstars $(DESTDIR)/$(BINDIR)

uninstall:
	rm $(DESTDIR)/$(BINDIR)/nstars
	rm $(DESTDIR)/$(BINDIR)/sstars
	rm $(DESTDIR)/$(BINDIR)/xstars
