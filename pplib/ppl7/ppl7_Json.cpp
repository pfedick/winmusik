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
 * Copyright (c) 2018, Patrick Fedick <patrick@pfp.de>
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
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif


#include "ppl7.h"

namespace ppl7 {

struct ParserState
{
	enum state {
		ExpectingKey,
		ExpectingColon,
		ExpectingValue,
		ExpectingNextOrEnd,
	};
};

static void readDict(ppl7::AssocArray &data, ppl7::FileObject &file);
static void readArray(ppl7::AssocArray &data, ppl7::FileObject &file);

static ppl7::String getString(ppl7::FileObject &file)
{
	ppl7::String str;
	int c;
	while (!file.eof()) {
		c=file.fgetc();
		if (c=='\\') {
			c=file.fgetc();
			if (c=='n') str.append("\n");
			else if (c=='r') str.append("\r");
			else if (c=='t') str.append("\t");
			else if (c=='b') str.append("\b");
			else if (c=='f') str.append("\f");
			else if (c=='"') str.append('"');
			else if (c=='\\') str.append('\\');
			else if (c=='/') str.append('/');
			else throw InvalidEscapeSequenceException("\\%c",c);
		} else if (c=='"') {
			return str;
		} else {
			str.append(c);
		}
	}
	throw ppl7::UnexpectedEndOfDataException();
}

static ppl7::String getNumber(ppl7::FileObject &file)
{
	ppl7::String str;
	int c;
	while (!file.eof()) {
		c=file.fgetc();
		if (c=='.' || (c>='0' && c<='9')) {
			str.append(c);
		} else {
			file.seek(-1,ppl7::File::SEEKCUR);
			return str;
		}
	}
	throw ppl7::UnexpectedEndOfDataException();
}

static void readChars(ppl7::FileObject &file, const char *chars)
{
	int c,p=0;
	while (chars[p]!=0) {
		if (file.eof()) throw ppl7::UnexpectedEndOfDataException();
		c=file.fgetc();
		if (c!=chars[p])
			throw ppl7::UnexpectedCharacterException("Excpected: >>%s<<, character: >>%c<<, got: >>%c<<",
					chars,chars[p],c);
		p++;
	}
}

static bool readVaue(ppl7::AssocArray &data, const ppl7::String &key, ppl7::FileObject &file, int c)
{
	if (c=='"') {
		ppl7::String value=getString(file);
		data.set(key,value);
		return true;
	} else if (c=='[') {  // Array
		ppl7::AssocArray value;
		readArray(value,file);
		data.set(key,value);
		return true;
	} else if (c=='{') {  // dict
		ppl7::AssocArray value;
		readDict(value,file);
		data.set(key,value);
		return true;
	} else if (c=='.' ||( c>='0' && c<='9')) {
		file.seek(-1,ppl7::File::SEEKCUR);
		ppl7::String value=getNumber(file);
		data.set(key,value);
		return true;
	} else if (c=='t') {  // true
		file.seek(-1,ppl7::File::SEEKCUR);
		readChars(file, "true");
		data.set(key,ppl7::String("true"));
		return true;
	} else if (c=='f') {  // false
		file.seek(-1,ppl7::File::SEEKCUR);
		readChars(file, "false");
		data.set(key,ppl7::String("false"));
		return true;
	} else if (c=='n') {  // null
		file.seek(-1,ppl7::File::SEEKCUR);
		readChars(file, "null");
		data.set(key,ppl7::String("null"));
		return true;
	}
	return false;
}

static void readArray(ppl7::AssocArray &data, ppl7::FileObject &file)
{
	int c;
	ParserState::state state=ParserState::ExpectingValue;
	while (!file.eof()) {
		c=file.fgetc();
		if (state==ParserState::ExpectingValue && readVaue(data,"[]",file,c)==true) {
			state=ParserState::ExpectingNextOrEnd;
		} else if (c==',' && state==ParserState::ExpectingNextOrEnd) {
			state=ParserState::ExpectingValue;
		} else if (c==']' && (state==ParserState::ExpectingValue || state==ParserState::ExpectingNextOrEnd)) {
			return;
		} else if (c!=' ' && c!='\n' && c!='\r' && c!='\t') {
			throw ppl7::UnexpectedCharacterException(">>%c<< at position %lld while parsing array",c,file.tell());
		}
	}
	throw ppl7::UnexpectedEndOfDataException();
}


static void readDict(ppl7::AssocArray &data, ppl7::FileObject &file)
{
	int c;
	ppl7::String key;
	ParserState::state state=ParserState::ExpectingKey;
	while (!file.eof()) {
		c=file.fgetc();
		if (c=='"' && state==ParserState::ExpectingKey) {
			key=getString(file);
			state=ParserState::ExpectingColon;
		} else if (c==':' && state==ParserState::ExpectingColon) {
			state=ParserState::ExpectingValue;
		} else if (c==',' && state==ParserState::ExpectingNextOrEnd) {
			state=ParserState::ExpectingKey;
		} else if (state==ParserState::ExpectingValue && readVaue(data,key,file,c)==true) {
			state=ParserState::ExpectingNextOrEnd;
		} else if (c=='}' && (state==ParserState::ExpectingNextOrEnd || state==ParserState::ExpectingKey)) {
			return;
		} else if (c!=' ' && c!='\n' && c!='\r' && c!='\t') {
			throw ppl7::UnexpectedCharacterException(">>%c<< at position %lld while parsing dict",c,file.tell());
		}
	}
	throw ppl7::UnexpectedEndOfDataException();
}

static void expectEof(ppl7::FileObject &file)
{
	int c;
	while (!file.eof()) {
		c=file.fgetc();
		if (c!=' ' && c!='\n' && c!='\r' && c!='\t') {
			throw ppl7::UnexpectedCharacterException(">>%c<< at position %lld while parsing dict",c,file.tell());
		}
	}
}

void Json::loads(ppl7::AssocArray &data, const ppl7::String &json)
{
	ppl7::MemFile file((void*)json.getPtr(),json.size());
	Json::load(data,file);
}

void Json::load(ppl7::AssocArray &data, ppl7::FileObject &file)
{
	int c;
	while (!file.eof()) {
		c=file.fgetc();
		if (c=='{') {
			readDict(data,file);
			expectEof(file);
			return;
		} else if (c=='[') {
			readArray(data,file);
			expectEof(file);
			return;
		} else if (c!=' ' && c!='\n' && c!='\r' && c!='\t') {
			throw ppl7::UnexpectedCharacterException(">>%c<< at position %lld while parsing dict",c,file.tell());
		}
	}
}

ppl7::AssocArray Json::loads(const ppl7::String &json)
{
	ppl7::AssocArray result;
	Json::loads(result,json);
	return result;
}

ppl7::AssocArray Json::load(ppl7::FileObject &file)
{
	ppl7::AssocArray result;
	Json::load(result,file);
	return result;
}

void Json::dumps(ppl7::String &json, const ppl7::AssocArray &data)
{
	ppl7::MemFile file((void*)NULL,0,true);
	Json::dump(file,data);
	size_t size=file.tell();
	unsigned char *str=(unsigned char *)malloc(size+1);
	if (!str) throw OutOfMemoryException();
	file.rewind();
	file.fread(str,size,1);
	str[size]=0;
	json.useadr(str,size+1,size);
}

ppl7::String Json::dumps(const ppl7::AssocArray &data)
{
	ppl7::String result;
	Json::dumps(result,data);
	return result;
}

static bool isArray(const ppl7::AssocArray &data)
{
	uint64_t v=0;
	ppl7::AssocArray::const_iterator it;
	for (it=data.begin();it!=data.end();++it) {
		ppl7::String expectedkey;
		expectedkey.setf("%llu",v);
		if ((*it).first!=expectedkey) return false;
		v++;
	}
	if (data.begin()==data.end()) return false;
	return true;
}

static void writeArray(const ppl7::AssocArray &data, ppl7::FileObject &file);
static void writeArray(const ppl7::Array &data, ppl7::FileObject &file);
static void writeDict(const ppl7::AssocArray &data, ppl7::FileObject &file);

static void writeValue(ppl7::FileObject &file, const ppl7::String &key, const ppl7::Variant *value)
{
	if (value->isString()) {
		const ppl7::String &str=value->toString();
		if (str.isNumeric() && (!str.has(","))) file.puts(str);
		else if(str=="true" || str=="false" || str=="null") file.puts(str);
		else file.putsf("\"%s\"",(const char*)ppl7::PythonHelper::escapeString(str));
	} else if (value->isWideString()) {
		const ppl7::WideString &wstr=value->toWideString();
		ppl7::ByteArray ba=wstr.toUtf8();
		ppl7::String str(ba);
		if (str.isNumeric() && (!str.has(","))) file.puts(str);
		else if(str=="true" || str=="false" || str=="null") file.puts(str);
		else file.putsf("\"%s\"",(const char*)ppl7::PythonHelper::escapeString(str));
	} else if (value->isArray()) {
		writeArray(value->toArray(),file);
	} else if (value->isAssocArray()) {
		writeDict(value->toAssocArray(),file);
	} else if (value->isByteArrayPtr()) {
		const ppl7::ByteArrayPtr &ba=value->toByteArrayPtr();
		ppl7::String str=ba.toBase64();
		file.fputc('"');
		file.fputs(str);
		file.fputc('"');

	} else {
		//printf ("Unexpected %s: %d\n",(const char*)key,value->type());
		throw UnsupportedDataTypeException("AssocArray Type >>%d<< at key >>%s<<",value->type(),(const char*)key);
	}

}

static void writeArray(const ppl7::AssocArray &data, ppl7::FileObject &file)
{
	file.fputc('[');
	ppl7::AssocArray::const_iterator it;
	ppl7::String key="array";
	for (it=data.begin();it!=data.end();++it) {
		if (it!=data.begin()) file.fputc(',');
		writeValue(file,key,(*it).second);
	}
	file.fputc(']');
}

static void writeArray(const ppl7::Array &data, ppl7::FileObject &file)
{
	file.fputc('[');
	for(size_t i=0;i<data.size();i++) {
		if (i>0) file.fputc(',');
		file.putsf("\"%s\"",(const char*)data.getPtr(i));
	}
	file.fputc(']');
}

static void writeDict(const ppl7::AssocArray &data, ppl7::FileObject &file)
{
	if (isArray(data)) {
		writeArray(data,file);
		return;
	}
	file.fputc('{');
	ppl7::AssocArray::const_iterator it;
	for (it=data.begin();it!=data.end();++it) {
		if (it!=data.begin()) file.fputc(',');
		const ppl7::String &key=(*it).first;
		file.putsf("\"%s\":",(const char*)key);
		writeValue(file,key,(*it).second);
	}
	file.fputc('}');

}

void Json::dump(ppl7::FileObject &file, const ppl7::AssocArray &data)
{
	file.rewind();
	file.truncate(0);
	writeDict(data,file);
}



} // end of namespace ppl7

