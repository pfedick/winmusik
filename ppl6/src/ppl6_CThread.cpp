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

#ifdef HAVE_STDARG_H
	#include <stdarg.h>
#endif
#ifdef HAVE_STRINGS_h
	#include <strings.h>
#endif
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef HAVE_PTHREADS
	#include <pthread.h>
#endif

#ifdef HAVE_LIMITS_H
	#include <limits.h>
#endif

#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN		// Keine MFCs
	// ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.1031/sdkintro/sdkintro/using_the_sdk_headers.htm
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x500
	#endif
	#include <windows.h>
#endif

#include "ppl6.h"
//#define THREADDEBUG

#include "thread.h"

namespace ppl6 {

static ppluint64 global_thread_id=0;
#ifdef HAVE_PTHREADS
static pthread_key_t thread_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;
#endif
static CMutex GlobalThreadMutex;

#ifdef HAVE_PTHREADS
static void make_key()
{
	(void) pthread_key_create(&thread_key, NULL);
}
#endif


#ifdef _WIN32
CMutex Win32ThreadMutex;
DWORD Win32ThreadTLS=TLS_OUT_OF_INDEXES;
#endif

THREADDATA * GetThreadData()
/*!\ingroup PPLGroupThreads
 */
{
#ifdef _WIN32
	THREADDATA *ptr;
	Win32ThreadMutex.Lock();
	if (Win32ThreadTLS==TLS_OUT_OF_INDEXES) {
		Win32ThreadTLS=TlsAlloc();
		if (Win32ThreadTLS==TLS_OUT_OF_INDEXES) {
			Win32ThreadMutex.Unlock();
			::MessageBox(NULL,"TLS_OUT_OF_INDEXES","Error: ppl6::GetThreadData",MB_ICONERROR);
			exit(0);
		}
	}
	ptr=(THREADDATA*)TlsGetValue(Win32ThreadTLS);
	if (!ptr) {
		ptr = new THREADDATA;
		if (ptr) {
			ptr->thread=GetCurrentProcess();
			ptr->ThreadClass=NULL;
			GlobalThreadMutex.Lock();
			ptr->threadid=global_thread_id;
			global_thread_id++;
			GlobalThreadMutex.Unlock();
			ptr->threadid=0;
			ptr->Error.err=0;
			ptr->Error.suberr=0;
			ptr->Error.flags=0;
			ptr->mysql_thread_end=NULL;
			ptr->ErrorStack=NULL;
			if (!TlsSetValue(Win32ThreadTLS,ptr)) {
				Win32ThreadMutex.Unlock();
				::MessageBox(NULL,"TlsSetValue failed","Error: ppl6::GetThreadData",MB_ICONERROR);
				exit(0);
			}
		} else {
			Win32ThreadMutex.Unlock();
			::MessageBox(NULL,"Out of Memory","Error: ppl6::GetThreadData",MB_ICONERROR);
			exit(0);
		}
	}
	Win32ThreadMutex.Unlock();
	return ptr;
#elif defined HAVE_PTHREADS
	THREADDATA *ptr=NULL;
	(void) pthread_once(&key_once, make_key);
	if ((ptr = (THREADDATA*)pthread_getspecific(thread_key)) == NULL) {
		// Nur der erste Thread kann hier landen, oder Threads die manuell ohne
		// CThread erstellt wurden
		ptr = new THREADDATA;
		if (ptr) {
			ptr->thread=pthread_self();
			ptr->ThreadClass=NULL;
			GlobalThreadMutex.Lock();
			ptr->threadid=global_thread_id;
			global_thread_id++;
			GlobalThreadMutex.Unlock();
			ptr->mysql_thread_end=NULL;
			ptr->threadid=0;
			ptr->Error.err=0;
			ptr->Error.suberr=0;
			ptr->Error.flags=0;
			ptr->ErrorStack=NULL;
			pthread_attr_init(&ptr->attr);
			(void) pthread_setspecific(thread_key, ptr);
		}
	}
	return ptr;
#endif
	return NULL;
}

void CleanupThreadData()
{
#ifdef _WIN32
	THREADDATA *ptr;
	Win32ThreadMutex.Lock();
	if (Win32ThreadTLS==TLS_OUT_OF_INDEXES) {
		Win32ThreadTLS=TlsAlloc();
		if (Win32ThreadTLS==TLS_OUT_OF_INDEXES) {
			Win32ThreadMutex.Unlock();
			::MessageBox(NULL,"TLS_OUT_OF_INDEXES","Error: ppl6::GetThreadData",MB_ICONERROR);
			exit(0);
		}
	}
	ptr=(THREADDATA*)TlsGetValue(Win32ThreadTLS);
	if (ptr) {
		while (ptr->ErrorStack) {
			ERRORSTACK *s=ptr->ErrorStack;
			ptr->ErrorStack=s->next;
			delete s;
		}
		if (ptr->mysql_thread_end) ptr->mysql_thread_end(ptr);
		delete ptr;
		TlsSetValue(Win32ThreadTLS,NULL);
	}
	Win32ThreadMutex.Unlock();
#elif defined HAVE_PTHREADS
	THREADDATA *ptr=NULL;
	(void) pthread_once(&key_once, make_key);
	if ((ptr = (THREADDATA*)pthread_getspecific(thread_key)) == NULL) return;

	pthread_attr_destroy(&ptr->attr);
	while (ptr->ErrorStack) {
		ERRORSTACK *s=ptr->ErrorStack;
		ptr->ErrorStack=s->next;
		delete s;
	}
	if (ptr->mysql_thread_end) ptr->mysql_thread_end(ptr);
	delete ptr;
	(void) pthread_setspecific(thread_key, NULL);

#endif
}

ppluint64 GetThreadID()
/*! \brief ThreadID zurückgeben
 * \ingroup PPLGroupThreads
 *
 * \desc
 * Diese Funktion liefert die ID des Threads zurück, innerhalb dessen der Aufruf der
 * Funktion erfolgt ist.
 *
 * \return Liefert einen 64-Bit-Wert mit der Thread-ID zurück. Der Hauptthread hat immer die ID 0
 * \see \ref PPLGroupThreads
 */
{
	THREADDATA *t=GetThreadData();
	if (t) {
		return t->threadid;
	}
	return 0;
}

PPL_ERROR *GetErrorThreadPtr()
/*!\ingroup PPLGroupThreads
 */
{
	THREADDATA *t=GetThreadData();
	if (t) {
		return &t->Error;
	}
	return NULL;
}

void PushError()
/*! \brief Der zuletzt aufgetretene Fehler wird auf den Stack geschrieben
 * \ingroup Errors
 *
 * Mit diesem Befehl wird der zuletzt aufgetretene Fehler für den aktuellen Thread auf einen
 * internen Stack geschrieben und somit für eine spätere Verwendung gespeichert. Nach einem
 * PushError(); muß immer ein PopError(); folgen, damit der Eintrag wieder aus dem Stack
 * gelöscht wird.
 * \see PopError
 */
{
	THREADDATA *thread=GetThreadData();
	if (!thread) return;
	ERRORSTACK *s=new ERRORSTACK;
	if (!s) return;
	s->err=thread->Error.err;
	s->suberr=thread->Error.suberr;
	s->ErrorText=thread->Error.ErrorText;
	s->next=thread->ErrorStack;
	thread->ErrorStack=s;
}

void PopError()
/*! \fn PopError()
 * \ingroup Errors
 * \brief Holt einen Fehler aus dem Stack
 *
 * Ein zuvor mit PushError(); gespeicherter Fehler wird aus dem Stack geholt und als
 * aktiver Fehler für diesen Thread gespeichert. Nach einem
 * PushError(); muß immer ein PopError(); folgen.
 * \see PushError
 */
{
	THREADDATA *thread=GetThreadData();
	if (!thread) return;
	ERRORSTACK *s=thread->ErrorStack;
	if (s) {
		thread->Error.err=s->err;
		thread->Error.suberr=s->suberr;
		thread->Error.ErrorText=s->ErrorText;
		thread->ErrorStack=s->next;
		delete s;
	}
}



int ThreadSetPriority(int priority)
/*! \brief Priorität des aktuellen Threads ändern
 * \ingroup PPLGroupThreads
 *
 * \ingroup PPLGroupThreadsPriority
 *
 */
{
#ifdef WIN32
	HANDLE h=GetCurrentProcess();
	int p=GetThreadPriority(h);
	switch(priority) {
		case THREAD_PRIORITY::LOWEST:
			p=THREAD_PRIORITY_LOWEST;
			break;
		case THREAD_PRIORITY::BELOW_NORMAL:
			p=THREAD_PRIORITY_BELOW_NORMAL;
			break;
		case THREAD_PRIORITY::NORMAL:
			p=THREAD_PRIORITY_NORMAL;
			break;
		case THREAD_PRIORITY::ABOVE_NORMAL:
			p=THREAD_PRIORITY_ABOVE_NORMAL;
			break;
		case THREAD_PRIORITY::HIGHEST:
			p=THREAD_PRIORITY_HIGHEST;
			break;
	}
	if (SetThreadPriority(h,p)) return 1;
	return 0;
#elif defined HAVE_PTHREADS
	struct sched_param s;
	pthread_t p=pthread_self();
	int policy,c;
	c=pthread_getschedparam(p,&policy,&s);
	if (c!=0) return 0;
	int min=sched_get_priority_min(policy);
	int max=sched_get_priority_max(policy);
	int normal=(min+max)/2;
	switch(priority) {
		case THREAD_PRIORITY::LOWEST:
			s.sched_priority=min;
			break;
		case THREAD_PRIORITY::BELOW_NORMAL:
			s.sched_priority=normal/2;
			break;
		case THREAD_PRIORITY::NORMAL:
			s.sched_priority=normal;
			break;
		case THREAD_PRIORITY::ABOVE_NORMAL:
			s.sched_priority=normal+normal/2;
			break;
		case THREAD_PRIORITY::HIGHEST:
			s.sched_priority=max;
			break;
		default:
			return 0;
	}
	c=pthread_setschedparam(p,policy,&s);
	if(c==0) return 1;
	return 0;
#else
	return 0;
#endif
}

int ThreadGetPriority()
/*! \brief Priorität des aktuellen Threads abfragen
 * \ingroup PPLGroupThreads
 *
 * \ingroup PPLGroupThreadsPriority
 */
{
#ifdef WIN32
	HANDLE h=GetCurrentProcess();
	int p=GetThreadPriority(h);
	switch(p) {
		case THREAD_PRIORITY_LOWEST:
			return THREAD_PRIORITY::LOWEST;
		case THREAD_PRIORITY_BELOW_NORMAL:
			return THREAD_PRIORITY::BELOW_NORMAL;
		case THREAD_PRIORITY_NORMAL:
			return THREAD_PRIORITY::NORMAL;
		case THREAD_PRIORITY_ABOVE_NORMAL:
			return THREAD_PRIORITY::ABOVE_NORMAL;
		case THREAD_PRIORITY_HIGHEST:
			return THREAD_PRIORITY::HIGHEST;
	}
#elif defined HAVE_PTHREADS
	struct sched_param s;
	pthread_t p=pthread_self();
	int policy,c;
	c=pthread_getschedparam(p,&policy,&s);
	if(c!=0) return 0;
	int min=sched_get_priority_min(policy);
	int max=sched_get_priority_max(policy);
	int normal=(min+max)/2;

	if (s.sched_priority==normal) return THREAD_PRIORITY::NORMAL;
	if (s.sched_priority==min) return THREAD_PRIORITY::LOWEST;
	if (s.sched_priority==max) return THREAD_PRIORITY::HIGHEST;
	if (s.sched_priority<normal) return THREAD_PRIORITY::BELOW_NORMAL;
	if (s.sched_priority>normal) return THREAD_PRIORITY::ABOVE_NORMAL;
	return THREAD_PRIORITY::UNKNOWN;

#else
	return THREAD_PRIORITY::UNKNOWN;
#endif
	return THREAD_PRIORITY::UNKNOWN;
}

class FunctionThread : public CThread
{
	public:
		void (*start_routine)(void *);
		void *data;
		virtual void ThreadMain(void *param) {
			start_routine(data);
		}

};

ppluint64 StartThread(void (*start_routine)(void *),void *data)
{
	FunctionThread *t=new FunctionThread;
	t->start_routine=start_routine;
	t->data=data;
	t->ThreadDeleteOnExit(1);
	if (!t->ThreadStart(NULL)) {
		ppl6::PushError();
		delete t;
		ppl6::PopError();
		return 0;
	}
	return t->ThreadGetID();
}


/*!\class CThread
 * \ingroup PPLGroupThreads
 * \brief Klasse zum Verwalten von Threads
 *
 * Klasse zum Starten und Verwalten von Threads.
 * \see \ref PPLGroupThreads
 * \par Beispiel:
 * \include CThread_ThreadMain.cpp
 *
 */


CThread::CThread()
/*! \brief Konstruktor der Thread-Klasse
 *
 * Konstruktor der Thread-Klasse. Es werden interne Variablen allokiert und mit
 * Default-Werten gefüllt.
 *
 * \see \ref PPLGroupThreads
 */
{
	threadid=0;
	flags=0;
	myPriority=THREAD_PRIORITY::NORMAL;
	THREADDATA *t=new THREADDATA;
	threaddata=t;
	if (t) {
		t->thread=0;
		t->threadid=0;
		t->ThreadClass=this;
		t->Error.err=0;
		t->Error.suberr=0;
		t->Error.flags=0;
		t->ErrorStack=NULL;
		t->mysql_thread_end=NULL;
	}
	IsRunning=0;
	IsSuspended=0;
	deleteMe=0;
	#ifdef HAVE_PTHREADS
		pthread_attr_init(&t->attr);
	#endif
}

CThread::~CThread()
/*! \brief Destruktor der Thread-Klasse
 *
 * Falls der Thread noch läuft, wird er zunächst gestoppt. Anschließend werden die
 * internen Variablen wieder freigegeben.
 *
 * \see \ref PPLGroupThreads
 */
{
	PushError();
	ThreadStop();
	THREADDATA *t=(THREADDATA *)threaddata;
	#ifdef HAVE_PTHREADS
		pthread_attr_destroy(&t->attr);
	#endif
	PopError();
	// ErrorStack löschen
	while (t->ErrorStack) {
		ERRORSTACK *s=t->ErrorStack;
		t->ErrorStack=s->next;
		delete s;
	}
	if (t->mysql_thread_end) t->mysql_thread_end(t);
	delete t;
	threaddata=NULL;
}

void CThread::ThreadSuspend()
/*! \brief Der Thread soll pausieren
 *
 * ThreadSuspend setzt das Suspended Flag. Hat nur Auswirkungen, wenn dieses Flag in ThreadMain
 * beachtet wird.
 *
 * \todo Es wäre besser, wenn diese Funktion den Thread Betriebssystemseitig schlafen legen
 * würde, bis ein Resume gegeben wird.
 *
 * \see CThread::ThreadResume
 * \see CThread::ThreadWaitSuspended
 * \see \ref PPLGroupThreads
 */
{
	threadmutex.Lock();
	flags|=2;
	threadmutex.Unlock();
}

void CThread::ThreadResume()
/*! \brief Der Thread soll weitermachen
 *
 * Dem Thread wird signalisiert, daß er weitermachen soll.
 *
 * \todo Es wäre besser, wenn diese Funktion vom Betriebssystemseitig erledigt würde.
 *
 * \see CThread::ThreadSuspend
 * \see CThread::ThreadWaitSuspended
 * \see \ref PPLGroupThreads
 */
{
	threadmutex.Lock();
	flags=flags&~2;
	if (IsSuspended) {
		threadmutex.Unlock();
		threadmutex.Signal();
	} else {
		threadmutex.Unlock();
	}
}

void CThread::ThreadStop()
/*! \brief Der Thread wird gestoppt
 *
 * Dem Thread wird zunächst signalisiert, dass er stoppen soll. Anschließend wartet die
 * Funktion, bis der Thread sich beendet hat.
 *
 * \note Die Thread-Funktion muß in regelmäßigen Abständen mittels der Funktion
 * ThreadShouldStop überprüfen, ob er stoppen soll. Ist dies der Fall, muß sich die
 * Funktion beenden.
 *
 * \see CThread::ThreadSignalStop
 * \see CThread::ThreadShouldStop
 * \see \ref PPLGroupThreads
 */
{
	threadmutex.Lock();
	flags|=1;
	//THREADDATA *t=(THREADDATA *)threaddata;
	if (IsSuspended) {
		threadmutex.Unlock();
		ThreadResume();
		threadmutex.Lock();
	}
	while (IsRunning) {
		threadmutex.Unlock();
		MSleep(1);
		threadmutex.Lock();
	}
	flags=flags&0xfffffffe;
	threadmutex.Unlock();
}

void CThread::ThreadSignalStop()
/*! \brief Dem Thread signalisieren, dass er stoppen soll
 *
 * Dem Thread wird nur signalisiert, dass er stoppen soll.
 *
 * \see CThread::ThreadStop
 * \see CThread::ThreadShouldStop
 * \see \ref PPLGroupThreads
 */
{
	threadmutex.Lock();
	flags|=1;
	//THREADDATA *t=(THREADDATA *)threaddata;
	if (IsSuspended) {
		threadmutex.Unlock();
		ThreadResume();
	} else {
		threadmutex.Unlock();
	}
}

#ifdef _WIN32
	DWORD WINAPI ThreadProc(void *param)
	{
		CThread *thread=(CThread*)param;
		thread->ThreadStartUp();
		if (thread->ThreadShouldDeleteOnExit()) delete thread;
		return 0;
	}
#else
	static void *ThreadProc(void *param)
	{
		//printf ("ThreadProc\n");
		THREADDATA *t=(THREADDATA *)param;
		CThread *thread=t->ThreadClass;
		if (!t->threadid) {
			GlobalThreadMutex.Lock();
			t->threadid=global_thread_id;
			global_thread_id++;
			GlobalThreadMutex.Unlock();
		}
		(void) pthread_once(&key_once, make_key);
		(void) pthread_setspecific(thread_key, t);
		thread->ThreadStartUp();
		if (thread->ThreadShouldDeleteOnExit()) delete thread;
		#ifdef _WIN32
		#elif defined HAVE_PTHREADS
			pthread_exit(NULL);
		#endif
		return NULL;
	}
#endif

int CThread::ThreadStart(void *param)
/*! \brief Der Thread wird gestartet
 *
 * ThreadStart startet den Thread und kehrt sofort zur aufrufenden Funktion zurück.
 *
 * \param param Ein optionaler Pointer auf eine Datenstruktur, der an die ThreadMain-Funktion
 * übergeben wird.
 * \return Konnte der Thread erfolgreich gestartet werden, wird 1 zurückgegeben, bei einem Fehler 0.
 * Der Aufgetretene Fehler kann über die \link Errors Fehlerfunktionen \endlink
 * abgefragt werden.
 *
 * \see CThread::ThreadMain
 * \see \ref PPLGroupThreads
 */
{
	Cppl6Core *core=PPLInit();
	if (!core) {
		SetError(355);
		return 0;
	}
	this->param=param;
	THREADDATA *t=(THREADDATA *)threaddata;
	threadmutex.Lock();
	IsRunning=1;
	IsSuspended=0;
	threadmutex.Unlock();
	#ifdef _WIN32
		t->thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&t->dwThreadID);
		if (t->thread!=NULL) {
			return 1;
		}
		threadmutex.Lock();
		IsRunning=0;
		threadmutex.Unlock();

