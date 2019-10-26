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
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
//#include <windows.h>
#endif


#include "ppl6.h"

namespace ppl6 {

/*!\class CNotifyHandler
 * \ingroup PPLGroupThreads
 */


CNotifyHandler::CNotifyHandler()
{
	NotifyHandler=NULL;
	NotifyObject=NULL;
}

CNotifyHandler::~CNotifyHandler()
{
	Unset();
}


void CNotifyHandler::SetHandler(void(*NotifyHandler)(void *object, void *data),void *object)
{
	this->NotifyHandler=NotifyHandler;
	NotifyObject=object;
}

void CNotifyHandler::Notify(void *data)
{
	if (NotifyHandler) {
		try {
			NotifyHandler(NotifyObject,data);
		}
		catch(...) {  // Handle all exceptions
			NotifyHandler=NULL;
		}
	}
}

void CNotifyHandler::Unset()
{
	NotifyHandler=NULL;
	NotifyObject=NULL;
}

/*!\class CCallback
 * \ingroup PPLGroupThreads
 */


CCallback::CCallback()
{
	callbackdata=NULL;
}

CCallback::~CCallback()
{
}

void CCallback::SetData(void *data)
{
	callbackdata=data;
}

void CCallback::Callback(void *data)
{
	return;
}



};	// EOF Namespace ppl
