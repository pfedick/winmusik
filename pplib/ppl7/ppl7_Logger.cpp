/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
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


#include "prolog_ppl7.h"
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

#include "ppl7.h"


namespace ppl7 {

typedef struct tagLOGHANDLER
{
	LogHandler *handler;
	struct tagLOGHANDLER *next;
	struct tagLOGHANDLER *previous;
} LOGHANDLER;


static const char *prioritylist[] = {
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

#endif



Logger::Logger()
{
	firsthandler=lasthandler=NULL;
	logconsole=false;
	logThreadId=true;
	console_enabled=false;
	console_priority=Logger::DEBUG;
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

Logger::~Logger()
{
	terminate();
	if (FilterModule) delete FilterModule;
	if (FilterFile) delete FilterFile;
}

void Logger::terminate()
{
	print(Logger::INFO,0,"ppl7::Logger","terminate",__FILE__,__LINE__,"=== Logfile-Class terminated ===============================");
	for (int i=0;i<NUMFACILITIES;i++) {
		logff[i].close();
	}
	mutex.lock();
	LOGHANDLER *h=(LOGHANDLER *)firsthandler;
	while (h) {
		LOGHANDLER *c=h;
		h=h->next;
		free(c);
	}
	firsthandler=lasthandler=NULL;
	logconsole=false;
	logThreadId=true;
	console_enabled=false;
	console_priority=Logger::DEBUG;
	console_level=1;
	for (int i=0;i<NUMFACILITIES;i++) {
		debuglevel[i]=0;
	}
	mutex.unlock();
#ifdef HAVE_OPENLOG
	closeSyslog();
#endif
}



void Logger::enableConsole(bool flag, PRIORITY prio, int level)
{
	mutex.lock();
	console_enabled=flag;
	console_priority=prio;
	console_level=level;
	mutex.unlock();
}

void Logger::openSyslog(const String &ident,SYSLOG_FACILITY facility)
{
#ifndef HAVE_OPENLOG
	throw UnsupportedFeatureException("syslog");
#else
	if (useSyslog) {
		print(Logger::INFO,0,"ppl7::Logger","openSyslog",__FILE__,__LINE__,"=== Reopen Syslog ===============================");
		closelog();
	}
	useSyslog=true;
	syslogIdent=ident;
	syslogFacility=facility;
	openlog(syslogIdent,LOG_NDELAY|LOG_PID,syslog_facility_lookup[facility]);
	print(Logger::INFO,0,"ppl7::Logger","openSyslog",__FILE__,__LINE__,"=== Enable Syslog ===============================");
#endif
}

void Logger::closeSyslog()
{
#ifndef HAVE_CLOSELOG
	throw UnsupportedFeatureException("syslog");
#else
	if (useSyslog) {
		print(Logger::INFO,0,"ppl7::Logger","closeSyslog",__FILE__,__LINE__,"=== Close Syslog ===============================");
		closelog();
		useSyslog=false;
	}
#endif
}


void Logger::setLogfile(PRIORITY prio, const String &filename)
{
	if (prio<1 || prio>=NUMFACILITIES) return;
	mutex.lock();
	try {
		logff[prio].close();
		if (filename.notEmpty()) {
			logfilename[prio]=filename;
			logff[prio].open(filename,File::APPEND);
			print(prio,0,"ppl7::Logger","SetLogfile",__FILE__,__LINE__,ToString("=== Logfile started for %s",prioritylist[prio]));
		}
	} catch (...) {
		mutex.unlock();
		throw;
	}
}

void Logger::setLogfile(PRIORITY prio, const String &filename, int level)
{
	if (prio<1 || prio>=NUMFACILITIES) return;
	mutex.lock();
	try {
		logff[prio].close();
		debuglevel[prio]=level;
		if (filename.notEmpty()) {
			logfilename[prio]=filename;
			logff[prio].open(filename,File::APPEND);
			print(prio,0,"ppl7::Logger","SetLogfile",__FILE__,__LINE__,ToString("=== Logfile started for %s",prioritylist[prio]));
		}
	} catch (...) {
		mutex.unlock();
		throw;
	}
}

void Logger::setLogRotate(uint64_t maxsize, int generations)
{
	if (generations>1 && maxsize>1024*1024) {
		mutex.lock();
		this->maxsize=maxsize;
		this->generations=generations;
		rotate_mechanism=1;
		mutex.unlock();
	}
}

void Logger::setLogLevel(PRIORITY prio, int level)
{
	if (prio>=0 && prio<NUMFACILITIES) {
		mutex.lock();
		debuglevel[prio]=level;
		mutex.unlock();
		print(prio,0,"ppl7::Logger","SetLogLevel",__FILE__,__LINE__,ToString("=== Setting Debuglevel for %s, to: %u",prioritylist[prio],level));
	}
}

int Logger::getLogLevel(PRIORITY prio)
{
	int ret=0;
	if (prio>=0 && prio<NUMFACILITIES) {
		mutex.lock();
		ret=debuglevel[prio];
		mutex.unlock();
	}
	return ret;
}

String Logger::getLogfile(PRIORITY prio)
{
	String ret;
	if (prio>=0 && prio<NUMFACILITIES) {
		mutex.lock();
		ret=logfilename[prio];
		mutex.unlock();
	}
	return ret;
}


void Logger::print (const String &text)
{
	if (!shouldPrint(NULL,NULL,NULL,0,Logger::DEBUG,0)) return;
	mutex.lock();
	output(Logger::DEBUG,0,NULL,NULL,NULL,0,text,true);
	mutex.unlock();
}

void Logger::print (int level, const String &text)
{
	if (!shouldPrint(NULL,NULL,NULL,0,Logger::DEBUG,level)) return;
	mutex.lock();
	output(Logger::DEBUG,level,NULL,NULL,NULL,0,text,true);
	mutex.unlock();
}

void Logger::print (PRIORITY prio, int level, const String &text)
{
	if (!shouldPrint(NULL,NULL,NULL,0,prio,level)) return;
	mutex.lock();
	output(prio,level,NULL,NULL,NULL,0,text,true);
	mutex.unlock();
}

void Logger::print (PRIORITY prio, int level, const char *file, int line, const String &text)
{
	if (!shouldPrint(NULL,NULL,file,line,prio,level)) return;
	mutex.lock();
	output(prio,level,NULL,NULL,file,line,text,true);
	mutex.unlock();
}

void Logger::print (PRIORITY prio, int level, const char *module, const char *function, const char *file, int line, const String &text)
{
	if (!shouldPrint(module,function,file,line,prio,level)) return;
	mutex.lock();
	output(prio,level,module,function,file,line,text,true);
	mutex.unlock();
}


void Logger::printArray (PRIORITY prio, int level, const AssocArray &a, const String &text)
{
	if (!shouldPrint(NULL,NULL,NULL,0,prio,level)) return;
	mutex.lock();
	output(prio,level,NULL,NULL,NULL,0,text,true);
	outputArray(prio,level,NULL,NULL,NULL,0,a,NULL);
	mutex.unlock();
}

void Logger::printArray (PRIORITY prio, int level, const char *module, const char *function, const char *file, int line, const AssocArray &a, const String &text)
{
	if (!shouldPrint(module,function,file,line,prio,level)) return;
	mutex.lock();
	output(prio,level,module,function,file,line,text,true);
	outputArray(prio,level,module,function,file,line,a,NULL);
	mutex.unlock();
}

void Logger::printArraySingleLine (PRIORITY prio, int level, const char *module, const char *function, const char *file, int line, const AssocArray &a, const String &text)
{
	if (!shouldPrint(module,function,file,line,prio,level)) return;
	mutex.lock();
	String s;
	s=text;
	outputArray(prio,level,module,function,file,line,a,NULL,&s);
	s.replace("\n","; ");
	s.replace("    ","");
	output(prio,level,module,function,file,line,s,true);
	mutex.unlock();
}

void Logger::outputArray(PRIORITY prio, int level, const char *module, const char *function, const char *file, int line, const AssocArray &a, const char *prefix, String *Out)
{
	String key, pre, out;
	if (prefix) key.setf("%s/",prefix);
	if (!Out) Out=&out;
	AssocArray::Iterator walk;
	a.reset(walk);
	while ((a.getNext(walk))) {
		const String &k=walk.key();
		const Variant &v=walk.value();
		if (v.isString()) {
			Out->appendf("%s%s=%s\n",(const char*)key,(const char*)k,(const char*)v.toString());
		} else if (v.isDateTime()) {
			Out->appendf("%s%s=%s\n",(const char*)key,(const char*)k,(const char*)v.toDateTime().get());
		} else if (v.isPointer()) {
			Out->appendf("%s%s=%tu\n",(const char*)key,(const char*)k,((uintptr_t)v.toPointer().ptr()));
		} else if (v.isAssocArray()) {
			pre.setf("%s%s",(const char*)key,(const char*)k);
			outputArray(prio,level,module,function,file,line,v.toAssocArray(),(const char*)pre,Out);
		} else if (v.isArray()) {
			const Array &a=v.toArray();
			for (size_t i=0;i<a.size();i++) {
				Out->appendf("%s%s/%zu=%s\n",(const char*)key,(const char*)k,i,(const char*)a.get(i));
			}
		} else if (v.isByteArray()==true || v.isByteArrayPtr()==true) {
			Out->appendf("%s%s=ByteArray, %zu Bytes\n",(const char*)key,(const char*)k,v.toByteArrayPtr().size());
		}
	}
	if (Out==&out) output(prio,level,module,function,file,line,(const char*)out,false);
}

void Logger::hexDump (const void * address, int bytes)
{
	hexDump(Logger::DEBUG,1,address,bytes);
}

void Logger::hexDump (PRIORITY prio, int level, const void * address, int bytes)
{
	if (!shouldPrint(NULL,NULL,NULL,0,prio,level)) return;
	if (address==NULL) return;
	mutex.lock();

	String line;
	String cleartext;

	char zeichen[2];
	zeichen[1]=0;
	//char buff[1024], tmp[10], cleartext[20];
	line.setf("HEXDUMP: %u Bytes starting at Address 0x%08tX (%tu):",
			bytes,(uintptr_t)address,(uintptr_t)address);
	output(prio,level,NULL,NULL,NULL,0,(const char*)line,true);

	char *_adresse=(char*)address;
	uint32_t spalte=0;
	line.setf("0x%08tX: ",(uintptr_t)_adresse);
	cleartext.clear();
	for (int i=0;i<bytes;i++) {
		if (spalte>15) {
			line.append("                                                               ");
			line.chopRight(60);
			line.append(": ");
			line.append(cleartext);
			output(prio,level,NULL,NULL,NULL,0,(const char*)line,false);
			line.setf("0x%08tX: ",(uintptr_t)(_adresse+i));
			cleartext.clear();
			spalte=0;
		}
		line.appendf("%02X ",(uint8_t)_adresse[i]);
		zeichen[0]=(uint8_t)_adresse[i];
		if ((uint8_t)_adresse[i]>31)	cleartext.append(zeichen);
		else cleartext.append(".");
		spalte++;
	}

	if (spalte>0) {
		line.append("                                                               ");
		line.chopRight(60);
		line.append(": ");
		line.append(cleartext);
		output(prio,level,NULL,NULL,NULL,0,(const char*)line,false);
		output(prio,level,NULL,NULL,NULL,0,"",false);
	}
	mutex.unlock();
}

void Logger::printException(const Exception &e)
{
	print (ERR, 1, e.toString());
}

void Logger::printException(const char *file, int line, const Exception &e)
{
	print (ERR, 1, file, line, e.toString());
}

void Logger::printException(const char *file, int line, const char *module, const char *function, const Exception &e)
{
	print (ERR, 1, module, function, file, line, e.toString());
}

void Logger::output(PRIORITY prio, int level, const char *module, const char *function, const char *file, int line, const String &buffer, bool printdate)
{
	String bf;
	if (printdate) {
		String d=DateTime::currentTime().get("%Y-%m-%d %H:%M:%S.%*");
		if (logThreadId) bf.setf("%s [%7s %2i] [%6llu] ",(const char*)d,prioritylist[prio],level,ThreadID());
		else bf.setf("%s [%7s %2i] ",(const char*)d,prioritylist[prio],level);
		bf.append("[");
		if (file) bf.appendf("%s:%i",file,line);
		bf.append("] {");
		if (module) {
			bf.appendf("%s",module);
			if (function) bf.appendf(": %s",function);
		}
		bf.append("}: ");
	} else {
		bf.setf("     ");
	}
	String bu=buffer;
	bu.trim();
	bu.replace("\n","\n     ");
	bu.append("\n");
#ifdef HAVE_SYSLOG_H
	if (useSyslog) {
		String log;
		if (logThreadId) log.setf("[%2i] [%6llu]",level,ThreadID());
		else log.setf("[%2i]",level);
		syslog(syslog_priority_lookup[prio],"%s %s",(const char*)log,(const char*)bu);
	}
#endif
	bf+=bu;
	if (level<=debuglevel[prio] && logff[prio].isOpen()==true) {
		logff[prio].puts(bf);
		logff[prio].flush();
		checkRotate(prio);
	}
	if (prio!=Logger::DEBUG
			&& level<=debuglevel[Logger::DEBUG]
			                     && logff[prio].isOpen()==true
			                     && (strcmp(logff[prio].filename(),logff[Logger::DEBUG].filename())!=0)) {
		logff[Logger::DEBUG].puts(bf);
		logff[Logger::DEBUG].flush();
		checkRotate(Logger::DEBUG);
	}
	LOGHANDLER *h=(LOGHANDLER *)firsthandler;
	while (h) {
		h->handler->logMessage(prio,level,(const char*)bf);
		h=h->next;
	}
}

void Logger::addLogHandler(LogHandler *handler)
{
	if (!handler) throw IllegalArgumentException("Logger::addLogHandler(LogHandler *handler)");
	LOGHANDLER *h=(LOGHANDLER *)malloc(sizeof(LOGHANDLER));
	if (!h) throw OutOfMemoryException();
	h->handler=handler;
	h->previous=NULL;
	h->next=NULL;
	mutex.lock();
	if (!lasthandler) {
		firsthandler=lasthandler=h;
		mutex.unlock();
		return;
	}
	LOGHANDLER *last=(LOGHANDLER *)lasthandler;
	last->next=h;
	h->previous=last;
	lasthandler=h;
	mutex.unlock();
}

void Logger::deleteLogHandler(LogHandler *handler)
{
	if (!handler) throw IllegalArgumentException("Logger::deleteLogHandler(LogHandler *handler)");
	mutex.lock();
	LOGHANDLER *h=(LOGHANDLER *)firsthandler;
	if (!h) {
		mutex.unlock();
		return;
	}
	while (h) {
		if (h->handler==handler) {
			if (h->previous) h->previous->next=h->next;
			if (h->next) h->next->previous=h->previous;
			if (h==firsthandler) firsthandler=h->next;
			if (h==lasthandler) lasthandler=h->previous;
			free(h);
			mutex.unlock();
			return;
		}
		h=h->next;
	}
	mutex.unlock();
}



void Logger::setFilter(const char *module, const char *function, int level)
{
	if (!module) {
		throw IllegalArgumentException("Logger::setFilter(const char *module)");
	}
	if (!FilterModule) FilterModule=new AssocArray;
	if (!FilterModule) {
		throw OutOfMemoryException();
	}
	String Name=module;
	if (function) Name.appendf("::%s",function);
	FilterModule->setf(Name,"%i",level);
}



void Logger::setFilter(const char *file, int line, int level)
{
	if (!file) {
		throw IllegalArgumentException("Logger::setFilter(const char *file)");
	}
	if (!FilterFile) FilterFile=new AssocArray;
	if (!FilterFile) {
		throw OutOfMemoryException();
	}
	String Name=file;
	Name.appendf(":%i",line);
	FilterFile->setf(Name,"%i",level);
}



void Logger::deleteFilter(const char *module, const char *function)
{
	if (!FilterModule) return;
	if (!module) return;
	String Name=module;
	if (function) Name.appendf("::%s",function);
	FilterModule->remove(Name);
}

void Logger::deleteFilter(const char *file, int line)
{
	if (!FilterFile) return;
	if (!file) return;
	String Name=file;
	Name.appendf(":%i",line);
	FilterFile->remove(Name);
}

bool Logger::shouldPrint(const char *module, const char *function, const char *file, int line, PRIORITY prio, int level)
{
	if (prio<1 || prio>=NUMFACILITIES) return false;
	if (debuglevel[prio]<level) return false;				// Wenn der Debuglevel kleiner ist, brauchen wir nicht weiter machen
	bool ret=true;
	if (isFiltered(module,function,file,line,level)) ret=false;
	return ret;
}


int Logger::isFiltered(const char *module, const char *function, const char *file, int line, int level)
{
	String Name;
	int l;
	if (FilterModule) {
		if (module) {
			Name=module;
			const String &tmp=FilterModule->getString(Name);
			if (tmp.notEmpty()) {
				l=tmp.toInt();
				if (level>=l) return 1;
			}
			if (function) {
				Name.appendf("::%s",function);
				const String &tmp=FilterModule->getString(Name);
				if (tmp.notEmpty()) {
					l=tmp.toInt();
					if (level>=l) return 1;
				}
			}
		}
	}
	if (FilterFile) {
		if (file) {
			Name.setf("%s:0",file);
			const String &tmp=FilterFile->getString(Name);
			if (tmp.notEmpty()) {
				l=tmp.toInt();
				if (level>=l) return 1;
			}
			Name.setf("%s:%i",file,line);
			const String &tmp2=FilterFile->getString(Name);
			if (tmp2.notEmpty()) {
				l=tmp2.toInt();
				if (level>=l) return 1;
			}
		}
	}
	return 0;
}



void Logger::checkRotate(PRIORITY prio)
{
	String f1,f2;
	if (inrotate) return;
	if (rotate_mechanism==1) {
		uint64_t size=logff[prio].size();
		if (size>0 && (uint64_t)size>maxsize) {
			inrotate=true;
			output(prio,0,"ppl7::Logger","CheckRotate",__FILE__,__LINE__,"Logfile Rotated");
			logff[prio].close();
			// Wir müssen die bisherigen Generationen umbenennen
			for (int i=generations;i>1;i--) {
				f1.setf("%s.%i",logfilename[prio].getPtr(),i-1);
				f2.setf("%s.%i",logfilename[prio].getPtr(),i);
				File::rename(f1,f2);
			}
			f2.setf("%s.1",logfilename[prio].getPtr());
			File::rename(logfilename[prio],f2);
			logff[prio].open(logfilename[prio],File::APPEND);
			f1.setf("=== Logging started for %s",prioritylist[prio]);
			output(prio,0,"ppl7::Logger","SetLogfile",__FILE__,__LINE__,f1);
			inrotate=false;
		}
	}
}




} // EOF namespace ppl7