	#elif defined HAVE_PTHREADS
		int ret=pthread_create(&t->thread,&t->attr,ThreadProc,t);
		if(ret==0) {
			if (pthread_detach(t->thread)==0) return 1;
			// TODO: Wenn pthread_detach fehlschlägt, kann der Thread trotzdem
			// laufen
		}
		threadmutex.Lock();
		IsRunning=0;
		threadmutex.Unlock();
	#else

	#endif
	return 0;
}

void CThread::ThreadIdle()
/*!\brief Der Thread gibt seine CPU-Zeit an andere Threads ab
 */
{
#ifdef _WIN32
#elif defined HAVE_PTHREADS
	#ifdef SOLARIS
	#else
		pthread_yield();
	#endif
#endif
}

void CThread::ThreadStartUp()
/*! \brief Interne Funktion
 *
 * ThreadStartUp wird unmittelbar nach Starten des Threads aufgerufen. Hier werden einige
 * Variablen initialisiert und dann ThreadMain aufgerufen.
 *
 * \note Diese Funktion wird intern verwendet und sollte nicht vom Anwender aufgerufen
 * werden
 *
 * \see CThread::ThreadMain
 * \see \ref PPLGroupThreads
 */
{
	threadmutex.Lock();
	threadid=GetThreadID();
	IsRunning=1;
	IsSuspended=0;
	threadmutex.Unlock();
	ThreadSetPriority(myPriority);

	THREADDATA * d=GetThreadData();

	ThreadMain(param);


	if (d) {
#ifdef HAVE_MYSQL
		if (d->mysql_thread_end!=NULL) d->mysql_thread_end(d);
#endif

	}

	threadmutex.Lock();
	flags=0;
	IsRunning=0;
	IsSuspended=0;
	//MemoryDebugRemoveThread();
	threadmutex.Unlock();
}

