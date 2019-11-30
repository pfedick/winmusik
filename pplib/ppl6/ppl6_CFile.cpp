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

//#define WIN32FILES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_WCHAR_H
	#include <wchar.h>
#endif
#include <time.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef HAVE_SYS_MMAN_H
	#include <sys/mman.h>
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

#ifdef CopyFile
#undef CopyFile
#endif


#ifdef MoveFile
#undef MoveFile
#endif

#endif
#include "ppl6.h"

namespace ppl6 {

#define COPYBYTES_BUFFERSIZE 1024*1024

/*!\class CFile
 * \ingroup PPLGroupFileIO
 * \brief Dateizugriff
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Klasse können Dateien geladen, verändert und gespeichert werden.
 * Sie dient als Wrapper-Klasse für die Low-Level Funktionen des Betriebssystems.
 *
 *
 * \section ppl6_CFile_Filemodi Dateimodi
 * Die CFile-Klasse unterstützt folgende Dateimodi:
 * <ul>
<li><b>r</b><br>Open text file for reading.  The stream is positioned at the beginning of the file.
</li>
<li><b>r+</b><br>Open for reading and writing.  The stream is positioned at the beginning of the file.
</li>
<li><b>w</b><br>Truncate  file to zero length or create text file for writing.  The stream is positioned at the beginning
of the file.
</li>
<li><b>w+</b><br>
Open for reading and writing.  The file is created if it does not exist, otherwise it is truncated. The
stream is positioned at the beginning of the file.
</li>
<li><b>a</b><br>Open  for  appending  (writing at end of file).  The file is created if it does not exist.
The stream is positioned at the end of the file.
</li>
<li><b>a+</b><br>Open for reading and appending (writing at end of file).  The file is
created if it does not exist.   The  initial  file  position for reading is at the beginning of
the file, but output is always appended to the end of the file.
</li>
<li><b>b</b> (Optionaler Zusatzparameter)<br>
The mode string can also include the letter ‘‘b’’ either as a last character or as a character between the char-
       acters in any of the two-character strings described above.  This is strictly for compatibility with C89 and has
       no effect; the ‘‘b’’ is ignored on all POSIX conforming systems, including Linux.  (Other systems may treat text
       files  and  binary files differently, and adding the ‘‘b’’ may be a good idea if you do I/O to a binary file and
       expect that your program may be ported to non-Unix environments.)
       </li>
</ul>
 *
 */

CFile::CFile ()
//! \brief Konstruktor der Klasse
{
	#ifdef USEWIN32
		hf=0;
	#endif
	MapBase=NULL;
	ff=NULL;
	size=pos=0;
	buffer=NULL;
	LastMapStart=LastMapSize=0;
	LastMapProtection=0;
	ReadAhead=0;
	isPopen=false;
}

CFile::CFile (const char * filename, const char * mode)
/*!\brief Konstruktor der Klasse mit gleichzeitigem Öffnen einer Datei
 *
 * Konstruktor der Klasse, mit dem gleichzeitig eine Datei geöffnet wird.
 * @param[in] filename Name der zu öffnenden Datei
 * @param[in] mode Zugriffsmodus. Defaultmäßig wird die Datei zum binären Lesen
 * geöffnet (siehe \ref ppl6_CFile_Filemodi)
 */
{
	MapBase=NULL;
	#ifdef USEWIN32
		hf=0;
	#endif
	ff=NULL;
	size=pos=0;
	buffer=NULL;
	LastMapStart=LastMapSize=0;
	LastMapProtection=0;
	ReadAhead=0;
	isPopen=false;
	Open (filename,mode);
}

CFile::CFile (const CString &filename, const char * mode)
/*!\brief Konstruktor der Klasse mit gleichzeitigem Öffnen einer Datei
 *
 * Konstruktor der Klasse, mit dem gleichzeitig eine Datei geöffnet wird.
 * @param[in] filename Name der zu öffnenden Datei
 * @param[in] mode Zugriffsmodus. Defaultmäßig wird die Datei zum binären Lesen
 * geöffnet (siehe \ref ppl6_CFile_Filemodi)
 */
{
	MapBase=NULL;
	#ifdef USEWIN32
		hf=0;
	#endif
	ff=NULL;
	size=pos=0;
	buffer=NULL;
	LastMapStart=LastMapSize=0;
	LastMapProtection=0;
	ReadAhead=0;
	isPopen=false;
	Open (filename.GetPtr(),mode);
}


CFile::CFile (FILE * handle)
//! \brief Konstruktor der Klasse mit Übernahme eines C-Filehandles einer bereits geöffneten Datei
{
	MapBase=NULL;
	ff=NULL;
	buffer=NULL;
	size=pos=0;
	LastMapStart=LastMapSize=0;
	LastMapProtection=0;
	ReadAhead=0;
	isPopen=false;
	if (handle!=NULL) {
		if (ff!=NULL) Close();
		ff=handle;
		size=Lof();
		Seek(0);
	}
}

CFile::~CFile()
//! \brief Destruktor der Klasse
{
	if (ff!=NULL) {
		PushError();
		Close();
		PopError();
	}
}

int CFile::Open (const CString &filename, const char * mode)
/*!\brief Datei öffnen
 *
 * Mit dieser Funktion wird eine Datei zum Lesen, Schreiben oder beides geöffnet.
 * @param[in] filename Dateiname
 * @param mode String, der angibt, wie die Datei geöffnet werden soll (siehe \ref ppl6_CFile_Filemodi)
 * @return Bei Erfolg liefert die Funktion True (1) zurück, sonst false (0).
 */
{
	Close();
	if (filename.IsEmpty()) {
		SetError(61);
		ff=NULL;
		size=0;
		pos=0;
		return 0;
	}

#ifdef WIN32
	CWString wideFilename=filename;
	CWString wideMode=mode;

	if ((ff=(FILE*)_wfopen((const wchar_t *)wideFilename,(const wchar_t*)wideMode))==NULL) {

#else
	if ((ff=(FILE*)fopen((const char*)filename,mode))==NULL) {
#endif
		SetError(TranslateErrno(errno),errno,"%s",(const char*)filename);
		ff=NULL;
		size=0;
		pos=0;
		return 0;
	}
	//#endif
	size=Lof();
	Seek(0);
	SetFilename(filename);
	return 1;
}

int CFile::Openf (const char * format, const char * mode, ...)
/*!\brief Datei zum Lesen oder Schreiben öffnen
 *
 * Mit dieser Funktion wird eine Datei zum Lesen, Schreiben oder beides geöffnet.
 *
 * \param format Formatstring für den Dateinamen
 * \param mode String, der angibt, wie die Datei geöffnet werden soll (siehe \ref ppl6_CFile_Filemodi)
 * \param ... Optionale Parameter für den Formatstring
 *
 * \return Bei Erfolg liefert die Funktion True (1) zurück, sonst false (0).
 */
{
	ppl6::CString f;
	va_list args;
	va_start(args, mode);
	f.VaSprintf(format,args);
	va_end(args);
	return Open(f,mode);
}

int CFile::OpenTempf(const char *filetemplate, ...)
/*!\brief Eine temporäre Datei zum Lesen und Schreiben öffnen
 *
 * Diese Funktion erzeugt eine temporäre Datei mit einem eindeutigen Namen.
 * Dieser Name wird aus \p filetemplate erzeugt. Dazu  müssen  die letzten
 * sechs  Buchstaben  des  Parameters template XXXXXX sein, diese werden dann
 * durch eine Zeichenkette ersetzt, die diesen Dateinamen eindeutig  macht.
 * Die  Datei  wird dann mit dem Modus read/write und den Rechten 0600 erzeugt.
 *
 * @param[in] filetemplate Pfad und Vorlage für den zu erstellenden Dateinamen
 * @param[in] ... Optionale Parameter
 * @return Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 */
{
	CString t;
	va_list args;
	va_start(args, filetemplate);
	t.VaSprintf(filetemplate,args);
	va_end(args);

	return OpenTemp(t);
}

int CFile::OpenTemp(const CString &filetemplate)
/*!\brief Eine temporäre Datei zum Lesen und Schreiben öffnen
 *
 * Diese Funktion erzeugt eine temporäre Datei mit einem eindeutigen Namen.
 * Dieser Name wird aus \p filetemplate erzeugt. Dazu  müssen  die letzten
 * sechs  Buchstaben  des  Parameters template XXXXXX sein, diese werden dann
 * durch eine Zeichenkette ersetzt, die diesen Dateinamen eindeutig  macht.
 * Die  Datei  wird dann mit dem Modus read/write und den Rechten 0600 erzeugt.
 *
 * @param[in] filetemplate Pfad und Vorlage für den zu erstellenden Dateinamen
 * @return Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 */
{
	#ifdef HAVE_MKSTEMP
		Close();
		CString t;
		t=filetemplate;
		int f=mkstemp((char*)((const char*)t));
		if (f<0) {
			SetError(TranslateErrno(errno),errno,"CFile::OpenTemp [%s]",(const char*)t);
			return 0;
		}
		FILE *ff=fdopen(f, "r+b");
		if (!ff) {
			SetError(TranslateErrno(errno),errno,"CFile::OpenTemp [fdopen (%i), %s]",f,(const char*)t);
			return 0;
		}
		if (!Open(ff)) {
			fclose(ff);
			close(f);
			return 0;
		}
		SetFilename(t);
		return 1;
	#else
		SetError(219);
		return 0;
	#endif
}

int CFile::Popen (const CString &command, const CString &mode)
/*!\brief Prozess öffnen
 *
 * \desc
 * Die  Funktion \c Popen öffnet einen Prozess dadurch, dass sie sich nach
 * Erzeugen einer Pipe aufteilt und eine Shell öffnet.  Da eine Pipe  nach
 * Definition  unidirektional  ist,  darf das Argument \p mode nur Lesen oder
 * Schreiben angeben,  nicht  beides;  der  resultierende  Datenkanal  ist
 * demzufolge nur-lesend oder nur-schreibend.
 *
 * \param[in] command Das  Argument \p command  enthält einen String,
 * der ein Shell-Kommandozeile enthält.  Dieses Kommando  wird  an \c /bin/sh
 * unter Verwendung des Flags \c -c übergeben. Interpretation, falls nötig, wird von
 * der Shell durchgeführt.
 * \param[in] mode Das Argument \p mode ist ein Zeiger auf einen mit NULL
 * beendeten  String,  der  entweder  ‘r’  für  Lesen  oder  ‘w’ für
 * Schreiben sein muss.
 *
 * @return Bei Erfolg liefert die Funktion 1 zurück, sonst 0
 */
{
	Close();
	if (command.Len()==0) {
		SetError(9);
		ff=NULL;
		size=0;
		pos=0;
		return 0;
	}
#ifdef WIN32
	CWString wideCommand(command);
	CWString wideMode(mode);
	ff=_wpopen((const wchar_t*)wideCommand,(const wchar_t*)wideMode);
#else
	ff=popen((const char*)command,(const char*)mode);
#endif

	if (ff==NULL) {
		SetError(9,errno,"%s",command.GetPtr());
		ff=NULL;
		size=0;
		pos=0;
		return 0;
	}
	isPopen=true;
	size=0;
	SetFilename(command);
	return 1;
}

int CFile::Popenf (const char * command, const char * mode, ...)
/*!\brief Prozess öffnen
 *
 * \desc
 * Die  Funktion \c Popen öffnet einen Prozess dadurch, dass sie sich nach
 * Erzeugen einer Pipe aufteilt und eine Shell öffnet.  Da eine Pipe  nach
 * Definition  unidirektional  ist,  darf das Argument \p mode nur Lesen oder
 * Schreiben angeben,  nicht  beides;  der  resultierende  Datenkanal  ist
 * demzufolge nur-lesend oder nur-schreibend.
 *
 * \param[in] command Das  Argument \p command  enthält einen String,
 * der ein Shell-Kommandozeile enthält.  Dieses Kommando  wird  an \c /bin/sh
 * unter Verwendung des Flags \c -c übergeben. Interpretation, falls nötig, wird von
 * der Shell durchgeführt.
 * \param[in] mode Das Argument \p mode ist ein Zeiger auf einen mit NULL
 * beendeten  String,  der  entweder  ‘r’  für  Lesen  oder  ‘w’ für
 * Schreiben sein muss.
 * \param[in] ... Optionale Parameter
 *
 * @return Bei Erfolg liefert die Funktion 1 zurück, sonst 0
 */

{
	CString f;
	va_list args;
	va_start(args, mode);
	f.VaSprintf(command,args);
	va_end(args);
	return Popen(f,mode);
}



int CFile::Open (FILE * handle)
/*!\brief Bereits geöffnete Datei übernehmen
 *
 * Mit dieser Funktion kann eine mit der C-Funktion \c fopen bereits geöffnete Datei
 * übernommen werden.
 * @param[in] handle Das Filehandle
 * @return Liefert 1 zurück, wenn das Filehandle erfolgreich übernommen wurde, sonst 0.
 * Ein Fehler kann nur auftreten, wenn das übergebene Filehandle selbst NULL ist.
 */
{
#ifdef WIN32FILES
	SetError(246);		// Funktion auf dieser Plattform nicht verf�gbar
	return 0;
#endif
	Close();

	if (handle==NULL) {
		SetError(9);
		return 0;
	}
	ff=handle;
	size=Lof();
	Seek(0);
	SetFilename("FILE");
	return 1;
}


int CFile::Close()
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
	if (MapBase!=NULL) {
		this->munmap(MapBase, (size_t)LastMapSize);
	}

	SetFilenamef(NULL);
	if (ff!=NULL) {
		int ret=1;
		#ifdef WIN32FILES
			CloseHandle((HANDLE)ff);
		#else
			if(isPopen) {
#ifdef WIN32
				if (_pclose ((FILE*)ff)!=0) ret=0;
#else
				if (pclose ((FILE*)ff)!=0) ret=0;
#endif
			} else {
				if (fclose ((FILE*)ff)!=0) ret=0;
			}
		#endif
		if (ret==0) SetError(TranslateErrno(errno),errno);
		isPopen=false;
		ff=NULL;
		size=0;
		pos=0;
		if (buffer!=0) {
			free (buffer);
			buffer=0;
		}
		return ret;
	}
	SetError(72);
	return 0;
}

int CFile::Delete()
/*!\brief Geöffnete Datei löschen
 *
 * Mit dieser Funktion wird die aktuelle Datei zunächst geschlossen und dann
 * gelöscht.
 *
 * @return Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 */
{
	if (IsOpen()) {
		CString Filename=GetFilename();
		Close();
		if (Filename.Len()>0) {
			return DeleteFile(Filename);
		}
		SetError(243);
		return 0;
	}
	SetError(72);
	return 0;
}

pplint64 CFile::Size () const
/*!\brief Größe der geöffneten Datei
 *
 * Diese Funktion liefert die Größe der geöffneten Datei in Bytes zurück.
 * \return Größe der Datei in Bytes, oder -1, wenn ein Fehler aufgetreten ist.
 *
 * \note Die Funktion ist CFile::Lof vorzuziehen
 */
{
	if (isPopen) return 0;
	if (ff!=NULL) {
		#ifdef WIN32FILES
			BY_HANDLE_FILE_INFORMATION info;
			if (GetFileInformationByHandle((HANDLE)ff,&info))
				return (((ppluint64)info.nFileSizeHigh)<<32)+info.nFileSizeLow;
		#else
			::fflush((FILE*)ff);
			struct stat buf;
			if ((fstat (fileno((FILE*)ff),&buf))==0)
				return ((pplint64)buf.st_size);
		#endif
		SetError(14);
		return (-1);
	}
	SetError(72);
	return -1;
}

int CFile::Seek(ppluint64 position)
/*!\brief Dateizeiger auf gewünschte Stelle bringen
 *
 * Diese Funktion bewegt den internen Dateizeiger auf die gewünschte Stelle
 *
 * \param[in] position Gewünschte Position innerhalb der Datei
 * \return Liefert 1 zurück, wenn der Dateizeiger erfolgreich auf die gewünschte
 * Position bewegt werden konnte, sonst 0. Bei Auftreten eines Fehlers kann
 * sich die Dateiposition dennoch geändert haben und sollte daher mit CFile::Ftell
 * abgefragt werden.
 */
{
	if (isPopen) {
		ppl6::SetError(362);
		return 0;
	}
	if (ff!=NULL) {
		#ifdef WIN32FILES
			LARGE_INTEGER in,out;
			in.QuadPart=position;
			if (SetFilePointerEx((HANDLE)ff,in,&out,FILE_BEGIN)) {
				pos=out.QuadPart;
			} else {
				SetError(11);
			}
		#else
			fpos_t p;
			#ifdef FPOS_T_STRUCT
				p.__pos=(__off64_t)position;
			#else
				p=(fpos_t)position;
			#endif
			int ret;
			if ((ret=::fsetpos((FILE*)ff,&p))!=0) {
				pos=Ftell();
				SetError(11,ret);
				return 0;
			}
			pos=Ftell();
		#endif
		if (pos!=position) {
			SetError(11);
			return 0;
		}
		return 1;
	}
	SetError(72);
	return 0;
}


int CFile::Fseek (ppluint64 offset, int origin )
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
	if (isPopen) {
		ppl6::SetError(362);
		return 0;
	}
	int suberr;
	if (ff!=NULL) {
		#ifdef WIN32FILES
			int o=FILE_BEGIN;
			if (origin==SEEK_CUR) o=FILE_CURRENT;
			if (origin==SEEK_END) o=FILE_END;
			if (origin==SEEK_SET) o=FILE_BEGIN;
			LARGE_INTEGER in,out;
			in.QuadPart=offset;
			if (SetFilePointerEx((HANDLE)ff,in,&out,o)) {
				pos=out.QuadPart;
				return 1;
			}
			SetError(11);
			return 0;
		#elif defined _HAVE_FSEEKO
			fpos_t p;
			p=(fpos_t) offset;
			suberr=::fseeko((FILE*)ff,p,origin);
		#else
			suberr=::fseek((FILE*)ff,(long)offset,origin);
		#endif
		if (suberr==0) {
			pos=Ftell();
			return 1;
		}
		SetError(TranslateErrno(errno),errno);
		return 0;
	}
	SetError(72);
	return 0;
}

