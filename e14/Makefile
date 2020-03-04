#
#       Copyright abandoned, 1983, The Rand Corporation
#
# The following groups go together
#   DEFS=-DTERMCAP and TERMCAP=-ltermcap

O       = o/
A       =
STD     = /usr/include/
E       = xe
FF      = ../ff1/
FFLIB   = ../ff1/libff.a
DEFS    = -DTERMSIM -DUNIXV7 -DNAME -DDELETE -DTERMCAP
MISCLIB = ../lib/libtmp.a
INCL    = -I. -I../include -I$(FF)
CFLAGS  = $(A) -V -O $(DEFS) $(INCL)
LIBC    = -lc # -lg 
TERMCAP = -ltermcap

# Defines for making "depend" below
# Be sure they agree with $(DEFS)
# and with defines in e.h
IFS     =\
	-dVAX\
	-dUNIXV7\
	-dSMOOTHOUT\
	-uUNIXV6\
	-iuCOMMENT

HFILES=	e.cm.h e.fn.h e.h e.m.h e.ru.h e.sg.h e.t.h e.tt.h

OFILES  = \
	$(O)e.cm.o $(O)e.d.o $(O)e.e.o $(O)e.f.o $(O)e.fs.o \
	$(O)e.in.o $(O)e.m.o $(O)e.mk.o $(O)e.nm.o $(O)e.o \
	$(O)e.out.o $(O)e.p.o $(O)e.pa.o $(O)e.q.o $(O)e.r.o \
	$(O)e.ru.o $(O)e.sb.o $(O)e.se.o $(O)e.sv.o $(O)e.t.o \
	$(O)e.tb.o $(O)e.tt.o $(O)e.u.o $(O)e.wi.o $(O)e.wk.o \
	$(O)e.x.o

BASE=	$(O)e.out.o $(O)e.p.o $(O)e.pa.o $(O)e.q.o $(O)e.r.o \
	$(O)e.tb.o $(O)e.tt.o $(O)e.u.o $(O)e.wi.o $(O)e.wk.o \
	$(O)e.x.o
OV1=	$(O)e.in.o $(O)e.m.o $(O)e.mk.o $(O)e.nm.o $(O)e.o
OV2=	$(O)e.cm.o $(O)e.d.o $(O)e.e.o $(O)e.f.o $(O)e.fs.o
OV3=	$(O)e.ru.o $(O)e.sb.o $(O)e.se.o $(O)e.sv.o $(O)e.t.o

CFILES=	e.cm.c e.d.c e.e.c e.f.c e.fs.c e.in.c e.m.c e.mk.c e.nm.c \
	e.c e.out.c e.p.c e.pa.c e.q.c e.r.c e.ru.c e.sb.c e.se.c \
	e.sv.c e.t.c e.tb.c e.tt.c e.u.c e.wi.c e.wk.c e.x.c

$(E): $(OFILES)  $(FFLIB) $(MISCLIB)
	make newrev
	ld -X -i /lib/crt0.o $(BASE) -Z $(OV1) -Z $(OV2) -Z $(OV3) -Y \
		$(FFLIB) $(MISCLIB) $(TERMCAP) -lovc
	-mv $(E) $(E).old
	mv a.out $(E)

#	$(CC) -X -i $(OFILES) $(FFLIB) $(MISCLIB) $(TERMCAP)
#	${LD} -X -i -n -o ${OFILE} ${CONFOBJ} \
#		-Z ${OV1} -Z ${OV2} -Z ${OV3} -Z ${OV4} \
#		-Z ${OV5} -Z ${OV6} -Z ${OV7} \
#		-Y ${BASE} vers.o param.o
#	size ${OFILE}
#	./checksys ${OFILE}

newrev:
	mv e.r.c ,e.r.c
	awk -f Newrev.awk ,e.r.c > e.r.c
	$(CC) $(CFLAGS) -c e.r.c
	mv e.r.o $(O)e.r.o

depend:
	echo $(IFS) > ,defines
	/bin/sh Makedep $(HFILES) $(CFILES)

lint:
	lint $(DEFS) -DLINT $(INCL) $(CFILES) \
	$(FF)/llib-lff \
	-lc

distribution:   clean
	rm -f $(E) $(E).old pres

clean:
	rm -f $(OFILES) $(O)pres.o

pres: $(O)pres.o $(MISCLIB)
	$(CC)        \
	$(O)pres.o \
	$(MISCLIB)
	-mv pres ,pres
	mv a.out pres

