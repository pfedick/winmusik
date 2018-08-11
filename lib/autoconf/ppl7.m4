dnl AX_PATH_LIB_PPL7([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN([AX_PATH_LIB_PPL7],[dnl

AC_ARG_WITH([libppl7],
	[  --with-libppl7[[=PATH]]  Prefix where PPL7-Library is installed],
	[ppl7_prefix="$withval"],
	[ppl7_prefix="no"])

#if test "$ppl7_prefix" != "no"
#then
	if test "$ppl7_prefix" = "no"
	then
		AC_PATH_PROG(ppl7config,ppl7-config)
	elif test "$ppl7_prefix" != "yes"
	then
		ppl7config="$ppl7_prefix/bin/ppl7-config"
	else
		AC_PATH_PROG(ppl7config,ppl7-config)
	fi
	
	AC_MSG_CHECKING([for lib ppl7])
	if test [ -z "$ppl7config" ]
	then
		AC_MSG_RESULT(no)
	    AC_MSG_ERROR([ppl7 library (libppl7) and/or headers not found])
		
		ifelse([$3], , :, [$3])
	else
		AC_MSG_RESULT(yes)
		min_ppl_version=ifelse([$1], ,6.0.0,[$1])
		AC_MSG_CHECKING(for ppl7 version >= $min_ppl_version)
		
		ppl_version=`${ppl7config} --version`
		ppl_config_major_version=`echo $ppl_version | \
			sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    	ppl_config_minor_version=`echo $ppl_version | \
			sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
		ppl_config_micro_version=`echo $ppl_version | \
			sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
		ppl_config_version=`expr $ppl_config_major_version \* 10000 + $ppl_config_minor_version \* 100 + $ppl_config_micro_version`

		ppl_req_major_version=`echo $min_ppl_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    	ppl_req_minor_version=`echo $min_ppl_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
		ppl_req_micro_version=`echo $min_ppl_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
		ppl_req_version=`expr $ppl_req_major_version \* 10000 + $ppl_req_minor_version \* 100 + $ppl_req_micro_version`
		
		if test $ppl_config_version -lt $ppl_req_version
		then
			AC_MSG_RESULT([no, have $ppl_version])
			ifelse([$3], , :, [$3])
		else 
			AC_MSG_RESULT([yes (version $ppl_version) ])
			#AC_MSG_CHECKING(ppl7 debug libraries)
			LIBPPL7_DEBUG_LIBS=`${ppl7config} --libs debug`
			#AC_MSG_RESULT($LIBPPL7_DEBUG_LIBS)
			#AC_MSG_CHECKING(ppl7 release libraries)
			LIBPPL7_RELEASE_LIBS=`${ppl7config} --libs release`
			LIBPPL7_RELEASE_ARCHIVE=`${ppl7config} --archive release`
			LIBPPL7_DEBUG_ARCHIVE=`${ppl7config} --archive debug`
			#AC_MSG_RESULT($LIBPPL7_RELEASE_LIBS)
			#AC_MSG_CHECKING(ppl7 includes)
			LIBPPL7_CFLAGS=`${ppl7config} --cflags`
			LIBPPL7=`${ppl7config} --ppllib release`
			LIBPPL7_DEBUG=`${ppl7config} --ppllib debug`
			
			#AC_MSG_RESULT($LIBPPL7_CFLAGS)
			ifelse([$2], , :, [$2])
		fi
	fi
#else
#	AC_MSG_RESULT(not configured)
#	AC_MSG_ERROR([ppl7 library is required])
#fi
])



dnl AX_PPL7_FEATURE([FEATURE, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN([AX_PPL7_FEATURE],[dnl
	AC_MSG_CHECKING([for ppl7-feature: $1])
	if test -z "${ppl_features}"
	then
		ppl_features=`${ppl7config} --features`
	fi
	echo ${ppl_features}| tr " " "\n" | grep -i "^$1" > /dev/null 2>&1
	if test $? -eq 0
	then
		AC_MSG_RESULT(yes)
		ifelse([$2], , :, [$2])
	else
		AC_MSG_RESULT(no)
		ifelse([$3], , :, [$3])
	fi
])

