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
 * Copyright (c) 2011, Patrick Fedick <patrick@pfp.de>
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
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ppl6.h"

namespace ppl6 {

Exception::Exception() throw()
{
	ErrorText=NULL;
}

Exception::~Exception() throw()
{
	if (ErrorText) free(ErrorText);
}

const char* Exception::what() const throw()
{
	return "PPLException";
}

Exception::Exception(const Exception &other) throw()
{
	if (other.ErrorText) {
		ErrorText=strdup(other.ErrorText);
	} else {
		ErrorText=NULL;
	}
}

Exception& Exception::operator= (const Exception &other) throw()
{
	if (other.ErrorText) {
		ErrorText=strdup(other.ErrorText);
	} else {
		ErrorText=NULL;
	}
	return *this;
}

Exception::Exception(const char *msg, ...)
{
	if (msg) {
		CString Msg;
		va_list args;
		va_start(args, msg);
		Msg.Vasprintf(msg,args);
		va_end(args);
		ErrorText=strdup((const char*)Msg);
	} else {
		ErrorText=NULL;
	}
}

Exception::Exception(const CString &msg)
{
	ErrorText=strdup((const char*)msg);
}

void Exception::copyText(const char *str) throw()
{
	free(ErrorText);
	ErrorText=strdup(str);
}

void Exception::copyText(const char *fmt, va_list args) throw()
{
	free(ErrorText);
	CString Msg;
	Msg.Vasprintf(fmt,args);
	ErrorText=strdup((const char*)Msg);
}

const char* Exception::text() const throw()
{
	if (ErrorText) return ErrorText;
	else return "";
}

CString Exception::toString() const throw()
{
	CString str;
	str.Sprintf("%s",what());
	if (ErrorText) str.Concatf(" [%s]",(const char*)ErrorText);
	return str;
}

void Exception::print() const
{
	PrintDebug("Exception: %s",what());
	if (ErrorText) PrintDebug(" [%s]",(const char*)ErrorText);
	PrintDebug ("\n");
}


}	// EOF namespace ppl6
