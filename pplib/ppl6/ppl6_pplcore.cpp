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
#include <stdarg.h>
#define _IS_PPL6_CORE

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "ppl6.h"
#include "thread.h"

#define PPL_COPYRIGHT		"(c) Copyright by Patrick F.-Productions in 2005-2013"



ppl6::Cppl6Core *PPLCore=NULL;

namespace ppl6 {


static int			coreref=0;
static CPUCaps		cpucaps;
CMutex	GlobalMutex;

/*
static ppluint64	usedmem=0;
static ppluint64	memref=0;
static bool			memdebug=false;
static CMutex	MemMutex;

static void MemoryDebugClearTable();
static void MemoryDebugAdd(void *ptr, size_t size, char *name, char *descr);
static void MemoryDebugUpdate(void *ptr_old, void *ptr_new, size_t size, char *name, char *descr);
static void MemoryDebugDelete(void *ptr, char *name, char *descr);
static void MemoryDebugDump();
static void MemoryDebugLog();


typedef struct tagMemEntry {
	struct		tagMemEntry *next, *previous;
	void		*ptr;
	ppluint64	size;
	char		*descr;
	char		*name;
	ppluint64	thread;
} MEMENTRY;
static MEMENTRY *mem_first=NULL, *mem_last=NULL;

*/


typedef struct tagExitFunctions {
	struct		tagExitFunctions *next;
	int			(*PPLExitFunction)(void *data);
	void		*data;
} EXITFUNCTIONS;

static EXITFUNCTIONS *last_exit=NULL;

static CHeap	*Heap_CString=NULL;
static CHeap	*Heap_CWString=NULL;
static CHeap	*Heap_CArrayItem=NULL;
static CHeap	*Heap_TREEITEMS=NULL;


static int	printdebug=0;

void SetGlobalOutput(int type)
/*!\brief Ausgabe für Debug-Output angeben
 *
 * Mit dieser Funktion wird festgelegt, an welcher Stelle die Textausgabe erfolgen soll.
 * Einige Funktionen (z.B. CAssocArray::List) geben Informationen aus. Standardmäßig werden diese
 * per STDOUT auf die Konsole ausgegeben. Unter Windows gibt es aber manchmal keine Konsole. Hier kann
 * es dann ganz hilfreich sein die Ausgabe im Debugger von Visual Studio zu sehen.
 *
 * \param[in] type 0=STDOUT, 1=VisualStudio Debugger
 */
{
	printdebug=type;
}

void PrintDebug(const char *format, ...)
/*!\brief Interne Funktion zur Ausgabe von Text
 *
 * Diese Funktion dient als Ersatz für "printf" und wird intern von einigen Funktionen/Klasse zur
 * Ausgabe von Text verwendet. Über die Funktion SetGlobalOutput kann bestimmt werden, ob dieser
 * Text per STDOUT auf die Konsole ausgegeben werden soll oder beispielsweise im Debugger von
 * VisualStudio unter Windows.
 *
 * \param[in] format Formatstring für den Text
 * \param[in] ...    Optionale Parameter, die im Formatstring eingesetzt werden sollen
 */
{
	if (!format) return;
	char *buff=NULL;
	va_list args;
	va_start(args, format);
	vasprintf (&buff, format, args);
	va_end(args);
	if (!buff) return;
	if (printdebug==1) {
#ifdef WIN32
		OutputDebugString(buff);
#endif
	} else {
		ppl_printf("%s",buff);
	}
	free(buff);
}

void PrintDebugTime(const char *format, ...)
/*!\brief Interne Funktion zur Ausgabe von Text
 *
 * Diese Funktion dient als Ersatz für "printf" und wird intern von einigen Funktionen/Klasse zur
 * Ausgabe von Text verwendet. Über die Funktion SetGlobalOutput kann bestimmt werden, ob dieser
 * Text per STDOUT auf die Konsole ausgegeben werden soll oder beispielsweise im Debugger von
 * VisualStudio unter Windows.
 *
 * \param[in] format Formatstring für den Text
 * \param[in] ...    Optionale Parameter, die im Formatstring eingesetzt werden sollen
 */
{
	if (!format) return;
	char *buff=NULL;
	va_list args;
	va_start(args, format);
	vasprintf (&buff, format, args);
	va_end(args);
	if (!buff) return;
	CDateTime now;
	now.setCurrentTime();
	CString Time=now.getISO8601withMsec();
	Time.Concatf(" [%llu]: ",GetThreadID());


	if (printdebug==1) {
#ifdef WIN32
		OutputDebugString((const char*)Time);
		OutputDebugString(buff);
#endif
	} else {
		ppl_printf("%s%s",(const char*)Time,buff);
	}
	free(buff);
}

void PPLExit()
{
	GlobalMutex.Lock();
	EXITFUNCTIONS *fn;
	while (last_exit!=NULL) {
		fn=last_exit;
		last_exit=fn->next;
		fn->PPLExitFunction(fn->data);
		free(fn);
	}
	GlobalMutex.Unlock();
	//SSL_Exit();
	if (PPLCore) delete PPLCore;
	if (Heap_CArrayItem) delete Heap_CArrayItem;
	if (Heap_TREEITEMS) delete Heap_TREEITEMS;
	if (Heap_CWString) delete Heap_CWString;
	if (Heap_CString) delete Heap_CString;
	CleanupThreadData();
	PPLCore=NULL;
}

Cppl6Core *PPLInit()
{
	GlobalMutex.Lock();
	if (PPLCore) {
		GlobalMutex.Unlock();
		return PPLCore;
	}
	PPLCore=new Cppl6Core;
	if (!PPLCore) {
		return NULL;
	}
	atexit(PPLExit);
	GlobalMutex.Unlock();
	return PPLCore;
}



Cppl6Core::Cppl6Core()
{
	if (!coreref) {
		ppl6::GetCPUCaps(&cpucaps);
	}
	coreref++;
}

Cppl6Core::~Cppl6Core()
{
	coreref--;
	if (coreref<1) {
	}
}

CHeap *Cppl6Core::GetHeap_CString()
/*!\brief Pointer auf den internen Heap-Speicher der CString-Klasse
 *
 * Diese Funktion liefert einen Pointer auf den Heap-Speicher zurück, der von der
 * Klasse CString benutzt wird. Wurde der Heap noch nicht angelegt, wird dies zuerst
 * durchgeführt. Der Heap wird bei Programmende automatisch freigegeben.
 *
 * Die Klasse ist Thread-sicher.
 *
 * \returns Pointer auf Heap der CString-Klasse
 *
 * \attention
 * Der Heap sollte von extrenen Anwendungen nicht verwendet und keinesfalls verändert werden!
 *
 * \see
 * - CHeap
 */
{
	GlobalMutex.Lock();
	if (!Heap_CString) {
		Heap_CString=new CHeap;
		if (!Heap_CString) {
			SetError(2);
		} else {
			Heap_CString->Name="CString Heap";
			if (!Heap_CString->Init(sizeof(CString),1024,2048)) {
				PushError();
				delete Heap_CString;
				Heap_CString=NULL;
				PopError();
			}
		}
	}
	GlobalMutex.Unlock();
	return Heap_CString;
}

CHeap *Cppl6Core::GetHeap_CWString()
/*!\brief Pointer auf den internen Heap-Speicher der CWString-Klasse
 *
 * Diese Funktion liefert einen Pointer auf den Heap-Speicher zurück, der von der
 * Klasse CWString benutzt wird. Wurde der Heap noch nicht angelegt, wird dies zuerst
 * durchgeführt. Der Heap wird bei Programmende automatisch freigegeben.
 *
 * Die Klasse ist Thread-sicher.
 *
 * \returns Pointer auf Heap der CWString-Klasse
 *
 * \attention
 * Der Heap sollte von extrenen Anwendungen nicht verwendet und keinesfalls verändert werden!
 *
 * \see
 * - CHeap
 */
{
	GlobalMutex.Lock();
	if (!Heap_CWString) {
		Heap_CWString=new CHeap;
		if (!Heap_CWString) {
			SetError(2);
		} else {
			Heap_CWString->Name="CWString Heap";
			if (!Heap_CWString->Init(sizeof(CWString),1024,2048)) {
				PushError();
				delete Heap_CWString;
				Heap_CWString=NULL;
				PopError();
			}
		}
	}
	GlobalMutex.Unlock();
	return Heap_CWString;
}

CHeap *Cppl6Core::GetHeap_CArrayItem()
/*!\brief Pointer auf den internen Heap-Speicher der CArrayItem-Klasse
 *
 * Diese Funktion liefert einen Pointer auf den Heap-Speicher zurück, der von der
 * Klasse CArrayItem, bzw. CAssocArray benutzt wird, um die einzelnen Knoten des Baums
 * zu speichern. Wurde der Heap noch nicht angelegt, wird dies zuerst
 * durchgeführt. Der Heap wird bei Programmende automatisch freigegeben.
 *
 * Die Klasse ist Thread-sicher.
 *
 * \returns Pointer auf Heap der CArrayItem-Klasse
 *
 * \attention
 * Der Heap sollte von extrenen Anwendungen nicht verwendet und keinesfalls verändert werden!
 *
 * \see
 * - CHeap
 */
{
	GlobalMutex.Lock();
	if (!Heap_CArrayItem) {
		Heap_CArrayItem=new CHeap;
		if (!Heap_CArrayItem) {
			SetError(2);
		} else {
			Heap_CArrayItem->Name="CAssocArray Item-Heap";
			if (!Heap_CArrayItem->Init(sizeof(CAssocArray),8192,8192)) {
				PushError();
				delete Heap_CArrayItem;
				Heap_CArrayItem=NULL;
				PopError();
			}
		}
	}
	GlobalMutex.Unlock();
	return Heap_CArrayItem;
}

CHeap *Cppl6Core::GetHeap_TREEITEMS()
/*!\brief Pointer auf den internen Heap-Speicher für TREEITEMS
 *
 * Diese Funktion liefert einen Pointer auf den Heap-Speicher zurück, der von der
 * Klasse CAVLTree benutzt wird, um die einzelnen Knoten des Baums
 * zu speichern. Wurde der Heap noch nicht angelegt, wird dies zuerst
 * durchgeführt. Der Heap wird bei Programmende automatisch freigegeben.
 *
 * Die Klasse ist Thread-sicher.
 *
 * \returns Pointer auf Heap der CAVLTree-Klasse
 *
 * \attention
 * Der Heap sollte von extrenen Anwendungen nicht verwendet und keinesfalls verändert werden!
 *
 * \see
 * - CHeap
 */
{
	GlobalMutex.Lock();
	if (!Heap_TREEITEMS) {
		Heap_TREEITEMS=new CHeap;
		if (!Heap_TREEITEMS) {
			SetError(2);
		} else {
			Heap_TREEITEMS->Name="TREEITEM";
			if (!Heap_TREEITEMS->Init(sizeof(TREEITEM),8192,8192)) {
				PushError();
				delete Heap_TREEITEMS;
				Heap_TREEITEMS=NULL;
				PopError();
			}
		}
	}
	GlobalMutex.Unlock();
	return Heap_TREEITEMS;
}


ppluint32 Cppl6Core::GetCPUCaps(CPUCaps *cpu)
{
	if (cpu) return ppl6::GetCPUCaps(cpu);
	return cpucaps.caps;
}

const CPUCaps *Cppl6Core::GetCPUCaps()
{
	return (const CPUCaps*) &cpucaps;
}

CString Cppl6Core::GetCopyright()
{
	CString s=PPL_COPYRIGHT;
	return s;
}

CString Cppl6Core::GetVersion()
{
	CString s;
	s.Setf("%u.%u.%u",PPL_VERSION_MAJOR,PPL_VERSION_MINOR,PPL_VERSION_BUILD);
	return s;
}

CString Cppl6Core::GetReleaseDate()
{
	CString s;
	char t[20];
	long2dat(t,PPL_RELEASEDATE);
	s.Set(t);
	return s;
}


void Cppl6Core::GetVersion(int *major, int *minor, int *build)
{
	*major=PPL_VERSION_MAJOR;
	*minor=PPL_VERSION_MINOR;
	*build=PPL_VERSION_BUILD;
}

/*!\brief Überprüft die Version der Library
 *
 * Diese Funktion überprüft, ob die verwendete PPL-Library identisch oder
 * größer der angegebenen Version ist.
 * \param major Die Hauptversion, z.B. 6
 * \param minor Die Unterversion, z.B. 0
 * \param build Die Build, z.B. 14
 * \return Gibt \c -1 zurück, wenn die Library kleiner ist, \c 0 bei identischer Version,
 * \c 1 bei höherer Version. Bei abweichender Version wird zusätzlich der Fehlercode 352
 * gesetzt.
 */
int Cppl6Core::MinVersion (int major, int minor, int build)
{
	int lib=((PPL_VERSION_MAJOR<<16)+(PPL_VERSION_MINOR<<8)+PPL_VERSION_BUILD);
	int req=((major<<16)+(minor<<8)+build);
	if (lib==req) return 0;
	SetError(352,"Erwartet: %i.%i.%i, Ist: %i.%i.%i - Bitte das Programm gegen die richtige PPL-Library linken",
		major,minor,build,
		PPL_VERSION_MAJOR, PPL_VERSION_MINOR, PPL_VERSION_BUILD);
	if (lib>req) return 1;
	return -1;
}

ppluint32 Cppl6Core::GetReleaseDateInt()
{
	return PPL_RELEASEDATE;
}


/*
void Cppl6Core::EnableMemoryDebug(bool flag)
{
	if (flag==memdebug) return;
	memdebug=flag;
	if (!flag) {
		Printf ("*** Memory Debugging wird deaktiviert ***");
		MemoryDebugLog();
		MemoryDebugClearTable();
	} else {
		Printf ("*** Memory Debugging aktiviert ***");
	}
}


void Cppl6Core::MemoryLog()
{
	MemoryDebugLog();
}

void Cppl6Core::MemoryDump()
{
	MemoryDebugDump();
}
*/

int Cppl6Core::AtExit(int (*PPLExitFunction)(void *data), void *data)
{

	if (PPLExitFunction==NULL) {
		SetError(194,"int Cppl6Core::AtExit(==> int PPLExitFunction(void *data) <== , void *data)");
		return 0;
	}

	EXITFUNCTIONS *fn=(EXITFUNCTIONS*)malloc(sizeof(EXITFUNCTIONS));
	if (!fn) {
		SetError(2);
		return 0;
	}
	fn->data=data;
	fn->PPLExitFunction=PPLExitFunction;
	GlobalMutex.Lock();
	fn->next=last_exit;
	last_exit=fn;
	GlobalMutex.Unlock();
	return 1;
}

/*
void MemoryDebugAdd(void *ptr, size_t size, char *name, char *descr)
{
	MEMENTRY *e=(MEMENTRY *)::malloc(sizeof(MEMENTRY));
	if (!e) return;
	e->descr=descr;
	if (name) e->name=::strdup(name);
	else e->name=NULL;
	e->ptr=ptr;
	e->size=size;
	e->thread=GetThreadID();
	e->next=NULL;
	e->previous=NULL;

	MemMutex.Lock();
	usedmem+=size;
	memref++;
	if (!mem_first) {
		mem_first=mem_last=e;
	} else {
		mem_last->next=e;
		e->previous=mem_last;
		mem_last=e;
	}
	MemMutex.Unlock();
}

void MemoryDebugUpdate(void *ptr_old, void *ptr_new, size_t size, char *name, char *descr)
{
	MemMutex.Lock();
	MEMENTRY *e=(MEMENTRY *)mem_last;
	while (e) {
		if (e->ptr==ptr_old) {
			usedmem-=e->size;
			usedmem+=size;
			e->size=size;
			e->ptr=ptr_new;
			if (descr) {
				if (e->descr) ::free(e->descr);
				e->descr=descr;
			}
			if (e->name) ::free(e->name);
			if (name) e->name=::strdup(name);
			break;
		}
		e=e->previous;
	}
	MemMutex.Unlock();
}

void MemoryDebugDelete(void *ptr, char *name, char *descr)
{
	MemMutex.Lock();
	MEMENTRY *e=(MEMENTRY *)mem_last;
	while (e) {
		if (e->ptr==ptr) {
			usedmem-=e->size;
			memref--;
			if (e->previous) e->previous->next=e->next;
			if (e->next) e->next->previous=e->previous;
			if (e==mem_first) mem_first=e->next;
			if (e==mem_last) mem_last=e->previous;
			if (e->descr) ::free(e->descr);
			if (e->name) ::free(e->name);
			::free(e);
			break;
		}
		e=e->previous;
	}
	MemMutex.Unlock();
}

void MemoryDebugClearTable()
{
	MemMutex.Lock();
	MEMENTRY *mem,*e=(MEMENTRY *)mem_last;

	while (e) {
		mem=e;
		e=e->previous;
		if (e->descr) ::free(e->descr);
		if (e->name) ::free(e->name);
		::free(e);
	}
	usedmem=0;
	memref--;
	MemMutex.Unlock();
}

void MemoryDebugRemoveThread()
{
	ppluint64 t=GetThreadID();
	MemMutex.Lock();
	MEMENTRY *e=(MEMENTRY *)mem_first;
	int c=0;
	while (e) {
		if (e->thread==t) {
			if (!c) {
				printf ("Memory-Error in Thread %llu! Folgende Speicherbloecke sind noch allokiert:\n",t);
				c++;
			}
			printf (" %llx: ",(ppluint64)e->ptr);
			printf ("%10llu Bytes, ",(ppluint64)e->size);
			printf ("%s:",e->name);
			printf ("%s\n",e->descr?e->descr:"unknown");
		}
		e=e->next;
	}
	MemMutex.Unlock();
}

void MemoryDebugLog()
{
	if (!PPLCore) return;
	MEMENTRY *e=(MEMENTRY *)mem_first;
	PPLCore->Printf("LogMemory: %llu Bytes allocated in %llu References\n",
		usedmem,memref);
	while (e) {
		PPLCore->Printf("  %8llx: Thread: %10llu, %10llu Bytes, %s:%s\n",
			(ppluint64)e->ptr, (ppluint64)e->thread, (ppluint64)e->size,e->name,e->descr?e->descr:"unknown");
		e=e->next;
	}
}

void MemoryDebugDump()
{
	MEMENTRY *e=(MEMENTRY *)mem_first;
	printf("LogMemory: %llu Bytes allocated in %llu References\n",
		usedmem,memref);
	while (e) {
		printf("  %8llx: Thread: %10llu, %10llu Bytes, %s:%s\n",
			(ppluint64)e->ptr, (ppluint64)e->thread, (ppluint64)e->size,e->name,e->descr?e->descr:"unknown");
		e=e->next;
	}
}
*/
/*
CpplMemory::CpplMemory()
{
	#ifdef DEBUG
	this->name=NULL;
	#endif
}

CpplMemory::~CpplMemory()
{
	#ifdef DEBUG
	if (this->name) ::free(this->name);
	#endif
}

void CpplMemory::SetMemoryName(const char *name)
{
	#ifdef DEBUG
	if (this->name) ::free(this->name);
	//this->name=::strdup(name);
	#endif
}

const char *CpplMemory::GetMemoryName()
{
	#ifdef DEBUG
	if (!name) return "unknown";
	return (const char*)name;
	#endif
	return "No DEBUG enabled";
}

void *CpplMemory::calloc(size_t nmemb, size_t size, char *debug, ...)
{
	#ifdef DEBUG
		void *mem=::calloc(nmemb,size);
		if (!memdebug) return mem;
		if (!mem) return mem;
		char *buff=NULL;
		if (debug) {
			va_list args;
			va_start(args, debug);
			vasprintf (&buff, debug, args);
			va_end(args);
		}
		MemoryDebugAdd(mem,size*nmemb,name,buff);
		return mem;
	#endif
	return ::calloc(nmemb,size);
}

void *CpplMemory::malloc(size_t size, char *debug, ...)
{
	#ifdef DEBUG
		void *mem=::malloc(size);
		if (!memdebug) return mem;
		if (!mem) return mem;
		char *buff=NULL;
		if (debug) {
			va_list args;
			va_start(args, debug);
			vasprintf (&buff, debug, args);
			va_end(args);
		}
		MemoryDebugAdd(mem,size,name,buff);
		return mem;
	#endif
	return ::malloc(size);
}

void CpplMemory::free(void *ptr, char *debug, ...)
{
	#ifdef DEBUG
		::free(ptr);
		if (!memdebug) return;
		if (!ptr) return;
		char *buff=NULL;
		if (debug) {
			va_list args;
			va_start(args, debug);
			vasprintf (&buff, debug, args);
			va_end(args);
		}
		MemoryDebugDelete(ptr,name,buff);
		return;
	#endif
	return ::free(ptr);
}

void *CpplMemory::realloc(void *ptr, size_t size, char *debug, ...)
{
	#ifdef DEBUG
		void *mem=::realloc(ptr,size);
		if (!memdebug) return mem;
		char *buff=NULL;
		if (debug) {
			va_list args;
			va_start(args, debug);
			vasprintf (&buff, debug, args);
			va_end(args);
		}
		if (!ptr) MemoryDebugAdd(mem,size,name,buff);
		else MemoryDebugUpdate(ptr,mem,size,name,buff);
		return mem;

	#endif
	return ::realloc(ptr,size);
}

char *CpplMemory::strdup(const char *s, char *debug, ...)
{
	#ifdef DEBUG
		char *mem=::strdup(s);
		if (!memdebug) return mem;
		if (!mem) return mem;
		size_t size=strlen(s);
		char *buff=NULL;
		if (debug) {
			va_list args;
			va_start(args, debug);
			vasprintf (&buff, debug, args);
			va_end(args);
		}
		MemoryDebugAdd(mem,size,name,buff);
		return mem;
	#endif
	return ::strdup(s);
}

*/


int Cppl6Core::haveMCrypt()
/*!\ingroup PPL6_Features
 * \brief Feature MCrypt vorhanden? (Verschlüsselung mit CMCrypt)
 *
 * Mit den statischen Funktionen Cppl6Core::have... wird überprüft, ob beim Kompilieren
 * der PPL-Library ein bestimmtes Feature mit eingebunden wurde und somit zur Laufzeit
 * genutzt werden kann.
 *
  * @return Die Funktion liefert 1 zurück, wenn das abgefragte Feature unterstützt wird,
 * andernfalls 0. Es wird kein Fehlercode gesetzt.
 */
{
#ifdef HAVE_LIBMCRYPT
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveMHash()
/*!\ingroup PPL6_Features
 * \brief Feature MHash vorhanden? (Verschiedene Hash-Algorithmen)
 *
 * Mit den statischen Funktionen Cppl6Core::have... wird überprüft, ob beim Kompilieren
 * der PPL-Library ein bestimmtes Feature mit eingebunden wurde und somit zur Laufzeit
 * genutzt werden kann.
 *
  * @return Die Funktion liefert 1 zurück, wenn das abgefragte Feature unterstützt wird,
 * andernfalls 0. Es wird kein Fehlercode gesetzt.
 */
{
#ifdef HAVE_LIBMHASH
	return 1;
#else
	return 0;
#endif
}


int Cppl6Core::haveIconv()
/*!\ingroup PPL6_Features
 * \brief Feature Iconv vorhanden? (Umwandlung verschiedener Charsets)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_ICONV
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveZlib()
/*!\ingroup PPL6_Features
 * \brief Feature ZLib vorhanden? (Komprimierung mit ZLib)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_LIBZ
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveBzip2()
/*!\ingroup PPL6_Features
 * \brief Feature BZip2 vorhanden? (Komprimierung mit BZip2)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_BZIP2
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::havePNG()
/*!\ingroup PPL6_Features
 * \brief Feature PNG vorhanden? (Lesen und Schreiben von PNG-Bildern)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_PNG
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveJPEG()
/*!\ingroup PPL6_Features
 * \brief Feature JPEG vorhanden? (Lesen und Schreiben von JPEG-Bildern)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_JPEG
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveFreetype2()
/*!\ingroup PPL6_Features
 * \brief Feature Freetype2 vorhanden? (Unterstützung von TrueType-Fonts)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_FREETYPE2
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveSDL()
/*!\ingroup PPL6_Features
 * \brief Feature SDL vorhanden? (Unterstützung der Grafiklibrary SDL)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_SDL
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveLame()
/*!\ingroup PPL6_Features
 * \brief Feature Lame vorhanden? (Erstellung von MP3-Dateien)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_LAME
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveLibmad()
/*!\ingroup PPL6_Features
 * \brief Feature Libmad vorhanden? (Dekodieren von MP3-Dateien)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_LIBMAD
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::havePCRE()
/*!\ingroup PPL6_Features
 * \brief Feature PCRE vorhanden? (Perl Regular Expressions in den String-Klassen)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_PCRE
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveMySQL()
/*!\ingroup PPL6_Features
 * \brief Feature MySQL vorhanden? (Unterstützung von MySQL-Datenbanken)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_MYSQL
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveSybase()
/*!\ingroup PPL6_Features
 * \brief Feature Sybase vorhanden? (Unterstützung von Sybase-Datenbanken)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_SYBASE
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::havePostgres()
/*!\ingroup PPL6_Features
 * \brief Feature Postgres vorhanden? (Unterstützung von Postgres-Datenbanken)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_POSTGRES
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveThreads()
/*!\ingroup PPL6_Features
 * \brief Feature Threads vorhanden? (Unterstützung von Threads)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_PTHREADS
	return 1;
#elif defined _WIN32
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveCurl()
/*!\ingroup PPL6_Features
 * \brief Feature Curl vorhanden? (Zugriff auf Webseiten via HTTP)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_LIBCURL
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveOpenSSL()
/*!\ingroup PPL6_Features
 * \brief Feature OpenSSL vorhanden? (Verschlüsselung innerhalb Curl und den Socket-Klassen)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_OPENSSL
	return 1;
#else
	return 0;
#endif
}

int Cppl6Core::haveDirectX9()
/*!\ingroup PPL6_Features
 * \brief Feature DirectX9 vorhanden? (Unterstützung von DirectX 9 unter Windows)
 * \copydetails Cppl6Core::haveMCrypt
 */
{
#ifdef HAVE_OPENSSL
	return 1;
#else
	return 0;
#endif
}

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif
#ifdef HAVE_BZIP2
#include <bzlib.h>
#endif

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#ifdef HAVE_PNG
#include <png.h>
#endif

#ifdef HAVE_JPEG
#include <jpeglib.h>
#endif

#ifdef HAVE_FREETYPE2
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#ifdef HAVE_PCRE
#include <pcre.h>
#endif

#ifdef HAVE_MYSQL
#ifdef MINGW32
#define __LCC__
#endif
#include <mysql.h>
#endif

#ifdef HAVE_LAME
#ifdef HAVE_LAME_LAME_H
#include <lame/lame.h>
#else
#include <lame.h>
#endif
#endif

#ifdef HAVE_SDL
#include <SDL/SDL_version.h>
#endif

#ifdef HAVE_LIBMAD
#include <mad.h>
#endif

#ifdef HAVE_LIBMCRYPT
#include <mcrypt.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/opensslv.h>
#endif

#ifdef HAVE_LIBCURL
#include <curl/curlver.h>
#endif

#ifdef HAVE_LIBMHASH
#undef PACKAGE_VERSION
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#include <mutils/mhash_config.h>
#endif

void  Cppl6Core::GetFeatures(CAssocArray &list)
{
	CString Tmp;
#ifdef HAVE_LIBZ
	list.Set("zlib/name","Zlib");
	list.Set("zlib/copyright","Copyright (C) 1995-2005 Jean-loup Gailly and Mark Adler");
	list.Setf("zlib/version","%s",ZLIB_VERSION);
#endif
#ifdef HAVE_BZIP2
	Tmp=BZ2_bzlibVersion();
	Tmp.Cut(",");
	list.Set("bzip2/name","Bzip2");
	list.Set("bzip2/copyright","Copyright (C) 1996-2007 Julian Seward <jseward@bzip.org>");
	list.Setf("bzip2/version","%s",(const char*)Tmp);

#endif

	#ifdef HAVE_ICONV
#ifdef _LIBICONV_VERSION
	int major=_LIBICONV_VERSION>>8;
	int minor=_LIBICONV_VERSION&0xff;
	list.Set("iconv/copyright","Copyright (C) 1999-2003, 2005-2006 Free Software Foundation, Inc.");
	list.Set("iconv/name","iconv");
	list.Setf("iconv/version","%i.%i",major,minor);
#else
	list.Setf("iconv/version","unknown");
	list.Setf("iconv/name","iconv");
	list.Set("iconv/copyright","Copyright (C) 1999-2003, 2005-2006 Free Software Foundation, Inc.");
#endif
#endif
#ifdef HAVE_LIBMCRYPT
	list.Set("libmcrypt/copyright","Copyright (C) 1998,1999,2002 Nikos Mavroyanopoulos");
	list.Set("libmcrypt/name","libmcrypt");
	list.Setf("libmcrypt/version","%s",LIBMCRYPT_VERSION);
#endif

#ifdef HAVE_LIBMHASH
	list.Set("libmhash/copyright","Copyright (C) 1999-2001 Sascha Schumman, Nikos Mavroyanopoulos");
	list.Set("libmhash/name","libmhash");
#ifdef PACKAGE_VERSION
	list.Setf("libmhash/version","%s",PACKAGE_VERSION);
#else
	list.Set("libmhash/version","unknown");
#endif

#endif

#ifdef HAVE_PNG
	list.Set("libpng/copyright","Copyright (c) 1998-2009 Glenn Randers-Pehrson");
	list.Set("libpng/name","libpng");
	list.Setf("libpng/version","%s",PNG_LIBPNG_VER_STRING);
#endif

#ifdef HAVE_JPEG
	list.Set("libjpeg/copyright","Copyright (C) 1991-1998, Thomas G. Lane");
	list.Set("libjpeg/name","libjpeg");
	list.Setf("libjpeg/version","%i",JPEG_LIB_VERSION);
#endif
#ifdef HAVE_FREETYPE2
	list.Set("freetype2/copyright","Copyright by David Turner, Robert Wilhelm, and Werner Lemberg");
	list.Set("freetype2/name","Freetype2");
	list.Setf("freetype2/version","%i.%i.%i",FREETYPE_MAJOR,FREETYPE_MINOR,FREETYPE_PATCH);
#endif
#ifdef HAVE_PCRE
	list.Set("pcre/copyright","Copyright (c) 1997-2009 University of Cambridge");
	list.Set("pcre/name","PCRE library");
	list.Setf("pcre/version","%i.%i",PCRE_MAJOR,PCRE_MINOR);
#endif
#ifdef HAVE_MYSQL
	list.Set("mysql/copyright","Copyright (C) 2000-2003 MySQL AB");
	list.Set("mysql/name","MySQL");
	list.Setf("mysql/version","%s",MYSQL_SERVER_VERSION);
#endif
#ifdef HAVE_LAME
	list.Set("lame/copyright","Copyright (c) 1999 Mark Taylor");
	list.Set("lame/name","LAME MP3 encoder");
	list.Setf("lame/version","%s",get_lame_version());
#endif
#ifdef HAVE_LIBMAD
	list.Set("libmad/copyright","Copyright (C) 2000-2004 Underbit Technologies, Inc.");
	list.Set("libmad/name","libmad - MPEG audio decoder library");
	list.Setf("libmad/version","%s",MAD_VERSION);
#endif
#ifdef HAVE_SDL
	list.Set("sdl/copyright","Copyright (C) 1997-2006 Sam Lantinga");
	list.Set("sdl/name","SDL - Simple DirectMedia Layer");
	list.Setf("sdl/version","%i.%i.%i",SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);
#endif
#ifdef HAVE_LIBCURL
	list.Set("curl/copyright","Copyright (C) 1998 - 2009, Daniel Stenberg, <daniel@haxx.se>, et al.");
	list.Set("curl/name","libcurl");
	list.Setf("curl/version","%s",LIBCURL_VERSION);
#endif
#ifdef HAVE_OPENSSL
	list.Set("openssl/copyright","Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)");
	list.Set("openssl/name","OpenSSL");
	list.Setf("openssl/version","%s",OPENSSL_VERSION_TEXT);
#endif
}

} // EOF namespace ppl6
