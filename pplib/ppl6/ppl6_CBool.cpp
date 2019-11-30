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

/*!\class CBool
 * \ingroup PPLGroupDataTypes
 *
 */

CBool::CBool()
{
	type=CVar::CBOOL;
	value=false;
}

CBool::CBool(int val)
{
	type=CVar::CBOOL;
	if (val) value=true;
	else value=false;
}

CBool::CBool(bool val)
{
	type=CVar::CBOOL;
	value=val;
}

CBool::CBool(const char *str)
{
	type=CVar::CBOOL;
	value=false;
	if ((str) && (atoi(str))) value=true;
	if (strcasecmp((char*)str,(char*)"true")==0) value=true;
	if (strcasecmp((char*)str,(char*)"wahr")==0) value=true;
}

CBool::CBool(CString *str)
{
	type=CVar::CBOOL;
	value=false;
	if (str) value=str->ToBool();
	if (strcasecmp((char*)str,(char*)"true")==0) value=true;
	if (strcasecmp((char*)str,(char*)"wahr")==0) value=true;

}

CBool::CBool(const CString &str)
{
	type=CVar::CBOOL;
	if((int)str)value=true;
	else value=false;
}

CBool::CBool(CInt *val)
{
	type=CVar::CBOOL;
	value=false;
	if (val->GetVal()) value=true;

}

CBool::CBool(const CInt &val)
{
	type=CVar::CBOOL;
	if((int)val)value=true;
	else value=false;
}

CBool::~CBool()
{
	value=false;
}


CBool::operator int() const
{
	if (value) return 1;
	return 0;
}

CBool::operator bool() const
{
	return value;
}

CBool::operator unsigned int() const
{
	if (value) return 1;
	return 0;
}

bool CBool::operator==(bool val) const
{
	if (val==value) return true;
	return false;
}

bool CBool::operator!=(bool val) const
{
	if (value!=val) return true;
	return false;
}

bool CBool::IsTrue()
{
	if (value) return true;
	return false;
}

bool CBool::IsFalse()
{
	if (value) return false;
	return true;
}

CBool& CBool::operator=(const char* str)
{
	value=false;
	if (str) {
		if(atoi(str)) value=true;
	}
	return *this;
}

CBool& CBool::operator=(const CString &str)
{
	value=(bool)(str);
	return *this;
}

CBool& CBool::operator=(const int val)
{
	if (val) value=true; else value=false;
	return *this;
}

CBool& CBool::operator=(const bool val)
{
	value=val;
	return *this;
}

CBool& CBool::operator=(const CInt &val)
{
	if ((int)val) value=true;
	else value=false;
	return *this;
}

CBool& CBool::operator=(const CBool &val)
{
	value=val.value;
	return *this;
}

} // EOF namespace ppl6
