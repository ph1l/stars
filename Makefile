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

check: starslib-test
	if [ ! -d test_results ]; then mkdir test_results; fi
	./starslib-test

starslib-test: starslib-test.o starslib.o
	gcc -o starslib-test starslib-test.o starslib.o -lcunit $(CFLAGS)

clean:
	rm -f *.o sstars xstars nstars print-starslib starslib-test
	rm -rf test_results

install:
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 nstars $(DESTDIR)$(BINDIR)
	install -m 755 sstars $(DESTDIR)$(BINDIR)
	install -m 755 xstars $(DESTDIR)$(BINDIR)

uninstall:
	rm $(DESTDIR)/$(BINDIR)/nstars
	rm $(DESTDIR)/$(BINDIR)/sstars
	rm $(DESTDIR)/$(BINDIR)/xstars
