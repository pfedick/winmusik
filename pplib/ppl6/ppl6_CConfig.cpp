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
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif

#include "ppl6.h"

namespace ppl6 {

typedef struct tagSections {
	struct tagSections *last, *next;
	char *name;
	CAssocArray *values;
} SECTION;

/*!\class CConfig
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Klasse können Konfigurationsdateien mit mehreren Sektionen geladen und
 * geschrieben werden.
 *
 * \example
 * Beispiel einer Konfigurationsdatei:
\code
[server]
ip=127.0.0.1
port=8080
path=/var/tmp

[user]
# In dieser Sektion werden Username und Passwort gespeichert
root=fhrquofhqufhqo
patrick=kngbhwloh

[allowedhosts]
# In dieser Sektion befinden sich IP-Adressen oder Hostnamen von den Rechnern,
# denen der Zugriff auf das Programm erlaubt ist.
192.168.0.1
192.168.0.2
192.168.0.3
192.168.0.4
\endcode
 */

CConfig::CConfig()
{
	Init();
}

CConfig::CConfig(const char *filename)
{
	Init();
	Load(filename);
}

CConfig::CConfig(CFileObject *file)
{
	Init();
	Load(file);
}

CConfig::~CConfig()
{
	PushError();
	if (separator) free(separator);
	if (currentsection) free(currentsection);
	separator=NULL;
	currentsection=NULL;
	Unload();
	PopError();
}


void CConfig::Init()
{
	separator=NULL;
	currentsection=NULL;
	SetSeparator("=");
	isused=false;
	first=last=section=NULL;
}

void CConfig::Unload()
{
	sections.Clear();
	SECTION *s=(SECTION *)first;
	while (s) {
		delete s->values;
		s=s->next;
		free (first);
		first=s;
	}
	section=first=last=NULL;
	if (isused) {
		isused=false;
	}
}

void CConfig::SetSeparator(const char *string)
{
	if (separator) free (separator);
	if (!string) string="=";
	separatorLength=strlen(string);
	separator=(char*)malloc(separatorLength+1);
	strcpy (separator,string);
}

int CConfig::SelectSection(const char *sectionname)
{
	section=FindSection(sectionname);
	if (section) return 1;
	SetError(221,sectionname);
	return 0;
}

void *CConfig::FindSection(const char *sectionname)
{
	if (!sectionname) return NULL;
	SECTION *s=(SECTION *)first;
	while (s) {
		if (strcasecmp(s->name,(char*)sectionname)==0) return (void *) s;
		s=s->next;
	}
	return NULL;
}

int CConfig::FirstSection()
{
	section=first;
	if (section) return 1;
	SetError(221,"Konfigurationsdatei enthaelt keine Sektionen");
	return 0;
}

int CConfig::NextSection()
{
	SECTION *s=(SECTION *)section;
	if (!s) return 0;
	section=s->next;
	if (!section) return 0;
	return 1;
}

const char *CConfig::GetSectionName()
{
	if (!section) return NULL;
	return (const char*) ((SECTION *)section)->name;
}


int CConfig::CreateSection(const char * sectionname)
{
	if (FindSection(sectionname)) return 1;		// Section existiert bereits
	SECTION *s;
	s=(SECTION *)malloc(sizeof(SECTION)+strlen(sectionname)+4);
	if (!s) {
		SetError(2);
		return 0;
	}
	s->name=((char*)s)+sizeof(SECTION);
	strcpy(s->name,sectionname);
	s->values=new CAssocArray();			// Array für die Werte erstellen
	s->next=NULL;
	s->last=(SECTION *) last;				// In die Kette haengen
	if (last) ((SECTION *)last)->next=s;
	last=s;
	if (!first) first=s;
	section=s;
	return 1;
}

int CConfig::DeleteSection(const char *sectionname)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) return 0;
	delete s->values;						// Array mit den Werten l�schen
	if (s->last) s->last->next=s->next;		// aus der Kette nehmen
	if (s->next) s->next->last=s->last;
	if (s==(SECTION *)last) last=s->last;
	if (s==(SECTION *)first) first=s->next;
	free(s);
	return 1;
}

void CConfig::Reset()
{
	if (!section) return;
	return ((SECTION *)section)->values->Reset();
}

int CConfig::GetFirst(char **key, char **value)
{
	if (!section) return 0;
	return ((SECTION *)section)->values->GetFirst(key,value);
}

int CConfig::GetFirst(CString *key, CString *value)
{
	if (!section) return 0;
	return ((SECTION *)section)->values->GetFirst(key,value);
}

