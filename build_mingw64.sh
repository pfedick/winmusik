#!/bin/sh
JENKINS_VERSION="/jenkins/workspace/win64_WinMusik/gui/include/version.h"
BUILDDATE=`date '+%Y%m%d'`
VERSION=${VERSION:=integration}
mkdir -p tmp
(
	echo "#ifndef VERSION_H_"
	echo "#define VERSION_H_"
	echo "#define WM_VERSION	\"$VERSION\""
	echo "#define WM_SVN_REVISION	\"$BUILD_NUMBER\""
	echo "#define WM_RELEASEDATE	$BUILDDATE"
	echo "#endif /* VERSION_H_ */"
) > tmp/version.h

if [ -f "$JENKINS_VERSION" ]
then
	sed "s/WM_RELEASEDATE.*$/WM_RELEASEDATE $BUILDDATE/" $JENKINS_VERSION > tmp/version.h
fi

diff tmp/version.h gui/include/version.h > /dev/null 2>&1
if [ $? -ne 0 ] ; then
	cp tmp/version.h include/version.h
fi

cat include/version.h

mingw32-make -j8 release
rm -rf release/deploy
windeployqt.exe --dir release/deploy release/WinMusik.exe

