#!/bin/sh
#
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


WORK=$CUR/tmp
DISTFILES=$CUR/distfiles

SOURCES="$DISTFILES/$PROGNAME-$VERSION-src.tar.bz2"

if [ `uname` != "FreeBSD" ] ; then
	echo "ERROR: can be build on FreeBSD only!"
	exit 0
fi

if [ ! -f "$SOURCES" ] ; then
	echo "ERROR: No source release found, expecting: $SOURCES"
	exit 0
fi

rm -rf $WORK/FreeBSD
create_dir $WORK/FreeBSD/winmusik
cd $CUR/Package/FreeBSD
cp winmusik/* $WORK/FreeBSD/winmusik
cd $DISTFILES
sha256 $PROGNAME-$VERSION-src.tar.bz2 > $WORK/FreeBSD/winmusik/distinfo
echo "SIZE ($PROGNAME-$VERSION-src.tar.bz2) = `stat -f \"%z\" $PROGNAME-$VERSION-src.tar.bz2`" >> $WORK/FreeBSD/winmusik/distinfo
cat $CUR/Package/FreeBSD/winmusik/Makefile | sed "s/^PORTVERSION=.*$/PORTVERSION=		$VERSION/" > $WORK/FreeBSD/winmusik/Makefile
cd $WORK/FreeBSD/winmusik; make clean;
cd $WORK/FreeBSD
shar `find winmusik | grep -v ".svn" `> $DISTFILES/$PROGNAME-$VERSION-FreeBSD-Port.shar

