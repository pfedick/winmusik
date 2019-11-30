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

#include "prolog.h"
#ifdef HAVE_WIDEC_H
#include <widec.h>
#endif

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
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif
#ifdef HAVE_WCTYPE_H
#include <wctype.h>
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#include "ppl6.h"
#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#ifdef HAVE_PCRE
#define PCRE_STATIC
#include <pcre.h>
#endif

namespace ppl6 {

static ppluint32 InitialBuffersize=128;
static char GlobalExtEncoding[32]="UTF-8";

#ifdef PPL6_ENABLE_STRING_BUFFERPOOL
//!\brief Interner Heap-Speicher für CString
static CHeap *Heap=NULL;

CHeap *GetCWStringHeap()
/*!\brief Pointer auf den internen Heap-Speicher der CString-Klasse
 *
 * Diese Funktion liefert einen Pointer auf den Heap-Speicher zurück, der von der
 * Klasse CString benutzt wird.
 *
 * \see
 * - CHeap
 */
{
	return Heap;
}

#endif

const char *GetGlobalEncoding()
{
	return GlobalExtEncoding;
}

int SetGlobalEncoding(const char *encoding)
/*!\brief Encoding für alle CWString-Klassen festlegen
 *
 * Standardmäßig erwartet die Klasse CWString bei allen "char*" oder "const char*" Parametern, dass der String UTF-8
 * kodiert ist. Ist dies nicht der Fall, kann mit dieser Funktion eine beliebige andere Kodierung
 * festgelegt werden. Alle CWString-Klassen, die nach Aufruf dieser Funktion instanziiert werden, erhalten
 * automatisch das neue Encoding.
 *
 * \param encoding Ein String im US-ASCII-Format, der den Namen der Kodierung enthält. Die Klasse
 * verwendet intern die iconv-Library, so dass alle Kodierungen möglich sind, die von dieser
 * unterstützt werden. Unter UNIX kann man sich durch Aufruf von "iconv -l" alle möglichen
 * Kodierungen anzeigen lassen.
 * \returns Wird die gewünschte Kodierung unterstützt, gibt die Funktion true (1) zurück, sonst false (0).
 * \example
 * \code
 * ppl6::SetGlobalEncoding("ISO-8859-15");
 * ppl6::CWString s;
 * s.Set("Dieser Text ist ISO-8859-15 kodiert.");
 * \endcode
 *
 * \see
 * Durch Aufruf der Member-Funktion ppl6::CWString::SetExtEncoding kann jeder String-Klasse ein eigenes Encoding
 * gegeben werden.
 */
{
	if (!encoding) {
		SetError(194,"int SetGlobalEncoding(const char *encoding)");
		return 0;
	}
	if (strlen(encoding)>31) {
		SetError(287);
		return 0;
	}
#ifdef HAVE_ICONV
	iconv_t ic;
	// Import
	ic=iconv_open(ICONV_UNICODE,encoding);
	if ((iconv_t)(-1)==ic) {
		SetError(287,"Import from: %s, to: %s",encoding,ICONV_UNICODE);
		return 0;
	}
	iconv_close(ic);
	// Export
	ic=iconv_open(encoding,ICONV_UNICODE);
	if ((iconv_t)(-1)==ic) {
		SetError(287,"Export from: %s to: %s",ICONV_UNICODE,encoding);
		return 0;
	}
	iconv_close(ic);
	strcpy((char*)GlobalExtEncoding,encoding);
	return 1;
#else
	SetError(286);
	return 0;
#endif
}


int IsTrue(const wchar_t *value)
/*!\brief Prüft, ob der Wide-Character-String "wahr" ist
 *
 * Diese Funktion überprüft den angegebenen Wide-Character-String, ob er "wahr" ist. Dies ist der Fall,
 * wenn eine der folgenden Bedingungen erfüllt ist:
 * - Der String enthält eine Ziffer ungleich 0
 * - Der String enthält das Wort "true" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "wahr" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "yes" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "ja" (Gross- oder Kleingeschrieben)
 *
 * \param[in] value Pointer auf einen 0-terminierten Wide-Character-String
 * \returns Liefert true (1) zurück, wenn der String "wahr" ist, sonst false (0). Ein Fehlercode wird nicht gesetzt
 * \see IsFalse(wchar_t *value)
 */
{
	if (!value) return 0;
	if (wcslen(value)==0) return 0;
	if (wcstol(value,NULL,0)!=0) return 1;
	CWString s=value;
	if (s.StrCaseCmp("true")==0) return 1;
	if (s.StrCaseCmp("wahr")==0) return 1;
	if (s.StrCaseCmp("ja")==0) return 1;
	if (s.StrCaseCmp("yes")==0) return 1;
	if (s.StrCaseCmp("t")==0) return 1;
	return 0;
}

int IsFalse(const wchar_t *value)
/*!\brief Prüft, ob der Wide-Character-String "unwahr" ist
 *
 * Diese Funktion überprüft den angegebenen Wide-Character-String, ob er "unwahr" ist. Dies ist der Fall,
 * wenn eine der folgenden Bedingungen erfüllt ist:
 * - Der String zeigt auf NULL
 * - Die Länge des Strings ist 0
 * - Der String enthält die Ziffer 0
 * - Der String enthält nicht das Wort "true", "wahr", "yes" oder "ja" (Gross-/Kleinschreibung egal)
 *
 * \param[in] value Pointer auf einen 0-terminierten Wide-Character-String
 * \returns Liefert true (1) zurück, wenn der String "unwahr" ist, sonst false (0). Ein Fehlercode wird nicht gesetzt
 * \see IsTrue(wchar_t *value)
 */
{
	if (IsTrue(value)) return 0;
	return 1;
}

int InWstr (const wchar_t * string, const wchar_t * such, unsigned int start)
/*! \brief Sucht nach Zeichen in einem Wide-Character String
 *
 * Sucht nach Zeichen in einem Wide-Character String.
 *
 * \param[in] string Pointer auf einen Wide-Character-String in dem gesucht werden soll
 * \param[in] such Pointer auf einen Wide-Character-String, der den zu suchenden String enthält
 * \param[in] start Optionaler Pointer auf den Beginn im Suchstring. Defaultmäßig wird der String
 * von Anfang an durchsucht.
 * \returns Wurde der Suchstring gefunden, wird dessen Startposition ab Position 0 (=erstes Zeichen im String)
 * zurückgeliefert. Wurde der String nicht gefunden, wird -1 zurückgegeben.
 */
{
	if (!string) return -1;
	if (!such) return -1;
	const wchar_t * _t;
	if (start<wcslen(string)) {
		_t=wcsstr((string+start),such);
		if (_t!=NULL) {
			return ((int)(_t-string));
		}
	}
	return (-1);
}


/*
 * Convert a wide character string to a long long integer.
 */
#ifdef WIN32
#elif HAVE_WCSTOLL
#else
static long long pplwcstoll(const wchar_t * __restrict nptr, wchar_t ** __restrict endptr, int base)
{
    const wchar_t *s;
    unsigned long long acc;
    wchar_t c;
    unsigned long long cutoff;
    int neg, any, cutlim;

    /*
     * See strtoll for comments as to the logic used.
     */
    s = nptr;
    do {
        c = *s++;
    } while (iswspace(c));
    if (c == L'-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == L'+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == L'0' && (*s == L'x' || *s == L'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == L'0' ? 8 : 10;
    acc = any = 0;
    if (base < 2 || base > 36)
        goto noconv;

    cutoff = neg ? (unsigned long long)-(LLONG_MIN + LLONG_MAX) + LLONG_MAX
        : LLONG_MAX;
    cutlim = cutoff % base;
    cutoff /= base;
    for ( ; ; c = *s++) {
#ifdef notyet
        if (iswdigit(c))
            c = digittoint(c);
        else
#endif
        if (c >= L'0' && c <= L'9')
            c -= L'0';
        else if (c >= L'A' && c <= L'Z')
            c -= L'A' - 10;
        else if (c >= L'a' && c <= L'z')
            c -= L'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? LLONG_MIN : LLONG_MAX;
        errno = ERANGE;
    } else if (!any) {
noconv:
        errno = EINVAL;
    } else if (neg)
        acc = -acc;
    if (endptr != NULL)
        *endptr = (wchar_t *)(any ? s - 1 : nptr);
    return (acc);
}
#endif


const CWString operator+(const CWString &str1, const CWString& str2)
{
	//mstring to return, assign the string
	CWString ms = str1;
	ms+=str2;
	return ms;
}

const CWString operator+(const char *str1, const CWString& str2)
{
	//mstring to return, assign the string
	CWString ms = str1;
	ms+=str2;
	return ms;
}

const CWString operator+(const CWString &str1, const char *str2)
{
	//mstring to return, assign the string
	CWString ms = str1;
	ms+=str2;
	return ms;
}

const CWString operator+(const wchar_t *str1, const CWString& str2)
{
	//mstring to return, assign the string
	CWString ms = str1;
	ms+=str2;
	return ms;
}

const CWString operator+(const CWString &str1, const wchar_t *str2)
{
	//mstring to return, assign the string
	CWString ms = str1;
	ms+=str2;
	return ms;
}


/*!\class CWString
 * \ingroup PPLGroupDataTypes
 * \ingroup PPLGroupStrings
 * \brief String-Klasse mit WideChars im Unicode-Format
 *
 * Diese Klasse bietet die gleiche Funktionalität, wie die ppl6::CString -Klasse, verwaltet Ihre Daten aber nicht in
 * als Multibyte-Zeichen in der UTF-8 Kodierung, sondern als Wide-Characters in Unicode-Kodierung. Mit Ihr
 * lassen sich daher problemlos alle Zeichen der Welt verarbeiten.
 *
 * Unter Windows sind Wide-Characters 16 Bit breit, unter Unix in der Regel 32 Bit.
 *
 * Für den Im- und Export von anderen Kodierungen wird die ICONV-Library verwendet.
 *
 */

#ifdef PPL6_ENABLE_STRING_BUFFERPOOL
void *CWString::operator new(size_t size)
/*!\brief Speicher allokieren
 *
 * Der New-Operator dieser Klasse wurde überschrieben, damit der Speicher nicht mehr aus dem
 * Hauptspeicher allokiert wird und diesen somit schnell fragmentiert, sondern aus dem internen
 * Heap.
 *
 * \param[in] size Größe des benötigten Speicherblocks. Dieser muss identisch sein mit
 * sizeof(CString)
 *
 * \returns bei Erfolg gibt die Funktion einen Pointer auf den Beginn des Speicherblocks zurück,
 * im Fehlerfall wird eine Exception generiert
 *
 * \see
 * - Cppl6Core::GetHeap_CString
 *
 *
 */
{
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_CWString();
			if (!Heap) {
				throw ppl6::GetError();
			}
		}
	}
	void *p=Heap->Malloc();
	if (!p) {
		throw ppl6::GetError();
	}
	return p;
}

void CWString::operator delete(void *ptr, size_t size)
/*!\brief Speicher freigeben
 *
 * Die Klasse verwendet Speicher aus dem internen Heap, der durch Aufruf dieses überladenen
 * delete-Operators wieder freigegeben wird
 *
 * \param[in] ptr Zeiger auf den freizugebenden Speicherblock
 * \param[in] size Größe des Speicherblocks.
 */
{
	if (ptr!=NULL && Heap!=NULL) Heap->Free(ptr);
}

#endif

/** @name Konstruktoren
 *  Folgende Konstruktoren stehen zur Verfügung:
 */
//@{
CWString::CWString()
/*!\brief Konstruktor: Leeren String erzeugen
 *
 * Erzeugt einen Leeren String
*/
{
	Init();
}

CWString::CWString(const char *str)
/*!\brief Konstruktor: String aus einem "const char *" erstellen
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist ein <tt>const char *</tt> auf einen 0-terminierten String im UTF-8-Format.
 *
 * \copydoc CWStringEncoding.dox
 */
{
	Init();
	Set(str);
}

CWString::CWString(const wchar_t *str)
/*!\brief Konstruktor: String aus einem "const wchar_t *" erstellen
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist ein <tt>const wchar_t *</tt> auf einen 0-terminierten String im Wide-Character Format (Unicode).
 */
{
	Init();
	Set(str);
}

CWString::CWString(const std::wstring &str)
{
	Init();
	Set(str.c_str());
}

CWString::CWString(const CString &str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist eine andere Instanz der String-Klasse.
 *
 * \copydoc CWStringEncoding.dox
 */
{
	Init();
	//printf("Aufruf mit const char &str: %s\n",str);
	Set((const char*)str.GetPtr());
}

CWString::CWString(const CString *str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist ein Pointer auf eine andere String-Klasse.
 *
 * \copydoc CWStringEncoding.dox
 */
{
	Init();
	//printf("Aufruf mit const char *str: %s\n",str);
	Set(str);
}


CWString::CWString(const CWString &str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist eine andere Instanz der String-Klasse.
 */
{
	Init();
	//printf("Aufruf mit const char &str: %s\n",str);
	Set(str.buffer);
}

CWString::CWString(const CWString *str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist ein Pointer auf eine andere String-Klasse.
 */
{
	Init();
	//printf("Aufruf mit const char *str: %s\n",str);
	Set(str);
}

CWString::CWString(const wchar_t c)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt des angegebenen Zeichens \p c
 * \param c ist der Unicode des gewünschten Zeichens
 */
{
	Init();
	wchar_t a[2];
	a[0]=c;
	a[1]=0;
	Set(a);
}

//@}

/** @name Destruktor
 */
//@{

CWString::~CWString()
/*!
 * \brief Destruktor der String-Klasse
 *
 * Der Destruktor gibt alle von dieser Instanz verwendeten Speicherbereiche wieder frei.
 */
{
	if (buffer) free(buffer);
	if (extbuffer) free(extbuffer);
	if (matches) delete matches;
	matches=NULL;
	buffer=NULL;
	extbuffer=NULL;
	extbuffersize=0;

#ifdef HAVE_ICONV
	if (iconvimport) iconv_close((iconv_t) iconvimport);
	if (iconvexport) iconv_close((iconv_t) iconvexport);
#endif
}
//@}

void CWString::Init()
/*!\brief Interne Funktion: Initialisierung des Strings
 *
 * Diese Funktion wird intern von den Konstruktoren aufgerufen, um alle internen Variablen und Puffer auf NULL
 * zu setzen
 */
{
	type=CVar::CWSTRING;
	matches=NULL;
	buffer=NULL;
	extbuffer=NULL;
	extbuffersize=0;
	len=buffersize=bufferused=0;
	iconvimport=NULL;
	iconvexport=NULL;
	strcpy((char*)extencoding,GlobalExtEncoding);
}

void CWString::ImportBuffer(wchar_t *buffer, size_t bytes)
/*!\brief Externen Buffer übernehmen
 *
 * Mit dieser Funktion kann ein externer Buffer in den String übernommen werden. Dazu muss
 * die Anwendung zunächst Speicher mit \c malloc allokieren und diesen mit einem gültigen
 * Wide Character String
 * versehen. Dieser muss mit einem 0-Wert enden. Der Pointer auf den Beginn des Speichers und die
 * allokierte Größe wird dann mit dieser Funktion in den String importiert. Die Verwaltung des
 * Speicherblocks geht dabei an die Stringklasse über, darf also nicht mehr von der Anwendung
 * verändert oder gelöscht werden!
 *
 * Danach prüft die Funktion noch die tatsächliche Länge des Strings durch Aufruf der
 * Funktion ReCalcLen, für den Fall dass mehr Speicher allokiert wurde, als
 * tatsächlich gebraucht wurde.
 *
 * Falls der String vor Aufruf dieser Funktion noch Daten enthalten hat, werden diese
 * ordnungsgemäß gelöscht.
 *
 * @param[in] buffer Pointer auf den Speicherbereich, ab dem der String beginnt
 * @param[in] bytes Anzahl Bytes, die beim \c malloc allokiert wurden. Dies muss zwingend ein
 * Vielfaches von sizeof(wchar_t) sein!
 *
 * \note
 * Falls der String nicht mit einem 0-Wert beendet wurde, wird automatisch das letzte Zeichen
 * des Buffers auf 0 gesetzt. Das könnte einen ungewollten Effekt haben, falls das Zeichen
 * an dieser Stelle wichtig war. Es sollte daher immer mindestens sizeof(wchar_t) Bytes
 * mehr allokiert werden, als tatsächlich benötigt wird.
 *
 * \since
 * Diese Funktion wurde mit Version 6.3.0 eingeführt
 */
{
	if (this->buffer) free(this->buffer);
	if (matches) delete matches;
	matches=NULL;
	this->buffer=NULL;
	buffersize=len=0;
	if (buffer==NULL || bytes==0 || bytes%(sizeof(wchar_t))>0) return;
	this->buffer=buffer;
	buffersize=bytes;
	ReCalcLen();
}

void CWString::Clear()
/*!
 * \brief Löscht den Inhalt des Strings
 *
 * Der Inhalt des Strings wird gelöscht und seine Länge auf 0 gesetzt. Außerden werden
 * alle belegten Speicherblöcke freigegeben.
 * \return Nichts
 */
{
	if (buffer) free(buffer);
	if (extbuffer) free(extbuffer);
	if (matches) delete matches;
	matches=NULL;
	buffer=NULL;
	buffersize=len=bufferused=0;
	extbuffer=NULL;
	extbuffersize=0;
}


int CWString::SetExtEncoding(const char *encoding)
/*!\brief Encoding für Stringübergabe festlegen
 *
 * Standardmäßig erwartet die Klasse bei allen "char*" oder "const char*" Parametern, dass der String UTF-8
 * kodiert ist. Ist dies nicht der Fall, kann mit dieser Funktion eine beliebige andere Kodierung
 * festgelegt werden.
 *
 * \param encoding Ein String im US-ASCII-Format, der den Namen der Kodierung enthält. Die Klasse
 * verwendet intern die iconv-Library, so dass alle Kodierungen möglich sind, die von dieser
 * unterstützt werden. Unter UNIX kann man sich durch Aufruf von "iconv -l" alle möglichen
 * Kodierungen anzeigen lassen.
 * \returns Wird die gewünschte Kodierung unterstützt, gibt die Funktion true (1) zurück, sonst false (0).
 * \example
 * \code
 * CWString s;
 * s.SetExtEncoding("ISO-8859-15");
 * s.Set("Dieser Text ist ISO-8859-15 kodiert.");
 * \endcode
 *
 * \note
 * Möchte man das Encoding für alle CWStrings auf globaler Ebene ändern, kann man die Funktion
 * ppl6::SetGlobalEncoding verwenden.
 */
{
	if (!encoding) {
		SetError(194,"int CWString::SetExtEncoding(const char *encoding)");
		return 0;
	}
	if (strlen(encoding)>31) {
		SetError(287);
		return 0;
	}
#ifdef HAVE_ICONV
	if (iconvimport) iconv_close((iconv_t) iconvimport);
	if (iconvexport) iconv_close((iconv_t) iconvexport);
	iconvimport=NULL;
	iconvexport=NULL;
	if (!InitImportEncoding(encoding)) return 0;
	if (!InitExportEncoding(encoding)) return 0;
	strcpy((char*)extencoding,encoding);
	return 1;
#else
	SetError(286);
	return 0;
#endif
}

int CWString::InitImportEncoding(const char *encoding)
/*!\brief Encoding für Import einstellen
 *
 * Diese Funktion wird intern aufgerufen, um die iconv-Funktion für den Import von UTF-8 Strings zu
 * initialisieren.
 */
{
#ifdef HAVE_ICONV
	if (iconvimport) iconv_close((iconv_t) iconvimport);
	iconvimport=iconv_open(ICONV_UNICODE,encoding);
	if ((iconv_t)(-1)==iconvimport) {
		iconvimport=NULL;
		SetError(287,"from: %s, to: %s",encoding,ICONV_UNICODE);
		return 0;
	}
	return 1;
#else
	SetError(286);
	return 0;
#endif
}

int CWString::InitExportEncoding(const char *encoding)
/*!\brief Encoding für Export einstellen
 *
 * Diese Funktion wird intern aufgerufen, um die iconv-Funktion für den Export von UTF-8 Strings zu
 * initialisieren.
 */
{
#ifdef HAVE_ICONV
	if (iconvexport) iconv_close((iconv_t) iconvexport);
	iconvexport=iconv_open(encoding,ICONV_UNICODE);
	if ((iconv_t)(-1)==iconvexport) {
		iconvexport=NULL;
		SetError(287,"from: %s to %s",ICONV_UNICODE,encoding);
		return 0;
	}
	return 1;
#else
	SetError(286);
	return 0;
#endif
}

/** @name Daten setzen
 *  Mit folgenden Funktionen wird der String mit Daten belegt
 */
//@{

int CWString::Set(const char *text, int size)
/*!
 * \brief Belegt den String mit einem 0-terminierten UTF-8 String
 *
 * Mit Set wird dem String ein Wert zugewiesen.
 *
 * \param[in] text Ein Pointer auf einen UTF-8 kodierten C-String, der in die String-Klasse kopiert werden soll.
 * \param[in] size Optionaler Parameter, der die Anzahl zu importierender Zeichen angibt.
 * Ist der Wert nicht angegeben, wird der komplette String übernommen. Ist der Wert größer als
 * der angegebene String, wird er ignoriert und der komplette String importiert.
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
#ifndef HAVE_ICONV
#ifdef HAVE_MBSTOWCS
	size_t inbytes;
	if (size>0) inbytes=size;
	else inbytes=strlen(text);
	size_t outbytes=(inbytes+1)*sizeof(wchar_t);
	if (outbytes>=buffersize) {
		if (buffer) free(buffer);
		buffersize=InitialBuffersize;
		if (buffersize<=outbytes) buffersize=((outbytes/InitialBuffersize)+1)*InitialBuffersize+sizeof(wchar_t);
		buffer=(wchar_t*)malloc(buffersize);
		if (!buffer) {
			SetError(2);
			return 0;
		}
	}
	size_t ret=mbstowcs(buffer, text, inbytes);
	if (ret==(size_t) -1) {
		buffer[0]=0;
		len=0;
		SetError(289,"%s",strerror(errno));
		return 0;
	}
	buffer[ret]=0;
	len=ret;
	bufferused=(len+1)*sizeof(wchar_t);
	return 1;
#endif

	SetError(286);
	return 0;
#else
	if (!text) {
		Clear();
		return 1;
	}
	if ((!iconvimport) && (!InitImportEncoding(extencoding))) return 0;
	size_t inbytes=strlen(text);
	size_t outbytes=(inbytes+1)*sizeof(wchar_t);
	wchar_t *newstring=(wchar_t*)malloc(outbytes);
	if (!newstring) {
		SetError(2);
		return 0;
	}
	char *outbuf=(char*)newstring;
	size_t res=iconv((iconv_t)iconvimport, (ICONV_CONST char **)&text, &inbytes,
				(char**)&outbuf, &outbytes);
	if (res==(size_t)(-1)) {
		if (buffer) buffer[0]=0;
		len=0;
		SetError(289,"%s",strerror(errno));
		PrintError();
		return 0;
	}
	((wchar_t*)outbuf)[0]=0;
	int ret=Set(newstring,size);
	free(newstring);
	return ret;
#endif
}

int CWString::Set(const wchar_t *text, int size)
/*!\brief Belegt den String mit einem Wide-Character Wert
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] text Pointer auf einen Wide-Character String
 * \param[in] size Die Anzahl Zeichen (nicht Bytes) im String, die kopiert werden sollen.
 * Wird der Parameter nicht angegeben, wird der komplette String übernommen. Ist \c size länger als
 * der in \c text enthaltene String, wird nur bis zum 0-Byte kopiert.
 *
 * \copydoc CWStringSetFunctions.dox
 */
{
	if (!text) {
		Clear();
		return 1;
	}
	size_t inbytes, inchars;
	if (size>0) inchars=size;
	else inchars=wcslen(text);
	inbytes=(inchars+1)*sizeof(wchar_t);
	if (inbytes>=buffersize) {
		if (buffer) free(buffer);
		buffersize=InitialBuffersize;
		if (buffersize<=inbytes) buffersize=((inbytes/InitialBuffersize)+1)*InitialBuffersize+sizeof(wchar_t);
		buffer=(wchar_t*)malloc(buffersize);
		if (!buffer) {
			SetError(2);
			return 0;
		}
	}
	wmemcpy(buffer,text,inchars);
	buffer[inchars]=0;
	len=inchars;
	bufferused=(len+1)*sizeof(wchar_t);
	return 1;
}

int CWString::Set(const CString *str, int size)
/*!\brief Belegt den String mit dem Inhalt eines CString
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str Pointer auf einen CString
 * \param[in] size Optionaler Parameter, der die Anzahl zu importierender Zeichen angibt.
 * Ist der Wert nicht angegeben, wird der komplette String übernommen. Ist der Wert größer als
 * der angegebene String, wird er ignoriert und der komplette String importiert.
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
	if (!str) {
		SetError(194,"int CWString::Set(CString *str)");
		return 0;
	}
	return Set((const char *)str->GetPtr(),size);
}

int CWString::Set(const CString &str, int size)
/*!\brief Belegt den String mit dem Inhalt eines CString
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str CString-Objekt
 * \param[in] size Optionaler Parameter, der die Anzahl zu importierender Zeichen angibt.
 * Ist der Wert nicht angegeben, wird der komplette String übernommen. Ist der Wert größer als
 * der angegebene String, wird er ignoriert und der komplette String importiert.
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
	return Set((const char *)str.GetPtr(),size);
}

int CWString::Set(const CWString *str, int chars)
/*!\brief Belegt den String mit dem Inhalt eines anderen CWString
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str Pointer auf einen Wide-Character String
 * \param[in] chars Die Anzahl Zeichen (nicht Bytes) im String, die kopiert werden sollen.
 * Wird der Parameter nicht angegeben, wird der komplette String übernommen. Ist \c chars länger als
 * der in \c str enthaltene String, wird nur bis zum 0-Byte kopiert.
 *
 * \copydoc CWStringSetFunctions.dox
 */
{
	return Set(str->buffer,chars);
}

int CWString::Set(const CWString &str, int chars)
/*!\brief Belegt den String mit dem Inhalt eines anderen CWString
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str Pointer auf einen Wide-Character String
 * \param[in] chars Die Anzahl Zeichen (nicht Bytes) im String, die kopiert werden sollen.
 * Wird der Parameter nicht angegeben, wird der komplette String übernommen. Ist \c chars länger als
 * der in \c str enthaltene String, wird nur bis zum 0-Byte kopiert.
 *
 * \copydoc CWStringSetFunctions.dox
 */
{
	return Set(str.buffer,chars);
}

int CWString::Strcpy(const char *str)
/*!\brief Belegt den String mit einem 0-terminierten C-String
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str Pointer auf einen 0-terminierten C-String im UTF-8-Format
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
	return Set(str);
}

int CWString::Strncpy(const char *str, int size)
/*!
 * \brief Belegt den String mit einem 0-terminierten UTF-8 String
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str Ein Pointer auf einen UTF-8 kodierten C-String, der in die String-Klasse kopiert werden soll.
 * \param[in] size Optionaler Parameter, der die Anzahl zu importierender Zeichen angibt.
 * Ist der Wert nicht angegeben, wird der komplette String übernommen. Ist der Wert größer als
 * der angegebene String, wird er ignoriert und der komplette String importiert.
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
	return Set(str,size);
}


int CWString::Copy(const char *str, int size)
/*!
 * \brief Belegt den String mit einem 0-terminierten UTF-8 String
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str Ein Pointer auf einen UTF-8 kodierten C-String, der in die String-Klasse kopiert werden soll.
 * \param[in] size Optionaler Parameter, der die Anzahl zu importierender Zeichen angibt.
 * Ist der Wert nicht angegeben, wird der komplette String übernommen. Ist der Wert größer als
 * der angegebene String, wird er ignoriert und der komplette String importiert.
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
	return Set(str,size);
}

int CWString::Copy(const CString &str, int size)
/*!
 * \brief Belegt den String mit dem Inhalt eines CString
 *
 * Mit dieser Funktion wird dem String ein Wert zugewiesen.
 *
 * \param[in] str Ein Pointer auf eine CString-Klasse
 * \param[in] size Optionaler Parameter, der die Anzahl zu importierender Zeichen angibt.
 * Ist der Wert nicht angegeben, wird der komplette String übernommen. Ist der Wert größer als
 * der angegebene String, wird er ignoriert und der komplette String importiert.
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
	return Set(str,size);
}

int CWString::Import(const char *encoding, void *buffer, int bytes)
/*!\brief Importiert einen String der im angegebenen Charset vorliegt
 *
 * Diese Funktion importiert einen String, der in einem beliebigen Charset vorliegt.
 *
 * \param encoding Name der Kodierung, in der der zu importierende String vorliegt
 * (z.B. "ISO-8859-15")
 * \param buffer Pointer auf den Speicherbereich, an dem der zu importierende
 * String beginnt. Sofern der Parameter \p len nicht angegeben wurde, muss dieser
 * mit einem 0-Byte enden
 * \param bytes Optionaler Parameter, der die Länge des Strings in Byte angibt
 * \returns Die Funktion liefert true (1) zurück, wenn der String erfolgreich
 * importiert werden konnte, andernfalls false (0).
 */
{
#ifndef HAVE_ICONV
	SetError(286);
	return 0;
#else
	if (!encoding) {
		SetError(194,"int CWString::Import(==> const char *encoding <==, void *buffer, int bytes)");
		return 0;
	}
	if (!buffer) {
		SetError(194,"int CWString::Import(const char *encoding, ==> void *buffer <==, int bytes)");
		return 0;
	}
	iconv_t ic;
	// Import
	ic=iconv_open(ICONV_UNICODE,encoding);
	if ((iconv_t)(-1)==ic) {
		SetError(287,"Import from: %s, to: %s",encoding,ICONV_UNICODE);
		return 0;
	}
	size_t inbytes;
	if (bytes>0) inbytes=bytes;
	else inbytes=strlen((char*)buffer);
	size_t outbytes=(inbytes+1)*sizeof(wchar_t);
	if (outbytes>=buffersize) {
		if (this->buffer) free(this->buffer);
		buffersize=InitialBuffersize;
		if (buffersize<=outbytes) buffersize=((outbytes/InitialBuffersize)+1)*InitialBuffersize+sizeof(wchar_t);
		this->buffer=(wchar_t*)malloc(buffersize);
		if (!this->buffer) {
			iconv_close(ic);
			SetError(2);
			return 0;
		}
	}
	char *outbuf=(char*)this->buffer;
	char *bb=(char*)buffer;
	size_t res=iconv(ic, (ICONV_CONST char **)&bb, &inbytes,
				(char**)&outbuf, &outbytes);
	if (res==(size_t)(-1)) {
		this->buffer[0]=0;
		len=0;
		bufferused=0;
		SetError(289,"%s",strerror(errno));
		iconv_close(ic);
		return 0;
	}
	iconv_close(ic);
	((wchar_t*)outbuf)[0]=0;
	len=wcslen(this->buffer);
	bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	return 1;
#endif
}

void CWString::HexDump(const void *buffer, int bytes, bool skipheader)
/*!\brief Hexdump eines Speicherbereichs im String ablegen
 *
 * Diese Funktion speichert den HexDump des angegebenen Speicherbereichs im String.
 *
 * \param[in] buffer Pointer auf den Beginn des Speicherbereichs, der gedumpt werden soll
 * \param[in] bytes Anzahl Bytes, die gedumpt werden sollen
 * \param[in] skipheader Optionaler Schalter, der angibt, ob vor dem Dump ein Header ausgegeben
 * werden soll (Default=false)
 */
{
	CString s;
	ppl6::HexDump(&s,buffer,len*sizeof(wchar_t)+1,skipheader);
	Set(s);
}

int CWString::Setf(const char *fmt, ...)
//! \brief Erzeugt einen formatierten String
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		int ret=Set(buff);
		va_end(args);
		free(buff);
	    return ret;
	}
	va_end(args);
    if (buff) free(buff);
    SetError(259);
    return 0;
}

int CWString::Setf(const wchar_t *fmt, ...)
//! \brief Erzeugt einen formatierten String
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	// TODO: Verwendung von vswprintf statt vasprintf
	CString f=fmt;
	if (vasprintf (&buff, (const char*)f, args)>0 && buff!=NULL) {
		int ret=Set(buff);
		va_end(args);
		free(buff);
	    return ret;
	}
	va_end(args);
    if (buff) free(buff);
    SetError(259);
    return 0;
}


int CWString::Sprintf(const char *fmt, ...)
//! \brief Erzeugt einen formatierten String
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		int ret=Set(buff);
		va_end(args);
		free(buff);
	    return ret;
	}
	va_end(args);
    if (buff) free(buff);
    SetError(259);
    return 0;
}

