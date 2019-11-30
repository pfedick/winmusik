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
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
*/
#include "ppl6.h"


namespace ppl6 {

/*!\class CInt
 * \ingroup PPLGroupDataTypes
 *
 */


CInt::CInt()
{
	type=CVar::CINT;
	value=0;
}

CInt::CInt(const int val)
{
	type=CVar::CINT;
	value=val;
}

CInt::CInt(const char *str)
{
	type=CVar::CINT;
	value=0;
	if (str) value=atoi(str);
}

CInt::CInt(const CInt &val)
{
	type=CVar::CINT;
	value=val.value;
}

CInt::CInt(const CString &str)
{
	type=CVar::CINT;
	value=0;
	value=(int)str.ToInt();
}


CInt::~CInt()
{
	value=0;
}

int CInt::GetVal()
{
	return value;
}

CInt::operator int() const
{
	return value;
}

CInt::operator unsigned int() const
{
	return (unsigned int)value;
}

CInt::operator long() const
{
	return (long)value;
}

CInt::operator unsigned long() const
{
	return (unsigned long)value;
}

CInt::operator short() const
{
	return (short)value;
}

CInt::operator unsigned short() const
{
	return (unsigned short)value;
}

CInt::operator char() const
{
	return (char)value;
}

CInt::operator unsigned char() const
{
	return (unsigned char)value;
}

bool CInt::operator==(int val) const
{
	if (val==value) return true;
	return false;
}

bool CInt::operator<=(int val) const
{
	if (value<=val) return true;
	return false;
}

bool CInt::operator<(int val) const
{
	if (value<val) return true;
	return false;
}

bool CInt::operator>=(int val) const
{
	if (value>=val) return true;
	return false;
}

bool CInt::operator>(int val) const
{
	if (value>val) return true;
	return false;
}

bool CInt::operator!=(int val) const
{
	if (value!=val) return true;
	return false;
}

CInt& CInt::operator=(const char* str)
{
	if (str) value=atoi(str);
	return *this;
}

CInt& CInt::operator=(const CString &str)
{
	value=(int)(str);
	return *this;
}

CInt& CInt::operator=(const int val)
{
	value=val;
	return *this;
}


CInt& CInt::operator=(const CInt &val)
{
	value=val.value;
	return *this;
}

CInt& CInt::operator+=(const int val)
{
	value+=val;
	return *this;
}

CInt& CInt::operator|=(const int val)
{
	value|=val;
	return *this;
}


CInt CInt::operator+(const int val)
{
	value+=val;
	return *this;
}

CInt CInt::operator+(const CInt &val)
{
	value+=val.value;
	return *this;
}

CInt& CInt::operator++()
{
	value++;
	return *this;
}

CInt& CInt::operator--()
{
	value--;
	return *this;
}


CInt& CInt::operator+=(const CInt &val)
{
	value+=val.value;
	return *this;
}

CInt& CInt::operator-=(const int val)
{
	value-=val;
	return *this;
}

CInt& CInt::operator-=(const CInt &val)
{
	value-=val.value;
	return *this;
}

} // EOF namespace ppl6
