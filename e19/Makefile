#
#	Copyright abandoned, 1983, The Rand Corporation
#

# Rand Editor with LA package
#
#       MAKEFILE FOR RAND "LA" LINE-ACCESS I/O PACKAGE
#


# -----------------------------------------------------------
# On a sun with Solaris :  (F.Perriollat Dec 1996)
# do not use -O : in e.t.c result in strang effect which give
#       a core dump on deleting char on command line !!!!
# -----------------------------------------------------------

# CFLAGS = -O -I. -I../include -I../la1 -I../ff3
CFLAGS = -I. -I../include -I../la1 -I../ff3 -g

OBJS=   e.main.o e.o    e.iit.o e.q.o   e.tb.o  e.put.o e.nm.o  e.sv.o \
	e.wi.o  e.wk.o  e.u.o   e.f.o   e.dif.o e.h.o   e.se.o  e.e.o \
	e.mk.o  e.ra.o  e.re.o  e.t.o   e.ru.o  e.cm.o  e.d.o   e.it.o \
	e.la.o  e.m.o   e.p.o   e.pa.o  e.sb.o  e.tt.o  e.x.o \
	e.keyboard_map.o

SRCS=   e.main.c e.c    e.iit.c e.q.c   e.tb.c  e.put.c e.nm.c  e.sv.c \
	e.wi.c  e.wk.c  e.u.c   e.f.c   e.dif.c e.h.c   e.se.c  e.e.c \
	e.mk.c  e.ra.c  e.re.c  e.t.c   e.ru.c  e.cm.c  e.d.c   e.it.c \
	e.la.c  e.m.c   e.p.c   e.pa.c  e.sb.c  e.tt.c  e.x.c \
	e.keyboard_map.c

LIBS=   ../la1/libla.a ../ff3/libff.a ../lib/libtmp.a

# For Ultrix, following line is needed !!
#TERMCAP = -ltermcap
TERMCAP =

ALL=a.out

$(ALL): $(OBJS) e.r.o  $(LIBS)
	$(CC) $(CFLAGS) -o $(ALL) $(OBJS) e.r.o  $(LIBS) -lcurses $(TERMCAP) 

e.r.c:
	cd ../; make e19/e.r.c

clean:
	rm -f $(OBJS) $(ALL)  e.r.c e.r.o ,* .e?1 .e?1.* core *.bak *.BAK
	cd term; rm -f ,* .e?1 .e?1.* core a.out *.bak *.BAK

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)
e.h:
	@echo Why doyou try to overwrite e.h? 

# DO NOT DELETE THIS LINE -- make depend depends on it.

