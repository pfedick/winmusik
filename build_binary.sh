#!/bin/sh
#
# This file is part of WinMusik 3 by Patrick Fedick
#
# $Author: pafe $
# $Revision: 1.15 $
# $Date: 2012/01/09 20:44:54 $
# $Id: build_binary.sh,v 1.15 2012/01/09 20:44:54 pafe Exp $
#
#
# Copyright (c) 2010 Patrick Fedick
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

MYPWD=`pwd`
WORK=$MYPWD/tmp
BUILD=$MYPWD/tmp/build
VERSION="3.0.4"
NAME="WinMusik"
PACKAGENAME="WinMusik3"
HOMEPAGE="http://www.winmusik.de/"
MAINTAINER="Patrick Fedick <patrick@pfp.de>"
DESCRIPTION="Datenbank zur Verwaltung von Musik-Tonträgern"
COMMENT=`cat README_en.TXT`
DISTFILES=$MYPWD
MAKE="make"
PPLPATH=~/sourceforge/ppl6
WINMUSIKPATH=~/svn/winmusik/client
TARGETPATH=/ftp/build
QTVERSION=4.7.4
QMAKE="qmake-qt4"
INNOSETUP=/c/Programme/Inno\ Setup\ 5/ISCC.exe
if [ -f /c/Program\ Files\ \(x86\)/Inno\ Setup\ 5/ISCC.exe ] ; then
	INNOSETUP=/c/Program\ Files\ \(x86\)/Inno\ Setup\ 5/ISCC.exe
fi

if [ -f /etc/lsb-release ] ; then
	. /etc/lsb-release
elif [ -f /etc/system-release ] ; then
	DISTRIB_ID=`cat /etc/system-release | awk '{print $1}'`
	DISTRIB_RELEASE=`cat /etc/system-release | awk '{print $3}'`
else
	DISTRIB_ID=`uname -s`
	DISTRIB_RELEASE=`uname -r`

	case "$DISTRIB_ID:$DISTRIB_RELEASE" in
		MINGW32*:1.0.11*)
			DISTRIB_ID="MINGW32";
			DISTRIB_RELEASE="1.0.11";
			;;
		MINGW32*:1.0.17*)
			DISTRIB_ID="MINGW32";
			DISTRIB_RELEASE="1.0.17";
			QTDIR=/c/Qt/$QTVERSION
			QMAKESPEC=win32-g++
			PATH=$PATH:/c/Qt/$QTVERSION/bin
			echo "PATH=$PATH"
			;;
	esac
fi

# Wir haben nun unter anderem:
#    DISTRIB_ID=Ubuntu
#    DISTRIB_RELEASE=9.10

echo "Baue WinMusik für: $DISTRIB_ID, $DISTRIB_RELEASE..."
echo ""

build_sources() {
mkdir -p tmp/build
if [ ! -d tmp/build ] ; then
	echo "Could not create temporary directory tmp/build"
	exit 1
fi

cd tmp/build
#rm -rf *
mkdir -p include lib bin src
cd src
mkdir -p ppl6 winmusik

echo "Kopiere PPL6 in temporäres Build-Verzeichnis"
cd $PPLPATH
if [ $? -ne 0 ] ; then
	echo "PPL6 not found in $PPLPATH"
	exit 1
fi

find *.m4 conf.sh configure Doxyfile *.TXT *.in *.ico VERSION autoconf config docs include resource src tools tests | cpio -pdmv $BUILD/src/ppl6

echo "Kopiere WinMusik in temporäres Build-Verzeichnis"
cd $WINMUSIKPATH
if [ $? -ne 0 ] ; then
	echo "WinMusik not found in $WINMUSIKPATH"
	exit 1
fi
find setup.iss docs forms include resources src *.TXT *.qm *.ts *.rc *.qrc | cpio -pdmv $BUILD/src/winmusik
cat WinMusik3.pro | sed -e "s/--libs/--archive/" > $BUILD/src/winmusik/WinMusik3.pro

cd $BUILD
mkdir -p WinMusik-$VERSION-src-complete/build
cd WinMusik-$VERSION-src-complete/build
mkdir -p include lib bin src
cd $BUILD

cp $WINMUSIKPATH/build_binary.sh WinMusik-$VERSION-src-complete

mv src WinMusik-$VERSION-src-complete/build
tar -czf $MYPWD/distfiles/WinMusik-$VERSION-src-complete.tar.gz WinMusik-$VERSION-src-complete
mv WinMusik-$VERSION-src-complete/build/src ./
rm -rf WinMusik-$VERSION-src-complete
	cp $MYPWD/distfiles/WinMusik-$VERSION-src-complete.tar.gz $TARGETPATH

}


