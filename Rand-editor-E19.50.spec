Summary: Rand full screen text Editor for Linux
Name: Rand
Version: E19
Release: 50
Copyright: Copyright abandoned, 1983, The Rand Corporation
Group: Applications/Editors
Source: ftp://pspc7715.cern.ch/usr/local/src/Rand/Rand-E19.50.tgz
Packager: Fabien PERRIOLLAT <Fabien.Perriollat@cern.ch>
Requires: XFree86
Exclusivearch: i386
Exclusiveos: Linux
Serial: 1

%description
The Rand editor is one of several editors running on UNIX systems. It is
a full screen text editor initialy developped by Rand Corporation,
(copyright abandoned, 1983, by the Rand Corporation). The Rand Corporation
initial version was called NED.
The linux version is an improved version of the E editor provided
at CERN on the PRIAM Unix service by Gordon Lee and Peter Villemoes.
It was enriched with more one-line help, new commands and key functions.
A default keyboard configuration for ANSI like terminals
(linux console, xterm family, vt200 family ..).
It has the capability to handle Unix and DOS text files.


# to rebuild the package (bin and source files) :
#   cd /usr/src/redhat/SPECS
#   rpm -ba --clean Rand-editor-E19.50.spec
#
# NB : the version and revision must be coherent with Rand-E19/e19/NewVersion file

# directory where to install the Rand editor
%define pkgdir Rand
%define targdir /usr/local
%define cfgdir /etc
%define pkgtargdir %{targdir}/%{pkgdir}
%define pkgcfgdir %{cfgdir}/%{pkgdir}
%define genericprgname /usr/bin/e


%prep
%setup

%build
make depend
make

%install
make preinstall TARGETDIR=%{targdir}
make install TARGETDIR=%{targdir}

# link the executable to the generic name
    rm -f %{genericprgname} %{targdir}/bin/e
    ln -s %{pkgtargdir}/e19 %{genericprgname}

# install system wide keyboard configuration files
    mkdir -p %{pkgcfgdir}/kbfiles
    install -m 644 ./help/kbfiles/vt200kbn %{pkgcfgdir}/kbfiles
    for i in universalkb linuxkb xtermkb nxtermkb ; do
	rm -f %{pkgcfgdir}/kbfiles/$i
	ln -s %{pkgcfgdir}/kbfiles/vt200kbn %{pkgcfgdir}/kbfiles/$i
    done

# link doc file to be copied into the default doc directory
    ln -s ./doc/Introduction_to_Rand.txt .

%files
%defattr(-,root,root)
%doc README
%doc Introduction_to_Rand.txt
%doc %{targdir}/man/man1/e.1
%dir %{pkgcfgdir}
%dir %{pkgcfgdir}/kbfiles
%config %{pkgcfgdir}/kbfiles/vt200kbn
%config %{pkgcfgdir}/kbfiles/universalkb
%config %{pkgcfgdir}/kbfiles/linuxkb
%config %{pkgcfgdir}/kbfiles/xtermkb
%config %{pkgcfgdir}/kbfiles/nxtermkb
%dir %{pkgtargdir}
%dir %{pkgtargdir}/kbfiles
%{pkgtargdir}/Crashdoc
%{pkgtargdir}/errmsg
%{pkgtargdir}/helpkey
%{pkgtargdir}/recovermsg
%{pkgtargdir}/e19
%{pkgtargdir}/center
%{pkgtargdir}/fill
%{pkgtargdir}/just
%{pkgtargdir}/kbfiles/vt200kbn
%{pkgtargdir}/kbfiles/universalkb
%{pkgtargdir}/kbfiles/linuxkb
%{pkgtargdir}/kbfiles/xtermkb
%{pkgtargdir}/kbfiles/nxtermkb
%{genericprgname}

%changelog
* Thu Jan  7 1999 Fabien Perriollat <Fabien.Perriollat@cern.ch>
- initial version of the packaging specification.