void CThread::ThreadDeleteOnExit(int flag)
/*! \brief Flag setzen: Klasse beim Beenden löschen
 *
 * Dem Thread wird mitgeteilt, ob er beim beenden seine eigene Klasse löschen soll. Der
 * Default ist, dass der Anwender selbst die Klasse löschen muß.
 *
 * \param flag kann entweder 1 (Klasse soll automatisch gelöscht werden) oder 0 sein
 * (Klasse nicht löschen).
 * \see CThread::ThreadDeleteOnExit
 * \see \ref PPLGroupThreads
 */
{
	if (flag) deleteMe=1;
	else deleteMe=0;
}

int  CThread::ThreadShouldDeleteOnExit()
/*! \brief Interne Funktion
 *
 * Diese Funktion wird intern beim beenden des Threads aufgerufen. Liefert sie "true" zurück,
 * wird die Thread-Klasse automatisch mit delete gelöscht.
 *
 * \return Liefert 1 zurück, wenn die Klasse gelöscht werden soll, sonst 0.
 * \see CThread::ThreadDeleteOnExit
 * \see \ref PPLGroupThreads
 */
{
	if (deleteMe) return 1;
	return 0;
}

int CThread::ThreadIsRunning()
/*! \brief Status abfragen: Läuft der Thread?
 *
 * Mit dieser Funktion kann überprüft werden, ob der Thread aktuell ausgeführt wird.
 *
 * \return Liefert 1 zurück, wenn der Thread läuft, sonst 0.
 * \see \ref PPLGroupThreads
 */
{
	int ret;
	threadmutex.Lock();
	ret=IsRunning;
	threadmutex.Unlock();
	return ret;
}

