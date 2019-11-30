dnl AC_CHECK_LIBMP3LAME([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN([AC_CHECK_LIBMP3LAME],[
	AC_ARG_WITH([lame],
		[[  --with-lame[=DIR]       enable support for MP3 encoding with lame [in DIR]]],
		[lame_prefix="$withval"],
		[lame_prefix="no"])
	AC_MSG_CHECKING(if lame should be included)
	if test "$with_lame" != "no"
	then
		AC_MSG_RESULT(yes)
		have_LAME="no"
		am_save_CPPFLAGS="$CPPFLAGS"
		am_save_LIBS="$LIBS"
		am_save_LDFLAGS="$LDFLAGS"
		
		if test -n "$with_lame" -a "$with_lame" != "yes"
		then
			intLAME_CFLAGS="-I$with_lame/include"
    		intLAME_LIBS="-L$with_lame/lib -lmp3lame -lm"
    		intLAME_LDFLAGS="-L$with_lame/lib"
    	else
			intLAME_CFLAGS=""
    		intLAME_LIBS="-lmp3lame -lm"
    		intLAME_LDFLAGS=""    	
  		fi
  		CPPFLAGS="$CPPFLAGS $intLAME_CFLAGS"
  		LDFLAGS="$LDFLAGS $intLAME_LDFLAGS"
  		AC_CHECK_HEADER(lame/lame.h, [
  			have_LAME="maybe",
    		AC_DEFINE(HAVE_LAME_LAME_H, 1, [Use <lame/lame.h> instead of <lame.h>])
  		], [
  			AC_CHECK_HEADER(lame.h, [
  				have_LAME="maybe"
  			])
  		])
  		if test "$have_LAME" != "no"
  		then
			LIBS="$LIBS $intLAME_LIBS"
    		AC_CHECK_LIB(mp3lame, lame_encode_flush_nogap,
    			LAME_CFLAGS=$intLAME_CFLAGS
    			LAME_LIBS=$intLAME_LIBS
    			LAME_LDFLAGS=$intLAME_LDFLAGS
    			ifelse([$2], , :, [$2])
				,
				ifelse([$3], , :, [$3])
			:)
    		AC_CHECK_LIB(mp3lame, hip_decode, [
    			AC_DEFINE(HAVE_LAME_HIP_DECODE, 1, [Lame has threadsafe decoder])
    		])
  		fi
		CPPFLAGS=$am_save_CPPFLAGS
		LIBS=$am_save_LIBS
		LDFLAGS=$am_save_LDFLAGD
	else
		AC_MSG_RESULT(no)
	fi
])
