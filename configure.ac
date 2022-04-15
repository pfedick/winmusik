###############################################################################
# This file is part of WinMusik 3 by Patrick Fedick                           #
# Web: https://www.pfp.de/winmusik/                                           #
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

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.69)
AC_INIT([winmusik], [3.7.0], [patrick@pfp.de], [winmusik-3.7.0.tar.bz2])
AC_CONFIG_AUX_DIR(autoconf)

AC_CONFIG_SRCDIR([gui/include/winmusik3.h])
AC_CONFIG_SRCDIR([ppl6/include/ppl6.h])
AC_CONFIG_SRCDIR([ppl6/include/config.h.in])
AC_CONFIG_SRCDIR([pplib/include/ppl7.h])
AC_CONFIG_SRCDIR([pplib/include/config_ppl7.h.in])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([ppl6/Makefile])
AC_CONFIG_HEADERS([ppl6/include/config.h])
AC_CONFIG_FILES([pplib/Makefile])
AC_CONFIG_HEADERS([pplib/include/ppl7-config.h])
AC_CONFIG_HEADERS([pplib/include/config_ppl7.h])
AC_CONFIG_FILES([gui/WinMusik.pro])
AC_CONFIG_FILES([Package/WinMusik.desktop])


###############################################################################
# Includes
###############################################################################

sinclude(pplib/autoconf/ax_cxx_compile_stdcxx.m4)
sinclude(pplib/autoconf/ax_path_lib_pcre.m4)
sinclude(pplib/autoconf/ax_pthread.m4)
sinclude(pplib/autoconf/checkfuncs.m4)
sinclude(pplib/autoconf/ax_gcc_x86_cpuid.m4)
sinclude(pplib/autoconf/ax_check_compile_flag.m4)
sinclude(pplib/autoconf/ax_check_compiler_flags.m4)
sinclude(pplib/autoconf/ax_gcc_archflag.m4)
sinclude(pplib/autoconf/lib-link.m4)
sinclude(pplib/autoconf/ax_check_openssl.m4)
sinclude(pplib/autoconf/libcurl.m4)
sinclude(pplib/autoconf/iconv.m4)
sinclude(pplib/autoconf/bzip2.m4)
sinclude(pplib/autoconf/ax_check_zlib.m4)
sinclude(pplib/autoconf/ax_lib_crypto.m4)
sinclude(pplib/autoconf/gtest.m4)

sinclude(autoconf/libcdio.m4)
sinclude(autoconf/ax_find_qmake.m4)


#sinclude(autoconf/ax_have_qt.m4)

report_have_pthreads="no"
report_have_iconv="no"
report_have_bzip2="no"
report_have_zlib="no"
report_have_pcre="no"
report_have_openssl="no"

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

OLD_CFLAGS=$CFLAGS
# Checks for programs.
CFLAGS=$OLD_CFLAGS
AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)
AC_EXEEXT


AX_CXX_COMPILE_STDCXX(11) 

AX_CHECK_COMPILE_FLAG([-no-pie],
    LDFLAGS="-no-pie $LDFLAGS"
    NOPIE="-no-pie",
    NOPIE=""
)
AC_SUBST(NOPIE)


  
case "$host_os" in
	freebsd* )
		CFLAGS="-I/usr/local/include $CFLAGS"
		LDFLAGS="-L/usr/local/lib $LDFLAGS"
  		;;
esac
 
case "$host_os" in
	*mingw32*|msys )
		report_have_pthreads="yes, Windows Threads"
  		;;
  	*mingw64* )
		report_have_pthreads="yes, Windows Threads"
  		;;
	*)
		AX_PTHREAD([report_have_pthreads="yes, pthreads"]
			AC_DEFINE(HAVE_PTHREADS,1, [Define to 1 if we have pthreads])			
		)
		;;
esac

AC_C_BIGENDIAN(
	report_byteorder="BIG ENDIAN"
	AC_DEFINE(__BIG_ENDIAN__),
	report_byteorder="LITTLE ENDIAN"
	AC_DEFINE(__LITTLE_ENDIAN__))

AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(wchar_t)
AC_CHECK_SIZEOF(size_t)
AC_CHECK_SIZEOF(float)
AC_CHECK_SIZEOF(double)
AC_CHECK_SIZEOF(void*)

AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINTPTR_T


if test $ac_cv_sizeof_wchar_t -eq 4; then
	if test "$report_byteorder" = "LITTLE ENDIAN"; then
		AC_DEFINE(ICONV_UNICODE,"UTF-32LE")
	else
		AC_DEFINE(ICONV_UNICODE,"UTF-32BE")
	fi