build_binary() {

case "$DISTRIB_ID:$DISTRIB_RELEASE" in
	MINGW32*:1.0.11)
		MAKE=gmake
		QMAKE=qmake
		;;
	MINGW32*:1.0.17)
		MAKE=make
		QMAKE=qmake
		;;
esac

cd $BUILD/src/ppl6

if [ ! -f Makefile ]
then
	echo "Konfiguriere PPL6 fuer $DISTRIB_ID:$DISTRIB_RELEASE..."

	case "$DISTRIB_ID:$DISTRIB_RELEASE" in
	FreeBSD:*)
        export CPPFLAGS=-I/usr/local/include
		export LDLAGS=-L/usr/local/lib
		./configure --prefix=$BUILD \
			--with-pcre=/usr/local --with-libmhash --with-libiconv-prefix=/usr/local --with-nasm \
			--with-libmcrypt-prefix --with-libiconv-prefix=/usr/local \
			--without-pgsql --without-png --without-lame --without-ft-prefix \
			--without-libmad --without-lame --without-x --without-mysql --without-sybase \
			--with-libcurl --with-sdl-prefix=$BUILD
		;;
	Ubuntu:*)
		./configure --prefix=$BUILD \
			--with-pcre=/usr --without-openssl --with-libiconv-prefix --with-nasm \
			--with-libmcrypt-prefix --with-libmhash \
			--without-pgsql --without-png --without-lame --without-ft-prefix \
			--without-libmad --without-lame --without-x --without-mysql --without-sybase \
			--with-libcurl --with-sdl-prefix=$BUILD
		;;	
	Fedora:*)
		./configure --prefix=$BUILD \
			--with-pcre=/usr --without-openssl --with-libiconv-prefix --with-nasm \
			--with-libmcrypt-prefix --with-libmhash \
			--without-pgsql --without-png --without-lame --without-ft-prefix \
			--without-libmad --without-lame --without-x --without-mysql --without-sybase \
			--with-libcurl --with-sdl-prefix=$BUILD
		;;	
	
	MINGW32*:1.0.*)
                export CPPFLAGS="-DCURL_STATICLIB -I/usr/local/include -I/sdk/WindowsSDK/include"
                export LDLAGS="-DCURL_STATICLIB -L/usr/local/lib -L/sdk/WindowsSDK/lib"
                export CFLAGS="-DCURL_STATICLIB"
                ./configure --prefix=$BUILD \
			--with-pcre=/usr/local --with-bzip2=/usr/local --with-zlib=/usr/local \
                        --with-nasm --with-libiconv-prefix=/usr/local \
                        --without-pgsql --without-png --without-lame --without-ft-prefix \
						--without-libmad --without-lame --without-x --without-mysql --without-sybase \
						--with-libcurl --with-sdl-prefix=$BUILD \
                        --with-libmhash \
                        --with-libmcrypt-prefix \
                        --with-libcurl
		;;
	*)
		./configure --prefix=$BUILD \
			--with-pcre=/usr --with-libiconv-prefix --with-nasm \
			--with-libmcrypt-prefix=no --with-libmhash=no \
			--without-pgsql --without-png --without-lame --without-ft-prefix \
			--without-libmad --without-lame --without-x --without-mysql --without-sybase \
			--with-libcurl=/usr --with-sdl-prefix=$BUILD		;;
	esac
	if [ $? -ne 0 ] ; then
		exit 1
	fi
fi
PATH=$BUILD/bin:$PATH

$MAKE -j2 release
if [ $? -ne 0 ] ; then
	exit 1
fi
$MAKE -j2 install
if [ $? -ne 0 ] ; then
	exit 1
fi


echo -n "ppl6-config liegt in "
which ppl6-config

cd $BUILD/src/winmusik

case "$DISTRIB_ID:$DISTRIB_RELEASE" in
	MINGW32*)
		echo "INCLUDEPATH += c:/MinGW/msys/1.0/$BUILD/include" >> WinMusik3.pro
		echo "LIBPATH += C:/MinGW/msys/1.0/$BUILD/lib" >> WinMusik3.pro
		;;
	*)
		echo "INCLUDEPATH += $BUILD/include" >> WinMusik3.pro
		echo "LIBPATH += $BUILD/lib" >> WinMusik3.pro
		;;
esac



$QMAKE
$QMAKE
if [ $? -ne 0 ] ; then
	exit 1
fi
make -j2 release
if [ $? -ne 0 ] ; then
	exit 1
fi
cp release/WinMusik3 $BUILD/bin
strip $BUILD/bin/WinMusik3


}

