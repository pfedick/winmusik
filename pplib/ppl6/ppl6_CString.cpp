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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <wctype.h>
#include <locale.h>
#include "ppl6.h"
#ifdef HAVE_PCRE
#define PCRE_STATIC
#include <pcre.h>
#endif


namespace ppl6 {

static ppluint32 InitialBuffersize=64;


void CString::setInitialBuffersize(ppluint32 size)
{
	InitialBuffersize=size;
}

#ifdef PPL6_ENABLE_STRING_BUFFERPOOL
//!\brief Interner Heap-Speicher für CString
static CHeap *Heap=NULL;
#endif

/*!\class CString
 * \ingroup PPLGroupDataTypes
 * \ingroup PPLGroupStrings
 * \brief String-Klasse für UTF-8 und andere Charsets
 *
 * Diese Klasse kann verwendet werden, um beliebige Strings im UTF-8-Format zu speichern und zu verarbeiten. Dabei
 * braucht sich der Anwender keine Gedanken um den verwendeten Speicher zu machen.
 *
 * \see ppl6::CWString Stringklasse zur Verwaltung von 32-Bit Wide-Characters im Unicode-Format.
 *
 *
 */

#ifdef PPL6_ENABLE_STRING_BUFFERPOOL
void *CString::operator new(size_t size)
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
 */
{
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_CString();
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

void CString::operator delete(void *ptr, size_t size)
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

#endif // PPL6_ENABLE_STRING_BUFFERPOOL

CString::CString()
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen Leeren String
*/
{
	Init();
}

CString::CString(const char *str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist ein <tt>const char *</tt> auf einen 0-terminierten String.
 */
{
	Init();
	//printf("Aufruf mit const char *str: %s\n",str);
	Set(str);
}

CString::CString(const char *str, size_t size)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist ein <tt>const char *</tt> auf einen 0-terminierten String.
 */
{
	Init();
	//printf("Aufruf mit const char *str: %s\n",str);
	Set(str,(int)size);
}

CString::CString(const wchar_t *str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist ein <tt>const char *</tt> auf einen 0-terminierten String.
 */
{
	Init();
	//printf("Aufruf mit const char *str: %s\n",str);
	Set(str);
}

CString::CString(const CString &str)
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

CString::CString(const std::string &str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist eine Instanz der Standard Template String-Klasse.
 */
{
	Init();
	//printf("Aufruf mit const char &str: %s\n",str);
	Set(str.c_str());
}

CString::CString(const CWString &str)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c str
 * \param str ist eine Instanz der Wide-String-Klasse.
 */
{
	Init();
	Set(str);
}


CString::CString(const CBinary &bin)
/*!
 * \brief Konstruktor der String-Klasse
 *
 * Erzeugt einen neuen String mit dem Inhalt von \c bin. Dies ist natürlich nur sinnvoll, wenn die Binary-Klasse auch darstellbaren Text enthält.
 *
 * \param bin ist eine Instanz von CBinary.
 */
{
	Init();
	//printf("Aufruf mit const char &str: %s\n",str);
	Set((char*)bin.GetPtr(),bin.GetSize());
}


CString::CString(CString *str)
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


void CString::Init()
{
	type=CVar::CSTRING;
	matches=NULL;
	buffer=NULL;
	len=buffersize=0;
}


/*!
 * \brief Destruktor der String-Klasse
 *
 * Der Destruktor gibt alle von dieser Instanz verwendeten Speicherbereiche wieder frei.
 */
CString::~CString()
{
	if (matches) delete matches;
	if (buffer) free(buffer);
	matches=NULL;
	buffer=NULL;
}

void CString::ImportBuffer(char *buffer, size_t bytes)
/*!\brief Externen Buffer übernehmen
 *
 * Mit dieser Funktion kann ein externer Buffer in den String übernommen werden. Dazu muss
 * die Anwendung zunächst Speicher mit \c malloc allokieren und diesen mit einem gültigen String
 * versehen. Dieser muss mit einem 0-Byte enden. Der Pointer auf den Beginn des Speichers und die
 * allokierte Größe wird dann mit dieser Funktion in den String importiert. Die Verwaltung des
 * Speicherblocks geht dabei an die Stringklasse über, darf also nicht mehr von der Anwendung
 * verändert oder gelöscht werden!
 *
 * Danach prüft die Funktion noch die tatsächliche Länge des Strings durch Aufruf der
 * Funktion ReCalcLen, für den Fall dass mehr Speicher allokiert wurde, als
 * tatsächlich gebraucht wurde. Zum Abschluss wird noch ein Change-Event ausgelöst.
 *
 * Falls der String vor Aufruf dieser Funktion noch Daten enthalten hat, werden diese
 * ordnungsgemäß gelöscht.
 *
 * @param[in] buffer Pointer auf den Speicherbereich, ab dem der String beginnt
 * @param[in] bytes Anzahl Bytes, die beim \c malloc allokiert wurden
 *
 * \note
 * Falls der String nicht mit einem 0-Byte beendet wurde, wird automatisch das letzte Byte
 * des Buffers auf 0 gesetzt:
 * \code
 * buffer[bytes-1]=0;
 * \endcode
 * Das könnte einen ungewollten Effekt haben, falls das Zeichen an dieser Stelle wichtig war.
 * Es sollte daher immer mindestens 1 Byte mehr allokiert werden, als tatsächlich benötigt wird.
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
	if (buffer==NULL || bytes==0) return;
	this->buffer=buffer;
	buffer[bytes-1]=0;
	buffersize=bytes;
	ReCalcLen();
}



/*!
 * \brief Gibt die Länge des Strings zurück
 *
 * Diese Funktion gibt die Länge des Strings zurück.
 *
 * \return Gibt entweder die Länge des Strings in Byte zurück, oder 0, wenn der String Leer ist.
 * \sa \link CString::Len Len \endlink
 * \sa \link CString::Length Length \endlink
 */
ppluint32 CString::Size() const
{
	return len;
}

/*!
 * \brief Gibt die Länge des Strings zurück
 *
 * Diese Funktion gibt die Länge des Strings zurück.
 *
 * \return Gibt entweder die Länge des Strings in Byte zurück, oder 0, wenn der String Leer ist.
 * \sa \link CString::Size Size \endlink
 * \sa \link CString::Length Length \endlink
 */
ppluint32 CString::Len() const
{
	return len;
}

/*!
 * \brief Gibt die Länge des Strings zurück
 *
 * Diese Funktion gibt die Länge des Strings zurück.
 *
 * \return Gibt entweder die Länge des Strings in Byte zurück, oder 0, wenn der String Leer ist.
 * \sa \link CString::Len Len \endlink
 * \sa \link CString::Size Size \endlink
 */	ppluint32 CString::Length() const
{
	return len;
}

void CString::Set(const char *text, int bytes)
/**
 * \brief Belegt den String mit einem Wert
 *
 * Mit Set wird dem String ein Wert zugewiesen.
 *
 * \param text Ein Pointer auf einen C-String, der in die String-Klasse kopiert werden soll
 * \param bytes Ein Optionaler Parameter, der die Anzahl zu kopierender Bytes angibt. Wird der
 * Parameter nicht angegeben, wird der komplette String übernommen. Ist \c bytes länger als
 * der in \c tmp enthaltene String, wird nur bis zum 0-Byte kopiert.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
{
	if (!text) {
		Clear();
		return;
	}
	int l;
	if (bytes>=0) l=ppl6::strnlen(text,bytes);
	else l=(int)strlen(text);
	if ((ppluint32)l<buffersize) {
		strncpy(buffer,text,l);
		len=l;
		buffer[len]=0;
		return;
	}
	if (buffer) free(buffer);
	buffer=NULL;
	buffersize=0;

	if ((ppluint32)l<InitialBuffersize) {
		buffer=(char *)malloc(InitialBuffersize);
		strncpy(buffer,text,l);
		len=l;
		buffer[len]=0;
		buffersize=InitialBuffersize;
	} else {
		buffersize=l+1;
		buffer=(char *)malloc(buffersize);
		strncpy(buffer,text,l);
		len=l;
		buffer[len]=0;
	}
}

void CString::Set(const wchar_t *text, int chars)
/**
 * \brief Belegt den String mit einem Wert
 *
 * Mit Set wird dem String ein Wert zugewiesen.
 *
 * \param text Ein Pointer auf einen WideCharacter-String, der in die String-Klasse kopiert werden soll
 * \param bytes Ein Optionaler Parameter, der die Anzahl zu kopierender Zeichen angibt. Wird der
 * Parameter nicht angegeben, wird der komplette String übernommen. Ist \c bytes länger als
 * der in \c tmp enthaltene String, wird nur bis zum 0-Byte kopiert.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
{
	if (!text) {
		Clear();
		return;
	}
	CWString w;
	w.Set(text,chars);
	Set(w.GetPtr());
}

void CString::Set(const CString *str, int bytes)
/*!\brief Belegt den String mit einem Wert
 *
 * Mit Set wird dem String ein Wert zugewiesen.
 *
 * \param str Ein Pointer auf eine CString-Klasse, dessen Inhalt kopiert werden soll
 * \param bytes Optionale Anzahl zu kopierender Bytes. Bei Angabe von 0 oder wenn
 * der Parameter weggelassen wird, wird der komplette String übernommen.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
{
	if (!str) {
		Clear();
		return;
	}
	Set((const char*)str->GetPtr(), bytes);
}

void CString::Set(const CString &str, int bytes)
/*!\brief Belegt den String mit einem Wert
 *
 * Mit Set wird dem String ein Wert zugewiesen.
 *
 * \param str Ein Pointer auf eine CString-Klasse, dessen Inhalt kopiert werden soll
 * \param bytes Optionale Anzahl zu kopierender Bytes. Bei Angabe von 0 oder wenn
 * der Parameter weggelassen wird, wird der komplette String übernommen.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */{
	if (str.IsEmpty()) {
		Clear();
		return;
	}
	Set((const char*)str.GetPtr(), bytes);
}

/*!\brief Belegt den String mit einem Wert
 *
 * Mit Set wird dem String ein Wert zugewiesen.
 *
 * \param str Ein Pointer auf eine CWString-Klasse, dessen Inhalt kopiert werden soll
 * \param size Optionale Anzahl zu kopierender Zeichen. Bei Angabe von 0 oder wenn
 * der Parameter weggelassen wird, wird der komplette String übernommen.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
void CString::Set(const CWString &str, int size)
{
	if (str.IsEmpty()) {
		Clear();
		return;
	}
	CWString s;
	s.Set(str,size);
	const char *p=s.GetPtr();
	if (!p) PrintError();
	Set(p);
}

void CString::Set(const std::string &str, int bytes)
{
	if (str.empty()) {
		Clear();
		return;
	}
	Set(str.c_str(),bytes);
}

void CString::Set(const std::wstring &str, int size)
{
	if (str.empty()) {
		Clear();
		return;
	}
	Set(str.c_str(),size);
}

/*!\brief String mit einem einzelnen Wide-Character belegen
 *
 * Mit dieser Funktion wird der String mit einem einzelnen Zeichen belegt.
 *
 * \param[in] c ASCII-Code des gewünschten Zeichens
 */
void CString::CString::SetChar(char c)
{
	if (!c) return;
	char b[2];
	b[0]=c;
	b[1]=0;
	return Set(b,1);
}

/*!\brief Einzelnes Zeichen an einer bestimmten Position überschreiben
 *
 * Mit dieser Funktion kann ein einzelnes Zeichen an einer bestimmten Position des Strings überschrieben werden.
 *
 * \param[in] c ASCII-Code des gewünschten Zeichens
 * \param[in] pos Position innerhalb des Strings, wobei 0 das erste Zeichen bedeutet. Ist der Wert negativ, wird die
 * Position vom Ende des Strings berechnet. -1 wäre dabei das letzte Zeichen des Strings. Eine besondere Behandlung
 * wird durchgeführt, wenn \p pos mit der Länge des Strings übereinstimmt. In diesem Fall wird die Funktion
 * CString::AddChar aufgerufen und das Zeichen am Ende angehangen.
 */
void CString::SetChar(char c, int pos)
{
	if (!c) return;
	if (pos<0) pos=(int)len+pos;
	if (pos<0) return;
	if ((size_t)pos<len) {
		buffer[pos]=c;
		return;
	}
	if ((size_t)pos==len) {
		AddChar(c);
		return;
	}
	SetError(436,"Stringlänge: %i, gewünschte Position: %i",len,pos);
	return;
}

/*!\brief Einzelnes Zeichen anfügen
 *
 * Mit dieser Funktion wird ein einzelnes Zeichen an den String angefügt.
 *
 * \param[in] c ASCII-Code des gewünschten Zeichens
 */
void CString::AddChar(char c)
{
	if (!c) return;
	char b[2];
	b[0]=c;
	b[1]=0;
	Concat(b,1);
}


void CString::Copy(const char *str, int bytes)
/*!
 * \brief Belegt den String mit einem Wert
 *
 * \descr
 *	Belegt den String mit einem Wert. Die Funktion ist identisch mit CString::Set.
 *
 * \param str Ein Pointer auf einen C-String, der in die String-Klasse kopiert werden soll
 * \param bytes Ein Optionaler Parameter, der die Anzahl zu kopierender Bytes angibt. Wird der
 * Parameter nicht angegeben, wird der komplette String übernommen. Ist \c bytes länger als
 * der in \c tmp enthaltene String, wird nur bis zum 0-Byte kopiert.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
{
	Set(str,bytes);
}

/*!
 * \brief Belegt den String mit einem Wert
 *
 *
 * \param str Ein Pointer auf eine CString-Klasse, dessen Inhalt kopiert werden soll
 * \param bytes Ein Optionaler Parameter, der die Anzahl zu kopierender Bytes angibt. Wird der
 * Parameter nicht angegeben, wird der komplette String übernommen. Ist \c bytes länger als
 * der in \c tmp enthaltene String, wird nur bis zum 0-Byte kopiert.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
void CString::Copy(CString &str, int bytes)
{
	Set((const char*)str.GetPtr(),bytes);
}

/*!
 * \brief Kopiert einen String
 *
 * \param str Ein Pointer auf einen C-String, der in die String-Klasse kopiert werden soll
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
void CString::Strcpy(const char *str)
{
	Set(str);
}

/*!
 * \brief Kopiert einen String
 *
 * \param str Der zu kopierende String
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
void CString::Strcpy(const CString &str)
{
	Set(str);
}

/*!
 * \brief Belegt den String mit einem Wert
 *
 *
 * \param str Ein Pointer auf eine CString-Klasse, dessen Inhalt kopiert werden soll
 * \param bytes Ein Optionaler Parameter, der die Anzahl zu kopierender Bytes angibt. Wird der
 * Parameter nicht angegeben, wird der komplette String übernommen. Ist \c bytes länger als
 * der in \c tmp enthaltene String, wird nur bis zum 0-Byte kopiert.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
void CString::Strncpy(const char *str, size_t bytes)
{
	Set(str, bytes);
}

/*!
 * \brief String teilweise kopieren
 *
 *
 * \param str Zu kopierender String
 * \param bytes Ein Optionaler Parameter, der die Anzahl zu kopierender Bytes angibt. Wird der
 * Parameter nicht angegeben, wird der komplette String übernommen. Ist \c bytes länger als
 * der in \c tmp enthaltene String, wird nur bis zum 0-Byte kopiert.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
void CString::Strncpy(const CString &str, size_t bytes)
{
	Set(str, bytes);
}

/*!
 * \brief Fügt einen String an den bestehenden String an
 *
 * \param str Ein Pointer auf eine CString-Klasse, dessen Inhalt angehangen werden soll
 * \param bytes Ein Optionaler Parameter, der die Anzahl zu kopierender Bytes angibt. Wird der
 * Parameter nicht angegeben, wird der komplette String übernommen. Ist \c bytes länger als
 * der in \c tmp enthaltene String, wird nur bis zum 0-Byte kopiert.
 * \return Nichts
 * \note Bei erfolgreichem Setzen des Strings wird die CVar::Change aufgerufen, die
 * ihrerseits eine Callback-Funktion aufruft, sofern diese definiert wurde.
 */
void CString::Add(const char *str, int bytes)
{
	Concat(str,bytes);
}


/*!
 * \brief Löscht den Inhalt des Strings
 *
 * Der Inhalt des Strings wird gelöscht und seine Länge auf 0 gesetzt. Außerden werden
 * alle belegten Speicherblöcke freigegeben.
 * \return Nichts
 */

void CString::Clear()
{
	if (buffer) free(buffer);
	if (matches) delete matches;
	matches=NULL;
	buffer=NULL;
	buffersize=len=0;
}

/*!
 * \brief Erzeugt einen formatierten String
 *
 * Diese Funktion ist vergleichbar mit dem Standard-C-Kommando sprintf und unterstützt die
 * gleiche Formatierung.
 * \param fmt Ein Pointer auf einen Format-String.
 * \param ... Bliebig viele weiteren Parameter.
 * \return Nichts
 *
 * \copydoc sprintf.dox
 */
void CString::Sprintf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		Set(buff);
	}
	va_end(args);
    if (buff) free(buff);
}

/*! \brief Erzeugt einen formatierten String
 *
 * \copydoc CString::Vasprintf
 */
void CString::VaSprintf(const char *fmt, va_list args)
{
	Vasprintf(fmt,args);
}

/*! \brief Erzeugt einen formatierten String
 *
 * Erzeugt einen formatierten String
 * \param fmt Der Formatstring
 * \param args Pointer auf Liste der Parameter. Muss zuvor mit va_start initialisiert worden sein.
 * \par Example:
 * \code
void MyFunction(const char *fmt, ...)
{
	CString String;
	va_list args;
	va_start(args, fmt);
	String.VaSprintf(fmt,args);
	va_end(args);
	printf ("Ergebnis: %s",(char*)String);
}
\endcode
 *
 * \copydoc sprintf.dox
 */
void CString::Vasprintf(const char *fmt, va_list args)
{
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		Set(buff);
	}
	if (buff) free(buff);
}

//! \brief Erzeugt einen formatierten String
void CString::Setf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		Set(buff);
	}
	va_end(args);
    if (buff) free(buff);
}