else
	if test "$report_byteorder" = "LITTLE ENDIAN"; then
		AC_DEFINE(ICONV_UNICODE,"UTF-16LE")
	else
		AC_DEFINE(ICONV_UNICODE,"UTF-16BE")
	fi
fi

if test $ac_cv_sizeof_char -eq 1; then
	AC_DEFINE(ppluint8,unsigned char)
	AC_DEFINE(pplint8,signed char)
fi
if test $ac_cv_sizeof_short -eq 2; then
	AC_DEFINE(ppluint16,unsigned short)
	AC_DEFINE(pplint16,signed short)
fi

if test $ac_cv_sizeof_int -eq 4; then
	AC_DEFINE(ppluint32,unsigned int)
	AC_DEFINE(pplint32,signed int)
fi

if test $ac_cv_sizeof_long_long -eq 8; then
		AC_DEFINE(ppluint64,unsigned long long)
		AC_DEFINE(pplint64,signed long long)
else
	if test $ac_cv_sizeof_long -eq 8; then
		AC_DEFINE(ppluint64,unsigned long)
		AC_DEFINE(pplint64,signed long)
	fi
fi

AC_CHECK_SIZEOF(float)
AC_CHECK_SIZEOF(double)
AC_CHECK_SIZEOF(void*)
if test $ac_cv_sizeof_voidp -eq 4; then
	AC_DEFINE(ppliptr,ppluint32)
elif test $ac_cv_sizeof_voidp -eq 8; then
	AC_DEFINE(ppliptr,ppluint64)
else
	AC_MSG_ERROR(size of void* is not supported)
fi

AC_CHECK_SIZEOF(fpos_t)


AC_SYS_LARGEFILE
AC_FUNC_FSEEKO

# AX_HAVE_QT scheint unter FreeBSD nicht kompatibel zu sein. Der produzierte Code
# läuft zwar unter FreeBSD, aber nicht unter Linux oder MinGW 
dnl AX_HAVE_QT

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
AC_C_CONST
AC_STRUCT_TM
AX_FPOS_ISSTRUCT()
AX_TM_HAS_GMTOFF()
AC_PATH_XTRA




report_have_gtest="no"
AC_CHECK_GTEST(
	report_have_gtest="yes"
	,
	report_have_gtest="no"
	report_codecoverage_enabled="no"
)


