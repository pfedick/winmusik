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
#ifdef HAVE_STRING_H
	#include <string.h>
#endif
#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN		// Keine MFCs
	#include <windows.h>
#endif

#include "ppl6.h"

//#define CPP_ASSOC_ARRAY_USE_TREE

namespace ppl6 {

/*!\class CBinary
 * \ingroup PPLGroupDataTypes
 *
 * Die Klasse CBinary wird verwendet, um Binäre Daten zu speichern
 *
 * \version 6.0.16
 *
 */

/*!\brief Konstruktor der CBinary-Klasse
 *
 * Dies ist der Konstruktor der CBinary-Klasse. Er initialisiert die internen
 * Datenstrukturen.
 */
CBinary::CBinary()
{
	InitInternal();
}

/*!\brief Destruktor der CBinary-Klasse
 *
 * Dies ist der Destruktor der CBinary-Klasse. Er sorgt dafür, dass durch diese
 * Klasse allokierter Speicher wieder freigegeben wird und deinitialisiert die internen
 * Datenstrukturen.
 *
 * \version 6.0.16
 */
CBinary::~CBinary()
{
	PushError();
	Clear();
	PopError();
}

void CBinary::InitInternal()
{
	type=CVar::CBINARY;
	data=NULL;
	ptr=NULL;
	size=0;
}

CBinary::CBinary(const char *str)
{
	InitInternal();
	Copy(str,strlen(str));
}

CBinary::CBinary(CFileObject& file)
{
	InitInternal();
	Set(file);
}

CBinary::CBinary(const CBinary& bin)
{
	InitInternal();
	Copy((CBinary&)bin);
}

CBinary::CBinary(const CMemoryReference& mem)
{
	InitInternal();
	Copy(mem);
}


CBinary::CBinary(const CString& str)
{
	InitInternal();
	Copy((CString&)str);
}

CBinary::CBinary(const CWString& str)
{
	InitInternal();
	Copy(str);
}


void CBinary::ClearInternal()
{
	ptr=NULL;
	if (data) free(data);
	data=NULL;
	size=0;
}

void CBinary::Clear()
{
	ClearInternal();
}

void *CBinary::Malloc(size_t size)
{
	ClearInternal();
	ptr=::malloc(size);
	data=ptr;
	if (ptr) this->size=size;
	else SetError(2);
	return ptr;
}

void *CBinary::Calloc(size_t size)
{
	ClearInternal();
	ClearInternal();
	ptr=::calloc(1,size);
	data=ptr;
	if (ptr) this->size=size;
	else SetError(2);
	return ptr;
}


void CBinary::ManageMemory()
/*!\brief CBinary-Klasse soll den Speicher verwalten
 *
 * Bei einigen Funktionen übernimmt die CBinary-Klasse nur Pointer auf Speicherbereiche, verwaltet diesen aber
 * nicht. Durch Aufruf dieser Funktion wird der Klasse mitgeteilt, dass sie die Verwaltung des Speichers
 * übernehmen soll. Das bewirkt, dass beim Löschen oder einer Neuzuordnung der vorher definierte Speicher
 * wieder freigegeben wird. Dazu wird die Funktion "free" verwendet.
 *
 * \since
 * Diese Funktion wurde in Version 6.2.2 hinzugefügt
 */
{
	if (!data) data=ptr;
}

int CBinary::Set(void *ptr, size_t size)
{
	if (!ptr) {
		SetError(194,"CBinary::Set(==> void *ptr <==, size_t size)");
		return 0;
	}
	ClearInternal();
	this->ptr=ptr;
	this->size=size;
	return 1;
}

int CBinary::Set(const CMemoryReference &mem)
{
	ClearInternal();
	this->ptr=mem.adr();
	this->size=mem.size();
	return 1;
}

int CBinary::Copy(const CMemoryReference &mem)
{
	return Copy(mem.adr(),mem.size());
}

int CBinary::Copy(const char *str)
{
	if (!str) {
		SetError(194);
		return 0;
	}
	return Copy(str,strlen(str));
}

int CBinary::Copy(const void *ptr, size_t size)
{
	if (!ptr) {
		SetError(194,"CBinary::Set(==> void *ptr <==, size_t size)");
		return 0;
	}
	ClearInternal();
	if (size) {
		data=malloc(size);
		if (!data) {
			SetError(2,"CBinary::Copy(void *ptr, size_t size=%llu)",(ppluint64)size);
			return 0;
		}
		if (!memcpy(data, ptr, size)) {
			SetError(357,"CBinary::Copy(void *ptr, size_t size=%llu)",(ppluint64)size);
			free(data);
			data=NULL;
			return 0;
		}
		this->ptr=data;
		this->size=size;
	}
	return 1;
}

int CBinary::Concat(void *ptr, size_t size)
{
	if (!ptr) {
		SetError(194,"int CBinary::Concat(==> void *ptr <==, size_t size)");
		return 0;
	}
	if (!size) {
		SetError(194,"int CBinary::Concat(void *ptr, ==> size_t size <==)");
		return 0;
	}
	if (data==NULL && this->ptr!=NULL) {
		// Wir müssen zuerst die vorhandenen Daten in einen selbst verwalteten Datenblock
		// kopieren
		data=malloc(this->size+size);
		if (!data) {
			SetError(2);
			return 0;
		}
		memcpy(data,this->ptr,this->size);
		this->ptr=data;
		// Jetzt den neuen Block hintendran
		memcpy((char*)data+this->size,ptr,size);
		this->size+=size;
		return 1;
	}
	void *r=realloc(data,this->size+size);
	if (!r) {
		SetError(2);
		return 0;
	}
	data=r;
	this->ptr=r;
	memcpy((char*)data+this->size,ptr,size);
	this->size+=size;
	return 1;
}

int CBinary::Set(CFileObject& file)
{
	ClearInternal();
	file.Seek(0);
	data=file.Load();
	if (!data) {
		PushError();
		PopError();
		return 0;
	}
	ptr=data;
	size=(size_t)file.Size();
	return 1;

}

int CBinary::Set(const CVar& object)
{
	int t=object.DataType();
	if (t==CVar::CBINARY) {
			const CBinary &bin= static_cast<const CBinary&>(object);  // Objekt zu CBinary umwandeln
			return Set(bin.GetPtr(),bin.Size());
	} else if (t==CVar::CSTRING) {
			const CString &str= static_cast<const CString&>(object);  // Objekt zu CString umwandeln
			return Set((void*)str.GetPtr(),str.Size());
	} else if (t==CVar::CWSTRING) {
			const CWString &wstr= static_cast<const CWString&>(object);  // Objekt zu CWString umwandeln
			return Set((void*)wstr.GetBuffer(),wstr.Size());
	}
	SetError(337);
	return 0;
}


int CBinary::Copy(const CVar& object)
{
	int t=object.DataType();
	if (t==CVar::CBINARY) {
			const CBinary &bin= static_cast<const CBinary&>(object);  // Objekt zu CBinary umwandeln
			return Copy(bin.GetPtr(),bin.Size());
	} else if (t==CVar::CSTRING) {
			const CString &str= static_cast<const CString&>(object);  // Objekt zu CString umwandeln
			return Copy(str.GetPtr(),str.Size());
	} else if (t==CVar::CWSTRING) {
			const CWString &wstr= static_cast<const CWString&>(object);  // Objekt zu CWString umwandeln
			return Copy(wstr.GetBuffer(),wstr.Size());
	}
	SetError(337);
	return 0;
}

int CBinary::Load(const CString &filename)
{
	CFile ff;
	if (!ff.Open(filename,"rb")) return 0;
	return Set(ff);
}

int CBinary::Save(const CString &filename) const
{
	CFile ff;
	if (!ff.Open(filename,"wb")) return 0;
	if (!ff.Write(ptr,(int)size)) return 0;
	ff.Close();
	return 1;
}

int CBinary::Loadf(const char *filename, ...)
{
	CFile ff;
	CString f;
	va_list args;
	va_start(args, filename);
	f.VaSprintf(filename,args);
	va_end(args);
	if (!ff.Open(&f,"rb")) return 0;
	return Set(ff);
}

int CBinary::Savef(const char *filename, ...) const
{
	CFile ff;
	CString f;
	va_list args;
	va_start(args, filename);
	f.VaSprintf(filename,args);
	va_end(args);
	if (!ff.Open(&f,"wb")) return 0;
	if (!ff.Write(ptr,(int)size)) return 0;
	ff.Close();
	return 1;
}

size_t CBinary::GetSize() const
{
	if (ptr==NULL && data==NULL) {
		SetError(111,"CBinary::GetSize()");
		return 0;
	}
	return size;
}

size_t CBinary::Size() const
{
	if (ptr==NULL && data==NULL) {
		SetError(111,"CBinary::Size()");
		return 0;
	}
	return size;
}

size_t CBinary::Len() const
{
	if (ptr==NULL && data==NULL) {
		SetError(111,"CBinary::Len()");
		return 0;
	}
	return size;
}

void *CBinary::GetPtr() const
{
	if (ptr==NULL && data==NULL) {
		SetError(111,"CBinary::GetPtr()");
		return NULL;
	}
	return ptr;
}

CBinary& CBinary::operator=(const CBinary &bin)
{
	Copy(bin);
	return *this;
}

CBinary& CBinary::operator=(const CBinary *bin)
{
	if (!bin) {
		SetError(337);
		return *this;
	}
	Copy(bin->GetPtr(),bin->Size());
	return *this;
}

CBinary& CBinary::operator=(const char *str)
{
	Copy(str,strlen(str));
	return *this;
}


CBinary& CBinary::operator=(const CString& str)
{
	Copy(str);
	return *this;
}

CBinary& CBinary::operator=(const CMemoryReference& mem)
{
	Copy(mem);
	return *this;
}

CBinary& CBinary::operator=(const CWString& str)
{
	Copy(str);
	return *this;
}

char CBinary::operator[](size_t pos) const
{
	if (!size) return 0;
	char *p=(char*)ptr;
	if (pos>size) return 0;
	return p[pos];
}

char CBinary::Get(size_t pos) const
{
	if (!size) return 0;
	char *p=(char*)ptr;
	if (pos>size) return 0;
	return p[pos];
}

CString CBinary::GetMD5Sum() const
/*!\brief MD5-Summe berechnen
 *
 * Diese Funktion berechnet die MD5-Summe der binären Daten und gibt diese als
 * CString zurück.
 *
 * \return Die Funktion liefert ein CString-Objekt zurück. Bei Erfolg enthält dieses
 * die MD5-Summe, im Fehlerfall ist es leer.
 * Ein Fehler kann auftreten, wenn die CBinary-Klasse keine Daten enthalten hat,
 * bei der Berechnung der MD5-Summe ein Fehler aufgetreten ist, oder der angegebene
 * Buffer NULL ist.
 *
 * \version 6.0.16
 */
{
	CString ret;
	char tmp[33];
	if ((ptr==NULL && data==NULL) || size==0 ) {
		SetError(111,"CBinary::GetMD5Sum()");
		return ret;
	}
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(const unsigned char *)ptr,size);
	MD5End(&ctx, tmp);
	tmp[32]=0;
	ret=tmp;
	return ret;
}

