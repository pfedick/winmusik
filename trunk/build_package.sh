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
# Copyright (c) 2013 Patrick Fedick <patrick@pfp.de>
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

PROGNAME="WinMusik"
VERSION="3.0.6"
REVISION="1"
NAME="WinMusik"
PACKAGENAME="WinMusik"
HOMEPAGE="http://www.winmusik.de/"
MAINTAINER="Patrick Fedick <patrick@pfp.de>"
DESCRIPTION="Music database to maintain songs on various devices"
TARGETPATH=/ftp/winmusik

PPL6SOURCE="../../ppl6"
PPL6REPO="https://svn.code.sf.net/p/pplib/code/lib/branches/RELENG_6"
WINMUSIKSOURCE=./
WINMUSIKREPO=svn://server.pfp.de/winmusik/client/trunk

MYPWD=`pwd`
CUR=`pwd`
WORK=$MYPWD/tmp
DISTFILES=$MYPWD/distfiles

MAKE="make"
QTVERSION=4.8.4
QMAKE="qmake-qt4"
QTDIR=""
QMAKESPEC=""
INNOSETUP=/c/Programme/Inno\ Setup\ 5/ISCC.exe
if [ -f /c/Program\ Files\ \(x86\)/Inno\ Setup\ 5/ISCC.exe ] ; then
	INNOSETUP=/c/Program\ Files\ \(x86\)/Inno\ Setup\ 5/ISCC.exe
fi

create_dir ()
{
	mkdir -p $1
	if [ $? -ne 0 ] ; then
		echo "ERROR: could not create: $1"
		exit 1
	fi
}

check_debian_package()
{
	PKG=$1
	echo -n "$PKG: "
	dpkg -l | grep $PKG >/dev/null 2>&1
	if [ $? -ne 0 ] ; then
		echo "no"
		MISSING="$MISSING $PKG"
	else
		echo "ok"
	fi
}

check_rpm_package ()
{
	PKG=$1
	echo -n "$PKG: "
	rpm -qa | grep $PKG >/dev/null 2>&1
	if [ $? -ne 0 ] ; then
		echo "no"
		MISSING="$MISSING $PKG"
	else
		echo "ok"
	fi
}

DISTRIB_ID=""
DISTRIB_RELEASE=""
ARCH=""
PLATFORM=""

identify_system()
{
	if [ -f /etc/lsb-release ] ; then
		. /etc/lsb-release
	elif [ -f /etc/system-release ] ; then
		DISTRIB_ID=`cat /etc/system-release | awk '{print $1}'`
		DISTRIB_RELEASE=`cat /etc/system-release | awk '{print $3}'`
	elif [ -f /etc/SuSE-release ] ; then
		DISTRIB_ID=`head -n1 /etc/SuSE-release | awk '{print $1}'`
		DISTRIB_RELEASE=`head -n1 /etc/SuSE-release | awk '{print $2}'`
		QMAKE=qmake
	else
		DISTRIB_ID=`uname -s`
		DISTRIB_RELEASE=`uname -r`
	
		case "$DISTRIB_ID:$DISTRIB_RELEASE" in
			MINGW32*:1.0.17*)
				DISTRIB_ID="MINGW32";
				DISTRIB_RELEASE="1.0.17";
				QTDIR=/c/Qt/$QTVERSION
				QMAKESPEC=win32-g++
				PATH="/c/Qt/$QTVERSION/bin:$PATH"
				QMAKE=qmake
				;;
		esac
	fi
	
	ARCH=`uname -m`;
	PLATFORM="i386";
	if [ "$ARCH" = "x86_64" ] || [ "$ARCH" = "amd64" ] ; then
	        PLATFORM="amd64"
	fi

	# Wir haben nun unter anderem:
	#    DISTRIB_ID=Ubuntu
	#    DISTRIB_RELEASE=9.10
	#    ARCH=amd64
	#    PLATFORM=amd64
}

