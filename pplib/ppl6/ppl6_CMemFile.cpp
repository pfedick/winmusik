/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 6 (PPL6).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2010, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "prolog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
// ERRORSFIXED=1

#ifndef _WIN32
#include <errno.h>
#include <unistd.h>
#else
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif
#include "ppl6.h"

#ifdef _WIN32
//#define USEWIN32
#endif

namespace ppl6 {

#define COPYBYTES_BUFFERSIZE 1024*1024


/********** MEMFile *******************************************/

/*!\class CMemFile
 * \ingroup PPLGroupFileIO
 * \brief Simulation von Dateizugriffen im Hauptspeicher
 *
 * Mit dieser von CFileObject abgeleiteten Klasse können Dateizugriffe im Hauptspeicher simuliert werden.
 * Sie ist hauptsächlich zum Lesen von Daten gedacht, z.B. aus einer Resourcendatei. Zwar können auch
 * Daten geschrieben werden, jedoch nicht über die initiale Größe des Speicherbereichs hinaus.
 *
 * Der zu verwendende Speicherbereich kann entweder über den Konstruktor abgegeben werden (siehe
 * CMemFile::CMemFile (void * adresse, size_t size) ) oder über die Funktion CMemFile::Open.
 *
 */

CMemFile::CMemFile ()
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::CMemFile ()",this);
	#endif

	size=0;
	pos=0;
	buffer=NULL;
	MemBase=NULL;
}


CMemFile::CMemFile (void * adresse, size_t size)
/*!\brief Konstruktor der Klasse mit Angabe eines Speicherbereichs
 *
 * Mit diesem Konstruktor wird gleichzeitig ein Pointer auf den Speicherbereich \p adresse mit einer
 * Größe von \p size Bytes übergeben. Sämtliche Dateizugriffe werden in diesem Speicherbereich
 * simuliert.
 *
 * @param adresse Pointer auf den zu verwendenden Speicherbereich
 * @param size Größe des Speicherbereichs
 */
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::CMemFile (adresse=%u, size=%u)",this,adresse,size);
	#endif

	MemBase=NULL;
	buffer=NULL;
	pos=0;
	Open(adresse,size);
}

/*!\brief Konstruktor der Klasse mit Angabe eines Speicherbereichs
 *
 * Mit diesem Konstruktor wird gleichzeitig ein Pointer auf den Speicherbereich \p adresse mit einer
 * Größe von \p size Bytes übergeben. Sämtliche Dateizugriffe werden in diesem Speicherbereich
 * simuliert.
 *
 * @param adresse Pointer auf den zu verwendenden Speicherbereich
 * @param size Größe des Speicherbereichs
 */
CMemFile::CMemFile (const CMemoryReference &memory)
{
	if (memory.isEmpty()) {
		MemBase=NULL;
		buffer=NULL;
		pos=0;
		return;
	}
	MemBase=(char*)memory.adr();
	size=memory.size();
	pos=0;
}

CMemFile::~CMemFile()
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::~CMemFile (), MemBase=%u, pos=%u, size=%u",this,MemBase,pos,size);
	#endif
	PopError();
}

int CMemFile::Open (void * adresse, ppluint32 size)
/*!\brief Speicherbereich öffnen
 *
 * Mit dieser Funktion wird die simulierte Datei im Hauptspeicher geöffnet. Dazu muss mit
 * \p adresse ein Pointer auf den Beginn des zu verwendenden Hauptspeichers angegeben werden,
 * sowie mit \p size seine Größe. Sämtliche nachfolgenden Dateizugriffe werden dann in diesem
 * Speicherbereich simuliert.
 *
 * @param adresse Pointer auf den zu verwendenden Speicherbereich
 * @param size Größe des Speicherbereichs
 * \return Die Funktion gibt bei Erfolg 1 zurück, im Fehlerfall 0. Ein Fehler kann nur auftreten,
 * wenn einer der Parameter \p adresse oder \p size einen Nullwert aufweisen.
 */

{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Open (adresse=%u, size=%u)",this,adresse,size);
	#endif

	if (adresse==NULL || size==0) {
		SetError(194);
		return 0;
	}
	MemBase=(char*)adresse;
	this->size=size;
	pos=0;
	return 1;
}

