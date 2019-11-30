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
// ERRORSFIXED=1
#include "ppl6.h"


namespace ppl6 {

CDebug::CDebug()
{
}

CDebug::~CDebug()
{
	Print("=== DEBUG-Class terminated ===============================");
}


void CDebug::SetConsole(bool flag)
{
	CLog::SetConsole(flag,LOG::DEBUG);
}

void CDebug::SetLogfile(const char *filename)
{
	CLog::Printf(LOG::DEBUG,1,__FILE__,__LINE__,"SetLogfile (%s) =====", filename?filename:"NULL");
	CLog::SetLogfile(LOG::DEBUG,filename);
	CLog::Print(LOG::DEBUG,1,(char*)"=== DEBUG started ========================================");
}

void CDebug::SetDebugLevel(int level)
{
	CLog::SetLogLevel(LOG::DEBUG,level);
	CLog::Printf(LOG::DEBUG,1,__FILE__,__LINE__,"=== Setting Debuglevel to: %u",level);
}

int CDebug::GetDebugLevel()
{
	return CLog::GetLogLevel(LOG::DEBUG);
}

void CDebug::Log (const char * text)
{
	CLog::Print(LOG::DEBUG,1,text);
}

void CDebug::Log (int level, const char * text)
{
	CLog::Print(LOG::DEBUG,level,text);
}

void CDebug::Logf (const char *fmt, ... )
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	vasprintf (&buff, (char*)fmt, args);
	va_end(args);
	if (buff) {
		CLog::Print(LOG::DEBUG,1,buff);
		free(buff);
	}
}

void CDebug::Logf (int level, const char *fmt, ... )
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	vasprintf (&buff, (char*)fmt, args);
	va_end(args);
	if (buff) {
		CLog::Print(LOG::DEBUG,level,buff);
		free(buff);
	}
}

void CDebug::Print (const char * text)
{
	CLog::Print(LOG::DEBUG,1,text);
}

void CDebug::Print (int level, const char * text)
{
	CLog::Print(LOG::DEBUG,level,text);
}

void CDebug::Printf (const char *fmt, ... )
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	vasprintf (&buff, (char*)fmt, args);
	va_end(args);
	if (buff) {
		CLog::Print(LOG::DEBUG,1,buff);
		free(buff);
	}
}

void CDebug::Printf (int level, const char *fmt, ... )
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	vasprintf (&buff, (char*)fmt, args);
	va_end(args);
	if (buff) {
		CLog::Print(LOG::DEBUG,level,buff);
		free(buff);
	}
}

} // EOF namespace ppl6

