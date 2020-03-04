#
#       Copyright abandoned, 1983, The Rand Corporation
#

O       = o/
A       = -g
DEFS    = -DUNIXV7 -DPDP
LOCALINCL = ../include
INCL    = -I. -I$(LOCALINCL)
CFLAGS  = $(A) -V -O $(DEFS) $(INCL)
LIB     = libff.a
# Move and fill are required by FFIO, but they are generally useful,
# and really belong in libc
MOVE = $(O)move.o
FILL = $(O)fill.o

# ranlib is a berkeleyism. if you don't have it, don't define it.
RANLIB = ranlib

OFILES = \
	$(O)close.o \
	$(O)fd.o \
	$(O)flush.o \
	$(O)free.o \
	$(O)getc.o \
	$(O)open.o \
	$(O)alloc.o \
	$(O)pos.o \
	$(O)putc.o \
	$(O)read.o \
	$(O)seek.o \
	$(O)size.o \
	$(O)sort.o \
	$(O)use.o \
	$(O)write.o \
	$(O)blk.o \
	$(O)div.o

CFILES = \
	close.c \
	fd.c \
	flush.c \
	free.c \
	getc.c \
	open.c \
	alloc.c \
	pos.c \
	putc.c \
	read.c \
	seek.c \
	size.c \
	sort.c \
	use.c \
	write.c \
	blk.c \
	div.c

$(LIB): $(OFILES) $(FILL) $(MOVE)
	ar r $(LIB) $(OFILES) $(MOVE) $(FILL)
	$(RANLIB) $(LIB)

ranlib:
	$(RANLIB) $(LIB)

lint:
	lint $(DEFS) $(INCL) $(CFILES) -lc

$(OFILES) : ff.h $(LOCALINCL)/c_env.h /usr/include/stdio.h

ff.h : /usr/include/stdio.h

clean:
	rm -f $(OFILES) $(LIB)

$(O)close.o: close.c
	$(CC) $(CFLAGS) -c close.c
	mv close.o $(O)close.o

$(O)fd.o: fd.c
	$(CC) $(CFLAGS) -c fd.c
	mv fd.o $(O)fd.o

$(O)flush.o: flush.c
	$(CC) $(CFLAGS) -c flush.c
	mv flush.o $(O)flush.o

$(O)free.o: free.c
	$(CC) $(CFLAGS) -c free.c
	mv free.o $(O)free.o

$(O)getc.o: getc.c
	$(CC) $(CFLAGS) -c getc.c
	mv getc.o $(O)getc.o

$(O)open.o: open.c
	$(CC) $(CFLAGS) -c open.c
	mv open.o $(O)open.o

$(O)pos.o: pos.c
	$(CC) $(CFLAGS) -c pos.c
	mv pos.o $(O)pos.o

$(O)putc.o: putc.c
	$(CC) $(CFLAGS) -c putc.c
	mv putc.o $(O)putc.o

$(O)read.o: read.c
	$(CC) $(CFLAGS) -c read.c
	mv read.o $(O)read.o

$(O)seek.o: seek.c
	$(CC) $(CFLAGS) -c seek.c
	mv seek.o $(O)seek.o

$(O)size.o: size.c
	$(CC) $(CFLAGS) -c size.c
	mv size.o $(O)size.o

$(O)write.o: write.c
	$(CC) $(CFLAGS) -c write.c
	mv write.o $(O)write.o

$(O)alloc.o: alloc.c
	$(CC) $(CFLAGS) -c alloc.c
	mv alloc.o $(O)alloc.o

$(O)use.o: use.c
	$(CC) $(CFLAGS) -c use.c
	mv use.o $(O)use.o

$(O)blk.o: blk.c
	$(CC) $(CFLAGS) -c blk.c
	mv blk.o $(O)blk.o

$(O)sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
	mv sort.o $(O)sort.o

$(O)div.o: div.c
	$(CC) $(CFLAGS) -c div.c
	mv div.o $(O)div.o

$(O)move.o: move.c
	$(CC) $(CFLAGS) -c move.c
	mv move.o $(O)move.o

$(O)fill.o: fill.c
	$(CC) $(CFLAGS) -c fill.c
	mv fill.o $(O)fill.o