int CThread::ThreadIsSuspended()
/*! \brief Status abfragen: Schläft der Thread?
 *
 * Mit dieser Funktion kann überprüft werden, ob der Thread aktuell schläft.
 *
 * \return Liefert 1 zurück, wenn der Thread schläft, sonst 0.
 * \see \ref PPLGroupThreads
 */
{
	int ret;
	threadmutex.Lock();
	ret=IsSuspended;
	threadmutex.Unlock();
	return ret;
}

int CThread::ThreadGetFlags()
/*! \brief Flags des Thread auslesen
 *
 * Mit dieser Funktion können die internen Flags ausgelesen werden.
 *
 * \return Liefert den Wert der internen Flag-Variable zurück
 * \deprecated
 * Diese Funktion ist veraltet und sollte nicht mehr verwendet werden.
 * \see \ref PPLGroupThreads
 */
{
	int ret;
	threadmutex.Lock();
	ret=flags;
	threadmutex.Unlock();
	return ret;
}

int CThread::ThreadShouldStop()
/*! \brief Prüfen, ob der Thread beendet werden soll
 *
 * Diese Funktion liefert \c true zurück, wenn der Thread gestoppt werden soll.
 * Dies ist der Fall, wenn vorher die Funktion ThreadStop oder ThreadShouldStop
 * aufgerufen wurde.
 *
 * \return Liefert 1 zurück, wenn der Thread gestoppt werden soll, sonst 0.
 * \see CThread::ThreadStop
 * \see CThread::ThreadShouldStop
 * \see \ref PPLGroupThreads
 */
{
	int ret;
	threadmutex.Lock();
	ret=flags&1;
	threadmutex.Unlock();
	return ret;
}