pplint64 CFile::Ftell()
/*!\brief Aktuelle Dateiposition ermitteln
 *
 * Die Funktion Ftell liefert den aktuellen Wert des Dateipositionszeigers für  den  Stream zurück.
 *
 * @return Position des Zeigers innerhalb der Datei. Im Fehlerfall wird -1 zurückgegeben.
 */
{
	if (isPopen) return 0;
	if (ff!=NULL) {
		#ifdef WIN32FILES
			LARGE_INTEGER in,out;
			in.QuadPart=0;
			if (SetFilePointerEx((HANDLE)ff,in,&out,FILE_CURRENT)) {
				return out.QuadPart;
			}
			SetError(11);
			return 0;
		#else
			fpos_t p;
			if (fgetpos((FILE*)ff,&p)!=0) {
				SetError(TranslateErrno(errno),errno);
				return -1;
			}
			ppluint64 ret;
			#ifdef FPOS_T_STRUCT
				ret=(pplint64)p.__pos;
			#else
				ret=(pplint64)p;
			#endif
			return ret;
		#endif
	}
	SetError(72);
	return -1;
}


size_t CFile::Fread(void * ptr, size_t size, size_t nmemb)
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
	if (ff==NULL) {
		SetError(72);
		return 0;
	}
	if (ptr==NULL) {
		SetError(194);
		return 0;
	}
	size_t by=fread(ptr,size,nmemb,(FILE*)ff);
	pos+=(by*size);
	if (by<nmemb) {
		if (feof((FILE*)ff)) {
			SetError(154);
		} else {
			SetError(TranslateErrno(errno),errno);
		}
	}
	return by;
}