int CConfig::GetNext(char **key, char **value)
{
	if (!section) return 0;
	return ((SECTION *)section)->values->GetNext(key,value);
}

int CConfig::GetNext(CString *key, CString *value)
{
	if (!section) return 0;
	return ((SECTION *)section)->values->GetNext(key,value);
}

int CConfig::Add(const char *sectionname, const char *key, const char *value)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) {
		if (!CreateSection(sectionname)) return 0;
		s=(SECTION *)FindSection(sectionname);
		if (!s) return 0;
	}
	return s->values->Concat(key,value);
}

int CConfig::Add(const char *sectionname, const char *key, int value)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) {
		if (!CreateSection(sectionname)) return 0;
		s=(SECTION *)FindSection(sectionname);
		if (!s) return 0;
	}
	return s->values->Concatf((const char*)key,"\n","%i",value);
}

int CConfig::Add(const char *sectionname, const char *key, bool value)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) {
		if (!CreateSection(sectionname)) return 0;
		s=(SECTION *)FindSection(sectionname);
		if (!s) return 0;
	}
	if (value) return s->values->Concatf((const char*)key,"\n","yes");
	return s->values->Concatf((const char*)key,"\n","no");
}


int CConfig::Add(const char *key, const char *value)
{
	if (!section) return 0;
	return ((SECTION *)section)->values->Concat(key,value);
}

int CConfig::Add(const char *key, int value)
{
	if (!section) return 0;
	return ((SECTION *)section)->values->Add(key,value);
}

int CConfig::Delete(const char *key)
{
	if (!section) return 0;
	return ((SECTION *)section)->values->Delete(key);
}

int CConfig::Delete(const char *sectionname, const char *key)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) return 0;
	return s->values->Delete(key);
}

const char *CConfig::Get(const char *key, const char *defaultvalue)
{
	if (!section) {
		SetError(293);
		return NULL;
	}
	const char *res=((SECTION *)section)->values->Get(key);
	if (!res) {
		SetError(294,"Section: %s, Key: %s",GetSectionName(),key);
		return (char*)defaultvalue;
	}
	return res;
}

const char *CConfig::Get(const char *sectionname, const char *key, const char *defaultvalue)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) return (char*)defaultvalue;
	const char *res=s->values->Get(key);
	if (!res) {
		SetError(294,"Section: %s, Key: %s",GetSectionName(),key);
		return (char*)defaultvalue;
	}
	return res;
}

bool CConfig::GetBool(const char *sectionname, const char *key, bool defaultvalue)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) return defaultvalue;
	const char *res=s->values->Get(key);
	if (!res) {
		SetError(294,"Section: %s, Key: %s",GetSectionName(),key);
		return defaultvalue;
	}
	return IsTrue(res);
}

bool CConfig::GetBool(const char *key, bool defaultvalue)
{
	if (!section) {
		SetError(293);
		return false;
	}
	const char *res=((SECTION *)section)->values->Get(key);
	if (!res) {
		SetError(294,"Section: %s, Key: %s",GetSectionName(),key);
		return defaultvalue;
	}
	return IsTrue(res);
}

int CConfig::GetInt(const char *sectionname, const char *key, int defaultvalue)
{
	SECTION *s=(SECTION *)FindSection(sectionname);
	if (!s) return defaultvalue;
	const char *res=s->values->Get(key);
	if (!res) {
		SetError(294,"Section: %s, Key: %s",GetSectionName(),key);
		return defaultvalue;
	}
	return ppl6::atoi(res);
}

int CConfig::GetInt(const char *key, int defaultvalue)
{
	if (!section) {
		SetError(293);
		return 0;
	}
	const char *res=((SECTION *)section)->values->Get(key);
	if (!res) {
		SetError(294,"Section: %s, Key: %s",GetSectionName(),key);
		return defaultvalue;
	}
	return ppl6::atoi(res);
}


const char *CConfig::GetSection(const char *section)
/*!\brief Inhalt einer Sektion als char* auslesen
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird der komplette Inhalt einer Sektion als char*
 * zurückgegeben
 *
 * \param section Der Name der Sektion ohne Eckige Klammern
 * \returns Im Erfolgsfall liefert die Funktion einen char-Pointer auf den Inhalt
 * der Sektion zurück. Im Fehlerfall wird NULL zurückgegeben und der Fehlercode
 * kann wie üblich mit den \link Errors Fehlerfunktionen \endlink abgefragt werden.
 */
{
	//sections.List("configSections");
	return sections.Get(section);
}

