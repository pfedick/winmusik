#!/bin/sh
###############################################################################
# This file is part of WinMusik 3 by Patrick Fedick                           #
# Web: https://www.pfp.de/winmusik/                                           #
# Sourcecode: https://github.com/pfedick/winmusik
###############################################################################
#
# Copyright (c) 2022, Patrick Fedick <patrick@pfp.de>
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