int CWString::VaSprintf(const char *fmt, va_list args)
/*! \brief Erzeugt einen formatierten String
 *
 * \copydoc CString::Vasprintf
 */
{
	return Vasprintf(fmt,args);
}

int CWString::Vasprintf(const char *fmt, va_list args)
/*! \brief Erzeugt einen formatierten String
 *
 * Erzeugt einen formatierten String
 * \param fmt Der Formatstring
 * \param args Pointer auf Liste der Parameter. Muss zuvor mit va_start initialisiert worden sein.
 * \par Example:
 * \code
void MyFunction(const char *fmt, ...)
{
	CWString String;
	va_list args;
	va_start(args, fmt);
	String.VaSprintf(fmt,args);
	va_end(args);
	printf ("Ergebnis: %s",(char*)String);
}
\endcode
 *
 * \copydoc formatstring
 */
{
	if (!fmt) {
		SetError(194,"int CWString::Vasprintf(==> const char *fmt <==, va_list args)");
		return 0;
	}
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		int ret=Set(buff);
		free(buff);
		return ret;
	}
    if (buff) free(buff);
    SetError(259);
	return 0;
}

//@}

/** @name String verlängern
 */
//@{

int CWString::Add(const char *str, int size)
/*!
 * \brief UTF-8-String anhängen
 *
 * Mit dieser Funktion der übergebene String an den bereits vorhandenen angehangen
 *
 * \param[in] str Ein Pointer auf einen 0-terminierten C-String im UTF-8-Format
 * \param[in] size Optionaler Parameter, der die Anzahl zu importierender Zeichen angibt.
 * Ist der Wert nicht angegeben, wird der komplette String übernommen. Ist der Wert größer als
 * der angegebene String, wird er ignoriert und der komplette String importiert.
 *
 * \copydoc CWStringSetFunctions.dox
 *
 * \copydoc CWStringEncoding.dox
 */
{
	return Concat(str,size);
}