size_t CFile::Fwrite(const void * ptr, size_t size, size_t nmemb)
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
	if (ff==NULL) {
		SetError(72);
		return 0;
	}
	if (ptr==NULL) {
		SetError(194);
		return 0;
	}
	size_t by=fwrite(ptr,size,nmemb,(FILE*)ff);
	pos+=(by*size);
	if (pos>this->size) this->size=pos;
	if (by<nmemb) {
		SetError(TranslateErrno(errno),errno);
	}
	return by;
}

ppluint64 CFile::DoCopy (CFileObject &quellfile, ppluint64 bytes)
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
	if (ff==NULL) {
		SetError(72);
		return 0;
	}
	if (buffer==NULL) {
		buffer=(char *)malloc(COPYBYTES_BUFFERSIZE);
		if (buffer==NULL) {
			SetError(2);
			return 0;
		}
	}
	if (quellfile.Size()>quellfile.Ftell()) {
		if ((quellfile.Ftell()+(pplint64)bytes)>quellfile.Size()) {
			bytes=quellfile.Lof()-quellfile.Ftell();
		}
		ppluint64 rest=bytes;
		ppluint64 by;
		while (rest>0) {
			by=rest;
			if (by>COPYBYTES_BUFFERSIZE) by=COPYBYTES_BUFFERSIZE;
			by=quellfile.Read (buffer,(size_t)by);
			Write (buffer,(ppluint32)by);
			rest-=by;
		}
	}
	return bytes;
}

char * CFile::Fgets (char *buffer, size_t num)
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
	int suberr;
	if (ff!=NULL) {
		char *res;
		res=fgets(buffer, num, (FILE*)ff);
		if (res==NULL) {
			suberr=ferror((FILE*)ff);
			if (feof((FILE*)ff)) SetError(154,suberr);
			else SetError(TranslateErrno(errno),errno);
			return NULL;
		}
		ppluint32 by=(ppluint32)strlen(buffer);
		pos+=by;
		return buffer;
	}
	SetError(72);
	return NULL;
}

wchar_t *CFile::Fgetws (wchar_t *buffer, size_t num)
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
 *
 */
{
	if (ff!=NULL) {
#ifndef HAVE_FGETWS
		SetError(246);
		return NULL;
#else
		int suberr;
		wchar_t *res;
		res=fgetws(buffer, num, (FILE*)ff);
		if (res==NULL) {
			suberr=ferror((FILE*)ff);
			if (feof((FILE*)ff)) SetError(154,suberr);
			else SetError(TranslateErrno(errno),errno);
			return NULL;
		}
		ppluint32 by=(ppluint32)wcslen(buffer);
		pos+=by;
		return buffer;
#endif
	}
	SetError(72);
	return NULL;
}


int CFile::Puts (const char *str)
/*!\brief String schreiben
 *
 * Puts schreibt die Zeichenkette \p str ohne sein nachfolgendes 0-Byte in
 * den Ausgabestrom.
 *
 * @param str Pointer auf den zu schreibenden String
 * @return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 */
{
	if (ff==NULL) {
		SetError(72);
		return 0;
	}
	if (fputs(str,(FILE*)ff)!=EOF) {
		pos+=strlen(str);
		if (pos>this->size) this->size=pos;
		return 1;
	}
	SetError(TranslateErrno(errno),errno);
	return 0;
}

int CFile::Putws (const wchar_t *str)
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
 */
{
	if (ff==NULL) {
		SetError(72);
		return 0;
	}
#ifndef HAVE_FPUTWS
		SetError(246);
		return 0;
#else
	if (fputws(str,(FILE*)ff)!=-1) {
		pos+=wcslen(str)*sizeof(wchar_t);
		if (pos>this->size) this->size=pos;
		return 1;
	}
	SetError(TranslateErrno(errno),errno);
	return 0;
#endif
}

