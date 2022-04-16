#!/bin/sh
BUILDDATE=`date '+%Y%m%d'`
VERSION=${VERSION:=integration}
BUILD_NUMBER=`git rev-list --all --count`
mkdir -p tmp
(
	echo "#ifndef VERSION_H_"
	echo "#define VERSION_H_"
	echo "#define WM_VERSION	\"$VERSION\""
	echo "#define WM_REVISION	\"$BUILD_NUMBER\""
	echo "#define WM_RELEASEDATE	$BUILDDATE"
	echo "#endif /* VERSION_H_ */"
) > tmp/version.h

diff tmp/version.h gui/include/version.h > /dev/null 2>&1
if [ $? -ne 0 ] ; then
	cp tmp/version.h gui/include/version.h
	cat gui/include/version.h
fi