CAssocArray *CConfig::ReadSection(const char *section, CAssocArray *a)
/*!\brief Inhalt einer Sektion ein einem Array speichern
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird der Inhalt einer Sektion ein ein Assoziatives Array kopiert.
 *
 * \param section Der Name der Sektion ohne Eckige Klammern
 * \param a Ein Optionaler Pointer auf ein Assoziatives Array, in dem die Sektion gespeichert
 * werden soll.
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf ein Assoziatives Array
 * zurück. Wurder der Parameter \p a angegeben, wird das Ergebnis darin gespeichert und der
 * Return-Wert ist identisch mit \p a. Ist das nicht der Fall, wird mit \p new ein neues Array
 * angelegt und der Benutzer muss dafür sorgen, dass dieses wieder gelöscht wird. Im Fehlerfall
 * wird NULL zurückgegeben und der Fehlercode kann wie üblich mit den
 * \link Errors Fehlerfunktionen \endlink abgefragt werden.
 */
{
	const char *s=sections.Get(section);
	if (!s) return NULL;
	if (!a) {
		CAssocArray *a=new CAssocArray;
		if (a->CreateFromTemplate(s,"\n",separator,"\n",true)) return a;
		delete a;
	} else {
		if (a->CreateFromTemplate(s,"\n",separator,"\n",true)) return a;
	}
	return NULL;
}

void CConfig::Print()
/*!\brief Konfiguration auf STDOUT ausgeben
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird die geladene Konfiguration auf STDOUT ausgegeben.
 *
 */
{
	SECTION *s=(SECTION *)first;
	while (s) {
		printf ("[%s]\n",s->name);
		s->values->List();
		printf ("\n");
		s=s->next;
	}
}

int CConfig::Load(const char *filename, ...)
/*!\brief Konfiguration aus einer Datei laden
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird eine Konfiguration aus einer Datei geladen.
 *
 * \param filename Ein Pointer auf dem Dateinamen oder einen Formatierungsstring
 * \param ... Optionale Parameter für den Formatstring
 * \returns Bei Erfolg gibt die Funktion true (1) zurück, im Fehlerfall false (0). Der genaue
 * Fehlercode kann wie üblich mit den \link Errors Fehlerfunktionen \endlink abgefragt werden.
 */
{
	if (!filename) { SetError(6); return 0; }
	va_list args;
	va_start(args, filename);
	char *buff=NULL;
	if (vasprintf (&buff, (char*)filename, args)>0 && buff!=NULL) {
		CFile ff;
		if (ff.Open(buff,"rb")) {
			int ret=Load(&ff);
			free(buff);
			va_end(args);
			return ret;
		}
	} else {
		SetError(2);
	}
	if (buff) free(buff);
	va_end(args);
	return 0;
}

int CConfig::LoadFromMemory(void *buffer, int bytes)
/*!\brief Konfiguration aus dem Speicher laden
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird eine bereits im Speicher befindliche Konfiguration in das
 * CConfig-Objekt geladen.
 *
 * \param buffer Ein Pointer auf den Beginn des Speicherbereichs
 * \param bytes Die Größe des Speicherbereichs
 * \returns Bei Erfolg gibt die Funktion true (1) zurück, im Fehlerfall false (0). Der genaue
 * Fehlercode kann wie üblich mit den \link Errors Fehlerfunktionen \endlink abgefragt werden.
 *
 * \since Diese Funktion wurde mit Version 6.0.19 eingeführt
 */
{
	if (!buffer) {
		SetError(194,"int CConfig::LoadFromMemory(==> void *buffer <== , int bytes)");
		return 0;
	}
	if (!bytes) {
		SetError(194,"int CConfig::LoadFromMemory(void *buffer, ==> int bytes <== )");
		return 0;
	}
	CMemFile ff;
	if (!ff.Open(buffer,bytes)) return 0;
	return Load(&ff);
}

int CConfig::LoadFromString(CString &string)
/*!\brief Konfiguration aus einem String laden
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird eine in einem CString enthaltene Konfiguration in das
 * CConfig-Objekt geladen.
 *
 * \param string Ein Pointer auf die CString-Klasse
 * \returns Bei Erfolg gibt die Funktion true (1) zurück, im Fehlerfall false (0). Der genaue
 * Fehlercode kann wie üblich mit den \link Errors Fehlerfunktionen \endlink abgefragt werden.
 *
 * \since Diese Funktion wurde mit Version 6.0.19 eingeführt
 */
{

	CMemFile ff;
	if (!ff.Open((void*)string.GetPtr(),string.Len())) return 0;
	return Load(&ff);
}