int CFile::Putc(int c)
/*!\brief Zeichen schreiben
 *
 * Putc schreibt das Zeichen \p c, umgesetzt in ein unsigned char,
 * in den Ausgabestrom.
 * @param c Zu schreibendes Zeichen
 * @return Bei Erfolg wird das geschriebende Zeichen als Integer Wert zurückgegeben,
 * im Fehlerfall -1;
 */
{
	int ret;
	if (ff) {
		ret=fputc(c,(FILE*)ff);
		if (ret!=EOF) {
			pos++;
			if (pos>this->size) this->size=pos;
			return ret;
		}
		SetError(TranslateErrno(errno),errno);
		return -1;
	}
	SetError(72);
	return -1;
}

int CFile::Getc()
/*!\brief Zeichen lesen
 *
 * Getc liest das  nächste Zeichen aus der Datei und gibt seinen unsigned char Wert gecastet
 * in einem int zurück.
 * @return Bei Erfolg wird der Wert des gelesenen Zeichens zurückgegeben, im
 * Fehlerfall -1.
 */
{
	int ret;
	if (ff) {
		ret=fgetc((FILE*)ff);
		if (ret!=EOF) {
			pos++;
			return ret;
		}
		SetError(TranslateErrno(errno),errno);
		return -1;
	}
	SetError(72);
	return -1;
}

int CFile::Putwc(wchar_t c)
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
	if (ff) {
#ifndef HAVE_FPUTWC
		SetError(246);
		return -1;
#else
		wint_t ret;
		ret=fputwc(c,(FILE*)ff);
		if (ret!=WEOF) {
			pos+=sizeof(wchar_t);
			if (pos>this->size) this->size=pos;
			return ret;
		}
		SetError(TranslateErrno(errno),errno);
		return -1;
#endif
	}
	SetError(72);
	return -1;
}

int CFile::Getwc()
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
	if (ff) {
#ifndef HAVE_FGETWC
		SetError(246);
		return -1;
#else
		wint_t ret;
		ret=fgetwc((FILE*)ff);
		if (ret!=WEOF) {
			pos+=sizeof(wchar_t);
			return(int) ret;
		}
		SetError(TranslateErrno(errno),errno);
		return -1;
#endif
	}
	SetError(72);
	return -1;
}

bool CFile::Eof() const
/*!\brief Prüfen, ob Dateiende erreicht ist
 *
 * Die Funktion prüft, ob das Dateiende erreicht wurde
 *
 * @return Liefert \c true zurück, wenn das Dateiende erreicht wurde, sonst \c false.
 * Falls die Datei nicht geöffnet war, wird ebenfalls \c false zurückgegeben.
 */
{
	if (ff!=NULL) {
		if (feof((FILE*)ff)!=0) return true;
		return false;

	}
	SetError(72);
	return false;
}

int CFile::GetFileNo() const
/*!\brief Filenummer der Datei
 *
 * Die Funktion liefert den Dateideskriptor als Integer zurück, wie er
 * von den Systemfunktionen open , read , write und close genutzt wird.
 *
 * @return Liefert die Filenummer zurück, oder -1, wenn die Datei nicht
 * geöffnet war.
 */
{
	if (!ff) {
		return -1;
	}
	return fileno((FILE*)ff);
}


int CFile::Flush()
/*!\brief Gepufferte Daten schreiben
 *
 * Die Funktion Flush bewirkt, dass alle gepufferten Daten des aktuellen Streams
 * mittels der zugrundeliegenden write-Funktion geschrieben werden. Der Status
 * des Streams wird dabei nicht berührt. Die Daten werden nicht zwangsweise auch
 * physikalisch auf die Platte geschrieben, sie können noch immer aus Performancegründen
 * vom Kernel oder Treiber gecached werden. Um 100 Prozent sicher zu gehen, kann man
 * die Funktion CFile::Sync verwenden.
 *
 * @return Bei erfolgreicher Ausführung wird 1 zurückgegeben, ansonsten 0.
 */
{
	if (ff!=NULL) {
		#ifdef WIN32FILES
			FlushFileBuffers((HANDLE)ff);
		#else
			if (fflush((FILE*)ff)==0) return 1;
			SetError(TranslateErrno(errno),errno);
			return 0;
		#endif
	}
	return 0;
}

int CFile::Sync()
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
	if (ff!=NULL) {
		#ifdef WIN32FILES
			SetError(513);
			return 0;
		#else
#ifdef HAVE_FSYNC
		int ret;
		ret=fsync(fileno((FILE*)ff));
		if (ret==0) return 1;
		SetErrorFromErrno("CFile::Sync");
		return 0;
#else
			SetError(513);
			return 0;
#endif
		#endif

	}
	SetError(239);
	return 0;
}

int CFile::Truncate(ppluint64 length)
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
	if (!ff) {
		SetError(72);
		return 0;
	}
	#ifdef HAVE_FTRUNCATE
		int fd=fileno((FILE*)ff);
		int ret=ftruncate(fd,(off_t)length);
		if (ret==0) {
			size=length;
			if (pos>size) Seek(size);
			return 1;
		}
		SetError(TranslateErrno(errno),errno);
		//SetError(282,errno);
		return 0;
	#endif
	SetError(246,"CFile::Truncate");
	return 0;
}

bool CFile::IsOpen() const
/*!\brief Prüfen, ob eine Datei geöffnet ist
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion kann geprüft werden, ob die mit CFile assoziierte Datei
 * gerade geöffnet ist.
 * \return Die Funktion liefert \p true zurück, wenn die Datei offen ist, ansonsten \p false.
 */
{
	if (ff!=NULL) return true;
	return false;
}


int CFile::LockExclusive(bool block)
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
 * \see Siehe auch CFile::LockShared und CFile::Unlock
 */
{
	if (!ff) {
		SetError(72);
		return 0;
	}
	#if defined HAVE_FCNTL
		int fd=fileno((FILE*)ff);
		int cmd=F_SETLK;
		if (block) cmd=F_SETLKW;
		struct flock f;
		f.l_start=0;
		f.l_len=0;
		f.l_whence=0;
		f.l_pid=getpid();
		f.l_type=F_WRLCK;
    	int ret=fcntl(fd,cmd,&f);
    	if (ret!=-1) return 1;
    	SetError(TranslateErrno(errno),errno);
    	return 0;

	#elif defined HAVE_FLOCK
		int fd=fileno((FILE*)ff);
		int flags=LOCK_EX;
		if (!block) flags|=LOCK_NB;
		int ret=flock(fd,flags);
		if (ret==0) return 1;
		SetError(TranslateErrno(errno),errno);
		return 0;
	#elif defined _WIN32
		// Argh, mir scheint dafür muss die komplette Klasse umgeschrieben
		// werden auf die Windows-spezifischen Funktionen :-(
		// return 1;
	#endif
	SetError(246,"CFile::LockExclusive");
	return 0;
}

int CFile::LockShared(bool block)
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
 * \see Siehe auch CFile::LockExclusive und CFile::Unlock
 */
{
	if (!ff) {
		SetError(72);
		return 0;
	}
	#if defined HAVE_FCNTL
		int fd=fileno((FILE*)ff);
		int cmd=F_SETLK;
		if (block) cmd=F_SETLKW;
		struct flock f;
		f.l_start=0;
		f.l_len=0;
		f.l_whence=0;
		f.l_pid=getpid();
		f.l_type=F_RDLCK;
    	int ret=fcntl(fd,cmd,&f);
    	if (ret!=-1) return 1;
    	SetError(TranslateErrno(errno),errno);
    	return 0;

	#elif defined HAVE_FLOCK
		int fd=fileno((FILE*)ff);
		int flags=LOCK_SH;
		if (!block) flags|=LOCK_NB;
		int ret=flock(fd,flags);
		if (ret==0) return 1;
		SetError(TranslateErrno(errno),errno);
		return 0;
	#elif defined _WIN32
		//return 1;	// siehe oben
	#endif
	SetError(246,"CFile::LockShared");
	return 0;
}

