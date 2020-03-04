#
#       Copyright abandoned, 1983, The Rand Corporation
#

# SEE empty below -- set for Fourth Berkeley VMUNIX

LIB     = libtmp.a
MALLOC  =
CFLAGS  = -O -V -DUNIXV7 -I../include

# ranlib is a late v7ism. if you don't have it, define it to echo
RANLIB = ranlib

OFILES  =\
	tfopen.o \
	getlong.o \
	getshort.o \
	intss.o \
	putlong.o \
	putshort.o \
	delay.o \
	empty.o \
	move.o \
	fill.o \
	$(MALLOC)

$(LIB): $(OFILES)
	-mv $(LIB) ,$(LIB)
	ar r $(LIB) $(OFILES)
	$(RANLIB) $(LIB)

# Malloc is here for testing purposes only.  If it is compiled with 'debug'
# defined, it runs slower but does rigorous checking to insure that the
# heap has not been corrupted.
# It is also modified to call the E 'fatal' routine if an ASSERT fails.
# Has 'veralloc()' to verify the arena
malloc.o: malloc.c
	$(CC) -c -Ddebug -O -g malloc.c
#       $(CC) -c -Ddebug -Dlongdebug -O -g malloc.c

clean:
	rm -f $(OFILES) $(LIB)
