##############################################################################
# This file is part of WinMusik 4 by Patrick Fedick
#
# Copyright (c) 2018 by Patrick Fedick
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
##############################################################################

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.69)
AC_INIT([winmusik], [4.0.0], [patrick@pfp.de], [winmusik-4.0.0.tar.bz2])
AC_CONFIG_AUX_DIR(autoconf)

AC_CONFIG_SRCDIR([include/config.h.in])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_HEADERS([include/config.h])


###############################################################################
# Includes
###############################################################################
sinclude(autoconf/gtest.m4)
sinclude(autoconf/ppl7.m4)


AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

OLD_CFLAGS=$CFLAGS
# Checks for programs.
AC_PROG_CC
CFLAGS=$OLD_CFLAGS
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX(11) 
AX_CXX_COMPILE_STDCXX(14)
AX_CXX_COMPILE_STDCXX(17)

AC_LANG(C++)

case "$host_os" in
	freebsd* )
		CFLAGS="-I/usr/local/include $CFLAGS"
		LDFLAGS="-L/usr/local/lib $LDFLAGS"
  		;;
esac
 
PKG_PROG_PKG_CONFIG()

#--------------------------------------------------------------------
#       Check for various typedefs and provide substitutes if
#       they don't exist.
#--------------------------------------------------------------------

AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UID_T
AC_TYPE_OFF_T

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

report_have_gtest="no"
AC_CHECK_GTEST(
	report_have_gtest="yes"
	,
	report_have_gtest="no"
	report_codecoverage_enabled="no"
)

AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([locale.h])

AC_FUNC_MKTIME
AC_CHECK_TYPE(size_t, unsigned)
AC_CHECK_TYPE(wchar_t, unsigned)

AC_CHECK_FUNCS([malloc free memmove memset])


AX_PATH_LIB_PPL7(7.0.0,
	AC_DEFINE(HAVE_PPL7,1)
	,
	AC_MSG_ERROR(ppl7 not found)
)

AC_CHECK_GTEST(
	AC_DEFINE(HAVE_GTEST,1)
	,
)

AX_PPL7_FEATURE(zlib,
	AC_DEFINE(HAVE_PPL7_ZLIB,1)
	,
)

AX_PPL7_FEATURE(bzip2,
	AC_DEFINE(HAVE_PPL7_BZIP2,1)
	,
)

AX_PPL7_FEATURE(pcre,
	AC_DEFINE(HAVE_PPL7_PCRE,1)
	,
)

AX_PPL7_FEATURE(openssl,
	AC_DEFINE(HAVE_PPL7_OPENSSL,1)
	,
)

AX_PPL7_FEATURE(mhash,
	AC_DEFINE(HAVE_PPL7_MHASH,1)
	,
)

AX_PPL7_FEATURE(mcrypt,
	AC_DEFINE(HAVE_PPL7_MCRYPT,1)
	,
)

AX_PPL7_FEATURE(iconv,
	AC_DEFINE(HAVE_PPL7_ICONV,1)
	,
)

AX_PPL7_FEATURE(mpg123,
	AC_DEFINE(HAVE_PPL7_MPG123,1)
	,
)

AX_PPL7_FEATURE(lame,
	AC_DEFINE(HAVE_PPL7_LAME,1)
	,
)


AC_SUBST(LIBPPL7_RELEASE_LIBS)
AC_SUBST(LIBPPL7_DEBUG_LIBS)
AC_SUBST(LIBPPL7_CFLAGS)
AC_SUBST(LIBPPL7)
AC_SUBST(GTEST_PREFIX)
AC_SUBST(HAVE_PPL7)
AC_SUBST(HAVE_GTEST)
AC_SUBST(includedir)
AC_SUBST(libdir)



###############################################################################
# Output
###############################################################################

AC_OUTPUT