//! \brief Liefert einen char-Pointer auf den internen String zurück
const char *CString::GetPtr() const
{
	if (!buffer) return "";
	return (const char *)buffer;
}

//! \brief Liefert einen char-Pointer auf den internen String zurück
CString::operator const char *() const
{
	if (!buffer) return "";
	return (const char *)buffer;
}

//! \brief Liefert den Integer-Wert des Strings zurück
CString::operator int() const
{
	if (!buffer) return (int)0;
	return atoi(buffer);
}

//! \brief Liefert je nach Inhalt des Strings true oder false zurück
CString::operator bool() const
{
	return (ppl6::IsTrue(buffer)) ;
}

//! \brief Liefert den Integer-Wert des Strings zurück
CString::operator unsigned int() const
{
	if (!buffer) return (int)0;
	return (unsigned int) atoi(buffer);
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
int CString::IsNumeric() const
{
	int c;
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
int CString::IsInteger() const
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

int CString::IsEmpty() const
/*! \brief Prüft, ob der String leer ist
 *
 * Diese Funktion prüft, ob der String Zeichen enthält
 *
 * \returns Ist der String leer, liefert die Funktion true (1) zurück, sonst false.
 * \see CString::NotEmpty
 */
{
	if (!len) return 1;
	return 0;
}

int CString::NotEmpty() const
/*! \brief Prüft, ob der String Daten enthält
 *
 * Diese Funktion prüft, ob der String Zeichen enthält
 *
 * \returns Enthält der String Daten (Länge > 0), liefert die Funktion true (1) zurück, sonst false.
 * \see CString::IsEmpty
 */
{
	if (len) return 1;
	return 0;
}

//! \brief Fügt einen formatierten String an das Ende des vorhandenen an
void CString::Concatf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char *buff=NULL;
	if (vasprintf (&buff, (char*)fmt, args)>0 && buff!=NULL) {
		Concat(buff);
	}
	va_end(args);
    if (buff) free(buff);
}

//! \brief Fügt einen String an das Ende des vorhandenen an
void CString::Concat(const char *text, int bytes)
{
	if (!buffer) {
		Set(text,bytes);
		return;
	}
	if (!text) return;
	int l;
	if (bytes>=0) l=bytes;
	else l=strlen(text);
	if (len+l<buffersize) {
		strncat(buffer,text,l);
		len+=l;
		buffer[len]=0;
		return;
	}
	char *t=(char*)realloc(buffer,len+l+4);
	//char *t=(char*)malloc(len+l+4);
	if (!t) {
		return;
	}
	//memcpy(t,buffer,l);		//
	//free(buffer);			//
	buffersize=len+l+4;
	buffer=t;
	strncat(buffer,text,l);
	len+=l;
	buffer[len]=0;
}

//! \brief Fügt einen String an das Ende des vorhandenen an
void CString::Concat(CString &str, int bytes)
{
	Concat(str.GetPtr(),bytes);
}

//! \brief Reserviert Speicher für den String
int CString::Reserve(ppluint32 size)
{
	if (size>buffersize) {
		if (buffer) {
			char *t=(char*)realloc(buffer,size);
			if (t) {
				buffer=t;
				buffersize=size;
				return 1;
			}
			SetError(2);
			return 0;
		}
		buffer=(char*)malloc(size);
		buffersize=size;
		len=0;
		buffer[0]=0;
	}
	return 1;
}

int CString::Capacity() const
{
	return buffersize;
}

void CString::ReCalcLen()
/* \brief Berechnet die Länge des Strings neu
 *
 * Wird von CString::ImportBuffer aufgerufen, um die tatsächliche Länge des
 * Strings festzustellen.
 */
{
	if (buffersize>0 && buffer!=NULL) {
		for (ppluint32 i=0;i<buffersize;i++) {
			if (buffer[i]==0) {
				len=i;
				return;
			}
		}
		len=buffersize-1;
		buffer[len]=0;
	} else {
		len=0;
	}
}

//! \brief Gibt den Inhalt des Strings auf STDOUT aus.
void CString::Print(bool attach_newline) const
{
	if (buffer) PrintDebug("%s",buffer);
	if (attach_newline) PrintDebug("\n");
}

//! \brief Macht einen Hexdump des Strings auf STDOUT
void CString::HexDump() const
{
	PrintDebug ("HexDump CString\n");
	if (buffer) {
		ppl6::HexDump(buffer,len);
	}
}

void CString::HexDump(const void *buffer, ppldd bytes, bool skipheader)
{
	ppl6::HexDump(this,buffer,bytes,skipheader);
}

//! \brief Gibt den Integer-Wert des Strings zurück
int CString::ToInt() const
{
	if (!buffer) return 0;
	return atoi(buffer);
}

//! \brief Gibt einen 64-Bit Integer-Wert des Strings zurück
pplint64 CString::ToInt64() const
{
	if (!buffer) return 0;
	return ppl6::atoll(buffer);
}

//! \brief Gibt je nach Inhalt des Strings true oder false zurück
bool CString::ToBool() const
{
	return (ppl6::IsTrue(buffer));
}

bool CString::IsTrue() const
{
	return (ppl6::IsTrue(buffer));
}

bool CString::IsFalse() const
{
	return (!ppl6::IsTrue(buffer));
}


//! \brief Gibt den Long-Wert des Strings zurück
long CString::ToLong() const
{
	if (!buffer) return 0;
	return atol(buffer);
}

//! \brief Gibt den Long Long-Wert des Strings zurück
long long CString::ToLongLong() const
{
	if (!buffer) return 0;
	//CString str=buffer;
	//str.Trim();
	return ppl6::atoll(buffer);
}

//! \brief Gibt den Float-Wert des Strings zurück
float CString::ToFloat() const
{
	if (!buffer) return 0;
	return (float)atof(buffer);
}

//! \brief Gibt den Double-Wert des Strings zurück
double CString::ToDouble() const
{
	if (!buffer) return 0;
	return atof(buffer);
}

//! \brief Schneidet Leerzeichen, Tabs Returns und Linefeeds am Anfanng und Ende des Strings ab
void CString::Trim()
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
				memmove(buffer,buffer+start,ende-start+2);
		}
		len=strlen(buffer);
	}
}

