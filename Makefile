
# Makefile to produce the RAND e-19 editor
#
#   Must called by the GNU make program (gmake),
#       it uses conditional facility and shell function of GNU make.

# Check the consistancy of the target directory with ./include/localenv.h

# Check the consistancy of :
# --------------------------
#       the version and release with the rpm specification file
#       the curent directory name with ${SRCDIR}

#+++ parameters which can be overwrite by the make call
#       see Rand-editor-E19.56.spec


SHELL := /bin/sh

# get the current directory
PWD := $(shell pwd)

# Get the system identifier
ifndef OS
    OS := $(shell uname -s)
endif

ifeq ($(OS), SunOS)
    OS=Solaris
endif


# defaut builder (when logon as root)
BUILDER=perrioll

VERSION=E19
RELEASE=56

# Default value for package directory
PKGDIR=Rand
TARGETDIR_PREFIX=/ps/local
PROG=e

# Default utilities (must be GNU version)
TAR = gtar
INSTALL=ginstall

ifeq ($(OS), SunOS)
    OS=Solaris
endif

# Installation target system and directory
# ----------------------------------------

TARG_OS := $(OS)


# For cross compilation on sunps1 with
#   environnement defined by :
#       source /usr/lynx/3.1.0/ppc/SETUP.csh
#       source /usr/lynx/3.1.0/ppc/SETUP.bash
ifdef LYNXTARGET
    TARG_OS=LynxOS
endif

TARGETDIR := $(TARGETDIR_PREFIX)/$(TARG_OS)
#TARGETDIR=/usr/local

ifeq ($(TARG_OS), Linux)
#    TARGETDIR=/usr/local
endif

ifeq ($(TARG_OS), AIX)
#    TARGETDIR=/ps/local/AIX
endif

ifeq ($(TARG_OS), Solaris)
#    TARGETDIR=/ps/local/Solaris
endif

ifeq ($(TARG_OS), LynxOS)
    TARGETDIR := $(TARGETDIR_PREFIX)/ppc
endif

# Utilities to be used
# --------------------

ifeq ($(OS), LynxOS)
    INSTALL=install
    TAR=tar
endif

ifeq ($(OS), Linux)
    INSTALL=install
    TAR=tar
endif

#-----------------------
# notice : now LIBDIR and BINDIR must be the same directory

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
BINTARFILE=$(TARPREFIX).$(RELEASE).$(TARG_OS).bin.tgz




all:    e19/le fill/fill
	@echo Ready to deliver for $(TARG_OS)

local:
	@./local.sh

e19/le: e19/e.r.c la1/libla.a ff3/libff.a lib/libtmp.a
	cd e19; $(MAKE) TARG_OS=$(TARG_OS)

e19/e.r.c::
	cd e19; ./NewVersion $(VERSION) $(RELEASE) $(PKGDIR) $(TARGETDIR) $(BUILDER);

la1/libla.a::
	cd la1; $(MAKE) TARG_OS=$(TARG_OS)

ff3/libff.a::
	cd ff3; $(MAKE) TARG_OS=$(TARG_OS)

lib/libtmp.a::
	cd lib; $(MAKE) TARG_OS=$(TARG_OS)

fill/fill::
	cd fill; $(MAKE) TARG_OS=$(TARG_OS)


.PHONY: tar bintar clean preinstall install depend target test

tar:
	$(MAKE) clean
	rm -f ./${TARFILE}.old ../${TARFILE}
	(if test ! -d ./${ARCHIVE}; then mkdir ./${ARCHIVE}; fi)
#       -(mv -f ${TAREXCL} ./${ARCHIVE}; mv ./${ARCHIVE}/${TARFILE} .)
#       mv -f ${TARALLSPEC} ./${ARCHIVE}
#       mv ./${ARCHIVE}/${TARSPEC} .
	(if test -f ./${TARFILE}; then mv -f ./${TARFILE} ./${TARFILE}.old; fi)
	cd ../; $(TAR) \
	 --exclude '*.bak' \
	 --exclude '*.old' --exclude '*.ref' --exclude '*.new' \
	 --exclude '*-old' --exclude '*-ref' --exclude '*-new' \
	 --exclude '*_old' --exclude '*_ref' --exclude '*_new' \
	 --exclude ./${SRCDIR}/archv --exclude ./${SRCDIR}/temp \
	 --exclude './${SRCDIR}/${TAREXCL}' \
	 --exclude './${SRCDIR}/${TAREXCLSPEC}' \
	 -czvf ./${TARFILE} ./${SRCDIR}; \
	 mv ./${TARFILE} ${SRCDIR}; \
	 cd ./${SRCDIR}