int CWString::Concat(const char *text, int size)
//! \brief Fügt einen String an das Ende des vorhandenen an
{
	if (!buffer) {
		return Set(text,size);
	}
	CWString s2;
	s2.Set(text,size);
	return Concat(s2.buffer,s2.len);
}

int CWString::Concat(const wchar_t *text, int size)
/*!\brief Fügt einen Wide-Character String an den bestehenden an
 *
 * \param[in] text Pointer auf einen Wide-Character String
 * \param[in] size Die Anzahl Zeichen (nicht Bytes) im String, die kopiert werden sollen.
 */
{
	if (!buffer) {
		return Set(text,size);
	}
	if (!text) {
		SetError(194,"int CWString::Concat(==> const wchar_t *text <==, int bytes)");
		return 0;
	}
	size_t inchars;
	if (size>0) inchars=size;
	else inchars=wcslen(text);
	size_t inbuffer=inchars*sizeof(wchar_t)+sizeof(wchar_t);

	if (bufferused+inbuffer>=buffersize) {
		buffersize=(((bufferused+inbuffer)/InitialBuffersize)+1)*InitialBuffersize+sizeof(wchar_t);
		wchar_t *t=(wchar_t*)realloc(buffer,buffersize);
		if (!t) {
			SetError(2,"int CWString::Concat(const wchar_t *text, int bytes)");
			return 0;
		}
		buffer=t;
	}
	wmemcpy(buffer+len,text,inchars);
	buffer[len+inchars]=0;
	len=len+inchars;
	bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	return 1;
}
int CWString::Concatf(const char *fmt, ...)
//! \brief Fügt einen formatierten String an das Ende des vorhandenen an
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		int ret=Concat(buff);
		va_end(args);
		free(buff);
		return ret;
	}
	va_end(args);
    if (buff) free(buff);
    return 0;
}
int CWString::Concatf(const wchar_t *fmt, ...)
//! \brief Fügt einen formatierten String an das Ende des vorhandenen an
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	// TODO: Verwendung von vswprintf statt vasprintf
	CString f=fmt;
	if (vasprintf (&buff, (const char*)f, args)>0 && buff!=NULL) {
		int ret=Concat(buff);
		va_end(args);
		free(buff);
		return ret;
	}
	va_end(args);
    if (buff) free(buff);
    return 0;
}


int CWString::Concat(CString *str, int size)
//! \brief Fügt einen String an das Ende des vorhandenen an
{
	if (!str) {
		SetError(194,"int CString::Concat(==> CString *str <==, int bytes)");
		return 0;
	}
	return Concat(str->GetPtr(),size);
}
int CWString::Concat(CString str, int size)
//! \brief Fügt einen String an das Ende des vorhandenen an
{
	return Concat(str.GetPtr(),size);
}
int CWString::Concat(CWString *str, int size)
//! \brief Fügt einen String an das Ende des vorhandenen an
{
	if (!str) {
		SetError(194,"int CString::Concat(==> CWString *str <==, int size)");
		return 0;
	}
	return Concat(str->buffer,size);
}
int CWString::Concat(CWString str, int size)
//! \brief Fügt einen String an das Ende des vorhandenen an
{
	return Concat(str.buffer,size);
}


//@}

/** @name String Informationen
 */
//@{




size_t CWString::Size() const
/*!\brief Gibt die Anzahl Bytes zurück, die durch den String belegt werden
 *
 * Diese Funktion gibt die Anzahl Bytes zurück, die durch den String belegt werden. Das
 * schließende Null-Byte wird dabei nicht mitgezählt.
 *
 * \return Gibt Anzahl Bytes zurück, die durch den String belegt werden oder 0, wenn der
 * String Leer ist.
 * \see
 * - CWString::Len
 * - CWString::Length
 * - CWString::Size
 */
{
	return (int)(len*sizeof(wchar_t));
}

size_t CWString::Len() const
/*!\brief Gibt die Länge des Strings zurück
 *
 * Diese Funktion gibt die Anzahl Zeichen zurück, die im String enthalten sind.
 *
 * \return Gibt Anzahl im String enthaltener Zeichen zurück, oder 0, wenn der String Leer ist.
 * \see
 * - CWString::Len
 * - CWString::Length
 * - CWString::Size
 */
{
	return (int)len;
}

size_t CWString::Length() const
/*!\brief Gibt die Länge des Strings zurück
 *
 * Diese Funktion gibt die Anzahl Zeichen zurück, die im String enthalten sind.
 *
 * \return Gibt Anzahl im String enthaltener Zeichen zurück, oder 0, wenn der String Leer ist.
 * \see
 * - CWString::Len
 * - CWString::Length
 * - CWString::Size
 */
{
	return (int)len;
}

