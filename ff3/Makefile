#
#       MAKEFILE FOR RAND "LA" LINE-ACCESS I/O PACKAGE
#
ARFLAGS = -r
# CFLAGS = -O -I. -I../include
CFLAGS = -I. -I../include -g

ALL=libff.a

OBJS=   open.o  close.o fd.o   free.o getc.o grow.o  pos.o \
	putc.o  read.o  seek.o size.o sync.o flush.o sort.o \
	write.o point.o move.o fill.o

SRCS=   open.c  close.c fd.c   free.c getc.c grow.c  pos.c \
	putc.c  read.c  seek.c size.c sync.c flush.c sort.c \
	write.c point.c move.c fill.c


$(ALL) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	rm -f $(OBJS) $(ALL)  ,* .e?1 .e?1.* core *.bak *.BAK *~ \#*

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

open.o: ff.local.h ../include/c_env.h ../include/localenv.h
open.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
open.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
open.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
open.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
open.o: /usr/include/errno.h /usr/include/sys/stat.h /usr/include/sys/mode.h
close.o: ff.local.h ../include/c_env.h ../include/localenv.h
close.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
close.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
close.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
close.o: /usr/include/float.h /usr/include/string.h ff.h
close.o: /usr/include/limits.h /usr/include/errno.h
fd.o: ff.local.h ../include/c_env.h ../include/localenv.h
fd.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
fd.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
fd.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
fd.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
fd.o: /usr/include/errno.h
free.o: ff.local.h ../include/c_env.h ../include/localenv.h
free.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
free.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
free.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
free.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
getc.o: ff.local.h ../include/c_env.h ../include/localenv.h
getc.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
getc.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
getc.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
getc.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
getc.o: /usr/include/errno.h
grow.o: ff.local.h ../include/c_env.h ../include/localenv.h
grow.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
grow.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
grow.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
grow.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
grow.o: /usr/include/errno.h
pos.o: ff.local.h ../include/c_env.h ../include/localenv.h
pos.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
pos.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
pos.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
pos.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
pos.o: /usr/include/errno.h
putc.o: ff.local.h ../include/c_env.h ../include/localenv.h
putc.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
putc.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
putc.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
putc.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
putc.o: /usr/include/errno.h
read.o: ff.local.h ../include/c_env.h ../include/localenv.h
read.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
read.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
read.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
read.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
read.o: /usr/include/errno.h
seek.o: ff.local.h ../include/c_env.h ../include/localenv.h
seek.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
seek.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
seek.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
seek.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
seek.o: /usr/include/errno.h
size.o: ff.local.h ../include/c_env.h ../include/localenv.h
size.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
size.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
size.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
size.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
size.o: /usr/include/errno.h
sync.o: ff.local.h ../include/c_env.h ../include/localenv.h
sync.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
sync.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
sync.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
sync.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
flush.o: ff.local.h ../include/c_env.h ../include/localenv.h
flush.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
flush.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
flush.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
flush.o: /usr/include/float.h /usr/include/string.h ff.h
flush.o: /usr/include/limits.h /usr/include/errno.h
sort.o: ff.local.h ../include/c_env.h ../include/localenv.h
sort.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
sort.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
sort.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
sort.o: /usr/include/float.h /usr/include/string.h ff.h /usr/include/limits.h
write.o: ff.local.h ../include/c_env.h ../include/localenv.h
write.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
write.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
write.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
write.o: /usr/include/float.h /usr/include/string.h ff.h
write.o: /usr/include/limits.h /usr/include/errno.h
point.o: ff.local.h ../include/c_env.h ../include/localenv.h
point.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
point.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
point.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
point.o: /usr/include/float.h /usr/include/string.h ff.h
point.o: /usr/include/limits.h
move.o: ../include/c_env.h
fill.o: ../include/c_env.h
