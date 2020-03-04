#
#	Copyright abandoned, 1983, The Rand Corporation
#

# Rand Editor with LA package
#
#       MAKEFILE FOR RAND "LA" LINE-ACCESS I/O PACKAGE
#
CFLAGS = -O -I. -I../include

ALL=center fill run
all: $(ALL)
center: center.c
	$(CC) $(CFLAGS) -o $@ $@.c

fill: fill.c
	$(CC) $(CFLAGS) -o $@ $@.c

run: run.c
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -f $(ALL) ,* .e?1 .e?1.* core *.bak *.BAK

depend:
	makedepend -- $(CFLAGS) -- center.c fill.c run.c

# DO NOT DELETE THIS LINE -- make depend depends on it.

center.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/features.h
center.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
center.o: /usr/include/_G_config.h /usr/include/gnu/types.h
center.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
center.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
center.o: /usr/include/stdio_lim.h /usr/include/ctype.h /usr/include/endian.h
center.o: /usr/include/bytesex.h /usr/include/sys/types.h /usr/include/time.h
center.o: /usr/include/sys/select.h /usr/include/selectbits.h
center.o: /usr/include/sys/stat.h /usr/include/statbuf.h
fill.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/features.h
fill.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fill.o: /usr/include/_G_config.h /usr/include/gnu/types.h
fill.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
fill.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
fill.o: /usr/include/stdio_lim.h /usr/include/ctype.h /usr/include/endian.h
fill.o: /usr/include/bytesex.h /usr/include/sys/types.h /usr/include/time.h
fill.o: /usr/include/sys/select.h /usr/include/selectbits.h
fill.o: /usr/include/sys/stat.h /usr/include/statbuf.h
run.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/features.h
run.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
run.o: /usr/include/_G_config.h /usr/include/gnu/types.h
run.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
run.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
run.o: /usr/include/stdio_lim.h
