# find qmake
AC_DEFUN([AX_FIND_QMAKE],[

AC_MSG_CHECKING([for qmake])
AC_ARG_WITH([qmake],
	[  --with-qmake=PATH       Path and name of the qmake program (optional)],
	[QMAKE="$withval"],
	[QMAKE="yes"])

dnl AC_MSG_RESULT([QMAKE=$QMAKE])

if test "$QMAKE" = "yes"; then
	if test -x "/usr/local/lib/qt5/bin/qmake" ; then
		QMAKE=/usr/local/lib/qt5/bin/qmake
	elif test -x "/usr/lib/qt5/bin/qmake" ; then
		QMAKE=/usr/lib/qt5/bin/qmake
	else
		AC_CHECK_PROGS(qmake,[qmake-qt5 qmake-qt4 qmake],no)
		if test "$qmake" = "no" ; then
			AC_MSG_ERROR(qmake not found)
		fi
		QMAKE=$qmake
	fi
elif test -x "$QMAKE" ; then
	AC_MSG_RESULT([$QMAKE])
	
else
	if test ! -x "$QMAKE"  ; then
		AC_MSG_ERROR(qmake not found)
	fi
	AC_MSG_RESULT($QMAKE)
fi

qt_version=`$QMAKE -v | grep Qt`
AC_SUBST(qt_version)
AC_SUBST(QMAKE)

])
