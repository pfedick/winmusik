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
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif


#include "ppl7.h"



namespace ppl7 {



String PythonHelper::escapeString(const String &s)
{
	String ret=s;
	ret.replace("\\","\\\\");
	ret.replace("\"","\\\"");
	ret.replace("\n","\\n");
	ret.replace("\r","\\r");
	ret.replace("\t","\\t");
	return ret;
}

String PythonHelper::escapeRegExp(const String &s)
{
	String ret=s;
	ret.pregEscape();
	return ret;
}

static ppl7::String getValue(const ppl7::String str)
{
	ppl7::String lstr=str.toLowerCase();
	if (str.isNumeric() && (str.instr(",")<0)) return str;
	else if(lstr=="true") return "True";
	else if(lstr=="false") return "False";
	else if(lstr=="null" || lstr=="none") return "None";
	else return ppl7::ToString("\"%s\"",(const char*)PythonHelper::escapeString(str));
}


static ppl7::String toHashRecurse(const AssocArray &a, int indention)
{
	String r;
	String key;
	AssocArray::Iterator it;
	a.reset(it);
	String indent;
	indent.repeat(' ',indention);
	while (a.getNext(it)) {
		const String &key=it.key();
		const Variant &res=it.value();
		if (res.isAssocArray()) {
			r.appendf("%s\"%s\": {\n",(const char*)indent,(const char*)key);
			r+=toHashRecurse(res.toAssocArray(),indention+4);
			r.appendf("%s}\n",(const char*)indent);
		} else {
			r.appendf("%s\"%s\": ",(const char*)indent,(const char*)key);
			r+=getValue(res.toString());
			r+=",\n";
		}
	}
	r.trimRight(",\n");
	r+="\n";
	return r;
}

String PythonHelper::toHash(const AssocArray &a, const String &name, int indention)
{
	String ret;
	String indent;
	indent.repeat(' ',indention);

	if (name.isEmpty()) return ret;
	ret.setf("%s%s = {",(const char*)indent,(const char*)name);
	if (a.count()) {
		ret+="\n";
		ret+=toHashRecurse(a,indention+4);
		ret+=indent;
	}
	ret+="}\n";
	return ret;
}

}	// EOF namespace ppl6
