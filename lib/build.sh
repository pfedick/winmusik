#!/bin/sh
PREFIX=/jenkins/local
make -j4 all
if [ $? -eq 0 ] ; then
	cp compile/libwinmusik3.a ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/winmusik3
	cp include/*.h ${PREFIX}/include/winmusik3
fi
