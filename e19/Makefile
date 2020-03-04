#
#	Copyright abandoned, 1983, The Rand Corporation
#

# Rand Editor with LA package
#
#       MAKEFILE FOR RAND "LA" LINE-ACCESS I/O PACKAGE
#
# Require GNU make (use "ifeq" GNU make syntax)

# -----------------------------------------------------------
# On a sun with Solaris :  (F.Perriollat Dec 1996)
# do not use -O : in e.t.c result in strang effect which give
#       a core dump on deleting char on command line !!!!
# -----------------------------------------------------------
# MYCFLAGS flag value : include
#       -DNOX11 if you do not want X11 package
#       -DNOXKB if your X11 does not provide X11 Keyboard Extension
# -----------------------------------------------------------

ifndef TARG_OS
    TARG_OS := $(shell uname -s)
endif

MYCFLAGS=

ifeq ($(TARG_OS), Solaris)
    MYCFLAGS = -DNOXKB -DHAVE_UTMPX_H
endif

ifeq ($(TARG_OS), LynxOS)
    MYCFLAGS = -DNOX11 -DNOstrcasecmp
#   MYCFLAGS += -Dinfo=myinfo
endif

# CFLAGS = -O -I. -I../include -I../la1 -I../ff3
CFLAGS = -I. -I../include -I../la1 -I../ff3 -g $(MYCFLAGS)



# OBJS=   e.main.o e.o    e.iit.o e.q.o   e.tb.o  e.put.o e.nm.o  e.sv.o \
#         e.wi.o  e.wk.o  e.u.o   e.f.o   e.dif.o e.h.o   e.se.o  e.e.o \
#         e.mk.o  e.ra.o  e.re.o  e.t.o   e.ru.o  e.cm.o  e.d.o   e.it.o \
#         e.la.o  e.m.o   e.p.o   e.pa.o  e.sb.o  e.tt.o  e.x.o \
#         e.keyboard_map.o e.resize.o e.who.o

SRCS=   e.main.c e.c    e.iit.c e.q.c   e.tb.c  e.put.c e.nm.c  e.sv.c \
	e.wi.c  e.wk.c  e.u.c   e.f.c   e.dif.c e.h.c   e.se.c  e.e.c \
	e.mk.c  e.ra.c  e.re.c  e.t.c   e.ru.c  e.cm.c  e.d.c   e.it.c \
	e.la.c  e.m.c   e.p.c   e.pa.c  e.sb.c  e.tt.c  e.x.c \
	e.keyboard_map.c e.resize.c e.who.c

ifeq ($(TARG_OS), Solaris)
	SRCS += my_scandir.c
endif

OBJS= $(SRCS:.c=.o)

LIBS=   ../la1/libla.a ../ff3/libff.a ../lib/libtmp.a
LIBSYS= -lcurses

LIBX=   -L/usr/X11R6/lib -lXt -lX11 -lxkbfile

ifeq ($(TARG_OS), Solaris)
    LIBX= -L/usr/X11R6/lib -lXt -lX11
    LIBSYS += -lxnet
endif

ifeq ($(TARG_OS), LynxOS)
    LIBX=
    LIBSYS += -lnetinet
endif


# For Ultrix, following line is needed !!
#TERMCAP = -ltermcap
TERMCAP =

NOX11=1

ALL=a.out

$(ALL): $(OBJS) e.r.o  $(LIBS)
	$(CC) $(CFLAGS) -o $(ALL) $(OBJS) e.r.o  $(LIBS) $(LIBX) $(LIBSYS) $(TERMCAP)

e.r.c:
	cd ../; make e19/e.r.c

clean:
	rm -f $(OBJS) $(ALL)  e.r.c e.r.o ,* .e?1 .e?1.* core *.bak *.BAK
	cd term; rm -f ,* .e?1 .e?1.* core a.out *.bak *.BAK

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)
e.h:
	@echo Why doyou try to overwrite e.h? 