/*!\brief Prüft, ob der String nummerisch ist
 *
 * \desc
 * Diese Funktion prüft, ob im String nur nummerische Zeichen vorhanden sind, also die Ziffern
 * 0-9, Punkt, Komma und Minus.
 *
 * @return Ist der String nummerisch, wird 1 zurückgegeben. Ist er es nicht oder ist der String
 * leer, wird 0 zurückgegeben.
 */
int CWString::IsNumeric() const
{
	wchar_t c;
	if (!len) return 0;
	for (ppluint32 i=0;i<len;i++) {
		c=buffer[i];
		if (c<'0' || c>'9') {
			if (c!='.' && c!=',' && c!='-') return 0;
			if (c=='-' && i>0) return 0;
		}
	}
	return 1;
}

/*!\brief Prüft, ob der String einen Integer Wert enthält
 *
 * \desc
 * Diese Funktion prüft, ob im String einen integer Wert enthält, also nur die Ziffern
 * 0-9 und optional ein Minus am Anfang enthalten sind
 *
 * @return Ist der String ein Integer, wird 1 zurückgegeben. Ist er es nicht oder ist der String
 * leer, wird 0 zurückgegeben.
 */
int CWString::IsInteger() const
{
	int c;
	if (!len) return 0;
	for (ppluint32 i=0;i<len;i++) {
		c=buffer[i];
		if (c<'0' || c>'9') {
			if (c=='-' && i==0) continue;		// Minus am Anfang ist erlaubt
			return 0;
		}
	}
	return 1;
}



int CWString::IsEmpty() const
/*! \brief Prüft, ob der String leer ist
 *
 * Diese Funktion prüft, ob der String Zeichen enthält
 *
 * \returns Ist der String leer, liefert die Funktion true (1) zurück, sonst false.
 * \see CWString::NotEmpty
 */
{
	if (!len) return 1;
	return 0;
}

int CWString::NotEmpty() const
/*! \brief Prüft, ob der String Daten enthält
 *
 * Diese Funktion prüft, ob der String Zeichen enthält
 *
 * \returns Enthält der String Daten (Länge > 0), liefert die Funktion true (1) zurück, sonst false.
 * \see CWString::IsEmpty
 */
{
	if (len) return 1;
	return 0;
}

int CWString::Capacity() const
/*!\brief Maximale Stringgröße ohne neuen Speicher zu allokieren
 *
 * \desc
 * Die Stringfunktion verwaltet intern einen Speicherblock, der größer ist, als für den aktuellen Inhalt
 * erforderlich wäre. Das hat den Vorteil, dass bei Änderungen am String in den meisten Fällen kein neuer Speicher
 * allokiert werden muß, da noch genug Reserve vorhanden ist. Dadurch wird der Speicher weniger fragmentiert
 * und man gewinnt etwas an Performance.
 *
 * Mit dieser Funktion kann man feststellen, wieviele Zeichen der String maximal groß werden darf, ohne dass neuer
 * Speicher allokiert werden muss.
 * \returns Anzahl Zeichen, die in den aktuellen Puffer passen (ohne terminierenden 0-Wert)
 * \note
 * Beim Import von Strings in einer anderen Kodierung (insbesondere bei Multibyte-Encodings wie UTF-8), geht die
 * Klasse immer vom "worst case" aus und errechnet mehr Speicherbedarf als wahrscheinlich notwendig wäre.
 * Dadurch kommt es vor, dass dennoch neuer Speicher allokiert wird, obwohl der String in den vorhandenen
 * passen würde.
 */
{
	int c=(buffersize/sizeof(wchar_t))-1;
	if (c>0) return c;
	return 0;
}

//@}


void CWString::HexDump() const
/*! \brief Macht einen Hexdump des Strings auf STDOUT
 *
 * Der Inhalt des aktuellen Strings wird als HexDump auf STDOUT ausgegeben.
 */
{
	PrintDebug ("HexDump CWString\n");
	if (buffer) {
		ppl6::HexDump(buffer,len*sizeof(wchar_t)+sizeof(wchar_t));
	}
}


int CWString::Reserve(int chars)
/*! \brief Reserviert Speicher für den String
 *
 * Reserviert Speicher fuer den String. Ein eventuell schon vorhandener
 * String geht dabei nicht verloren. Wenn ein größerer String aus vielen kleinen Einzelteilen zusammengesetzt
 * werden soll, bringt der Aufruf dieser Funktion einen kleinen Performance-Vorteil, da nicht so häufig
 * (im Idealfall garnicht) Speicher neu allokiert werden muß.
 *
 * \param[in] chars Anzahl Zeichen, für die Speicher allokiert werden soll.
 * \returns Liefert true (1) zurück, wenn der gewünschte Speicher allokiert werden konnte, sonst false (0).
 *
 * \remarks
 * Es ist zu beachten, dass die CWString-Klasse den String intern als Unicode im Datentyp wchar_t
 * speichert. Dieser ist in der Regel 16 (Windows) oder 32-Bit (Unix) breit.
 * Der Speicherbedarf errechnet sich daher aus Anzahl Zeichen * 2 bzw. *4.
 *
 * Ist der Puffer des Strings bereits größer wie gewünscht, bleibt der Puffer unverändert.
 */
{
	if (chars>Capacity()) {
		size_t size=chars*sizeof(wchar_t)+sizeof(wchar_t);
		if (buffer) {
			wchar_t *t=(wchar_t*)realloc(buffer,size);
			if (t) {
				buffer=t;
				buffersize=size;
				return 1;
			}
			SetError(2);
			return 0;
		}
		buffer=(wchar_t*)malloc(size);
		buffersize=size;
		len=0;
		buffer[0]=0;
	}
	return 1;
}

void CWString::ReCalcLen()
/* \brief Berechnet die Länge des Strings neu
 *
 * Wird von CWString::ImportBuffer aufgerufen, um die tatsächliche Länge des
 * Strings festzustellen.
 *
 * Die Funktion berechnet die Länge und den Speicherbedarf neu, indem sie den String bis zum Auftreten des ersten
 * 0-Wertes durchgeht. Sollte der Buffer unsachgemäß manipuliert worden sein, kann es dabei zu unerwünschten
 * Nebenwirkungen kommen, zum Beispiel Überschreiben anderer Speicherbereiche oder Schutzverletzungen
 * (Segmentation fault).
 */
{
	if (buffersize>0 && buffer!=NULL) {
		size_t l=buffersize/sizeof(wchar_t);
		for (size_t i=0;i<l;i++) {
			if (buffer[i]==0) {
				len=i;
				bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
				return;
			}
		}
		len=(buffersize/sizeof(wchar_t))-1;
		buffer[len]=0;
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	} else {
		len=0;
	}
}



void CWString::Print(bool attach_newline) const
/*!\brief String auf STDOUT ausgeben
 *
 * Diese Funktion gibt den aktuellen String auf STDOUT aus. Dazu ist es notwendig den String vom internen
 * Unicode-Format in das Encoding des Betriebssystems umzurechnen. Da dieses von den lokalen Einstellungen
 * des Betriebssystems und des Users abhängig ist, wird die Environment-Variable "LANG" ausgewertet.
 * Ist diese nicht gesetzt oder enthält ein unbekanntes Encoding, wird der String immer in UTF-8 ausgegeben.
 *
 * \param attach_newline Ein optionaler Parameter, der angibt, ob nach der Ausgabe ein Zeilenumbruch
 * angehangen werden soll (default=false)
 */
{
#ifndef HAVE_ICONV
	CString s=*this;
	s.Print(attach_newline);
	return;
#else
	if (buffer) {
		// Wir müssen die Daten konvertieren
		const char *enc=getenv("LANG");
		if (enc) {
			int p=instr(enc,".",0);
			if (p>0) enc=enc+p+1;
		} else enc="UTF-8";

		iconv_t ic=iconv_open(enc,ICONV_UNICODE);
		if ((iconv_t)(-1)==ic) {
			//iconvexport=NULL;
			SetError(287,"from: %s to %s",ICONV_UNICODE,enc);
			PrintError();
			return;
		}
		char *b=(char*)malloc(buffersize);
		char *outbuf=b;
		char *inbuffer=(char*)buffer;
		size_t outbytes=buffersize;
		size_t inbytes=len*sizeof(wchar_t);
		size_t res=iconv((iconv_t)ic, (ICONV_CONST char **)&inbuffer, &inbytes,
					(char**)&outbuf, &outbytes);
		if (res==(size_t)(-1)) {
			free(b);
			iconv_close(ic);
			SetError(289,"%s",strerror(errno));
			PrintError();
			return;
		}
		iconv_close(ic);
		outbuf[0]=0;
		PrintDebug("%s",b);
		free(b);
	}
	if (attach_newline) PrintDebug("\n");
#endif

}





const char *CWString::GetPtr()
/*!\brief Pointer auf die UTF-8-Version des Strings holen
 *
 * Diese Funktion wandelt den aktuellen String in eine UTF-8-Version um und liefert einen Pointer
 * darauf zurück. Der Speicherbereich des Strings wird von der Klasse selbst verwaltet und
 * darf von der Anwendung weder verändert noch freigegeben werden.
 *
 * \returns Pointer auf die UTF-8-Version des Strings
 *
 * \remarks Die Funktion ruft iconv auf, um den aktuellen Wide-String in eine UTF-8-Version
 * umzuwandeln. Dieser wird als Pointer zurückgegeben und kann von der Anwendung zum Beispiel
 * innerhalb eines printf verwendet werden.
 *
 * Für die UTF-8-Version wird ein eigener Speicherblock reserviert, der von der Klasse selbst
 * verwaltet wird. Er wird automatisch gelöscht, wenn die Klasse gelöscht wird.
 *
 * Verändert sich der Inhalt des Wide-Strings, wird die UTF-8-Version nicht automatisch
 * angepaßt. Erst bei einem erneuten Aufruf von CWString::GetPtr wird der String erneut
 * umgerechnet.
 *
 * \copydoc CWStringEncoding.dox
 */
{
	if (!len) return "";
#ifdef HAVE_ICONV
	if ((!iconvexport) && (!InitExportEncoding(extencoding))) return NULL;
#endif
	if (extbuffer!=NULL && extbuffersize<=bufferused) {
		free(extbuffer);
		extbuffer=NULL;
		extbuffersize=0;
	}
	if (!extbuffer) {
		extbuffersize=bufferused+sizeof(wchar_t);
		extbuffer=(char*)malloc(extbuffersize);
		if (!extbuffer) {
			SetError(2);
			return NULL;
		}
	}
#ifdef HAVE_ICONV
	size_t outbytes=extbuffersize;
	char *b=extbuffer;
	char *inbuffer=(char*)buffer;
	size_t inbytes=bufferused;

	size_t res=iconv((iconv_t)iconvexport, (ICONV_CONST char **)&inbuffer, &inbytes,
			(char**)&b, &outbytes);
	if (res==(size_t)(-1)) {
		SetError(289,"%s",strerror(errno));
		return NULL;
	}
	return extbuffer;
#else
#ifdef HAVE_WCSTOMBS
	size_t ret=wcstombs(extbuffer, buffer, extbuffersize);
	if (ret==(size_t)-1) {
		SetError(289,"%s",strerror(errno));
		return NULL;
	}
	extbuffer[ret]=0;
	return extbuffer;
#endif
	SetError(286);
	return NULL;
#endif
}

const void *CWString::GetBuffer() const
/*!\brief Pointer auf die Unicode-Version des Strings holen
 *
 * Diese Funktion liefert einen Pointer auf die Unicode-Rohdaten zurück.
 *
 * \returns Pointer auf die Unicode-Version des Strings
 *
 */
{
	if (!len) return NULL;
	return buffer;
}

const wchar_t *CWString::GetWPtr() const
{
	if (!len) return L"";
	return buffer;
}

void CWString::StripSlashes()
{
	if (len==0 || buffer==NULL) return;
	size_t p=0, np=0;
	wchar_t a, lastchar=0;
	while ((a=buffer[p])) {
		if (lastchar!='\\' && p>0) {
			buffer[np]=lastchar;
			np++;
		}
		lastchar=a;
		p++;
	}
	if (lastchar) {
		buffer[np]=lastchar;
		np++;
	}
	buffer[np]=0;
	if (len!=np) {
		len=np;
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

CWString CWString::GetMD5() const
{
	CWString m;
	char b[33];
	if (len>0 && buffer!=NULL) {
		MD5_CTX ctx;
		MD5Init(&ctx);
		MD5Update(&ctx,(const unsigned char *)buffer,len*sizeof(wchar_t));
		MD5End(&ctx, b);
		b[32]=0;
		m.Set(b);
	}
	return m;
}

int CWString::MD5(CWString &str)
{
	if (str.len==0 || str.buffer==NULL) {
		SetError(27);
		return 0;
	}
	char b[33];
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(const unsigned char *)str.buffer,str.len*sizeof(wchar_t));
	MD5End(&ctx, b);
	b[32]=0;
	return Set(b);
}

int CWString::MD5(const char *str, int size)
/*!\brief MD5-Summe für einen String errechnen
 *
 * Errechnet die MD5-Summe für den angegebenen String
 * \param str Pointer auf einen String. Dieser muss mit einem 0-Byte enden, sofern der Parameter \p size nicht angegeben
 * wurde
 * \param size Optionaler Parameter, der die Größe des Strings in Bytes angibt.
 * \returns Die Funktion liefert true (1) zurück, wenn die MD5-Summe erfolgreich errechnet wurde, andernfalls false (0).
 * \remarks
 * Diese Funktion nimmt keine Konvertierung nach \p wchar_t (Unicode) vor, sondern nimmt den String so wie er ist
 */
{
	if (!str) {
		SetError(194,"int CWString::MD5(const char *str)");
		return 0;
	}
	if (size<0) size=(int)strlen(str);
	char b[33];
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(const unsigned char *)str,size);
	MD5End(&ctx, b);
	b[32]=0;
	return Set(b);
}


