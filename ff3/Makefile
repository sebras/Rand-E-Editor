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
open.o: /usr/include/sys/types.h /usr/include/features.h
open.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
open.o: /usr/include/gnu/types.h /usr/include/time.h
open.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
open.o: /usr/include/endian.h /usr/include/bytesex.h
open.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
open.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
open.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
open.o: /usr/include/stdio_lim.h /usr/include/limits.h
open.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
open.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
open.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
open.o: /usr/include/asm/errno.h /usr/include/sys/stat.h
open.o: /usr/include/statbuf.h
close.o: ff.local.h ../include/c_env.h ../include/localenv.h
close.o: /usr/include/sys/types.h /usr/include/features.h
close.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
close.o: /usr/include/gnu/types.h /usr/include/time.h
close.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
close.o: /usr/include/endian.h /usr/include/bytesex.h
close.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
close.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
close.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
close.o: /usr/include/stdio_lim.h /usr/include/limits.h
close.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
close.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
close.o: /usr/include/errno.h /usr/include/errnos.h
close.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
fd.o: ff.local.h ../include/c_env.h ../include/localenv.h
fd.o: /usr/include/sys/types.h /usr/include/features.h
fd.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fd.o: /usr/include/gnu/types.h /usr/include/time.h
fd.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
fd.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
fd.o: /usr/include/selectbits.h ff.h /usr/include/stdio.h
fd.o: /usr/include/libio.h /usr/include/_G_config.h
fd.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
fd.o: /usr/include/stdio_lim.h /usr/include/limits.h
fd.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
fd.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
fd.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
fd.o: /usr/include/asm/errno.h
free.o: ff.local.h ../include/c_env.h ../include/localenv.h
free.o: /usr/include/sys/types.h /usr/include/features.h
free.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
free.o: /usr/include/gnu/types.h /usr/include/time.h
free.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
free.o: /usr/include/endian.h /usr/include/bytesex.h
free.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
free.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
free.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
free.o: /usr/include/stdio_lim.h /usr/include/limits.h
free.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
free.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
getc.o: ff.local.h ../include/c_env.h ../include/localenv.h
getc.o: /usr/include/sys/types.h /usr/include/features.h
getc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
getc.o: /usr/include/gnu/types.h /usr/include/time.h
getc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
getc.o: /usr/include/endian.h /usr/include/bytesex.h
getc.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
getc.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
getc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
getc.o: /usr/include/stdio_lim.h /usr/include/limits.h
getc.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
getc.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
getc.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
getc.o: /usr/include/asm/errno.h
grow.o: ff.local.h ../include/c_env.h ../include/localenv.h
grow.o: /usr/include/sys/types.h /usr/include/features.h
grow.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
grow.o: /usr/include/gnu/types.h /usr/include/time.h
grow.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
grow.o: /usr/include/endian.h /usr/include/bytesex.h
grow.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
grow.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
grow.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
grow.o: /usr/include/stdio_lim.h /usr/include/limits.h
grow.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
grow.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
grow.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
grow.o: /usr/include/asm/errno.h
pos.o: ff.local.h ../include/c_env.h ../include/localenv.h
pos.o: /usr/include/sys/types.h /usr/include/features.h
pos.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
pos.o: /usr/include/gnu/types.h /usr/include/time.h
pos.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
pos.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
pos.o: /usr/include/selectbits.h ff.h /usr/include/stdio.h
pos.o: /usr/include/libio.h /usr/include/_G_config.h
pos.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
pos.o: /usr/include/stdio_lim.h /usr/include/limits.h
pos.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
pos.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
pos.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
pos.o: /usr/include/asm/errno.h
putc.o: ff.local.h ../include/c_env.h ../include/localenv.h
putc.o: /usr/include/sys/types.h /usr/include/features.h
putc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
putc.o: /usr/include/gnu/types.h /usr/include/time.h
putc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
putc.o: /usr/include/endian.h /usr/include/bytesex.h
putc.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
putc.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
putc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
putc.o: /usr/include/stdio_lim.h /usr/include/limits.h
putc.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
putc.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
putc.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
putc.o: /usr/include/asm/errno.h
read.o: ff.local.h ../include/c_env.h ../include/localenv.h
read.o: /usr/include/sys/types.h /usr/include/features.h
read.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
read.o: /usr/include/gnu/types.h /usr/include/time.h
read.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
read.o: /usr/include/endian.h /usr/include/bytesex.h
read.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
read.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
read.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
read.o: /usr/include/stdio_lim.h /usr/include/limits.h
read.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
read.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
read.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
read.o: /usr/include/asm/errno.h
seek.o: ff.local.h ../include/c_env.h ../include/localenv.h
seek.o: /usr/include/sys/types.h /usr/include/features.h
seek.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
seek.o: /usr/include/gnu/types.h /usr/include/time.h
seek.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
seek.o: /usr/include/endian.h /usr/include/bytesex.h
seek.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
seek.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
seek.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
seek.o: /usr/include/stdio_lim.h /usr/include/limits.h
seek.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
seek.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
seek.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
seek.o: /usr/include/asm/errno.h
size.o: ff.local.h ../include/c_env.h ../include/localenv.h
size.o: /usr/include/sys/types.h /usr/include/features.h
size.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
size.o: /usr/include/gnu/types.h /usr/include/time.h
size.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
size.o: /usr/include/endian.h /usr/include/bytesex.h
size.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
size.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
size.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
size.o: /usr/include/stdio_lim.h /usr/include/limits.h
size.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
size.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
size.o: /usr/include/errno.h /usr/include/errnos.h /usr/include/linux/errno.h
size.o: /usr/include/asm/errno.h
sync.o: ff.local.h ../include/c_env.h ../include/localenv.h
sync.o: /usr/include/sys/types.h /usr/include/features.h
sync.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sync.o: /usr/include/gnu/types.h /usr/include/time.h
sync.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
sync.o: /usr/include/endian.h /usr/include/bytesex.h
sync.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
sync.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
sync.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
sync.o: /usr/include/stdio_lim.h /usr/include/limits.h
sync.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
sync.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
flush.o: ff.local.h ../include/c_env.h ../include/localenv.h
flush.o: /usr/include/sys/types.h /usr/include/features.h
flush.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
flush.o: /usr/include/gnu/types.h /usr/include/time.h
flush.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
flush.o: /usr/include/endian.h /usr/include/bytesex.h
flush.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
flush.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
flush.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
flush.o: /usr/include/stdio_lim.h /usr/include/limits.h
flush.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
flush.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
flush.o: /usr/include/errno.h /usr/include/errnos.h
flush.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
sort.o: ff.local.h ../include/c_env.h ../include/localenv.h
sort.o: /usr/include/sys/types.h /usr/include/features.h
sort.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sort.o: /usr/include/gnu/types.h /usr/include/time.h
sort.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
sort.o: /usr/include/endian.h /usr/include/bytesex.h
sort.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
sort.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
sort.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
sort.o: /usr/include/stdio_lim.h /usr/include/limits.h
sort.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
sort.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
write.o: ff.local.h ../include/c_env.h ../include/localenv.h
write.o: /usr/include/sys/types.h /usr/include/features.h
write.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
write.o: /usr/include/gnu/types.h /usr/include/time.h
write.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
write.o: /usr/include/endian.h /usr/include/bytesex.h
write.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
write.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
write.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
write.o: /usr/include/stdio_lim.h /usr/include/limits.h
write.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
write.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
write.o: /usr/include/errno.h /usr/include/errnos.h
write.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
point.o: ff.local.h ../include/c_env.h ../include/localenv.h
point.o: /usr/include/sys/types.h /usr/include/features.h
point.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
point.o: /usr/include/gnu/types.h /usr/include/time.h
point.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
point.o: /usr/include/endian.h /usr/include/bytesex.h
point.o: /usr/include/sys/select.h /usr/include/selectbits.h ff.h
point.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
point.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
point.o: /usr/include/stdio_lim.h /usr/include/limits.h
point.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
point.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h
move.o: ../include/c_env.h
fill.o: ../include/c_env.h
