#
# Makefile to produce the RAND e-19 editor
#   on linux platform for i386 hardware

# Check the consistancy of the target directory with ./include/localenv.h

# Check the consistancy of :
# --------------------------
#       the version and release with the rpm specification file
#       the curent directory name with ${SRCDIR}

#+++ parameters which can be overwrite by the make call
#       see Rand-editor-E19.51.spec

# defaut builder (when logon as root)
BUILDER=perrioll

VERSION=E19
RELEASE=53

PKGDIR=Rand
TARGETDIR=/usr/local
#---

BINDIR=$(TARGETDIR)/$(PKGDIR)
LIBDIR=$(BINDIR)
KBFDIR=$(BINDIR)/kbfiles
MANE=1
MANDIR=$(TARGETDIR)/man/man$(MANE)

TARPREFIX=$(PKGDIR)-$(VERSION)
TARFILE=$(TARPREFIX).$(RELEASE).tgz
TAREXCL=$(TARPREFIX).*.tgz*
TARALLSPEC=$(PKGDIR)-editor-*.spec
TAREXCLSPEC=$(PKGDIR)-editor-E19.?[!$(RELEASE)].spec
TARSPEC=$(PKGDIR)-editor-$(VERSION).$(RELEASE).spec
SRCDIR=$(PKGDIR)-$(VERSION)
ARCHIVE=archv

all:    e19/le fill/fill
	@echo Ready to deliver

e19/le: e19/e.r.c la1/libla.a ff3/libff.a lib/libtmp.a
	cd e19; make

e19/e.r.c::
	cd e19; ./NewVersion $(VERSION) $(RELEASE) $(PKGDIR) $(TARGETDIR) $(BUILDER);

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
	(if test ! -d ./${ARCHIVE}; then mkdir ./${ARCHIVE}; fi)
	-(mv -f ${TAREXCL} ./${ARCHIVE}; mv ./${ARCHIVE}/${TARFILE} .)
	mv -f ${TARALLSPEC} ./${ARCHIVE}
	mv ./${ARCHIVE}/${TARSPEC} .
	(if test -e ./${TARFILE}; then mv -f ./${TARFILE} ./${TARFILE}.old; fi)
	cd ../; tar --exclude '*.old' --exclude '*.ref' --exclude '*.new' \
	 --exclude ./${SRCDIR}/archv --exclude ./${SRCDIR}/temp \
	 --exclude './${SRCDIR}/${TAREXCL}' \
	 --exclude './${SRCDIR}/${TAREXCLSPEC}' \
	 -czvf ./${TARFILE} ./${SRCDIR}; \
	 mv ./${TARFILE} ${SRCDIR}; \
	 cd ./${SRCDIR}

#        --exclude './${SRCDIR}/${TAREXCLSPEC}' \
#        -czvf ./${TARFILE} ./${SRCDIR} ./${SRCDIR}/${TARSPEC}; \

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
	@/bin/rm -f $(KBFDIR)/universalkb $(KBFDIR)/xtermkb $(KBFDIR)/nxtermkb
	@/bin/rm -f $(KBFDIR)/vt200kbn $(KBFDIR)/linuxkb
	install -m 444 help/kbfiles/vt200kbn $(KBFDIR)
	install -m 444 help/kbfiles/linuxkb $(KBFDIR)
	(cd $(KBFDIR); ln -s vt200kbn ./universalkb)
	(cd $(KBFDIR); ln -s vt200kbn ./xtermkb)
	(cd $(KBFDIR); ln -s vt200kbn ./nxtermkb)

install:
	@/bin/rm -f $(TARGETDIR)/bin/e $(LIBDIR)/e19 $(LIBDIR)/fill $(LIBDIR)/run $(LIBDIR)/center $(LIBDIR)/just
	install --strip e19/a.out $(LIBDIR)/e19.$(RELEASE)
	install --strip fill/fill fill/run fill/center $(LIBDIR)
	(cd $(LIBDIR); ln -s fill ./just)
	(cd $(LIBDIR); ln -s e19.$(RELEASE) ../bin/e)

depend:
	for f in fill la1 ff3 lib e19; do cd $$f; make depend; cd ..; done
