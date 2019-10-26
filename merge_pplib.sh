#!/bin/sh
PPL6_DIR=../ppl6

WORK=`pwd`
if [ ! -d $PPL6_DIR ] ; then
	echo "ERROR: ppl6 not found!"
	exit 1
fi

rm -rf pplib/ppl6/* pplib/include/* && mkdir -p pplib/ppl6 pplib/include

cd $PPL6_DIR
sources=`find include/compat.h include/config.h.in include/internal.h include/ppl6.h \
	include/ppl6-algorithms.h include/ppl6-exceptions.h \
	include/ppl6-sound.h include/ppl6-types.h include/prolog.h include/thread.h `

for source_file in $sources
do
	target=`basename $source_file`
	cp $source_file $WORK/pplib/include/$target
done	
	

sources=`find src/sound/AudioInfo.cpp src/sound/CID3Tag.cpp src/sound/mp3.cpp \
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


