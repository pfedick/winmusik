#!/bin/sh
###############################################################################
# This file is part of WinMusik 3 by Patrick Fedick                           #
# Web: https://www.pfp.de/winmusik/                                           #
###############################################################################
#
# Copyright (c) 2019, Patrick Fedick <patrick@pfp.de>
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

PPL6_DIR=../ppl6
PPL7_DIR=../ppl7

WORK=`pwd`
if [ ! -d $PPL6_DIR ] ; then
	echo "ERROR: ppl6 not found!"
	exit 1
fi
if [ ! -d $PPL7_DIR ] ; then
	echo "ERROR: ppl7 not found!"
	exit 1
fi


rm -rf pplib/ppl6/* pplib/ppl7/* pplib/include/* 
mkdir -p pplib/ppl6 pplib/ppl7 pplib/include

#######################################################################
# PPL6
#######################################################################
cd $PPL6_DIR
sources=`find include/compat.h include/config.h.in include/internal.h include/ppl6.h \
	include/ppl6-algorithms.h include/ppl6-exceptions.h \
	include/ppl6-sound.h include/ppl6-types.h include/prolog.h include/thread.h `

for source_file in $sources
do
	target=`basename $source_file`
	cp $source_file $WORK/pplib/include/$target
done	
	

sources=`find src/sound/AudioInfo.cpp src/sound/CID3Tag.cpp \
	src/sound/AudioCD.cpp src/sound/Cddb.cpp \
	src/vars/*.cpp src/core/CAVLTree.cpp src/core/CDebug.cpp src/core/CDir.cpp \
	src/core/CFile.cpp src/core/CFileObject.cpp src/core/CIconv.cpp \
	src/core/CList.cpp src/core/CLog.cpp src/core/CMemoryReference.cpp \
	src/core/CMutex.cpp src/core/compat.cpp src/core/CSignal.cpp src/core/CThread.cpp \
	src/core/CTok.cpp src/core/CTree.cpp src/core/Errors.cpp src/core/Exception.cpp \
	src/core/functions.cpp src/core/pplcore.cpp src/core/strings.cpp src/core/time.cpp \
	src/core/CConfig.cpp src/core/CHeap.cpp src/core/CNotifyHandler.cpp \
	src/core/MemoryHeap.cpp src/core/CMemory.cpp src/core/cpu.cpp src/core/CMemFile.cpp \
	src/math/random.cpp src/math/md5.cpp \
	src/internet/CCurl.cpp `

for source_file in $sources
do
	target=`basename $source_file`
	cp $source_file $WORK/pplib/ppl6/ppl6_$target
done

cp LICENSE.TXT $WORK/pplib/ppl6

#######################################################################
# PPL7
#######################################################################
cd $PPL7_DIR
sources=`find include/compat_ppl7.h include/config_ppl7.h.in include/ppl7.h \
	include/ppl7-algorithms.h include/ppl7-exceptions.h include/ppl7-ppl6compat.h \
	include/ppl7-audio.h include/ppl7-types.h include/prolog_ppl7.h include/threads_ppl7.h \
include/ppl7-inet.h include/ppl7-crypto.h include/ppl7-config.h.in `

for source_file in $sources
do
	target=`basename $source_file`
	cp $source_file $WORK/pplib/include/$target
done	
sources=`find src/audio/AudioCD.cpp src/audio/AudioInfo.cpp src/audio/Cddb.cpp \
	src/audio/ID3Tag.cpp src/audio/Mp3.cpp \
	src/core/*.cpp \
	src/types/*.cpp \
	src/crypto/*.cpp \
	src/math/*.cpp \
	src/internet/curl.cpp `

for source_file in $sources
do
	target=`basename $source_file`
	cp $source_file $WORK/pplib/ppl7/ppl7_$target
done
rm -rf $WORK/pplib/ppl7/ppl7_Resourcen.cpp

cp LICENSE.TXT $WORK/pplib/ppl7


#######################################################################
# Final
#######################################################################
cd $WORK/pplib
rm -rf debug release
./genMakefile.in





