# Relocation directory
%define targdir /usr/local
Prefix: %{targdir}


Summary: Rand full screen text Editor for Linux
Name: Rand
Version: E19
Release: 53
Copyright: Copyright abandoned, 1983, The Rand Corporation
Group: Applications/Editors
Source: ftp://pspc7715.cern.ch/usr/local/src/%{name}/%{name}-%{version}.%{release}.tgz
Packager: Fabien PERRIOLLAT <Fabien.Perriollat@cern.ch>
URL: http://home.cern.ch/~perrioll/Rand_Editor/linux
Requires: XFree86
Exclusivearch: i386
Exclusiveos: Linux
Serial: 3

%define pkgname %{name}

%description
The %{name} editor is one of several editors running on UNIX systems. It is
a full screen text editor initialy developped by Rand Corporation,
(copyright abandoned, 1983, by the Rand Corporation). The Rand Corporation
initial version was called NED.
The linux version is an improved version of the E editor provided
at CERN on the PRIAM Unix service by Gordon Lee and Peter Villemoes.
It was enriched with more one-line help, new commands and key functions.
A default keyboard configuration for ANSI like terminals
(linux console, xterm family, vt200 family ..).
It has the capability to handle Unix and DOS text files.
.
The source (.src.rpm file) and binary for linux (.rpm) can be get from :
    %{url}
The current version is : %{name}-%{version}.%{release}

# ----------------------------------------------------------------------------
# to rebuild the package (bin and source files) :
#   cd /usr/src/redhat/SPECS
#   rpm -ba --clean Rand-editor-E19.53.spec
#
#   To load somewhere else than the default build in : /usr/local
#   rpm -ivh --prefix <somewhere> Rand-E19-53.i386.rpm
#   rpm -ivh --relocate /usr/local=<somewhere> Rand-E19-53.i386.rpm
#       to prevent creation of links in the doc system directory use
#           --excludedocs rpm option flag
#
# NB : the version and revision must be coherent with Rand-E19/e19/NewVersion file
# ----------------------------------------------------------------------------

# directory where to install the Rand editor
%define pkgdir %{pkgname}
%define targdir /usr/local
%define cfgdir %{targdir}/etc
%define docdir %{targdir}/doc/%{pkgname}
%define pkgtargdir %{targdir}/%{pkgdir}
%define pkgcfgdir %{cfgdir}/%{pkgdir}
%define bindir %{targdir}/bin
%define genericprgname %{bindir}/e

# keyboard definition files
%define KbDir kbfiles
%define RefKbFile vt200kbn
%define KbFiles universalkb linuxkb xtermkb nxtermkb
%define KbscDir ./help/%{KbDir}

%prep
%setup

%build
make depend
make VERSION=%{version} RELEASE=%{release} TARGETDIR=%{targdir} PKGDIR=%{pkgdir}

%install
make preinstall TARGETDIR=%{targdir} PKGDIR=%{pkgdir}
make install TARGETDIR=%{targdir} PKGDIR=%{pkgdir}

# link the executable to the generic name
    rm -f %{genericprgname}
    (cd %{bindir}; ln -s ../%{pkgname}/e19.%{release} ./e)

# install system wide keyboard configuration files
    mkdir -p %{pkgcfgdir}/%{KbDir}
    install -m 644 %{KbscDir}/%{RefKbFile} %{pkgcfgdir}/%{KbDir}
    for i in %{KbFiles} ; do
	if [ -e %{KbscDir}/$i -a ! -L %{KbscDir}/$i ]; then
	    rm -f %{pkgcfgdir}/%{KbDir}/$i
	    install -m 644 %{KbscDir}/$i %{pkgcfgdir}/%{KbDir}
	else
	    ( cd %{pkgcfgdir}/%{KbDir}; \
		rm -f ./$i; \
		ln -s ./%{RefKbFile} ./$i; \
	    )
	fi
    done;

