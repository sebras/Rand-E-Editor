#
#       Copyright abandoned, 1983, The Rand Corporation
#

CFLAGS = -O -V

all: fill just center

fill: fill.o
	$(CC) fill.o
	-mv fill fill.old
	mv a.out fill

just: fill
	-rm -f just
	ln fill just

center: center.o
	$(CC) center.o
	-mv center center.old
	mv a.out center

install:
	cp fill /etc/e
	-rm -f /etc/e/just
	ln /etc/e/fill /etc/e/just
	cp center /etc/e

clean:
	rm -f fill just center *.o