void CThread::ThreadWaitSuspended(int msec)
/*! \brief Prüfen, ob der Thread schlafen soll
 *
 * ThreadWaitSuspended prüft, ob der Thread schlafen (suspend) soll, und wenn
 * ja, wartet sie solange, bis ein unsuspend oder stop signalisiert wird.
 * Der optionale Parameter gibt an, nach wievielen Millisekunden jeweils der Status
 * geprüft werden soll.
 *
 * \param msec Millisekunden, nach denen jeweils der Status geprüft werden soll.
 * Wird der Parameter nicht angegeben, wartet die Funktion so lange, bis entweder
 * die Funktion ThreadResume, TheadSignalStop, ThreadStop oder der Destruktor
 * der Klasse aufgerufen wird.
 *
 * \see CThread::ThreadSuspend
 * \see CThread::ThreadResume
 * \see \ref PPLGroupThreads
 */
{
	threadmutex.Lock();
	//THREADDATA *t=(THREADDATA *)threaddata;
	while ((flags&3)==2) {
		IsSuspended=1;
		threadmutex.Wait(msec);
	}
	IsSuspended=0;
	threadmutex.Unlock();
}

ppluint64 CThread::ThreadGetID()
/*! \brief ThreadID zurückgeben
 *
 * Diese Funktion liefert die interne ID des Threads zurück.
 *
 * \return Liefert einen 64-Bit-Wert mit der Thread-ID zurück.
 * \see \ref PPLGroupThreads
 */
{
	THREADDATA *t=(THREADDATA *)threaddata;
	if (!t) return 0;
	if (!t->threadid) {
		t->threadid=GetThreadID();
	}
	return t->threadid;
}


