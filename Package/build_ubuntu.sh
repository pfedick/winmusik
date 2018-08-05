#!/bin/sh
#
# This file is part of WinMusik 3 by Patrick Fedick
#
# $Author: pafe $
# $Revision: 404 $
# $Date: 2014-05-28 19:15:08 +0200 (Mi, 28 Mai 2014) $
# $Id: build_package.sh 404 2014-05-28 17:15:08Z pafe $
#
#
# Copyright (c) 2014 Patrick Fedick <patrick@pfp.de>
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

CUR=`pwd`

if [ -f "WinMusik.pro" ] ; then
    CUR=`pwd`
elif [ -f "build_source.sh" ] ; then
    cd ..
    CUR=`pwd`
else
    echo "ERROR: Wrong directory, did not found sources"
    exit 1
fi
. $CUR/Package/config.sh



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
        cat ../README_en.TXT | while read line
        do
                if [ -z "$line" ] ; then
                    echo " ."
                else
                    echo "  $line"
                fi
        done
        ) > debian/control
}

ubuntu_check_dependencies ()
{
    if [ ! -f ".dependency_checked" ] ; then
            #
            # Check Dependencies
            #
            MISSING=""
            echo "INFO: Checking dependency packages..."
            check_debian_package "libpcre3-dev"
            check_debian_package "libjpeg8-dev"
            check_debian_package "libssl-dev"
            check_debian_package "libcurl4-openssl-dev"
            check_debian_package "libmcrypt-dev"
            check_debian_package "libpng-dev"
            check_debian_package "libbz2-dev"
            check_debian_package "zlib1g-dev"
            check_debian_package "libcdio-dev"
            check_debian_package "qt5-default"
            check_debian_package "qt5-qmake"
            check_debian_package "nasm"
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
}


ubuntu_check_dependencies

echo "INFO: Build Debian-Packet for $DISTRIB_ID $DISTRIB_RELEASE: $DISTRIB_CODENAME"
DISTNAME="$PROGNAME-$VERSION"
rm -rf debian
create_dir tmp/debian
create_dir tmp/debian/DEBIAN
create_dir tmp/debian/usr/bin
create_dir tmp/debian/usr/share/applications
create_dir tmp/debian/usr/share/pixmaps
create_dir tmp/debian/usr/share/doc/$PROGNAME
chmod -R 755 tmp/debian
cp release/$PROGNAME tmp/debian/usr/bin/$PROGNAME
cp resources/icon64.png tmp/debian/usr/share/pixmaps/$PROGNAME.png
cp docs/Userguide_de.pdf tmp/debian/usr/share/doc/$PROGNAME

DEPENDS=""
cat Package/WinMusik.desktop.in | sed "s/Exec=.*/Exec=\/usr\/bin\/$PROGNAME/" | sed "s/Icon=.*/Icon=\/usr\/share\/pixmaps\/$PROGNAME.png/" > tmp/debian/usr/share/applications/$PROGNAME.desktop

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
) > tmp/debian/changelog


cd tmp

debian_write_control
dpkg-shlibdeps debian/usr/bin/$PROGNAME
DEPENDS=`grep "shlibs:Depends" debian/substvars | sed -e "s/shlibs:Depends=//"`
debian_write_control
cp debian/control debian/DEBIAN
rm -rf debian/control
rm -rf debian/substvars
rm -rf debian/changelog
dpkg -b debian $DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb
if [ $? -ne 0 ] ; then
	echo "ERROR: Could not create debian package"
	exit 1
fi
echo "DONE. Debian package successfully created: $DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb"
echo ""
echo "Install with sudo dpkg -i $DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb"