/*!\brief Speicherbereich öffnen
 *
 * Mit dieser Funktion wird die simulierte Datei im Hauptspeicher geöffnet. Dazu muss mit
 * \p adresse ein Pointer auf den Beginn des zu verwendenden Hauptspeichers angegeben werden,
 * sowie mit \p size seine Größe. Sämtliche nachfolgenden Dateizugriffe werden dann in diesem
 * Speicherbereich simuliert.
 *
 * @param memory Referenz auf eine CMemoryReference-Klasse, die den zu verwendenden Speicherbereich enthält
 * \return Die Funktion gibt bei Erfolg 1 zurück, im Fehlerfall 0. Ein Fehler kann nur auftreten,
 * wenn einer der Parameter \p adresse oder \p size einen Nullwert aufweisen.
 */
int CMemFile::Open(const CMemoryReference &memory)
{
	if (memory.isEmpty()) {
		SetError(544);
		return 0;
	}
	MemBase=(char*)memory.adr();
	size=memory.size();
	pos=0;
	return 1;
}

bool CMemFile::IsOpen() const
{
	if (MemBase!=NULL) return true;
	return false;
}

int CMemFile::Close()
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Close (), MemBase=%u, pos=%u, size=%u",this,MemBase,pos,size);
	#endif

	if (MemBase!=NULL) {
		MemBase=NULL;
		size=0;
		pos=0;
		if (buffer!=0) {
			free (buffer);
			buffer=0;
		}
		return 1;
	}
	SetError(72);
	return 0;
}

pplint64 CMemFile::Size () const
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Lof (), MemBase=%u, pos=%u, size=%u",this,MemBase,pos,size);
	#endif

	if (MemBase!=NULL) {
		return (pplint64)size;
	}
	SetError(72);
	return -1;
}

int CMemFile::Seek(ppluint64 position)
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Seek (%u), MemBase=%u, pos=%u, size=%u",this,position,MemBase,pos,size);
	#endif

	if (MemBase!=NULL) {
		if (position<size) {
			pos=position;
		} else {
			return 0;
		}
		return 1;
	}
	SetError(72);
	return 0;
}

int	 CMemFile::Fseek (ppluint64 offset, int origin )
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Fseek (offset=%u, origin=%u), MemBase=%u, pos=%u, size=%u",this,offset,origin,MemBase,pos,size);
	#endif

	if (MemBase!=NULL) {
		//suberr=::fseek(ff,offset,origin);
		//if (suberr==0) {
		SetError(0);
		switch (origin) {
			case SEEK_CUR:
				pos+=offset;
				if (pos>size) return 1;
				return 1;
			case SEEK_END:
				if (offset>size) {
					pos=0; return 1;
				}
				pos=size-offset;
				return 1;
			case SEEK_SET:
				pos=offset;
				return 1;
		}
		return 0;
	}
	SetError(72);
	return 1;
}

pplint64 CMemFile::Ftell()
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Ftell (), MemBase=%u, pos=%u, size=%u",this,MemBase,pos,size);
	#endif

	if (MemBase!=NULL) {
		return pos;
	}
	SetError(72);
	return -1;
}


size_t CMemFile::Fread(void *ptr, size_t size, size_t nmemb)
{
	if (MemBase==NULL) {
		SetError(72);
		return 0;
	}
	size_t by=nmemb;
	if (pos+(by*size)>this->size) by=(size_t)(this->size-pos)/size;
	memmove(ptr,MemBase+pos,by*size);
	pos+=(by*size);
	if (by<nmemb) SetError(154);
	return by;
}

size_t CMemFile::Fwrite(const void *ptr, size_t size, size_t nmemb)
{
	if (MemBase==NULL) {
		SetError(72);
		return 0;
	}
	size_t by=nmemb;
	if (pos+(by*size)>this->size) by=(size_t)(this->size-pos)/size;
	memmove(MemBase+pos,ptr,by*size);
	pos+=(by*size);
	if (by<nmemb) SetError(154);
	return by;
}