void CThread::ThreadMain(void *param)
/*!\brief Einsprungfunktion bei Start des Threads
 *
 * ThreadMain ist die Funktion, die nach Starten des Threads aufgerufen wird.
 * Sie muß von der abgeleiteten Klasse überschrieben werden und enthält den vom
 * Thread auszuführenden Code.
 *
 * \param param Ein optionaler Void-Pointer, der mit der Funktion ThreadStart
 * übergeben wurde.
 * \return Die Funktion liefert keinen Return-Wert, jedoch wird bei Verlassen
 * der Thread automatisch gestoppt. Wurde zuvor die Funktion CThread::ThreadShouldDeleteOnExit()
 * aufgerufen, wird außerdem die Klasse gelöscht.
 * \see \ref PPLGroupThreads
 * \par Example
 * \include CThread_ThreadMain.cpp
 */
{

}

int CThread::ThreadGetPriority()
/*! \brief Priorität des Threads auslesen
 * \ingroup PPLGroupThreadsPriority
 *
 * Liefert die Priorität des Threads zurück.
 *
 * \return liefert einen Wert zurück, der die Priorität des Threads angibt.
 * \see \ref PPLGroupThreads
 */
{
	THREADDATA *t=(THREADDATA *)threaddata;
#ifdef WIN32
	int p=GetThreadPriority(t->thread);
	switch(p) {
		case THREAD_PRIORITY_LOWEST:
			return THREAD_PRIORITY::LOWEST;
		case THREAD_PRIORITY_BELOW_NORMAL:
			return THREAD_PRIORITY::BELOW_NORMAL;
		case THREAD_PRIORITY_NORMAL:
			return THREAD_PRIORITY::NORMAL;
		case THREAD_PRIORITY_ABOVE_NORMAL:
			return THREAD_PRIORITY::ABOVE_NORMAL;
		case THREAD_PRIORITY_HIGHEST:
			return THREAD_PRIORITY::HIGHEST;
	}
#elif defined HAVE_PTHREADS
	struct sched_param s;
	int policy,c;
	c=pthread_getschedparam(t->thread,&policy,&s);
	if(c!=0) return 0;
	int min=sched_get_priority_min(policy);
	int max=sched_get_priority_max(policy);
	int normal=(min+max)/2;

	if (s.sched_priority==normal) return THREAD_PRIORITY::NORMAL;
	if (s.sched_priority==min) return THREAD_PRIORITY::LOWEST;
	if (s.sched_priority==max) return THREAD_PRIORITY::HIGHEST;
	if (s.sched_priority<normal) return THREAD_PRIORITY::BELOW_NORMAL;
	if (s.sched_priority>normal) return THREAD_PRIORITY::ABOVE_NORMAL;
	return THREAD_PRIORITY::UNKNOWN;

#else
	return THREAD_PRIORITY::UNKNOWN;
#endif

	return THREAD_PRIORITY::UNKNOWN;
}

