#!/bin/sh
#
# This file is part of WinMusik 3 by Patrick Fedick
#
# $Author: pafe $
# $Revision: 1.8 $
# $Date: 2010/10/17 11:55:53 $
# $Id: build_binary.sh,v 1.8 2010/10/17 11:55:53 pafe Exp $
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
VERSION="3.0.2"
NAME="WinMusik"
PACKAGENAME="WinMusik3"
HOMEPAGE="http://www.winmusik.de/"
MAINTAINER="Patrick Fedick <patrick@pfp.de>"
DESCRIPTION="Datenbank zur Verwaltung von Musik-Tonträgern"
DISTFILES=$MYPWD
MAKE="make"
QMAKE="qmake-qt4"

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
			TESTGUI=TestGUI.exe
			;;
	esac
fi

# Wir haben nun unter anderem:
#    DISTRIB_ID=Ubuntu
#    DISTRIB_RELEASE=9.10

echo "Baue WinMusik für: $DISTRIB_ID, $DISTRIB_RELEASE..."
echo ""


build_binary() {

case "$DISTRIB_ID:$DISTRIB_RELEASE" in
	MINGW32*:1.0.11*)
		MAKE=gmake
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
			--with-pcre=/usr/local --with-openssl --with-libmhash=no --with-libiconv-prefix=/usr/local --with-nasm \
			--with-libmcrypt-prefix=no --with-libiconv-prefix=/usr/local \
			--without-pgsql --without-png --without-lame --without-ft-prefix \
			--without-libmad --without-lame --without-x --without-mysql --without-sybase \
			--with-libcurl --with-sdl-prefix=$BUILD
		;;
	Ubuntu:*)
		./configure --prefix=$BUILD \
			--with-pcre=/usr --without-openssl --with-libiconv-prefix --with-nasm \
			--with-libmcrypt-prefix=no --with-libmhash=no \
			--without-pgsql --without-png --without-lame --without-ft-prefix \
			--without-libmad --without-lame --without-x --without-mysql --without-sybase \
			--with-libcurl --with-sdl-prefix=$BUILD
		;;	
	Fedora:*)
		./configure --prefix=$BUILD \
			--with-pcre=/usr --without-openssl --with-libiconv-prefix --with-nasm \
			--with-libmcrypt-prefix=no --with-libmhash=no \
			--without-pgsql --without-png --without-lame --without-ft-prefix \
			--without-libmad --without-lame --without-x --without-mysql --without-sybase \
			--with-libcurl --with-sdl-prefix=$BUILD
		;;	
	
	MINGW32*:1.0.11*)
                export CPPFLAGS="-DCURL_STATICLIB -I/usr/local/include -I/sdk/WindowsSDK/include"
                export LDLAGS="-DCURL_STATICLIB -L/usr/local/lib -L/sdk/WindowsSDK/lib"
                export CFLAGS="-DCURL_STATICLIB"
                ./configure --prefix=$BUILD \
			--with-pcre=/usr/local --with-bzip2=/usr/local --with-zlib=/usr/local \
                        --with-nasm --with-libiconv-prefix=/usr/local \
                        --without-pgsql --without-png --without-lame --without-ft-prefix \
						--without-libmad --without-lame --without-x --without-mysql --without-sybase \
						--with-libcurl --with-sdl-prefix=$BUILD \
                        --with-libmhash=no \
                        --with-libmcrypt-prefix=no \
                        --without-openssl --with-libcurl
		MAKE=gmake
		;;
	*)
		./configure --prefix=$BUILD \
			--with-pcre=/usr --with-openssl --with-libiconv-prefix --with-nasm \
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
	mkdir -p $DISTFILES
	dpkg -b debian $DISTFILES/$DISTNAME-Ubuntu-$DISTRIB_RELEASE-$PLATFORM.deb
	if [ $? -ne 0 ] ; then
		echo "ERROR: Fehler beim Erstellen des Pakets"
		exit
	fi
	cp $DISTFILES/$DISTNAME-Ubuntu-$DISTRIB_RELEASE-$PLATFORM.deb /ftp/winmusik
	
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
	//cp distfiles/$NAME-$VERSION-Win32Setup.exe g:/gui
	
}



##############################################################################################

ARCH=`uname -m`;
PLATFORM="i386";

if [ "$ARCH" = "x86_64" ] ; then
	PLATFORM="amd64"
fi

##############################################################################################
WORK=$MYPWD
BUILD=$MYPWD/build

build_binary

##############################################################################################
# Pakete bauen
if [ "$DISTRIB_ID" = "Ubuntu" ] ; then
	build_ubuntu
elif [ "$DISTRIB_ID" = "MINGW32" ] ; then
	build_mingw32
fi

##############################################################################################
# Aufräumen
# rm -rf $WORK

