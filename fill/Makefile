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

center.o: /usr/include/stdio.h /usr/include/standards.h
center.o: /usr/include/va_list.h /usr/include/sys/types.h
center.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
center.o: /usr/include/sys/limits.h /usr/include/float.h /usr/include/ctype.h
center.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
center.o: /usr/include/locale.h /usr/include/sys/localedef31.h
center.o: /usr/include/sys/stat.h /usr/include/sys/mode.h
fill.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
fill.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
fill.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
fill.o: /usr/include/float.h /usr/include/ctype.h
fill.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
fill.o: /usr/include/locale.h /usr/include/sys/localedef31.h
fill.o: /usr/include/sys/stat.h /usr/include/sys/mode.h
run.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
run.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
run.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
run.o: /usr/include/float.h