test:
	@echo TARG_OS is $(TARG_OS)
	@echo TARG_OS is defined : $(origin TARG_OS)
	@echo CFLAGS = $(CFLAGS)
	@echo LIBSYS = $(LIBSYS)
	@echo LIBX = $(LIBX)
	@echo -e "OBJS is\n $(OBJS)"


# DO NOT DELETE THIS LINE -- make depend depends on it.

e.main.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.main.o: /usr/include/standards.h /usr/include/va_list.h
e.main.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.main.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.main.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.main.o: /usr/include/limits.h e.t.h
e.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.o: /usr/include/standards.h /usr/include/va_list.h /usr/include/sys/types.h
e.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
e.o: /usr/include/sys/limits.h /usr/include/float.h /usr/include/string.h
e.o: ../la1/la.h ../ff3/ff.h /usr/include/limits.h e.t.h e.e.h e.it.h e.tt.h
e.o: e.wi.h e.fn.h e.sg.h /usr/include/termio.h /usr/include/sys/termio.h
e.o: /usr/include/sys/ioctl.h /usr/include/sys/ttychars.h
e.o: /usr/include/sys/ttydev.h /usr/include/sys/ttmap.h
e.o: /usr/include/termios.h e.inf.h /usr/include/sys/stat.h
e.o: /usr/include/sys/mode.h /usr/include/fcntl.h /usr/include/sys/flock.h
e.o: /usr/include/unistd.h /usr/include/sys/access.h /usr/include/sys/lockf.h
e.o: /usr/include/time.h /usr/include/stddef.h /usr/include/signal.h
e.o: /usr/include/sys/signal.h /usr/include/sys/context.h
e.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
e.iit.o: /usr/include/string.h e.h ../include/c_env.h ../include/localenv.h
e.iit.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
e.iit.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.iit.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.iit.o: /usr/include/float.h ../la1/la.h ../ff3/ff.h /usr/include/limits.h
e.iit.o: e.t.h e.it.h
e.q.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.q.o: /usr/include/standards.h /usr/include/va_list.h
e.q.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.q.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.q.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.q.o: /usr/include/limits.h e.t.h e.m.h e.cm.h e.fn.h e.tt.h e.it.h
e.q.o: /usr/include/signal.h /usr/include/sys/signal.h
e.q.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
e.q.o: /usr/include/sys/mstsave.h
e.tb.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.tb.o: /usr/include/standards.h /usr/include/va_list.h
e.tb.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.tb.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.tb.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.tb.o: /usr/include/limits.h e.t.h e.m.h e.cm.h
e.put.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.put.o: /usr/include/standards.h /usr/include/va_list.h
e.put.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.put.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.put.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.put.o: /usr/include/limits.h e.t.h e.m.h e.cm.h e.e.h
e.nm.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.nm.o: /usr/include/standards.h /usr/include/va_list.h
e.nm.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.nm.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.nm.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.nm.o: /usr/include/limits.h e.t.h e.inf.h e.m.h e.cm.h
e.nm.o: /usr/include/sys/stat.h /usr/include/sys/mode.h /usr/include/unistd.h
e.nm.o: /usr/include/sys/access.h /usr/include/sys/lockf.h
e.sv.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.sv.o: /usr/include/standards.h /usr/include/va_list.h
e.sv.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.sv.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.sv.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.sv.o: /usr/include/limits.h e.t.h e.m.h e.cm.h /usr/include/sys/stat.h
e.sv.o: /usr/include/sys/mode.h /usr/include/unistd.h
e.sv.o: /usr/include/sys/access.h /usr/include/sys/lockf.h
e.wi.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.wi.o: /usr/include/standards.h /usr/include/va_list.h
e.wi.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.wi.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.wi.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.wi.o: /usr/include/limits.h e.t.h e.cm.h e.inf.h e.tt.h e.wi.h
e.wk.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.wk.o: /usr/include/standards.h /usr/include/va_list.h
e.wk.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.wk.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.wk.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.wk.o: /usr/include/limits.h e.t.h e.inf.h
e.u.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.u.o: /usr/include/standards.h /usr/include/va_list.h
e.u.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.u.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.u.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.u.o: /usr/include/limits.h e.t.h e.m.h e.cm.h /usr/include/sys/stat.h
e.u.o: /usr/include/sys/mode.h /usr/include/pwd.h /usr/include/unistd.h
e.u.o: /usr/include/sys/access.h /usr/include/sys/lockf.h
e.f.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.f.o: /usr/include/standards.h /usr/include/va_list.h
e.f.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.f.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.f.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.f.o: /usr/include/limits.h e.t.h /usr/include/sys/stat.h
e.f.o: /usr/include/sys/mode.h
e.h.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.h.o: /usr/include/standards.h /usr/include/va_list.h
e.h.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.h.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.h.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.h.o: /usr/include/limits.h e.t.h e.cm.h e.m.h e.tt.h e.h.h
e.h.o: /usr/include/sys/time.h
e.se.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.se.o: /usr/include/standards.h /usr/include/va_list.h
e.se.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.se.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.se.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.se.o: /usr/include/limits.h e.t.h e.m.h e.cm.h e.se.h
e.e.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.e.o: /usr/include/standards.h /usr/include/va_list.h
e.e.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.e.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.e.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.e.o: /usr/include/limits.h e.t.h e.m.h e.cm.h e.e.h
e.mk.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.mk.o: /usr/include/standards.h /usr/include/va_list.h
e.mk.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.mk.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.mk.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.mk.o: /usr/include/limits.h e.t.h e.inf.h e.m.h
e.ra.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.ra.o: /usr/include/standards.h /usr/include/va_list.h
e.ra.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.ra.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.ra.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.ra.o: /usr/include/limits.h e.t.h e.m.h e.cm.h
e.re.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.re.o: /usr/include/standards.h /usr/include/va_list.h
e.re.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.re.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.re.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.re.o: /usr/include/limits.h e.t.h e.m.h e.cm.h e.se.h
e.t.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.t.o: /usr/include/standards.h /usr/include/va_list.h
e.t.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.t.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.t.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.t.o: /usr/include/limits.h e.t.h e.inf.h e.m.h e.tt.h e.wi.h
e.t.o: /usr/include/errno.h /usr/include/sys/time.h
e.ru.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.ru.o: /usr/include/standards.h /usr/include/va_list.h
e.ru.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.ru.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.ru.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.ru.o: /usr/include/limits.h e.t.h e.e.h e.m.h e.cm.h e.ru.h
e.ru.o: /usr/include/signal.h /usr/include/sys/signal.h
e.ru.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
e.ru.o: /usr/include/sys/mstsave.h
e.cm.o: /usr/include/unistd.h /usr/include/sys/access.h
e.cm.o: /usr/include/standards.h /usr/include/sys/lockf.h
e.cm.o: /usr/include/dirent.h /usr/include/sys/dir.h /usr/include/sys/types.h
e.cm.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
e.cm.o: /usr/include/sys/lkup.h /usr/include/limits.h
e.cm.o: /usr/include/sys/limits.h /usr/include/float.h /usr/include/string.h
e.cm.o: /usr/include/sys/stat.h /usr/include/sys/mode.h e.h
e.cm.o: ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.cm.o: /usr/include/va_list.h ../la1/la.h ../ff3/ff.h e.t.h e.e.h e.m.h
e.cm.o: e.ru.h e.cm.h e.wi.h e.tt.h /usr/include/signal.h
e.cm.o: /usr/include/sys/signal.h /usr/include/sys/context.h
e.cm.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
e.d.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.d.o: /usr/include/standards.h /usr/include/va_list.h
e.d.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.d.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.d.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.d.o: /usr/include/limits.h e.t.h e.tt.h e.it.h /usr/include/signal.h
e.d.o: /usr/include/sys/signal.h /usr/include/sys/context.h
e.d.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
e.it.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.it.o: /usr/include/standards.h /usr/include/va_list.h
e.it.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.it.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.it.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.it.o: /usr/include/limits.h e.t.h e.it.h
e.la.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.la.o: /usr/include/standards.h /usr/include/va_list.h
e.la.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.la.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.la.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.la.o: /usr/include/limits.h e.t.h e.m.h /usr/include/signal.h
e.la.o: /usr/include/sys/signal.h /usr/include/sys/context.h
e.la.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
e.m.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.m.o: /usr/include/standards.h /usr/include/va_list.h
e.m.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.m.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.m.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.m.o: /usr/include/limits.h e.t.h e.cm.h e.inf.h e.m.h e.tt.h e.e.h
e.m.o: /usr/include/sys/time.h
e.p.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.p.o: /usr/include/standards.h /usr/include/va_list.h
e.p.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.p.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.p.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.p.o: /usr/include/limits.h e.t.h e.cm.h e.inf.h e.m.h /usr/include/ctype.h
e.p.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
e.p.o: /usr/include/locale.h /usr/include/sys/localedef31.h
e.pa.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.pa.o: /usr/include/standards.h /usr/include/va_list.h
e.pa.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.pa.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.pa.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.pa.o: /usr/include/limits.h e.t.h e.m.h e.cm.h
e.sb.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.sb.o: /usr/include/standards.h /usr/include/va_list.h
e.sb.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.sb.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.sb.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.sb.o: /usr/include/limits.h e.t.h e.fn.h e.sg.h /usr/include/termio.h
e.sb.o: /usr/include/sys/termio.h /usr/include/sys/ioctl.h
e.sb.o: /usr/include/sys/ttychars.h /usr/include/sys/ttydev.h
e.sb.o: /usr/include/sys/ttmap.h /usr/include/termios.h /usr/include/signal.h
e.sb.o: /usr/include/sys/signal.h /usr/include/sys/context.h
e.sb.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
e.sb.o: /usr/include/sys/stat.h /usr/include/sys/mode.h /usr/include/fcntl.h
e.sb.o: /usr/include/sys/flock.h /usr/include/unistd.h
e.sb.o: /usr/include/sys/access.h /usr/include/sys/lockf.h /usr/include/pwd.h
e.tt.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.tt.o: /usr/include/standards.h /usr/include/va_list.h
e.tt.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.tt.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.tt.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.tt.o: /usr/include/limits.h e.t.h e.tt.h e.sg.h /usr/include/termio.h
e.tt.o: /usr/include/sys/termio.h /usr/include/sys/ioctl.h
e.tt.o: /usr/include/sys/ttychars.h /usr/include/sys/ttydev.h
e.tt.o: /usr/include/sys/ttmap.h /usr/include/termios.h term/standard.c
e.tt.o: term/tcap.c term/vt100.c
e.x.o: e.h ../include/c_env.h ../include/localenv.h /usr/include/stdio.h
e.x.o: /usr/include/standards.h /usr/include/va_list.h
e.x.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.x.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.x.o: /usr/include/float.h /usr/include/string.h ../la1/la.h ../ff3/ff.h
e.x.o: /usr/include/limits.h e.t.h e.e.h e.inf.h e.m.h e.sg.h
e.x.o: /usr/include/termio.h /usr/include/sys/termio.h
e.x.o: /usr/include/sys/ioctl.h /usr/include/sys/ttychars.h
e.x.o: /usr/include/sys/ttydev.h /usr/include/sys/ttmap.h
e.x.o: /usr/include/termios.h /usr/include/signal.h /usr/include/sys/signal.h
e.x.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
e.x.o: /usr/include/sys/mstsave.h
e.keyboard_map.o: /usr/include/stdlib.h /usr/include/standards.h
e.keyboard_map.o: /usr/include/sys/wait.h /usr/include/sys/types.h
e.keyboard_map.o: /usr/include/sys/inttypes.h /usr/include/sys/m_types.h
e.keyboard_map.o: /usr/include/sys/resource.h /usr/include/sys/time.h
e.keyboard_map.o: /usr/include/sys/signal.h /usr/include/sys/context.h
e.keyboard_map.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
e.keyboard_map.o: /usr/include/sys/localedef.h /usr/include/sys/limits.h
e.keyboard_map.o: /usr/include/float.h /usr/include/sys/lc_core.h
e.keyboard_map.o: /usr/include/locale.h /usr/include/sys/localedef31.h
e.keyboard_map.o: /usr/include/signal.h e.h ../include/c_env.h
e.keyboard_map.o: ../include/localenv.h /usr/include/stdio.h
e.keyboard_map.o: /usr/include/va_list.h /usr/include/string.h ../la1/la.h
e.keyboard_map.o: ../ff3/ff.h /usr/include/limits.h e.t.h e.cm.h e.it.h
e.keyboard_map.o: e.tt.h /usr/include/termios.h /usr/include/termio.h
e.keyboard_map.o: /usr/include/sys/termio.h /usr/include/sys/ioctl.h
e.keyboard_map.o: /usr/include/sys/ttychars.h /usr/include/sys/ttydev.h
e.keyboard_map.o: /usr/include/sys/ttmap.h /usr/include/unistd.h
e.keyboard_map.o: /usr/include/sys/access.h /usr/include/sys/lockf.h
e.keyboard_map.o: /usr/include/time.h /usr/include/stddef.h
e.keyboard_map.o: /usr/include/setjmp.h /usr/include/X11/Xlib.h
e.keyboard_map.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
e.keyboard_map.o: /usr/include/X11/Xosdefs.h /usr/include/X11/Intrinsic.h
e.keyboard_map.o: /usr/include/X11/Xutil.h /usr/include/X11/Xresource.h
e.keyboard_map.o: /usr/include/X11/Core.h /usr/include/X11/Composite.h
e.keyboard_map.o: /usr/include/X11/Constraint.h /usr/include/X11/Object.h
e.keyboard_map.o: /usr/include/X11/RectObj.h /usr/include/X11/Xatom.h
e.keyboard_map.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
e.keyboard_map.o: /usr/include/X11/aix_keysym.h /usr/include/X11/XKBlib.h
e.keyboard_map.o: /usr/include/X11/extensions/XKBstr.h
e.keyboard_map.o: /usr/include/X11/extensions/XKB.h
e.keyboard_map.o: /usr/include/X11/extensions/XKBfile.h
e.keyboard_map.o: /usr/include/X11/extensions/XKBgeom.h
e.resize.o: /usr/include/string.h /usr/include/signal.h
e.resize.o: /usr/include/sys/signal.h /usr/include/sys/context.h
e.resize.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
e.resize.o: /usr/include/sys/m_types.h e.h ../include/c_env.h
e.resize.o: ../include/localenv.h /usr/include/stdio.h
e.resize.o: /usr/include/standards.h /usr/include/va_list.h
e.resize.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.resize.o: /usr/include/sys/limits.h /usr/include/float.h ../la1/la.h
e.resize.o: ../ff3/ff.h /usr/include/limits.h e.t.h e.cm.h e.inf.h e.tt.h
e.resize.o: e.wi.h
e.who.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/va_list.h
e.who.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
e.who.o: /usr/include/sys/m_types.h /usr/include/sys/limits.h
e.who.o: /usr/include/float.h /usr/include/unistd.h /usr/include/sys/access.h
e.who.o: /usr/include/sys/lockf.h /usr/include/string.h /usr/include/netdb.h
e.who.o: /usr/include/netinet/in.h /usr/include/net/nh.h
e.who.o: /usr/include/sys/machine.h /usr/include/sys/socket.h
e.who.o: /usr/include/sys/uio.h /usr/include/sys/xmem.h
e.who.o: /usr/include/arpa/inet.h /usr/include/netinet/if_ether.h
e.who.o: /usr/include/net/if_dl.h /usr/include/utmp.h /usr/include/time.h
e.who.o: /usr/include/stddef.h /usr/include/sys/stat.h
e.who.o: /usr/include/sys/mode.h /usr/include/errno.h