int CFile::Unlock()
/*!\brief Dateisperre aufheben
 *
 * Mit Unlock wird eine mit LockShared oder LockExclusive eingerichtete
 * Sperre wieder aufgehoben, so dass auch andere Prozesse wieder uneingeschränkt
 * auf die Datei zugreifen können.
 *
 * @return Bei Erfolg liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \see Siehe auch CFile::LockShared und CFile::LockExclusive
 */
{
	if (!ff) {
		SetError(72);
		return 0;
	}
	#if defined HAVE_FCNTL
		int fd=fileno((FILE*)ff);
		struct flock f;
		f.l_start=0;
		f.l_len=0;
		f.l_whence=0;
		f.l_pid=getpid();
		f.l_type=F_UNLCK;
    	int ret=fcntl(fd,F_SETLKW,&f);
    	if (ret!=-1) return 1;
    	SetError(TranslateErrno(errno),errno);
    	return 0;

	#elif defined HAVE_FLOCK
		int fd=fileno((FILE*)ff);
		int ret=flock(fd,LOCK_UN);
		if (ret==0) return 1;
		if (errno==EBADF) SetError(72);		// The argument fd is an invalid descriptor
		else if (errno==EINVAL) SetError(279);	// The argument fd refers to an object other than a file.
		else if (errno==EOPNOTSUPP) SetError(278);	// The argument fd refers to an object that does not support file locking.
		else SetError(281,errno);
		return 0;
	#elif defined _WIN32
		//return 1;	// siehe oben
	#endif
	SetError(246,"CFile::Unlock");
	return 0;
}

void CFile::SetMapReadAhead(size_t bytes)
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
	ReadAhead=bytes;
}

const char *CFile::Map(ppluint64 position, size_t bytes)
/*!\brief Datei in den Speicher mappen
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
 * auch geschrieben werden soll, ist die Funktion CFile::MapRW zu verwenden.
 *
 * @param[in] position Die gewünschte Startposition innerhalb der Datei
 * @param[in] bytes Die Anzahl Bytes, die gemapped werden sollen.
 * @return Bei Erfolg gibt die Funktion einen Pointer auf den Speicherbereich zurück,
 * in dem sich die Datei befindet, im Fehlerfall NULL.
 */
{
	if (!ff) {
		SetError(72);
		return 0;
	}

	if (position+bytes<=size) {
		if (MapBase!=NULL) {
			if (LastMapStart==position) {	// Dateiausschnitt schon gemapped?
				if (bytes<=LastMapSize) return MapBase;
			}
			if (position>LastMapStart && (bytes+position-LastMapStart)<=LastMapSize) return MapBase+position-LastMapStart;
			this->munmap(MapBase, (size_t)LastMapSize);
		}
		LastMapStart=position;
		if (ReadAhead>0 && bytes<ReadAhead) {
			bytes=ReadAhead;
			if (position+(ppluint64)bytes>size) bytes=(size_t)(size-position);
		}
		LastMapSize=bytes;
		return (const char*)this->mmap(position,bytes,1,0);
	}
	SetError(517);
	return NULL;
}

char *CFile::MapRW(ppluint64 position, size_t bytes)
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
 * (insbesondere schreibend), muss die Funktion CFile::Unmap aufgerufen werden.
 *
 *
 * @param[in] position Die gewünschte Startposition innerhalb der Datei
 * @param[in] bytes Die Anzahl Bytes, die gemapped werden sollen.
 * @return Bei Erfolg gibt die Funktion einen Pointer auf den Speicherbereich zurück,
 * in dem sich die Datei befindet, im Fehlerfall NULL.
 */
{
	if (!ff) {
		SetError(72);
		return 0;
	}
	if (position+bytes<=size) {
		if (MapBase!=NULL) {
			if ((LastMapProtection&2)) {	// Schon als read/write gemapped?
				if (LastMapStart==position) {	// Dateiausschnitt schon gemapped?
					if (bytes<=LastMapSize) return MapBase;
				}
				if (position>LastMapStart && (bytes+position-LastMapStart)<=LastMapSize) return MapBase+position-LastMapStart;
			}
			this->munmap(MapBase, (size_t)LastMapSize);
		}
		if (ReadAhead>0 && bytes<ReadAhead) {
			bytes=ReadAhead;
			if (position+bytes>size) bytes=(size_t)(size-position);
		}
		return (char*)this->mmap(position,bytes,3,0);
	}
	SetError(517);
	return NULL;
}

void CFile::Unmap()
{
	this->munmap(MapBase, (size_t)LastMapSize);
}
#ifdef HAVE_SYSCONF
static int __pagesize=0;
#endif

void *CFile::mmap(ppluint64 position, size_t size, int prot, int flags)
{
#ifdef HAVE_MMAP
	int mflags=0;
	if (prot&1) mflags|=PROT_READ;
	if (prot&2) mflags|=PROT_WRITE;
	if (prot&4) mflags|=PROT_EXEC;
	size_t rest=0;
#ifdef HAVE_SYSCONF
	if (!__pagesize) __pagesize=sysconf(_SC_PAGE_SIZE);
	// position muss an einer pagesize aligned sein
	rest=position%__pagesize;
	if (rest) {
		// Wir müssen alignen
		size_t multiplyer=position/__pagesize;
		position=multiplyer*__pagesize;
		size+=rest;
	}
#endif

	void *adr=::mmap(NULL, size, mflags,MAP_PRIVATE, fileno((FILE*)ff), (off_t) position);
	if (adr==MAP_FAILED) {
		SetError(TranslateErrno(errno),errno);
		MapBase=NULL;
		LastMapSize=0;
		return NULL;
	}
	LastMapSize=size;
	LastMapProtection=prot;
	LastMapStart=position;
	MapBase=(char*)adr;
	return (MapBase+rest);

#else
	size_t bytes;
	char *adr=(char*)malloc((size_t)size+1);
	if (adr) {
		Seek((ppldd)position);
		bytes=Fread(adr,1,size);
		if (!bytes) {
			free(adr);
			return 0;
		}
		adr[bytes]=0;
		MapBase=adr;
		LastMapSize=bytes;
		LastMapProtection=prot;
		LastMapStart=position;
		return (MapBase);
	}
	SetError(2);
	return NULL;
#endif

}

int CFile::munmap(void *addr, size_t len)
{
	if (!addr) return 1;
#ifdef HAVE_MMAP
	::munmap(addr, len);
#else
	if ((LastMapProtection&2)) {			// Speicher war schreibbar und muss
		if (Seek(LastMapStart)) {		// Zurückgeschrieben werden
			Fwrite(MapBase,1,len);
		}
	}
	free (MapBase);
#endif
	LastMapStart=LastMapSize=0;
	MapBase=NULL;
	LastMapProtection=0;
	return 1;
}



// ####################################################################
// Statische Funktionen
// ####################################################################

int CFile::LoadFile(CVar &object, const CString &filename)
/*!\ingroup PPLGroupFileIO
 * \brief Datei öffnen und den kompletten Inhalt in ein Objekt laden
 *
 * Mit dieser Funktion wird eine Datei zum Lesen geöffnet und der komplette Inhalt in das
 * angegebene Objekt geladen. Unterstützt werden zur Zeit folgende Objekte:
 * - CString
 * - CWString
 * - CBinary
 *
 * @param[out] object Das gewünschte Zielobjekt
 * @param[in] filename Der Dateiname
 * @return Liefert 1 zurück, wenn die Datei geöffnet und der Inhalt geladen werden konnte, sonst 0.
 */
{
	CFile ff;
	if (!ff.Open(filename,"rb")) return 0;
	size_t size=(size_t)ff.Size();
	char *buffer=(char*)malloc((size_t)ff.size+1);
	if (!buffer) {
		SetError(2);
		return 0;
	}
	size_t by=ff.Fread(buffer,1,size);
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
	SetError(337);
	return 0;
}

