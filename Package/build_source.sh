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
		# ohne Tests: tests/gcovr tests/ppl6-tests.h tests/src tests/testdata 
		find *.m4 autoconf config configure configure.in docs Doxyfile HISTORY.TXT include LICENSE.TXT Makefile.in ppl6-config.in README.TXT TODO.TXT resource tools src tests/Makefile.in | cpio -pdm "$TARGET/ppl6" > /dev/null 2>&1
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
	WMDIR=$TARGET/winmusik
	if [ -d "$WINMUSIKSOURCE" ] ; then
		echo "INFO: Copy WinMusik-sources from local directory: $WINMUSIKSOURCE..."
		echo "INFO: Ziel: $TARGET/winmusik"
		create_dir "$TARGET/winmusik"
		cd $WINMUSIKSOURCE
		find *.TXT WinMusik.pro *.qm *.ts setup.iss include Doxyfile resource.rc resources resources.qrc src widgets forms docs| cpio -pdm "$TARGET/winmusik" > /dev/null 2>&1
		echo "INFO: done"
    else
		echo "INFO: checkout WinMusik-sources from svn repository..."
                svn checkout $WINMUSIKREPO $TARGET/winmusik
                if [ $? -ne 0 ] ; then
                        echo "ERROR: checkout failed"
                        exit 1
                fi
                echo "INFO: done"
	fi
	cat $TARGET/winmusik/WinMusik.pro | sed "s!\`ppl6-config!-L../tmp/lib \`../tmp/bin/ppl6-config!" > $TARGET/winmusik/WinMusik.pro.tmp
	mv $TARGET/winmusik/WinMusik.pro.tmp $TARGET/winmusik/WinMusik.pro
	(
		echo "#ifndef WINMUSIK_VERSION_H_"
		echo "#define WINMUSIK_VERSION_H_"
		echo "#define WM_VERSION		\"$VERSION\""
		echo "#define WM_SVN_REVISION		\"$SVN_REVISION\""
		BUILDDATE=`date '+%Y%m%d'`
		echo "#define WM_RELEASEDATE		$BUILDDATE"
		echo "#endif /* WINMUSIK_VERSION_H_ */"
	) > $TARGET/winmusik/include/version.h
	
	cd $CUR
}

create_configure()
{
	TARGET=$1
	cd $WORK/$PROGNAME-$VERSION
	cp $CUR/Package/configure.ac configure.ac
	create_dir autoconf
	cp $CUR/Package/autoconf/* autoconf
	aclocal
	automake --add-missing --copy
	autoconf
	
}

create_build_scripts()
{
	cd $CUR
	(
		echo "#!/bin/sh"
		echo "VERSION=$VERSION"
		echo "SVN_REVISION=$SVN_REVISION"
		echo ""
		cat $CUR/Package/config.sh
	) > $WORK/build_pre.sh
	(
		cat $WORK/build_pre.sh
		cat $CUR/Package/build_ubuntu.sh
	) > $WORK/$PROGNAME-$VERSION/build_ubuntu.sh
	chmod 755 $WORK/$PROGNAME-$VERSION/build_ubuntu.sh
	
}

echo "Building source package for $PROGNAME $VERSION..."
echo ""

rm -rf $WORK

create_dir $WORK
create_dir $DISTFILES
create_dir "$WORK/$PROGNAME-$VERSION"
gather_sources "$WORK/$PROGNAME-$VERSION"
create_configure "$WORK/$PROGNAME-$VERSION"
create_build_scripts

cd $CUR
cp Package/Makefile.in $WORK/$PROGNAME-$VERSION
cp Package/WinMusik.desktop.in $WORK/$PROGNAME-$VERSION

cp *.TXT $WORK/$PROGNAME-$VERSION

cd $WORK
tar -cjf $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2 --exclude .svn "$PROGNAME-$VERSION"
if [ -d "$TARGETPATH" ] ; then
	cp $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2 $TARGETPATH
fi

echo ""
echo "done: $DISTFILES/$PROGNAME-$VERSION-src.tar.bz2"
		
