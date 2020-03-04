#
# Makefile to produce the RAND e-19 editor
#   on linux platform for i386 hardware

# Check the consistancy of the target directory with ./include/localenv.h

# Check the consistancy of :
# --------------------------
#       the version and release with the rpm specification file
#       the curent directory name with ${SRCDIR}

# defaut builder (when logon as root)
BUILDER=perrioll

VERSION=E19
RELEASE=50

PKGDIR=Rand
TARGETDIR=/usr/local

BINDIR=$(TARGETDIR)/$(PKGDIR)
LIBDIR=$(BINDIR)
KBFDIR=$(BINDIR)/kbfiles
MANE=1
MANDIR=$(TARGETDIR)/man/man$(MANE)

TARFILE=$(PKGDIR)-$(VERSION).$(RELEASE).tgz
SRCDIR=$(PKGDIR)-$(VERSION)

all:    e19/le fill/fill
	@echo Ready to deliver

e19/le: e19/e.r.c la1/libla.a ff3/libff.a lib/libtmp.a
	cd e19; make

e19/e.r.c::
	cd e19; ./NewVersion $(BUILDER);

la1/libla.a::
	cd la1; make

ff3/libff.a::
	cd ff3; make

lib/libtmp.a::
	cd lib; make

fill/fill::
	cd fill; make

tar:
	make clean
	rm -f ./${TARFILE}.old ../${TARFILE}
	(if test -e ./${TARFILE}; then mv -f ./${TARFILE} ./${TARFILE}.old; fi)
	cd ../; tar --exclude ./${SRCDIR}/archv --exclude ./${SRCDIR}/temp --exclude ./${SRCDIR}/${TARFILE}.old -czvf ./${TARFILE} ./${SRCDIR}; mv ./${TARFILE} ${SRCDIR}; cd ./${SRCDIR}

clean:
	rm -f ,* a.out core .e?1 .e?1.*
	for f in fill la1 ff3 lib e19; do cd $$f; make clean; cd ..; done
	for f in include help doc; do cd $$f; rm -f ,* .e?1 .e?1.*; cd ..; done
	for f in help/kbfiles; do cd $$f; rm -f ,* .e?1 .e?1.*; cd ../..; done
	for f in doc/man; do cd $$f; rm -f ,* .e?1 .e?1.*; cd ../..; done

preinstall:
	-mkdir -p $(MANDIR) $(BINDIR) $(LIBDIR) $(KBFDIR)
	install -m 444 help/Crashdoc $(LIBDIR)
	install -m 444 help/errmsg $(LIBDIR)
	install -m 444 help/recovermsg $(LIBDIR)
	install -m 444 help/helpkey $(LIBDIR)
	install -m 444 doc/man/e.l $(MANDIR)/e.$(MANE)
	install -m 444 help/kbfiles/vt200kbn $(KBFDIR)
	@/bin/rm -f $(KBFDIR)/universalkb $(KBFDIR)/linuxkb $(KBFDIR)/xtermkb $(KBFDIR)/nxtermkb
	ln -s $(KBFDIR)/vt200kbn $(KBFDIR)/universalkb
	ln -s $(KBFDIR)/vt200kbn $(KBFDIR)/linuxkb
	ln -s $(KBFDIR)/vt200kbn $(KBFDIR)/xtermkb
	ln -s $(KBFDIR)/vt200kbn $(KBFDIR)/nxtermkb

install:
	@/bin/rm -f $(TARGETDIR)/bin/e $(LIBDIR)/e19 $(LIBDIR)/fill $(LIBDIR)/run $(LIBDIR)/center $(LIBDIR)/just
	install --strip e19/a.out $(LIBDIR)/e19
	install --strip fill/fill fill/run fill/center $(LIBDIR)
	ln -s $(LIBDIR)/fill $(LIBDIR)/just
	ln -s $(LIBDIR)/e19 $(TARGETDIR)/bin/e 

depend:
	for f in fill la1 ff3 lib e19; do cd $$f; make depend; cd ..; done