bintar:
	@echo "$(TAR) for ${BINTARFILE} in $(PWD)/.."
	rm -f ../${BINTARFILE}.old
	(if test -f ../${BINTARFILE}; then mv -f ../${BINTARFILE} ../${BINTARFILE}.old; fi)
	cd ${LIBDIR}; cd ../; $(TAR) \
	--exclude './${PKGDIR}/.e?1' \
	--exclude './${PKGDIR}/.e?1.*' \
	--exclude './${PKGDIR}/,*' \
	--exclude './${PKGDIR}/.nfs*' \
	--exclude './${PKGDIR}/kbfiles/.e?1.*' \
	--exclude './${PKGDIR}/kbfiles/.e?1*' \
	--exclude './${PKGDIR}/kbfiles/,*' \
	--exclude './${PKGDIR}/kbfiles/*.[0-9]*' \
	--exclude './${PKGDIR}/e19.*' \
	-cvf $(PWD)/../${BINTARFILEPFX}.tar ./${PKGDIR}
	cd ${LIBDIR}; cd ../; $(TAR) \
	-rvf $(PWD)/../${BINTARFILEPFX}.tar ./${PKGDIR}/e19.$(RELEASE)
	cd ${PWD}
	rm -f ../${BINTARFILEPFX}.tar.gz
	gzip ../${BINTARFILEPFX}.tar
	mv ../${BINTARFILEPFX}.tar.gz ../${BINTARFILE}


clean:
	rm -f ,* a.out core .e?1 .e?1.*
	for f in fill la1 ff3 lib e19; do cd $$f; $(MAKE) clean; cd ..; done
	for f in include help doc; do cd $$f; rm -f ,* .e?1 .e?1.*; cd ..; done
	for f in help/kbfiles; do cd $$f; rm -f ,* .e?1 .e?1.*; cd ../..; done
	for f in doc/man; do cd $$f; rm -f ,* .e?1 .e?1.*; cd ../..; done

preinstall:
	-mkdir -p $(MANDIR) $(BINDIR) $(LIBDIR) $(KBFDIR)
	$(INSTALL) -m 444 help/Crashdoc $(LIBDIR)
	$(INSTALL) -m 444 help/errmsg $(LIBDIR)
	$(INSTALL) -m 444 help/recovermsg $(LIBDIR)
	$(INSTALL) -m 444 help/helpkey $(LIBDIR)
	$(INSTALL) -m 444 doc/man/e.l $(MANDIR)/e.$(MANE)
	@/bin/rm -f $(KBFDIR)/universalkb $(KBFDIR)/xtermkb $(KBFDIR)/nxtermkb
	@/bin/rm -f $(KBFDIR)/vt200kbn $(KBFDIR)/linuxkb
	$(INSTALL) -m 444 help/kbfiles/vt200kbn $(KBFDIR)
	$(INSTALL) -m 444 help/kbfiles/linuxkb $(KBFDIR)
	(cd $(KBFDIR); ln -s vt200kbn ./universalkb)
	(cd $(KBFDIR); ln -s vt200kbn ./xtermkb)
	(cd $(KBFDIR); ln -s vt200kbn ./nxtermkb)

install:
	@-/bin/rm -f $(TARGETDIR)/bin/e19
	@/bin/rm -f $(LIBDIR)/e19 $(LIBDIR)/fill $(LIBDIR)/run $(LIBDIR)/center $(LIBDIR)/just
	$(INSTALL) --strip e19/a.out $(LIBDIR)/e19.$(RELEASE)
	$(INSTALL) --strip fill/fill fill/run fill/center $(LIBDIR)
	(cd $(LIBDIR); ln -s fill ./just)
	(cd $(LIBDIR); ln -s ./e19.$(RELEASE) ./e19)
	-(cd $(TARGETDIR)/bin; ln -s ../$(PKGDIR)/e19 ./$(PROG))

depend:
	for f in fill la1 ff3 lib e19; do cd $$f; $(MAKE) TARG_OS=$(TARG_OS) depend; cd ..; done


target:
	@echo
	@echo "Target system is assumed to be : $(TARG_OS)"
	@echo "    installation directory : $(BINDIR)"
	@echo

test:
	@echo "shell is $(SHELL)"
	@echo "MAKE is $(MAKE)"
	@echo "TAR is $(TAR)"
	@echo "  bin tar file : $(BINTARFILE)"
	cd e19; $(MAKE) TARG_OS=$(TARG_OS) test


.PHONY: truc

truc:
	@echo "shell is $(SHELL)"
	@a=`which e`; \
	if test -L $$a; then b=`readlink $$a`; fi; \
	echo "e is : \"$$a\" --"; echo a link to $$b
