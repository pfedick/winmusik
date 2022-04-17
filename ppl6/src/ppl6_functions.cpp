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
#ifdef HAVE_TIME_H
	#include <time.h>
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
#ifdef HAVE_STDARG_H
	#include <stdarg.h>
#endif
#ifdef HAVE_DIRENT_H
	#include <dirent.h>
#endif
#ifdef HAVE_FNMATCH_H
	#include <fnmatch.h>
#endif
#ifdef HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#ifdef HAVE_CTYPE_H
	#include <ctype.h>
#endif

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN		// Keine MFCs
	#include <time.h>
	#include <windows.h>
	#include <io.h>
	#include <tchar.h>
	#include <memory.h>
	#include <direct.h>
	#undef MessageBox
#endif


#include "ppl6.h"


namespace ppl6 {

/*! \brief Unnötige Zeichen am Anfang und Ende eines Textes löschen
 *
 * Die \c trim-Funktion löscht alle Leerzeilen, Tabs, Linefeeds und Returns am
 * Zeilenanfang und -ende.
 *
 * \param text Ein Pointer auf einen Null-terminierten String
 * \return Die Funktion gibt NULL zurück, wenn der Parameter \c text NULL war.
 * Ansonsten wird der durch \c text übergebene Pointer zurückgegeben.
 * \warning Die Funktion allokiert keinen neuen Speicher, sondern modifiziert den Text innerhalb des
 * übergebenen Pointers. Soll der Originaltext erhalten bleiben, muß dieser vorher gesichert werden.
 * \see Siehe auch die Funktion trimchar, die ein einzelnes definierbares Zeichen am Anfang und Ende
 * löscht.
 */
pplchar * trim (pplchar * text)
{
	size_t l,i,start,ende,s;
	if (!text) return NULL;

	l=strlen(text);
	if (l>0) {
		start=0; s=0;
		ende=l;
		for (i=0;i<l;i++) {
			if (text[i]==13||text[i]==10||text[i]==32||text[i]=='\t') {
				if (s==0) start=i+1;
			} else {
				s=1; ende=i;
			}
		}
		text [ende+1]=0;
		if (start==0) return (text);
		memmove(text,text+start,ende-start+2);
		return (text);
	}
    return (text);
}

/*! \brief Zeichen am Anfang und Ende eines Textes löschen
 *
 * Die \c trimchar-Funktion löscht das angegebene Zeichen am
 * Zeilenanfang und -ende.
 *
 * \param text Ein Pointer auf einen Null-terminierten String
 * \param chr Das zu löschende Zeichen
 * \return Die Funktion gibt NULL zurück, wenn der Parameter \c text NULL oder \c chr 0 war.
 * Ansonsten wird der durch \c text übergebene Pointer zurückgegeben.
 * \warning Die Funktion allokiert keinen neuen Speicher, sondern modifiziert den Text innerhalb des
 * übergebenen Pointers. Soll der Originaltext erhalten bleiben, muß dieser vorher gesichert werden.
 * \see Siehe auch die Funktion trimchar, die ein einzelnes definierbares Zeichen am Anfang und Ende
 * löscht.
 */

pplchar * trimchar (pplchar * text, ppluchar chr)
{
	size_t i,l,start,ende,s;
	if (!chr) return NULL;
	if (!text) return NULL;
	l=strlen(text);
	if (l>0) {
		start=0; s=0;
		ende=l;
		for (i=0;i<l;i++) {
			if (text[i]==chr) {
				if (s==0) start=i+1;
			}
			else {
				s=1; ende=i;
			}
		}
		text [ende+1]=0;
		if (start==0) return (text);
		memmove(text,text+start,ende-start+2);
		return (text);
	}
	return (NULL);
}

/*! \brief Ersetzt ein Zeichen in einem String durch ein anderes
 *
 * Ersetzt ein Zeichen in einem String durch ein anderes
 * \param str1 Der zu durchsuchende String
 * \param alt Das alte Zeichen, das ersetzt werden soll
 * \param neu Das neue Zeichen, das eingesetzt werden soll.
 * \warning Theoretisch kann die Funktion auch mehrere Zeichen ersetzen. Die neue
 * Zeichenkette darf aber nicht länger sein als die alte, da sonst Speicher
 * überschrieben werden könnte. Die Funktion fängt diesen Fall ab und macht
 * dann garnichts.
 */
void strxchg (char * str1, char *alt, char *neu)
{
	size_t l,i,ll;
	l=strlen(str1);
	ll=strlen(alt);
	if (strlen(neu)>ll) return;
	if (!ll) return;
	for ( i=0; i<l; i++) {
		if (strncmp(&str1[i],alt,ll)==0)
			strncpy(&str1[i],neu,ll);
	}
}

/*! \brief Ersetzt ein Zeichen in einem String durch ein anderes
 *
 * Ersetzt ein Zeichen in einem String durch ein anderes
 * \param str1 Der zu durchsuchende String
 * \param alt Das alte Zeichen, das ersetzt werden soll
 * \param neu Das neue Zeichen, das eingesetzt werden soll.
 * \warning Theoretisch kann die Funktion auch mehrere Zeichen ersetzen. Die neue
 * Zeichenkette darf aber nicht länger sein als die alte, da sonst Speicher
 * überschrieben werden könnte. Die Funktion fängt diesen Fall ab und macht
 * dann garnichts.
 */
void strxchg (char * str1, const char *alt, const char *neu)
{
	size_t l,i,ll;
	l=strlen(str1);
	ll=strlen(alt);
	if (strlen(neu)>ll) return;
	if (!ll) return;
	for ( i=0; i<l; i++) {
		if (strncmp(&str1[i],alt,ll)==0)
			strncpy(&str1[i],neu,ll);
	}
}

void hex (char * ziel, ppldd ziffer,ppldd groesse)
{
	char tmp [17];
	ppldd l;
	sprintf (tmp,"%%%ux",groesse);
	sprintf (ziel,tmp,ziffer);
	for (l=0;l<groesse;l++)
		if (ziel[l]==' ') ziel[l]='0';
}

void dumphex (char *adresse,ppldd zeilen)
{
	unsigned char b;
	char *_adresse;
	size_t _i,_z;
	char tmp [17];

	_adresse=(char *)adresse;
	for (_z=0;_z<zeilen;_z++) {
		ppl_sprintf (tmp,"%8llX:",(ppliptr)(_adresse));
		strxchg (tmp, (char*)" ",(char*)"0");
		ppl_printf ("%s ",tmp);
		for (_i=0;_i<16;_i++) {
			b=_adresse[_i];
			ppl_sprintf (tmp,"%2X",b);
			strxchg (tmp, (char*)" ",(char*)"0");
			ppl_printf ("%s ",tmp);
		}
		printf (" ");
		for (_i=0;_i<16;_i++) {
			b=_adresse[_i];
			if (b>31)
				ppl_printf ("%c",b);
			else
				ppl_printf (".");
		}
		ppl_printf ("\n");
		_adresse+=16;
	}
}

char * getargv (int argc, char * argv[], const char * argument)
{
	int i;
	size_t argl,l;
	char *ret;
	if (argc>1) {
		argl=strlen(argument);
		for (i=1;i<argc;i++) {
			if (strncmp(argv[i],argument,argl) == 0) {
				l=strlen(argv[i]);
				if (l>argl || argv[i+1]==NULL) {
					ret=(argv[i]+argl);
					//if (ret[0]=='-') return (char*)"";
					//if (ret[0]=='\\' && ret[1]=='-') return ret+1;
					return ret;
				} else {
					ret=(argv[i+1]);
					if (ret[0]=='-') return (char*)"";
					if (ret[0]=='\\' && ret[1]=='-') return ret+1;
					return ret;
				}
			}
		}
	}
	return (NULL);
}

CString GetPath(const CString &Path)
{
	CString res;
	size_t i,l,pos;
	char c;
	l=Path.Len();
	pos=0;
	for (i=0;i<l;i++) {
		c=Path[i];
		if (c=='/' || c==':' || c=='\\') pos=i;
	}
	res.Strncpy(Path,pos);
	return res;
}

CString GetPath(const char *path)
{
	CString res;
	size_t i,l,pos;
	char c;
	l=strlen(path);
	pos=0;
	for (i=0;i<l;i++) {
		c=path[i];
		if (c=='/' || c==':' || c=='\\') pos=i;
	}
	res.Strncpy(path,pos);
	return res;
}

CString GetFilename(const CString &Path)
{
	CString res;
	size_t i,l,pos;
	char c;
	l=Path.Len();
	pos=0;
	for (i=0;i<l;i++) {
		c=Path[i];
		if (c=='/' || c==':' || c=='\\') pos=i+1;
	}
	res=Path.Mid(pos);
	return res;
}

CString GetFilename(const char *path)
{
	CString p=path;
	return GetFilename(p);
}

CString FileSuffix(const CString &Filename)
{
	CString ret;
	const char *s=strrchr(Filename,'.');
	if (!s) return ret;
	ret=s+1;
	return ret;
}

int countbyte (char * text, char *byte)
{
	unsigned int i,c;
	c=0;
	for (i=0;i<strlen(text);i++)
		if (text[i]==byte[0]) c++;
	return (c);
}


char *strtolower (char * text)
{
	size_t i,l;
	if (text != NULL) {
		l=strlen(text);
		for (i=0;i<l;i++)
			text[i]=tolower(text[i]);
	}
	return text;
}

char *strtoupper (char * text)
{
	size_t i,l;
	if (text != NULL) {
		l=strlen(text);
		for (i=0;i<l;i++)
			text[i]=toupper(text[i]);
	}
	return text;
}

/*! \brief Sucht nach Zeichen in einem String
 *
 * \code
int instr (char * string, char * such, unsigned int start);
int instr (const char * string, const char * such, unsigned int start);
int instr (char * string, const char * such, unsigned int start);
int instr (const char * string, char * such, unsigned int start);
int instrcase (char * string, char * such, unsigned int start);
int instrcase (const char * string, const char * such, unsigned int start);
int instrcase (char * string, const char * such, unsigned int start);
int instrcase (const char * string, char * such, unsigned int start);
\endcode
 * Diese Funktionen suchen nach einer Zeichenkette innerhalb eines Strings, wobei
 * die Funktion \c instr zwischen Gross- und Kleinschreibung unterschiedet, und
 * die Funktion \c instrcase nicht.
 *
 * \param string ist ein Pointer (char* oder const char*) auf einen Null-terminierten String, der
 * den zu durchsuchenden String enthält.
 * \param such ist ein Pointer (char* oder const char*) auf einen Null-terminierten String, der
 * den zu suchenden String enthält.
 * \param start ist ein optionaler Parameter, der die Startposition innerhalb des Suchstrings angibt.
 * Der Default ist 0, das heißt der String wurd vom Anfang an durchsucht.
 * \return Wird der String \c such gefunden, wird seine Position zurückgegeben. Befindet sich der
 * String gleich an erster Stelle des Suchstrings, wird 0 zurückgegeben. Wird der String nicht gefunden,
 * wird -1 zurückgegeben.
 */
int instr (const char * string, const char * such, unsigned int start)
{
	if (!string) return -1;
	if (!such) return -1;
	const char * _t;
	if (start<strlen(string)) {
		_t=strstr((string+start),such);
		if (_t!=NULL) {
			return ((long)(_t-string));
		}
	}
	return (-1);
}


/*! \brief Sucht nach Zeichen in einem String unter ignorierung von Gross-/Kleinschreibung
 *
 * \copydoc instr(const char * string, const char * such, unsigned int start)
 */
int instrcase (const char * string, const char * such, unsigned int start)
{
	if (!string) return -1;
	if (!such) return -1;
	const char * _t;
	if (start<strlen(string)) {
		_t=strcasestr((string+start),such);
		if (_t!=NULL) {
			return ((long)(_t-string));
		}
	}
	return (-1);
}

ppldd getstrr(char * buffer, char * adresse)
{
	long i,ende;
	ende=i=0;
	buffer[0]=0;
	while (ende==0) {
		if (adresse[i]==13 || adresse[i]==10 || adresse[i]==0) {
			ende=1;
			strncpy (buffer,adresse,i);
			buffer[i]=0;
			return i;
		}
		i++;
	}
	return 0;
}


int getstring (char * ziel, ppldd laenge, char * quelle)
{
	if (strlen(quelle)>laenge)
		strncpy (ziel,quelle,laenge);
	else {
		memset (ziel,32,laenge);
		strcpy (ziel,quelle);
	}
	return ((int) strlen(ziel));
}


void pokeb (char *Adresse, ppldd Wert)
{
	Adresse[0]=(ppldb)(Wert & 255);
}

void pokew (char *Adresse, ppldd Wert)
{
	#ifdef __BIG_ENDIAN__
		Adresse[0]=(ppldb)(Wert & 255);
		Adresse[1]=(ppldb)((Wert / 256)&255);
	#else
		((ppldw*)Adresse)[0]=(ppldw)Wert;
	#endif

}

void poked (char *Adresse, ppldd Wert)
{
	#ifdef __BIG_ENDIAN__
		Adresse[0]=(ppldb)(Wert & 255);
		Adresse[1]=(ppldb)((Wert / 256)&255);
		Adresse[2]=(ppldb)((Wert / 65536)&255);
		Adresse[3]=(ppldb)((Wert / 16777216)&255);
	#else
		((ppldd*)Adresse)[0]=Wert;
	#endif

}

void poked64 (char *Adresse, ppld64 Wert)
{
	#ifdef __BIG_ENDIAN__
		Adresse[0]=(ppldb)(Wert & 255);
		Adresse[1]=(ppldb)((Wert >>8)&255);
		Adresse[2]=(ppldb)((Wert >>16)&255);
		Adresse[3]=(ppldb)((Wert >>24)&255);
		Adresse[4]=(ppldb)((Wert >>32)&255);
		Adresse[5]=(ppldb)((Wert >>40)&255);
		Adresse[6]=(ppldb)((Wert >>48)&255);
		Adresse[7]=(ppldb)((Wert >>56)&255);
	#else
		((ppld64*)Adresse)[0]=Wert;
	#endif

}

void Poke8 (char *Adresse, ppluint32 Wert)
{
	((ppluint8*)Adresse)[0]=(ppluint8)(Wert & 255);
}

void Poke16 (char *Adresse, ppluint32 Wert)
{
	#ifdef __BIG_ENDIAN__
		Adresse[0]=(ppldb)(Wert & 255);
		Adresse[1]=(ppldb)((Wert / 256)&255);
	#else
		((ppluint16*)Adresse)[0]=(ppluint16)(Wert & 0xffff);
	#endif

}

void Poke24 (char *Adresse, ppluint32 Wert)
{
	Adresse[0]=(ppldb)(Wert & 255);
	Adresse[1]=(ppldb)((Wert / 256)&255);
	Adresse[2]=(ppldb)((Wert / 65536)&255);
}

void Poke32 (char *Adresse, ppluint32 Wert)
{
	#ifdef __BIG_ENDIAN__
		Adresse[0]=(ppldb)(Wert & 255);
		Adresse[1]=(ppldb)((Wert / 256)&255);
		Adresse[2]=(ppldb)((Wert / 65536)&255);
		Adresse[3]=(ppldb)((Wert / 16777216)&255);
	#else
		((ppluint32*)Adresse)[0]=Wert;
	#endif
}

void Poke64 (char *Adresse, ppluint64 Wert)
{
	#ifdef __BIG_ENDIAN__
		Adresse[0]=(ppldb)(Wert & 255);
		Adresse[1]=(ppldb)((Wert >>8)&255);
		Adresse[2]=(ppldb)((Wert >>16)&255);
		Adresse[3]=(ppldb)((Wert >>24)&255);
		Adresse[4]=(ppldb)((Wert >>32)&255);
		Adresse[5]=(ppldb)((Wert >>40)&255);
		Adresse[6]=(ppldb)((Wert >>48)&255);
		Adresse[7]=(ppldb)((Wert >>56)&255);
	#else
		((ppluint64*)Adresse)[0]=Wert;
	#endif
}

void PokeFloat(void *Adresse, float Wert)
{
	#ifdef __BIG_ENDIAN__
		((float*)Adresse)[0]=Wert;
	#else
		((float*)Adresse)[0]=Wert;
	#endif
}

void poket (char *Adresse, ppldd Wert)
{
	Adresse[0]=(ppldb)(Wert & 255);
	Adresse[1]=(ppldb)((Wert / 256)&255);
	Adresse[2]=(ppldb)((Wert / 65536)&255);
}

//#pragma warning( disable : 4035 )

ppldd peekb (const char *Adresse)
{
	ppldb wert;
	wert=Adresse[0];
	return((ppldd)wert);
}

ppldd peekw (const char * Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2;
		wert1=Adresse[0];
		wert2=Adresse[1];
		return((ppldd) wert1+(wert2<<8));
	#else
		return (ppldd) ((ppldw*)Adresse)[0];
	#endif
}

ppldd peekt (const char *Adresse)
{
	ppldb wert1,wert2,wert3;
	wert1=Adresse[0];
	wert2=Adresse[1];
	wert3=Adresse[2];

	return((ppldd) wert1+(wert2<<8)+(wert3<<16));
}

ppldd peekd (const char *Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2,wert3,wert4;
		wert1=Adresse[0];
		wert2=Adresse[1];
		wert3=Adresse[2];
		wert4=Adresse[3];

		return((ppldd) wert1+(wert2<<8)+(wert3<<16)+(wert4<<24));
	#else
		return (ppldd) ((ppldd*)Adresse)[0];
	#endif
}

ppld64 peekd64 (const char *Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2,wert3,wert4,wert5,wert6,wert7,wert8;
		wert1=Adresse[0];
		wert2=Adresse[1];
		wert3=Adresse[2];
		wert4=Adresse[3];
		wert5=Adresse[4];
		wert6=Adresse[5];
		wert7=Adresse[6];
		wert8=Adresse[7];

		return((ppld64) (ppluint64)wert1+((ppluint64)wert2<<8)+((ppluint64)wert3<<16)+((ppluint64)wert4<<24)+((ppluint64)wert5<<32)+((ppluint64)wert6<<40)+((ppluint64)wert7<<48)+((ppluint64)wert8<<56));
	#else
		return (ppld64) ((ppld64*)Adresse)[0];
	#endif
}

ppluint32 Peek8 (const char *Adresse)
{
	return (ppluint32)((ppluint8*)Adresse)[0];
}

ppluint32 Peek16 (const char * Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2;
		wert1=Adresse[0];
		wert2=Adresse[1];
		return((ppluint32) (ppluint32)wert1+((ppluint32)wert2<<8));
	#else
		return (ppluint32) ((ppluint16*)Adresse)[0];
	#endif
}

ppluint32 Peek24 (const char *Adresse)
{
	ppldb wert1,wert2,wert3;
	wert1=Adresse[0];
	wert2=Adresse[1];
	wert3=Adresse[2];
	return((ppldd) wert1+(wert2<<8)+(wert3<<16));
}

ppluint32 Peek32 (const char *Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2,wert3,wert4;
		wert1=Adresse[0];
		wert2=Adresse[1];
		wert3=Adresse[2];
		wert4=Adresse[3];

		return((ppldd) (ppluint32)wert1+((ppluint32)wert2<<8)+((ppluint32)wert3<<16)+((ppluint32)wert4<<24));
	#else
		return (ppluint32) ((ppluint32*)Adresse)[0];
	#endif
}

ppluint64 Peek64 (const char *Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2,wert3,wert4,wert5,wert6,wert7,wert8;
		wert1=Adresse[0];
		wert2=Adresse[1];
		wert3=Adresse[2];
		wert4=Adresse[3];
		wert5=Adresse[4];
		wert6=Adresse[5];
		wert7=Adresse[6];
		wert8=Adresse[7];

		return((ppld64) (ppluint64)wert1+((ppluint64)wert2<<8)+((ppluint64)wert3<<16)+((ppluint64)wert4<<24)+((ppluint64)wert5<<32)+((ppluint64)wert6<<40)+((ppluint64)wert7<<48)+((ppluint64)wert8<<56));
	#else
		return (ppluint64) ((ppluint64*)Adresse)[0];
	#endif
}

float PeekFloat(const void *Adresse)
{
	#ifdef __BIG_ENDIAN__
		return (float)((float*)Adresse)[0];
	#else
		return (float)((float*)Adresse)[0];
	#endif
}

void pokes (char * adresse, const char * string)
{
  char *_a;
  size_t _l;
  _a=adresse;
  _l=strlen(string);
  memcpy (_a,string,_l);
}

void pokesz (char * adresse, const char * string)
{
  char *_a;
  _a=adresse;
  strcpy (_a,string);
}

void pokesn (char * adresse, const char * string, long laenge)
{
  strncpy (adresse,string,laenge);
}

void pokesnz (char * adresse, const char * string, long laenge)
{
  strncpy (adresse,string,laenge);
  adresse[laenge]=0;
}


// Network-Byte-Order
void PokeN8 (char *Adresse, ppluint32 Wert)
{
	((ppluint8*)Adresse)[0]=(ppluint8)(Wert & 255);
}

void PokeN16 (char *Adresse, ppluint32 Wert)
{
	#ifdef __BIG_ENDIAN__
		// Direktes schreiben löst bei unalignten Adressen unter Solaris einen Bus-Error aus
		//((ppluint16*)Adresse)[0]=(ppluint16)(Wert & 0xffff);
		Adresse[1]=(ppldb)(Wert & 255);
		Adresse[0]=(ppldb)((Wert >> 8)&255);
	#else
		Adresse[1]=(ppldb)(Wert & 255);
		Adresse[0]=(ppldb)((Wert >> 8)&255);
	#endif

}

void PokeN32 (char *Adresse, ppluint32 Wert)
{
	#ifdef __BIG_ENDIAN__
		// Direktes schreiben löst bei unalignten Adressen unter Solaris einen Bus-Error aus
		//((ppluint32*)Adresse)[0]=Wert;
		Adresse[3]=(ppldb)(Wert & 255);
		Adresse[2]=(ppldb)((Wert >> 8)&255);
		Adresse[1]=(ppldb)((Wert >> 16)&255);
		Adresse[0]=(ppldb)((Wert >> 24)&255);
	#else
		Adresse[3]=(ppldb)(Wert & 255);
		Adresse[2]=(ppldb)((Wert >> 8)&255);
		Adresse[1]=(ppldb)((Wert >> 16)&255);
		Adresse[0]=(ppldb)((Wert >> 24)&255);
	#endif

}

void PokeN64 (char *Adresse, ppluint64 Wert)
{
	#ifdef __BIG_ENDIAN__
		// Direktes schreiben löst bei unalignten Adressen unter Solaris einen Bus-Error aus
		//((ppluint64*)Adresse)[0]=Wert;
		Adresse[7]=(ppldb)(Wert & 255);
		Adresse[6]=(ppldb)((Wert >>8)&255);
		Adresse[5]=(ppldb)((Wert >>16)&255);
		Adresse[4]=(ppldb)((Wert >>24)&255);
		Adresse[3]=(ppldb)((Wert >>32)&255);
		Adresse[2]=(ppldb)((Wert >>40)&255);
		Adresse[1]=(ppldb)((Wert >>48)&255);
		Adresse[0]=(ppldb)((Wert >>56)&255);
	#else
		Adresse[7]=(ppldb)(Wert & 255);
		Adresse[6]=(ppldb)((Wert >>8)&255);
		Adresse[5]=(ppldb)((Wert >>16)&255);
		Adresse[4]=(ppldb)((Wert >>24)&255);
		Adresse[3]=(ppldb)((Wert >>32)&255);
		Adresse[2]=(ppldb)((Wert >>40)&255);
		Adresse[1]=(ppldb)((Wert >>48)&255);
		Adresse[0]=(ppldb)((Wert >>56)&255);
	#endif
}

ppluint32 PeekN8 (const char *Adresse)
{
	return (ppluint32)((ppluint8*)Adresse)[0];
}

ppluint32 PeekN16 (const char * Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2;
		wert1=Adresse[1];
		wert2=Adresse[0];
		return((ppluint32) (ppluint32)wert1+((ppluint32)wert2<<8));

	#else
		ppldb wert1,wert2;
		wert1=Adresse[1];
		wert2=Adresse[0];
		return((ppluint32) (ppluint32)wert1+((ppluint32)wert2<<8));
	#endif
}

ppluint32 PeekN32 (const char *Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2,wert3,wert4;
		wert1=Adresse[3];
		wert2=Adresse[2];
		wert3=Adresse[1];
		wert4=Adresse[0];
		return((ppldd) (ppluint32)wert1+((ppluint32)wert2<<8)+((ppluint32)wert3<<16)+((ppluint32)wert4<<24));

	#else
		ppldb wert1,wert2,wert3,wert4;
		wert1=Adresse[3];
		wert2=Adresse[2];
		wert3=Adresse[1];
		wert4=Adresse[0];
 		return((ppldd) (ppluint32)wert1+((ppluint32)wert2<<8)+((ppluint32)wert3<<16)+((ppluint32)wert4<<24));
	#endif
}

ppluint64 PeekN64 (const char *Adresse)
{
	#ifdef __BIG_ENDIAN__
		ppldb wert1,wert2,wert3,wert4,wert5,wert6,wert7,wert8;
		wert1=Adresse[7];
		wert2=Adresse[6];
		wert3=Adresse[5];
		wert4=Adresse[4];
		wert5=Adresse[3];
		wert6=Adresse[2];
		wert7=Adresse[1];
		wert8=Adresse[0];
		return((ppld64) (ppluint64)wert1+((ppluint64)wert2<<8)+((ppluint64)wert3<<16)+((ppluint64)wert4<<24)+((ppluint64)wert5<<32)+((ppluint64)wert6<<40)+((ppluint64)wert7<<48)+((ppluint64)wert8<<56));

	#else
		ppldb wert1,wert2,wert3,wert4,wert5,wert6,wert7,wert8;
		wert1=Adresse[7];
		wert2=Adresse[6];
		wert3=Adresse[5];
		wert4=Adresse[4];
		wert5=Adresse[3];
		wert6=Adresse[2];
		wert7=Adresse[1];
		wert8=Adresse[0];
 		return((ppld64) (ppluint64)wert1+((ppluint64)wert2<<8)+((ppluint64)wert3<<16)+((ppluint64)wert4<<24)+((ppluint64)wert5<<32)+((ppluint64)wert6<<40)+((ppluint64)wert7<<48)+((ppluint64)wert8<<56));
	#endif
}




int WritePFPHeader (char * adresse, PFPSTRUCT * pfp)
{
	if (!adresse || !pfp) {
		SetError(6);
		return 0;
	}
	if (pfp->header_version==2) {
		memcpy(adresse,"PFP-File",8);
		pokeb(adresse+8,2);
		pokeb(adresse+9,40);
		strncpy(adresse+10,(char *)pfp->file_id,4);
		pokeb(adresse+14,pfp->subversion);
		pokeb(adresse+15,pfp->mainversion);
		pokew(adresse+16,pfp->offset_name);
		pokew(adresse+18,pfp->offset_autor);
		poked(adresse+20,pfp->date_creation);
		poked(adresse+24,pfp->time_creation);
		poked(adresse+28,pfp->date_update);
		poked(adresse+32,pfp->time_update);
		pokew(adresse+36,0);
		pokew(adresse+38,0);
		return 1;
	}
	SetError(241);
	return 0;
}


int SavePFPHeader (CFile *ff, PFPSTRUCT *pfp)
{
	char * adr;
	adr=(char *)malloc(128);
	if (adr!=NULL) {
		WritePFPHeader (adr,pfp);
		ff->Seek(0);
		ff->Write(adr,40);
		free(adr);
		return 1;
	}
	SetError(2);
	return 0;
}

int PresetPFPHeader (PFPSTRUCT *pfp)
{
	if (pfp!=NULL) {
		memset (pfp,0,sizeof(PFPSTRUCT));
		memcpy ((char *)pfp->header,"PFP-File",8);
		pfp->header_version=2;
		pfp->header_length=40;
		memcpy((char *)pfp->file_id,"    ",4);
		pfp->date_creation=longdatum();
		pfp->time_creation=longzeit();
		pfp->date_update=pfp->date_creation;
		pfp->time_update=pfp->time_creation;
		return 1;
	}
	SetError(194);
	return 0;
}

char * resizebuffer(char *buffer, ppldd oldsize, ppldd newsize)
{
	char *n;
	if (!newsize) return NULL;
	n=(char*) malloc(newsize);
	if (!n) return NULL;
	if (oldsize<=newsize) {
		memcpy(n,buffer,oldsize);
		n[oldsize]=0;
	} else {
		memcpy(n,buffer,newsize);
	}
	free(buffer);
	return n;
}

int IsTrue(const char *value)
{
	if (!value) return 0;
	if (strlen(value)==0) return 0;
	if (atoi(value)!=0) return 1;
	if (strcasecmp((char*)value,(char*)"true")==0) return 1;
	if (strcasecmp((char*)value,(char*)"wahr")==0) return 1;
	if (strcasecmp((char*)value,(char*)"ja")==0) return 1;
	if (strcasecmp((char*)value,(char*)"yes")==0) return 1;
	if (strcasecmp((char*)value,(char*)"on")==0) return 1;
	if (strcasecmp((char*)value,(char*)"an")==0) return 1;
	return 0;
}

int IsFalse(const char *value)
{
	if (IsTrue(value)) return 0;
	return 1;
}



int IsDir(const char *path)
{
	if (!path) return 0;
	CDirEntry d;
	if (!CFile::Stat(path,d)) return 0;
	if (d.Attrib&CPPLDIR_DIR) return 1;
	return 0;
}

int IsFile(const char *file)
{
	if (!file) return 0;
	CDirEntry d;
	if (!CFile::Stat(file,d)) return 0;
	if (d.Attrib&CPPLDIR_FILE) return 1;
	return 0;
}

int FileExists(const char *fmt, ...)
{
	char *buff;
    va_list args;
    va_start(args, fmt);
    if (vasprintf (&buff, (char*)fmt, args)<0 || buff==NULL) {
        // Nicht genuegend RAM
    	va_end(args);
        return 0;
    }
    va_end(args);
    ppl6::CString filename(buff);
    free(buff);
    return CFile::Exists(filename);
}

void SetRect(RECT *r, int left, int top, int right, int bottom)
{
	r->left=left;
	r->top=top;
	r->right=right;
	r->bottom=bottom;
}

void CopyRect(RECT *dest, RECT *src)
{
	if (dest==NULL || src==NULL) return;
	memcpy(dest,src,sizeof(RECT));
	/*
	dest->left=src->left;
	dest->right=src->right;
	dest->top=src->top;
	dest->bottom=src->bottom;
	*/
}

int InsideRect(RECT *r, POINT *p)
{
	if (p->x>=r->left && p->x<=r->right
		&& p->y>=r->top && p->y<=r->bottom) return 1;
	return 0;
}

void HexDump(const void *address, ppldd bytes)
{
	HexDump(address,bytes,false);
}

void HexDump(const void *address, ppldd bytes, bool skipheader)
{
    char buff[1024], tmp[10], cleartext[20];
    if (!skipheader) {
    	ppl_sprintf (buff,"HEXDUMP: %u Bytes starting at Address 0x%016llX (%llu):",bytes,(ppliptr)address,(ppliptr)address);
    	PrintDebug("%s\n",buff);
    }

    const char *_adresse=(char*)address;
    ppldd spalte=0;
    ppl_sprintf (buff,"0x%08X: ",0);
    memset(cleartext,0,20);
    for (ppldd i=0;i<bytes;i++) {
        if (spalte>15) {
            strcat(buff,"                                                               ");
            buff[60]=0;
            strcat (buff,": ");
            strcat (buff,cleartext);
            PrintDebug("%s\n",buff);
            ppl_sprintf (buff,"0x%08X: ",i);
            memset(cleartext,0,20);
            spalte=0;
        }
        ppl_sprintf (tmp,"%02X ",(ppldb)_adresse[i]);
        strcat (buff,tmp);
        if ((ppldb)_adresse[i]>31 && (ppldb)_adresse[i]<128)  cleartext[spalte]=(ppldb)_adresse[i];
        else cleartext[spalte]='.';
        spalte++;
    }

    if (spalte>0) {
        strcat(buff,"                                                               ");
        buff[60]=0;
        strcat (buff,": ");
        strcat (buff,cleartext);
        PrintDebug("%s\n",buff);
    }
    if (!skipheader) PrintDebug("\n");
}

void HexDump(CString *s, const void *address, ppldd bytes, bool skipheader)
{
    char buff[1024], tmp[10], cleartext[20];
    if (!s) return;
    if (!skipheader) {
    	s->Concatf("HEXDUMP: %u Bytes starting at Address 0x%08X (%u):\n",bytes,address,address);
    }
    const char *_adresse=(char*)address;
    ppldd spalte=0;
    ppl_sprintf (buff,"0x%08X: ",0);
    memset(cleartext,0,20);
    for (ppldd i=0;i<bytes;i++) {
        if (spalte>15) {
            strcat(buff,"                                                               ");
            buff[60]=0;
            strcat (buff,": ");
            strcat (buff,cleartext);
            s->Concatf("%s\n",buff);
            ppl_sprintf (buff,"0x%08X: ",i);
            memset(cleartext,0,20);
            spalte=0;
        }
        ppl_sprintf (tmp,"%02X ",(ppldb)_adresse[i]);
        strcat (buff,tmp);
        if ((ppldb)_adresse[i]>31 && (ppldb)_adresse[i]<128)  cleartext[spalte]=(ppldb)_adresse[i];
        else cleartext[spalte]='.';
        spalte++;
    }

    if (spalte>0) {
        strcat(buff,"                                                               ");
        buff[60]=0;
        strcat (buff,": ");
        strcat (buff,cleartext);
        s->Concatf("%s\n",buff);
    }
    if (!skipheader) s->Concatf("\n");
}

int MkDir(const char *path)
{
	return MkDir(path,0);
}

int MkDir(const char *path, int recursive)
{
#ifdef _WIN32
	return MkDir(path,0,recursive);
#else
	return MkDir(path,
		S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH,
		recursive);
#endif
}

int MkDir(const char *path, ppluint32 mode, int recursive)
{
	CString s;
	if (!path) return 0;
	// Wenn es das Verzeichnis schon gibt, koennen wir sofort aussteigen
	if (IsDir((char*)path)) return 1;
	// 1=erfolgreich, 0=Fehler
	if (!recursive) {
#ifdef _WIN32
		s=path;
		s.Replace("/","\\");
		if (mkdir(s)==0) return 1;
#else
		if (mkdir(path,mode)==0) return 1;
#endif
		return 0;
	}
	// Wir hangeln uns von unten nach oben
	s.Clear();
	CTok tok(path,"/");
	const char *t;
	if(path[0]=='/')s.Concat("/");
	while ((t=tok.GetNext())) {
		s.Concat(t);

		//printf ("t: %s, s: %s\n",t,s.GetPtr());
		// Prüfen, ob das Verzeichnis da ist.
		if (!IsDir((char*)s.GetPtr())) {
#ifdef _WIN32
			if(s.Right(1)!=":") {
				s.Replace("/","\\");
				if (mkdir(s.GetPtr())!=0) return 0;
			}
#else
			if (mkdir(s.GetPtr(),mode)!=0) return 0;
#endif
		}
		s.Concat("/");
	}
	return 1;
}



int StringToRect(CString *s, RECT *r)
{
	if (!s) return 0;
	if (!r) return 0;
	CTok tok;
	const char *tmp;
	r->left=r->top=r->bottom=r->right=0;
	tmp=tok.GetFirst(s,",");
	if (tmp) r->left=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->top=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->right=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->bottom=atol(tmp);
	return 1;
}

int StringToRect(CString *s, WINRECT *r)
{
	if (!s) return 0;
	if (!r) return 0;
	CTok tok;
	const char *tmp;
	r->left=r->top=r->width=r->height=0;
	tmp=tok.GetFirst(s,",");
	if (tmp) r->left=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->top=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->width=atol(tmp);
	tmp=tok.GetNext();
	if (tmp) r->height=atol(tmp);
	return 1;
}

int RectToString(WINRECT *r, CString *s)
{
	if (!s) return 0;
	if (!r) return 0;
	s->Setf("%i,%i,%i,%i",r->left,r->top,r->width,r->height);
	return 1;
}

int RectToString(RECT *r, CString *s)
{
	if (!s) return 0;
	if (!r) return 0;
	s->Setf("%i,%i,%i,%i",r->left,r->top,r->right,r->bottom);
	return 1;
}



int MessageBox(int type, const char *caption, char * fmt, ...)
{
#ifdef _WIN32
	int ret;
	// TODO: vasprintf verwenden!
	char *buff=(char *)malloc(strlen(fmt)+4096);		// Laenge des Formatstrings plus Sicherheitsbuffer fuer Parameter
	if (!buff) return 0;
	va_list args;
    va_start(args, fmt);
	vsprintf (buff, fmt, args);
	va_end(args);
	ret=::MessageBoxA(NULL,buff,caption,type);
	free (buff);
	return ret;
#else
	return 0;
#endif
}


#ifdef _WIN32
int MessageBox(HWND hwnd, int type, char *caption, char * fmt, ...)
{
	int ret;
	char *buff=(char *)malloc(strlen(fmt)+4096);		// Laenge des Formatstrings plus Sicherheitsbuffer fuer Parameter
	if (!buff) return 0;
	// TODO: vasprintf verwenden!
	va_list args;
    va_start(args, fmt);
	vsprintf (buff, fmt, args);
	va_end(args);
	ret=::MessageBoxA(hwnd,buff,caption,type);
	free (buff);
	return ret;
}


#endif





}	// EOF namespace ppl6
