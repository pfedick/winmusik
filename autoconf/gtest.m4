dnl AC_CHECK_GTEST([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])

AC_DEFUN([AC_CHECK_GTEST],[
AC_MSG_CHECKING(if googletest 111 should be used for unittesting)

AC_ARG_ENABLE([gtest],[
AS_HELP_STRING([--enable-gtest=PATH],
                  [Enable tests using the Google C++ Testing Framework.
                  (Default is enabled.)])],
  [],
  [enable_gtest="yes"])

HAVE_GTEST="no"

if test "$enable_gtest" != "no"
then
	if test "$enable_gtest" = "yes"
	then
		enable_gtest="wmlib/tests/googletest-release-1.10.0"
	fi

	if test -d "$enable_gtest"
	then
		if test -f "$enable_gtest/src/gtest-all.cc"
		then
			if test -d "$enable_gtest/include/gtest"
			then
				HAVE_GTEST="yes"
				GTEST_PREFIX="$enable_gtest"
				AC_SUBST([GTEST_PREFIX])
			fi
		fi
		if test -f "$enable_gtest/googletest/src/gtest-all.cc"
		then
			HAVE_GTEST="yes"
			GTEST_PREFIX="$enable_gtest/googletest"
			AC_SUBST([GTEST_PREFIX])
		fi
	fi
	if test "$HAVE_GTEST" = "yes"
	then
		AC_MSG_RESULT(yes)

	else
		AC_MSG_RESULT([no: not found])
	fi
else
	AC_MSG_RESULT([no: disabled by user])
fi
AC_SUBST([HAVE_GTEST])
if test $HAVE_GTEST = "yes"
then
	ifelse([$1], , :, [$1])
else
	ifelse([$2], , :, [$2])
fi

])