void *CFile::LoadFile(const CString &filename, size_t *size)
/*!\ingroup PPLGroupFileIO
 * \brief Kompletten Inhalt einer Datei laden
 *
 * Mit dieser Funktion wird eine Datei zum Lesen geöffnet und der komplette Inhalt in den
 * Speicher geladen.
 *
 * @param[in] filename Der Dateiname
 * @param[out] size Wird ein Pointer auf \p size übergeben, wird darin die Größe der geladenen
 * Datei gespeichert.
 * @return Bei Erfolg liefert die Funktion einen Pointer auf den Speicherbereich zurück, in den
 * die Datei geladen wurde. Der Aufrufer ist dafür verantwortlich, dass der Speicher nach Gebrauch
 * mit \c free wieder freigegeben wird. Im Fehlerfall wird NULL zurückgegeben.
 */
{
	CFile ff;
	if (!ff.Open(filename,"rb")) return 0;
	size_t s=(size_t)ff.Size();
	char *buffer=(char*)malloc(s+1);
	if (!buffer) {
		SetError(2);
		return NULL;
	}
	size_t by=ff.Fread(buffer,1,s);
	if (by!=s) {
		free(buffer);
		return NULL;
	}
	if (size) *size=by;
	buffer[by]=0;
	return buffer;
}

int CFile::Truncate(const CString &filename, ppluint64 bytes)
/*!\ingroup PPLGroupFileIO
 * \brief Datei abschneiden
 *
 * Die Funktionen Truncate bewirkt, dass die mit \p filename angegebene Datei
 * an der Position \p bytes abgeschnitten wird.
 *
 * Wenn die Datei vorher größer war, gehen überschüssige Daten verloren. Wenn die Datei
 * vorher kleiner war, wird sie vergrößert und die zusätzlichen Bytes werden als Nullen geschrieben.
 *
 * @param filename Der Name der gewünschten Datei
 * @param bytes Position, an der die Datei abgeschnitten werden soll.
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
#ifdef HAVE_TRUNCATE
		// truncate-Funktion vorhanden
		if (truncate(filename,(off_t)bytes)==0) return 1;
		SetError(TranslateErrno(errno),errno);
		return 0;
#elif defined HAVE_FTRUNCATE
	int fd;
#ifdef WIN32
	CWString wideFilename=filename;
	fd=::_wopen((const wchar_t*)wideFilename, O_RDWR);
#else
	fd=::open((const char*)filename, O_RDWR);
#endif
	if (fd<0) {
		SetError(TranslateErrno(errno),errno);
		return 0;
	}
	if (ftruncate(fd,(off_t)bytes)!=0) {
		SetError(TranslateErrno(errno),errno);
		::close(fd);
		return 0;
	}
	::close(fd);
	return 1;

#endif
	SetError(246,"CFile::Truncate");
	return 0;
}

int CFile::Existsf(const char * fmt, ...)
/*!\ingroup PPLGroupFileIO
 * \brief Prüfen, ob eine Datei existiert
 *
 * Mit Exists kann geprüft werden, ob eine Datei im Filesystem vorhanden ist.
 *
 * \param fmt Name der gewünschten Datei oder ein Formatstring, falls der Dateiname anhand
 * von weiteren Parametern zusammengesetzt werden muss
 * \param ... Optionale weitere Parameter, die in den vorhergehenden Formatstring \p fmt eingesetzt
 * werden sollen
 * \return Ist die Datei forhanden, gibt die Funktion 1 zurück, andernfalls 0.
 */
{
	va_list args;
	va_start(args, fmt);
	CString filename;
	filename.VaSprintf(fmt,args);
	va_end(args);
	return CFile::Exists(filename);
}

int CFile::Exists(const CString &filename)
/*!\ingroup PPLGroupFileIO
 * \brief Prüfen, ob eine Datei existiert
 *
 * Mit Exists kann geprüft werden, ob eine Datei im Filesystem vorhanden ist.
 *
 * \param fmt Name der gewünschten Datei
 * \return Ist die Datei forhanden, gibt die Funktion 1 zurück, andernfalls 0.
 */
{

#ifdef _WIN32
	struct _stat st;
	CWString wideFilename=filename;
	if (_wstat((const wchar_t*)wideFilename,&st)!=0) return 0;
#else
	struct stat st;
	if (stat((const char*)filename,&st)!=0) return 0;
#endif
	SetError(9,"%s",(const char*)filename);
	return 1;
}

int CFile::CopyFile(const CString &oldfile, const CString &newfile, size_t buffersize)
/*!\ingroup PPLGroupFileIO
 * \brief Datei kopieren
 *
 * Mit CopyFile wird die Datei \p oldfile nach \p newfile kopiert. Dazu wird die Quelldatei Stück
 * für Stück zunächst in den Hauptspeicher geladen und dann in die Zieldatei geschrieben. War die Zieldatei
 * \p newfile schon vorhanden, wird sie überschrieben. Anders als bei CFile::MoveFile oder CFile::RenameFile
 * müssen die beiden Dateien nicht im gleichen Filesystem liegen.
 *
 * \param oldfile Name der zu kopierenden Datei
 * \param newfile Name der Zieldatei.
 * \param buffersize Optionaler Wert, der die Größe des Copy-Buffers im Speicher angibt. Der
 *        Default ist 1 MB.
 * \return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	if (oldfile==newfile) return 1;	// Nix zu tun
	CFile f1, f2;
	if (!f1.Open(oldfile,"rb")) return 0;
	if (!f2.Open(newfile,"wb")) return 0;
	pplint64 bsize=buffersize;
	if (bsize==0) bsize=1024*1024;
	if (f1.Size()<bsize) bsize=f1.Size();
	void *buffer=malloc((size_t)bsize);
	if (!buffer) {
		SetError(2);
		return 0;
	}
	ppluint64 rest=f1.Size();
	ppluint64 bytes, done;
	while (rest) {
		bytes=bsize;
		if (bytes>rest) bytes=rest;
		done=f1.Read(buffer,(int)bytes);
		if (done!=bytes) {
			SetError(TranslateErrno(errno),errno);
			PushError();
			f2.Close();
			f1.Close();
			CFile::DeleteFile(newfile);
			free(buffer);
			PopError();
			return 0;
		}
		done=f2.Write(buffer,(int)bytes);
		if (done!=bytes) {
			SetError(TranslateErrno(errno),errno);
			PushError();
			f2.Close();
			f1.Close();
			CFile::DeleteFile(newfile);
			free(buffer);
			PopError();
			return 0;
		}
		rest-=bytes;
	}
	f1.Close();
	f2.Close();
	free(buffer);
	return 1;
}


int CFile::MoveFile(const CString &oldfile, const CString &newfile)
/*!\ingroup PPLGroupFileIO
 * \brief Datei verschieben oder umbenennen
 *
 * Mit dieser Funktion wird die Datei \p oldfile zu \p newfile umbenannt. Sie ist identisch mit
 * CFile::RenameFile. Beide Funktionen arbeiten am effizientesten, wenn die Zieldatei im gleichen
 * Filesystem liegt, da in diesem Fall nur die Verzeichniseinträge geändert werden müssen.
 * Ist dies nicht der Fall, wird automatisch die wesentlich langsamere Funktion CFile::CopyFile
 * gefolgt von CFile::DeleteFile aufgerufen.
 *
 * \param oldfile Name der zu verschiebenden bzw. umzubenennenden Datei
 * \param newfile Neuer Name
 * \return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 */
{
	if (oldfile==newfile) return 1;	// Nix zu tun
	return RenameFile(oldfile,newfile);
}



