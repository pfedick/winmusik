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
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#include "ppl6.h"


namespace ppl6 {

typedef struct tagLOGHANDLER
{
	CLogHandler *handler;
	struct tagLOGHANDLER *next;
	struct tagLOGHANDLER *previous;
} LOGHANDLER;

void CLogHandler::LogMessage(int facility, int level, const char *msg)
{

}

static const char *facilitylist[] = {
	"NONE   ",						// 0
	"EMERG  ",						// 1
	"ALERT  ",						// 2
	"CRIT   ",						// 3
	"ERROR  ",						// 4
	"WARNING",						// 5
	"NOTICE ",						// 6
	"INFO   ",						// 7
	"DEBUG  ",						// 8
	""
};



#ifdef HAVE_SYSLOG_H
static const int syslog_facility_lookup[] = {
		LOG_USER,				// = 0
		LOG_AUTH,				// SYSLOG_AUTH=1
		LOG_AUTHPRIV,			// SYSLOG_AUTHPRIV
#ifdef LOG_CONSOLE
		LOG_CONSOLE,			// SYSLOG_CONSOLE
#else
		LOG_USER,
#endif
		LOG_CRON,				// SYSLOG_CRON
		LOG_DAEMON,				// SYSLOG_DAEMON
		LOG_FTP,				// SYSLOG_FTP
		LOG_KERN,				// SYSLOG_KERN
		LOG_LPR,				// SYSLOG_LPR
		LOG_MAIL,				// SYSLOG_MAIL
		LOG_NEWS,				// SYSLOG_NEWS
#ifdef LOG_NTP					// SYSLOG_NTP
		LOG_NTP,
#else
		LOG_USER,
#endif
#ifdef LOG_SECURITY				// SYSLOG_SECURITY
		LOG_SECURITY,
#else
		LOG_USER,
#endif
		LOG_SYSLOG,				// SYSLOG_SYSLOG
		LOG_USER,				// SYSLOG_USER
		LOG_UUCP,				// SYSLOG_UUCP
		LOG_LOCAL0,				// SYSLOG_LOCAL0
		LOG_LOCAL1,				// SYSLOG_LOCAL1
		LOG_LOCAL2,				// SYSLOG_LOCAL2
		LOG_LOCAL3,				// SYSLOG_LOCAL3
		LOG_LOCAL4,				// SYSLOG_LOCAL4
		LOG_LOCAL5,				// SYSLOG_LOCAL5
		LOG_LOCAL6,				// SYSLOG_LOCAL6
		LOG_LOCAL7				// SYSLOG_LOCAL7

};

/*
static const int syslog_priority_lookup[] = {
		LOG_DEBUG,
		LOG_EMERG,
		LOG_ALERT,
		LOG_CRIT,
		LOG_ERR,
		LOG_WARNING,
		LOG_NOTICE,
		LOG_INFO,
		LOG_DEBUG
};
*/

#endif



CLog::CLog()
{
	firsthandler=lasthandler=NULL;
	logconsole=false;
	logThreadId=true;
	console_enabled=false;
	console_facility=LOG::DEBUG;
	console_level=1;
	for (int i=0;i<NUMFACILITIES;i++) {
		debuglevel[i]=0;
	}
	FilterModule=NULL;
	FilterFile=NULL;
	rotate_mechanism=0;
	maxsize=1024*1024*1024;
	generations=1;
	inrotate=false;
	useSyslog=false;
	syslogFacility=SYSLOG_USER;
}

CLog::~CLog()
{
	Terminate();
	if (FilterModule) delete FilterModule;
	if (FilterFile) delete FilterFile;
}

void CLog::Terminate()
{
	PushError();
	Printf(LOG::INFO,0,"ppl6::CLog","Terminate",__FILE__,__LINE__,"=== Logfile-Class terminated ===============================");
	for (int i=0;i<NUMFACILITIES;i++) {
		logff[i].Close();
	}
	mutex.Lock();
	LOGHANDLER *c,*h=(LOGHANDLER *)firsthandler;
	while (h) {
		c=h;
		h=h->next;
		free(c);
	}
	firsthandler=lasthandler=NULL;
	logconsole=false;
	logThreadId=true;
	console_enabled=false;
	console_facility=LOG::DEBUG;
	console_level=1;
	for (int i=0;i<NUMFACILITIES;i++) {
		debuglevel[i]=0;
	}
	mutex.Unlock();
#ifdef HAVE_CLOSELOG
	try {
		closeSyslog();
	} catch (...) {

	}
#endif
	PopError();
}



void CLog::SetConsole(bool flag, int facility, int level)
{
	mutex.Lock();
	console_enabled=flag;
	console_facility=facility;
	console_level=level;
	mutex.Unlock();
}

void CLog::openSyslog(const CString &ident,SYSLOG_FACILITY facility)
{
#ifndef HAVE_OPENLOG
	throw UnsupportedFeatureException("syslog");
#else
	if (useSyslog) {
		//print(Logger::INFO,0,"ppl7::Logger","openSyslog",__FILE__,__LINE__,"=== Reopen Syslog ===============================");
		closelog();
	}
	useSyslog=true;
	syslogIdent=ident;
	syslogFacility=facility;
	openlog(syslogIdent,LOG_NDELAY|LOG_PID,syslog_facility_lookup[facility]);
	Print(LOG::INFO,1,"ppl6::CLog","openSyslog",__FILE__,__LINE__,"=== Enable Syslog ===============================");
#endif
}

void CLog::closeSyslog()
{
#ifndef HAVE_CLOSELOG
	throw UnsupportedFeatureException("syslog");
#else
	if (useSyslog) {
		Print(LOG::INFO,0,"ppl6::CLog","closeSyslog",__FILE__,__LINE__,"=== Close Syslog ===============================");
		closelog();
		useSyslog=false;
	}
#endif
}



int CLog::SetLogfile(int facility, const char *filename)
{
	if (facility<1 || facility>=NUMFACILITIES) return 0;
	mutex.Lock();
	logff[facility].Close();
	if (filename) {
		if (logff[facility].Open(filename,"ab")) {
			logfilename[facility]=filename;
			mutex.Unlock();
			Printf(facility,0,"ppl6::CLog","SetLogfile",__FILE__,__LINE__,"=== Logfile started for %s",facilitylist[facility]);
			return 1;
		}
	}
	mutex.Unlock();
	return 0;
}

int CLog::SetLogRotate(ppluint64 maxsize, int generations)
{
	if (generations>1 && maxsize>10000) {
		mutex.Lock();
		this->maxsize=maxsize;
		this->generations=generations;
		rotate_mechanism=1;
		mutex.Unlock();
		return 1;
	}
	return 0;
}

void CLog::SetLogLevel(int facility, int level)
{
	if (facility>=0 && facility<NUMFACILITIES) {
		mutex.Lock();
		debuglevel[facility]=level;
		mutex.Unlock();
		Printf(facility,0,"ppl6::CLog","SetLogLevel",__FILE__,__LINE__,"=== Setting Debuglevel for %s, to: %u",facilitylist[facility],level);
	}
}

int CLog::GetLogLevel(int facility)
{
	int ret=0;
	if (facility>=0 && facility<NUMFACILITIES) {
		mutex.Lock();
		ret=debuglevel[facility];
		mutex.Unlock();
	}
	return ret;
}


void CLog::Print (int facility, int level, const char * text)
{
	if (!shouldPrint(NULL,NULL,NULL,0,facility,level)) return;
	if (text==NULL) return;
	mutex.Lock();
	Output(facility,level,NULL,NULL,NULL,0,text,true);
	mutex.Unlock();
}

void CLog::Print (int facility, int level, const char *file, int line, const char * text)
{
	if (!shouldPrint(NULL,NULL,file,line,facility,level)) return;
	if (text==NULL) return;
	//CString s;
	//s.Sprintf("[%s:%i] %s",file, line, text);
	mutex.Lock();
	Output(facility,level,NULL,NULL,file,line,text,true);
	mutex.Unlock();
}

void CLog::Print (int facility, int level, const char *module, const char *function, const char *file, int line, const char * text)
{
	if (!shouldPrint(module,function,file,line,facility,level)) return;
	if (text==NULL) return;
	mutex.Lock();
	Output(facility,level,module,function,file,line,text,true);
	mutex.Unlock();
}

void CLog::Printfs (int facility, int level, const char *fmt, ... )
{
	if (fmt==NULL) return;
	CString s;
	va_list args;
	va_start(args, fmt);
	s.Vasprintf(fmt,args);
	va_end(args);
	Print(facility,level,s);
}

void CLog::Printf (int facility, int level, const char *file, int line, const char *fmt, ... )
{
	if (fmt==NULL) return;
	CString s;
	va_list args;
	va_start(args, fmt);
	s.Vasprintf(fmt,args);
	va_end(args);
	Print(facility,level,file,line,s);
}

void CLog::Printf (int facility, int level, const char *module, const char *function, const char *file, int line, const char *fmt, ... )
{
	//if (!shouldPrint(module,function,file,line,facility,level)) return;
	// shouldPrint führt an dieser Stelle zu Problemen, da die Funktion IsFiltered aufruft, in der neue Fehler auftreten können,
	// wodurch die Pointer in den variablen Argumenten zum Teil ungültig werden.

	CString s;
	va_list args;
	va_start(args, fmt);
	s.Vasprintf(fmt,args);
	va_end(args);
	Print(facility,level,module,function,file,line,s);
}


void CLog::Print (int level, const char * text)
{
	Print(LOG::DEBUG,level,text);
}

void CLog::Printfs (int level, const char *fmt, ... )
{
	CString s;
	va_list args;
	va_start(args, fmt);
	s.Vasprintf(fmt,args);
	va_end(args);
	Print(LOG::DEBUG,level,s);
}


void CLog::PrintArray (int facility, int level, const CAssocArray *a, const char *fmt, ... )
{
	if (!shouldPrint(NULL,NULL,NULL,0,facility,level)) return;
	CString s;
	va_list args;
	va_start(args, fmt);
	s.Vasprintf(fmt,args);
	va_end(args);
	mutex.Lock();
	Output(facility,level,NULL,NULL,NULL,0,s,true);
	OutputArray(facility,level,NULL,NULL,NULL,0,a,NULL);
	mutex.Unlock();
}

void CLog::PrintArray (int facility, int level, const char *module, const char *function, const char *file, int line, const CAssocArray *a, const char *fmt, ... )
{
	if (!shouldPrint(module,function,file,line,facility,level)) return;
	CString s;
	va_list args;
	va_start(args, fmt);
	s.Vasprintf(fmt,args);
	va_end(args);
	mutex.Lock();
	Output(facility,level,module,function,file,line,s,true);
	OutputArray(facility,level,module,function,file,line,a,NULL);
	mutex.Unlock();
}

void CLog::PrintArraySingleLine (int facility, int level, const char *module, const char *function, const char *file, int line, const CAssocArray *a, const char *fmt, ... )
{
	if (!shouldPrint(module,function,file,line,facility,level)) return;
	CString s;
	va_list args;
	va_start(args, fmt);
	s.Vasprintf(fmt,args);
	va_end(args);
	mutex.Lock();
	OutputArray(facility,level,module,function,file,line,a,NULL,&s);
	s.Replace("\n","; ");
	s.Replace("    ","");
	Output(facility,level,module,function,file,line,s,true);
	mutex.Unlock();
}

void CLog::OutputArray(int facility, int level, const char *module, const char *function, const char *file, int line, const CAssocArray *a, const char *prefix, CString *Out)
{
	CString key, pre, out;
	if (prefix) key.Sprintf("%s/",prefix);

	if (!a) return;
	if (!Out) Out=&out;
	CTreeWalker walk;
	a->Reset(walk);
	ARRAY_RESULT row;
	CString *string;
	CAssocArray *array;
	CBinary *binary;
	const char *k;
	void *pointer;
	while ((row=a->GetNext(walk))) {
		k=a->GetKey(row);
		if ((string=a->GetString(row))) {
			Out->Concatf("%s%s=%s\n",(const char*)key,k,string->GetPtr());
			//Output(facility,level,(char*)out,false);
		} else if ((pointer=a->GetPointer(row))) {
			Out->Concatf("%s%s=%llu\n",(const char*)key,k,((ppluint64)(ppliptr)pointer));
			//Output(facility,level,(char*)out,false);
		} else if ((array=a->GetArray(row))) {
			pre.Sprintf("%s%s",(const char*)key,k);
			OutputArray(facility,level,module,function,file,line,array,(const char*)pre,Out);
		} else if ((binary=a->GetBinary(row))) {
			Out->Concatf("%s%s=CBinary, %llu Bytes\n",(const char*)key,k,(ppluint64)binary->Size());
			//Output(facility,level,(char*)out,false);
		}
	}
	if (Out==&out) Output(facility,level,module,function,file,line,(const char*)out,false);
}


void CLog::HexDump (const void * address, int bytes)
{
	HexDump(LOG::DEBUG,1,address,bytes);
}

void CLog::HexDump (int facility, int level, const void * address, int bytes)
{
	if (!shouldPrint(NULL,NULL,NULL,0,facility,level)) return;
	if (address==NULL) return;
	mutex.Lock();

	CString line;
	CString cleartext;

	char zeichen[2];
	zeichen[1]=0;
	//char buff[1024], tmp[10], cleartext[20];
	line.Sprintf("HEXDUMP: %u Bytes starting at Address 0x%08llX (%llu):",
			bytes,(ppluint64)(ppliptr)address,(ppluint64)(ppliptr)address);
	Output(facility,level,NULL,NULL,NULL,0,(const char*)line,true);

	char *_adresse=(char*)address;
	ppldd spalte=0;
	line.Sprintf("0x%08llX: ",(ppluint64)(ppliptr)_adresse);
	cleartext.Clear();
	for (int i=0;i<bytes;i++) {
		if (spalte>15) {
			line.Concat("                                                               ");
			line.TrimR(60);
			line.Concat(": ");
			line.Concat(cleartext);
			Output(facility,level,NULL,NULL,NULL,0,(const char*)line,false);
			line.Sprintf("0x%08llX: ",(ppluint64)(ppliptr)(_adresse+i));
			cleartext.Clear();
			spalte=0;
		}
		line.Concatf("%02X ",(ppldb)_adresse[i]);
		zeichen[0]=(ppldb)_adresse[i];
		if ((ppldb)_adresse[i]>31)	cleartext.Concat(zeichen);
		else cleartext.Concat(".");
		spalte++;
	}

	if (spalte>0) {
		line.Concat("                                                               ");
		line.TrimR(60);
		line.Concat(": ");
		line.Concat(cleartext);
		Output(facility,level,NULL,NULL,NULL,0,(const char*)line,false);
		Output(facility,level,NULL,NULL,NULL,0,"",false);
	}
	mutex.Unlock();
}

void CLog::LogErrorCode (int level, ppluint32 errorcode)
{
	Printfs(LOG::ERR,level,"ERROR %u: %s",errorcode,GetError(errorcode));
}

void CLog::LogErrorCode (ppluint32 errorcode)
{
	Printfs(LOG::ERR,1,"ERROR %u: %s",errorcode,GetError(errorcode));
}

void CLog::LogError (int level)
{
	Printfs(LOG::ERR,level,"ERROR %u: %s (%s)",GetErrorCode(),GetError(),GetExtendedError());
}

void CLog::LogError ()
{
	Printfs(LOG::ERR,1,"ERROR %u: %s (%s)",GetErrorCode(),GetError(),GetExtendedError());
}

void CLog::LogError (const char *file, int line)
{
	Printf(LOG::ERR,1,file,line,"ERROR %u: %s (%s)",GetErrorCode(),GetError(),GetExtendedError());
}

void CLog::LogError (const char *module, const char *function, const char *file, int line)
{
	Printf(LOG::ERR,1,module,function,file,line,"ERROR %u: %s (%s)",GetErrorCode(),GetError(),GetExtendedError());
}

void CLog::LogError (int facility, int level, const char *file, int line)
{
	Printf(facility,level,file,line,"ERROR %u: %s (%s)",GetErrorCode(),GetError(),GetExtendedError());
}

void CLog::LogError (int facility, int level, const char *module, const char *function, const char *file, int line)
{
	Printf(facility,level,module,function,file,line,"ERROR %u: %s (%s)",GetErrorCode(),GetError(),GetExtendedError());
}

#ifdef HAVE_SYSLOG_H
static int getSyslogLevel(int facility)
{
	switch (facility) {
		case LOG::EMERG: return LOG_EMERG;
		case LOG::ALERT: return LOG_ALERT;
		case LOG::CRIT: return LOG_CRIT;
		case LOG::ERR: return LOG_ERR;
		case LOG::WARNING: return LOG_WARNING;
		case LOG::NOTICE: return LOG_NOTICE;
		case LOG::INFO: return LOG_INFO;
		case LOG::DEBUG: return LOG_DEBUG;
		default:
			return LOG_DEBUG;
	}
}

static const char *getSyslogLdevelName(int facility)
{
	switch (facility) {
		case LOG::EMERG: return "EMERGENCY";
		case LOG::ALERT: return "ALERT";
		case LOG::CRIT: return "CRITICAL";
		case LOG::ERR: return "ERROR";
		case LOG::WARNING: return "WARNING";
		case LOG::NOTICE: return "NOTICE";
		case LOG::INFO: return "INFO";
		case LOG::DEBUG: return "DEBUG";
		default:
			return "DEBUG";
	}
}
#endif

void CLog::Output(int facility, int level, const char *module, const char *function, const char *file, int line, const char *buffer, bool printdate)
{
	PushError();
	CString bf;
	char d[20], z[20];
	if (printdate) {
		datum (d);
		zeit(z);
		double s=ppl6::GetMicrotime();
		s=(s-(ppluint64)s)*1000;
		if (logThreadId) bf.Sprintf("%s-%s.%03i [%7s %2i] [%6llu] ",d,z,(int)s,facilitylist[facility],level,GetThreadID());
		else bf.Sprintf("%s-%s [%7s %2i] ",d,z,facilitylist[facility],level);
		bf.Concat("[");
		if (file) bf.Concatf("%s:%i",file,line);
		bf.Concat("] {");
		if (module) {
			bf.Concatf("%s",module);
			if (function) bf.Concatf(": %s",function);
		}
		bf.Concat("}: ");
	} else {
		bf.Sprintf("     ");
	}
	CString bu=buffer;
	bu.Trim();
	bf+=bu;
	bf.Replace("\n","\n     ");
	bf.Concat("\n");

#ifdef HAVE_OPENLOG
	if (useSyslog) {
		CString log;
		if (logThreadId) log.Sprintf("[%llu]",GetThreadID());
		/*
		log.Concat("[");
		if (file) log.Concatf("%s:%i",file,line);
		log.Concat("] {");
		if (module) {
			log.Concatf("%s",module);
			if (function) log.Concatf(": %s",function);
		}
		log.Concat("}: ");
		*/
		syslog(getSyslogLevel(facility),"[%s] %s %s",getSyslogLdevelName(facility), (const char*)log, (const char*)bu);
	}
#endif

	if (level<=debuglevel[facility]) {
		logff[facility].Puts(bf);
		logff[facility].Flush();
		CheckRotate(facility);
	}
	if (facility!=LOG::DEBUG && level<=debuglevel[LOG::DEBUG] && (
		strcmp(logff[facility].GetFilename(),logff[LOG::DEBUG].GetFilename())!=0)) {
		logff[LOG::DEBUG].Puts(bf);
		logff[LOG::DEBUG].Flush();
		CheckRotate(LOG::DEBUG);
	}
	LOGHANDLER *h=(LOGHANDLER *)firsthandler;
	while (h) {
		h->handler->LogMessage(facility,level,(const char*)bf);
		h=h->next;
	}
	PopError();
}

int CLog::AddLogHandler(CLogHandler *handler)
{
	if (!handler) {
		SetError(194);
		return 0;
	}
	LOGHANDLER *h=(LOGHANDLER *)malloc(sizeof(LOGHANDLER));
	if (!h) {
		SetError(2);
		return 0;
	}
	h->handler=handler;
	h->previous=NULL;
	h->next=NULL;
	mutex.Lock();
	if (!lasthandler) {
		firsthandler=lasthandler=h;
		mutex.Unlock();
		return 1;
	}
	LOGHANDLER *last=(LOGHANDLER *)lasthandler;
	last->next=h;
	h->previous=last;
	lasthandler=h;
	mutex.Unlock();
	return 1;
}

int CLog::DeleteLogHandler(CLogHandler *handler)
{
	if (!handler) {
		SetError(194);
		return 0;
	}
	mutex.Lock();
	LOGHANDLER *h=(LOGHANDLER *)firsthandler;
	if (!h) {
		mutex.Unlock();
		return 1;
	}
	while (h) {
		if (h->handler==handler) {
			if (h->previous) h->previous->next=h->next;
			if (h->next) h->next->previous=h->previous;
			if (h==firsthandler) firsthandler=h->next;
			if (h==lasthandler) lasthandler=h->previous;
			free(h);
			mutex.Unlock();
			return 1;
		}
		h=h->next;
	}
	mutex.Unlock();
	return 1;
}

int CLog::SetFilter(const char *module, const char *function, int level)
{
	if (!module) {
		SetError(194,"int CLog::SetFilter(==> const char *module <==, const char *file, int level)");
		return 0;
	}
	if (!FilterModule) FilterModule=new CAssocArray;
	if (!FilterModule) {
		SetError(2);
		return 0;
	}
	CString Name=module;
	if (function) Name.Concatf("::%s",function);
	FilterModule->Setf(Name,"%i",level);
	return 1;
}

int CLog::SetFilter(const char *file, int line, int level)
{
	if (!file) {
		SetError(194,"int CLog::SetFilter(==> const char *file <==, int line, int level)");
		return 0;
	}
	if (!FilterFile) FilterFile=new CAssocArray;
	if (!FilterFile) {
		SetError(2);
		return 0;
	}
	CString Name=file;
	Name.Concatf(":%i",line);
	FilterFile->Setf(Name,"%i",level);
	return 1;
}



void CLog::DeleteFilter(const char *module, const char *function)
{
	if (!FilterModule) return;
	if (!module) return;
	CString Name=module;
	if (function) Name.Concatf("::%s",function);
	FilterModule->Delete(Name);
}

void CLog::DeleteFilter(const char *file, int line)
{
	if (!FilterFile) return;
	if (!file) return;
	CString Name=file;
	Name.Concatf(":%i",line);
	FilterFile->Delete(Name);
}


bool CLog::shouldPrint(const char *module, const char *function, const char *file, int line, int facility, int level)
{
	if (facility<1 || facility>=NUMFACILITIES) return false;
	if (debuglevel[facility]<level) return false;				// Wenn der Debuglevel kleiner ist, brauchen wir nicht weiter machen
	bool ret=true;

	ppl6::PushError();
	if (IsFiltered(module,function,file,line,level)) ret=false;
	ppl6::PopError();
	return ret;
}

int CLog::IsFiltered(const char *module, const char *function, const char *file, int line, int level)
{
	CString Name;
	const char *tmp;
	int l;
	if (FilterModule) {
		if (module) {
			Name=module;
			tmp=FilterModule->Get(Name);
			if (tmp) {
				l=ppl6::atoi(tmp);
				if (level>=l) return 1;
			}
			if (function) {
				Name.Concatf("::%s",function);
				tmp=FilterModule->Get(Name);
				if (tmp) {
					l=ppl6::atoi(tmp);
					if (level>=l) return 1;
				}
			}
		}
	}
	if (FilterFile) {
		if (file) {
			Name.Setf("%s:0",file);
			tmp=FilterFile->Get(Name);
			if (tmp) {
				l=ppl6::atoi(tmp);
				if (level>=l) return 1;
			}
			Name.Setf("%s:%i",file,line);
			tmp=FilterFile->Get(Name);
			if (tmp) {
				l=ppl6::atoi(tmp);
				if (level>=l) return 1;
			}
		}
	}
	return 0;
}


void CLog::CheckRotate(int facility)
{
	CString f1,f2;
	if (inrotate) return;
	if (rotate_mechanism==1) {
		pplint64 size=logff[facility].Size();
		if (size>0 && (ppluint64)size>maxsize) {
			inrotate=true;
			Output(facility,0,"ppl6::CLog","CheckRotate",__FILE__,__LINE__,"Logfile Rotated");
			logff[facility].Close();
			// Wir müssen die bisherigen Generationen umbenennen
			for (int i=generations;i>1;i--) {
				f1.Setf("%s.%i",logfilename[facility].GetPtr(),i-1);
				f2.Setf("%s.%i",logfilename[facility].GetPtr(),i);
				CFile::RenameFile(f1,f2);
			}
			f2.Setf("%s.1",logfilename[facility].GetPtr());
			CFile::RenameFile(logfilename[facility],f2);
			logff[facility].Open(logfilename[facility],"ab");
			f1.Setf("=== Logfile started for %s",facilitylist[facility]);
			Output(facility,0,"ppl6::CLog","SetLogfile",__FILE__,__LINE__,f1);
			inrotate=false;
		}
	}
}


} // EOF namespace ppl6

