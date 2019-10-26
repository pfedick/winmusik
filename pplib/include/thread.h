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
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
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
namespace ppl6 {
typedef struct tagErrorStack {
	struct tagErrorStack	*next;
	CString				ErrorText;
	pplint32				err, suberr;
} ERRORSTACK;
              
typedef struct tagThreadData {
	ppluint64 threadid;
	CThread *ThreadClass;
#ifdef _WIN32
	HANDLE	thread;
	DWORD	dwThreadID;
#elif defined HAVE_PTHREADS
	pthread_t thread;
	pthread_attr_t	attr;
#else
	int	thread;
#endif
	PPL_ERROR	Error;
	ERRORSTACK	*ErrorStack;
	void		(*mysql_thread_end)(struct tagThreadData *thread);
				// Bit  0: Thread hat MySQL benutzt

} THREADDATA;


THREADDATA * GetThreadData();

int MySQLThreadStart();

}
