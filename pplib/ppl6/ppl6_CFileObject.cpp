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

#include <time.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_FILE_H
	#include <sys/file.h>
#endif
#ifdef HAVE_STDARG_H
	#include <stdarg.h>
#endif
#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif
#ifdef _WIN32
#include <io.h>
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#define popen _popen
#define pclose _pclose

#include <windows.h>
#ifdef DeleteFile
#undef DeleteFile
#endif
#endif
#include "ppl6.h"

namespace ppl6 {

#define COPYBYTES_BUFFERSIZE 1024*1024

/*!\class CFileObject
 * \ingroup PPLGroupFileIO
 * \brief Virtuelle Basisklasse für CFile und CMemFile
 *
 * Dies ist die virtuelle Basisklasse für die Dateizugriffsklasse CFile und die
 * Speicherzugriffsklasse CMemFile. Sie beinhaltet Funktionen, die unabhängig vom
 * tatsächlichen Speicherort sind, sowie virtuelle Funktionen, deren eigentliche
 * Implementierung in den abgeleiteten Klassen erfolgt.
 *
 * CFileObject ist eine allgemeine Klasse zum Zugriff auf Dateien, die sich entweder
 * auf einem Datenträger oder im Hauptspeicher des Rechners befinden können. Sie
 * bietet Funktionen, zum Öffnen, Schließen, Lesen und Schreiben, die im Wesentlichen
 * an die Funktionen der Libc angelehnt sind.
 *
 * \see
 * - CFile
 * - CMemFile
 *
 * \since
 * Mit Version 6.3.0 wurden die Fileklassen komplett überarbeitet und diese neue Klasse
 * als Basis für CFile und CMemFile eingeführt.
 *
 */

CFileObject::CFileObject()
/*!\brief Konstruktor der Klasse
 *
 * Dies ist der Konstruktor der Klasse. Da es sich um eine Basisklasse handelt, die selbst fast keine
 * Funktionalität hat, gibt es hier im Augenblick nichts zu tun.
 *
 */
{

}

CFileObject::~CFileObject()
/*!\brief Destruktor der Klasse
 *
 */
{

}

void CFileObject::SetFilenamef(const char *filename, ...)
/*!\brief Dateiname festlegen
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird der interne Dateiname festgelegt, der z.B. mit
 * GetFilename ausgelesen werden kann. Die Funktion wird intern auch von den
 * Open-Funktionen mit Dateinamen aufgerufen
 *
 * \param[in] filename Ein Formatstring oder der Dateiname
 * \param[in] ... beliebig viele Parameter, sofern \p filename ein Formatstring ist
 */
{
	if (!filename) {
		MyFilename.Clear();
		return;
	}
	va_list args;
	va_start(args, filename);
	MyFilename.VaSprintf(filename, args);
	va_end(args);
}

void CFileObject::SetFilename(const CString &filename)
/*!\brief Dateiname festlegen
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird der interne Dateiname festgelegt, der z.B. mit
 * GetFilename ausgelesen werden kann. Die Funktion wird intern auch von den
 * Open-Funktionen mit Dateinamen aufgerufen
 *
 * \param[in] filename Ein CString, der den Dateinamen enthält.
 */
{
	MyFilename=filename;
}

const char *CFileObject::GetFilename() const
/*!\brief Dateiname auslesen
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird der interne Dateiname ausgelesen. Dieser wird über
 * die Open-Funktionen oder die Funktion SetFilename festgelegt.
 *
 * \return Die Funktion liefert entweder den Dateinamen zurück oder "unknown"
 */
{
	if (MyFilename.Len()>0) return (const char*)MyFilename;
	return "unknown";
}

CString CFileObject::Filename() const
/*!\brief Dateiname auslesen
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird der interne Dateiname ausgelesen. Dieser wird über
 * die Open-Funktionen oder die Funktion SetFilename festgelegt.
 *
 * \return Die Funktion liefert entweder den Dateinamen zurück oder "unknown"
 */
{
	if (MyFilename.Len()>0) return MyFilename;
	return "unknown";
}


size_t CFileObject::Write (const void * source, size_t bytes, ppluint64 fileposition)
/*!\brief Daten schreiben
 *
 * Mit dieser Funktion wird ein beliebiger Speicherbereich auf den Datenträger
 * geschrieben. Die Funktion ist nicht virtuell und existiert nur in der Basisklasse.
 * Sie ruft die virtuellen Funktionen Seek und Fwrite auf, um den
 * eigentlichen Schreibvorgang durchzuführen.
 *
 * @param source Pointer auf den Speicherbereich, der geschrieben werden soll
 * @param bytes Anzahl zu schreibender Bytes
 * @param fileposition Position in der Datei, an der die Daten gespeichert werden solle
 * @return Bei Erfolg liefert die Funktion die Anzahl geschriebener Bytes zurück, im
 * Fehlerfall 0
 */
{
	if (!Seek(fileposition)) return 0;
	return Fwrite(source,1,bytes);
}

size_t CFileObject::Write (const void * source, size_t bytes)
/*!\brief Daten schreiben
 *
 * Mit dieser Funktion wird ein beliebiger Speicherbereich auf den Datenträger
 * geschrieben. Die Funktion ist nicht virtuell und existiert nur in der Basisklasse.
 * Sie ruft die virtuellen Funktionen Seek und Fwrite auf, um den
 * eigentlichen Schreibvorgang durchzuführen.
 *
 * @param source Pointer auf den Speicherbereich, der geschrieben werden soll
 * @param bytes Anzahl zu schreibender Bytes
 * @return Bei Erfolg liefert die Funktion die Anzahl geschriebener Bytes zurück, im
 * Fehlerfall 0
 */
{
	return Fwrite(source,1,bytes);
}

size_t CFileObject::Write (const CVar &object, size_t bytes)
/*!\brief Daten eines von CVar abgeleiteten Objekts schreiben
 *
 * Mit dieser Funktion wird der Speicherinhalt eines CVar-Objekts auf den Datenträger
 * geschrieben. Die Funktion ist nicht virtuell und existiert nur in der Basisklasse.
 * Sie ruft die virtuellen Funktionen Seek und Fwrite auf, um den
 * eigentlichen Schreibvorgang durchzuführen.
 *
 * @param object Das zu speichernde CVar Objekt
 * @param bytes Anzahl zu schreibender Bytes
 * @return Bei Erfolg liefert die Funktion die Anzahl geschriebener Bytes zurück, im
 * Fehlerfall 0
 *
 * \note Folgende CVar Datentypen werden derzeit unterstützt:
 * - CString
 * - CWString (Falls \p bytes verwendet wird, sollt es ein vielfaches von sizeof(wchar_t) sein
 * - CBinary
 */
{
	int t=object.DataType();
	if (t==CVar::CBINARY) {
			const CBinary &bin= static_cast<const CBinary&>(object);  // Objekt zu CBinary umwandeln
			if (bytes==0 || bytes>bin.Size()) bytes=bin.Size();
			return Fwrite(bin.GetPtr(),1,bytes);
	} else if (t==CVar::CSTRING) {
			const CString &str= static_cast<const CString&>(object);  // Objekt zu CString umwandeln
			if (bytes==0 || bytes>str.Size()) bytes=str.Size();
			return Fwrite((void*)str.GetPtr(),1,bytes);
	} else if (t==CVar::CWSTRING) {
			const CWString &wstr= static_cast<const CWString&>(object);  // Objekt zu CWString umwandeln
			if (bytes==0 || bytes>wstr.Size()) bytes=wstr.Size();
			return Fwrite((void*)wstr.GetBuffer(),1,bytes);
	}
	SetError(337);
	return 0;
}


size_t CFileObject::Read (void * target, size_t bytes, ppluint64 fileposition)
/*!\brief Daten lesen
 *
 * Mit dieser Funktion wird beliebiger Bereich der geöffneten Datei in den Hauptspeicher
 * geladen. Die Funktion ist nicht virtuell und existiert nur in der Basisklasse.
 * Sie ruft die virtuellen Funktionen Seek und Fread auf, um den eigentlichen
 * Lesevorgang durchzuführen.
 *
 * @param target Pointer auf den Speicherbereich, in den die gelesenen Daten geschrieben werden sollen.
 * Dieser muss zuvor vom Aufrufer allokiert worden sein und mindestens \p bytes gross sein.
 * @param bytes Anzahl zu lesender Bytes
 * @param fileposition Position in der Datei, an der die Daten gelesen werden sollen
 * @return Bei Erfolg liefert die Funktion die Anzahl gelesender Bytes zurück.
 * Wenn  ein Fehler  auftritt  oder  das
 * Dateiende erreicht ist, wird eine kleinere Zahl oder 0 zurückgegeben. Falls
 * das Dateiende erreicht wurde oder ein anderer Fehler aufgetreten ist,
 * kann der Fehlercode über den üblichen Weg ausgelesen werden.
 */
{
	if (!Seek(fileposition)) return 0;
	return Fread(target,1,bytes);
}

size_t CFileObject::Read (void * target, size_t bytes)
/*!\brief Daten lesen
 *
 * Diese Funktion liest \p bytes Bytes ab der aktuellen Position des Dateistroms
 * und speichert sie im Hauptspeicher an der duch \p target bestimmten Position.
 * Die Funktion ist nicht virtuell und existiert nur in der Basisklasse.
 * Sie ruft die virtuellen Funktionen Seek und Fread auf, um den eigentlichen
 * Lesevorgang durchzuführen.
 *
 * @param target Pointer auf den Speicherbereich, in den die gelesenen Daten geschrieben werden sollen.
 * Dieser muss zuvor vom Aufrufer allokiert worden sein und mindestens \p bytes gross sein.
 * @param bytes Anzahl zu lesender Bytes
 * @return Bei Erfolg liefert die Funktion die Anzahl gelesender Bytes zurück.
 * Wenn  ein Fehler  auftritt  oder  das
 * Dateiende erreicht ist, wird eine kleinere Zahl oder 0 zurückgegeben. Falls
 * das Dateiende erreicht wurde oder ein anderer Fehler aufgetreten ist,
 * kann der Fehlercode über den üblichen Weg ausgelesen werden.
 */
{
	return Fread(target,1,bytes);
}

size_t CFileObject::Read (CVar &target, size_t bytes)
/*!\brief Daten in ein Objekt einlesen
 *
 * Diese Funktion liest \p bytes Bytes ab der aktuellen Position des Dateistroms
 * und speichert sie im Objekt \p target.
 *
 * Die Funktion ist nicht virtuell und existiert nur in der Basisklasse.
 * Sie ruft die virtuellen Funktionen Seek und Fread auf, um den eigentlichen
 * Lesevorgang durchzuführen.
 *
 * @param target Das Objekt, in dem die gelesenen Daten gespeichert werden
 * sollen.
 * @param bytes Anzahl zu lesender Bytes
 * @return Bei Erfolg liefert die Funktion die Anzahl gelesender Bytes zurück.
 * Wenn  ein Fehler  auftritt  oder  das
 * Dateiende erreicht ist, wird eine kleinere Zahl oder 0 zurückgegeben. Falls
 * das Dateiende erreicht wurde oder ein anderer Fehler aufgetreten ist,
 * kann der Fehlercode über den üblichen Weg ausgelesen werden.
 *
 * \note Folgende CVar Datentypen werden derzeit unterstützt:
 * - CString
 * - CWString (\p bytes sollte hier ein vielfaches von sizeof(wchar_t) sein)
 * - CBinary
 *
 */
{
	if (!bytes) {
		SetError(453);		// Schreib- oder Lesevorgang mit 0 Bytes
		return 0;
	}
	void *buffer=malloc(bytes+4);
	if (!buffer) {
		SetError(2);
		return 0;
	}
	size_t b=Fread(buffer,1,bytes);
	if (!b) {
		free(buffer);
		return 0;
	}
	((char*)buffer)[b]=0;

	int err=0;
	if (b!=bytes) err=1;
	if (err) PushError();
	int t=target.DataType();
	if (t==CVar::CBINARY) {
			CBinary &bin= static_cast<CBinary&>(target);  // Objekt zu CBinary umwandeln
			bin.Set(buffer,b);
			bin.ManageMemory();
	} else if (t==CVar::CSTRING) {
			CString &str= static_cast<CString&>(target);  // Objekt zu CString umwandeln
			str.ImportBuffer((char*)buffer,bytes+4);
	} else if (t==CVar::CWSTRING) {
			CWString &wstr= static_cast<CWString&>(target);  // Objekt zu CWString umwandeln
			((char*)buffer)[b+1]=0;
			((char*)buffer)[b+2]=0;
			((char*)buffer)[b+3]=0;
			wstr.ImportBuffer((wchar_t*)buffer,bytes+4);
	} else {
		if (err) PopError();
		err=0;
		SetError(337);
		free(buffer);
		b=0;
	}
	if (err) PopError();
	return b;

}

ppluint64 CFileObject::Copy (CFileObject &quellfile, ppluint64 quelloffset, ppluint64 bytes, ppluint64 zieloffset)
/*!\brief Daten aus einer anderen Datei kopieren
 *
 * Mit dieser Funktion kann ein Datenblock aus einer anderen Datei in diese
 * hineinkopiert werden. Dabei werden \p bytes Bytes ab der Position \p quelloffset der
 * Quelldatei \p quellfile gelesen an die Position \p zieloffset dieser Datei kopiert.
 *
 * @param quellfile Das Dateiobjekt, aus dem gelesen werden soll
 * @param quelloffset Position innerhalb der Quelldatei, ab der die Daten gelesen werden sollen
 * @param bytes Anzahl zu kopierender Bytes
 * @param zieloffset Position in dieser Datei, an die die Daten geschrieben werden sollen
 * @return Bei Erfolg liefert die Funktion die Anzahl kopierter Bytes zurück.
 * Im Fehlerfall kann der Wert auch keiner als die Anzahl Bytes sein oder auch 0.
 *
 * \note Die Funktion verwendet einen internen Buffer zum Zwischenspeichern
 * der gelesenen Daten.
 */

{
	if (!quellfile.Seek(quelloffset)) return 0;
	if (!Seek(zieloffset)) return 0;
	return (DoCopy (quellfile,bytes));
}

ppluint64 CFileObject::Copy (CFileObject &quellfile, ppluint64 bytes)
/*!\brief Daten aus einer anderen Datei kopieren
 *
 * Mit dieser Funktion kann ein Datenblock aus einer anderen Datei in diese
 * hineinkopiert werden. Die Daten werden dabei ab dem aktuellen Dateipositionszeiger
 * des \p quellfile an den aktuellen Zeiger dieser Datei kopiert.
 *
 * @param quellfile Das Dateiobjekt, aus dem gelesen werden soll
 * @param bytes Anzahl zu kopierender Bytes
 * @return Bei Erfolg liefert die Funktion die Anzahl kopierter Bytes zurück.
 * Im Fehlerfall kann der Wert auch keiner als die Anzahl Bytes sein oder auch 0.
 *
 * \note Die Funktion verwendet einen internen Buffer zum Zwischenspeichern
 * der gelesenen Daten.
 */
{
	return (DoCopy (quellfile,bytes));
}

char * CFileObject::Gets (char *buffer, size_t num)
{
	return Fgets(buffer,num);
}

wchar_t* CFileObject::Getws (wchar_t *buffer, size_t num)
{
	return Fgetws(buffer,num);
}


int CFileObject::Gets (CString &buffer, size_t num)
/*!\brief String lesen
 *
 * Gets liest höchstens \p num minus ein Zeichen aus der Datei und speichert
 * sie im String-Objekt \p buffer. Das Lesen stoppt nach einem
 * EOF oder Zeilenvorschub. Wenn ein Zeilenvorschub gelesen wird, wird
 * er in dem Puffer gespeichert. Am Ende der gelesenen Daten wird ein
 * 0-Byte angehangen.
 *
 * @param buffer String-Objekt, in dem die gelesenen Daten gespeichert werden
 * sollen.
 * @param num Anzahl zu lesender Zeichen
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0. Der Inhalt von \p buffer
 * ist im Fehlerfall undefiniert.
 */
{
	if (!num) {
		SetError(194);
		return 0;
	}
	char *b=(char*)malloc(num+1);
	if (!b) {
		SetError(2);
		return 0;
	}
	char *ret=Fgets(b,num);
	if (ret==NULL) {
		PushError();
		free(b);
		buffer.Clear();
		PopError();
		return 0;
	}
	buffer.Set(b);
	free(b);
	return 1;
}

CString CFileObject::Gets (size_t num)
/*!\brief String lesen
 *
 * Gets liest höchstens \p num minus ein Zeichen aus der Datei und liefert deren
 * Inhalt als String-Objekt zurück. Das Lesen stoppt nach einem
 * EOF oder Zeilenvorschub. Wenn ein Zeilenvorschub gelesen wird, wird
 * er in dem Puffer gespeichert. Am Ende der gelesenen Daten wird ein
 * 0-Byte angehangen.
 *
 * @param num Anzahl zu lesender Zeichen
 * @return Die Funktion gibt ein CString-Objekt mit den gelesenen Daten zurück.
 * Im Fehlerfall ist das Objekt leer (CString::IsEmpty()).
 */
{
	CString s;
	Gets(s,num);
	return s;
}

int CFileObject::Getws (CWString &buffer, size_t num)
/*!\brief Wide-Character String lesen
 *
 * Gets liest höchstens \p num minus ein Zeichen aus der Datei und speichert
 * sie im Wide-Character-String-Objekt \p buffer. Das Lesen stoppt nach einem
 * WEOF oder Zeilenvorschub. Wenn ein Zeilenvorschub gelesen wird, wird
 * er in dem Puffer gespeichert. Am Ende der gelesenen Daten wird ein
 * 0-Byte angehangen.
 *
 * @param buffer Wide-Character-String-Objekt, in dem die gelesenen Daten gespeichert werden
 * sollen.
 * @param num Anzahl zu lesender Zeichen. Hierbei handelt es sich tatsächlich um
 * Zeichen, nicht um Bytes. Die Anzahl zu lesender Bytes wird intern mit der Formel
 * \p num * \c sizeof(wchar_t) errechnet.
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0. Der Inhalt von \p buffer
 * ist im Fehlerfall undefiniert.
 */
{
	if (!num) {
		SetError(194);
		return 0;
	}
	wchar_t *b=(wchar_t*)malloc((num+1)*sizeof(wchar_t));
	if (!b) {
		SetError(2);
		return 0;
	}
	wchar_t *ret=Fgetws(b,num);
	if (ret==NULL) {
		free(b);
		SetError(0);
		buffer.Clear();
		return 0;
	}
	buffer.Set(b);
	free(b);
	return 1;
}

CWString CFileObject::Getws (size_t num)
/*!\brief Wide-Character String lesen
 *
 * Gets liest höchstens \p num minus ein Zeichen aus der Datei liefert sie als
 * Wide-Character-String-Objekt zurück. Das Lesen stoppt nach einem
 * WEOF oder Zeilenvorschub. Wenn ein Zeilenvorschub gelesen wird, wird
 * er in dem Puffer gespeichert. Am Ende der gelesenen Daten wird ein
 * 0-Byte angehangen.
 *
 * @param num Anzahl zu lesender Zeichen. Hierbei handelt es sich tatsächlich um
 * Zeichen, nicht um Bytes. Die Anzahl zu lesender Bytes wird intern mit der Formel
 * \p num * \c sizeof(wchar_t) errechnet.
 * @return Die Funktion gibt ein CWString-Objekt mit den gelesenen Daten zurück.
 * Im Fehlerfall ist das Objekt leer (CString::IsEmpty()).
 */
{
	CWString s;
	Getws(s,num);
	return s;
}


int CFileObject::Putsf (const char *fmt, ... )
/*!\brief Formatierten String schreiben
 *
 * Putsf schreibt das Ergebnis nach Kontrolle des Formatierungsstrings \p fmt und
 * Einsetzen der optionalen Parameter ohne sein nachfolgendes 0-Byte in den Ausgabestrom.
 *
 * @param fmt Pointer auf den Formatierungsstring
 * @param ... Optionale Parameter, die im Formatierungsstring verwendet werden.
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 */
{
	if (!fmt) {
		SetError(194);
		return 0;
	}
	char *buff;
	va_list args;
	va_start(args, fmt);
	vasprintf (&buff, (char*)fmt, args);
	va_end(args);
	if (!buff) {
		SetError(2);
		return 0;
	}
	int ret=Puts(buff);
	free(buff);
	return ret;
}

int CFileObject::Puts (const CString &str)
/*!\brief String schreiben
 *
 * Diese Funktion schreibt den Inhalt des String-Objekts \p str
 * ohne sein nachfolgendes 0-Byte in den Ausgabestrom.
 *
 * @param str String-Objekt mit den zu schreibenden Daten
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 */
{
	return Puts((const char*)str);
}

int CFileObject::Putws (const CWString &str)
/*!\brief Wide-Character-String schreiben
 *
 * Diese Funktion schreibt den Inhalt des Wide-Character-String-Objekts \p str
 * ohne sein nachfolgendes 0-Byte in den Ausgabestrom.
 *
 * @param str String-Objekt mit den zu schreibenden Daten
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 */
{
	return Putws((const wchar_t*)str);
}

pplint64 CFileObject::Lof() const
/*!\brief Größe der geöffneten Datei
 *
 * Diese Funktion liefert die Größe der geöffneten Datei in Bytes zurück.
 * \return Größe der Datei in Bytes, oder -1, wenn ein Fehler aufgetreten ist.
 *
 * \note Die Funktion ruft selbst nur die Funktion CFileObject::Size auf, die daher
 * aus Geschwindigkeitsgründen vorzuziehen ist.
 */
{
	return Size();
}

const char *CFileObject::Map()
/*!\brief Datei in den Speicher mappen
 *
 * Mit dieser Funktion wird der komplette Inhalt der Datei in den Speicher gemapped.
 * Falls das Betriebssystem <a href="http://en.wikipedia.org/wiki/Mmap">mmap</a> versteht,
 * wird dieser verwendet. Dabei wird die Datei nicht sofort komplett in den Speicher
 * geladen, sondern nur die Teile, die gerade benötigt werden. Steht \c mmap nicht
 * zur Verfügung, wird die Datei in den Hauptspeicher geladen. Die File-Klasse kümmert
 * sich selbst daraum, dass der Speicher auch wieder freigegeben wird.
 *
 * Ein mit Map gemappter Speicher darf nur gelesen, aber nicht beschrieben werden!
 *
 * @return Bei Erfolg gibt die Funktion einen Pointer auf den Speicherbereich zurück,
 * in dem sich die Datei befindet, im Fehlerfall NULL.
 */
{
	return Map(0,(size_t)Lof());
}

char *CFileObject::Load()
/*!\brief Den kompletten Inhalt der Datei laden
 *
 * Mit dieser Funktion wird der komplette Inhalt der geöffneten Datei in den
 * Hauptspeicher geladen. Der benötigte Speicher wird von der Funktion
 * automatisch allokiert und muss vim Aufrufer nach Gebrauch mit \c free wieder
 * freigegeben werden.
 *
 * @return Pointer auf den Speicherbereich mit dem Inhalt der Datei. Dieser muss
 * vom Aufrufer nach Gebrauch mit \c free selbst wieder freigegeben werden.
 * Im Fehlerfall wird NULL.
 */
{
	ppluint64 s=Lof();
	char *b=(char*)malloc((size_t)s+1);
	if (!b) {
		SetError(2);
		return NULL;
	}
	ppluint64 r=Read(b,(size_t)s,0);
	if (r!=s) {
		free(b);
		return NULL;
	}
	b[s]=0;
	return b;
}

int CFileObject::Load(CVar &object)
/*!\brief Den kompletten Inhalt der Datei in ein Objekt laden
 *
 * Mit dieser Funktion wird der komplette Inhalt der geöffneten Datei in das
 * angegebene Objekt geladen. Unterstützt werden zur Zeit folgende Objekte:
 * - CString
 * - CWString
 * - CBinary
 *
 * @param[out] object Das gewünschte Zielobjekt
 * @return Liefert 1 zurück, wenn der Inhalt geladen werden konnte, sonst 0.
 */
{
	if (!IsOpen()) {
		SetError(72);
		return 0;
	}
	ppluint64 size=Lof();
	Seek(0);
	char *buffer=(char*)malloc((size_t)size+1);
	if (!buffer) {
		SetError(2);
		return 0;
	}
	size_t by=Fread(buffer,1,(size_t)size);
	if (by!=size) {
		free(buffer);
		return 0;
	}
	buffer[by]=0;
	int t=object.DataType();
	if (t==CVar::CBINARY) {
			CBinary &bin= static_cast< CBinary&>(object);  // Objekt zu CBinary umwandeln
			bin.Set(buffer,by);
			bin.ManageMemory();
			return 1;
	} else if (t==CVar::CSTRING) {
			CString &str= static_cast<CString&>(object);  // Objekt zu CString umwandeln
			str.ImportBuffer((char*)buffer,by+1);
			return 1;
	} else if (t==CVar::CWSTRING) {
			CWString &wstr= static_cast<CWString&>(object);  // Objekt zu CWString umwandeln
			wstr.ImportBuffer((wchar_t*)buffer,by+1);
			return 1;
	}
	free(buffer);
	SetError(337);
	return 0;
}


// Virtuelle Funktionen

int CFileObject::Close ()
/*!\brief Datei schließen
 *
 * Diese Funktion schließt die aktuell geöffnete Datei. Sie wird automatisch vom Destruktor der
 * Klasse aufgerufen, so dass ihr expliziter Aufruf nicht erforderlich ist.
 *
 * Wenn  der  Stream  zur  Ausgabe  eingerichtet  war,  werden  gepufferte  Daten  zuerst  durch CFileObject::Flush
 * geschrieben. Der zugeordnete Datei-Deskriptor wird geschlossen.
 *
 * \return Bei  Erfolg  wird  1  zurückgegeben, im Fehlerfall 0.
 * In jedem Fall ist kein weiterer Zugriff auf den Stream möglich.
 */
{
	SetError(180);
	return 0;
}

int CFileObject::Seek (ppluint64 position)
/*!\brief Dateizeiger auf gewünschte Stelle bringen
 *
 * Diese Funktion bewegt den internen Dateizeiger auf die gewünschte Stelle
 *
 * \param[in] position Gewünschte Position innerhalb der Datei
 * \return Liefert 1 zurück, wenn der Dateizeiger erfolgreich auf die gewünschte
 * Position bewegt werden konnte, sonst 0. Bei Auftreten eines Fehlers kann
 * sich die Dateiposition dennoch geändert haben und sollte daher mit CFileObject::Ftell
 * abgefragt werden.
 */
{
	SetError(180);
	return 0;
}

int CFileObject::Fseek (ppluint64 offset, int origin)
/*!\brief Dateizeiger auf gewünschte Stelle bringen
 *
 * Die Funktion Fseek setzt den Dateipositionszeiger für den Stream. Die neue Position,
 * gemessen in Byte, wird erreicht durch addieren von  \p offset  zu  der  Position,  die  durch  \p origin
 * angegeben  ist. Wenn \p origin auf SEEK_SET, SEEK_CUR, oder SEEK_END, gesetzt ist, ist der Offset relativ
 * zum Dateianfang, der aktuellen Position, oder dem Dateiende.
 *
 * Ein  erfolgreicher  Aufruf  der  Funktion fseek  löscht  den Dateiendezeiger für den Stream.
 *
 * @param offset Anzahl Bytes, die gesprungen werden soll.
 * @param origin Gibt die Richtung an, in die der Dateizeiger bewegt werden soll. Es kann einen
 * der folgenden Werte annehmen:
 * - SEEK_SET \p offset wird vom Beginn der Datei berechnet
 * - SEEK_CUR \p offset wird von der aktuellen Dateiposition gerechnet
 * - SEEK_END \p offset wird vom Ende der Datei aus nach vorne berechnet
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	SetError(180);
	return 0;
}

pplint64 CFileObject::Ftell()
/*!\brief Aktuelle Dateiposition ermitteln
 *
 * Die Funktion Ftell liefert den aktuellen Wert des Dateipositionszeigers für  den  Stream zurück.
 *
 * @return Position des Zeigers innerhalb der Datei. Im Fehlerfall wird -1 zurückgegeben.
 */
{
	SetError(180);
	return -1;
}

size_t CFileObject::Fread(void * ptr, size_t size, size_t nmemb)
/*!\brief Lesen eines Datenstroms
 *
 * Die  Funktion  Fread  liest \p nmemb Datenelemente vom Dateistrom und speichert
 * es an  der  Speicherposition,  die  durch \p ptr bestimmt ist.  Jedes davon ist
 * \ size Byte lang.
 *
 * @param[out] ptr Pointer auf den Speicherbereich, in den die gelesenen Daten
 * abgelegt werden sollen. Der Aufrufer muss vorher mindestens \p size * \p nmemb
 * Bytes Speicher reserviert haben.
 * @param[in] size Größe der zu lesenden Datenelemente
 * @param[in] nmemb Anzahl zu lesender Datenelemente
 * @return Fread  gibt die Anzahl der erfolgreich gelesenen Elemente zurück
 * (nicht die Anzahl  der  Zeichen).   Wenn  ein Fehler  auftritt  oder  das
 * Dateiende erreicht ist, wird eine kleinere Zahl oder 0 zurückgegeben. Falls
 * das Dateiende erreicht wurde oder ein anderer Fehler aufgetreten ist,
 * kann der Fehlercode über den üblichen Weg ausgelesen werden.
 */
{
	SetError(180);
	return 0;
}

size_t CFileObject::Fwrite(const void * ptr, size_t size, size_t nmemb)
/*!\brief Schreiben eines Datenstroms
 *
 * Die Funktion Fwrite schreibt \p nmemb Datenelemente der Größe \p size Bytes,
 * in  den  Dateistrom. Sie werden von der Speicherstelle, die durch \p ptr angegeben ist, gelesen.
 *
 * @param ptr Pointer auf den Beginn des zu schreibenden Speicherbereiches.
 * @param size Größe der zu schreibenden Datenelemente
 * @param nmemb Anzahl zu schreibender Datenelemente
 * @return Fwrite gibt die Anzahl der erfolgreich geschriebenen Elemente zurück (nicht die
 * Anzahl der Zeichen). Wenn ein Fehler auftritt, wird eine kleinere Zahl oder 0 zurückgegeben.
 * Der Fehler kann auf dem üblichen Weg ausgelesen werden.
 *
 */
{
	SetError(180);
	return 0;
}


ppluint64 CFileObject::DoCopy (CFileObject &quellfile, ppluint64 bytes)
/*!\brief Daten aus einer anderen Datei kopieren
 *
 * Mit dieser Funktion kann ein Datenblock aus einer anderen Datei in diese
 * hineinkopiert werden. Die Daten werden dabei ab dem aktuellen Dateipositionszeiger
 * des \p quellfile an den aktuellen Zeiger dieser Datei kopiert.
 *
 * @param quellfile Das Dateiobjekt, aus dem gelesen werden soll
 * @param bytes Anzahl zu kopierender Bytes
 * @return Bei Erfolg liefert die Funktion die Anzahl kopierter Bytes zurück.
 * Im Fehlerfall kann der Wert auch keiner als die Anzahl Bytes sein oder auch 0.
 *
 * \note Die Funktion verwendet einen internen Buffer zum Zwischenspeichern
 * der gelesenen Daten.
 */
{
	SetError(180);
	return 0;
}

char *CFileObject::Fgets (char *buffer, size_t num)
/*!\brief String lesen
 *
 * Gets liest höchstens \p num minus ein Zeichen aus der Datei und speichert
 * sie in dem Puffer, auf den \p buffer zeigt. Das Lesen stoppt nach einem
 * EOF oder Zeilenvorschub. Wenn ein Zeilenvorschub gelesen wird, wird
 * er in dem Puffer gespeichert. Am Ende der gelesenen Daten wird ein
 * 0-Byte angehangen.
 *
 * @param buffer Pointer auf den Speicherbereich, in den die gelesenen Daten
 * geschrieben werden sollen. Dieser muss vorher vom Aufrufer allokiert worden
 * sein und mindestens \p num Bytes groß sein.
 * @param num Anzahl zu lesender Zeichen
 * @return Bei Erfolg wird \p buffer zurückgegeben, im Fehlerfall NULL.
 */
{
	SetError(180);
	return NULL;
}

wchar_t *CFileObject::Fgetws (wchar_t *buffer, size_t num)
/*!\brief Wide-Character String lesen
 *
 * Gets liest höchstens \p num minus ein Zeichen (nicht Bytes) aus der Datei
 * und speichert sie in dem Puffer, auf den \p buffer zeigt. Das Lesen stoppt
 * nach einem EOF oder Zeilenvorschub. Wenn ein Zeilenvorschub gelesen wird,
 * wird er in dem Puffer gespeichert. Am Ende der gelesenen Daten wird ein
 * 0-Byte angehangen.
 *
 * @param buffer Pointer auf den Speicherbereich, in den die gelesenen Daten
 * geschrieben werden sollen. Dieser muss vorher vom Aufrufer allokiert worden
 * sein und mindestens \p num * \c sizeof(wchar_t) Bytes groß sein.
 * @param num Anzahl zu lesender Zeichen
 * @return Bei Erfolg wird \p buffer zurückgegeben, im Fehlerfall NULL.
 *
 * \note Die Funktion ist unter Umständen nicht auf jedem Betriebssystem
 * verfügbar. In diesem Fall wird Fehlercode 246 zurückgegeben.
 */
{
	SetError(180);
	return NULL;
}
int CFileObject::Puts (const char *str)
/*!\brief String schreiben
 *
 * Puts schreibt die Zeichenkette \p str ohne sein nachfolgendes 0-Byte in
 * den Ausgabestrom.
 *
 * @param str Pointer auf den zu schreibenden String
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 */
{
	SetError(180);
	return 0;
}

int CFileObject::Putws (const wchar_t *str)
/*!\brief Wide-Character String schreiben
 *
 * Puts schreibt die Zeichenkette \p str ohne sein nachfolgendes 0-Byte in
 * den Ausgabestrom.
 *
 * @param str Pointer auf den zu schreibenden String
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 *
 * \note Die Funktion ist unter Umständen nicht auf jedem Betriebssystem
 * verfügbar. In diesem Fall wird Fehlercode 246 zurückgegeben.
 *
 */
{
	SetError(180);
	return 0;
}


int CFileObject::Putc (int c)
/*!\brief Zeichen schreiben
 *
 * Putc schreibt das Zeichen \p c, umgesetzt in ein unsigned char,
 * in den Ausgabestrom.
 * @param c Zu schreibendes Zeichen
 * @return Bei Erfolg wird das geschriebende Zeichen als Integer Wert zurückgegeben,
 * im Fehlerfall -1;
 */
{
	SetError(180);
	return -1;
}

int CFileObject::Getc()
/*!\brief Zeichen lesen
 *
 * Getc liest das  nächste Zeichen aus der Datei und gibt seinen unsigned char Wert gecastet
 * in einem int zurück.
 * @return Bei Erfolg wird der Wert des gelesenen Zeichens zurückgegeben, im
 * Fehlerfall -1.
 */
{
	SetError(180);
	return -1;
}

int CFileObject::Putwc (wchar_t c)
/*!\brief Wide-Character Zeichen schreiben
 *
 * Putwc schreibt das Wide-Character Zeichen \p c in den Ausgabestrom.
 * @param c Zu schreibendes Zeichen
 * @return Bei Erfolg wird das geschriebende Zeichen als Integer Wert zurückgegeben,
 * im Fehlerfall -1;
 *
 * \note Die Funktion ist unter Umständen nicht auf jedem Betriebssystem
 * verfügbar. In diesem Fall wird Fehlercode 246 zurückgegeben.
 */
{
	SetError(180);
	return -1;
}

int CFileObject::Getwc()
/*!\brief Wide-Character Zeichen lesen
 *
 * Getwc liest das nächste Zeichen aus der Datei und gibt seinen Wert als Integer
 * zurück.
 * @return Bei Erfolg wird das gelesene Zeichen als Integer Wert zurückgegeben,
 * im Fehlerfall -1;
 *
 * \note Die Funktion ist unter Umständen nicht auf jedem Betriebssystem
 * verfügbar. In diesem Fall wird Fehlercode 246 zurückgegeben.
 */
{
	SetError(180);
	return -1;
}

bool CFileObject::Eof() const
/*!\brief Prüfen, ob Dateiende erreicht ist
 *
 * Die Funktion prüft, ob das Dateiende erreicht wurde
 *
 * @return Liefert \c true zurück, wenn das Dateiende erreicht wurde, sonst \c false
 * Falls die Datei nicht geöffnet war, wird ebenfalls \c false zurückgegeben.
 */
{
	SetError(180);
	return false;
}

pplint64 CFileObject::Size() const
/*!\brief Größe der geöffneten Datei
 *
 * Diese Funktion liefert die Größe der geöffneten Datei in Bytes zurück.
 * \return Größe der Datei in Bytes, oder -1, wenn ein Fehler aufgetreten ist.
 *
 * \note Die Funktion ist CFileObject::Lof vorzuziehen.
 */
{
	SetError(180);
	return -1;
}

int CFileObject::GetFileNo() const
/*!\brief Filenummer der Datei
 *
 * Die Funktion liefert den Dateideskriptor als Integer zurück, wie er
 * von den Systemfunktionen open , read , write und close genutzt wird.
 *
 * @return Liefert die Filenummer zurück, oder -1, wenn die Datei nicht
 * geöffnet war.
 */
{
	SetError(180);
	return 0;
}

int CFileObject::Flush()
/*!\brief Gepufferte Daten schreiben
 *
 * Die Funktion Flush bewirkt, dass alle gepufferten Daten des aktuellen Streams
 * mittels der zugrundeliegenden write-Funktion geschrieben werden. Der Status
 * des Streams wird dabei nicht berührt. Die Daten werden nicht zwangsweise auch
 * physikalisch auf die Platte geschrieben, sie können noch immer aus Performancegründen
 * vom Kernel oder Treiber gecached werden. Um 100 Prozent sicher zu gehen, kann man
 * die Funktion CFileObject::Sync verwenden.
 *
 * @return Bei erfolgreicher Ausführung wird 1 zurückgegeben, ansonsten 0.
 */
{
	SetError(180);
	return 0;
}

int CFileObject::Sync()
/*!\brief Dateiänderungen sofort auf die Platte schreiben
 *
 * \desc
 * Für gewöhnlich cached das Betriebssysteme Schreibzugriffe auf die Festplatte, um die Performance
 * zu steigern. Je nach Filesystem und Betriebssystem können zwischen Schreibzugriff der Software bis zum
 * tatsächlichen Schreiben auf die Festplatte zwischen einigen wenigen Sekunden bis zu einer Minute vergehen!
 * Tritt in diesem Zeitraum ein System-Crash oder Stromausfall auf, führt dies unweigerlich zu Datenverlust.
 *
 * Ein Aufruf dieser Funktion bewirkt, dass alle Dateiänderungen sofort auf die Festplatte
 * geschrieben werden. Sie sollte daher vor dem Schließen einer kritischen Datei mit CFile::Close aufgerufen
 * werden, unter Umständen sogar nach jedem Schreibzugriff.
 *
 * @return Die Funktion kehrt erst zurück, wenn alle Daten vollständig geschrieben wurden und liefert dann true (1)
 * zurück. Können die Daten nicht geschrieben werden, liefert sie false (0) zurück und ein entsprechender
 * Fehlercode wird gesetzt.
 *
 * @since Die Funktion wurde mit Version 6.2.5 eingeführt
 */
{
	SetError(180);
	return 0;
}

int CFileObject::Truncate(ppluint64 length)
/*!\brief Datei abschneiden
 *
 * Die Funktionen Truncate bewirkt, dass die aktuell geöffnete Datei auf eine Größe von
 * exakt \p length Bytes abgeschnitten wird.
 *
 * Wenn die Datei vorher größer war, gehen überschüssige Daten verloren. Wenn die Datei
 * vorher kleiner war, wird sie vergrößert und die zusätzlichen Bytes werden als Nullen geschrieben.
 *
 * Der Dateizeiger wird nicht verändert. Die Datei muss zum Schreiben geöffnet sein.
 *
 * @param length Position, an der die Datei abgeschnitten werden soll.
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	SetError(180);
	return 0;
}

bool CFileObject::IsOpen() const
/*!\brief Prüfen, ob eine Datei geöffnet ist
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion kann geprüft werden, ob die mit CFile assoziierte Datei
 * gerade geöffnet ist.
 * \return Die Funktion liefert \p true zurück, wenn die Datei offen ist, ansonsten \p false.
 */
{
	SetError(180);
	return false;
}

int CFileObject::LockShared(bool block)
/*!\brief Datei zum Lesen sperren
 *
 * Mit LockShared wird die Datei zum Lesen gesperrt. Andere Prozesse können weiterhin
 * auf die Datei zugreifen, allerdings ebenfalls nur lesend.
 *
 * @param block Gibt an, ob die Funktion warten soll (blocken), bis die Datei
 * gesperrt werden kann (block=true) oder sofort mit einer Fehlermeldung
 * zurückkehren soll (block=false).
 * @return Bei Erfolg liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \see Siehe auch CFileObject::LockExclusive und CFileObject::Unlock
 */
{
	SetError(180);
	return 0;
}

int CFileObject::LockExclusive(bool block)
/*!\brief Datei exklusiv sperren
 *
 * Mit LockExclusive wird die Datei exklusiv zum Schreiben gesperrt. Andere
 * Prozesse können nicht auf die Datei zugreifen, solange die Sperre besteht.
 *
 * @param block Gibt an, ob die Funktion warten soll (blocken), bis die Datei
 * gesperrt werden kann (block=true) oder sofort mit einer Fehlermeldung
 * zurückkehren soll (block=false).
 * @return Bei Erfolg liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \see Siehe auch CFileObject::LockShared und CFileObject::Unlock
 */
{
	SetError(180);
	return 0;
}

int CFileObject::Unlock()
/*!\brief Dateisperre aufheben
 *
 * Mit Unlock wird eine mit LockShared oder LockExclusive eingerichtete
 * Sperre wieder aufgehoben, so dass auch andere Prozesse wieder uneingeschränkt
 * auf die Datei zugreifen können.
 *
 * @return Bei Erfolg liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \see Siehe auch CFileObject::LockShared und CFileObject::LockExclusive
 */
{
	SetError(180);
	return 0;
}

void CFileObject::SetMapReadAhead(size_t bytes)
/*!\brief Minimalgröße des Speicherblocks bei Zugriffen mit CFileObject::Map
 *
 * Falls mit Map viele aufeinanderfolgende kleine Speicherblöcke gemapped werden,
 * ist es sinnvoll größere Blöcke zu laden, die dann bereits im Cache bzw. Hauptspeicher
 * liegen, wenn sie gebraucht werden. Mit dieser Funktion kann bestimmt werden, wie
 * viele Daten im Voraus gemapped werden sollen.
 *
 * @param bytes Anzahl Bytes, die im Voraus gemapped werden sollen.
 */
{
}

const char *CFileObject::Map(ppluint64 position, size_t size)
/*!\brief Datei Read-Only in den Speicher mappen
 *
 * \descr
 * Mit dieser Funktion wird ein Teil der Datei in den Speicher gemapped.
 * Falls das Betriebssystem <a href="http://en.wikipedia.org/wiki/Mmap">mmap</a> versteht,
 * wird dieser verwendet. Dabei wird der gewünschte Datenblock nicht sofort komplett
 * in den Speicher geladen, sondern nur der Teil, auf den gerade zugegriffen wird.
 * Steht \c mmap nicht zur Verfügung, wird die Datei in den Hauptspeicher geladen.
 * Die File-Klasse kümmert sich selbst daraum, dass der Speicher auch wieder freigegeben
 * wird.
 * \par
 * Ein mit Map gemappter Speicher darf nur gelesen, aber nicht beschrieben werden! Falls
 * auch geschrieben werden soll, ist die Funktion CFileObject::MapRW zu verwenden.
 *
 * @param[in] position Die gewünschte Startposition innerhalb der Datei
 * @param[in] size Die Anzahl Bytes, die gemapped werden sollen.
 * @return Bei Erfolg gibt die Funktion einen Pointer auf den Speicherbereich zurück,
 * in dem sich die Datei befindet, im Fehlerfall NULL.
 */
{
	SetError(180);
	return NULL;
}

char *CFileObject::MapRW(ppluint64 position, size_t size)
/*!\brief Datei Les- und Schreibbar in den Speicher mappen
 *
 * \descr
 * Mit dieser Funktion wird ein Teil der Datei in den Speicher gemapped.
 * Falls das Betriebssystem <a href="http://en.wikipedia.org/wiki/Mmap">mmap</a> versteht,
 * wird dieser verwendet. Dabei wird der gewünschte Datenblock nicht sofort komplett
 * in den Speicher geladen, sondern nur der Teil, auf den gerade zugegriffen wird.
 * Steht \c mmap nicht zur Verfügung, wird die Datei in den Hauptspeicher geladen.
 * Die File-Klasse kümmert sich selbst daraum, dass der Speicher nach Gebrauch wieder
 * zurück in die Datei geschrieben und freigegeben wird.
 * \par
 * Ein mit MapRW gemappter Speicher darf sowohl gelesen als auch beschrieben werden!
 * Bevor mit anderen Funktionen auf den gleichen Speicher zugegriffen werden soll
 * (insbesondere schreibend), muss die Funktion CFileObject::Unmap aufgerufen werden.
 *
 * @param[in] position Die gewünschte Startposition innerhalb der Datei
 * @param[in] size Die Anzahl Bytes, die gemapped werden sollen.
 * @return Bei Erfolg gibt die Funktion einen Pointer auf den Speicherbereich zurück,
 * in dem sich die Datei befindet, im Fehlerfall NULL.
 */
{
	SetError(180);
	return NULL;
}

void CFileObject::Unmap()
{
	return;
}

CString CFileObject::MD5Sum()
{
	char result[40];
	MD5_CTX ctx;
	MD5Init(&ctx);
	Seek(0);
	ppluint64 pos=0,bytes, buffersize=1024*1024;
	ppluint64 rest=Size();
	while (rest>0) {
		bytes=buffersize;
		if (bytes>rest) bytes=rest;
		const unsigned char *buffer=(const unsigned char *)Map(pos,(size_t)bytes);
		if (!buffer) return CString();
		MD5Update(&ctx,buffer,bytes);
		rest-=bytes;
		pos+=bytes;
	}
	MD5End(&ctx, result);
	return CString(result);
}

} // end of namespace ppl6
