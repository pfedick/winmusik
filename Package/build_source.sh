#!/bin/sh
###############################################################################
# This file is part of WinMusik 3 by Patrick Fedick                           #
# Web: https://www.pfp.de/winmusik/                                           #
###############################################################################
#
# Copyright (c) 2019, Patrick Fedick <patrick@pfp.de>
# All rights reserved.
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
###############################################################################
 
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



gather_sources()
{
	TARGET=$1
	cd $CUR
	WMDIR=$TARGET
	if [ -d "$WINMUSIKSOURCE" ] ; then
		echo "INFO: Copy WinMusik-sources from local directory: $WINMUSIKSOURCE..."
		echo "INFO: Ziel: $WMDIR"
		create_dir "$WMDIR"
		cd $WINMUSIKSOURCE
		find *.TXT *.md configure configure.ac *.m4 autoconf WinMusik.pro.in \
			*.qm *.ts include Doxyfile resource.rc resources resources.qrc src \
			widgets forms docs pplib| cpio -pdm "$WMDIR" > /dev/null 2>&1
		echo "INFO: done"
    else
    	echo "ERROR: Winmusik Sources not found!"
    	exit 1
	fi
	(
		echo "#ifndef WINMUSIK_VERSION_H_"
		echo "#define WINMUSIK_VERSION_H_"
		echo "#define WM_VERSION		\"$VERSION\""
		echo "#define WM_SVN_REVISION		\"$SVN_REVISION\""
		BUILDDATE=`date '+%Y%m%d'`
		echo "#define WM_RELEASEDATE		$BUILDDATE"
		echo "#endif /* WINMUSIK_VERSION_H_ */"
	) > $WMDIR/include/version.h
	
	cd $CUR
}

echo "Building source package for $PROGNAME $VERSION..."
echo ""

rm -rf $WORK

create_dir $WORK
create_dir $DISTFILES
create_dir "$WORK/$PROGNAME-$VERSION"
gather_sources "$WORK/$PROGNAME-$VERSION"

#create_build_scripts

echo "INFO: create and compress tar file..."

cd $WORK
tar -cjf $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2 --exclude .gitignore "$PROGNAME-$VERSION"
if [ -d "$TARGETPATH" ] ; then
	cp $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2 $TARGETPATH
fi

echo ""
echo "done: $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2"
		