/*!\brief MD5-Summe überprüfen
 *
 * Diese Funktion berechnet die MD5-Summe der binären Daten in diesem Objekt
 * und vergleicht sie mit dem angegebenen String.
  *
 * \param md5 Ein Pointer auf eine CString-Klasse, die den erwarteten MD5-Hash
 * enthält
 * \return Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 *
 * \version 6.0.16
 */
int CBinary::CheckMD5(const CString &md5) const
{
	CString compare=GetMD5Sum();
	if (compare.IsEmpty()) return 0;
	if (compare.StrCaseCmp(md5)==0) return 1;
	SetError(359,"%s <> %s",(const char*)compare,md5.GetPtr());
	return 0;
}

int CBinary::ToFile(CMemFile *File)
/*!\brief Binary einer File-Klasse zuweisen
 *
 * Die in der Klasse enthaltenen Daten werden einer CFile-Klasse (genauer: CMemFile)
 * zugewiesen.
 *
 * \param[in] File Pointer auf eine gültige CMemFile-Klasse
 * \returns Bei Erfolg liefert die Klasse true (1) zurück, im Fehlerfall false (0).
 *
 * \note
 * Das Binary wird nicht kopiert, sondern nur die interne Adresse an die File-Klasse
 * übergeben. Das CBinary-Objekt darf daher nicht verändert oder gelöscht werden, solange
 * die File-Klasse noch verwendet wird!
 */
{
	if (!File) {
		SetError(194,"int CBinary::ToFile(==> CMemFile *File <==)");
		return 0;
	}
	if (ptr==NULL && data==NULL) {
		SetError(111,"CBinary::ToFile");
		return 0;
	}
	return File->Open(ptr,size);
}


