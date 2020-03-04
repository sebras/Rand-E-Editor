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
	e.keyboard_map.o e.resize.o e.who.o

SRCS=   e.main.c e.c    e.iit.c e.q.c   e.tb.c  e.put.c e.nm.c  e.sv.c \
	e.wi.c  e.wk.c  e.u.c   e.f.c   e.dif.c e.h.c   e.se.c  e.e.c \
	e.mk.c  e.ra.c  e.re.c  e.t.c   e.ru.c  e.cm.c  e.d.c   e.it.c \
	e.la.c  e.m.c   e.p.c   e.pa.c  e.sb.c  e.tt.c  e.x.c \
	e.keyboard_map.c e.resize.c e.who.c

LIBS=   ../la1/libla.a ../ff3/libff.a ../lib/libtmp.a
LIBX=   -L/usr/X11R6/lib -lXt -lX11 -lxkbfile

# For Ultrix, following line is needed !!
#TERMCAP = -ltermcap
TERMCAP =

ALL=a.out

$(ALL): $(OBJS) e.r.o  $(LIBS)
	$(CC) $(CFLAGS) -o $(ALL) $(OBJS) e.r.o  $(LIBS) $(LIBX) -lcurses $(TERMCAP)

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

e.main.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.main.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.main.o: /usr/include/gnu/stubs.h
e.main.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.main.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.main.o: /usr/include/bits/types.h /usr/include/libio.h
e.main.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.main.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.main.o: /usr/include/endian.h /usr/include/bits/endian.h
e.main.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.main.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.main.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.main.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.main.o: /usr/include/bits/posix2_lim.h e.t.h
e.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.o: /usr/include/gnu/stubs.h
e.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.o: /usr/include/bits/types.h /usr/include/libio.h /usr/include/_G_config.h
e.o: /usr/include/bits/stdio_lim.h /usr/include/string.h
e.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
e.o: /usr/include/bits/endian.h /usr/include/sys/select.h
e.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
e.o: /usr/include/sys/sysmacros.h ../la1/la.h ../ff3/ff.h
e.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.o: /usr/include/bits/posix2_lim.h e.t.h e.e.h e.it.h e.tt.h e.wi.h e.fn.h
e.o: e.sg.h /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
e.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
e.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
e.o: /usr/include/termio.h /usr/include/termios.h /usr/include/bits/termios.h
e.o: e.inf.h /usr/include/sys/stat.h /usr/include/bits/stat.h
e.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h /usr/include/unistd.h
e.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
e.o: /usr/include/getopt.h /usr/include/signal.h /usr/include/bits/signum.h
e.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
e.o: /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h
e.o: /usr/include/bits/sigstack.h
e.iit.o: /usr/include/string.h /usr/include/features.h
e.iit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.iit.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h e.h
e.iit.o: ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.iit.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.iit.o: /usr/include/bits/types.h /usr/include/libio.h
e.iit.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.iit.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
e.iit.o: /usr/include/bits/endian.h /usr/include/sys/select.h
e.iit.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
e.iit.o: /usr/include/sys/sysmacros.h ../la1/la.h ../ff3/ff.h
e.iit.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.iit.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.iit.o: /usr/include/bits/posix2_lim.h e.t.h e.it.h
e.q.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.q.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.q.o: /usr/include/gnu/stubs.h
e.q.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.q.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.q.o: /usr/include/bits/types.h /usr/include/libio.h
e.q.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.q.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.q.o: /usr/include/endian.h /usr/include/bits/endian.h
e.q.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.q.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.q.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.q.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.q.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h e.fn.h e.tt.h
e.q.o: /usr/include/signal.h /usr/include/bits/signum.h
e.q.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
e.q.o: /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h
e.q.o: /usr/include/bits/sigstack.h
e.tb.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.tb.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.tb.o: /usr/include/gnu/stubs.h
e.tb.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.tb.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.tb.o: /usr/include/bits/types.h /usr/include/libio.h
e.tb.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.tb.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.tb.o: /usr/include/endian.h /usr/include/bits/endian.h
e.tb.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.tb.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.tb.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.tb.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.tb.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h
e.put.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.put.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.put.o: /usr/include/gnu/stubs.h
e.put.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.put.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.put.o: /usr/include/bits/types.h /usr/include/libio.h
e.put.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.put.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.put.o: /usr/include/endian.h /usr/include/bits/endian.h
e.put.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.put.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.put.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.put.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.put.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h e.e.h
e.nm.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.nm.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.nm.o: /usr/include/gnu/stubs.h
e.nm.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.nm.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.nm.o: /usr/include/bits/types.h /usr/include/libio.h
e.nm.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.nm.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.nm.o: /usr/include/endian.h /usr/include/bits/endian.h
e.nm.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.nm.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.nm.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.nm.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.nm.o: /usr/include/bits/posix2_lim.h e.t.h e.inf.h e.m.h e.cm.h
e.nm.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
e.nm.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
e.nm.o: /usr/include/bits/confname.h /usr/include/getopt.h
e.sv.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.sv.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.sv.o: /usr/include/gnu/stubs.h
e.sv.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.sv.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.sv.o: /usr/include/bits/types.h /usr/include/libio.h
e.sv.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.sv.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.sv.o: /usr/include/endian.h /usr/include/bits/endian.h
e.sv.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.sv.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.sv.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.sv.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.sv.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h
e.sv.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
e.sv.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
e.sv.o: /usr/include/bits/confname.h /usr/include/getopt.h
e.wi.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.wi.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.wi.o: /usr/include/gnu/stubs.h
e.wi.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.wi.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.wi.o: /usr/include/bits/types.h /usr/include/libio.h
e.wi.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.wi.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.wi.o: /usr/include/endian.h /usr/include/bits/endian.h
e.wi.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.wi.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.wi.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.wi.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.wi.o: /usr/include/bits/posix2_lim.h e.t.h e.cm.h e.inf.h e.tt.h e.wi.h
e.wk.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.wk.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.wk.o: /usr/include/gnu/stubs.h
e.wk.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.wk.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.wk.o: /usr/include/bits/types.h /usr/include/libio.h
e.wk.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.wk.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.wk.o: /usr/include/endian.h /usr/include/bits/endian.h
e.wk.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.wk.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.wk.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.wk.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.wk.o: /usr/include/bits/posix2_lim.h e.t.h e.inf.h
e.u.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.u.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.u.o: /usr/include/gnu/stubs.h
e.u.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.u.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.u.o: /usr/include/bits/types.h /usr/include/libio.h
e.u.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.u.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.u.o: /usr/include/endian.h /usr/include/bits/endian.h
e.u.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.u.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.u.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.u.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.u.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h
e.u.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/pwd.h
e.u.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
e.u.o: /usr/include/bits/confname.h /usr/include/getopt.h
e.f.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.f.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.f.o: /usr/include/gnu/stubs.h
e.f.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.f.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.f.o: /usr/include/bits/types.h /usr/include/libio.h
e.f.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.f.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.f.o: /usr/include/endian.h /usr/include/bits/endian.h
e.f.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.f.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.f.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.f.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.f.o: /usr/include/bits/posix2_lim.h e.t.h /usr/include/sys/stat.h
e.f.o: /usr/include/bits/stat.h
e.h.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.h.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.h.o: /usr/include/gnu/stubs.h
e.h.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.h.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.h.o: /usr/include/bits/types.h /usr/include/libio.h
e.h.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.h.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.h.o: /usr/include/endian.h /usr/include/bits/endian.h
e.h.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.h.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.h.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.h.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.h.o: /usr/include/bits/posix2_lim.h e.t.h e.cm.h e.m.h e.tt.h e.h.h
e.h.o: /usr/include/sys/time.h /usr/include/bits/time.h
e.se.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.se.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.se.o: /usr/include/gnu/stubs.h
e.se.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.se.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.se.o: /usr/include/bits/types.h /usr/include/libio.h
e.se.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.se.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.se.o: /usr/include/endian.h /usr/include/bits/endian.h
e.se.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.se.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.se.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.se.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.se.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h e.se.h
e.e.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.e.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.e.o: /usr/include/gnu/stubs.h
e.e.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.e.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.e.o: /usr/include/bits/types.h /usr/include/libio.h
e.e.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.e.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.e.o: /usr/include/endian.h /usr/include/bits/endian.h
e.e.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.e.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.e.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.e.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.e.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h e.e.h
e.mk.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.mk.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.mk.o: /usr/include/gnu/stubs.h
e.mk.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.mk.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.mk.o: /usr/include/bits/types.h /usr/include/libio.h
e.mk.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.mk.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.mk.o: /usr/include/endian.h /usr/include/bits/endian.h
e.mk.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.mk.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.mk.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.mk.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.mk.o: /usr/include/bits/posix2_lim.h e.t.h e.inf.h e.m.h
e.ra.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.ra.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.ra.o: /usr/include/gnu/stubs.h
e.ra.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.ra.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.ra.o: /usr/include/bits/types.h /usr/include/libio.h
e.ra.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.ra.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.ra.o: /usr/include/endian.h /usr/include/bits/endian.h
e.ra.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.ra.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.ra.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.ra.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.ra.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h
e.re.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.re.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.re.o: /usr/include/gnu/stubs.h
e.re.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.re.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.re.o: /usr/include/bits/types.h /usr/include/libio.h
e.re.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.re.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.re.o: /usr/include/endian.h /usr/include/bits/endian.h
e.re.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.re.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.re.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.re.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.re.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h e.se.h
e.t.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.t.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.t.o: /usr/include/gnu/stubs.h
e.t.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.t.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.t.o: /usr/include/bits/types.h /usr/include/libio.h
e.t.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.t.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.t.o: /usr/include/endian.h /usr/include/bits/endian.h
e.t.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.t.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.t.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.t.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.t.o: /usr/include/bits/posix2_lim.h e.t.h e.inf.h e.m.h e.tt.h e.wi.h
e.t.o: /usr/include/sys/time.h /usr/include/bits/time.h
e.ru.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.ru.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.ru.o: /usr/include/gnu/stubs.h
e.ru.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.ru.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.ru.o: /usr/include/bits/types.h /usr/include/libio.h
e.ru.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.ru.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.ru.o: /usr/include/endian.h /usr/include/bits/endian.h
e.ru.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.ru.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.ru.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.ru.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.ru.o: /usr/include/bits/posix2_lim.h e.t.h e.e.h e.m.h e.cm.h e.ru.h
e.ru.o: /usr/include/signal.h /usr/include/bits/signum.h
e.ru.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
e.ru.o: /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h
e.ru.o: /usr/include/bits/sigstack.h
e.cm.o: /usr/include/unistd.h /usr/include/features.h
e.cm.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.cm.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
e.cm.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.cm.o: /usr/include/bits/confname.h /usr/include/getopt.h
e.cm.o: /usr/include/dirent.h /usr/include/bits/dirent.h
e.cm.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
e.cm.o: /usr/include/linux/limits.h /usr/include/string.h
e.cm.o: /usr/include/sys/stat.h /usr/include/bits/stat.h e.h
e.cm.o: ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.cm.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.cm.o: /usr/include/libio.h /usr/include/_G_config.h
e.cm.o: /usr/include/bits/stdio_lim.h /usr/include/sys/types.h
e.cm.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
e.cm.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.cm.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.cm.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix2_lim.h
e.cm.o: e.t.h e.e.h e.m.h e.ru.h e.cm.h e.wi.h e.tt.h /usr/include/signal.h
e.cm.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
e.cm.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
e.cm.o: /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h
e.d.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.d.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.d.o: /usr/include/gnu/stubs.h
e.d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.d.o: /usr/include/bits/types.h /usr/include/libio.h
e.d.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.d.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.d.o: /usr/include/endian.h /usr/include/bits/endian.h
e.d.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.d.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.d.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.d.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.d.o: /usr/include/bits/posix2_lim.h e.t.h e.tt.h e.it.h
e.d.o: /usr/include/signal.h /usr/include/bits/signum.h
e.d.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
e.d.o: /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h
e.d.o: /usr/include/bits/sigstack.h
e.it.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.it.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.it.o: /usr/include/gnu/stubs.h
e.it.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.it.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.it.o: /usr/include/bits/types.h /usr/include/libio.h
e.it.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.it.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.it.o: /usr/include/endian.h /usr/include/bits/endian.h
e.it.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.it.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.it.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.it.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.it.o: /usr/include/bits/posix2_lim.h e.t.h e.it.h
e.la.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.la.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.la.o: /usr/include/gnu/stubs.h
e.la.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.la.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.la.o: /usr/include/bits/types.h /usr/include/libio.h
e.la.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.la.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.la.o: /usr/include/endian.h /usr/include/bits/endian.h
e.la.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.la.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.la.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.la.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.la.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h /usr/include/signal.h
e.la.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
e.la.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
e.la.o: /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h
e.m.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.m.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.m.o: /usr/include/gnu/stubs.h
e.m.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.m.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.m.o: /usr/include/bits/types.h /usr/include/libio.h
e.m.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.m.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.m.o: /usr/include/endian.h /usr/include/bits/endian.h
e.m.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.m.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.m.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.m.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.m.o: /usr/include/bits/posix2_lim.h e.t.h e.cm.h e.inf.h e.m.h e.tt.h e.e.h
e.m.o: /usr/include/sys/time.h /usr/include/bits/time.h
e.p.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.p.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.p.o: /usr/include/gnu/stubs.h
e.p.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.p.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.p.o: /usr/include/bits/types.h /usr/include/libio.h
e.p.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.p.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.p.o: /usr/include/endian.h /usr/include/bits/endian.h
e.p.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.p.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.p.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.p.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.p.o: /usr/include/bits/posix2_lim.h e.t.h e.cm.h e.inf.h e.m.h
e.p.o: /usr/include/ctype.h
e.pa.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.pa.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.pa.o: /usr/include/gnu/stubs.h
e.pa.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.pa.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.pa.o: /usr/include/bits/types.h /usr/include/libio.h
e.pa.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.pa.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.pa.o: /usr/include/endian.h /usr/include/bits/endian.h
e.pa.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.pa.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.pa.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.pa.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.pa.o: /usr/include/bits/posix2_lim.h e.t.h e.m.h e.cm.h
e.sb.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.sb.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.sb.o: /usr/include/gnu/stubs.h
e.sb.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.sb.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.sb.o: /usr/include/bits/types.h /usr/include/libio.h
e.sb.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.sb.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.sb.o: /usr/include/endian.h /usr/include/bits/endian.h
e.sb.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.sb.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.sb.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.sb.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.sb.o: /usr/include/bits/posix2_lim.h e.t.h e.fn.h e.sg.h
e.sb.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
e.sb.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
e.sb.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
e.sb.o: /usr/include/termio.h /usr/include/termios.h
e.sb.o: /usr/include/bits/termios.h /usr/include/signal.h
e.sb.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
e.sb.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
e.sb.o: /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h
e.sb.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/fcntl.h
e.sb.o: /usr/include/bits/fcntl.h /usr/include/pwd.h
e.tt.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.tt.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.tt.o: /usr/include/gnu/stubs.h
e.tt.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.tt.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.tt.o: /usr/include/bits/types.h /usr/include/libio.h
e.tt.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.tt.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.tt.o: /usr/include/endian.h /usr/include/bits/endian.h
e.tt.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.tt.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.tt.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.tt.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.tt.o: /usr/include/bits/posix2_lim.h e.t.h e.tt.h e.sg.h
e.tt.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
e.tt.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
e.tt.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
e.tt.o: /usr/include/termio.h /usr/include/termios.h
e.tt.o: /usr/include/bits/termios.h term/standard.c term/tcap.c term/vt100.c
e.x.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.x.o: /usr/include/features.h /usr/include/sys/cdefs.h
e.x.o: /usr/include/gnu/stubs.h
e.x.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.x.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.x.o: /usr/include/bits/types.h /usr/include/libio.h
e.x.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.x.o: /usr/include/string.h /usr/include/sys/types.h /usr/include/time.h
e.x.o: /usr/include/endian.h /usr/include/bits/endian.h
e.x.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.x.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h ../la1/la.h
e.x.o: ../ff3/ff.h /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.x.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.x.o: /usr/include/bits/posix2_lim.h e.t.h e.e.h e.inf.h e.m.h e.sg.h
e.x.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
e.x.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
e.x.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
e.x.o: /usr/include/termio.h /usr/include/termios.h
e.x.o: /usr/include/bits/termios.h /usr/include/signal.h
e.x.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
e.x.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
e.x.o: /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h
e.keyboard_map.o: e.h ../include/c_env.h ../include/localenv.h
e.keyboard_map.o: /usr/include/stdio.h /usr/include/features.h
e.keyboard_map.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.keyboard_map.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.keyboard_map.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.keyboard_map.o: /usr/include/bits/types.h /usr/include/libio.h
e.keyboard_map.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.keyboard_map.o: /usr/include/string.h /usr/include/sys/types.h
e.keyboard_map.o: /usr/include/time.h /usr/include/endian.h
e.keyboard_map.o: /usr/include/bits/endian.h /usr/include/sys/select.h
e.keyboard_map.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
e.keyboard_map.o: /usr/include/sys/sysmacros.h ../la1/la.h ../ff3/ff.h
e.keyboard_map.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.keyboard_map.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.keyboard_map.o: /usr/include/bits/posix2_lim.h e.t.h e.cm.h e.it.h e.tt.h
e.keyboard_map.o: /usr/include/stdlib.h /usr/include/alloca.h
e.keyboard_map.o: /usr/include/termios.h /usr/include/bits/termios.h
e.keyboard_map.o: /usr/include/sys/ttydefaults.h /usr/include/unistd.h
e.keyboard_map.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
e.keyboard_map.o: /usr/include/getopt.h /usr/include/sys/time.h
e.keyboard_map.o: /usr/include/bits/time.h /usr/include/X11/Xlib.h
e.keyboard_map.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
e.keyboard_map.o: /usr/include/X11/Xosdefs.h /usr/include/X11/Intrinsic.h
e.keyboard_map.o: /usr/include/X11/Xutil.h /usr/include/X11/Xresource.h
e.keyboard_map.o: /usr/include/X11/Core.h /usr/include/X11/Composite.h
e.keyboard_map.o: /usr/include/X11/Constraint.h /usr/include/X11/Object.h
e.keyboard_map.o: /usr/include/X11/RectObj.h /usr/include/X11/Xatom.h
e.keyboard_map.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
e.keyboard_map.o: /usr/include/X11/XKBlib.h
e.keyboard_map.o: /usr/include/X11/extensions/XKBstr.h
e.keyboard_map.o: /usr/include/X11/extensions/XKB.h
e.keyboard_map.o: /usr/include/X11/extensions/XKBfile.h
e.keyboard_map.o: /usr/include/X11/extensions/XKBgeom.h
e.resize.o: /usr/include/string.h /usr/include/features.h
e.resize.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.resize.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.resize.o: /usr/include/signal.h /usr/include/bits/sigset.h
e.resize.o: /usr/include/bits/types.h /usr/include/bits/signum.h
e.resize.o: /usr/include/time.h /usr/include/bits/siginfo.h
e.resize.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
e.resize.o: /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h e.h
e.resize.o: ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.resize.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.resize.o: /usr/include/libio.h /usr/include/_G_config.h
e.resize.o: /usr/include/bits/stdio_lim.h /usr/include/sys/types.h
e.resize.o: /usr/include/endian.h /usr/include/bits/endian.h
e.resize.o: /usr/include/sys/select.h /usr/include/bits/select.h
e.resize.o: /usr/include/sys/sysmacros.h ../la1/la.h ../ff3/ff.h
e.resize.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
e.resize.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
e.resize.o: /usr/include/bits/posix2_lim.h e.t.h e.cm.h e.inf.h e.tt.h e.wi.h
e.who.o: /usr/include/stdio.h /usr/include/features.h
e.who.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
e.who.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
e.who.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
e.who.o: /usr/include/bits/types.h /usr/include/libio.h
e.who.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
e.who.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
e.who.o: /usr/include/bits/endian.h /usr/include/sys/select.h
e.who.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
e.who.o: /usr/include/sys/sysmacros.h /usr/include/unistd.h
e.who.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
e.who.o: /usr/include/getopt.h /usr/include/string.h /usr/include/netdb.h
e.who.o: /usr/include/rpc/netdb.h /usr/include/sys/socket.h
e.who.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
e.who.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
e.who.o: /usr/include/netinet/in.h /usr/include/stdint.h
e.who.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
e.who.o: /usr/include/arpa/inet.h /usr/include/utmp.h
e.who.o: /usr/include/bits/utmp.h /usr/include/paths.h
e.who.o: /usr/include/sys/time.h /usr/include/bits/time.h
e.who.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
e.who.o: /usr/include/errno.h /usr/include/bits/errno.h
e.who.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