$(O)e.cm.o: e.cm.c
	$(CC) $(CFLAGS) -c e.cm.c
	mv e.cm.o $(O)e.cm.o

$(O)e.d.o: e.d.c
	$(CC) $(CFLAGS) -c e.d.c
	mv e.d.o $(O)e.d.o

$(O)e.e.o: e.e.c
	$(CC) $(CFLAGS) -c e.e.c
	mv e.e.o $(O)e.e.o

$(O)e.f.o: e.f.c
	$(CC) $(CFLAGS) -c e.f.c
	mv e.f.o $(O)e.f.o

$(O)e.fs.o: e.fs.c
	$(CC) $(CFLAGS) -c e.fs.c
	mv e.fs.o $(O)e.fs.o

$(O)e.in.o: e.in.c
	$(CC) $(CFLAGS) -c e.in.c
	mv e.in.o $(O)e.in.o

$(O)e.m.o: e.m.c
	$(CC) $(CFLAGS) -c e.m.c
	mv e.m.o $(O)e.m.o

$(O)e.mk.o: e.mk.c
	$(CC) $(CFLAGS) -c e.mk.c
	mv e.mk.o $(O)e.mk.o

$(O)e.nm.o: e.nm.c
	$(CC) $(CFLAGS) -c e.nm.c
	mv e.nm.o $(O)e.nm.o

$(O)e.out.o: e.out.c
	$(CC) $(CFLAGS) -c e.out.c
	mv e.out.o $(O)e.out.o

$(O)e.o: e.c
	$(CC) $(CFLAGS) -c e.c
	mv e.o   $(O)e.o

$(O)e.p.o: e.p.c
	$(CC) $(CFLAGS) -c e.p.c
	mv e.p.o $(O)e.p.o

$(O)e.pa.o: e.pa.c
	$(CC) $(CFLAGS) -c e.pa.c
	mv e.pa.o $(O)e.pa.o

$(O)e.q.o: e.q.c
	$(CC) $(CFLAGS) -c e.q.c
	mv e.q.o $(O)e.q.o

$(O)e.r.o: e.r.c
	$(CC) $(CFLAGS) -c e.r.c
	mv e.r.o $(O)e.r.o

$(O)e.ru.o: e.ru.c
	$(CC) $(CFLAGS) -c e.ru.c
	mv e.ru.o $(O)e.ru.o

$(O)e.sb.o: e.sb.c
	$(CC) $(CFLAGS) -c e.sb.c
	mv e.sb.o $(O)e.sb.o

$(O)e.se.o: e.se.c
	$(CC) $(CFLAGS) -c e.se.c
	mv e.se.o $(O)e.se.o

$(O)e.sv.o: e.sv.c
	$(CC) $(CFLAGS) -c e.sv.c
	mv e.sv.o $(O)e.sv.o

$(O)e.t.o: e.t.c
	$(CC) $(CFLAGS) -c e.t.c
	mv e.t.o $(O)e.t.o

$(O)e.tb.o: e.tb.c
	$(CC) $(CFLAGS) -c e.tb.c
	mv e.tb.o $(O)e.tb.o

$(O)e.tt.o: e.tt.c
	$(CC) $(CFLAGS) -c e.tt.c
	mv e.tt.o $(O)e.tt.o

$(O)e.u.o: e.u.c
	$(CC) $(CFLAGS) -c e.u.c
	mv e.u.o $(O)e.u.o

$(O)e.wi.o: e.wi.c
	$(CC) $(CFLAGS) -c e.wi.c
	mv e.wi.o $(O)e.wi.o

$(O)e.wk.o: e.wk.c
	$(CC) $(CFLAGS) -c e.wk.c
	mv e.wk.o $(O)e.wk.o

$(O)e.x.o: e.x.c
	$(CC) $(CFLAGS) -c e.x.c
	mv e.x.o $(O)e.x.o

$(O)pres.o: pres.c
	$(CC) $(CFLAGS) -c pres.c
	mv pres.o $(O)pres.o

e.h:
	touch e.h

e.sg.h:
	touch e.sg.h

# DO NOT DELETE THIS LINE

