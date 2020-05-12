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

print-starslib: starslib.o print-starslib.o
	gcc -o print-starslib starslib.o print-starslib.o $(CFLAGS)

clean:
	rm -f *.o sstars xstars print-starslib

install:
	install -d $(DESTDIR)/$(BINDIR)
	install -m 755 nstars $(DESTDIR)/$(BINDIR)
	install -m 755 sstars $(DESTDIR)/$(BINDIR)
	install -m 755 xstars $(DESTDIR)/$(BINDIR)

uninstall:
	rm $(DESTDIR)/$(BINDIR)/nstars
	rm $(DESTDIR)/$(BINDIR)/sstars
	rm $(DESTDIR)/$(BINDIR)/xstars
