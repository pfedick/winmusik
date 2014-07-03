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
 

PROGNAME="WinMusik"
VERSION=${VERSION:=trunk}
SVN_REVISION=${SVN_REVISION:=0}
REVISION="1"
DESCRIPTION="Music database to maintain songs on various devices"
PACKAGENAME="WinMusik"
HOMEPAGE="http://www.winmusik.de/"
MAINTAINER="Patrick Fedick <patrick@pfp.de>"


PPL6SOURCE=${PPL6SOURCE:="../ppl6"}
PPL6REPO="https://svn.code.sf.net/p/pplib/code/lib/branches/RELENG_6"
WINMUSIKREPO=https://svn.code.sf.net/p/winmusik/client/trunk
WINMUSIKDIR="$PROGNAME-$VERSION"

TARGETPATH=/ftp/winmusik
QMAKE=qmake-qt4
MAKE=make



DISTFILES=$CUR/distfiles
WORK=$CUR/tmp
WINMUSIKSOURCE=$CUR


create_dir ()
{
	mkdir -p $1
	if [ $? -ne 0 ] ; then
		echo "ERROR: could not create: $1"
		exit 1
	fi
}

#
# Identify System
#
if [ -f /etc/lsb-release ] ; then
	. /etc/lsb-release
elif [ -f /etc/system-release ] ; then
	DISTRIB_ID=`cat /etc/system-release | awk '{print $1}'`
	DISTRIB_RELEASE=`cat /etc/system-release | awk '{print $3}'`
elif [ -f /etc/os-release ] ; then
    DISTRIB_ID=`grep "^ID=" /etc/os-release | sed -e "s/ID=//g"`
    DISTRIB_RELEASE=`grep "^VERSION_ID=" /etc/os-release | sed -e "s/VERSION_ID=//g" | sed -e "s/\"//g"`
    if [ "$DISTRIB_ID" = "debian" ] ; then
        DISTRIB_ID="Debian"
    fi	
elif [ -f /etc/SuSE-release ] ; then
	DISTRIB_ID=`head -n1 /etc/SuSE-release | awk '{print $1}'`
	DISTRIB_RELEASE=`head -n1 /etc/SuSE-release | awk '{print $2}'`
	QMAKE=qmake
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

ARCH=`uname -m`;
PLATFORM="i386";
if [ "$ARCH" = "x86_64" ] || [ "$ARCH" = "amd64" ] ; then
        PLATFORM="amd64"
fi





