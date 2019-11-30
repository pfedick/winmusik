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

#ifndef PPL7_PPL6COMPAT_H_
#define PPL7_PPL6COMPAT_H_

#ifndef _PPL6_INCLUDE
    #include <ppl6.h>
#endif
#ifndef _PPL7_INCLUDE
    #ifdef PPL7LIB
        #include "ppl7.h"
    #else
        #include <ppl7.h>
    #endif
#endif

namespace ppl7 {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static ppl7::String operator+(const ppl7::String &str1, const ppl6::CString& str2)
{
	ppl7::String p7(str2);
	return str1+p7;
}

static ppl7::String operator+(const ppl6::CString &str1, const ppl7::String& str2)
{
	ppl7::String p7(str1);
	return p7+str2;
}

static ppl6::CString to6(const ppl7::String &str)
{
	return ppl6::CString(str.c_str(),str.size());
}

static ppl6::CAssocArray to6(const ppl7::AssocArray &arr)
{
	ppl7::ByteArray bin;
	arr.exportBinary(bin);
	ppl6::CAssocArray a6;
	a6.ImportBinary(bin.ptr(),bin.size());
	return a6;
}

static ppl7::String to7(const ppl6::CString &str)
{
	return ppl7::String(str.GetPtr(),str.Size());
}

static ppl7::DateTime to7(const ppl6::CDateTime &date)
{
	ppl7::DateTime d;
	d.setLongInt(date.longInt());
	return d;
}

static ppl6::CDateTime to6(const ppl7::DateTime &date)
{
	ppl6::CDateTime d;
	d.setLongInt(date.longInt());
	return d;
}

static ppl7::AssocArray to7(const ppl6::CAssocArray &arr)
{
	ppl6::CBinary *bin=arr.ExportBinary();
	if (bin)  {
		try {
			ppl7::AssocArray a7;
			a7.importBinary(bin->GetPtr(),bin->Size());
			delete bin;
			return a7;
		} catch (...) {
		}
		delete bin;
	}
	return ppl7::AssocArray();
}

#pragma GCC diagnostic pop

} // EOF namespace ppl7

#endif /* PPL7_PPL6COMPAT_H_ */