int CConfig::Load(CFileObject *file)
/*!\brief Konfiguration aus einem CFileObject-Objekt laden
 *
 * \header \#include <ppl6.h>
 * \desc
 * Mit dieser Funktion wird eine Konfiguration aus einem CFileObject-Objekt geladen.
 * Die Funktion wird intern von den anderen Load-Funktionen verwendet.
 *
 * \param file Ein Pointer auf eine CFileObject-Klasse
 * \returns Bei Erfolg gibt die Funktion true (1) zurück, im Fehlerfall false (0). Der genaue
 * Fehlercode kann wie üblich mit den \link Errors Fehlerfunktionen \endlink abgefragt werden.
 *
 * \see Load(CString *string)
 * \see Load(char *filename, ...)
 * \see LoadFromMemory(void *buffer, int bytes)
 */
{
	Unload();
	char *cursect=NULL, *line;
	CString key,value;
	CString Line;
	char *buffer=(char *)malloc(65536);
	if (!buffer) { SetError(2); return 0; }
	char *sectionname=(char *)malloc(1024);
	if (!sectionname) { free(buffer); SetError(2); return 0; }
	size_t l;
	ppldds trenn;

	while (!file->Eof()) {			// Zeilen lesen, bis keine mehr kommt
		if (!file->Gets(buffer,65536)) break;
		Line=buffer;
		Line.LTrim();
		line=trim(buffer);		// überflüssige Zeichen wegschneiden
		l=strlen(line);
		if (cursect) {
			if (l==0 || (l>0 && line[0]!='[' && line[l-1]!=']')) {
				sections.Concat(cursect,buffer);
			}
		}
		if (l>0) {
			if (line[0]=='[' && line[l-1]==']') {		// Neue [Sektion] erkannt
				cursect=NULL;
				if (l<1024) {							// nur gültig, wenn < 1024 Zeichen
					strncpy(sectionname,line+1,l-2);	// Ohne die Klammern in den Buffer
					sectionname[l-2]=0;					// kopieren und 0-Byte anhängen
					cursect=trim(sectionname);			// eventuelle Spaces wegschneiden
					sections.Concat(cursect,"","");
				}
			} else if ((cursect) && line[0]!='#' && line[0]!=';') {		// Kommentare ignorieren
				trenn=Line.Instr(separator,0);			// Trennzeichen suchen
				if (trenn>0) {							// Wenn eins gefunden wurde, dann
					key=Line.Left(trenn);				// Key ist alles vor dem Trennzeichen
					key.Trim();
					value=Line.Mid(trenn+separatorLength);	// Value der rest danach
					value.RTrim("\n\r");
					//value.HexDump();
					//printf ("Key: >>%s<<, Value: >>%s<<\n",(const char*)key, (const char*)value);
					Add(cursect,(const char*)key,(const char*)value);				// Und das ganze dann hinzuf�gen ins Array
				}
			}
		}
	}

	free (sectionname);
	free (buffer);
	return 1;
}

int CConfig::Save(const char *filename)
{
	if (!filename) { SetError(6); return 0; }
	CFile ff;
	if (ff.Open(filename,"w")) {
		return Save(&ff);
	}
	return 0;
}

int CConfig::Save(CFileObject *file)
{
	char *key, *value;
	int res;
	SECTION *s=(SECTION *)first;
	while (s) {
		file->Putsf("\n[%s]\n",s->name);
		res=s->values->GetFirst(&key,&value);
		while (res) {
			//printf ("[%s]%s=%s\n",s->name,key,value);
			file->Putsf("%s%s%s\n",key,separator,value);
			res=s->values->GetNext(&key, &value);
		}
		s=s->next;
	}
	return 1;
}

int CConfig::IsTrue(const char *value)
{
	if (!value) return 0;
	CString v=value;
	v.Trim();
	v.LCase();
	if (v.IsEmpty()) return 0;
	if (v=="true") return 1;
	if (v=="ja") return 1;
	if (v=="yes") return 1;
	if (v=="1") return 1;
	if (v.ToInt()>0) return 1;
	return 0;
}

int CConfig::String2Point(const char *value, POINT *p)
{
	// Erwartet wird ein String mit einem Komma
	CTok tok;
	const char *tmp;
	tmp=tok.GetFirst(value,",");
	if (!tmp) return 0;
	p->x=atol(tmp);
	tmp=tok.GetNext();
	if (!tmp) return 0;
	p->y=atol(tmp);
	return 1;
}

void CConfig::String2Rect(const char *value, RECT *r)
{
	CTok tok;
	const char *tmp;
	r->left=r->top=r->bottom=r->right=0;
	tmp=tok.GetFirst(value,",");
	if (tmp) r->left=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->top=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->right=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->bottom=atol(tmp);

}


} // end of namespace ppl

