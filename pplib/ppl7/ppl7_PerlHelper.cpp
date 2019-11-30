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



String PerlHelper::escapeString(const String &s)
{
	String ret=s;
	ret.replace("\\","\\\\");
	ret.replace("\"","\\\"");
	ret.replace("@","\\@");
	return ret;
}

String PerlHelper::escapeRegExp(const String &s)
{
	String ret=s;
	ret.pregEscape();
	return ret;
}


static String toHashRecurse(const AssocArray &a, const String &name)
{
	String r;
	String key;
	AssocArray::Iterator it;
	a.reset(it);
	while (a.getNext(it)) {
		const String &key=it.key();
		const Variant &res=it.value();
		if (res.isAssocArray()) {
			String newName;
			newName=name+"{"+key+"}";
			r+=toHashRecurse(res.toAssocArray(),newName);
		} else {
			r+=name+"{"+key+"}=\""+PerlHelper::escapeString(res.toString())+"\";\n";
		}
	}
	return r;
}

String PerlHelper::toHash(const AssocArray &a, const String &name)
{
	String ret;
	if (name.isEmpty()) return ret;
	ret="my %"+name+";\n";
	String n;
	n="$"+name;
	ret+=toHashRecurse(a,n);
	return ret;
}

}	// EOF namespace ppl7