e.h: $(STD)sys/types.h
e.h: $(FF)ff.h
e.h: e.t.h
e.sg.h: $(STD)sgtty.h
$(O)e.cm.o: e.h
$(O)e.cm.o: e.m.h
$(O)e.cm.o: e.ru.h
$(O)e.cm.o: e.cm.h
$(O)e.d.o: e.h
$(O)e.d.o: e.tt.h
$(O)e.e.o: e.h
$(O)e.e.o: e.fsd.h
$(O)e.e.o: e.m.h
$(O)e.e.o: e.cm.h
$(O)e.f.o: e.h
$(O)e.f.o: $(STD)sys/stat.h
$(O)e.fs.o: e.h
$(O)e.fs.o: e.fsd.h
$(O)e.fs.o: e.m.h
$(O)e.fs.o: $(STD)signal.h
$(O)e.in.o: e.h
$(O)e.in.o: e.fsd.h
$(O)e.in.o: e.m.h
$(O)e.in.o: $(STD)signal.h
$(O)e.m.o: e.h
$(O)e.m.o: e.m.h
$(O)e.m.o: e.tt.h
$(O)e.mk.o: e.h
$(O)e.mk.o: e.m.h
$(O)e.nm.o: e.h
$(O)e.nm.o: e.m.h
$(O)e.nm.o: e.cm.h
$(O)e.nm.o: $(STD)sys/stat.h
$(O)e.o: e.h
$(O)e.o: e.tt.h
$(O)e.o: e.fn.h
$(O)e.o: e.sg.h
$(O)e.o: $(STD)sys/stat.h
$(O)e.o: $(STD)signal.h
$(O)e.out.o: e.h
$(O)e.out.o: e.fsd.h
$(O)e.out.o: e.m.h
$(O)e.out.o: $(STD)signal.h
$(O)e.p.o: e.h
$(O)e.p.o: e.m.h
$(O)e.pa.o: $(STD)ctype.h
$(O)e.pa.o: e.h
$(O)e.pa.o: e.m.h
$(O)e.pa.o: e.cm.h
$(O)e.q.o: e.h
$(O)e.q.o: e.m.h
$(O)e.q.o: e.cm.h
$(O)e.q.o: e.fn.h
$(O)e.q.o: e.tt.h
$(O)e.q.o: $(STD)signal.h
$(O)e.r.o: e.h
$(O)e.ru.o: e.h
$(O)e.ru.o: e.fsd.h
$(O)e.ru.o: e.m.h
$(O)e.ru.o: e.cm.h
$(O)e.ru.o: e.ru.h
$(O)e.ru.o: $(STD)signal.h
$(O)e.sb.o: e.h
$(O)e.sb.o: e.fn.h
$(O)e.sb.o: e.sg.h
$(O)e.sb.o: $(STD)signal.h
$(O)e.se.o: $(STD)ctype.h
$(O)e.se.o: e.h
$(O)e.se.o: e.m.h
$(O)e.se.o: e.cm.h
$(O)e.sv.o: e.h
$(O)e.sv.o: e.m.h
$(O)e.sv.o: e.cm.h
$(O)e.sv.o: $(STD)sys/stat.h
$(O)e.t.o: e.h
$(O)e.t.o: e.tt.h
$(O)e.t.o: $(STD)signal.h
$(O)e.tb.o: e.h
$(O)e.tb.o: e.m.h
$(O)e.tb.o: e.cm.h
$(O)e.tt.o: e.h
$(O)e.tt.o: e.tt.h
$(O)e.tt.o: ../term/eoldfix.h
$(O)e.tt.o: ../term/standard.c
$(O)e.tt.o: ../term/tcap.c
$(O)e.tt.o: ../term/annarbor.c
$(O)e.tt.o: ../term/ambas.c
$(O)e.tt.o: ../term/adm3a.c
$(O)e.tt.o: ../term/adm31.c
$(O)e.tt.o: ../term/dy.c
$(O)e.tt.o: ../term/dm4000.c
$(O)e.tt.o: ../term/h19.c
$(O)e.tt.o: ../term/intext.c
$(O)e.tt.o: ../term/intext2.c
$(O)e.tt.o: ../term/adm42.c
$(O)e.tt.o: ../term/c100.c
$(O)e.u.o: e.h
$(O)e.u.o: e.m.h
$(O)e.u.o: e.cm.h
$(O)e.u.o: $(STD)sys/stat.h
$(O)e.wi.o: e.h
$(O)e.wi.o: e.tt.h
$(O)e.wk.o: e.h
$(O)e.x.o: e.h
$(O)e.x.o: e.fsd.h
$(O)e.x.o: e.m.h
$(O)e.x.o: e.sg.h

# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