ubuntu_write_control() {
	(
		echo "Source: $PACKAGENAME"
     	echo "Section: misc"
     	echo "Priority: optional"
     	echo "Maintainer: $MAINTAINER"
		echo "Package: $PACKAGENAME"
		echo "Version: $VERSION"
		echo "Homepage: $HOMEPAGE"
		echo "Architecture: $PLATFORM"
		echo "Depends: $DEPENDS"
		echo "Description: $DESCRIPTION"
		cat $BUILD/src/winmusik/README_en.TXT | while read line
		do
			if [ -n "$line" ] ; then
				echo " $line"
			fi
		done
		
	) > debian/control

}

#################################################################################
# UBUNTU-Paket bauen
#################################################################################
build_ubuntu() {
	echo "Baue Ubuntu-Paket für $DISTRIB_CODENAME"
	DISTNAME="$NAME-$VERSION";
	cd $WORK
	if [ $? -ne 0 ] ; then
		echo "Konnte nicht nach $WORK wechseln"
		exit
	fi
	rm -rf debian
	mkdir -p debian
	cd debian
	if [ $? -ne 0 ] ; then
		echo "Konnte nicht nach $WORK/debian wechseln"
		exit
	fi
	mkdir -p DEBIAN usr/bin usr/share/applications usr/share/pixmaps
	cp $BUILD/bin/WinMusik3 usr/bin
	cp $BUILD/src/winmusik/resources/icon48.png usr/share/pixmaps/WinMusik3.png

	cd $WORK
	DEPENDS=""
	ubuntu_write_control
	(
		echo "[Desktop Entry]"
		echo "Encoding=UTF-8"
		echo "Name=$PACKAGENAME"
		echo "Comment=$DESCRIPTION"
		echo "Exec=WinMusik3"
		echo "Terminal=false"
		echo "Type=Application"
		echo "Categories=GTK;GNOME;AudioVideo;"
		echo "Icon=/WinMusik3.png"
	) > debian/usr/share/applications/$NAME.desktop
	(
		echo "WinMusik ($VERSION) unstable; urgency=low"
		echo ""
		echo " * No upstream changes."
		echo ""
		echo -n "-- $USERNAME   "
		date 
		echo ""
		echo "Local variables:"
     	echo "mode: debian-changelog"
     	echo "End:"
	) > debian/changelog
	
	dpkg-shlibdeps debian/usr/bin/WinMusik3
	
	DEPENDS=`grep "shlibs:Depends" debian/substvars | sed -e "s/shlibs:Depends=//"`
	ubuntu_write_control
	
	cp debian/control debian/DEBIAN
	rm debian/control
	rm debian/substvars
	rm debian/changelog
	
	mkdir -p $DISTFILES
	dpkg -b debian $DISTFILES/$DISTNAME-Ubuntu-$DISTRIB_RELEASE-$PLATFORM.deb
	if [ $? -ne 0 ] ; then
		echo "ERROR: Fehler beim Erstellen des Pakets"
		exit
	fi
	cp $DISTFILES/$DISTNAME-Ubuntu-$DISTRIB_RELEASE-$PLATFORM.deb $TARGETPATH
	
}


build_specfile() {
	COMMENT=`cat $MYPWD/README_en.TXT| grep -v "^===.*" `
	cd $WORK
	(
	echo "Name:		$NAME"
	echo "Version:	$VERSION"
	echo "Release:	1%{?dist}"
	echo "Summary:	$DESCRIPTION"
	echo ""
	echo "Group:		Applications/Office"
	echo "License:	GPL"
	echo "URL:		$HOMEPAGE"
	#echo "Source:	$MYPWD/distfiles/WinMusik-$VERSION-src-complete.tar.gz"
	echo "Source:	WinMusik-%{version}-src-complete.tar.gz"
	echo "BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)"
	echo ""
	echo "BuildRequires:	gcc, gcc-c++, libgcc, bzip2-devel, zlib-devel, libstdc++-devel, openssl-devel, libcurl-devel, qt-devel, libidn-devel, glibc-devel, pcre-devel"
	echo "#Requires:"
	echo ""	
	echo "%description"
	echo "$COMMENT"
	echo ""
	echo "%prep"
	echo "%setup -q -n %{name}-%{version}-src-complete"
	echo ""
	echo "%build"
	echo "./build_binary.sh"
	echo ""
	echo "%install"
	echo "rm -rf \$RPM_BUILD_ROOT"
	echo "mkdir -p \$RPM_BUILD_ROOT/usr/bin"
	echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/icons"
	echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/applications"
	echo "cp build/bin/WinMusik3 \$RPM_BUILD_ROOT/usr/bin"
	echo "cp build/src/winmusik/resources/icon64.png \$RPM_BUILD_ROOT/usr/share/icons/WinMusik3.png"
	echo ""
	echo "# Desktop menu entry"
	echo "cat > %{name}.desktop << EOF"
	echo "[Desktop Entry]"
	echo "Name=$NAME"
	echo "Comment=$COMMENT"
	echo "Exec=WinMusik3"
	echo "Icon=WinMusik3.png"
	echo "Terminal=0"
	echo "Type=Application"
	echo "EOF"
	echo ""
	echo "desktop-file-install --vendor DENIC \\"
  	echo "  --dir \$RPM_BUILD_ROOT/usr/share/applications \\"
  	echo "  --add-category Application \\"
  	echo "  --add-category Office \\"
  	echo "%{name}.desktop"
  	echo ""
	echo "%clean"
	echo "rm -rf \$RPM_BUILD_ROOT"
	echo ""
	echo "%files"
	echo "%defattr(-,root,root,-)"
	echo "/usr/bin/*"
	echo "/usr/share/icons/*"
	echo "/usr/share/applications/*"
	echo "%doc"
	echo ""
	echo "%changelog"
	echo ""
	) > winmusik.spec
}

