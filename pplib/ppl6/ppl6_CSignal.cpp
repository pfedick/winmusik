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
#ifdef HAVE_STDARG_H
	#include <stdarg.h>
#endif
#ifdef HAVE_STRINGS_h
	#include <strings.h>
#endif
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef HAVE_SIGNAL_H
	#include <signal.h>
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


namespace ppl6 {

static CSignal *signalhandler=NULL;

static void pplsignal(int sig)
{
	if (signalhandler) {
		signalhandler->Signal(sig);
	}
}


/*!\class CSignal
 * \ingroup PPLGroupThreads
 */
CSignal::CSignal()
{
	if (!signalhandler) signalhandler=this;
}

CSignal::~CSignal()
{
	if (signalhandler==this) signalhandler=NULL;
}

int CSignal::CatchSignal(int sig)
{
	if (SIG_ERR!=signal(sig, pplsignal)) return 1;
	return 0;
}

int CSignal::IgnoreSignal(int sig)
{
	if (SIG_ERR!=signal(sig, SIG_IGN)) return 1;
	return 0;
}

int CSignal::ClearSignal(int sig)
{
	if (SIG_ERR!=signal(sig, SIG_DFL)) return 1;
	return 0;
}

void CSignal::Signal(int sig)
{
}


}	// EOF namespace ppl6

