###############################################################################
###
### WinMusik Configure
###
###############################################################################
#
# This file is part of WinMusik 3 by Patrick Fedick
#
# $Author: pafe $
# $Revision: 404 $
# $Date: 2014-05-28 19:15:08 +0200 (Mi, 28 Mai 2014) $
# $Id: build_package.sh 404 2014-05-28 17:15:08Z pafe $
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
###############################################################################

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.63)
AC_INIT([winmusik], [3.3.0])
AC_CONFIG_AUX_DIR([.])
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([WinMusik.desktop])

AC_PROG_CC
AC_PROG_CXX

case "$host_os" in
	freebsd* )
		CFLAGS="-I/usr/local/include $CFLAGS"
		LDFLAGS="-L/usr/local/lib $LDFLAGS"
  		;;
esac

dnl AC_MSG_RESULT([CFLAGS=$CFLAGS, host_os=$host_os])

AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([sys/types.h])
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([math.h])

# find qmake
AC_MSG_CHECKING([for qmake])
AC_ARG_WITH([qmake],
	[  --with-qmake=PATH       Path and name of the qmake program (optional)],
	[QMAKE="$withval"],
	[QMAKE="no"])

if test "$QMAKE" = "no" -o -n "$QMAKE" ; then
	AC_CHECK_PROGS(qmake,[qmake-qt4 qmake],no)
	if test "$qmake" = "no" ; then
		AC_MSG_ERROR(qmake not found)
	fi
	QMAKE=$qmake
else
	if test ! -x "$QMAKE"  ; then
		AC_MSG_ERROR(qmake not found)
	fi
	AC_MSG_RESULT($QMAKE)
fi
AC_SUBST(QMAKE)

AC_CONFIG_SUBDIRS([ppl6])


AC_OUTPUT

AC_MSG_RESULT([
******************************************************************************
** Configuration summary for $PACKAGE_NAME Version $PACKAGE_VERSION
** Target: $target_os        CPU: $target_cpu
**
** qmake:             $QMAKE
])