# doc file to be installed into the package doc directory
    mkdir -p %{pkgtargdir}/doc
    install -m 644 ./doc/Introduction_to_Rand.txt %{pkgtargdir}/doc
    install -m 644 ./doc/Rand-E19_man.html %{pkgtargdir}/doc
    install -m 644 ./README %{pkgtargdir}
    rm -f %{docdir}/Introduction_to_Rand.txt %{docdir}/README %{docdir}/Rand-E19_man.html

# doc file to be linked into the default doc directory
    mkdir -p %{docdir}
    ( cd %{docdir}; \
	ln -s ../../%{pkgname}/doc/Introduction_to_Rand.txt ./; \
	ln -s ../../%{pkgname}/doc/Rand-E19_man.html ./; \
	ln -s ../../%{pkgname}/README ./; \
    )


%postun
    if [ -d %{docdir} ]; then
	rmdir --ignore-fail-on-non-empty %{docdir}
    fi
    if [ -d %{pkgcfgdir} ]; then
	rmdir --ignore-fail-on-non-empty %{pkgcfgdir}/%{KbDir}
	rmdir --ignore-fail-on-non-empty %{pkgcfgdir}
    fi

%files
%defattr(-,root,root)
%{targdir}/man/man1/e.1
%doc %{docdir}/README
%doc %{docdir}/Introduction_to_Rand.txt
%doc %{docdir}/Rand-E19_man.html
# %dir %{pkgcfgdir}
# %dir %{pkgcfgdir}/%{KbDir}
%config(missingok) %{pkgcfgdir}/%{KbDir}/vt200kbn
%config(missingok) %{pkgcfgdir}/%{KbDir}/universalkb
%config(missingok) %{pkgcfgdir}/%{KbDir}/linuxkb
%config(missingok) %{pkgcfgdir}/%{KbDir}/xtermkb
%config(missingok) %{pkgcfgdir}/%{KbDir}/nxtermkb
%dir %{pkgtargdir}
%{pkgtargdir}/Crashdoc
%{pkgtargdir}/errmsg
%{pkgtargdir}/helpkey
%{pkgtargdir}/recovermsg
%{pkgtargdir}/e19.%{release}
%{pkgtargdir}/center
%{pkgtargdir}/fill
%{pkgtargdir}/just
%{pkgtargdir}/run
%dir %{pkgtargdir}/%{KbDir}
%{pkgtargdir}/%{KbDir}/vt200kbn
%{pkgtargdir}/%{KbDir}/universalkb
%{pkgtargdir}/%{KbDir}/linuxkb
%{pkgtargdir}/%{KbDir}/xtermkb
%{pkgtargdir}/%{KbDir}/nxtermkb
%{pkgtargdir}/README
%dir %{pkgtargdir}/doc
%{pkgtargdir}/doc/Introduction_to_Rand.txt
%{pkgtargdir}/doc/Rand-E19_man.html
%{genericprgname}

%changelog
* Fri Mar 03 2000 Fabien Perriollat <Fabien.Perriollat@cern.ch>
- revision 53
- Resize of terminal screen supported (with the restricion
  that it can be done only when a single editing window is used).
- Navigation in the command line history.
- Navigation in the edited file list.
* Mon Jan 31 2000 Fabien Perriollat <Fabien.Perriollat@cern.ch>
- revision 52
- keyboard definition file can use "#include <file_name>" directive.
- Try to use X11 Keyboard Extention for better understanding of key mapping.
- better installation of system wide keyboard configuration files.
* Fri Oct 16 1999 Fabien Perriollat <Fabien.Perriollat@cern.ch>
- revision 51
- package build with relocation capability
- all version and directory dependent Rand internal sysmbols are
    built from the variables defined in this specification file.
    (pacakage name, version, release #, default installation directory)
* Thu Jan  7 1999 Fabien Perriollat <Fabien.Perriollat@cern.ch>
- initial version of the packaging specification.