int CWString::MD5()
/*!\brief MD5 Summe für den enthaltenen String errechnen
 *
 * Diese Funktion berechnet die MD5-Summe für den enthaltenen String und speichert
 * das Ergebnis als neuen String-Wert
 *
 * \returns
 * Liefert true (1) zurück, wenn die MD5-Summe erfolgreich berechnet wurde, sonst false (0).
 * \attention
 * Der Ursprüngliche Inhalt dieses Strings wird durch die MD5-Summe ersetzt und geht somit verloren!
 */
{
	if (len==0 || buffer==NULL) {
		SetError(27);
		return 0;
	}
	char b[33];
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(const unsigned char *)buffer,len*sizeof(wchar_t));
	MD5End(&ctx, b);
	b[32]=0;
	return Set(b);
}


void CWString::Trim()
//! \brief Schneidet Leerzeichen, Tabs Returns und Linefeeds am Anfang und Ende des Strings ab
{
	if (buffer) {
		size_t i,start,ende,s;
		if (len>0) {
			start=0; s=0;
			ende=len;
			for (i=0;i<len;i++) {
				if (buffer[i]==13||buffer[i]==10||buffer[i]==32||buffer[i]=='\t') {
					if (s==0) start=i+1;
				} else {
					s=1; ende=i;
				}
			}
			buffer[ende+1]=0;
			if (start>0)
				memmove(buffer,buffer+start,(ende-start+2)*sizeof(wchar_t));
		}
		len=wcslen(buffer);
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::LTrim()
//! \brief Schneidet Leerzeichen, Tabs Returns und Linefeeds am Anfang des Strings ab
{
	if (buffer) {
		size_t i,start,s;
		if (len>0) {
			start=0; s=0;
			for (i=0;i<len;i++) {
				if (buffer[i]==13||buffer[i]==10||buffer[i]==32||buffer[i]=='\t') {
					if (s==0) start=i+1;
				} else {
					s=1;
				}
			}
			if (start>0)
				memmove(buffer,buffer+start,(len-start+1)*sizeof(wchar_t));
		}
		len=wcslen(buffer);
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::RTrim()
//! \brief Schneidet Leerzeichen, Tabs Returns und Linefeeds am Ende des Strings ab
{
	if (buffer!=NULL && len>0) {
		size_t i,ende;
		wchar_t w;
		ende=0;
		for (i=len;i>0;i--) {
			w=buffer[i-1];
			if (w!=13 && w!=10 && w!=32 && w!='\t') {
				ende=i;
				break;
			}
		}
		buffer[ende]=0;
		len=wcslen(buffer);
		buffer[len]=0;
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::Trim(wchar_t c)
//! \brief Schneidet das angegebene Zeichen am Anfang und Ende des Strings ab
{
	if (buffer) {
		size_t i,start,ende,s;
		if (len>0) {
			start=0; s=0;
			ende=len;
			for (i=0;i<len;i++) {
				if (buffer[i]==c) {
					if (s==0) start=i+1;
				} else {
					s=1; ende=i;
				}
			}
			buffer[ende+1]=0;
			if (start>0)
				memmove(buffer,buffer+start,(ende-start+2)*sizeof(wchar_t));
		}
		len=wcslen(buffer);
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::LTrim(wchar_t c)
//! \brief Schneidet das angegebene Zeichen am Anfang des Strings ab
{
	if (buffer) {
		size_t i,start,s;
		if (len>0) {
			start=0; s=0;
			for (i=0;i<len;i++) {
				if (buffer[i]==c) {
					if (s==0) start=i+1;
				} else {
					s=1;
				}
			}
			if (start>0)
				memmove(buffer,buffer+start,(len-start+1)*sizeof(wchar_t));
		}
		len=wcslen(buffer);
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::RTrim(wchar_t c)
//! \brief Schneidet das angegebene Zeichen am Ende des Strings ab
{
	if (buffer) {
		size_t i,ende;
		if (len>0) {
			ende=len;
			for (i=0;i<len;i++) {
				if (buffer[i]==c) {
					//
				} else {
					ende=i;
				}
			}
			buffer[ende+1]=0;
		}
		len=wcslen(buffer);
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::LTrim(const char *str)
//! \brief Schneidet die definierten Zeichen am Anfang des Strings ab
{
	if (!str) return;
	if (buffer) {
		CWString cut=str;
		size_t cutl=cut.Len();
		if (cutl==0) return;
		size_t i,start,s,z;
		int match;
		if (len>0) {
			start=0; s=0;
			for (i=0;i<len;i++) {
				match=0;
				for (z=0;z<cutl;z++) {
					if (buffer[i]==cut.GetChar(z)) {
						if (s==0) start=i+1;
						match=1;
						break;
					}
				}
				if (!match) {
					s=1;
				}
			}
			if (start>0)
				memmove(buffer,buffer+start,(len-start+1)*sizeof(wchar_t));
		}
		len=wcslen(buffer);
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::RTrim(const char *str)
//! \brief Schneidet die definierten Zeichen am Ende des Strings ab
{
	CWString cut=str;
	size_t l_str=cut.Len();
	if (buffer!=NULL && len>0 && l_str>0) {
		size_t i,ende,z;
		int match;
		wchar_t w;
		ende=0;
		for (i=len;i>0;i--) {
			w=buffer[i-1];
			match=0;
			for (z=0;z<l_str;z++) {
				if (w==cut.GetChar(z)) {
					match=1;
					break;
				}
			}
			if (!match) {
				ende=i;
				break;
			}
		}
		buffer[ende]=0;
		len=wcslen(buffer);
		bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
	}
}

void CWString::Trim(const char *str)
//! \brief Schneidet bestimmte Zeichen am Anfang und Ende des Strings ab
{
	LTrim(str);
	RTrim(str);
}


void CWString::Chomp()
//! \brief Schneidet Returns und Linefeeds am Anfanng und Ende des Strings ab
{
	RTrim("\n\r");
	LTrim("\n\r");
}

void CWString::Chop(int chars)
//! \brief Schneidet Zeichen am Ende des Strings ab
{
	if (buffer) {
		if (len>0) {
			size_t c=(size_t)chars;
			if (len<c) c=len;
			len-=c;
			buffer[len]=0;
			bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
		}
	}
}

void CWString::Cut(int position)
/*! \brief Schneidet den String an einer bestimmten Stelle ab
 *
 * Der String wird an einer bestimmten Stelle einfach abgeschnitten
 * \param position Die Position, an der der String abgeschnitten wird. Bei Angabe von 0 ist der String anschließend
 * komplett leer. Ist \c position größer als die Länge des Strings, passiert nichts.
 */
{
	size_t c=(size_t)position;
	if (!len) return;
	if (c>len) return;
	buffer[c]=0;
	len=c;
	bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
}



void CWString::TrimL(int chars)
/*!Schneidet Zeichen auf der linken Seite des Strings ab
 * \param chars gibt an, wieviele Zeichen abgeschnitten werden sollen
 */
{
	if (!len) return;
	size_t c=(size_t)chars;
	if (c>len) c=len;
	memmove(buffer,buffer+chars,(len-chars)*sizeof(wchar_t));
	len-=chars;
	buffer[len]=0;
	bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
}

void CWString::TrimR(int chars)
/*!Schneidet Zeichen auf der rechten Seite des Strings ab
 * \param chars gibt an, wieviele Zeichen abgeschnitten werden sollen
 */
{
	if (!len) return;
	size_t c=(size_t)chars;
	if (c>len) c=len;
	len-=chars;
	buffer[len]=0;
	bufferused=len*sizeof(wchar_t)+sizeof(wchar_t);
}

void CWString::LCase()
/*! \brief Wandelt alle Zeichen des Strings in Kleinbuchstaben um
 *
 * Diese Funktion wandelt alle Zeichen des Strings in Kleinbuchstaben um. Die genaue Funktionsweise hängt davon ab,
 * welche Spracheinstellungen aktiv sind, genauer vom Wert "LC_CTYPE".
 *
 * \attention Unter UNIX (und möglicherweise anderen Betriebssystemen) werden die Lokalisationseinstellungen der
 * Umgebung nicht automatisch übernommen, sondern stehen standardmäßig auf "C". Dadurch werden nur US-ASCII
 * (ASCII 32 bis 127) umgewandelt. Man sollte daher nach Programmstart mit "setlocale" die gewünschte
 * Spracheinstellung vornehmen.
 *
 * \example
 * \code
 * #include <locale.h>
 * ...
 * setlocale(LC_CTYPE,"de_DE.UTF-8");
 * \endcode
 */
{
	if (!buffer) return;
	wchar_t c,wc;
	for (size_t i=0;i<len;i++) {
		wc=buffer[i];
		c=towlower(wc);
		if (c!=(wchar_t)WEOF) {
			buffer[i]=c;
		}
	}
}

void CWString::UCase()
/*! \brief Wandelt alle Zeichen des Strings in Grossbuchstaben um
 *
 * Diese Funktion wandelt alle Zeichen des Strings in Großbuchstaben um. Die genaue Funktionsweise hängt davon ab,
 * welche Spracheinstellungen aktiv sind, genauer vom Wert "LC_CTYPE".
 *
 * \attention Unter UNIX (und möglicherweise anderen Betriebssystemen) werden die Lokalisationseinstellungen der
 * Umgebung nicht automatisch übernommen, sondern stehen standardmäßig auf "C". Dadurch werden nur US-ASCII
 * (ASCII 32 bis 127) umgewandelt. Man sollte daher nach Programmstart mit "setlocale" die gewünschte
 * Spracheinstellung vornehmen.
 *
 * \example
 * \code
 * #include <locale.h>
 * ...
 * setlocale(LC_CTYPE,"de_DE.UTF-8");
 * \endcode
 */
{
	if (!buffer) return;
	wchar_t c,wc;
	for (size_t i=0;i<len;i++) {
		wc=buffer[i];
		c=towupper(wc);
		if (c!=(wchar_t)WEOF) {
			buffer[i]=c;
		}
	}
}

void CWString::UCWords()
/*!\brief Anfangsbuchstaben der Wörter groß
 *
 * \desc
 * Diese Funktion wandelt die Anfangsbuchstaben aller im String enthaltenen Wörter in
 * Großbuchstaben um.
 */
{
	if (!buffer) return;
	wchar_t c,wc;
	bool wordstart=true;
	for (size_t i=0;i<len;i++) {
		wc=buffer[i];
		if (wordstart) {
			c=towupper(wc);
			if (c!=(wchar_t)WEOF) {
				buffer[i]=c;
			}
		}
		if (wc!='\'' && (wc<48 || (wc>57 && wc<65) || (wc>90 && wc<97) || (wc>122 && wc<127)) ) {
			wordstart=true;
		} else {
			wordstart=false;
		}
	}
}


CWString CWString::Left(int len) const
// \brief erstellt aus den ersten \a len Zeichen einen neuen String
{
	CWString ms;

	//Proceed only if there's a contained string
	if(buffer != NULL && len > 0) {
		//Adjust the len if the contained string is shorter
		if((size_t)len > this->len) len = this->len;
		ms.Set(buffer,len);
	}
	return ms;
}

CWString CWString::Right(int len) const
// \brief erstellt aus den letzten \a len Zeichen einen neuen String
{
	CWString ms;
	//proceed only if there's a contained string
	if(buffer != NULL && len > 0) {
		//Adjust the len if the contained string is shorter
		if((size_t)len > this->len)
			len = this->len;
		ms.Set(buffer+this->len-len,len);
	}
	return ms;
}


CWString CWString::Mid(int start, int len) const
// \brief erstellt einen neuen String ab der Position \start mit \a len Zeichen
{
	CWString ms;
	//Length of the contained string
	size_t slen = this->len;
	size_t l=(size_t)len;
	size_t s=(size_t)start;
	if (len<0) l=this->len;
	/* Proceed only if there's a contained string and the
	   parameters have reasonable values */
	if(buffer != NULL && l > 0 && s < slen) {
		//Check the length to return
		if(slen - s < l)
			l = slen - s;
		ms.Set((const wchar_t*)(buffer+s),(int)l);
	}
	return ms;
}

CWString CWString::SubStr(int start, int len) const
// \brief erstellt einen neuen String ab der Position \start mit \a len Zeichen
{
	CWString ms;
	//Length of the contained string
	size_t slen = this->len;
	size_t l=(size_t)len;
	size_t s=(size_t)start;
	if (len<0) l=this->len;
	/* Proceed only if there's a contained string and the
	   parameters have reasonable values */
	if(buffer != NULL && l > 0 && s < slen) {
		//Check the length to return
		if(slen - s < l)
			l = slen - s;
		ms.Set((const wchar_t*)(buffer+s),(int)l);
	}
	return ms;
}


void CWString::Shl(wchar_t c, int size)
/*! \brief Schiebt den String nach links
 *
 * Der String wird um die mit \c size angegebenen Zeichen nach links verschoben und rechts mit dem durch \c c angegebenen
 * Zeichen aufgefüllt.
 * \param c Das Zeichen, mit dem der String auf der rechten Seite aufgefüllt werden soll. Wird der Wert 0 verwendet, findet keine
 * Auffüllung statt, d.h. der String verkürzt sich einfach.
 * \param size Die Anzahl Zeichen, um die der String nach links verschoben werden soll. Ist \c size größer als die Länge
 * des Strings, wird der String komplett geleert und ist anschließend so groß wie size, sofern c>0 war.
 */
{
	if (!len) return;
	if (!size) return;
	if ((ppluint32)size>len) size=len;
	CWString t=Mid(size);
	if (c) {
		CWString a;
		a.Repeat(c,size);
		t+=a;
	}
	Set(&t);
}


void CWString::Shr(wchar_t c, int size)
/*! \brief Schiebt den String nach rechts
 *
 * Der String wird um die mit \c size angegebenen Zeichen nach rechts verschoben und links mit dem durch \c c angegebenen
 * Zeichen aufgefüllt.
 * \param c Das Zeichen, mit dem der String auf der linken Seite aufgefüllt werden soll. \c c muß größer 0 sein.
 * \param size Die Anzahl Zeichen, um die der String nach rechts verschoben werden soll. Ist \c size größer als die Länge
 * des Strings, wird der String komplett geleert und ist anschließend so groß wie size.
 */
{
	if (!len) return;
	if (!size) return;
	if (!c) return;
	if ((ppluint32)size>len) size=len;
	CWString t;
	t.Repeat(c,size);
	t+=buffer;
	t.Cut(len);
	Set(&t);
}

CWString& CWString::Repeat(int num)
{
	if ((!buffer)||len==0) return *this;
	wchar_t *buf=(wchar_t*)malloc((len*num+1)*sizeof(wchar_t));
	if (!buf) return *this;
	wchar_t *tmp=buf;
	for (int i=0;i<num;i++) {
		memcpy(tmp,buffer,len*sizeof(wchar_t));
		tmp+=len;
	}
	tmp[0]=0;
	Set(buf);
	free(buf);
	return *this;
}

CWString& CWString::Repeat(const char *str, int num)
{
	if (!str) {
		Clear();
		return *this;
	}
	CWString rep=str;
	return Repeat(&rep,num);
}

CWString& CWString::Repeat(const wchar_t *str, int num)
{
	if (!str) {
		Clear();
		return *this;
	}
	CWString rep=str;
	return Repeat(&rep,num);
}

CWString& CWString::Repeat(const wchar_t ascii, int num)
/*! \brief Füllt den String mit einem Zeichen
 *
 * Der String wird mit einem gewünschten Zeichen gefüllt
 * \param ascii Der ASCII-Code des Zeichens, mit dem der String gefüllt werden soll
 * \param num Die Länge des gewünschten Strings
 * \returns liefert einen neuen String zurück
 */
{
	if (!ascii) {
		Clear();
		return *this;
	}
	if (!num) {
		Clear();
		return *this;
	}
	wchar_t *buf=(wchar_t*)malloc((num+1)*sizeof(wchar_t));
	if (!buf) return *this;
	for (int i=0;i<num;i++) buf[i]=ascii;
	Set(buf,num);
	free(buf);
	return *this;
}

CWString& CWString::Repeat(const CWString& str, int num)
{
	if (str.len==0 || num==0 || str.buffer==NULL) {
		Clear();
		return *this;
	}
	wchar_t *rep=str.buffer;
	size_t l=str.len;
	if (l==0 || num==0 || rep==NULL) {
		Clear();
		return *this;
	}
	wchar_t *buf=(wchar_t*)malloc((l*num+1)*sizeof(wchar_t));
	if (!buf) return *this;
	wchar_t *tmp=buf;
	for (int i=0;i<num;i++) {
		memcpy(tmp,rep,l*sizeof(wchar_t));
		tmp+=l;
	}
	tmp[0]=0;
	Set(buf);
	free(buf);
	return *this;
}

CWString& CWString::Repeat(const CString& str, int num)
{
	CWString s=str;
	return Repeat(s,num);
}


int CWString::Find(const char* str, int pos) const
/*! \brief Sucht nach einem String
 *
 * Find sucht nach dem Suchstring \a str ab der gewünschten Position \a pos.
 * \returns Liefert die Position innerhalb des Strings, an der der Suchstring gefunden wurde
 * oder -1 wenn er nicht gefunden wurde.
 */
{
	if (!len) return -1;
	//Position to return
	int p = -1;
	//Length of the string to search for
	CWString search=str;
	size_t len = this->len, lstr = search.len;
	//Current position to search from and position of found string
	wchar_t *found = NULL, *tmp = NULL;

	//Proceed only if there's a contained and a given string
	if(buffer != NULL && lstr > 0) {
		//Search forward
		if(pos >= 0) {
			//Search first occurence, starting at the given position...
			found = wcsstr(buffer + pos, search.buffer);
			//...and calculate the position to return if str was found
			if (found != NULL) {
				p = found - buffer;
			}
		} else {
			//Search backward
			//Start counting from behind at 0, not -1
			pos++;

			/* Beginning at the start of the contained string, start searching for
			   every occurence of the str and make it the position last found as long
			   as the found string doesn't exceed the defined end of the search */
			while((found = wcsstr((tmp == NULL ? buffer : tmp + 1), search.buffer)) != NULL && found - buffer + lstr <= len + pos)
	        tmp = found;

			//Calculate the position to return if str was found
			if(tmp != NULL) {
				p = tmp - buffer;
			}
		}
	}
	return p;
}

int CWString::Find(const CWString &search, int pos) const
/*! \brief Sucht nach einem String
 *
 * Find sucht nach dem Suchstring \a str ab der gewünschten Position \a pos.
 * \returns Liefert die Position innerhalb des Strings, an der der Suchstring gefunden wurde
 * oder -1 wenn er nicht gefunden wurde.
 */
{
	if (!len) return -1;
	//Position to return
	int p = -1;
	//Length of the string to search for
	size_t len = this->len, lstr = search.len;
	//Current position to search from and position of found string
	wchar_t *found = NULL, *tmp = NULL;

	//Proceed only if there's a contained and a given string
	if(buffer != NULL && lstr > 0) {
		//Search forward
		if(pos >= 0) {
			//Search first occurence, starting at the given position...
			found = wcsstr(buffer + pos, search.buffer);
			//...and calculate the position to return if str was found
			if (found != NULL) {
				p = found - buffer;
			}
		} else {
			//Search backward
			//Start counting from behind at 0, not -1
			pos++;

			/* Beginning at the start of the contained string, start searching for
			   every occurence of the str and make it the position last found as long
			   as the found string doesn't exceed the defined end of the search */
			while((found = wcsstr((tmp == NULL ? buffer : tmp + 1), search.buffer)) != NULL && found - buffer + lstr <= len + pos)
	        tmp = found;

			//Calculate the position to return if str was found
			if(tmp != NULL) {
				p = tmp - buffer;
			}
		}
	}
	return p;
}


CWString& CWString::Replace(const char* str, const char* byStr)
//! \brief Ersetzt einen Teilstring durch einen anderen
{
	ppluint32 start = 0, slen = strlen(str);
	int end;
	// collect the result
	CWString ms;
	//Proceed only if there's a string to search for
	if(len > 0) {
		//Do while str is found in the contained string
		while((end = Find(str, start)) >= 0) {
			//The result is built from the parts that don't match str and the replacement string
			ms += Mid(start, end - start);
			ms += byStr;
			//New start for search is behind the replaced part
			start = end + slen;
		}
		//Add the remaining part of the contained string to the result
		ms += Mid(start);
		//The result is assigned to this mstring
		Set(ms);
	}
	return *this;
}

CWString& CWString::Replace(const CWString &str, const CWString &byStr)
//! \brief Ersetzt einen Teilstring durch einen anderen
{
	ppluint32 start = 0, slen = str.Len();
	int end;
	// collect the result
	CWString ms;
	//Proceed only if there's a string to search for
	if(len > 0) {
		//Do while str is found in the contained string
		while((end = Find(str, start)) >= 0) {
			//The result is built from the parts that don't match str and the replacement string
			ms += Mid(start, end - start);
			ms += byStr;
			//New start for search is behind the replaced part
			start = end + slen;
		}
		//Add the remaining part of the contained string to the result
		ms += Mid(start);
		//The result is assigned to this mstring
		Set(ms);
	}
	return *this;
}

CWString& CWString::ReplaceLetterList(const CWString &letters, wchar_t replacement)
{
	for (size_t i=0;i<len;i++) {
		for (size_t o=0;o<letters.len;o++) {
			if (buffer[i]==letters.buffer[o]) buffer[i]=replacement;
		}
	}
	return *this;
}

int CWString::StrCmp(const char *str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	CWString s=str;
	return StrCmp(s.buffer,size);
}

int CWString::StrCmp(const wchar_t *str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	const wchar_t *mystr=buffer;
	if (!mystr) mystr=L"";
	if (!str) str=L"";
	if (size) return wcsncmp(mystr,str,(size_t)size);
	return wcscmp(mystr,str);
}

int CWString::StrCmp(const CWString &str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	const wchar_t *mystr=buffer;
	const wchar_t *otherstr=str.buffer;
	if (len==0) mystr=L"";
	if (str.len==0) otherstr=L"";
	if (size!=0) return wcsncmp(mystr,otherstr,size);
	return wcscmp(mystr,otherstr);
}

int CWString::StrCmp(const CString &str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	CWString s=str;
	return StrCmp(s.buffer,size);
}

int CWString::StrCaseCmp(const char *str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch, wobei Gross-/Kleinschreibung ignoriert wird
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	CWString s=str;
	return StrCaseCmp(s.buffer,size);
}

int CWString::StrCaseCmp(const wchar_t *str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch, wobei Gross-/Kleinschreibung ignoriert wird
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	const wchar_t *mystr=buffer;
	if (!mystr) mystr=L"";
	if (!str) str=L"";
#ifdef HAVE_WCSCASECMP
	if (size) return wcsncasecmp(mystr,str,(size_t)size);
	return wcscasecmp(mystr,str);
#elif WIN32
	if (size) return wcsnicmp(mystr,str,(size_t)size);
	return wcsicmp(mystr,str);
#else
	CWString b=mystr;
	CWString s=str;
	s.LCase();
	b.LCase();
	if (size) return wcsncmp(b.buffer,s.buffer,(size_t)size);
	return wcscmp(b.buffer,s.buffer);
#endif
}


int CWString::StrCaseCmp(const CWString &str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch, wobei Gross-/Kleinschreibung ignoriert wird
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	return StrCaseCmp(str.buffer,size);
}

int CWString::StrCaseCmp(const CString &str, int size) const
/*!\brief Führt einen Vergleich mit einem anderen String durch, wobei Gross-/Kleinschreibung ignoriert wird
 * \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
{
	CWString s=str;
	return StrCaseCmp(s.buffer,size);
}


int CWString::Instr(const char *str, int pos) const
{
	if (!str) return -1;
	CWString s=str;
	return Instr(&s,pos);
}

int CWString::Instr(const wchar_t *str, int pos) const
{
	if (!buffer) return -1;
	if (!str) return -1;
	return InWstr(buffer,str,pos);
}

int CWString::Instr(const CWString &str, int pos) const
{
	return InWstr(buffer,str.buffer,pos);
}

int CWString::Instr(const CString &str, int pos) const
{
	CWString s=str;
	return Instr(&s,pos);
}


int CWString::ToInt() const
{
	if (!len) return 0;
	return wcstol(buffer,NULL,0);
}

pplint64 CWString::ToInt64() const
{
	if (!len) return 0;
#ifdef HAVE_WCSTOLL
	return (pplint64) wcstoll(buffer,NULL,0);
#elif WIN32
	return (pplint64) _wcstoi64(buffer,NULL,0);
#else
	return (pplint64) pplwcstoll(buffer,NULL,0);
#endif
}

ppluint32 CWString::ToUInt() const
{
	if (!len) return 0;
	return wcstoul(buffer,NULL,0);
}

ppluint64 CWString::ToUInt64() const
{
	if (!len) return 0;
#ifdef HAVE_WCSTOULL
	return (ppluint64) wcstoull(buffer,NULL,0);
#elif WIN32
	return (ppluint64) _wcstoi64(buffer,NULL,0);
#else
	return (ppluint64) pplwcstoll(buffer,NULL,0);
#endif
}

int CWString::IsTrue() const
/*!\brief Prüft, ob der String "wahr" ist
 *
 * Diese Funktion überprüft den aktuellen String, ob er "wahr" ist. Dies ist der Fall,
 * wenn eine der folgenden Bedingungen erfüllt ist:
 * - Der String enthält eine Ziffer ungleich 0
 * - Der String enthält das Wort "true" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "wahr" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "yes" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "ja" (Gross- oder Kleingeschrieben)
 *
 * \returns Liefert true (1) zurück, wenn der String "wahr" ist, sonst false (0). Ein Fehlercode wird nicht gesetzt
 * \see CWString::IsFalse()
 */
{
	if (!len) return 0;
	if (wcstol(buffer,NULL,0)!=0) return 1;
	if (StrCaseCmp("true")==0) return 1;
	if (StrCaseCmp("wahr")==0) return 1;
	if (StrCaseCmp("ja")==0) return 1;
	if (StrCaseCmp("yes")==0) return 1;
	return 0;
}

int CWString::IsFalse() const
/*!\brief Prüft, ob der String "unwahr" ist
 *
 * Diese Funktion überprüft den aktuellen String, ob er "unwahr" ist. Dies ist der Fall,
 * wenn eine der folgenden Bedingungen erfüllt ist:
 * - Der String zeigt auf NULL
 * - Die Länge des Strings ist 0
 * - Der String enthält die Ziffer 0
 * - Der String enthält nicht das Wort "true", "wahr", "yes" oder "ja" (Gross-/Kleinschreibung egal)
 *
 * \returns Liefert true (1) zurück, wenn der String "unwahr" ist, sonst false (0). Ein Fehlercode wird nicht gesetzt
 * \see CWString::IsTrue()
 */
{
	if (IsTrue()) return 0;
	return 1;
}

bool CWString::ToBool() const
{
	if (!len) return false;
	if (IsTrue()) return true;
	return false;
}

long CWString::ToLong() const
{
	if (!len) return 0;
	return wcstol(buffer,NULL,0);
}

long long CWString::ToLongLong() const
{
	if (!len) return 0;
#ifdef HAVE_WCSTOLL
	return (long long) wcstoll(buffer,NULL,0);
#elif WIN32
	return (long long) _wcstoi64(buffer,NULL,0);
#else
	return (long long) pplwcstoll(buffer,NULL,0);
#endif
}

float CWString::ToFloat() const
{
	if (!len) return 0;
#ifdef WIN32
	return (float)wcstod(buffer,NULL);
#elif HAVE_WCSTOF
	return (float)wcstof(buffer,NULL);
#elif HAVE_WCSTOD
	return (float)wcstod(buffer,NULL);
#else
	#error *** Keine Funktion, um einen Wide-String in einen float zu konvertieren!
	return 0;
#endif
}

double CWString::ToDouble() const
{
	if (!len) return 0;
#ifdef WIN32
	return wcstod(buffer,NULL);
#elif HAVE_WCSTOD
	return wcstod(buffer,NULL);
#else
	#error *** Keine Funktion, um einen Wide-String in einen double zu konvertieren!
	return 0;
#endif
}

CString CWString::ToCString() const
{
	CString s=buffer;
	return s;
}

CBinary CWString::ToCBinary() const
{
	CBinary s;
	s.Copy(*this);
	return s;
}

/*******************************************************************************************
 * Regular Expressions
 *******************************************************************************************/
/** @name Regular Expressions
 */
//@{

void CWString::PregEscape()
/*! \brief Fügt dem String Escape-Zeichen zu, zur Verwendung in einem Match
 *
 * Der Befehl scannt den String nach Zeichen mit besonderer Bedeutung in einer Perl-Regular-Expression und
 * escaped sie mit einem Slash. Das Ergebnis kann dann in einer Regular Expression verwendet werden.
 *
 * Folgende Zeichen werden escaped: - + \ * /
 */
{
	if (!len) return;
	CWString t;
	CWString compare="-+\\*/";
	wchar_t ascii[2];
	ascii[1]=0;
	for (size_t i=0;i<len;i++) {
		ascii[0]=buffer[i];
		if (compare.Instr(ascii,0)>=0) t+="\\";
		t+=ascii;
	}
	if (StrCmp(&t)!=0) Set(&t);
}

int CWString::PregMatch(const char *expr, CArray *res)
/*! \brief Der String wird anhand einer Regular Expression durchsucht
 *
 * Durchsucht den String anhand einer Perl-Expression
\param[in] expr Ist eine Perl-kompatible Regular Expression im UTF-8-Format, die mit Slash (/) beginnt und
endet. Optional können nach dem schließenden Slash folgende Optionen angegeben werden:

\copydoc pregexpr.dox

\param[out] res ist ein Optionales Array, in dem die gefundenen Matches gespeichert werden. Wird
res nicht angegeben, werden die Ergebnisse intern gespeichert und können mit der Funktion
GetMatch ausgelesen werden. Die Ergebnisse werden im Array im UTF-8-Format abgelegt.
\return Liefert \c true(1) zurück, wenn ein Match gefunden wurde, ansonsten \c false(0)
\remarks
Der String wird intern zuerst nach UTF-8 kodiert, bevor die pcre-Funktionen aufgerufen werden.
\note

\copydoc pcrenote.dox
*/
{
	#ifndef HAVE_PCRE
		SetError(276);
		return 0;
	#else
	if (!res) {
		if (!matches) matches=new CArray;
		res=matches;
	}
	res->Clear();
	if (!expr) {
		SetError(194,"int CWString::PregMatch(==> const char *expr <==, CArray *res)");
		return 0;
	}
	if (!buffer) {
		SetError(27);
		return 0;
	}
	if (!len) {
		SetError(27);
		return 0;
	}
	const char *utf8=GetPtr();		// String nach UTF8 kodieren
	if (!utf8) return 0;
	size_t ll=strlen(utf8);

	char *r=strdup(expr+1);
	int flags=0;
	// letzten Slash in regex finden
	char *options=strrchr(r,'/');
	if (options) {
		options[0]=0;
		options++;
		if (strchr(options,'i')) flags|=PCRE_CASELESS;
		if (strchr(options,'m')) flags|=PCRE_MULTILINE;
		if (strchr(options,'x')) flags|=PCRE_EXTENDED;
		if (strchr(options,'s')) flags|=PCRE_DOTALL;
		if (strchr(options,'8')) flags|=PCRE_UTF8;
		if (strchr(options,'a')) flags|=PCRE_ANCHORED;
		if (strchr(options,'u')) flags|=PCRE_UNGREEDY;
	}
	const char *perr;
	size_t maxmatches=16;
	int re,erroffset;
	int ovectorsize=(maxmatches+1)*2;
	int *ovector=(int*)malloc(ovectorsize*sizeof(int));
	int perrorcode;
	pcre *reg;
	//printf ("r=%s, flags=%i\n",r,flags);
	CWString__PregMatch_Restart:
	reg=pcre_compile2(r,flags,&perrorcode,&perr, &erroffset, NULL);
	if (!reg) {
		free(r);
		free(ovector);
		return 0;
	}
	memset(ovector,0,ovectorsize*sizeof(int));
	if ((re=pcre_exec(reg, NULL, (char*) utf8,ll,0, 0, ovector, ovectorsize))>=0) {
		if (re>0) maxmatches=re;
		else maxmatches=maxmatches*2/3;
		for (size_t i=0;i<maxmatches;i++) {
			const char *tmp=NULL;
			pcre_get_substring(utf8,ovector,30,i,(const char**)&tmp);
			if (tmp) {
				//printf("tmp[%i]=%s\n",i,tmp);
				res->Set(i,tmp);
				pcre_free_substring(tmp);
			}
		}
		pcre_free(reg);
		free(ovector);
		free(r);
		return 1;
	} else if ((flags&PCRE_UTF8)==PCRE_UTF8 && (re==PCRE_ERROR_BADUTF8 || re==PCRE_ERROR_BADUTF8_OFFSET)) {
		// Wir haben ungültiges UTF_8
		//printf ("ungültiges UTF-8");
		// Vielleicht matched es ohne UTF-8-Flag
		flags-=PCRE_UTF8;
		pcre_free(reg);
		goto CWString__PregMatch_Restart;
	}
	pcre_free(reg);
	free(ovector);
	free(r);
	return 0;
#endif
}

/*!
 * Liefert mit PregMatch gefundene Ergebnisse zurück
 *
 * \param index ist ein Index auf die Ergebnisse. 0 liefert immer den kompletten String zurück,
 * 1 den ersten Match, 2 den zweiten usw.
 * \return Die Funktion liefert einen const char * zurück, sofern der Match vorhanden
 * ist, ansonsten NULL.
 * \note \copydoc pcrenote.dox
 */
const char *CWString::GetMatch(int index) const
{
	if (!matches) return NULL;
	return matches->Get(index);
}

int CWString::PregReplace(const char *expr, CWString &replace, int maxreplace)
/*! \brief Es wird ein Suchen und Ersetzen anhand einer Regular Expression durchgeführt
 *
\param[in] expr is a perl compatible regular expression, starting and ending with slash (/).

\copydoc pregexpr.dox

\param[in] replace ist ein Pointer auf eine Stringklasse, die den Text enthält, der anstelle
des Matches eingesetzt wird. Werden in der Expression Klammern zum capturen verwendet,
können diese Werte mit \c $1 bis \c $9 im Replace-String verwendet werden.
\param[in] maxreplace ist optional. Wenn vorhanden, werden nur soviele Matches ersetzt, wi
mit maxreplace angegeben. Wurden zum Beispiel 10 Matches gefunden, aber maxreplace wurde
mit 5 angegeben, werden nur die ersten 5 Matches ersetzt.
\return Liefert \c true(1) zurück, wenn ein Match gefunden wurde, ansonsten \c false(0)
\note

\copydoc pcrenote.dox
*/
{
	if (!replace) {
		SetError(194,"PregReplace(const char *expr, ==> CString *replace <==, int maxreplac)");
		return 0;
	}
	return PregReplace(expr,replace.GetPtr(),maxreplace);
}

int CWString::PregReplace(const char *expr, CString &replace, int maxreplace)
/*! \brief Es wird ein Suchen und Ersetzen anhand einer Regular Expression durchgeführt
 *
\param[in] expr is a perl compatible regular expression, starting and ending with slash (/).

\copydoc pregexpr.dox

\param[in] replace ist ein Pointer auf eine Stringklasse, die den Text enthält, der anstelle
des Matches eingesetzt wird. Werden in der Expression Klammern zum capturen verwendet,
können diese Werte mit \c $1 bis \c $9 im Replace-String verwendet werden.
\param[in] maxreplace ist optional. Wenn vorhanden, werden nur soviele Matches ersetzt, wi
mit maxreplace angegeben. Wurden zum Beispiel 10 Matches gefunden, aber maxreplace wurde
mit 5 angegeben, werden nur die ersten 5 Matches ersetzt.
\return Liefert \c true(1) zurück, wenn ein Match gefunden wurde, ansonsten \c false(0)
\note

\copydoc pcrenote.dox
*/
{
	if (!replace) {
		SetError(194,"PregReplace(const char *expr, ==> CString *replace <==, int maxreplac)");
		return 0;
	}
	return PregReplace(expr,replace.GetPtr(),maxreplace);
}

int CWString::PregReplace(const char *expr, const char *replace, int maxreplace)
/*! \brief Es wird ein Suchen und Ersetzen anhand einer Regular Expression durchgeführt
 *
\param expr is a perl compatible regular expression, starting and ending with slash (/).

\copydoc pregexpr.dox

\param replace ist ein Pointer auf eine Stringklasse, die den Text enthält, der anstelle
des Matches eingesetzt wird. Werden in der Expression Klammern zum capturen verwendet,
können diese Werte mit \c $1 bis \c $9 im Replace-String verwendet werden.
\param maxreplace ist optional. Wenn vorhanden, werden nur soviele Matches ersetzt, wi
mit maxreplace angegeben. Wurden zum Beispiel 10 Matches gefunden, aber maxreplace wurde
mit 5 angegeben, werden nur die ersten 5 Matches ersetzt.
\return Liefert \c true(1) zurück, wenn ein Match gefunden wurde, ansonsten \c false(0)
\note

\copydoc pcrenote.dox
 */
{
#ifndef HAVE_PCRE
	SetError(276);
	return 0;
#else
	if (!expr) {
		SetError(194,"int CWString::PregMatch(==> const char *expr <==, CArray *res)");
		return 0;
	}
	if (!buffer) {
		SetError(27);
		return 0;
	}
	if (!len) {
		SetError(27);
		return 0;
	}
	const char *utf8=GetPtr();		// String nach UTF8 kodieren
	if (!utf8) return 0;
	//size_t ll=strlen(utf8);

	if (!matches) matches=new CArray;
	CString pattern;
	int ret=0;
	char *r=strdup(expr+1);
	int flags=PCRE_UTF8;
	char *tmp;
	// letzten Slash in regex finden
	char *options=strrchr(r,'/');
	if (options) {
		options[0]=0;
		options++;
		if (strchr(options,'i')) flags|=PCRE_CASELESS;
		if (strchr(options,'m')) flags|=PCRE_MULTILINE;
		if (strchr(options,'x')) flags|=PCRE_EXTENDED;
		if (strchr(options,'s')) flags|=PCRE_DOTALL;
		if (strchr(options,'a')) flags|=PCRE_ANCHORED;
		if (strchr(options,'u')) flags|=PCRE_UNGREEDY;
	}

	pattern+=r;
	const char *perr;
	int re,erroffset, ovector[30];
	int perrorcode;
	pcre *reg;
	//printf ("r=%s, flags=%i\n",r,flags);
	reg=pcre_compile2(r,flags,&perrorcode,&perr, &erroffset, NULL);
	CString neu;
	CString Replace;
	char rep[5];
	if (reg) {
		CString rest=utf8;
		while (1) {		// Endlosschleife, bis nichts mehr matched
			memset(ovector,0,30*sizeof(int));
			if ((re=pcre_exec(reg, NULL, (const char*) rest,rest.Len(),0, 0, ovector, 30))>=0) {
				ret++;
				Replace=replace;
				for (int i=0;i<14;i++) {
					tmp=NULL;
					pcre_get_substring(utf8,ovector,30,i,(const char**)&tmp);
					if (tmp) {
						//printf("tmp[%i]=%s\n",i,tmp);
						sprintf(rep,"$%i",i);
						Replace.Replace(rep,tmp);
						//matches->Set(i,tmp);
						pcre_free_substring(tmp);
					}
				}
				// Erstes Byte steht in ovector[0], letztes in ovecor[1]
				neu.Concat(rest,ovector[0]);
				neu+=Replace;
				rest.TrimL(ovector[1]);		// rest abschneiden
				//printf ("Match %i\nNeu: %s\nRest (%i): %s\n",ret,(char*)neu,rest.Len(),(char*)rest);
				if (maxreplace>0 && ret>=maxreplace) {
					neu+=rest;
					break;
				}
			} else {
				// Kein Match, Schleife beenden
				neu+=rest;
				break;
			}
		}
		free(reg);
	}
	free(r);
	if (ret) Set(&neu);
	return ret;
#endif
}

//@}




/*******************************************************************************************
 * Operatoren
 *******************************************************************************************/

/** @name Operatoren
 */
//@{

CWString::operator const char *()
/*!\brief Liefert einen const char-Pointer auf die UTF-8-Version des String zurück
 *
 * \copydoc GetPtr
 */
{
	return GetPtr();
}


CWString::operator const wchar_t *() const
/*!\brief Liefert einen const wchar_t-Pointer auf die Unicode-Version des String zurück
 *
 */
{
	if (!len) return NULL;
	return (const wchar_t *)buffer;
}

CWString& CWString::operator=(const char* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}

CWString& CWString::operator=(const wchar_t* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}

CWString& CWString::operator=(char* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}

CWString& CWString::operator=(const CWString* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}

CWString& CWString::operator=(const CWString& str)
{
	Set(str.buffer);
	return *this;
}

CWString& CWString::operator=(const CString* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}

CWString& CWString::operator=(const CString& str)
{
	Set(str);
	return *this;
}

CWString& CWString::operator=(const wchar_t c)
/*!
 * \brief Unicode-Zeichen zuweisen
 *
 * Der String wird mit dem angegebenen Zeichen \c belegt.
 * \param c ist der Unicode des gewünschten Zeichens
 */
{
	wchar_t a[2];
	a[0]=c;
	a[1]=0;
	Set(a);
	return *this;
}


CWString& CWString::operator+=(const char* str)
{
	if (!str) return *this;
	Concat(str);
	return *this;
}

CWString& CWString::operator+=(const wchar_t* str)
{
	if (!str) return *this;
	Concat(str);
	return *this;
}

CWString& CWString::operator+=(const wchar_t c)
{
	if (!c) return *this;
	ConcatChar(c);
	return *this;
}

CWString& CWString::operator+=(const CWString& str)
{
	Concat(str.buffer);
	return *this;
}

CWString& CWString::operator+=(const CString& str)
{
	Concat(str);
	return *this;
}


bool CWString::operator==(const CWString &str) const
{
	if (StrCmp(str)==0) return true;
	return false;
}

bool CWString::operator!=(const CWString &str) const
{
	if (StrCmp(str)==0) return false;
	return true;
}

bool CWString::operator<(const CWString &str) const
{
	if (StrCmp(str)<0) return true;
	return false;
}

bool CWString::operator<=(const CWString &str) const
{
	if (StrCmp(str)<=0) return true;
	return false;
}

bool CWString::operator>(const CWString &str) const
{
	if (StrCmp(str)>0) return true;
	return false;
}

bool CWString::operator>=(const CWString &str) const
{
	if (StrCmp(str)>=0) return true;
	return false;
}

CWString::operator int() const
//! \brief Liefert den Integer-Wert des Strings zurück
{
	return ToInt();
}

CWString::operator unsigned int() const
//! \brief Liefert den Integer-Wert des Strings zurück
{
	return ToUInt();
}

wchar_t CWString::operator[](int pos)
/*!\brief Zeichen an einer bestimmten Stelle auslesen
 *
 * Mit diesem Operator kann ein einzelnes Zeichen an einer bestimmten Stelle des Strings ausgelesen werden.
 *
 * \param pos Position innerhalb des String beginnend bei 0. Bei negativen Werten wird die Position vom Ende des Strings
 * berechnet. Ein Wert von \c -1 würde daher das letzte Zeichen des Strings zurückliefern.
 *
 * \returns Ist das Zeichen an der Stelle \p pos vorhanden, wird dessen Unicode-Wert zurückgeliefert. Ist das Zeichen
 * nicht im String enthalten, wird 0 zurückgegeben.
 */
{
	if (!len) return 0;
	if (!buffer) return 0;
	if (pos>=(int)len) return 0;
	if (pos<0) pos=(int)len+pos;
	if (pos<0) return 0;
	return buffer[pos];
}

//@}


wchar_t CWString::GetChar(int pos) const
/*!\brief Zeichen an einer bestimmten Stelle auslesen
 *
 * Mit dieser Funktion kann ein einzelnes Zeichen an einer bestimmten Stelle des Strings ausgelesen werden.
 *
 * \param pos Position innerhalb des String beginnend bei 0. Bei negativen Werten wird die Position vom Ende des Strings
 * berechnet. Ein Wert von \c -1 würde daher das letzte Zeichen des Strings zurückliefern.
 *
 * \returns Ist das Zeichen an der Stelle \p pos vorhanden, wird dessen Unicode-Wert zurückgeliefert. Ist das Zeichen
 * nicht im String enthalten, wird 0 zurückgegeben.
 */
{
	if (!len) return 0;
	if (pos>=(int)len) return 0;
	if (pos<0) pos=(int)len+pos;
	if (pos<0) return 0;
	return buffer[pos];
}



int CWString::SetChar(wchar_t c)
/*!\brief String mit einem einzelnen Wide-Character belegen
 *
 * Mit dieser Funktion wird der String mit einem einzelnen Wide-Character im Unicode-Format belegt.
 *
 * \param[in] c Unicode des gewünschten Zeichens
 * \returns Die Funktion liefert true (1) zurück, wenn der String mit dem Zeichen belegt werden konnte.
 */
{
	if (!c) {
		SetError(437,"%i",c);
		return 0;
	}
	wchar_t b[2];
	b[0]=c;
	b[1]=0;
	return Set(b,1);
}

int CWString::SetChar(wchar_t c, int pos)
/*!\brief Einzelnes Zeichen an einer bestimmten Position überschreiben
 *
 * Mit dieser Funktion kann ein einzelnes Zeichen an einer bestimmten Position des Strings überschrieben werden.
 *
 * \param[in] c Unicode des gewünschten Zeichens
 * \param[in] pos Position innerhalb des Strings, wobei 0 das erste Zeichen bedeutet. Ist der Wert negativ, wird die
 * Position vom Ende des Strings berechnet. -1 wäre dabei das letzte Zeichen des Strings. Eine besondere Behandlung
 * wird durchgeführt, wenn \p pos mit der Länge des Strings übereinstimmt. In diesem Fall wird die Funktion
 * CWString::AddChar aufgerufen und das Zeichen am Ende angehangen.
 * \returns Die Funktion liefert true (1) zurück, wenn der String mit dem Zeichen belegt werden konnte. Ist pos < 0
 * oder größer wie die aktuelle Stringinhalt, wird 0 zurückgegeben und der Fehlercode 436 gesetzt.
 */
{
	if (!c) {
		SetError(437,"%i",c);
		return 0;
	}
	if (pos<0) pos=(int)len+pos;
	if (pos<0) {
		SetError(438);
		return 0;
	}
	if ((size_t)pos<len) {
		buffer[pos]=c;
		return 1;
	}
	if ((size_t)pos==len) return AddChar(c);
	SetError(436,"Stringlänge: %i, gewünschte Position: %i",len,pos);
	return 0;
}

int CWString::AddChar(wchar_t c)
/*!\brief Einzelnes Zeichen anfügen
 *
 * Mit dieser Funktion wird ein einzelnes Zeichen an den String angefügt.
 *
 * \param[in] c Unicode des gewünschten Zeichens
 * \returns Die Funktion liefert true (1) zurück, wenn der String mit dem Zeichen belegt werden konnte. Bei einem Fehler wird
 * 0 zurückgegeben.
 * \see
 * Die Funktion ist identisch mit CWString::ConcatChar und dem Operator CWString::operator+=(const wchar_t c)
 */
{
	if (!c) {
		SetError(437,"%i",c);
		return 0;
	}
	wchar_t b[2];
	b[0]=c;
	b[1]=0;
	return Concat(b,1);
}

int CWString::ConcatChar(wchar_t c)
/*!\brief Einzelnes Zeichen anfügen
 *
 * Mit dieser Funktion wird ein einzelnes Zeichen an den String angefügt.
 *
 * \param[in] c Unicode des gewünschten Zeichens
 * \returns Die Funktion liefert true (1) zurück, wenn der String mit dem Zeichen belegt werden konnte. Bei einem Fehler wird
 * 0 zurückgegeben.
 * \see
 * Die Funktion ist identisch mit CWString::AddChar und dem Operator CWString::operator+=(const wchar_t c)
 */
{
	if (!c) {
		SetError(437,"%i",c);
		return 0;
	}
	wchar_t b[2];
	b[0]=c;
	b[1]=0;
	return Concat(b,1);
}

int CWString::InsertChar(wchar_t c, int pos)
/*!\brief Zeichen an einer bestimmten Stelle einfügen
 *
 * Diese Funktion fügt ein einzelnes Zeichen an einer bestimmten Position des Strings ein. Alle Zeichen rechts davon
 * werden dabei um eine Position nach rechts verschoben.
 *
 * \param[in] c Unicode des gewünschten Zeichens
 * \param[in] pos Position an der das Zeichen eingefügt werden soll. Ist der Wert negativ, wird die
 * Position vom Ende des Strings berechnet. -1 wäre dabei das letzte Zeichen des Strings. Eine besondere Behandlung
 * wird durchgeführt, wenn \p pos mit der Länge des Strings übereinstimmt. In diesem Fall wird die Funktion
 * CWString::AddChar aufgerufen und das Zeichen am Ende angehangen.
 * \returns Die Funktion liefert true (1) zurück, wenn das Zeichen eingefügt werden konnte. Im Fehlerfall
 * wird false (0) zurückgegeben, der String könnte aber teilweise schon manipuliert worden sein.
 */
{
	if (!c) {
		SetError(437,"%i",c);
		return 0;
	}
	if (pos<0) pos=(int)len+pos;
	if (pos<0) {
		SetError(438);
		return 0;
	}
	if ((size_t)pos<len) {
		CWString buf=Mid(pos);
		Cut(pos);
		wchar_t b[2];
		b[0]=c;
		b[1]=0;
		if (!Concat(b,1)) return 0;
		return Concat(&buf);
	}
	if ((size_t)pos==len) return AddChar(c);
	SetError(436,"Stringlänge: %i, gewünschte Position: %i",len,pos);
	return 0;
}

int CWString::Delete(int pos, int num)
/*!\brief Teil des Strings löschen
 *
 * Mit dieser Funktion kann man einen beliebig langen Teil des Strings ab einer gewünschten Position löschen.
 * Alle Zeichen rechts davon werden nach links verschoben.
 *
 * \param[in] pos Startposition, ab der gelöscht werden soll
 * \param[in] num Anzahl Zeichen, die gelöscht werden sollen
 * \returns Bei Erfolg liefert die Funktion true (1) zurück. Im Fehlerfall wird false (0) zurückgegeben, der String könnte
 * aber teilweise schon manipuliert worden sein.
 */
{
	if (!num) return 1;
	if (pos<0) pos=(int)len+pos;
	if (pos<0) {
		SetError(438);
		return 0;
	}
	if ((size_t)pos>len) {
		SetError(436,"Stringlänge: %i, gewünschte Position: %i",len,pos);
		return 0;
	}
	CWString buf=Mid(pos+num);
	Cut(pos);
	if(buf.IsEmpty()) return 1;
	return Concat(&buf);
}

std::ostream& operator<<(std::ostream& s, const CWString &str)
{
	CString localstr(str);
	return s.write((const char*)localstr,localstr.Size());
}

}	// EOF namespace ppl6