//! \brief Schneidet Returns und Linefeeds am Anfanng und Ende des Strings ab
void CString::Chomp()
{
	if (buffer) {
		size_t i,start,ende,s;
		if (len>0) {
			start=0; s=0;
			ende=len;
			for (i=0;i<len;i++) {
				if (buffer[i]==13||buffer[i]==10) {
					if (s==0) start=i+1;
				} else {
					s=1; ende=i;
				}
			}
			buffer[ende+1]=0;
			if (start>0)
				memmove(buffer,buffer+start,ende-start+2);
		}
		len=strlen(buffer);
	}
}

//! \brief Schneidet Zeichen am Ende des Strings ab
void CString::Chop(int chars)
{
	if (buffer) {
		if (len>0) {
			ppluint32 c=chars;
			if (len<c) c=len;
			len-=c;
			buffer[len]=0;
		}
	}
}

//! \brief Schneidet Leerzeichen, Tabs Returns und Linefeeds am Anfanng des Strings ab
void CString::LTrim()
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
				memmove(buffer,buffer+start,len-start+1);
		}
		len=strlen(buffer);
	}
}

//! \brief Schneidet Leerzeichen, Tabs Returns und Linefeeds am Ende des Strings ab
void CString::RTrim()
{
	if (buffer!=NULL && len>0) {
		size_t i,ende;
		char w;
		ende=0;
		for (i=len;i>0;i--) {
			w=buffer[i-1];
			if (w!=13 && w!=10 && w!=32 && w!='\t') {
				ende=i;
				break;
			}
		}
		buffer[ende]=0;
		len=strlen(buffer);
		buffer[len]=0;
	}
}