int CFile::RenameFile(const CString &oldfile, const CString &newfile)
/*!\ingroup PPLGroupFileIO
 * \brief Datei verschieben oder umbenennen
 *
 * Mit dieser Funktion wird die Datei \p oldfile zu \p newfile umbenannt. Sie ist identisch mit
 * CFile::RenameFile. Beide Funktionen arbeiten am effizientesten, wenn die Zieldatei im gleichen
 * Filesystem liegt, da in diesem Fall nur die Verzeichniseinträge geändert werden müssen.
 * Ist dies nicht der Fall, wird automatisch die wesentlich langsamere Funktion CFile::CopyFile
 * gefolgt von CFile::DeleteFile aufgerufen.
 *
 * \param oldfile Name der zu verschiebenden bzw. umzubenennenden Datei
 * \param newfile Neuer Name
 * \return Bei Erfolg wird 1 zurückgegeben, im Fehlerfall 0.
 *
 * \todo Diese Funktion ist nicht Windows-kompatibel, wenn Non-US-ASCII-Zeichen in den Dateinamen vorkommen!!!
 * Statt rename muss _wrename verwendet werden, statt fopen _wfopen, jeweils mit "const wchar_t *" Pointern.
 */
{
	if (oldfile==newfile) return 1;	// Nix zu tun
#ifdef WIN32
	CWString wideOldFile=oldfile;
	CWString wideNewFile=newfile;
	CFile::DeleteFile(newfile);
	if (_wrename(wideOldFile,wideNewFile)==0) {
#else
	if (rename(oldfile,newfile)==0) {
#endif
		FILE *fd=NULL;
		//printf ("buffer=%s\n",buff);
#ifdef WIN32
		fd=_wfopen((const wchar_t*)wideOldFile,L"rb");		// Versuchen die Datei zu oeffnen
#else
		fd=fopen((const char*)oldfile,"rb");		// Versuchen die Datei zu oeffnen
#endif
		if (fd) {
			fclose(fd);
			if (CFile::DeleteFile(oldfile)) return 1;
			SetErrorFromErrno("%s => %s",(const char*)oldfile,(const char*)newfile);
			unlink(newfile);
			return 0;
		}
		return 1;
	}
	if (errno==EXDEV) {	// oldfile und newfile befinden sich nicht im gleichen Filesystem.
		if (!CFile::CopyFile(oldfile,newfile)) return 0;
		if (CFile::DeleteFile(oldfile)) return 1;
	}
	SetErrorFromErrno("%s => %s",(const char*)oldfile,(const char*)newfile);
	return 0;
}

int CFile::TouchFilef(const char *filename, ...)
/*!\ingroup PPLGroupFileIO
 * \brief Leere Datei anlegen oder die Zeitstempel des letzten Zugriffs aktualisieren
 *
 * TouchFile arbeitet ähnlich wie das Unix-Lommando \c touch. Ist die angegebene Datei
 * \p filename noch nicht vorhanden, wird sie als leere Datei angelegt. Ist sie bereits vorhanden,
 * wird der Zeitstempel des letzen Zugriffs aktualisiert.
 *
 * \param filename Name der gewünschten Datei oder ein Formatstring, falls der Dateiname anhand
 * von weiteren Parametern zusammengesetzt werden muss
 * \param ... Optionale weitere Parameter, die in den vorhergehenden Formatstring \p filename eingesetzt
 * werden sollen
 * \return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	CFile ff;
	char *buff=NULL;
	va_list args;
	va_start(args, filename);
	vasprintf (&buff, (char*)filename, args);
	va_end(args);
	if (!buff) {
		return 0;
	}
	int ret=0;
	if (ff.Open(buff,"w")) {
		ret=1;
	}
	free(buff);
	return ret;
}

int CFile::TouchFile(const CString &filename)
/*!\ingroup PPLGroupFileIO
 * \brief Leere Datei anlegen oder die Zeitstempel des letzten Zugriffs aktualisieren
 *
 * TouchFile arbeitet ähnlich wie das Unix-Lommando \c touch. Ist die angegebene Datei
 * \p filename noch nicht vorhanden, wird sie als leere Datei angelegt. Ist sie bereits vorhanden,
 * wird der Zeitstempel des letzen Zugriffs aktualisiert.
 *
 * \param filename Name der gewünschten Datei
 * \return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	CFile ff;
	int ret=0;
	if (ff.Open(filename,"w")) {
		ret=1;
	}
	return ret;
}


int CFile::DeleteFilef(const char *filename, ...)
/*!\ingroup PPLGroupFileIO
 * \brief Datei löschen
 *
 * Mit dieser Funktion wird die Datei \p filename vom Datenträger gelöscht.
 *
 * \param filename Name der gewünschten Datei oder ein Formatstring, falls der Dateiname anhand
 * von weiteren Parametern zusammengesetzt werden muss
 * \param ... Optionale weitere Parameter, die in den vorhergehenden Formatstring \p filename eingesetzt
 * werden sollen
 * \return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0. Ein Fehler kann auftreten, wenn die
 * Datei garnicht vorhanden ist oder die notwendigen Zugriffsrechte fehlen.
 */
{
	char *buff=NULL;
	va_list args;
	va_start(args, filename);
	vasprintf (&buff, (char*)filename, args);
	va_end(args);
	if (!buff) {
		return 0;
	}
	CString fname(buff);
	free(buff);
	return DeleteFile(fname);
}

int CFile::DeleteFile(const CString &filename)
/*!\ingroup PPLGroupFileIO
 * \brief Datei löschen
 *
 * Mit dieser Funktion wird die Datei \p filename vom Datenträger gelöscht.
 *
 * \param filename Name der gewünschten Datei
 * \return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0. Ein Fehler kann auftreten, wenn die
 * Datei garnicht vorhanden ist oder die notwendigen Zugriffsrechte fehlen.
 */
{
	int ret;
#ifdef WIN32
	CWString wideFilename=filename;
	/*
	if (DeleteFileW((const wchar_t *)wideFilename)) return 1;
	ret=GetLastError();
	switch (ret) {
		case ERROR_FILE_NOT_FOUND: SetError(9,"%s",(const char*)filename); return 0;
		case ERROR_PATH_NOT_FOUND: SetError(9,"%s",(const char*)filename); return 0;
		case ERROR_ACCESS_DENIED: SetError(19,"%s",(const char*)filename); return 0;
	}
	SetError(13,"CFile::DeleteFile: %s",(const char*)filename);
	return 0;
	*/
	ret=::_wunlink((const wchar_t*)wideFilename);
#else
	ret=::unlink((const char*)filename);
#endif
	if (ret==0) return 1;
	SetErrorFromErrno(filename);
	return 0;
}

int CFile::WriteFile(const void *content, size_t size, const CString &filename)
{
	CFile ff;
	if (ff.Open(filename,"wb")) {
		if (ff.Fwrite(content,1,size)==size) return 1;
	}
	return 0;
}

int CFile::WriteFile(const CVar &object, const CString &filename)
{
	CFile ff;
	if (ff.Open(filename,"wb")) {
		return ff.Write(object);
	}
	return 0;
}

int CFile::WriteFilef(const void *content, size_t size, const char *filename, ...)
/*!\ingroup PPLGroupFileIO
 * \brief Daten in Datei schreiben
 *
 * Mit dieser Funktion werden \p size Bytes aus dem Speicherbereich beginnend bei
 * \p content in die Datei \p filename geschrieben. War die Datei bereits vorhanden,
 * wird sie überschrieben.
 *
 * \param content Pointer auf den Speicherbereich, der in die Datei geschrieben werdem soll
 * \param size Anzahl zu schreibender Bytes
 * \param filename Name der gewünschten Datei oder ein Formatstring, falls der Dateiname anhand
 * von weiteren Parametern zusammengesetzt werden muss
 * \param ... Optionale weitere Parameter, die in den vorhergehenden Formatstring \p filename eingesetzt
 * werden sollen
 * \return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	CFile ff;
	char *buff=NULL;
	va_list args;
	va_start(args, filename);
	vasprintf (&buff, (char*)filename, args);
	va_end(args);
	if (!buff) {
		SetError(2);
		return 0;
	}
	int ret=WriteFile(content,size,buff);
	free(buff);
	return ret;
}

int CFile::WriteFilef(const CVar &object, const char *filename, ...)
/*!\ingroup PPLGroupFileIO
 * \brief Daten eines von CVar abgeleiteten Objekts in Datei schreiben
 *
 * Mit dieser Funktion wird der Speicher des von CVar abgeleiteten Objekts \p object
 * in die Datei \p filename geschrieben. War die Datei bereits vorhanden, wird sie überschrieben.
 *
 * \param object Ein von CVar abgeleitetes Objekt. Derzeit werden folgende Objekte unterstützt_
 * - CString
 * - CWString
 * - CBinary
 * \param filename Name der gewünschten Datei oder ein Formatstring, falls der Dateiname anhand
 * von weiteren Parametern zusammengesetzt werden muss
 * \param ... Optionale weitere Parameter, die in den vorhergehenden Formatstring \p filename eingesetzt
 * werden sollen
 * \return Bei Erfolg gibt die Funktion die Anzahl geschriebender Bytes zurück, im Fehlerfall 0.
 */
{
	CFile ff;
	char *buff=NULL;
	va_list args;
	va_start(args, filename);
	vasprintf (&buff, (char*)filename, args);
	va_end(args);
	if (!buff) {
		SetError(2);
		return 0;
	}
	int ret=0;
	if (ff.Open(buff,"wb")) {
		ret=ff.Write(object);
	}
	free(buff);
	return ret;
}

CString CFile::MD5(const CString &filename)
{
	CFile ff;
	if (!ff.Open(filename,"rb")) return CString();
	return ff.MD5Sum();
}

/*!\namespace FILEATTR
 * \ingroup PPLGroupFileIO
 * \brief Definitionen der Datei-Attribute
 *
 * \desc
 * Der Namespace FILEATTR enthält die Definitionen der Datei-Attribute. Folgende Werte
 * sind enthalten:
 *
 * \copydoc FileAttribute.dox
 *
 * \see ppl6::FileAttr
 * \see ppl6::Chmod
 *
 */

static mode_t translate_fileattr(int attr)
{
	mode_t m=0;
#ifdef _WIN32
	if (attr&FILEATTR::USR_READ) m|=_S_IREAD;
	if (attr&FILEATTR::USR_WRITE) m|=_S_IWRITE;
	if (attr&FILEATTR::GRP_READ) m|=_S_IREAD;
	if (attr&FILEATTR::GRP_WRITE) m|=_S_IWRITE;
	if (attr&FILEATTR::OTH_READ) m|=_S_IREAD;
	if (attr&FILEATTR::OTH_WRITE) m|=_S_IWRITE;

#else
	if (attr&FILEATTR::ISUID) m+=S_ISUID;
	if (attr&FILEATTR::ISGID) m+=S_ISGID;
	if (attr&FILEATTR::STICKY) m+=S_ISVTX;
	if (attr&FILEATTR::USR_READ) m+=S_IRUSR;
	if (attr&FILEATTR::USR_WRITE) m+=S_IWUSR;
	if (attr&FILEATTR::USR_EXECUTE) m+=S_IXUSR;
	if (attr&FILEATTR::GRP_READ) m+=S_IRGRP;
	if (attr&FILEATTR::GRP_WRITE) m+=S_IWGRP;
	if (attr&FILEATTR::GRP_EXECUTE) m+=S_IXGRP;
	if (attr&FILEATTR::OTH_READ) m+=S_IROTH;
	if (attr&FILEATTR::OTH_WRITE) m+=S_IWOTH;
	if (attr&FILEATTR::OTH_EXECUTE) m+=S_IXOTH;
#endif
	return m;
}

int CFile::FileAttrf(int attr, const char *filename, ...)
/*! \brief Setz die Attribute einer exisitierenden Datei
 * \ingroup PPLGroupFileIO
 *
 * \desc
 * Mit dieser Funktion können die Zugriffsattribute einer existierenden Datei
 * gesetzt werden.
 *
 * \param attr Ein Wert, der die Attribute enthält
 * \copydoc FileAttribute.dox
 * \param filename Der Dateinamen oder ein Formatstring für den Dateinamen
 * \param ... Optionale Parameter für den Formatstring
 * \return Bei Erfolg gibt die Funktion true (1) zurück, im Fehlerfall wird ein
 * Fehlercode gesetzt, der mit den PPL-Fehlerfunktionen abgefragt werden kann, und die
 * Funktion gibt false (0) zurück.
 *
 * \see Chmod
 * \version 6.0.16
 */
{
	CString f;
	va_list args;
	va_start(args, filename);
	f.VaSprintf(filename,args);
	va_end(args);
	return Chmod((const char*)f,attr);
}

int CFile::FileAttr(int attr, const CString &filename)
/*! \brief Setz die Attribute einer exisitierenden Datei
 * \ingroup PPLGroupFileIO
 *
 * \desc
 * Mit dieser Funktion können die Zugriffsattribute einer existierenden Datei
 * gesetzt werden.
 *
 * \param attr Ein Wert, der die Attribute enthält
 * \copydoc FileAttribute.dox
 * \param filename Der Dateiname
 * \return Bei Erfolg gibt die Funktion true (1) zurück, im Fehlerfall wird ein
 * Fehlercode gesetzt, der mit den PPL-Fehlerfunktionen abgefragt werden kann, und die
 * Funktion gibt false (0) zurück.
 *
 * \see Chmod
 * \version 6.0.16
 */
{
	return Chmod((const char*)filename,attr);
}


int CFile::Chmod(const CString &filename, int attr)
/*! \brief Setz die Attribute einer exisitierenden Datei
 * \ingroup PPLGroupFileIO
 *
 * \desc
 * Mit dieser Funktion können die Zugriffsattribute einer existierenden Datei
 * gesetzt werden.
 *
 * \param filename Der Dateinamen
 * \param attr Die Attribute
 * \copydoc FileAttribute.dox
 * \return Bei Erfolg gibt die Funktion true (1) zurück, im Fehlerfall wird ein
 * Fehlercode gesetzt, der mit den PPL-Fehlerfunktionen abgefragt werden kann, und die
 * Funktion gibt false (0) zurück.
 *
 * \see FileAttr
 * \version 6.0.16
 */
{
	mode_t m=translate_fileattr(attr);
#ifdef _WIN32
	CWString wideFilename=filename;
	if (_wchmod((const wchar_t*)wideFilename,m)==0) return 1;
#else
	if (chmod(filename,m)==0) return 1;
#endif
	SetErrorFromErrno("%s",(const char*)filename);
	return 0;
}


int CFile::Stat(const CString &filename, CDirEntry &out)
{
#ifdef _WIN32
	struct _stat st;
	CWString wideFilename=filename;
	if (_wstat((const wchar_t*)wideFilename,&st)!=0) return 0;
#else
	struct stat st;
	if (stat((const char*)filename,&st)!=0) return 0;
#endif
	out.ATime=st.st_atime;
	out.CTime=st.st_ctime;
	out.MTime=st.st_mtime;
	out.Attrib=0;
	out.Size=(ppld64)st.st_size;
	out.File=filename;
	out.Path=GetPath(filename);
	out.Filename=ppl6::GetFilename(filename);


	if (st.st_mode & S_IFDIR) out.Attrib|=CPPLDIR_DIR;
	if (st.st_mode & S_IFREG) out.Attrib|=CPPLDIR_FILE;
	memset(out.AttrStr,'-',10);
	out.AttrStr[10]=0;

	//#if ( defined (WIN32) || defined (__DJGPP__) )
	#ifdef _WIN32
		if (st.st_mode & _S_IREAD) out.AttrStr[1]='r';
		if (st.st_mode & _S_IWRITE) out.AttrStr[2]='w';
		if (st.st_mode & _S_IEXEC) out.AttrStr[3]='x';
	#else
		#ifndef __DJGPP__
			if (st.st_mode & S_IFLNK) out.Attrib|=CPPLDIR_LINK;
		#endif
		if (st.st_mode & S_IRUSR) out.AttrStr[1]='r';
		if (st.st_mode & S_IWUSR) out.AttrStr[2]='w';
		if (st.st_mode & S_IXUSR) out.AttrStr[3]='x';
		if (st.st_mode & S_ISUID) out.AttrStr[3]='s';

		if (st.st_mode & S_IRGRP) out.AttrStr[4]='r';
		if (st.st_mode & S_IWGRP) out.AttrStr[5]='w';
		if (st.st_mode & S_IXGRP) out.AttrStr[6]='x';
		if (st.st_mode & S_ISGID) out.AttrStr[6]='s';

		if (st.st_mode & S_IROTH) out.AttrStr[7]='r';
		if (st.st_mode & S_IWOTH) out.AttrStr[8]='w';
		if (st.st_mode & S_IXOTH) out.AttrStr[9]='x';
	#endif

	if (out.Attrib&CPPLDIR_DIR) out.AttrStr[0]='d';
	if (out.Attrib&CPPLDIR_LINK) out.AttrStr[0]='l';
	return 1;

}

} // end of namespace ppl6
