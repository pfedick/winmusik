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
#ifndef _WIN32
#include <strings.h>
#endif
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif


#include "ppl6.h"

namespace ppl6 {
CTok::CTok()
{
	token=new CArray();
	ptr=0;
}

CTok::CTok(const CString &string, const char *suchstring)
{
	token=new CArray();
	ptr=0;
	Split(string,suchstring);
}


CTok::CTok(const char *string, const char *suchstring)
{
	token=new CArray();
	ptr=0;
	Split(string,suchstring);
}



CTok::~CTok()
{
	delete token;
}

int CTok::Split(const char *string, const char *suchstring)
{
	if (!token) {
		SetError(2);
		return 0;
	}
	token->Clear();
	size_t l;
	if ((!string) ||(!suchstring)) {
		SetError(194);
		return 0;
	}
	if ((l=strlen(string))==0) return 0;
	size_t dwsize=strlen(suchstring);
	// String zerlegen
	int tokfind;
	while (1) {
		while ((tokfind=instr(string,suchstring,0))==0) {
			string+=dwsize;
		}
		if(tokfind>-1) {
			token->Add((char*)string,tokfind);
			string+=tokfind+dwsize;
		} else {
			if (strlen(string)>0) {
				token->Add((char*)string);
			}
			break;
		}
	}
	ptr=0;
	return 1;
}

int CTok::Split(const CString &string, const char *suchstring)
{
	return Split((const char*)string.GetPtr(), suchstring);
}


const char *CTok::GetFirst(const char *string, const char *suchstring)
{
	if (!Split(string,suchstring)) return NULL;
	return GetNext();
}

const char *CTok::GetFirst(const CString &string, const char *suchstring)
{
	if (!Split(string,suchstring)) return NULL;
	return GetNext();
}



const char *CTok::GetFirst()
{
	ptr=0;
	return GetNext();
}

void CTok::Reset()
{
	ptr=0;
}

const char * CTok::GetNext()
{
	const char *ret=token->Get(ptr);
	if (ret) ptr++;
	return ret;
}

const char *CTok::GetLast(const char *string, const char *suchstring)
{
	if (!Split(string,suchstring)) return NULL;
	return GetLast();
}

const char *CTok::GetLast(const CString &string, const char *suchstring)
{
	if (!Split(string,suchstring)) return NULL;
	return GetLast();
}

const char *CTok::GetLast()
{
	ptr=token->Num()-1;
	return GetPrevious();
}

const char *CTok::GetPrevious()
{
	if (ptr<0) return NULL;
	const char *ret=token->Get(ptr);
	if (ptr>=0) ptr--;
	return ret;
}


const char *CTok::Get(int index)
{
	if (index<0) return NULL;
	return token->Get(index);
}

const char *CTok::operator[](int index)
{
	if (index<0) return NULL;
	return token->Get(index);
}

void CTok::Clear()
{
	ptr=0;
	token->Clear();
}

CString CTok::GetRest(const char *trenn)
{
	CString str;
	GetRest(str,ptr,trenn);
	return str;
}

int CTok::GetRest(CString &str, const char *trenn)
{
	return GetRest(str,ptr,trenn);
}

CString CTok::GetRest(int index, const char *trenn)
{
	CString str;
	GetRest(str,index,trenn);
	return str;
}

int CTok::GetRest(CString &str, int index, const char *trenn)
{
	const char *tok;
	int count=0;
	while ((tok=token->Get(index+count))) {
		if (count) str.Concat(trenn);
		str.Concat(tok);
		count++;
	}
	if (!count) return 0;
	return 1;
}

void CTok::List() const
{
	if (token) token->List();
}

int CTok::Num() const
{
	if (token) return token->Num();
	return 0;
}


} // end of namespace ppl