void CBinary::HexDump() const
{
	char buff[1024], tmp[10], cleartext[20];
	//if (!s) return;
	char *_adresse=(char*)ptr;
	ppldd spalte=0;
	sprintf (buff,"0x%08X: ",0);
	memset(cleartext,0,20);
	for (size_t i=0;i<size;i++) {
		if (spalte>15) {
			strcat(buff,"                                                               ");
			buff[60]=0;
			strcat (buff,": ");
			strcat (buff,cleartext);
			printf("%s\n",buff);
			sprintf (buff,"0x%08tX: ",i);
			memset(cleartext,0,20);
			spalte=0;
		}
		sprintf (tmp,"%02X ",(ppldb)_adresse[i]);
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
		printf("%s\n",buff);
	}
}

void CBinary::HexDump(CString &s) const
{
	char buff[1024], tmp[10], cleartext[20];
	//if (!s) return;
	char *_adresse=(char*)ptr;
	ppldd spalte=0;
	sprintf (buff,"0x%08X: ",0);
	memset(cleartext,0,20);
	for (size_t i=0;i<size;i++) {
		if (spalte>15) {
			strcat(buff,"                                                               ");
			buff[60]=0;
			strcat (buff,": ");
			strcat (buff,cleartext);
			s.Concatf("%s\n",buff);
			sprintf (buff,"0x%08tX: ",i);
			memset(cleartext,0,20);
			spalte=0;
		}
		sprintf (tmp,"%02X ",(ppldb)_adresse[i]);
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
		s.Concatf("%s\n",buff);
	}
}