int CThread::ThreadSetPriority(int priority)
/*! \brief Priorität des Threads ändern
 * \ingroup PPLGroupThreadsPriority
 *
 * Setz die Priorität des Threads
 * \param priority Gibt die Priorität des Threads an. Die möglichen Werte sind im
 * Kapitel \link PPLGroupThreadsPriority Thread Prioritäten \endlink beschrieben.
 * \return Liefert 1 zurück, wenn die Priorität erfolgreich geändert wurde, sonst 0.
 * \see \ref PPLGroupThreads
 */
{
	THREADDATA *t=(THREADDATA *)threaddata;
	myPriority=priority;
	if(!t->thread) return 1;
#ifdef WIN32
	int p=GetThreadPriority(t->thread);
	switch(priority) {
		case THREAD_PRIORITY::LOWEST:
			p=THREAD_PRIORITY_LOWEST;
			break;
		case THREAD_PRIORITY::BELOW_NORMAL:
			p=THREAD_PRIORITY_BELOW_NORMAL;
			break;
		case THREAD_PRIORITY::NORMAL:
			p=THREAD_PRIORITY_NORMAL;
			break;
		case THREAD_PRIORITY::ABOVE_NORMAL:
			p=THREAD_PRIORITY_ABOVE_NORMAL;
			break;
		case THREAD_PRIORITY::HIGHEST:
			p=THREAD_PRIORITY_HIGHEST;
			break;
	}
	if (SetThreadPriority(t->thread,p)) return 1;
	return 0;
#elif defined HAVE_PTHREADS
	struct sched_param s;
	int policy,c;
	c=pthread_getschedparam(t->thread,&policy,&s);
	if (c!=0) return 0;
	int min=sched_get_priority_min(policy);
	int max=sched_get_priority_max(policy);
	int normal=(min+max)/2;
	switch(priority) {
		case THREAD_PRIORITY::LOWEST:
			s.sched_priority=min;
			break;
		case THREAD_PRIORITY::BELOW_NORMAL:
			s.sched_priority=normal/2;
			break;
		case THREAD_PRIORITY::NORMAL:
			s.sched_priority=normal;
			break;
		case THREAD_PRIORITY::ABOVE_NORMAL:
			s.sched_priority=normal+normal/2;
			break;
		case THREAD_PRIORITY::HIGHEST:
			s.sched_priority=max;
			break;
		default:
			return 0;
	}
	c=pthread_setschedparam(t->thread,policy,&s);
	if(c==0) return 1;
	return 0;
#else
	return 0;
#endif

}

int CThread::ThreadSetStackSize(size_t size)
/*! \brief Stack-Größe des Threads setzen
 * \ingroup PPLGroupThreadsStacksize
 *
 * \see \ref PPLGroupThreadsStacksize
 * \see \ref PPLGroupThreads
 */
{
	#ifdef HAVE_PTHREADS
		#ifndef _POSIX_THREAD_ATTR_STACKSIZE
			SetError(315);
			return 0;
		#endif
		THREADDATA *t=(THREADDATA *)threaddata;
		if (size==0) size=PTHREAD_STACK_MIN;
		if (size<(size_t)PTHREAD_STACK_MIN) {
			SetError(314,PTHREAD_STACK_MIN,"Stacksize muss mindestens %u Byte sein",PTHREAD_STACK_MIN);
			return 0;
		}
		if (pthread_attr_setstacksize(&t->attr,size)==0) return 1;
	#endif
	return 0;
}

size_t CThread::ThreadGetMinimumStackSize()
/*! \brief Minimale Stack-Größe auslesen
 * \ingroup PPLGroupThreadsStacksize
 *
 * \see \ref PPLGroupThreadsStacksize
 * \see \ref PPLGroupThreads
 */
{
	#ifdef HAVE_PTHREADS
		#ifndef _POSIX_THREAD_ATTR_STACKSIZE
			SetError(315);
			return 0;
		#endif
		return PTHREAD_STACK_MIN;
		#endif
	return 0;
}

