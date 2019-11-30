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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ppl6.h"

namespace ppl6 {

typedef struct {
	char *value;
} ROW;


/*!\class CArray
 * \ingroup PPLGroupDataTypes
 *
 */

/*!\brief Konstruktor
 *
 * \desc
 * Der Standard-Konstruktor erstellt ein leeres Array
 */
CArray::CArray()
{
	type=CVar::CARRAY;
	num=0;
	maxnum=0;
	rows=NULL;
	pos=0;
}

/*!\brief Copy-Konstruktor
 *
 * \desc
 * Mit dem Copy-Konstruktor wird der Inhalt des Arrays \p other 1:1 kopiert.
 *
 * @param other Anderes Array
 */
CArray::CArray(const CArray &other)
{
	type=CVar::CARRAY;
	num=0;
	maxnum=0;
	rows=NULL;
	pos=0;
	Copy(other);
}

/*!\brief Konstruktor aus String
 *
 * \desc
 * Mit diesem Konstruktor wird der String \p str anhand des Trennzeichens
 * \p delimiter in einzelne Elemente zerlegt
 *
 * @param str String
 * @param delimiter Trennzeichen oder Trennstring
 * @param limit Maximale Anzahl Elemente, normalerweise unbegrenzt
 * @param skipemptylines Leere Elemente überspringen. Folgen zwei Trennzeichen hintereinander, würde
 * normalerweise ein leeres Element in das Array eingefügt. Durch setzen dieses Parameters auf \c true
 * werden keine leeren Elemente eingefügt.
 *
 * \see
 * Dieser Konstruktor verwendet die Funktion Array::explode zum Zerlegen des Strings.
 */
CArray::CArray(const CString &str, const CString &delimiter, size_t limit, bool skipemptylines)
{
	type=CVar::CARRAY;
	num=0;
	maxnum=0;
	rows=NULL;
	pos=0;
	Explode(str,delimiter,limit,skipemptylines);
}


CArray::~CArray()
{
	Clear();
}


void CArray::Clear()
{
	ROW *r=(ROW*)rows;
	if (r) {
		for (ppldd i=0;i<maxnum;i++) {
			if (r[i].value) {
				free(r[i].value);
				r[i].value=NULL;
			}
		}
		free(rows);
		rows=NULL;
		maxnum=0;
		num=0;
		pos=0;
	}
}

/*!\brief Platz reservieren
 *
 * \desc
 * Durch Aufruf dieser Funktion wird vorab Speicher allokiert, um die durch \p size angegebene
 * Anzahl Elemente aufnehmen zu können. Die Funktion sollte immer dann aufgerufen werden, wenn
 * schon vor dem Befüllen des Array bekannt ist, wieviele Elemente es aufnehmen soll. Insbesondere
 * bei großen Arrays ist dies sinnvoll, da dadurch das Reallokieren und Kopieren von Speicher
 * während der Befüllung reduziert wird.
 *
 * @param size Anzahl Elemente, die das Array aufnehmen soll
 *
 * \note
 * Ist die Kapazität des Arrays bei Aufruf der Funktion bereits höher als der angegebene Wert
 * \p size, bleibt das Array unverändert. Die Kapazität kann nachträglich nicht verkleinert werden.
 *
 * \see
 * Mit der Funktion Array::capacity kann abgefragt werden, für wieviele Elemente derzeit Speicher
 * reserviert ist.
 */
void CArray::Reserve (ppldd size)
{
	if (size>maxnum) {
		ROW *r;
		// Array muss vergroessert werden
		void *newrows=realloc(rows,(size)*sizeof(ROW));
		if (!newrows) {
			throw OutOfMemoryException();
		}
		r=(ROW*)newrows;
		for (ppldd i=maxnum;i<size;i++) {
			r[i].value=NULL;
		}
		rows=newrows;
		maxnum=size;
	}
}



int CArray::Copy(const CArray *a)
{
	if (!a) {
		SetError(194);
		return 0;
	}
	return Copy(*a);
}

int CArray::Copy(const CArray &a)
{
	Clear();
	ROW *r=(ROW*)a.rows;
	for (ppluint32 i=0;i<a.num;i++) {
		if (!Set(i,r[i].value)) return 0;
	}
	return 1;
}

int CArray::Copy(const CAssocArray *a)
{
	if (!a) {
		SetError(194);
		return 0;
	}
	return Copy(*a);
}

int CArray::Copy(const CAssocArray &a)
{
	Clear();
	return a.GetCArray(*this);
}


CArray& CArray::operator=(const CArray &a)
{
	Copy(a);
	return *this;
}

CArray& CArray::operator=(const CArray *a)
{
	Copy(a);
	return *this;
}

CArray& CArray::operator=(const CAssocArray &a)
{
	Copy(a);
	return *this;
}

CArray& CArray::operator=(const CAssocArray *a)
{
	Copy(a);
	return *this;
}


int CArray::Add(const char *value, int bytes)
{
	return Set(num,value,bytes);
}

int CArray::Addf(const char *fmt, ...)
{
	if (!fmt) {
		SetError(194,"const char *fmt");
		return 0;
	}
	CString value;
	va_list args;
	va_start(args, fmt);
	value.Vasprintf(fmt,args);
	va_end(args);
	return Set(num,value);
}

int CArray::Add(int value)
{
	CString v;
	v.Setf("%i",value);
	return Set(num,v.GetPtr(),v.Len());
}

int CArray::Add(const CString &value, int bytes)
{
	return Set(num,(const char*)value.GetPtr(),bytes);
}

int CArray::Add(const CWString &value, int chars)
{
	CString s;
	if (chars>=0) {
		CWString a;
		a=value.Left(chars);
		s=a;
	} else {
		s=value;
	}
	return Set(num,(const char*)s.GetPtr());
}

int CArray::Add(const CArray &a)
{
	ROW *r=(ROW*)a.rows;
	for (ppluint32 i=0;i<a.num;i++) {
		if (!Set(num,r[i].value)) return 0;
	}
	return 1;
}

int CArray::Push(const char *value, int bytes)
{
	return Set(num,value,bytes);
}

int CArray::Set(ppldd index, const char *value, int bytes)
{
	ROW *r;
	if (index>=maxnum) {
		// Array muss vergroessert werden
		Reserve(index+10);
	}
	r=(ROW*)rows;
	if (r[index].value) {
		free(r[index].value);
		r[index].value=NULL;
	}
	if ((index+1)>num) num=index+1;
	if (value) {
		if (bytes>=0) {
			r[index].value=(char*)malloc(bytes+1);
			if (!r[index].value) {
				SetError(2);
				return 0;
			}
			strncpy(r[index].value,value,bytes);
			r[index].value[bytes]=0;
		} else {
			r[index].value=strdup(value);
		}
		if (r[index].value) {
			return 1;
		}
		SetError(2);
		return 0;
	}
	return 1;
}

int CArray::GetNumRows() const
{
	return num;
}

/*!\brief Anzahl Elemente im Array
 *
 * \desc
 * Diese Funktion gibt die Anzahl Elemente im Array zurück.
 *
 * @return Anzahl Elemente
 *
 * \note
 * Wird bei einem leeren Array ein String an der Position 5 eingefügt, werden die Positionen 0 bis 4 automatisch
 * als leere Elemente interpretiert. Array::size wird daher 6 zurückgeben.
 * \see
 * Die Funktionen CArray::Size und CArray::Num sind identisch.
 */
int CArray::Num() const
{
	return num;
}

/*!\brief Anzahl Elemente im Array
 *
 * \desc
 * Diese Funktion gibt die Anzahl Elemente im Array zurück.
 *
 * @return Anzahl Elemente
 *
 * \note
 * Wird bei einem leeren Array ein String an der Position 5 eingefügt, werden die Positionen 0 bis 4 automatisch
 * als leere Elemente interpretiert. Array::size wird daher 6 zurückgeben.
 * \see
 * Die Funktionen CArray::Size und CArray::Num sind identisch.
 */
size_t CArray::Size() const
{
	return num;
}

void CArray::List() const
{
	ROW *r=(ROW*)rows;
	if ((!rows) || num==0) {
		PrintDebug("Array ist leer\n");
	}
	for (ppldd i=0;i<num;i++) {
		PrintDebug ("%6u: %s\n",i,r[i].value);
	}
}


CString CArray::GetString(ppldd index) const
{
	CString s;
	ROW *r=(ROW*)rows;
	if (index<num) s=r[index].value;
	return s;
}

const char *CArray::Get(ppldd index) const
{
	char *ret=NULL;
	ROW *r=(ROW*)rows;
	if (index<num) ret=r[index].value;
	return ret;
}

const char *CArray::GetRandom() const
{
	char *ret=NULL;
	if (!num) return NULL;
	ROW *r=(ROW*)rows;
	ppldd index=ppl6::rand(0,num-1);
	ret=r[index].value;
	return ret;
}

const char *CArray::operator[](ppldd index) const
{
	char *ret=NULL;
	ROW *r=(ROW*)rows;
	if (index<num) ret=r[index].value;
	return ret;
}


int CArray::File2Array(const char *filename, const char *trenn)
{
	CFile ff;
	if (!ff.Open(filename,"rb")) {
		return 0;
	}
	return File2Array(ff,trenn);
}

int CArray::File2Array(CFileObject &ff, const char *trenn)
{
	const char *map=ff.Map();
	return Text2Array(map, trenn);
}

int CArray::Text2Array(const CString &text, const char *trenn)
{
	return Text2Array(text.GetPtr(),trenn);
}

int CArray::Text2Array(const char *text, const char *trenn)
{
	int ret=1;
	int p;
	int t=(int)strlen(trenn);
	while (1) {
		p=instr(text,trenn,0);
		//printf ("p=%i\n",p);
		if (p>=0) {
			ret=Add(text,p);
			if (!ret) return 0;
			text=text+p+t;
		} else {
			ret=Add(text);
			return ret;
		}
	}
	return ret;
}

int CArray::Explode(const CString &text, const char *trenn, int limit, bool skipemptylines)
{
	return Explode((const char*)text.GetPtr(), trenn, limit, skipemptylines);
}

int CArray::Explode(const char *text, const char *trenn, int limit, bool skipemptylines)
{
	if (!text) {
		SetError(194,"const char *text");
		return 0;
	}
	if (!trenn) {
		SetError(194,"const char *trenn");
		return 0;
	}
	if (text[0]==0) return 1; 	// Leerer String ergibt leeres Array
	int ret=1;
	int p;
	int t=(int)strlen(trenn);
	int count=0;
	const char *_t;
	while (1) {
		_t=strstr(text,trenn);
		if (_t) {
			p=_t-text;
			if (p==0 && skipemptylines==true) {
				text+=t;
				continue;
			}
			if (limit>0 && count>=limit) {
				return 1;
			}
			ret=Set(num,(char*)text,p);
			if (!ret) return ret;
			text=text+p+t;
			count++;
		} else {
			if (skipemptylines==false || strlen(text)>0) return Add((char*)text);
			return 1;
		}
	}
	return ret;
}


void CArray::Reset()
{
	pos=0;
}

const char *CArray::GetFirst()
{
	const char *ret=Get(0);
	pos=1;
	return ret;
}

const char *CArray::GetNext()
{
	const char *ret=Get(pos);
	pos++;
	return ret;
}

const char *CArray::Shift()
{
	const char *ret=NULL;
	if (num) {
		ROW *r=(ROW*)rows;
		if (num>0) ret=r[0].value;
		for (ppldd i=0;i<num;i++) {
			if (i+1<num) r[i].value=r[i+1].value;
			else r[i].value=NULL;
		}
		num--;
	}
	return ret;
}

int CArray::Unshift(const char *value, int bytes)
{
	// Zuerst f�gen wir das Element am Ende ein, um sicherzugehen,
	// da� am Ende des Array Platz ist
	if (!Set(num,value,bytes)) return 0;
	// Einiziges Element?
	if (num==1) {
		return 1;
	}
	// Und holen es wieder raus
	ROW *r=(ROW*)rows;
	char *element=r[num-1].value;
	for (ppldd i=num-1;i>0;i--) {
		r[i].value=r[i-1].value;
	}
	r[0].value=element;
	return 1;
}

const char *CArray::Pop()
{
	if (!num) {
		return NULL;
	}
	ROW *r=(ROW*)rows;
	char *ret=r[num-1].value;
	r[num-1].value=NULL;
	num--;
	return ret;
}

CArray *Explode(const CString &text, const char *trenn, int limit, bool skipemptylines)
{
	return Explode((const char*)text.GetPtr(),trenn, limit, skipemptylines);
}

CArray *Explode(const char *text, const char *trenn, int limit, bool skipemptylines)
{
	CArray *a=new CArray();
	if (!a) {
		SetError(2);
		return NULL;
	}
	if (a->Explode(text,trenn,limit,skipemptylines)) return a;
	PPL_ERROR *err=StoreError();
	delete a;
	RestoreError(err,true);
	return NULL;
}

CArray *Text2Array(const char *text, const char *trenn)
{
	CArray *a=new CArray();
	if (!a) return NULL;
	a->Text2Array(text,trenn);
	return a;
}

CArray *File2Array(const char *filename, const char *trenn)
{
	CArray *a=new CArray();
	if (!a) return NULL;
	a->File2Array(filename,trenn);
	return a;
}

CArray *File2Array(CFile &file, char *trenn)
{
	CArray *a=new CArray();
	if (!a) return NULL;
	a->File2Array(file,trenn);
	return a;
}


class ArraySort : private CAVLTree
{
	public:
		~ArraySort();
		virtual int	Compare(const void *value1, const void *value2) const;
		virtual int DestroyValue(void *item) const;
		int			Add(const CString &s);
		void		Reset();
		CString		*GetNext();
		CString		*GetPrevious();
};

ArraySort::~ArraySort()
{
	Clear();
}

int ArraySort::Compare(const void *value1, const void *value2) const
{
	CString *s1=(CString*)value1;
	CString *s2=(CString*)value2;
	if (*s2 < *s1) return -1;
	if (*s2 > *s1) return 1;
	return 0;
}

int ArraySort::DestroyValue(void *item) const
{
	CString *s=(CString*)item;
	delete s;
	return 1;
}

int ArraySort::Add(const CString &s)
{
	CString *a=new CString(s);
	if (!a) {
		SetError(2);
		return 0;
	}
	if (!CAVLTree::Add(a)) {
		PushError();
		delete a;
		PopError();
		return 0;
	}
	return 1;
}

void ArraySort::Reset()
{
	CAVLTree::Reset();
}


CString *ArraySort::GetNext()
{
	return (CString *)CAVLTree::GetNext();
}

CString *ArraySort::GetPrevious()
{
	return (CString *)CAVLTree::GetPrevious();
}

CArray Sort(const CArray &a)
{
	CArray r;
	ArraySort s;
	for (int i=0;i<a.Num();i++) {
		s.Add(a.GetString(i));
	}
	s.Reset();
	CString *n;
	while ((n=s.GetNext())) {
		r.Add(*n);
	}

	return r;
}


void CArray::Sort()
{
	ArraySort s;
	for (ppldd i=0;i<num;i++) {
		s.Add(GetString(i));
	}
	s.Reset();
	Clear();
	CString *n;
	while ((n=s.GetNext())) {
		Add(*n);
	}

}

void CArray::MakeUnique()
{
	ppl6::CAssocArray a;
	Reset();
	const char *value;
	while ((value=GetNext())) {
		a.Add(value,"1");
	}
	Clear();
	a.Reset();
	ppl6::CString Key,Value;
	while (a.GetNext(Key,Value)) {
		Add(Key);
	}
}

CString CArray::Implode(const CString &trenn) const
{
	CString ret;
	for (ppldd i=0;i<num;i++) {
		if (i) ret+=trenn;
		ret+=GetString(i);
	}
	return ret;
}


CArray &CArray::fromArgs(int argc, const char **argv)
{
	Clear();
	for (int i=0;i<argc;i++) {
		Add(argv[i]);
	}
	return *this;
}

CArray &CArray::fromArgs(const CString &args)
{
	Clear();
	CWString buffer(args);
	CWString arg;
	// Kommandozeile in argc und argv[] umwandeln

	size_t l=buffer.Len();
	Add(args);
	bool inDoubleQuote=false;
	bool inSingleQuote=false;
	size_t start=0;
	for (size_t i=0;i<l;i++) {
		if(buffer[i]==34 && inDoubleQuote==false && inSingleQuote==false) {
			if (i==0) {
				inDoubleQuote=true;
				start=i+1;
			}
			else if (buffer[i-1]!='\\') {
				inDoubleQuote=true;
				start=i+1;
			}
		} else if(buffer[i]=='\'' && inDoubleQuote==false && inSingleQuote==false) {
				if (i==0) {
					inSingleQuote=true;
					start=i+1;
				}
				else if (buffer[i-1]!='\\') {
					inSingleQuote=true;
					start=i+1;
				}

		} else if(buffer[i]==34 && inDoubleQuote==true && buffer[i-1]!='\\') {
			inDoubleQuote=false;
			arg=buffer.Mid(start,i-start);
			if (arg.NotEmpty()) Add(arg);

			//if(argv[argc][0]!=0) argc++;
			start=i+1;
		} else if(buffer[i]=='\'' && inSingleQuote==true && buffer[i-1]!='\\') {
			inSingleQuote=false;
			arg=buffer.Mid(start,i-start);
			if (arg.NotEmpty()) Add(arg);

			//if(argv[argc][0]!=0) argc++;
			start=i+1;
		} else if((buffer[i]==' ' || buffer[i]=='\t') && inDoubleQuote==false && inSingleQuote==false) {
			arg=Trim(buffer.Mid(start,i-start));
			if (arg.NotEmpty()) Add(arg);
			start=i+1;
		}
	}
	if (start<l) {
		arg=Trim(buffer.Mid(start,l-start));
		if (arg.NotEmpty()) Add(arg);
	}
	return *this;
}




}	// eof namespace ppl

