#
#       MAKEFILE FOR RAND "LA" LINE-ACCESS I/O PACKAGE
#
ARFLAGS = -r
# CFLAGS = -O -I. -I../include
CFLAGS = -I. -I../include -g

ALL=libtmp.a

OBJS=   getlong.o getshort.o intss.o putlong.o putshort.o \
	delay.o  empty.o   fatal.o

SRCS=   getlong.c getshort.c intss.c putlong.c putshort.c \
	delay.c  empty.c   fatal.c

$(ALL)  : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	rm -f $(OBJS) $(ALL)  ,* .e?1 .e?1.* core *.bak *.BAK *~ \#*

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

getlong.o: /usr/include/stdio.h /usr/include/features.h
getlong.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
getlong.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
getlong.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
getlong.o: /usr/include/bits/types.h /usr/include/libio.h
getlong.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
getshort.o: /usr/include/stdio.h /usr/include/features.h
getshort.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
getshort.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
getshort.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
getshort.o: /usr/include/bits/types.h /usr/include/libio.h
getshort.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
putlong.o: /usr/include/stdio.h /usr/include/features.h
putlong.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
putlong.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
putlong.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
putlong.o: /usr/include/bits/types.h /usr/include/libio.h
putlong.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
putlong.o: ../include/c_env.h
putshort.o: /usr/include/stdio.h /usr/include/features.h
putshort.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
putshort.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
putshort.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
putshort.o: /usr/include/bits/types.h /usr/include/libio.h
putshort.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
putshort.o: ../include/c_env.h
empty.o: ../include/localenv.h /usr/include/sys/ioctl.h
empty.o: /usr/include/features.h /usr/include/sys/cdefs.h
empty.o: /usr/include/gnu/stubs.h /usr/include/bits/ioctls.h
empty.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
empty.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