size_t CThread::ThreadGetStackSize()
/*! \brief Stack-Größe des Threads auslesen
 * \ingroup PPLGroupThreadsStacksize
 *
 * \see \ref PPLGroupThreadsStacksize
 * \see \ref PPLGroupThreads
 */
{
	#ifdef HAVE_PTHREADS
		#ifndef _POSIX_THREAD_ATTR_STACKSIZE
			SetError(315);
			return 0;
		#endif
		THREADDATA *t=(THREADDATA *)threaddata;
		size_t s;
		if (pthread_attr_getstacksize(&t->attr,&s)==0) return s;
	#endif
	return 0;
}



/*********************************************************************************
 * ThreadPool
 *********************************************************************************/

/*!\class CThreadPool
 * \ingroup PPLGroupThreads
 * \ingroup PPLGroupThreadsCThreadPool
 */

typedef struct tagThreadEntry {
	struct	tagThreadEntry *next, *previous;
	CThread *thread;
} THREADENTRY;

CThreadPool::CThreadPool()
{
	first=last=NULL;
	count=0;
}

CThreadPool::~CThreadPool()
{
	Clear();
}

void CThreadPool::Clear()
{
	mutex.Lock();
	THREADENTRY *t,*e=(THREADENTRY*)first;
	while (e) {
		t=e;
		e=e->next;
		free(t);
	}
	first=last=NULL;
	count=0;
	mutex.Unlock();
}

int CThreadPool::AddThread(CThread *thread)
{
	THREADENTRY *e=(THREADENTRY*)malloc(sizeof(THREADENTRY));
	if (!e) return 0;
	e->previous=NULL;
	e->next=NULL;
	e->thread=thread;
	mutex.Lock();
	if (!first) {
		first=last=e;
	} else {
		e->previous=(THREADENTRY *)last;
		((THREADENTRY *)last)->next=e;
		last=e;
	}
	count++;
	mutex.Unlock();
	return 1;
}

void CThreadPool::DeleteThread(CThread *thread)
{
	mutex.Lock();
	THREADENTRY *e=(THREADENTRY *)first;
	while (e) {
		if (e->thread==thread) {
			if (e->previous) e->previous->next=e->next;
			if (e->next) e->next->previous=e->previous;
			if (e==first) first=e->next;
			if (e==last) last=e->previous;
			free(e);
			count--;
			break;
		}
		e=e->next;
	}
	mutex.Unlock();
}

void CThreadPool::Suspend()
{
	mutex.Lock();
	THREADENTRY *e=(THREADENTRY *)first;
	while (e) {
		e->thread->ThreadSuspend();
		e=e->next;
	}
	mutex.Unlock();
}

void CThreadPool::Resume()
{
	mutex.Lock();
	THREADENTRY *e=(THREADENTRY *)first;
	while (e) {
		e->thread->ThreadResume();
		e=e->next;
	}
	mutex.Unlock();
}

void CThreadPool::Stop()
{
	SignalStop();		// Erst signalisieren, dann geht's schneller beim Stop
	while (true) {
		bool isRunning=false;
		mutex.Lock();
		THREADENTRY *e=(THREADENTRY *)first;
		while (e) {
			if (e->thread->ThreadIsRunning()) isRunning=true;
			e=e->next;
		}
		mutex.Unlock();
		if (!isRunning) break;
		MSleep(1);

	}
}

void CThreadPool::SignalStop()
{
	mutex.Lock();
	THREADENTRY *e=(THREADENTRY *)first;
	while (e) {
		e->thread->ThreadSignalStop();
		e=e->next;
	}
	mutex.Unlock();
}

int CThreadPool::Count()
{
	int ret=0;
	mutex.Lock();
	ret=count;
	mutex.Unlock();
	return ret;
}

void CThreadPool::DestroyThreads()
{
	SignalStop();		// Erst signalisieren, dann geht's schneller beim Stop
	mutex.Lock();
	THREADENTRY *e=(THREADENTRY *)first;
	THREADENTRY *t;
	while (e) {
		e->thread->ThreadStop();
		delete e->thread;
		t=e;
        e=e->next;
		free(t);
    }
	first=last=NULL;
	count=0;
    mutex.Unlock();

}

int CThreadPool::Lock()
{
	return mutex.Lock();
}

int CThreadPool::Unlock()
{
	return mutex.Unlock();
}

void CThreadPool::Reset()
{
	walk=first;
}

CThread *CThreadPool::GetFirst()
{
	walk=first;
	return GetNext();
}

CThread *CThreadPool::GetNext()
{
	THREADENTRY *e=(THREADENTRY *)walk;
	if (!e) return NULL;
	walk=e->next;
	return e->thread;
}

void CThreadPool::ChangePriority(int prio)
/*!\ingroup PPLGroupThreadsPriority
 */
{
	mutex.Lock();
	THREADENTRY *e=(THREADENTRY *)first;
	while (e) {
		e->thread->ThreadSetPriority(prio);
		e=e->next;
	}
	mutex.Unlock();
}


}	// EOF namespace ppl6

