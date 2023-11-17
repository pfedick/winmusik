dnl AX_CHECK_LIBCDIO([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl Autor: Patrick Fedick <patrick@pfp.de>
dnl
dnl If library is found, it sets the following defines:
dnl    HAVE_LIBCDIO
dnl    LIBCDIO_CFLAGS
dnl    LIBCDIO_LDFLAGS

AC_DEFUN([AX_CHECK_LIBCDIO],[
AC_MSG_CHECKING(if we should use libcdio)

AC_ARG_WITH([libcdio],
	[  --with-libcdio[[=PATH]]   support accesing audio cds (optional)],
	[libcdio_prefix="$withval"],
	[libcdio_prefix="auto"])

	have_libcdio="no"

	if test "$libcdio_prefix" != "no"
	then
		AC_MSG_RESULT(yes)
		
		am_save_CPPFLAGS="$CPPFLAGS"
		am_save_LIBS="$LIBS"
		am_save_LDFLAGS="$LDFLAGS"
		
		if test -n "$libcdio_prefix" -a "$libcdio_prefix" != "yes" -a "$libcdio_prefix" != "auto"
		then
		    LIBCDIO_CFLAGS="-I$with_libcdio/include"
		    LIBCDIO_LDFLAGS="-L$with_libcdio/lib -lcdio " 
		else
			AC_PATH_PROG([PKGCONFIG], [pkg-config], [no])
			if test "$PKGCONFIG" != "no" && `$PKGCONFIG --exists libcdio`
			then
				LIBCDIO_CFLAGS=`$PKGCONFIG --cflags libcdio `
		    	LIBCDIO_LDFLAGS=`$PKGCONFIG --libs libcdio `
		    	else 
		    		PKG_CHECK_MODULES([libcdio], [libcdio],
					LIBCDIO_CFLAGS=$libcdio_CFLAGS
					LIBCDIO_LDFLAGS=$libcdio_LIBS
				,
					LIBCDIO_LDFLAGS="-lcdio "
				)
			fi
		fi
		
		CFLAGS="$CFLAGS $LIBCDIO_CFLAGS"
		LDFLAGS="$LDFLAGS $LIBCDIO_LDFLAGS"
		AC_MSG_CHECKING(if we can link against libcdio)
		AC_LINK_IFELSE([AC_LANG_PROGRAM([[
			#include <stdio.h>
			#include <cdio/cdio.h>
			]], [[
				CdIo_t *p_cdio = cdio_open (NULL, DRIVER_DEVICE);
			]])],[AC_MSG_RESULT(yes)
				have_libcdio="yes"
			],[AC_MSG_RESULT(no)
			])
				
			
		CPPFLAGS=$am_save_CPPFLAGS
		LIBS=$am_save_LIBS
		LDFLAGS=$am_save_LDFLAGS
	else
		AC_MSG_RESULT(no)
	fi
	if test $have_libcdio = "yes"
	then
  		AC_DEFINE(HAVE_LIBCDIO, 1, [Define if you have the libcdio library])
		ifelse([$1], , :, [$1])
	else
		LIBCDIO_CFLAGS=""
		LIBCDIO_LDFLAGS=""
		ifelse([$2], , :, [$2])
	fi
	AC_SUBST(LIBCDIO_CFLAGS)
	AC_SUBST(LIBCDIO_LDFLAGS)
	
])
