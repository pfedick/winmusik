#!/bin/sh

MYPWD=`pwd`
WORK=$MYPWD/tmp
BUILD=$MYPWD/tmp/build
VERSION="3.0.0"
NAME="WinMusik"
PACKAGENAME="WinMusik3"
HOMEPAGE="http://www.winmusik.de/"
MAINTAINER="Patrick Fedick <patrick@pfp.de>"
DESCRIPTION="Datenbank zur Verwaltung von Musik-Tonträgern"
COMMENT=`cat README_en.TXT`
DISTFILES=$MYPWD/distfiles


if [ -f /etc/lsb-release ] ; then
	. /etc/lsb-release
elif [ -f /etc/system-release ] ; then
	DISTRIB_ID=`cat /etc/system-release | awk '{print $1}'`
	DISTRIB_RELEASE=`cat /etc/system-release | awk '{print $3}'`
else
	DISTRIB_ID=`uname -s`
	DISTRIB_RELEASE=`uname -r`
fi

# Wir haben nun unter anderem:
#    DISTRIB_ID=Ubuntu
#    DISTRIB_RELEASE=9.10

echo "Baue WinMusik für: $DISTRIB_ID, $DISTRIB_RELEASE..."
echo ""

build_binary() {

mkdir -p tmp/build
if [ ! -d tmp/build ] ; then
	echo "Could not create temporary directory tmp/build"
	exit 1
fi

cd tmp/build
#rm -rf *
mkdir -p include lib bin src
cd src
mkdir -p ppl6 wm30

echo "Kopiere PPL6 in temporäres Build-Verzeichnis"
cd ~/cvs-pfp/ppl/ppl6
if [ $? -ne 0 ] ; then
	echo "PPL6 not found in ~/cvs-pfp/ppl/ppl6"
	exit 1
fi

find *.m4 configure Doxyfile *.TXT *.in *.ico VERSION autoconf config docs include resource src tools tests | cpio -pdmv $BUILD/src/ppl6

cd $BUILD/src/ppl6

if [ ! -f Makefile ]
then
	echo "Konfiguriere PPL6..."

	case "$DISTRIB_ID:$DISTRIB_RELEASE" in
	FreeBSD:*)
        export CPPFLAGS=-I/usr/local/include
		export LDLAGS=-L/usr/local/lib
		./configure --prefix=$BUILD \
			--with-pcre=/usr/local --with-openssl --wit-libmhash=no --with-libiconv-prefix=/usr/local --with-nasm \
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
	
	*)
		echo "Unknown system"
		exit 1
		;;
	esac
	if [ $? -ne 0 ] ; then
		exit 1
	fi
fi
PATH=$BUILD/bin:$PATH

make -j2 release
if [ $? -ne 0 ] ; then
	exit 1
fi
make -j2 install
if [ $? -ne 0 ] ; then
	exit 1
fi


echo -n "ppl6-config liegt in "
which ppl6-config

echo "Kopiere WinMusik in temporäres Build-Verzeichnis"
cd ~/cvs-pfp/WinMusik/wm30
if [ $? -ne 0 ] ; then
	echo "WinMusik not found in ~/cvs-pfp/WinMusik/wm30"
	exit 1
fi
find docs forms include resources src *.TXT *.qm *.ts *.pro *.rc *.qrc | cpio -pdmv $BUILD/src/wm30
cat WinMusik3.pro | sed -e "s/--libs/--archive/" > $BUILD/src/wm30/WinMusik3.pro

cd $BUILD/src/wm30

echo "INCLUDEPATH += $BUILD/include" >> WinMusik3.pro
qmake-qt4
qmake-qt4
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
		cat $MYPWD/README_en.TXT | while read line
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
	cp $MYPWD/resources/icon48.png usr/share/pixmaps/WinMusik3.png

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
	
}


build_rpm() {
	cd $WORK
	(
		echo "Summary: Grafische Oberfläche des ITStt-Testsystems"
		echo "Name: $NAME"
		echo "Version: $VERSION"
		echo "Release: 1"
		echo "Copyright: GPL"
		echo "Packager: $MAINTAINER"
		echo "Group: Applications/Productivity"
		echo "Source: http://test2.opsblau.de/index.html?&CURPATH=&downloadfile=%2FDENIC-ITStt-TestGUI-1.0.0.tar.gz"
		echo "BuildRoot: /home/patrickf/cvs/testsystem/testgui/tmp/rpm/tmp"
		echo ""
		echo "%description"
		echo "$COMMENT"
		echo ""
		echo "%prep"
		echo ""
		echo "%build"
		echo ""
		echo "%install"
		echo "rm -rf \$RPM_BUILD_ROOT"
		echo "mkdir -p \$RPM_BUILD_ROOT/usr/bin"
		echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/pixmaps"
		echo ""
		echo "install -s -m 755 $BUILD/bin/TestGUI \$RPM_BUILD_ROOT/usr/bin/TestGUI"
		echo "install -m 644  $MYPWD/resources/icon64.png \$RPM_BUILD_ROOT/usr/share/pixmaps/testgui.png"
		echo ""
		echo "#%cleancd"
		echo "#rm -rf $RPM_BUILD_ROOT"
		echo ""
		echo "%files"
		echo "%defattr(-,root,root)"
		echo "/usr/bin/TestGUI"
		echo "/usr/share/pixmaps/testgui.png"
		echo ""
		echo "%changelog"
		echo ""
	) > testgui-$VERSION.spec
}

##############################################################################################

ARCH=`uname -m`;
PLATFORM="i386";

if [ "$ARCH" = "x86_64" ] ; then
	PLATFORM="amd64"
fi

##############################################################################################
# Libs und Binary bauen
build_binary

##############################################################################################
# Pakete bauen
if [ "$DISTRIB_ID" = "Ubuntu" ] ; then
	build_ubuntu
elif [ "$DISTRIB_ID" = "Fedora" ] ; then
	build_rpm
fi


##############################################################################################
# Aufräumen
# rm -rf $WORK

