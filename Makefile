#
#       Copyright abandoned, 1983, The Rand Corporation
#

VER = e14

all: $(VER)/xe $(VER)/pres ambas/ambas

install: mkdirs etc_e_files installroot

installroot:
	echo 'you should be root when you do this'
	cd fill; make install
	cp $(VER)/xe /bin/e
	cp ambas/ambas /bin
	chmod a-w /etc/e

mkdirs:
	mkdir /etc/e
	chown root /etc/e
	chmod o-w /etc/e
	mkdir /tmp/etmp
	chmod 777 /tmp/etmp

$(VER)/xe: fill/fill fill/just fill/center \
	  lib/libtmp.a ff1/libff.a
	cd $(VER); make

lib/libtmp.a:
	cd lib; make

ff1/libff.a:
	cd ff1; make

etc_e_files:
	cp $(VER)/etc_e_errmsg /etc/e/errmsg
	chmod 444 /etc/e/errmsg

fill/fill fill/just fill/center:
	cd fill; make

$(VER)/pres: lib/libtmp.a
	cd $(VER); make pres

ambas/ambas:
	cd ambas; make

distribution:
	cd lib; make distribution
	cd ff1; make distribution
	cd $(VER); make distribution
	cd fill; make distribution
	cd ambas; make distribution

clean:
	cd lib; make clean
	cd ff1; make clean
	cd $(VER); make clean
	cd fill; make clean
	cd ambas; make clean
