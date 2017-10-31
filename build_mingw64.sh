#!/bin/sh
qmake INCLUDEPATH+=/jenkins/local/include -o Makefile WinMusik.pro
mingw32-make -j8 release debug