ppluint64 CMemFile::DoCopy (CFileObject &quellfile, ppluint64 bytes)
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Copy (quellfile=%u, bytes=%u)",this,quellfile,bytes);
	#endif

	if (MemBase==NULL) {
		SetError(72);
		return 0;
	}
	if (buffer==NULL) {
		buffer=(char *)malloc(COPYBYTES_BUFFERSIZE);
		if (buffer==NULL) {
			SetError(2);
			return 0;
		}
	}
	if (quellfile.Size()>quellfile.Ftell()) {
		if (quellfile.Ftell()+(pplint64)bytes>quellfile.Size()) {
			bytes=(ppldd)quellfile.Lof()-(ppldd)quellfile.Ftell();
		}
		ppluint64 rest=bytes;
		ppluint32 by;

		while (rest>0) {
			by=quellfile.Fread (buffer,1,COPYBYTES_BUFFERSIZE);
			Write (buffer,by);
			if (GetErrorCode()!=0) {
				bytes-=rest;
				break;
			}
			rest-=by;
		}

	}
	return bytes;
}

char *CMemFile::Fgets (char *buffer1, size_t num)
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Gets (buffer=%u, size=%u), MemBase=%u, pos=%u, size=%u",this,buffer1,bufferlength,MemBase,pos,size);
	#endif

	if (MemBase!=NULL) {
		if (pos==size) {
			SetError(154);
			return NULL;
		}
		ppldd by;
		by=num-1;
		if (pos+by>size) by=(ppluint32)(size-pos);
		char *ptr=MemBase+pos;
		ppldd i;
		for (i=0;i<by;i++) {
			if ((buffer1[i]=ptr[i])=='\n') {
				i++;
				break;
			}
		}
		//memcpy ((void*)buffer1,MemBase+pos,by);
		buffer1[i]=0;
		//by=(ppluint32)strlen(buffer1);
		pos+=i;
		if (pos==size) SetError(154);
		#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
			Debug.Log ("  Bytes read=%u, pos=%u, size=%u",by,pos,size);
	  #endif

		return buffer1;
	}
	SetError(72);
	return NULL;
}


int CMemFile::Puts (const char *str)
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Puts (LPSTR fmt, ...), MemBase=%u, pos=%u, size=%u",this,MemBase,pos,size);
	#endif

	if (MemBase!=NULL) {
		Fwrite ((void*)str,1,(ppluint32)strlen(str));
		#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
			Debug.Log ("  Bytes written=%u, pos=%u, size=%u",strlen(buff),pos,size);
		#endif
		SetError(0);
		return 1;

	}
	SetError(72);
	return 0;
}

int CMemFile::Putc(int c)
{
	if (MemBase!=NULL) {
		MemBase[pos++]=(ppldb)c;
		return c;
	}
	SetError(72);
	return -1;
}

int CMemFile::Getc()
{
	if (MemBase!=NULL) {
		return MemBase[pos++];
	}
	SetError(72);
	return -1;
}
bool CMemFile::Eof() const
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Eof() pos>=size %u>=%u",this,pos,size);
	#endif

	if (MemBase!=NULL) {
		if (pos>=size) return true;
		return false;

	}
	SetError(72);
	return false;
}

char *CMemFile::Adr(ppluint32 adresse)
{
	#ifdef DEBUGLOG	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUG
		Debug.Log ("%u->CMemFile::Adr(adresse=%u)",this,adresse);
	#endif

	if (MemBase!=NULL) {
		return (MemBase+adresse);
	}
	SetError(72);
	return 0;
}

const char *CMemFile::Map(ppluint64 position, size_t bytes)
{
	if (position+bytes<=size) {
		return (MemBase+position);
	}
	return NULL;
}

char *CMemFile::MapRW(ppluint64 position, size_t bytes)
{
	if (position+bytes<=size) {
		return (MemBase+position);
	}
	return NULL;
}

void CMemFile::Unmap()
{
	return;
}


int CMemFile::Flush()
{
	return 1;
}

int CMemFile::Sync()
{
	return 1;
}



}	// EOF namespace ppl6