build_srpm() {
	if [ ! -d ~.rpmmacros ] ; then
		echo "Bereite RPM-Buildsystem vor..."
		mkdir -p ~/rpmbuild/BUILD ~/rpmbuild/RPMS ~/rpmbuild/SOURCES ~/rpmbuild/SPECS ~/rpmbuild/SRPMS
		if [ $? -ne 0 ] ; then
			echo "Konnte RPM-Verzeichnisse nicht anlegen: ~/rpmbuild/{BUILD,RPMS,S{OURCE,PEC,RPM}S}"
			exit 1
		fi
		echo "%_topdir $HOME/rpmbuild" > ~/.rpmmacros
	fi
	TOPDIR=`cat ~/.rpmmacros | grep "%_topdir" | grep -v grep | awk {'print $2'}`
	if [ -z "$TOPDIR" ] ; then
		echo "%_topdir ist nicht in ~/.rpmmacros vorhanden"
		exit 1;
	fi
	if [ ! -d "$TOPDIR" ] ; then
		echo "%_topdir ist nicht vorhanden: $TOPDIR"
		exit 1
	fi
	if [ ! -d "$TOPDIR/SOURCES" ] ; then
		echo "%_topdir/SOURCES ist nicht vorhanden: $TOPDIR/SOURCES"
		exit 1
	fi
	cp $MYPWD/distfiles/WinMusik-$VERSION-src-complete.tar.gz $TOPDIR/SOURCES
	cd $WORK
	rpmbuild -bs winmusik.spec
	if [ $? -ne 0 ] ; then
		echo "Build fehlgeschlagen"
		exit 1
	fi
	cp $TOPDIR/SRPMS/WinMusik-$VERSION-1.src.rpm $DISTFILES
	cp $TOPDIR/SRPMS/WinMusik-$VERSION-1.src.rpm $TARGETPATH
}


build_mingw32()
{
	echo "*******************************************************"
	echo "Erstelle Windows-Setup Programm"
	cd $BUILD/src/winmusik
	cat setup.iss | sed -e "s/OutputBaseFilename=.*/OutputBaseFilename=$NAME-$VERSION-Win32Setup/" \
		| sed -e "s/AppVerName=.*/AppVerName=WinMusik $VERSION/" \
		| sed -e "s/AppVersion=.*/AppVersion=$VERSION/" \
		| sed -e "s/VersionInfoVersion=.*/VersionInfoVersion=$VERSION/" \
		> setup2.iss

	"$INNOSETUP" setup2.iss
	if [ $? -ne 0 ] ; then
		echo "Inno-Setup fehlgeschlagen"
		exit 1
	fi
	cd $BUILD/src/winmusik
	cp distfiles/$NAME-$VERSION-Win32Setup.exe ../../../
	cp distfiles/$NAME-$VERSION-Win32Setup.exe $TARGETPATH
	
}



##############################################################################################

ARCH=`uname -m`;
PLATFORM="i386";

if [ "$ARCH" = "x86_64" ] ; then
	PLATFORM="amd64"
fi

##############################################################################################
# Sourcen zusammenfassen, sofern wir im Sourceverzeichnis der GUI sind
if [ -f WinMusik3.pro ] ; then
	echo "Erstelle Source-Distribution für WinMusik3..."
	build_sources
	echo "Fertig"
	echo "========================================================================================"
fi


WORK=$MYPWD/tmp
BUILD=$MYPWD/build

##############################################################################################
# Libs und Binary bauen
if [ "$1" != "nobin" ] ; then 
	build_binary
fi


##############################################################################################
# Pakete bauen
if [ "$DISTRIB_ID" = "Ubuntu" ] ; then
	if [ "$1" != "nobin" ] ; then
		build_ubuntu
	fi
	build_specfile
	build_srpm
	
elif [ "$DISTRIB_ID" = "MINGW32" ] ; then
	build_mingw32
fi

##############################################################################################
# Aufräumen
rm -rf $WORK
#echo "rm -rf $WORK"