//! \brief Schneidet die definierten Zeichen am Anfang des Strings ab
void CString::LTrim(const char *str)
{
	if (buffer) {
		size_t i,start,s, l_str,z;
		int match;
		l_str=strlen(str);
		if (len>0) {
			start=0; s=0;
			for (i=0;i<len;i++) {
				match=0;
				for (z=0;z<l_str;z++) {
					if (buffer[i]==str[z]) {
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
				memmove(buffer,buffer+start,len-start+1);
		}
		len=strlen(buffer);
	}
}

//! \brief Schneidet die definierten Zeichen am Ende des Strings ab
void CString::RTrim(const char *str)
{
	size_t l_str=strlen(str);
	if (buffer!=NULL && len>0 && l_str>0) {
		size_t i,ende,z;
		int match;
		char w;
		ende=0;
		for (i=len;i>0;i--) {
			w=buffer[i-1];
			match=0;
			for (z=0;z<l_str;z++) {
				if (w==str[z]) {
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
		len=strlen(buffer);
	}
}

//! \brief Schneidet die definierten Zeichen am Anfang und Ende des Strings ab
void CString::Trim(const char *str)
{
	LTrim(str);
	RTrim(str);
}


//! \brief Wandelt alle Zeichen des Strings in Kleinbuchstaben um
void CString::LCase()
{
	/*
	size_t i,l;
	if (buffer) {
		l=strlen(buffer);
		for (i=0;i<l;i++)
			buffer[i]=tolower(buffer[i]);
		Change();
	}
	*/
	if (!buffer) return;
	ppl6::CWString res=buffer;
	res.LCase();
	Set(res);
	/*
	char buf[MB_CUR_MAX];
	char *p=buffer;
	int charlen;
	wchar_t wc;
	wint_t lower;
	while ((charlen = mbtowc(&wc, p, MB_CUR_MAX)) && charlen>0) {
		lower=towlower(wc);
		p += charlen;
		charlen=wctomb(buf,lower);
		res.Concat(buf,charlen);
	}
	Set(&res);
	*/
}

//! \brief Wandelt alle Zeichen des Strings in Grossbuchstaben um
void CString::UCase()
{
	if (!buffer) return;
	ppl6::CWString res=buffer;
	res.UCase();
	Set(res);
}

void CString::UCWords()
/*!\brief Anfangsbuchstaben der Wörter groß
 *
 * \desc
 * Diese Funktion wandelt die Anfangsbuchstaben aller im String enthaltenen Wörter in
 * Großbuchstaben um.
 */
{
	if (!buffer) return;
	ppl6::CWString res=buffer;
	res.UCWords();
	Set(res);
}

//! \brief Führt einen Vergleich mit einem anderen String durch
/* \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
int CString::StrCmp(const char *str, int size) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) str="";
	if (size) return strncmp(mystr,str,size);
	return strcmp(mystr,str);
}

//! \brief Führt einen Vergleich mit einem anderen String durch
/* \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
int CString::StrCmp(const CString &str, int size) const
{
	const char *mystr=buffer;
	const char *otherstr=str.buffer;
	if (!mystr) mystr="";
	if (!otherstr) otherstr="";
	if (size) return strncmp(mystr,otherstr,size);
	return strcmp(mystr,otherstr);
}

//! \brief Führt einen Vergleich mit einem anderen String durch, wobei Gross-/Kleinschreibung ignoriert wird
/* \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
int CString::StrCaseCmp(const char *str, int size) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) str="";
	if (size) return strncasecmp(mystr,str,size);
	return strcasecmp(mystr,str);
}

//! \brief Führt einen Vergleich mit einem anderen String durch, wobei Gross-/Kleinschreibung ignoriert wird
/* \returns Ist der String kleiner als der mit \a str angegebene, wird ein negativer Wert
 * zurückgegeben, ist er identisch, wird 0 zurückgegeben, ist er größer, erfolgt ein positiver
 * Return-Wert.
 */
int CString::StrCaseCmp(const CString &str, int size) const
{
	const char *mystr=buffer;
	const char *otherstr=str.buffer;
	if (!mystr) mystr="";
	if (!otherstr) otherstr="";
	if (size) return strncasecmp(mystr,otherstr,size);
	return strcasecmp(mystr,otherstr);
}

//! \brief Ersetzt einen Teilstring durch einen anderen
CString& CString::Replace(const char* str, const char* byStr)
{
	ppluint32 start = 0, slen = strlen(str);
	int end;
	// collect the result
	CString ms;
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

/*! \brief Sucht nach einem String
 *
 * Find sucht nach dem Suchstring \a str ab der gewünschten Position \a pos.
 * \returns Liefert die Position innerhalb des Strings, an der der Suchstring gefunden wurde
 * oder -1 wenn er nicht gefunden wurde.
 */
int CString::Find(const char* str, int pos) const
{
	if (!buffer) return -1;
	//Position to return
	int p = -1;
	//Length of the string to search for
	ppluint32 len = this->len, lstr = strlen(str);
	//Current position to search from and position of found string
	char *found = NULL, *tmp = NULL;

	//Proceed only if there's a contained and a given string
	if(buffer != NULL && lstr > 0) {
		//Search forward
		if(pos >= 0) {
			//Search first occurence, starting at the given position...
			found = strstr(buffer + pos, str);
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
			while((found = strstr((tmp == NULL ? buffer : tmp + 1), str)) != NULL && found - buffer + lstr <= len + pos)
	        tmp = found;

			//Calculate the position to return if str was found
			if(tmp != NULL) {
				p = tmp - buffer;
			}
		}
	}
	return p;
}

// \brief erstellt aus den ersten \a len Zeichen einen neuen String
CString CString::Left(ppluint32 len) const
{
	CString ms;

	//Proceed only if there's a contained string
	if(buffer != NULL && len > 0) {
		//Adjust the len if the contained string is shorter
		if(len > this->len) len = this->len;
		ms.Set(buffer,len);
	}
	return ms;
}

// \brief erstellt aus den letzten \a len Zeichen einen neuen String
CString CString::Right(ppluint32 len) const
{
	CString ms;
	//proceed only if there's a contained string
	if(buffer != NULL && len > 0) {
		//Adjust the len if the contained string is shorter
		if(len > this->len)
			len = this->len;
		ms.Set(buffer+this->len-len,len);
	}
	return ms;
}

/*!\brief String-Ausschnitt
 *
 * \desc
 * Mit dieser Funktion wird ein Ausschnitt des vorhandenen Strings ab der Position \p start mit einer Länge von
 * \p len Bytes (nicht Zeichen!) in einen neuen String kopiert und zurückgegeben.
 *
 * @param start Startposition innerhalb des Strings
 * @param len Optionale Angabe der Länge. Wird der Parameter nicht angegeben, wird der String ab der Startposition \p start
 * bis zum Ende kopiert
 * @return Neuer String, der den gewünschten Ausschnitt enthält. Ist \p start größer als
 * der vorhandene String, wird ein leerer String zurückgegeben. Ist \p start + \p len größer als
 * der vorhandene String, wird nur der Teil ab \p start bis zum Ende zurückgegeben.
 */
CString CString::Mid(ppluint32 start, ppluint32 len) const
{
	CString ms;
	//Length of the contained string
	ppluint32 slen = this->len;
	/* Proceed only if there's a contained string and the
	   parameters have reasonable values */
	if(buffer != NULL && len > 0 && start < slen) {
		//Check the length to return
		if(slen - start < len)
			len = slen - start;
		ms.Set(buffer+start,len);
	}
	return ms;
}

/*!\brief String-Ausschnitt
 *
 * \desc
 * Mit dieser Funktion wird ein Ausschnitt des vorhandenen Strings ab der Position \p start mit einer Länge von
 * \p len Bytes (nicht Zeichen!) in einen neuen String kopiert und zurückgegeben.
 *
 * @param start Startposition innerhalb des Strings
 * @param len Optionale Angabe der Länge. Wird der Parameter nicht angegeben, wird der String ab der Startposition \p start
 * bis zum Ende kopiert
 * @return Neuer String, der den gewünschten Ausschnitt enthält. Ist \p start größer als
 * der vorhandene String, wird ein leerer String zurückgegeben. Ist \p start + \p len größer als
 * der vorhandene String, wird nur der Teil ab \p start bis zum Ende zurückgegeben.
 */
CString CString::SubStr(ppluint32 start, ppluint32 len) const
{
	CString ms;
	//Length of the contained string
	ppluint32 slen = this->len;
	/* Proceed only if there's a contained string and the
	   parameters have reasonable values */
	if(buffer != NULL && len > 0 && start < slen) {
		//Check the length to return
		if(slen - start < len)
			len = slen - start;
		ms.Set(buffer+start,len);
	}
	return ms;
}

int CString::PregMatch(const ppl6::CString &expression, CArray &res) const
{
#ifndef HAVE_PCRE
	SetError(276);
	return 0;
#else
	res.Clear();
	if (expression.IsEmpty()) return 0;
	if (!buffer) return 0;
	if (!len) return 0;
	char *r=strdup(expression.GetPtr()+1);
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
	// Wenn das System auf UTF-8 eingestellt ist, setzen wir das UTF8-Flag automatisch
	if (instrcase(setlocale(LC_CTYPE,NULL),"UTF-8",0)>=0) flags|=PCRE_UTF8;
	const char *perr;
	size_t maxmatches=16;
	int re,erroffset;
	int ovectorsize=(maxmatches+1)*2;
	int *ovector=(int*)malloc(ovectorsize*sizeof(int));
	int perrorcode;
	pcre *reg;
	//printf ("r=%s, flags=%i\n",r,flags);
	CString__PregMatch_Restart:
	reg=pcre_compile2(r,flags,&perrorcode,&perr, &erroffset, NULL);
	if (!reg) {
		free(r);
		free(ovector);
		return 0;
	}
	memset(ovector,0,ovectorsize*sizeof(int));
	if ((re=pcre_exec(reg, NULL, (char*) buffer,len,0, 0, ovector, ovectorsize))>=0) {
		if (re>0) maxmatches=re;
		else maxmatches=maxmatches*2/3;
		for (size_t i=0;i<maxmatches;i++) {
			const char *tmp=NULL;
			pcre_get_substring(buffer,ovector,30,i,(const char**)&tmp);
			if (tmp) {
				//printf("tmp[%i]=%s\n",i,tmp);
				res.Add(tmp);
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
		goto CString__PregMatch_Restart;
	}
	pcre_free(reg);
	free(ovector);
	free(r);
	return 0;
#endif
}

/*! \brief Der String wird anhand einer Regular Expression durchsucht
 *
 * Durchsucht den String anhand einer Perl-Expression
\param expr Ist eine Perl-kompatible Regular Expression, die mit Slash (/) beginnt und
endet. Optional können nach dem schließenden Slash folgende Optionen angegeben werden:
\copydoc pregexpr.dox
\param res ist ein Optionales Array, in dem die gefundenen Matches gespeichert werden. Wird
res nicht angegeben, werden die Ergebnisse intern gespeichert und können mit der Funktion
GetMatch ausgelesen werden.
\return Liefert \c true(1) zurück, wenn ein Match gefunden wurde, ansonsten \c false(0)

\copydoc pcrenote.dox
*/
int CString::PregMatch(const char *expr, CArray *res)
{
	#ifndef HAVE_PCRE
		SetError(276);
		return 0;
	#else
	if (!res) {
		if (!matches) matches=new CArray;
		res=matches;
	}
	return PregMatch(expr,*res);
	#endif
}

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
\note \copydoc pcrenote.dox
 */
int CString::PregReplace(const char *expr, CString &replace, int maxreplace)
{
	return PregReplace(expr,replace.GetPtr(),maxreplace);
}

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

\copydoc pcrenote.dox
 */
int CString::PregReplace(const char *expr, const char *replace, int maxreplace)
{
	#ifndef HAVE_PCRE
		SetError(276);
		return 0;
	#else
	if (!expr) return 0;
	if (!buffer) return 0;
	if (!len) return 0;
	if (!matches) matches=new CArray;
	CString pattern;
	int ret=0;
	char *r=strdup(expr+1);
	int flags=0;
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
		if (strchr(options,'8')) flags|=PCRE_UTF8;
		if (strchr(options,'a')) flags|=PCRE_ANCHORED;
		if (strchr(options,'u')) flags|=PCRE_UNGREEDY;
	}
	// Wenn das System auf UTF-8 eingestellt ist, setzen wir das UTF8-Flag automatisch
	if (instrcase(setlocale(LC_CTYPE,NULL),"UTF-8",0)>=0) flags|=PCRE_UTF8;

	pattern+=r;
	const char *perr;
	int re,erroffset, ovector[30];
	int perrorcode;
	pcre *reg;
	//printf ("r=%s, flags=%i\n",r,flags);
	CString neu;
	CString Replace;
	char rep[5];

CString__PregReplace_Restart:
    reg=pcre_compile2(r,flags,&perrorcode,&perr, &erroffset, NULL);
	if (reg) {
		CString rest=buffer;
		while (1) {		// Endlosschleife, bis nichts mehr matched
			memset(ovector,0,30*sizeof(int));
			if ((re=pcre_exec(reg, NULL, (const char*) rest,rest.Len(),0, 0, ovector, 30))>=0) {
				ret++;
				Replace=replace;
				for (int i=0;i<14;i++) {
					tmp=NULL;
					pcre_get_substring((const char*)rest,ovector,30,i,(const char**)&tmp);
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
			} else if ((flags&PCRE_UTF8)==PCRE_UTF8 && (re==PCRE_ERROR_BADUTF8 || re==PCRE_ERROR_BADUTF8_OFFSET)) {
				// Wir haben ungültiges UTF_8
				// Vielleicht matched es ohne UTF-8-Flag
				flags-=PCRE_UTF8;
				free(reg);
				goto CString__PregReplace_Restart;

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

/*! \brief Fügt dem String Escape-Zeichen zu, zur Verwendung in einem Match
 *
 * Der Befehl scannt den String nach Zeichen mit besonderer Bedeutung in einer Perl-Regular-Expression und
 * escaped sie mit einem Slash. Das Ergebnis kann dann in einer Regular Expression verwendet werden.
 *
 * Es werden folgende Zeichen Escaped:
 * <b>.\+*?[^]$(){}=!<>|:/</b>
 */

void CString::PregEscape()
{
	if (!len) return;
	CString t;
	char ascii[2];
	ascii[1]=0;
	for (ppluint32 i=0;i<len;i++) {
		ascii[0]=buffer[i];
		// .\\+*?[^]$(){}=!<>|:
		if (instr(".\\+*?[^]$(){}=!<>|:/",ascii,0)>=0) t+="\\";
		t+=ascii;
	}
	if (StrCmp(t)!=0) Set(t);
}

/*! \brief Schiebt den String nach links
 *
 * Der String wird um die mit \c size angegebenen Zeichen nach links verschoben und rechts mit dem durch \c c angegebenen
 * Zeichen aufgefüllt.
 * \param c Das Zeichen, mit dem der String auf der rechten Seite aufgefüllt werden soll. Wird der Wert 0 verwendet, findet keine
 * Auffüllung statt, d.h. der String verkürzt sich einfach.
 * \param size Die Anzahl Zeichen, um die der String nach links verschoben werden soll. Ist \c size größer als die Länge
 * des Strings, wird der String komplett geleert und ist anschließend so groß wie size, sofern c>0 war.
 */
void CString::Shl(char c, int size)
{
	if (!len) return;
	if (!size) return;
	if ((ppluint32)size>len) size=len;
	CString t=Mid(size);
	if (c) {
		CString a;
		a.Repeat(c,size);
		t+=a;
	}
	Set(t);
}


/*! \brief Schiebt den String nach rechts
 *
 * Der String wird um die mit \c size angegebenen Zeichen nach rechts verschoben und links mit dem durch \c c angegebenen
 * Zeichen aufgefüllt.
 * \param c Das Zeichen, mit dem der String auf der linken Seite aufgefüllt werden soll. \c c muß größer 0 sein.
  * \param size Die Anzahl Zeichen, um die der String nach rechts verschoben werden soll. Ist \c size größer als die Länge
 * des Strings, wird der String komplett geleert und ist anschließend so groß wie size.
 */
void CString::Shr(char c, int size)
{
	if (!len) return;
	if (!size) return;
	if (!c) return;
	if ((ppluint32)size>len) size=len;
	CString t;
	t.Repeat(c,size);
	t+=buffer;
	t.Cut(len);
	Set(t);
}

/*! \brief Schneidet den String an einer bestimmten Stelle ab
 *
 * Der String wird an einer bestimmten Stelle einfach abgeschnitten
 * \param position Die Position, an der der String abgeschnitten wird. Bei Angabe von 0 ist der String anschließend
 * komplett leer. Ist \c position größer als die Länge des Strings, passiert nichts.
 */
void CString::Cut(int position)
{
	if (!len) return;
	if ((ppluint32)position>len) return;
	buffer[position]=0;
	len=position;
}

void CString::Cut(const char *letter)
/*! \brief Schneidet den String beim ersten Auftauchen eines Zeichens/Strings ab
 *
 * Der String wird beim ersten Auftauchen eines Zeichens oder eines Strings abgeschnitten.
 * \param[in] letter Buchstabe oder Buchstabenkombination, an der der String abgeschnitten werden
 * soll. Zeigt der Pointer auf NULL oder ist der String leer, passiert nichts.
  */

{
	if (!len) return;
	if (!letter) return;
	if (strlen(letter)==0) return;
	int p=instr(buffer,letter,0);
	if (p>=0) {
		buffer[p]=0;
		len=p;
	}
}



/*!Schneidet Zeichen auf der linken Seite des Strings ab
 * \param chars gibt an, wieviele Zeichen abgeschnitten werden sollen
 */
void CString::TrimL(ppluint32 chars)
{
	if (!len) return;
	if (chars>len) chars=len;
	memmove(buffer,buffer+chars,len-chars);
	len-=chars;
	buffer[len]=0;
}

/*!Schneidet Zeichen auf der rechten Seite des Strings ab
 * \param chars gibt an, wieviele Zeichen abgeschnitten werden sollen
 */
void CString::TrimR(ppluint32 chars)
{
	if (!len) return;
	if (chars>len) chars=len;
	len-=chars;
	buffer[len]=0;
}

int CString::Instr(const CString &str, int pos) const
{
	if (!str.buffer) return -1;
	return instr(buffer,str.buffer,pos);
}

int CString::Instr(const char *str, int pos) const
{
	return instr(buffer,str,pos);
}

int CString::InstrCase(const CString &str, int pos) const
{
	if (!str.buffer) return -1;
	return instrcase(buffer,str.buffer,pos);
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
const char *CString::GetMatch(int index) const
{
	if (!matches) return NULL;
	return matches->Get(index);
}

char CString::Get(int pos) const
{
	if (!len) return 0;
	if (pos>(int)len) return 0;
	if (pos<0) pos=(int)len+pos;
	if (pos<0) return 0;
	return buffer[pos];
}

/*
 * Operatoren
 */

char CString::operator[](int pos) const
{
	if (!len) return 0;
	if (pos>(int)len) return 0;
	if (pos<0) pos=(int)len+pos;
	if (pos<0) return 0;
	return buffer[pos];
}

bool CString::operator==(const char *str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) return false;
	if (strcmp(mystr,str)==0) return true;
	return false;
}

bool CString::operator!=(const char *str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) return true;
	if (strcmp(mystr,str)==0) return false;
	return true;
}

bool CString::operator<(const char *str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) return false;
	if (strcmp(mystr,str)<0) return true;
	return false;
}

bool CString::operator<=(const char *str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) return false;
	if (strcmp(mystr,str)<=0) return true;
	return false;
}

bool CString::operator>(const char *str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) return false;
	if (strcmp(mystr,str)>0) return true;
	return false;
}

bool CString::operator>=(const char *str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	if (!str) return false;
	if (strcmp(mystr,str)>=0) return true;
	return false;
}


bool CString::operator<(const CString &str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	const char *otherstr=str.buffer;
	if (!otherstr) otherstr="";
	if (strcmp(mystr,otherstr)<0) return true;
	return false;
}

bool CString::operator<=(const CString &str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	const char *otherstr=str.buffer;
	if (!otherstr) otherstr="";
	if (strcmp(mystr,otherstr)<=0) return true;
	return false;

}

bool CString::operator==(const CString &str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	const char *otherstr=str.buffer;
	if (!otherstr) otherstr="";
	if (strcmp(mystr,otherstr)==0) return true;
	return false;
}

bool CString::operator!=(const CString &str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	const char *otherstr=str.buffer;
	if (!otherstr) otherstr="";
	if (strcmp(mystr,otherstr)!=0) return true;
	return false;
}

bool CString::operator>=(const CString &str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	const char *otherstr=str.buffer;
	if (!otherstr) otherstr="";
	if (strcmp(mystr,otherstr)>=0) return true;
	return false;
}

bool CString::operator>(const CString &str) const
{
	const char *mystr=buffer;
	if (!mystr) mystr="";
	const char *otherstr=str.buffer;
	if (!otherstr) otherstr="";
	if (strcmp(mystr,otherstr)>0) return true;
	return false;
}



CString& CString::operator=(const char* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}

CString& CString::operator=(const wchar_t* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}


CString& CString::operator=(char* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str);
	return *this;
}

CString& CString::operator=(const CString* str)
{
	if (!str) {
		Clear();
		return *this;
	}
	Set(str->GetPtr());
	return *this;
}

CString& CString::operator=(const CString& str)
{
	Set(str.buffer);
	return *this;
}

CString& CString::operator=(const std::string& str)
{
	Set(str.c_str());
	return *this;
}

CString& CString::operator=(const CWString& str)
{
	Set(str);
	return *this;
}

CString& CString::operator=(const CBinary& bin)
{
	Set((char*)bin.GetPtr(),bin.GetSize());
	return *this;
}

CString& CString::operator+=(int c)
{
	Concatf("%c",c);
	return *this;
}


CString& CString::operator+=(const char* str)
{
	if (!str) return *this;
	Concat(str);
	return *this;
}

CString& CString::operator+=(const CString& str)
{
	Concat(str.buffer);
	return *this;
}

CString& CString::Repeat(int num)
{
	if ((!buffer)||len==0) return *this;
	char *buf=(char*)malloc(len*num+1);
	if (!buf) return *this;
	char *tmp=buf;
	for (int i=0;i<num;i++) {
		strncpy(tmp,buffer,len);
		tmp+=len;
	}
	tmp[0]=0;
	Set(buf);
	free(buf);
	return *this;
}

CString& CString::Repeat(const char *str, int num)
{
	if (!str) {
		Set("");
		return *this;
	}
	int l=strlen(str);
	if (l==0 || num==0) {
		Set("");
		return *this;
	}
	char *buf=(char*)malloc(l*num+1);
	if (!buf) return *this;
	char *tmp=buf;
	for (int i=0;i<num;i++) {
		strncpy(tmp,str,l);
		tmp+=l;
	}
	tmp[0]=0;
	Set(buf);
	free(buf);
	return *this;
}

/*! \brief Füllt den String mit einem Zeichen
 *
 * Der String wird mit einem gewünschten Zeichen gefüllt
 * \param ascii Der ASCII-Code des Zeichens, mit dem der String gefüllt werden soll
 * \param num Die Länge des gewünschten Strings
 * \returns liefert einen neuen String zurück
 */
CString& CString::Repeat(char ascii, int num)
{
	if (!ascii) {
		Set("");
		return *this;
	}
	if (!num) {
		Set("");
		return *this;
	}
	char *buf=(char*)malloc(num+1);
	if (!buf) return *this;
	for (int i=0;i<num;i++) buf[i]=ascii;
	Set(buf,num);
	free(buf);
	return *this;
}


CString& CString::Repeat(const CString& str, int num)
{
	if (str.len==0 || num==0 || str.buffer==NULL) {
		Set("");
		return *this;
	}
	char *buf=(char*)malloc(str.len*num+1);
	if (!buf) return *this;
	char *tmp=buf;
	for (int i=0;i<num;i++) {
		strncpy(tmp,str.buffer,str.len);
		tmp+=str.len;
	}
	tmp[0]=0;
	Set(buf);
	free(buf);
	return *this;
}

CString CString::GetMD5() const
{
	CString m;
	if (len>0 && buffer!=NULL) {
		if (!m.Reserve(33)) return m;
		MD5_CTX ctx;
		MD5Init(&ctx);
		MD5Update(&ctx,(const unsigned char *)buffer,len);
		MD5End(&ctx, m.buffer);
		m.buffer[32]=0;
		m.len=32;
	}
	return m;
}

int CString::MD5(const CString &str)
{
	if (str.len==0 || str.buffer==NULL) {
		SetError(27);
		return 0;
	}
	if (!Reserve(33)) return 0;
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(const unsigned char *)str.buffer,str.len);
	MD5End(&ctx, buffer);
	buffer[32]=0;
	len=32;
	return 1;
}

int CString::MD5(const char *str)
{
	if (!str) {
		SetError(194);
		return 0;
	}
	int len=(int)strlen(str);
	return MD5(str, len);
}

int CString::MD5(const char *str, int len)
{
	if ((!str) || (len<1)) {
		SetError(194);
		return 0;
	}
	if (!Reserve(33)) return 0;
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(const unsigned char *)str,len);
	MD5End(&ctx, buffer);
	buffer[32]=0;
	len=32;
	return 1;
}

int CString::MD5()
{
	if (len==0 || buffer==NULL) {
		SetError(27);
		return 0;
	}
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(const unsigned char *)buffer,len);
	if (!Reserve(33)) {
		return 0;
	}
	MD5End(&ctx, buffer);
	buffer[32]=0;
	len=32;
	return 1;
}

void CString::StripSlashes()
{
	if (len==0 || buffer==NULL) return;
	int p=0, np=0;
	char a, lastchar=0;
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
	len=np;
}

int CString::Transcode(const char *fromcharset, const char *tocharset)
/*!\brief String von einem Charset in ein anderes umwandeln
 *
 * \header \#include <ppl6.h>
 * \desc
 * Sofern die "libiconv"-Library einkompiliert wurde, kann diese Funktion verwendet
 * werden, um den String von einem Character-Encoding in ein anders umzuwandeln.
 *
 * \param[in] fromcharset Ein Pointer auf einen String, der den Namen des
 * Quell-Encodings enthält, beispielsweise "ISO-8859-1".
 * \param[in] tocharset Ein Pointer auf einen String, der den Namen des
 * Ziel-Encodings enthält, beispielsweise "UTF-8"
 * \return Im Erfolgsfall liefert die Funktion true (1) zurück, im Fehlerfall
 * false (0). Es können folgende Fehler auftreten_
 * - 2:   Nicht genug Speicher (ENOMEM)
 * - 194: Parameter darf nicht NULL sein
 * - 286: LibIconv ist nicht einkompiliert
 * - 287: Charset wird nicht unterstuetzt
 * - 288: Charset ist nicht initialisiert
 * - 289: Konvertierung fehlgeschlagen
 *
 * \see Siehe auch die Klasse CIconv
 * \since PPL Version 6.0.19
*/
{
	#ifndef HAVE_ICONV
		SetError(286);
		return 0;
	#else
		if (!buffer) return 1;
		if (!len) return 1;
		if (!fromcharset) {
			SetError(194,"int CString::Transcode( ==> const char *fromcharset <== , const char *tocharset)");
			return 0;
		}
		if (!tocharset) {
			SetError(194,"int CString::Transcode(const char *fromcharset, ==> const char *tocharset <== )");
			return 0;
		}
		CIconv Iconv;
		if (!Iconv.Init(fromcharset,tocharset)) return 0;
		char *ret=Iconv.Transcode((const char*)buffer);
		if (!ret) return 0;
		Set(ret);
		free(ret);
		return 1;
	#endif
}

int CString::Transcode(const char *fromcharset, const char *tocharset, CBinary &Target) const
{
#ifndef HAVE_ICONV
	SetError(286);
	return 0;
#else
	Target.Clear();
	if (!fromcharset) {
		SetError(194,"int CString::Transcode( ==> const char *fromcharset <== , const char *tocharset)");
		return 0;
	}
	if (!tocharset) {
		SetError(194,"int CString::Transcode(const char *fromcharset, ==> const char *tocharset <== )");
		return 0;
	}
	CIconv Iconv;
	if (!Iconv.Init(fromcharset,tocharset)) return 0;
	if (buffer==NULL || len==0) return Iconv.Transcode("",0,Target);
	return Iconv.Transcode(buffer,len,Target);
#endif
}

int CString::TranscodeText(const char *text, const char *fromcharset, const char *tocharset)
/*!\brief String von einem Charset in ein anderes umwandeln
 *
 * \header \#include <ppl6.h>
 * \desc
 * Sofern die "libiconv"-Library einkompiliert wurde, kann diese Funktion verwendet
 * werden, um den String von einem Character-Encoding in ein anders umzuwandeln.
 *
 * \param[in] text Pointer auf den zu transkodierenden String
 * \param[in] fromcharset Ein Pointer auf einen String, der den Namen des
 * Quell-Encodings enthält, beispielsweise "ISO-8859-1".
 * \param[in] tocharset Ein Pointer auf einen String, der den Namen des
 * Ziel-Encodings enthält, beispielsweise "UTF-8"
 * \return Im Erfolgsfall liefert die Funktion true (1) zurück, im Fehlerfall
 * false (0). Es können folgende Fehler auftreten_
 * - 2:   Nicht genug Speicher (ENOMEM)
 * - 194: Parameter darf nicht NULL sein
 * - 286: LibIconv ist nicht einkompiliert
 * - 287: Charset wird nicht unterstuetzt
 * - 288: Charset ist nicht initialisiert
 * - 289: Konvertierung fehlgeschlagen
 *
 * \see Siehe auch die Klasse CIconv
 * \since PPL Version 6.0.19
*/
{
	#ifndef HAVE_ICONV
		SetError(286);
		return 0;
	#else
		if (!text) {
			SetError(194,"int CString::Transcode( ==> char *text <===, const char *fromcharset, const char *tocharset)");
			return 0;
		}

		if (!fromcharset) {
			SetError(194,"int CString::Transcode(char *text, ==> const char *fromcharset <== , const char *tocharset)");
			return 0;
		}
		if (!tocharset) {
			SetError(194,"int CString::Transcode(char *text, const char *fromcharset, ==> const char *tocharset <== )");
			return 0;
		}
		CIconv Iconv;
		if (!Iconv.Init(fromcharset,tocharset)) return 0;
		char *ret=Iconv.Transcode((const char*)text);
		if (!ret) return 0;
		Set(ret);
		free(ret);
		return 1;
	#endif
}

int CString::TranscodeText(const CString &text, const char *fromcharset, const char *tocharset)
/*!\brief String von einem Charset in ein anderes umwandeln
 *
 * \header \#include <ppl6.h>
 * \desc
 * Sofern die "libiconv"-Library einkompiliert wurde, kann diese Funktion verwendet
 * werden, um den String von einem Character-Encoding in ein anders umzuwandeln.
 *
 * \param[in] text Der zu transkodierende String
 * \param[in] fromcharset Ein Pointer auf einen String, der den Namen des
 * Quell-Encodings enthält, beispielsweise "ISO-8859-1".
 * \param[in] tocharset Ein Pointer auf einen String, der den Namen des
 * Ziel-Encodings enthält, beispielsweise "UTF-8"
 * \return Im Erfolgsfall liefert die Funktion true (1) zurück, im Fehlerfall
 * false (0). Es können folgende Fehler auftreten_
 * - 2:   Nicht genug Speicher (ENOMEM)
 * - 194: Parameter darf nicht NULL sein
 * - 286: LibIconv ist nicht einkompiliert
 * - 287: Charset wird nicht unterstuetzt
 * - 288: Charset ist nicht initialisiert
 * - 289: Konvertierung fehlgeschlagen
 *
 * \see Siehe auch die Klasse CIconv
 * \since PPL Version 6.0.19
*/
{
	#ifndef HAVE_ICONV
		SetError(286);
		return 0;
	#else
		if (!fromcharset) {
			SetError(194,"int CString::Transcode(char *text, ==> const char *fromcharset <== , const char *tocharset)");
			return 0;
		}
		if (!tocharset) {
			SetError(194,"int CString::Transcode(char *text, const char *fromcharset, ==> const char *tocharset <== )");
			return 0;
		}
		CIconv Iconv;
		if (!Iconv.Init(fromcharset,tocharset)) return 0;
		CString s=Iconv.Transcode(text);
		if (s.IsEmpty()) return 0;
		Set(s);
		return 1;
	#endif
}


int CString::TranscodeText(const char *text, int size, const char *fromcharset, const char *tocharset)
/*!\brief String von einem Charset in ein anderes umwandeln
 *
 * \header \#include <ppl6.h>
 * \desc
 * Sofern die "libiconv"-Library einkompiliert wurde, kann diese Funktion verwendet
 * werden, um den String von einem Character-Encoding in ein anders umzuwandeln.
 *
 * \param[in] text Pointer auf den zu transkodierenden String
 * \param[in] size Die Größe des Strings in Bytes
 * \param[in] fromcharset Ein Pointer auf einen String, der den Namen des
 * Quell-Encodings enthält, beispielsweise "ISO-8859-1".
 * \param[in] tocharset Ein Pointer auf einen String, der den Namen des
 * Ziel-Encodings enthält, beispielsweise "UTF-8"
 * \return Im Erfolgsfall liefert die Funktion true (1) zurück, im Fehlerfall
 * false (0). Es können folgende Fehler auftreten_
 * - 2:   Nicht genug Speicher (ENOMEM)
 * - 194: Parameter darf nicht NULL sein
 * - 286: LibIconv ist nicht einkompiliert
 * - 287: Charset wird nicht unterstuetzt
 * - 288: Charset ist nicht initialisiert
 * - 289: Konvertierung fehlgeschlagen
 *
 * \see Siehe auch die Klasse CIconv
 * \since PPL Version 6.0.19
*/
{
	#ifndef HAVE_ICONV
		SetError(286);
		return 0;
	#else
		if (!text) {
			SetError(194,"int CString::Transcode( ==> char *text <===, const char *fromcharset, const char *tocharset)");
			return 0;
		}

		if (!fromcharset) {
			SetError(194,"int CString::Transcode(char *text, ==> const char *fromcharset <== , const char *tocharset)");
			return 0;
		}
		if (!tocharset) {
			SetError(194,"int CString::Transcode(char *text, const char *fromcharset, ==> const char *tocharset <== )");
			return 0;
		}
		CIconv Iconv;
		if (!Iconv.Init(fromcharset,tocharset)) return 0;
		char *ret=Iconv.Transcode((const char*)text, size);
		if (!ret) return 0;
		Set(ret);
		free(ret);
		return 1;
	#endif
}

CString operator+(const CString &str1, const CString& str2)
{
	//mstring to return, assign the string
	CString ms = str1;
	ms+=str2;
	return ms;
}

CString operator+(const char *str1, const CString& str2)
{
	//mstring to return, assign the string
	CString ms = str1;
	ms+=str2;
	return ms;
}

CString operator+(const CString &str1, const char *str2)
{
	//mstring to return, assign the string
	CString ms = str1;
	ms+=str2;
	return ms;
}


std::ostream& operator<<(std::ostream& s, const CString &str)
{
	return s.write((const char*)str,str.Size());
}



} // EOF namespace ppl6