CBinary::operator CMemoryReference () const
{
	return CMemoryReference(ptr,size);
}

CString CBinary::ToHex() const
{
	unsigned char *buff=(unsigned char*)ptr;
	CString str;
	for(size_t i=0;i<size;i++) str.Concatf("%02x",buff[i]);
	return str;
}

int	CBinary::FromHex(const CString &s)
{
	ClearInternal();
	if (s.IsEmpty()) return 1;
	const unsigned char *p=(const unsigned char *)s.GetPtr();
	unsigned char *t;
	size_t bytes=s.Len();
	if ((bytes&1)==1) {
		SetError(562,"ungerade Anzahl Zeichen");
		return 0;
	}
	ptr=malloc(bytes>>1);
	if (!ptr) {
		SetError(2);
		return 0;
	}
	t=(unsigned char*)ptr;
	size=bytes>>1;
	unsigned char value;
	for (size_t source=0, target=0;source<bytes;source+=2,target++) {
		unsigned char first=p[source];
		unsigned char second=p[source+1];
		if (first>='0' && first<='9') value=(first-'0');
		else if (first>='a' && first<='f') value=(first-'a'+10);
		else if (first>='A' && first<='F') value=(first-'A'+10);
		else {
			ClearInternal();
			SetError(562,"ungültiges Zeichen im String");
			return 0;
		}
		value=value<<4;
		if (second>='0' && second<='9') value|=(second-'0');
		else if (second>='a' && second<='f') value|=(second-'a'+10);
		else if (second>='A' && second<='F') value|=(second-'A'+10);
		else {
			ClearInternal();
			SetError(562,"ungültiges Zeichen im String");
			return 0;
		}
		t[target]=value;
	}
	return 1;
}

int CBinary::memcmp(const CBinary &other) const
{
	size_t min=size;
	//if (other.ptrsize>max) max=other.ptrsize;
	if (other.size<min) min=other.size;
	for (size_t i=0;i<min;i++) {
		if ( ((char*)ptr)[i] < ((char*)other.ptr)[i] ) return -1;
		if ( ((char*)ptr)[i] > ((char*)other.ptr)[i] ) return 1;
	}
	if (size<other.size) return -1;
	if (size>other.size) return 1;
	return 0;
}


bool CBinary::operator<(const CBinary &other) const
{
	int c=memcmp(other);
	if (c<0) return true;
	return false;
}

bool CBinary::operator<=(const CBinary &other) const
{
	int c=memcmp(other);
	if (c<=0) return true;
	return false;
}

bool CBinary::operator==(const CBinary &other) const
{
	int c=memcmp(other);
	if (c==0) return true;
	return false;
}

bool CBinary::operator!=(const CBinary &other) const
{
	int c=memcmp(other);
	if (c!=0) return true;
	return false;
}

bool CBinary::operator>=(const CBinary &other) const
{
	int c=memcmp(other);
	if (c>=0) return true;
	return false;
}

bool CBinary::operator>(const CBinary &other) const
{
	int c=memcmp(other);
	if (c>0) return true;
	return false;
}


} // EOF namespace ppl6

