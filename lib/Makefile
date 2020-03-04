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

getlong.o: /usr/include/stdio.h /usr/include/standards.h
getlong.o: /usr/include/va_list.h /usr/include/sys/types.h
getlong.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
getlong.o: /usr/include/sys/limits.h /usr/include/float.h
getshort.o: /usr/include/stdio.h /usr/include/standards.h
getshort.o: /usr/include/va_list.h /usr/include/sys/types.h
getshort.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
getshort.o: /usr/include/sys/limits.h /usr/include/float.h
putlong.o: /usr/include/stdio.h /usr/include/standards.h
putlong.o: /usr/include/va_list.h /usr/include/sys/types.h
putlong.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
putlong.o: /usr/include/sys/limits.h /usr/include/float.h ../include/c_env.h
putshort.o: /usr/include/stdio.h /usr/include/standards.h
putshort.o: /usr/include/va_list.h /usr/include/sys/types.h
putshort.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
putshort.o: /usr/include/sys/limits.h /usr/include/float.h ../include/c_env.h
empty.o: ../include/localenv.h /usr/include/stdio.h /usr/include/standards.h
empty.o: /usr/include/va_list.h /usr/include/sys/types.h
empty.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
empty.o: /usr/include/sys/limits.h /usr/include/float.h /usr/include/string.h
empty.o: /usr/include/sys/ioctl.h /usr/include/sys/ttychars.h
empty.o: /usr/include/sys/ttydev.h