gather_sources()
{
	TARGET=$1
	cd $CUR
	PPL6DIR=$TARGET/ppl6
	if [ -d "$PPL6SOURCE" ] ; then
		echo "INFO: Copy PPL6-sources from local directory: $PPL6SOURCE..."
		echo "INFO: Ziel: $TARGET/ppl6"
		create_dir "$TARGET/ppl6"
		cd $PPL6SOURCE
		find *.m4 autoconf config configure docs Doxyfile include Makefile.in ppl6-config.in *.TXT \
		   resource src tests/Makefile* tests/valgrind* tests/src tests/*.h tools | grep -v ".svn" | cpio -pdm "$TARGET/ppl6" > /dev/null 2>&1
		echo "INFO: done"
	else
		echo "INFO: checkout PPL6-sources from svn repository..."
		svn checkout $PPL6REPO $TARGET/ppl6
		if [ $? -ne 0 ] ; then
			echo "ERROR: checkout failed"
			exit 1
		fi
		echo "INFO: done"
	fi
	cd $CUR
	WINMUSIKDIR=$TARGET/WinMusik
	if [ -d "$WINMUSIKSOURCE" ] ; then
		echo "INFO: Copy WinMusik-sources from local directory: $WINMUSIKSOURCE..."
		echo "INFO: Ziel: $TARGET/WinMusik"
		create_dir "$TARGET/WinMusik"
		cd $WINMUSIKSOURCE
		find *.TXT WinMusik.pro *.qm *.ts setup.iss Doxyfile resource.rc resources resources.qrc src widgets include forms docs| grep -v ".svn" | cpio -pdm "$TARGET/WinMusik" > /dev/null 2>&1
        echo "INFO: done"
    else
		echo "INFO: checkout WinMusik-sources from svn repository..."
                svn checkout $WINMUSIKREPO $TARGET/WinMusik
                if [ $? -ne 0 ] ; then
                        echo "ERROR: checkout failed"
                        exit 1
                fi
                echo "INFO: done"
	fi
	cd $CUR
}

build_FreeBSD_package ()
{
	create_dir $WORK/FreeBSD
	cd $CUR/FreeBSD
	find ./ | grep -v ".svn" | cpio -pdmv $WORK/FreeBSD > /dev/null 2>&1
	cd $DISTFILES
	sha256 $PROGNAME-$VERSION-src.tar.bz2 > $WORK/FreeBSD/winmusik/distinfo
	echo "SIZE ($PROGNAME-$VERSION-src.tar.bz2) = `stat -f \"%z\" $PROGNAME-$VERSION-src.tar.bz2`" >> $WORK/FreeBSD/winmusik/distinfo
	cd $WORK/FreeBSD/winmusik; make clean;
	cd ..
	shar `find winmusik | grep -v ".svn" `| sed "s/^XPORTVERSION=.*$/XPORTVERSION=	$VERSION/" > $DISTFILES/$PROGNAME-$VERSION-FreeBSD-Port.shar
	if [ -d "$TARGETPATH" ] ; then
		cp $DISTFILES/$PROGNAME-$VERSION-FreeBSD-Port.shar $TARGETPATH
	fi
	cd $WORK
}

CONFIGURE=""

ppl6_linux_configure()
{
    CONFIGURE="--with-pcre=/usr --with-libiconv-prefix --with-nasm --with-libmcrypt-prefix"
    CONFIGURE="$CONFIGURE --without-libmhash --without-postgresql --with-png --with-jpeg --without-lame"
    CONFIGURE="$CONFIGURE --without-ft-prefix --without-libmad --without-lame --without-x --without-mysql "
    CONFIGURE="$CONFIGURE --with-libcurl --with-sdl-prefix=$WORK --without-libldns --without-freetds"
    CONFIGURE="$CONFIGURE --without-ogg"
}

build_ppl6 ()
{
    echo "INFO: building ppl6..."
    create_dir $1/bin
    create_dir $1/lib
    create_dir $1/include
    cd $PPL6DIR
    if [ ! -f Makefile ] ; then
        echo "INFO: Configuring ppl6..."
        ./configure --prefix=$1 $CONFIGURE
    fi
    if [ $? -ne 0 ] ; then
        echo "ERROR: configure for ppl6 failed"
        exit 1
    fi
    $MAKE -j2
    if [ $? -ne 0 ] ; then
        echo "ERROR: make for ppl6 failed"
        exit 1
    fi
    $MAKE install
    if [ $? -ne 0 ] ; then
        echo "ERROR: make install for ppl6 failed"
        exit 1
    fi
    cd ..
    echo "INFO: building ppl6 done"
    sleep 2
}

build_winmusik ()
{
    echo "INFO: building $PROGNAME..."
    create_dir $1/bin
    PATH="$1/bin:$PATH"
    export PATH
    echo "PATH=$PATH"
    cd $WINMUSIKDIR
    echo "INFO: calling $QMAKE"
    $QMAKE
    if [ $? -ne 0 ] ; then
                echo "ERROR: qmake for $PROGNAME failed"
                exit 1
        fi
    echo "INFO: calling $MAKE in `pwd`"
    $MAKE release
    if [ $? -ne 0 ] ; then
                echo "ERROR: make for $PROGNAME failed"
                exit 1
        fi
    cp release/$PROGNAME $1/bin/$PROGNAME
    echo "INFO: building $PROGNAME done"
}

write_desktop_application ()
{
    PREFIX=$1
    FILE=$2
    (
                echo "[Desktop Entry]"
                echo "Encoding=UTF-8"
                echo "Name=$PROGNAME"
                echo "Comment=$DESCRIPTION"
                echo "Exec=$PREFIX/bin/$PROGNAME"
                echo "Terminal=false"
                echo "Type=Application"
                echo "Categories=GTK;GNOME;AudioVideo;"
                echo "Icon=$PREFIX/share/pixmaps/$PROGNAME.png"
        ) > $FILE
}


debian_write_control ()
{
    (
        echo "Source: $PROGNAME"
        echo "Section: video"
        echo "Priority: optional"
        echo "Maintainer: $MAINTAINER"
        echo "Package: $PROGNAME"
        echo "Version: $VERSION"
        echo "Homepage: $HOMEPAGE"
        echo "Architecture: $PLATFORM"
        echo "Depends: $DEPENDS"
        echo "Installed-Size: 1000"
        echo "Description: $DESCRIPTION"
        cat WinMusik/README_en.TXT | while read line
        do
                if [ -z "$line" ] ; then
                    echo " ."
                else
                    echo "  $line"
                fi
        done
        ) > debian/control
}

#################################################################################
# DEBIAN/UBUNTU-Paket bauen
#################################################################################
build_debian ()
{
    cd $WORK
    if [ ! -f ".dependency_checked" ] ; then
	    #
	    # Check Dependencies
	    #
	    MISSING=""
	    echo "INFO: Checking dependency packages..."
	    check_debian_package "libpcre3-dev"
	    check_debian_package "libjpeg-dev"
	    check_debian_package "libssl-dev"
	    check_debian_package "libcurl4-openssl-dev"
	    check_debian_package "libmcrypt-dev"
	    check_debian_package "libpng12-dev"
	    check_debian_package "libbz2-dev"
	    check_debian_package "zlib1g-dev"
	    check_debian_package "libqt4-dev"
	    check_debian_package "qt4-dev-tools"
	    check_debian_package "qt4-qmake"
	    check_debian_package "nasm"
	    check_debian_package "subversion"
	    check_debian_package "dpkg-dev"
	    
	    if [ "$MISSING" ] ; then
	        echo "ERROR: Missing packages, please run the following command:"
	        echo ""
	        echo "   sudo apt-get install $MISSING"
	        echo ""
	        exit 1
	    fi
	    touch .dependency_checked
    fi
	echo "INFO: all required packages are installed"

	# CONFIGURE-Variable füllen
    ppl6_linux_configure
    
    build_ppl6 $WORK
    build_winmusik $WORK
    cd $WORK

    echo "INFO: Build Debian-Packet for $DISTRIB_ID $DISTRIB_RELEASE: $DISTRIB_CODENAME"
    DISTNAME="$PROGNAME-$VERSION"
    rm -rf debian
    create_dir debian
    create_dir debian/DEBIAN
    create_dir debian/usr/bin
    create_dir debian/usr/share/applications
    create_dir debian/usr/share/pixmaps
    cp bin/$PROGNAME debian/usr/bin/$PROGNAME
    cp WinMusik/resources/icon64.png debian/usr/share/pixmaps/$PROGNAME.png

    DEPENDS=""
    write_desktop_application "/usr" debian/usr/share/applications/$PROGNAME.desktop
        (
                echo "$PROGNAME ($VERSION) unstable; urgency=low"
                echo ""
                echo " * No upstream changes."
                echo ""
                echo -n "-- $USER   "
                date 
                echo ""
                echo "Local variables:"
            echo "mode: debian-changelog"
            echo "End:"
        ) > debian/changelog

    debian_write_control
    dpkg-shlibdeps debian/usr/bin/$PROGNAME
        DEPENDS=`grep "shlibs:Depends" debian/substvars | sed -e "s/shlibs:Depends=//"`
        debian_write_control
    cp debian/control debian/DEBIAN
        rm debian/control
        rm debian/substvars
        rm debian/changelog

    dpkg -b debian $DISTFILES/$DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb
        if [ $? -ne 0 ] ; then
                echo "ERROR: Fehler beim Erstellen des Pakets"
                exit
        fi
    if [ -n "$TARGETPATH" ] ; then
        if [ -d "$TARGETPATH" ] ; then
            cp $DISTFILES/$DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb $TARGETPATH
        fi
    fi
}

#################################################################################
# Windows-Installer bauen
#################################################################################
build_mingw32()
{
    cd $WORK
    echo "*******************************************************"
    echo "Baue PPL6 und WinMusik unter Windows"
    export CPPFLAGS="-DCURL_STATICLIB -I/usr/local/include -I/sdk/WindowsSDK/include"
    export LDLAGS="-DCURL_STATICLIB -L/usr/local/lib -L/sdk/WindowsSDK/lib"
    export CFLAGS="-DCURL_STATICLIB"
    
    CONFIGURE="--with-pcre=/usr/local --with-bzip2=/usr/local --with-zlib=/usr/local"
    CONFIGURE="$CONFIGURE --with-nasm --with-libiconv-prefix=/usr/local --without-postgresql "
    CONFIGURE="$CONFIGURE --with-png=/usr/local --with-jpeg=/usr/local --without-lame --without-ft-prefix"
    CONFIGURE="$CONFIGURE --without-libmad --without-lame --without-x --without-mysql --without-freetds"
    CONFIGURE="$CONFIGURE --with-libcurl --with-sdl-prefix=$WORK --without-libmhash"
    CONFIGURE="$CONFIGURE --with-libmcrypt-prefix --without-libldns --without-ogg"
    
    echo "INCLUDEPATH += $WORK/include" >> $WORK/WinMusik/WinMusik.pro
    echo "QMAKE_LIBDIR += $WORK/lib" >> $WORK/WinMusik/WinMusik.pro
    
    
    build_ppl6 $WORK
    build_winmusik $WORK
    cd $WORK
        
    echo "*******************************************************"
    echo "Erstelle Windows-Setup Programm"
    cd $WORK/WinMusik
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
    cd $WORK/WinMusik
    cp distfiles/$NAME-$VERSION-Win32Setup.exe $DISTFILES
    if [ -n "$TARGETPATH" ] ; then
        if [ -d "$TARGETPATH" ] ; then
            cp distfiles/$NAME-$VERSION-Win32Setup.exe $TARGETPATH
        fi
    fi
}

#################################################################################
# FreeBSD-Paket bauen
#################################################################################
build_freebsd ()
{
	CONFIGURE="--with-pcre=/usr/local --with-libiconv-prefix=/usr/local --with-nasm --with-jpeg --with-png --with-libtiff=/usr/local"
	cd $WORK 
    build_ppl7 $WORK
	cd $WORK
	echo -n "Current Dir="
	pwd
	sleep 2
	build_osm $WORK
	cd $WORK
	echo "INFO: Build FreeBSD-Packet for $DISTRIB_ID $DISTRIB_RELEASE"
	DISTNAME="$PROGNAME-$VERSION-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM"
	create_dir package
	create_dir package/bin
	create_dir package/share/applications
	create_dir package/share/pixmaps
	
		(
                echo "[Desktop Entry]"
                echo "Encoding=UTF-8"
                echo "Name=$PROGNAME"
                echo "Comment=$DESCRIPTION"
                echo "Exec=/usr/local/bin/$PROGNAME"
                echo "Terminal=false"
                echo "Type=Application"
                echo "Categories=GTK;GNOME;AudioVideo;"
                echo "Icon=/usr/local/share/pixmaps/$PROGNAME.png"
        ) > package/share/applications/$PROGNAME.desktop
	cp osm/resources/icon256x256.png package/share/pixmaps/$PROGNAME.png
	cp bin/$PROGNAME package/bin
	(
		echo "@cwd /usr/local"
		echo "@srcdir $WORK/package"
		freebsd_dep pcre
		freebsd_dep png
		freebsd_dep jpeg
		freebsd_dep libiconv
		freebsd_dep qt4-gui
		freebsd_dep qt4-corelib
		echo "share/applications/$PROGNAME.desktop"
		echo "share/pixmaps/$PROGNAME.png"
		echo "bin/$PROGNAME"
	) > pkg_list
	
	pkg_create -v -d ../README.TXT -p package -f pkg_list -c "-$DESCRIPTION" $PROGNAME-$VERSION
	cp $PROGNAME-$VERSION.tbz $DISTFILES/$DISTNAME.tbz	
}

write_source_specfile() {
    FILE=$1
    BUILDREQUIRES=$2
    COMMENT=`cat $WORK/WinMusik/README_en.TXT| grep -v "^===.*" `
    # CONFIGURE-Variable füllen
    ppl6_linux_configure
    
    (
    echo "Name:     $PROGNAME"
    echo "Version:  $VERSION"
    echo "Release:  $REVISION"
    echo "Summary:  $DESCRIPTION"
    echo "Autoreq:  yes"
    echo ""
    echo "Group:    Applications/Multimedia"
    echo "License:  GPL3"
    echo "URL:      $HOMEPAGE"
    echo "Source:   $PROGNAME-%{version}-src.tar.bz2"
    echo "BuildRoot:    %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)"
    echo ""
    echo "BuildRequires:    $BUILDREQUIRES"
    echo "" 
    echo "%description"
    echo "$COMMENT"
    echo ""
    echo "%prep"
    echo "%setup -q -n %{name}-%{version}"
    echo ""
    echo "%build"
    echo "WORK=\`pwd\`"
    echo "mkdir -p bin lib include"
    echo "PATH=\"\$WORK/bin:\$PATH\""
    echo "cd ppl6"
    echo "./configure --prefix=\$WORK $CONFIGURE"
    echo "make -j2 install"
    echo "cd ../WinMusik"
	echo "echo \"INCLUDEPATH += \$WORK/include\" >> WinMusik.pro"
    echo "echo \"QMAKE_LIBDIR += \$WORK/lib\" >> WinMusik.pro"
    echo "$QMAKE"
    echo "make -j2 release"
    echo "cp release/WinMusik \$WORK/bin/$PROGNAME"
    echo ""
    echo "%install"
    echo "rm -rf \$RPM_BUILD_ROOT"
    echo "mkdir -p \$RPM_BUILD_ROOT/usr/bin"
    echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/pixmaps"
    echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/doc/$PROGNAME"
    echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/applications"
    echo "cp bin/$PROGNAME \$RPM_BUILD_ROOT/usr/bin"
    echo "cp WinMusik/resources/icon64.png \$RPM_BUILD_ROOT/usr/share/pixmaps/$PROGNAME.png"
    echo "cp WinMusik/docs/Userguide_de.pdf \$RPM_BUILD_ROOT/usr/share/doc/$PROGNAME"
    echo ""
    echo "# Desktop menu entry"
    echo "cat > \$RPM_BUILD_ROOT/usr/share/applications/$PROGNAME.desktop << EOF"
    echo "[Desktop Entry]"
    echo "Name=$PROGNAME"
    echo "Comment=$DESCRIPTION"
    echo "Exec=/usr/bin/$PROGNAME"
    echo "Icon=/usr/share/pixmaps/$PROGNAME.png"
    echo "Terminal=0"
    echo "Type=Application"
    echo "Categories=Qt;AudioVideo;"
    echo "EOF"
    echo ""
    echo "%clean"
    echo "rm -rf \$RPM_BUILD_ROOT"
    echo ""
    echo "%files"
    echo "%defattr(-,root,root,-)"
    echo "/usr/bin/$PROGNAME"
    echo "/usr/share/pixmaps/$PROGNAME.png"
    echo "/usr/share/applications/$PROGNAME.desktop"
    echo "/usr/share/doc/$PROGNAME/*"
    echo "%doc"
    echo ""
    echo "%changelog"
    echo ""
    ) > $FILE
    
}



#################################################################################

identify_system

#################################################################################
# Sourcen zusammenfassen, sofern wir im Sourceverzeichnis von WinMusik sind
#################################################################################
if [ -f WinMusik.pro ] ; then
	create_dir $WORK
	create_dir $DISTFILES
	cd $WORK
	if [ "$1" = "source" ] ; then
		echo "Erstelle Source-Distribution für WinMusik..."
		rm -rf $PROGNAME-$VERSION
		create_dir "$WORK/$PROGNAME-$VERSION"
		gather_sources "$WORK/$PROGNAME-$VERSION"
		cd $WORK
		cp ../build_package.sh "$PROGNAME-$VERSION"
		cp ../*.TXT "$PROGNAME-$VERSION"
		tar -cjf $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2 --exclude .svn "$PROGNAME-$VERSION"
		if [ -d "$TARGETPATH" ] ; then
			cp $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2 $TARGETPATH
		fi
		if [ "$DISTRIB_ID" = "FreeBSD" ] ; then
			build_FreeBSD_package
		fi
		# Specfiles erstellen
        BUILDREQUIRES="desktop-file-utils, gcc, gcc-c++, libgcc, bzip2-devel, openssl-devel, zlib-devel, libcurl-devel, libjpeg-devel, libpng-devel, nasm, libstdc++-devel, qt-devel, glibc-devel, pcre-devel"
        write_source_specfile "$DISTFILES/$PROGNAME-$VERSION-el6.spec" "$BUILDREQUIRES"
        BUILDREQUIRES="desktop-file-utils, gcc, gcc-c++, libgcc_s1, libbz2-devel, openssl-devel, zlib-devel, libcurl-devel, libjpeg8-devel, libpng15-devel, nasm, libstdc++-devel, libqt4-devel, glibc-devel, pcre-devel"
        write_source_specfile "$DISTFILES/$PROGNAME-$VERSION-suse.spec" "$BUILDREQUIRES"
        if [ -d "$TARGETPATH" ] ; then
        	cp $DISTFILES/$PROGNAME-$VERSION-el6.spec $DISTFILES/$PROGNAME-$VERSION-suse.spec $TARGETPATH
        fi
		
		# RedHat Source-RPM erstellen
		#build_srpm
		exit 0
	fi
	gather_sources "$WORK"
else
	WORK=$MYPWD
	DISTFILES=$MYPWD
fi

#################################################################################
# Binary-Paket bauen
#################################################################################
echo "Baue $PROGNAME $VERSION für: $DISTRIB_ID, $DISTRIB_RELEASE on $PLATFORM..."
echo ""
cd $WORK
PPL6DIR=$WORK/ppl6
WINMUSIKDIR=$WORK/WinMusik

if [ "$DISTRIB_ID" = "Ubuntu" ] ; then
	build_debian
elif [ "$DISTRIB_ID" = "Debian" ] ; then
	build_debian
elif [ "$DISTRIB_ID" = "FreeBSD" ] ; then
	build_freebsd
elif [ "$DISTRIB_ID" = "MINGW32" ] ; then
    build_mingw32 $1
elif [ "$DISTRIB_ID" = "CentOS" ] ; then
	build_redhat $1
elif [ "$DISTRIB_ID" = "Fedora" ] ; then
	build_redhat $1
elif [ "$DISTRIB_ID" = "RedHat" ] ; then
	build_redhat $1
elif [ "$DISTRIB_ID" = "openSUSE" ] ; then
	build_suse $1
else
	echo "ERROR: no automated build for this system"
	echo "INFO: DISTRIB_ID=$DISTRIB_ID"
	exit 1
fi

exit 0
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################
##############################################################################################



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
	echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/pixmaps"
	echo "mkdir -p \$RPM_BUILD_ROOT/usr/share/applications"
	echo "cp build/bin/WinMusik \$RPM_BUILD_ROOT/usr/bin"
	echo "cp build/src/winmusik/resources/icon48.png \$RPM_BUILD_ROOT/usr/share/pixmaps/WinMusik.png"
	echo ""
	echo "# Desktop menu entry"
	echo "cat > %{name}.desktop << EOF"
	echo "[Desktop Entry]"
	echo "Name=$NAME"
	echo "Comment=$DESCRIPTION"
	echo "Exec=WinMusik"
	echo "Icon=WinMusik.png"
	echo "Terminal=0"
	echo "Type=Application"
	echo "Categories=Qt;AudioVideo;Database"
	echo "EOF"
	echo ""
	echo "desktop-file-install --vendor \"Patrick F.-Productions\" \\"
  	echo "  --dir \$RPM_BUILD_ROOT/usr/share/applications \\"
  	echo "  --add-category Qt \\"
  	echo "  --add-category AudioVideo \\"
  	echo "  --add-category Database \\"
  	echo "%{name}.desktop"
  	echo ""
	echo "%clean"
	echo "rm -rf \$RPM_BUILD_ROOT"
	echo ""
	echo "%files"
	echo "%defattr(-,root,root,-)"
	echo "/usr/bin/*"
	echo "/usr/share/pixmaps/*"
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