e.main.o: e.h ../include/c_env.h ../include/localenv.h
e.main.o: /usr/include/sys/types.h /usr/include/features.h
e.main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.main.o: /usr/include/gnu/types.h /usr/include/time.h
e.main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.main.o: /usr/include/endian.h /usr/include/bytesex.h
e.main.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.main.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.main.o: /usr/include/_G_config.h
e.main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.main.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.main.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.main.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h
e.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.o: /usr/include/libio.h /usr/include/_G_config.h
e.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.o: /usr/include/stdio_lim.h /usr/include/limits.h /usr/include/posix1_lim.h
e.o: /usr/include/local_lim.h /usr/include/linux/limits.h
e.o: /usr/include/posix2_lim.h e.t.h e.e.h e.it.h e.tt.h e.wi.h e.fn.h e.sg.h
e.o: /usr/include/sys/ioctl.h /usr/include/ioctls.h /usr/include/asm/ioctls.h
e.o: /usr/include/asm/ioctl.h /usr/include/ioctl-types.h
e.o: /usr/include/sys/ttydefaults.h /usr/include/termio.h
e.o: /usr/include/termios.h /usr/include/termbits.h e.inf.h
e.o: /usr/include/sys/stat.h /usr/include/statbuf.h /usr/include/string.h
e.o: /usr/include/fcntl.h /usr/include/fcntlbits.h /usr/include/unistd.h
e.o: /usr/include/posix_opt.h /usr/include/confname.h /usr/include/signal.h
e.o: /usr/include/sigset.h /usr/include/signum.h /usr/include/sigaction.h
e.o: /usr/include/sigcontext.h /usr/include/asm/sigcontext.h
e.iit.o: e.h ../include/c_env.h ../include/localenv.h
e.iit.o: /usr/include/sys/types.h /usr/include/features.h
e.iit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.iit.o: /usr/include/gnu/types.h /usr/include/time.h
e.iit.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.iit.o: /usr/include/endian.h /usr/include/bytesex.h
e.iit.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.iit.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.iit.o: /usr/include/_G_config.h
e.iit.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.iit.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.iit.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.iit.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.it.h
e.q.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.q.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.q.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.q.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.q.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.q.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.q.o: /usr/include/libio.h /usr/include/_G_config.h
e.q.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.q.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.q.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.q.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.q.o: e.cm.h e.fn.h e.tt.h /usr/include/signal.h /usr/include/sigset.h
e.q.o: /usr/include/signum.h /usr/include/sigaction.h
e.q.o: /usr/include/sigcontext.h /usr/include/asm/sigcontext.h
e.tb.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.tb.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.tb.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.tb.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.tb.o: /usr/include/endian.h /usr/include/bytesex.h
e.tb.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.tb.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.tb.o: /usr/include/_G_config.h
e.tb.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.tb.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.tb.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.tb.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.tb.o: e.cm.h
e.put.o: e.h ../include/c_env.h ../include/localenv.h
e.put.o: /usr/include/sys/types.h /usr/include/features.h
e.put.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.put.o: /usr/include/gnu/types.h /usr/include/time.h
e.put.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.put.o: /usr/include/endian.h /usr/include/bytesex.h
e.put.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.put.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.put.o: /usr/include/_G_config.h
e.put.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.put.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.put.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.put.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.put.o: e.cm.h e.e.h
e.nm.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.nm.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.nm.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.nm.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.nm.o: /usr/include/endian.h /usr/include/bytesex.h
e.nm.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.nm.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.nm.o: /usr/include/_G_config.h
e.nm.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.nm.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.nm.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.nm.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.inf.h
e.nm.o: e.m.h e.cm.h /usr/include/sys/stat.h /usr/include/statbuf.h
e.nm.o: /usr/include/unistd.h /usr/include/posix_opt.h
e.nm.o: /usr/include/confname.h
e.sv.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.sv.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.sv.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.sv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.sv.o: /usr/include/endian.h /usr/include/bytesex.h
e.sv.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.sv.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.sv.o: /usr/include/_G_config.h
e.sv.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.sv.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.sv.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.sv.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.sv.o: e.cm.h /usr/include/sys/stat.h /usr/include/statbuf.h
e.sv.o: /usr/include/unistd.h /usr/include/posix_opt.h
e.sv.o: /usr/include/confname.h
e.wi.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.wi.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.wi.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.wi.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.wi.o: /usr/include/endian.h /usr/include/bytesex.h
e.wi.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.wi.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.wi.o: /usr/include/_G_config.h
e.wi.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.wi.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.wi.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.wi.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.cm.h
e.wi.o: e.inf.h e.tt.h e.wi.h
e.wk.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.wk.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.wk.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.wk.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.wk.o: /usr/include/endian.h /usr/include/bytesex.h
e.wk.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.wk.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.wk.o: /usr/include/_G_config.h
e.wk.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.wk.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.wk.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.wk.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.inf.h
e.u.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.u.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.u.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.u.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.u.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.u.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.u.o: /usr/include/libio.h /usr/include/_G_config.h
e.u.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.u.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.u.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.u.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.u.o: e.cm.h /usr/include/sys/stat.h /usr/include/statbuf.h
e.u.o: /usr/include/pwd.h /usr/include/unistd.h /usr/include/posix_opt.h
e.u.o: /usr/include/confname.h
e.f.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.f.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.f.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.f.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.f.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.f.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.f.o: /usr/include/libio.h /usr/include/_G_config.h
e.f.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.f.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.f.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.f.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h
e.f.o: /usr/include/sys/stat.h /usr/include/statbuf.h /usr/include/string.h
e.h.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.h.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.h.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.h.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.h.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.h.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.h.o: /usr/include/libio.h /usr/include/_G_config.h
e.h.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.h.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.h.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.h.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.cm.h
e.h.o: e.m.h e.tt.h e.h.h /usr/include/string.h /usr/include/sys/time.h
e.h.o: /usr/include/timebits.h
e.se.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.se.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.se.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.se.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.se.o: /usr/include/endian.h /usr/include/bytesex.h
e.se.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.se.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.se.o: /usr/include/_G_config.h
e.se.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.se.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.se.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.se.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.se.o: e.cm.h e.se.h
e.e.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.e.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.e.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.e.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.e.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.e.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.e.o: /usr/include/libio.h /usr/include/_G_config.h
e.e.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.e.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.e.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.e.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.e.o: e.cm.h e.e.h
e.mk.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.mk.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.mk.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.mk.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.mk.o: /usr/include/endian.h /usr/include/bytesex.h
e.mk.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.mk.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.mk.o: /usr/include/_G_config.h
e.mk.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.mk.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.mk.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.mk.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.inf.h
e.mk.o: e.m.h
e.ra.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.ra.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.ra.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.ra.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.ra.o: /usr/include/endian.h /usr/include/bytesex.h
e.ra.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.ra.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.ra.o: /usr/include/_G_config.h
e.ra.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.ra.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.ra.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.ra.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.ra.o: e.cm.h
e.re.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.re.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.re.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.re.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.re.o: /usr/include/endian.h /usr/include/bytesex.h
e.re.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.re.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.re.o: /usr/include/_G_config.h
e.re.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.re.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.re.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.re.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.re.o: e.cm.h e.se.h
e.t.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.t.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.t.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.t.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.t.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.t.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.t.o: /usr/include/libio.h /usr/include/_G_config.h
e.t.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.t.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.t.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.t.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.inf.h
e.t.o: e.m.h e.tt.h e.wi.h /usr/include/sys/time.h /usr/include/timebits.h
e.ru.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.ru.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.ru.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.ru.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.ru.o: /usr/include/endian.h /usr/include/bytesex.h
e.ru.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.ru.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.ru.o: /usr/include/_G_config.h
e.ru.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.ru.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.ru.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.ru.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.e.h
e.ru.o: e.m.h e.cm.h e.ru.h /usr/include/signal.h /usr/include/sigset.h
e.ru.o: /usr/include/signum.h /usr/include/sigaction.h
e.ru.o: /usr/include/sigcontext.h /usr/include/asm/sigcontext.h
e.cm.o: /usr/include/unistd.h /usr/include/features.h
e.cm.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.cm.o: /usr/include/posix_opt.h /usr/include/gnu/types.h
e.cm.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.cm.o: /usr/include/confname.h e.h ../include/c_env.h ../include/localenv.h
e.cm.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
e.cm.o: /usr/include/bytesex.h /usr/include/sys/select.h
e.cm.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h
e.cm.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
e.cm.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.cm.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.cm.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.cm.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.e.h
e.cm.o: e.m.h e.ru.h e.cm.h e.wi.h e.tt.h /usr/include/signal.h
e.cm.o: /usr/include/sigset.h /usr/include/signum.h /usr/include/sigaction.h
e.cm.o: /usr/include/sigcontext.h /usr/include/asm/sigcontext.h
e.d.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.d.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.d.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.d.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.d.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.d.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.d.o: /usr/include/libio.h /usr/include/_G_config.h
e.d.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.d.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.d.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.d.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.tt.h
e.d.o: e.it.h /usr/include/signal.h /usr/include/sigset.h
e.d.o: /usr/include/signum.h /usr/include/sigaction.h
e.d.o: /usr/include/sigcontext.h /usr/include/asm/sigcontext.h
e.it.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.it.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.it.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.it.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.it.o: /usr/include/endian.h /usr/include/bytesex.h
e.it.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.it.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.it.o: /usr/include/_G_config.h
e.it.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.it.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.it.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.it.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.it.h
e.la.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.la.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.la.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.la.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.la.o: /usr/include/endian.h /usr/include/bytesex.h
e.la.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.la.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.la.o: /usr/include/_G_config.h
e.la.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.la.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.la.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.la.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.la.o: /usr/include/signal.h /usr/include/sigset.h /usr/include/signum.h
e.la.o: /usr/include/sigaction.h /usr/include/sigcontext.h
e.la.o: /usr/include/asm/sigcontext.h
e.m.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.m.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.m.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.m.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.m.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.m.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.m.o: /usr/include/libio.h /usr/include/_G_config.h
e.m.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.m.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.m.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.m.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.cm.h
e.m.o: e.inf.h e.m.h e.tt.h e.e.h /usr/include/sys/time.h
e.m.o: /usr/include/timebits.h
e.p.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.p.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.p.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.p.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.p.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.p.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.p.o: /usr/include/libio.h /usr/include/_G_config.h
e.p.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.p.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.p.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.p.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.cm.h
e.p.o: e.inf.h e.m.h /usr/include/ctype.h
e.pa.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.pa.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.pa.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.pa.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.pa.o: /usr/include/endian.h /usr/include/bytesex.h
e.pa.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.pa.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.pa.o: /usr/include/_G_config.h
e.pa.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.pa.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.pa.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.pa.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.m.h
e.pa.o: e.cm.h
e.sb.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.sb.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.sb.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.sb.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.sb.o: /usr/include/endian.h /usr/include/bytesex.h
e.sb.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.sb.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.sb.o: /usr/include/_G_config.h
e.sb.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.sb.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.sb.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.sb.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.fn.h
e.sb.o: e.sg.h /usr/include/sys/ioctl.h /usr/include/ioctls.h
e.sb.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
e.sb.o: /usr/include/ioctl-types.h /usr/include/sys/ttydefaults.h
e.sb.o: /usr/include/termio.h /usr/include/termios.h /usr/include/termbits.h
e.sb.o: /usr/include/signal.h /usr/include/sigset.h /usr/include/signum.h
e.sb.o: /usr/include/sigaction.h /usr/include/sigcontext.h
e.sb.o: /usr/include/asm/sigcontext.h /usr/include/sys/stat.h
e.sb.o: /usr/include/statbuf.h /usr/include/fcntl.h /usr/include/fcntlbits.h
e.sb.o: /usr/include/pwd.h
e.tt.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.tt.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.tt.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.tt.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.tt.o: /usr/include/endian.h /usr/include/bytesex.h
e.tt.o: /usr/include/sys/select.h /usr/include/selectbits.h ../la1/la.h
e.tt.o: ../ff3/ff.h /usr/include/stdio.h /usr/include/libio.h
e.tt.o: /usr/include/_G_config.h
e.tt.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.tt.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.tt.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.tt.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.tt.h
e.tt.o: e.sg.h /usr/include/sys/ioctl.h /usr/include/ioctls.h
e.tt.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
e.tt.o: /usr/include/ioctl-types.h /usr/include/sys/ttydefaults.h
e.tt.o: /usr/include/termio.h /usr/include/termios.h /usr/include/termbits.h
e.tt.o: term/standard.c term/tcap.c term/vt100.c
e.x.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/sys/types.h
e.x.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.x.o: /usr/include/gnu/stubs.h /usr/include/gnu/types.h /usr/include/time.h
e.x.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.x.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/sys/select.h
e.x.o: /usr/include/selectbits.h ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.x.o: /usr/include/libio.h /usr/include/_G_config.h
e.x.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.x.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.x.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.x.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h e.e.h
e.x.o: e.inf.h e.m.h e.sg.h /usr/include/sys/ioctl.h /usr/include/ioctls.h
e.x.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
e.x.o: /usr/include/ioctl-types.h /usr/include/sys/ttydefaults.h
e.x.o: /usr/include/termio.h /usr/include/termios.h /usr/include/termbits.h
e.x.o: /usr/include/signal.h /usr/include/sigset.h /usr/include/signum.h
e.x.o: /usr/include/sigaction.h /usr/include/sigcontext.h
e.x.o: /usr/include/asm/sigcontext.h
e.keyboard_map.o: e.h ../include/c_env.h ../include/localenv.h
e.keyboard_map.o: /usr/include/sys/types.h /usr/include/features.h
e.keyboard_map.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.keyboard_map.o: /usr/include/gnu/types.h /usr/include/time.h
e.keyboard_map.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
e.keyboard_map.o: /usr/include/endian.h /usr/include/bytesex.h
e.keyboard_map.o: /usr/include/sys/select.h /usr/include/selectbits.h
e.keyboard_map.o: ../la1/la.h ../ff3/ff.h /usr/include/stdio.h
e.keyboard_map.o: /usr/include/libio.h /usr/include/_G_config.h
e.keyboard_map.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
e.keyboard_map.o: /usr/include/stdio_lim.h /usr/include/limits.h
e.keyboard_map.o: /usr/include/posix1_lim.h /usr/include/local_lim.h
e.keyboard_map.o: /usr/include/linux/limits.h /usr/include/posix2_lim.h e.t.h
e.keyboard_map.o: e.it.h e.tt.h /usr/include/stdlib.h /usr/include/alloca.h
e.keyboard_map.o: /usr/include/termios.h /usr/include/termbits.h
e.keyboard_map.o: /usr/include/sys/ttydefaults.h /usr/include/string.h
e.keyboard_map.o: /usr/include/linux/kd.h /usr/include/linux/types.h
e.keyboard_map.o: /usr/include/linux/posix_types.h
e.keyboard_map.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
e.keyboard_map.o: /usr/include/linux/keyboard.h /usr/include/X11/keysym.h
e.keyboard_map.o: /usr/include/X11/keysymdef.h
