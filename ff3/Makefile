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
open.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
open.o: /usr/include/gnu/stubs.h
open.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
open.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
open.o: /usr/include/bits/types.h /usr/include/libio.h
open.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
open.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
open.o: /usr/include/endian.h /usr/include/bits/endian.h
open.o: /usr/include/sys/select.h /usr/include/bits/select.h
open.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
open.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
open.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
open.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
open.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
open.o: /usr/include/asm/errno.h /usr/include/sys/stat.h
open.o: /usr/include/bits/stat.h
close.o: ff.local.h ../include/c_env.h ../include/localenv.h
close.o: /usr/include/stdio.h /usr/include/features.h
close.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
close.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
close.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
close.o: /usr/include/bits/types.h /usr/include/libio.h
close.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
close.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
close.o: /usr/include/endian.h /usr/include/bits/endian.h
close.o: /usr/include/sys/select.h /usr/include/bits/select.h
close.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
close.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
close.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
close.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
close.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
close.o: /usr/include/asm/errno.h
fd.o: ff.local.h ../include/c_env.h ../include/localenv.h
fd.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
fd.o: /usr/include/gnu/stubs.h
fd.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
fd.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
fd.o: /usr/include/bits/types.h /usr/include/libio.h /usr/include/_G_config.h
fd.o: /usr/include/bits/stdio_lim.h /usr/include/string.h
fd.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
fd.o: /usr/include/bits/endian.h /usr/include/sys/select.h
fd.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fd.o: /usr/include/sys/sysmacros.h ff.h /usr/include/limits.h
fd.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
fd.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
fd.o: /usr/include/errno.h /usr/include/bits/errno.h
fd.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
free.o: ff.local.h ../include/c_env.h ../include/localenv.h
free.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
free.o: /usr/include/gnu/stubs.h
free.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
free.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
free.o: /usr/include/bits/types.h /usr/include/libio.h
free.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
free.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
free.o: /usr/include/endian.h /usr/include/bits/endian.h
free.o: /usr/include/sys/select.h /usr/include/bits/select.h
free.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
free.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
free.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
free.o: /usr/include/bits/posix2_lim.h
getc.o: ff.local.h ../include/c_env.h ../include/localenv.h
getc.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
getc.o: /usr/include/gnu/stubs.h
getc.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
getc.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
getc.o: /usr/include/bits/types.h /usr/include/libio.h
getc.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
getc.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
getc.o: /usr/include/endian.h /usr/include/bits/endian.h
getc.o: /usr/include/sys/select.h /usr/include/bits/select.h
getc.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
getc.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
getc.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
getc.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
getc.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
getc.o: /usr/include/asm/errno.h
grow.o: ff.local.h ../include/c_env.h ../include/localenv.h
grow.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
grow.o: /usr/include/gnu/stubs.h
grow.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
grow.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
grow.o: /usr/include/bits/types.h /usr/include/libio.h
grow.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
grow.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
grow.o: /usr/include/endian.h /usr/include/bits/endian.h
grow.o: /usr/include/sys/select.h /usr/include/bits/select.h
grow.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
grow.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
grow.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
grow.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
grow.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
grow.o: /usr/include/asm/errno.h
pos.o: ff.local.h ../include/c_env.h ../include/localenv.h
pos.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
pos.o: /usr/include/gnu/stubs.h
pos.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
pos.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
pos.o: /usr/include/bits/types.h /usr/include/libio.h
pos.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
pos.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
pos.o: /usr/include/endian.h /usr/include/bits/endian.h
pos.o: /usr/include/sys/select.h /usr/include/bits/select.h
pos.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
pos.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
pos.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
pos.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
pos.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pos.o: /usr/include/asm/errno.h
putc.o: ff.local.h ../include/c_env.h ../include/localenv.h
putc.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
putc.o: /usr/include/gnu/stubs.h
putc.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
putc.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
putc.o: /usr/include/bits/types.h /usr/include/libio.h
putc.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
putc.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
putc.o: /usr/include/endian.h /usr/include/bits/endian.h
putc.o: /usr/include/sys/select.h /usr/include/bits/select.h
putc.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
putc.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
putc.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
putc.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
putc.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
putc.o: /usr/include/asm/errno.h
read.o: ff.local.h ../include/c_env.h ../include/localenv.h
read.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
read.o: /usr/include/gnu/stubs.h
read.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
read.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
read.o: /usr/include/bits/types.h /usr/include/libio.h
read.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
read.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
read.o: /usr/include/endian.h /usr/include/bits/endian.h
read.o: /usr/include/sys/select.h /usr/include/bits/select.h
read.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
read.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
read.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
read.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
read.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
read.o: /usr/include/asm/errno.h
seek.o: ff.local.h ../include/c_env.h ../include/localenv.h
seek.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
seek.o: /usr/include/gnu/stubs.h
seek.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
seek.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
seek.o: /usr/include/bits/types.h /usr/include/libio.h
seek.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
seek.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
seek.o: /usr/include/endian.h /usr/include/bits/endian.h
seek.o: /usr/include/sys/select.h /usr/include/bits/select.h
seek.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
seek.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
seek.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
seek.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
seek.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
seek.o: /usr/include/asm/errno.h
size.o: ff.local.h ../include/c_env.h ../include/localenv.h
size.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
size.o: /usr/include/gnu/stubs.h
size.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
size.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
size.o: /usr/include/bits/types.h /usr/include/libio.h
size.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
size.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
size.o: /usr/include/endian.h /usr/include/bits/endian.h
size.o: /usr/include/sys/select.h /usr/include/bits/select.h
size.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
size.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
size.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
size.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
size.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
size.o: /usr/include/asm/errno.h
sync.o: ff.local.h ../include/c_env.h ../include/localenv.h
sync.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
sync.o: /usr/include/gnu/stubs.h
sync.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
sync.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
sync.o: /usr/include/bits/types.h /usr/include/libio.h
sync.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
sync.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
sync.o: /usr/include/endian.h /usr/include/bits/endian.h
sync.o: /usr/include/sys/select.h /usr/include/bits/select.h
sync.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
sync.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
sync.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
sync.o: /usr/include/bits/posix2_lim.h
flush.o: ff.local.h ../include/c_env.h ../include/localenv.h
flush.o: /usr/include/stdio.h /usr/include/features.h
flush.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
flush.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
flush.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
flush.o: /usr/include/bits/types.h /usr/include/libio.h
flush.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
flush.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
flush.o: /usr/include/endian.h /usr/include/bits/endian.h
flush.o: /usr/include/sys/select.h /usr/include/bits/select.h
flush.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
flush.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
flush.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
flush.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
flush.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
flush.o: /usr/include/asm/errno.h
sort.o: ff.local.h ../include/c_env.h ../include/localenv.h
sort.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
sort.o: /usr/include/gnu/stubs.h
sort.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
sort.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
sort.o: /usr/include/bits/types.h /usr/include/libio.h
sort.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
sort.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
sort.o: /usr/include/endian.h /usr/include/bits/endian.h
sort.o: /usr/include/sys/select.h /usr/include/bits/select.h
sort.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
sort.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
sort.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
sort.o: /usr/include/bits/posix2_lim.h
write.o: ff.local.h ../include/c_env.h ../include/localenv.h
write.o: /usr/include/stdio.h /usr/include/features.h
write.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
write.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
write.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
write.o: /usr/include/bits/types.h /usr/include/libio.h
write.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
write.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
write.o: /usr/include/endian.h /usr/include/bits/endian.h
write.o: /usr/include/sys/select.h /usr/include/bits/select.h
write.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
write.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
write.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
write.o: /usr/include/bits/posix2_lim.h /usr/include/errno.h
write.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
write.o: /usr/include/asm/errno.h
point.o: ff.local.h ../include/c_env.h ../include/localenv.h
point.o: /usr/include/stdio.h /usr/include/features.h
point.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
point.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
point.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
point.o: /usr/include/bits/types.h /usr/include/libio.h
point.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
point.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
point.o: /usr/include/endian.h /usr/include/bits/endian.h
point.o: /usr/include/sys/select.h /usr/include/bits/select.h
point.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ff.h
point.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
point.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
point.o: /usr/include/bits/posix2_lim.h
move.o: ../include/c_env.h
fill.o: ../include/c_env.h