AC_CHECK_HEADERS([stdlib.h stdint.h inttypes.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([math.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([sys/file.h])
AC_CHECK_HEADERS([time.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/types.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([sys/poll.h])
AC_CHECK_HEADERS([netinet/in.h])
AC_CHECK_HEADERS([arpa/nameser.h])
AC_CHECK_HEADERS([resolv.h],,,[
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <arpa/nameser.h>
])
AC_CHECK_HEADERS([netdb.h])
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([signal.h])
AC_CHECK_HEADERS([dirent.h])
AC_CHECK_HEADERS([fnmatch.h])
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([ctype.h])
AC_CHECK_HEADERS([wctype.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([wchar.h])
AC_CHECK_HEADERS([widec.h])
AC_CHECK_HEADERS([syslog.h])
AC_CHECK_HEADERS([sys/mman.h])
AC_CHECK_HEADERS([valgrind/helgrind.h])

AC_FUNC_MKTIME
AC_CHECK_TYPE(size_t, unsigned)
AC_CHECK_TYPE(wchar_t, unsigned)

# Checks for library functions.
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([strtolower strtoupper])
AC_CHECK_FUNCS([bzero bcopy gethostbyname gettimeofday inet_ntoa memmove memset mkdir socket __mingw_vasprintf vasprintf asprintf])
AC_CHECK_FUNCS([gethostname sethostname getdomainname setdomainname uname])
AC_CHECK_FUNCS([strcasecmp strlen strnlen strdup strerror strncasecmp strndup strstr strcasestr strcmp strncmp])
AC_CHECK_FUNCS([strncpy strlcpy strncat strlcat])
AC_CHECK_FUNCS([strtok strtok_r])
AC_CHECK_FUNCS([htol atoll atoi atol atof strtoll strtoull])
AC_CHECK_FUNCS([flock fcntl truncate ftruncate sync fsync unlink remove])
AC_CHECK_FUNCS([fgets fputs fgetws fputws fputc fgetc fputwc fgetwc mmap munmap sysconf pagesize])
AC_CHECK_FUNCS([usleep nanosleep localtime localtime_r gmtime gmtime_r])
AC_CHECK_FUNCS([signal syslog openlog closelog])
AC_CHECK_FUNCS([strtok_r])
AC_CHECK_FUNCS([getpid getppid getuid geteuid])
AC_CHECK_FUNCS([mkstemp])
AC_CHECK_FUNCS([wcscmp wcsncmp wcscasecmp wcsncasecmp wcstol wcstoll wcslen wcsstr wcstoul wcstoull wcstod wcstof])
AC_CHECK_FUNCS([wcstombs mbstowcs wcsrtombs wcsnrtombs mbsrtowcs mbsnrtowcs])
AC_CHECK_FUNCS([wprintf fwprintf swprintf vwprintf vfwprintf vswprintf])
AC_CHECK_FUNCS([vsnprintf])
AC_CHECK_FUNCS([wstol wstoll watoi watoll wstod watof])
AC_CHECK_FUNCS([poll select pselect])
AC_CHECK_FUNCS([opendir closedir readdir readdir_r stat])
AC_CHECK_FUNCS([clock_gettime])
AC_CHECK_FUNCS([inet_ntop inet_pton inet_aton inet_ntoa inet_ntoa_r inet_addr inet_network inet_makeaddr])

AC_CHECK_LIB(pthread, pthread_set_name_np,
	AC_DEFINE(HAVE_PTHREAD_SET_NAME_NP,1),,)
AC_CHECK_LIB(pthread, pthread_setname_np,
	AC_DEFINE(HAVE_PTHREAD_SETNAME_NP,1),,)


AC_CACHE_CHECK([for sa_len in struct sockaddr], ac_cv_struct_sa_len,
[AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/socket.h>], [struct sockaddr s; s.sa_len;],
ac_cv_struct_sa_len=yes, ac_cv_struct_sa_len=no)])
if test $ac_cv_struct_sa_len = yes; then
  AC_DEFINE(HAVE_SOCKADDR_SA_LEN)
fi


AM_ICONV
if test "$am_cv_func_iconv_works" = "yes" 
then
	report_have_iconv="yes"
	ICONV_CFLAGS=$INCICONV
	ICONV_LIBS=$LIBICONV
	AC_SUBST(ICONV_CFLAGS)
	AC_SUBST(ICONV_LIBS)
fi


# Bzip2 Vorhanden?
AM_BZIP2
AX_CHECK_ZLIB(report_have_zlib="yes"
	AC_DEFINE([HAVE_LIBZ], [1], [Define to 1 if you have `z' library (-lz)])
	ZLIB_CFLAGS="-I${ZLIB_HOME}/include"
	ZLIB_LIBS="-L${ZLIB_HOME}/lib -lz"
	AC_SUBST(ZLIB_CFLAGS)
	AC_SUBST(ZLIB_LIBS)
)


dnl do we need to specify -lm explicitly?
AC_CHECK_FUNC(sin,,[AC_CHECK_LIB(m,sin)])

# PCRE vorhanden?
AX_PATH_LIB_PCRE(
            AC_DEFINE(HAVE_PCRE, 1, [ Define if you have pcre. ])
            report_have_pcre="yes"
            ,
            )
       
AX_CHECK_OPENSSL(
	AC_DEFINE(HAVE_OPENSSL, 1, [ Define if you have openssl. ])
	report_have_openssl="yes"
	
	am_save_CPPFLAGS="$CPPFLAGS"
	am_save_CFLAGS="$CFLAGS"
	am_save_LIBS="$LIBS"
	am_save_LDFLAGS="$LDFLAGS"
	
	LIBS="$LIBS $OPENSSL_LIBS"
	CFLAGS="$CFLAGS $OPENSSL_INCLUDES"
	
	AC_CHECK_FUNCS([TLS_method TLS_server_method TLS_client_method])
	AC_CHECK_FUNCS([EVP_aes_128_ecb EVP_aes_192_ecb EVP_aes_256_ecb EVP_aria_128_ecb EVP_aria_192_ecb EVP_aria_256_ecb])
	AC_CHECK_FUNCS([EVP_bf_ecb EVP_camellia_128_ecb EVP_camellia_192_ecb EVP_camellia_256_ecb EVP_cast5_ecb])
	AC_CHECK_FUNCS([EVP_des_ecb EVP_des_ede3_ecb EVP_idea_ecb EVP_rc2_ecb EVP_rc5_32_12_16_ecb])
	AC_CHECK_FUNCS([SSLv23_method SSLv23_server_method SSLv23_client_method])
	
	
	CPPFLAGS="$am_save_CPPFLAGS"
	CFLAGS="$am_save_CFLAGS"
	LIBS="$am_save_LIBS"
	LDFLAGS="$am_save_LDFLAGS"	
	
	, :)

report_have_curl="no"
LIBCURL_CHECK_CONFIG([yes],[7.30.0],
	#LIBS="$LIBS $LIBCURL"
	#CFLAGS="$CFLAGS $LIBCURL_CPPFLAGS "
	AC_SUBST(LIBCURL)
	AC_SUBST(LIBCURL_CPPFLAGS)
	report_have_curl="yes",:)
	
report_have_libcdio="no"
AX_CHECK_LIBCDIO(
        report_have_libcdio="yes",
        :)

report_have_qt="no"
AX_FIND_QMAKE()
if test -x "$QMAKE"
then
	report_have_qt="yes, $qt_version"
fi
if test -x "$qmake"
then
	report_have_qt="yes 2, $qt_version"
fi
if test -x "$ac_cv_path_qmake"
then
	report_have_qt="yes 3, $qt_version"
fi

SRCDIR=`pwd`
AC_SUBST(SRCDIR)

 
# If we support __thread, that can speed up tcmalloc a bit.
# Note, however, that our code tickles a bug in gcc < 4.1.2
# involving TLS and -fPIC (which our libraries will use) on x86:
#   http://gcc.gnu.org/ml/gcc-bugs/2006-09/msg02275.html
AC_MSG_CHECKING([for __thread])
AC_LINK_IFELSE([AC_LANG_PROGRAM([#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)) && ((__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 1) || (__GNUC__ == 4 && __GNUC_MINOR__ == 1 && __GNUC_PATCHLEVEL__ < 2))
#error gcc has this bug: http://gcc.gnu.org/ml/gcc-bugs/2006-09/msg02275.html
#endif], [static __thread int p = 0])],
               [AC_DEFINE(HAVE_TLS, 1,
                      Define to 1 if compiler supports __thread)
                AC_MSG_RESULT([yes])],
               [AC_MSG_RESULT([no])])

# We also need to check if the kernel supports __thread, which requires uname()
AC_CHECK_DECLS(uname,,, [#include <sys/utsname.h>])
 



WINDOWS_WINSOCK_LIBS=""
case "$host_os" in
	*mingw32* )
		MINGW32=yes
		AC_DEFINE(MINGW32, 1, [Define if you compile on Windows MINGW32])
		AC_DEFINE(HAVE_LOCALTIME_R,1,[on mingw-w64 we always have localtime_r])
		AC_DEFINE(HAVE_GMTIME_R,1,[on mingw-w64 we always have gmtime_r])
		#LIBS="$LIBS -lws2_32"
		WINDOWS_WINSOCK_LIBS="-lws2_32"
  		;;
  	*mingw64*|msys )
		MINGW32=yes
		MINGW64=yes
		AC_DEFINE(MINGW32, 1, [Define if you compile on Windows MINGW32])
		AC_DEFINE(MINGW64, 1, [Define if you compile on Windows MINGW64])
		AC_DEFINE(HAVE_LOCALTIME_R,1,[on mingw-w64 we always have localtime_r])
		AC_DEFINE(HAVE_GMTIME_R,1,[on mingw-w64 we always have gmtime_r])
		WINDOWS_WINSOCK_LIBS="-lws2_32"
  		;;
	* )
		MINGW32=no
		;;
esac

AC_SUBST(WINDOWS_WINSOCK_LIBS)

case "${host_cpu}-${host_os}" in
	*solaris*)
		CFLAGS=`echo $CFLAGS | $SED 's/-I\/usr\/local\/include //g' | $SED 's/-I\/usr\/include //g' `
        ;;
esac;

         


AC_SUBST(report_have_zlib)
AC_SUBST(report_have_bzip2)
AC_SUBST(report_have_openssl)
AC_SUBST(report_have_pcre)
AC_SUBST(report_have_iconv)
AC_SUBST(report_have_pthreads)
AC_SUBST(report_have_curl)
AC_SUBST(report_have_libcdio)

###
JPEG_CFLAGS=""
MYSQL_CFLAGS=""
POSTGRESQL_CFLAGS=""
SQLITE3_CFLAGS=""
LAME_CFLAGS=""
LIBMICROHTTPD_CFLAGS=""
LIBCDIO_CFLAGS=""
LIBIDN_CFLAGS=""
LIBLDNS_CFLAGS=""
IMLIB_CFLAGS=""
FT2_CFLAGS=""
HAVE_X86_ASSEMBLER=""
MPG123_CFLAGS=""
SDL2_CFLAGS=""
IMLIB_LIBS=""
FT2_LIBS=""
MYSQL_LDFLAGS=""
POSTGRESQL_LDFLAGS=""
SQLITE3_LDFLAGS=""
LAME_LIBS=""
LIBLDNS_LIBS=""
MPG123_LIBS=""
SDL2_LIBS=""
LIBMICROHTTPD_LIBS=""
LIBIDN_LIBS=""
ASM=""
ASMFLAGS=""
ASMDEBUGFLAGS=""

AC_SUBST(JPEG_CFLAGS)
AC_SUBST(MYSQL_CFLAGS)
AC_SUBST(POSTGRESQL_CFLAGS)
AC_SUBST(SQLITE3_CFLAGS)
AC_SUBST(LAME_CFLAGS)
AC_SUBST(LIBMICROHTTPD_CFLAGS)
AC_SUBST(LIBCDIO_CFLAGS)
AC_SUBST(LIBIDN_CFLAGS)
AC_SUBST(LIBLDNS_CFLAGS)
AC_SUBST(IMLIB_CFLAGS)
AC_SUBST(FT2_CFLAGS)
AC_SUBST(HAVE_X86_ASSEMBLER)
AC_SUBST(MPG123_CFLAGS)
AC_SUBST(SDL2_CFLAGS)
AC_SUBST(IMLIB_LIBS)
AC_SUBST(FT2_LIBS)
AC_SUBST(MYSQL_LDFLAGS)
AC_SUBST(POSTGRESQL_LDFLAGS)
AC_SUBST(SQLITE3_LDFLAGS)
AC_SUBST(LAME_LIBS)
AC_SUBST(LIBLDNS_LIBS)
AC_SUBST(MPG123_LIBS)
AC_SUBST(SDL2_LIBS)
AC_SUBST(LIBMICROHTTPD_LIBS)
AC_SUBST(LIBIDN_LIBS)
AC_SUBST(ASM)
AC_SUBST(ASMFLAGS)
AC_SUBST(ASMDEBUGFLAGS)

###############################################################################
# Output
###############################################################################

AC_OUTPUT


###############################################################################
# Report
###############################################################################

dnl report configuration
AC_MSG_RESULT([
******************************************************************************
** Configuration summary for $PACKAGE_NAME Version $PACKAGE_VERSION
** Target: $target_os        CPU: $target_cpu
** 
** Byteorder: $report_byteorder
**   Size of char         $ac_cv_sizeof_char     Size of float        $ac_cv_sizeof_float
**   Size of short        $ac_cv_sizeof_short     Size of double       $ac_cv_sizeof_double
**   Size of int          $ac_cv_sizeof_int     Size of void*        $ac_cv_sizeof_voidp
**   Size of long         $ac_cv_sizeof_long     Size of wchar_t      $ac_cv_sizeof_wchar_t
**   Size of long long    $ac_cv_sizeof_long_long     
**   Size of size_t       $ac_cv_sizeof_size_t
******************************************************************************
**
** Threads:                $report_have_pthreads
** Qt:                     $report_have_qt
**
** Libs:
**   ZLIB library:         $report_have_zlib
**   BZIP2 library:        $report_have_bzip2
**   OpenSSL:              $report_have_openssl
**   PCRE:                 $report_have_pcre
**   ICONV:                $report_have_iconv
**   Curl library:         $report_have_curl
**   CDIO library:         $report_have_libcdio
**
** Testing:
**   Unittests:            $report_have_gtest
******************************************************************************
])

ERR=0
if test "$report_have_pthreads" = "no"
then
	AC_MSG_RESULT([ERROR: no threads libraray])
	ERR=1
fi

if test "$report_have_qt" = "no"
then
	AC_MSG_RESULT([ERROR: Qt not found])
	ERR=1
fi

if test "$report_have_pcre" = "no"
then
	AC_MSG_RESULT([ERROR: pcre library not found])
	ERR=1
fi

if test "$report_have_pcre" = "no"
then
	AC_MSG_RESULT([ERROR: pcre library not found])
	ERR=1
fi

if test $ERR -ne 0
then
	AC_MSG_ERROR([Can't compile, required features or 3rd party libs not found!])
else
	AC_MSG_RESULT([])
fi

