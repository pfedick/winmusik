dnl AM_BZIP2([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN([AM_BZIP2],[dnl
AC_MSG_CHECKING([for compatible bzip2 library and headers])
AC_ARG_WITH([bzip2],
	[  --with-bzip2[[=PATH]]     Prefix where bzip2 is installed (optional)],
	[bzip2_prefix="$withval"],
	[bzip2_prefix="no"])

am_save_CPPFLAGS="$CPPFLAGS"
am_save_LIBS="$LIBS"
am_save_LDFLAGS="$LDFLAGS"
BZ2_LIBS="-lbz2"
BZ2_CFLAGS=""
report_have_bzip2="no"
	if test "$bzip2_prefix" != "yes"
	then
		if test "$bzip2_prefix" != "no"
		then
			LIBS="-L$bzip2_prefix/lib -lbz2"
			CPPFLAGS="-I$bzip2_prefix/include"
			BZ2_LIBS="-L$bzip2_prefix/lib -lbz2"
			BZ2_CFLAGS="-I$bzip2_prefix/include"
		else
			LIBS="$LIBS -lbz2"
			BZ2_LIBS="-lbz2"
		fi
	else
		LIBS="$LIBS -lbz2"
		BZ2_LIBS="-lbz2"
	fi
    AC_LINK_IFELSE( [AC_LANG_SOURCE([[
         #include <bzlib.h>
         int main()
         {
            char* source = "Hello World";
            char* dest;
            unsigned int destLen = 1000;
            unsigned int sourceLen = 1000;
            int blockSize100k = 5;
            int verbosity = 0;
            int workFactor = 5;
            BZ2_bzBuffToBuffCompress( dest, &destLen, source,
                                      sourceLen, blockSize100k,
                                      verbosity, workFactor);
         }
      ]]) ],
      [AC_MSG_RESULT(yes)
      AC_DEFINE(HAVE_BZIP2, 1, [ Define if you have bzip2. ])
      AC_SUBST(BZ2_CFLAGS)
	  AC_SUBST(BZ2_LIBS)
      report_have_bzip2="yes"
    
      ],
      [
         AC_MSG_RESULT(no)
      ]
    )

CPPFLAGS=$am_save_CPPFLAGS
LIBS=$am_save_LIBS
LDFLAGS=$am_save_LDFLAGS


])
