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

#include "ppl7.h"

namespace ppl7 {

/*!\class PFPChunk
 * \brief Daten-Chunk eines PFP-File Version 3
 *
 * \desc
 * In dieser Klasse werden die Daten eines einzelnen Chunks eines PFP-Files Version 3
 * gespeichert. Um einen Chunk zu erstellen, wird mit "new" eine neue Instanz
 * der Klasse generiert und mit der Funktion PFPFile::addChunk einem PFP-File
 * hinzugefügt. Um einen existierenden Chunk zu löschen, muß die Funktion
 * PFPFile::deleteChunk aufgerufen werden.
 *
 * \see PFPFile
 *
 */

PFPChunk::PFPChunk()
/*!\brief Konstruktor des PFPChunk
 *
 * \desc
 * Der Konstruktor initialisiert den Datenbereich mit NULL und setzt als
 * Name "UNKN" ein. Ein derartiger Chunk würde durch die PFPFile-Klasse
 * nicht gespeichert.
 *
 */
{
	chunkname=L"UNKN";
	chunkdata=NULL;
	chunksize=0;
}

PFPChunk::~PFPChunk()
/*!\brief Destruktor des PFPChunk
 *
 * \desc
 * Der Destruktor sorgt dafür, dass der Datenbereich des Chunks freigegeben
 * wird, sofern er definiert wurde, und wenn er einem PFPFile zugeordnet
 * wurde, wird er daraus entfernt.
 *
 */
{
	if (chunkdata) free(chunkdata);
	chunkdata=NULL;
}

void PFPChunk::setName(const String &chunkname)
/*!\brief Name des Chunks setzen
 *
 * \desc
 * Mit dieser Funktion wird der Name eines Chunks definiert. Der Name muss
 * exakt 4 Byte lang sein und darf nur Großbuchstaben enthalten (es wird
 * eine automatische Konvertierung durchgeführt). Ausserdem sind nur Zeichen
 * aus dem Zeichensatz US-ASCII erlaubt.
 *
 * \param chunkname String mit dem Namen des Strings
 * \exception IllegalArgumentException Wird geworfen, wenn der Name des Chunks ungültig ist
 *
 */
{
	if (chunkname.len()!=4) throw IllegalArgumentException();
	String s=chunkname;
	s.upperCase();
	for (size_t i=0;i<4;i++) {
		wchar_t c=s[i];
		if (c<32 || c>127) throw IllegalArgumentException();
	}
	this->chunkname=s;
}

void PFPChunk::setData(const void *ptr, size_t size)
/*!\brief Nutzdaten des Chunks setzen
 *
 *
 * \desc
 * Mit dieser Funktion werden die Nutzdaten des Chunks angegeben. Die
 * Daten werden dabei in einen eigenen Speicherbereich kopiert.
 *
 * \param ptr Ein Pointer auf den Beginn der Daten
 * \param size Größe der Daten in Byte. Ein Chunk darf maximal 2^32 - 9 Bytes groß sein
 * \exception NullPointerException Wird geworfen, wenn \p ptr auf NULL zeigt
 * \exception OutOfMemoryException Nicht genug Speicher
 * \exception IllegalArgumentException Der Parameter \p size ist zu groß
 */
{
	if (!ptr) throw NullPointerException();
	if (size>(0xffffffff-8)) throw IllegalArgumentException();
	if (chunkdata) free(chunkdata);
	chunksize=0;
	chunkdata=malloc(size);
	if (!chunkdata) throw OutOfMemoryException();
	memcpy(chunkdata,ptr,size);
	chunksize=size;
}

void PFPChunk::setData(const char *s)
/*!\brief String als Nutzdaten des Chunks setzen
 *
 * \desc
 * Mit dieser Funktion werden die Nutzdaten des Chunks angegeben. Die
 * Daten werden dabei in einen eigenen Speicherbereich kopiert.
 *
 * \param s Pointer auf einen 0-Terminierten String, der als
 * Nutzdaten übernommen werden soll.
 * \exception NullPointerException Wird geworfen, wenn der Parameter \p s auf NULL zeigt
 * \exception OutOfMemoryException Nicht genug Speicher
 * \exception IllegalArgumentException Der String ist zu groß
 */
{
	if (!s) throw NullPointerException();
	size_t l=strlen(s);
	setData((void*)s,l+1);
}

void PFPChunk::setData(const String &s)
/*!\brief String als Nutzdaten des Chunks setzen
 *
 * \desc
 * Mit dieser Funktion werden die Nutzdaten des Chunks angegeben. Die
 * Daten werden dabei in einen eigenen Speicherbereich kopiert.
 *
 * \param s Ein String, dessen Inhalt als Nutzdaten übernommen werden soll.
 * Der String wird dazu in das lokale 8-Bit-Format des Systems konvertiert.
 * \exception OutOfMemoryException Nicht genug Speicher
 * \exception IllegalArgumentException Der String ist zu groß
 * \exception CharacterEncodingException Der String konnte nicht konvertiert werden
 */
{
	setData((const char*)s,s.size());
}

/*!\brief Nutzdaten des Chunks setzen
 *
 * \desc
 * Mit dieser Funktion werden die Nutzdaten des Chunks angegeben. Die
 * Daten werden dabei in einen eigenen Speicherbereich kopiert.
 *
 * \param data Eine Referenz aif ein ByteArray oder ByteArrayPtr
 * \exception NullPointerException Wird geworfen, wenn \p ptr auf NULL zeigt
 * \exception OutOfMemoryException Nicht genug Speicher
 */
void PFPChunk::setData(const ByteArrayPtr &data)
{
	setData(data.ptr(),data.size());
}

size_t PFPChunk::size()
/*!\brief Größe des Chunks auslesen
 *
 * \desc
 * Mit dieser Funktion kann die Größe des Chunks ausgelesen werden.
 *
 * \returns Enthält der Chunk Daten, wird die Anzahl Bytes zurückgegeben.
 * Ist er leer, wird 0 zurückgegeben und der Fehlercode 424 gesetzt.
 *
 */
{
	return chunksize;
}

const void *PFPChunk::data()
/*!\brief Pointer auf die Daten des Chunks holen
 *
 * \desc
 * Mit dieser Funktion wir der Pointer auf die Nutzdaten des Chunks
 * ausgelesen.
 *
 * \returns Enthält der Chunk Daten, wird ein Pointer darauf zurückgegeben.
 * Wenn nicht, wird NULL zurückgegeben.
 *
 */
{
	return chunkdata;
}

const String &PFPChunk::name()
/*!\brief Name des Chunks auslesen
 *
 * \desc
 * Diese Funktion liefert den Namen des Chunks zurück. Dieser ist
 * immer 4 Byte groß.
 *
 * \returns String mit dem Namen des Chunks.
 *
 */
{
	return chunkname;
}




/*!\class PFPFile
 * \brief Klasse zum Lesen und schreiben von PFP-Files Version 3
 *
 * Mit dieser Klasse können Dateien mit "PFP-File"-Header der Version 3 gelesen und geschrieben werden.
 * Mit Version 3 wurde ein mehr generisches Format definiert, als in den beiden Vorgängerversionen.
 * Jedes File, ganz gleich welchen Inhalt es hat, hat bis zum Ende den gleichen Aufbau. Wichtigste
 * Neuerung dabei sind die sogenannten Chunks. Ein File kann aus bliebig vielen Chunks bestehen.
 * Diese werden von der Klasse PFPChunk abgeleitet, bekommen einen Namen und einen beliebigen
 * Inhalt. Diese können dann mit PFPFile::Add in das File hinzugefügt werden.
 *
 * \copydoc PFPFileVersion3
 *
 * \since Version 6.1.0
 *
 */

/*!\var PFPFile::Chunks
 * \brief Verwaltung aller Chunks in einer Liste
 *
 * \since Version 6.1.0
 */

/*!\var PFPFile::id
 * \brief enthält die ID des Chunks. Die ID ist immer 4 Byte lang, gefolgt von einem 0-Byte
 *
 * \since Version 6.1.0
 */

/*!\var PFPFile::mainversion
 * \brief Hier wird die Hauptversion des Files gespeichert
 *
 * \since Version 6.1.0
 */

/*!\var PFPFile::subversion
 * \brief Hier wird die Unterversion des Files gespeichert
 *
 * \since Version 6.1.0
 */

/*!\var PFPFile::comp
 * \brief Kompressions-Flag
 *
 * In dieser Variable wird die Art der Komprimierung gespeichert:
 * - 0 = keine Komprimierung
 * - 1 = Zlib
 * - 2 = Bzip2
 *
 * \since Version 6.1.0
 */

/*!\var PFPFile::findchunk
 * \brief Chunkname für FindFirstChunk und FindNextChunk
 *
 * \since Version 6.1.0
 */


PFPFile::PFPFile()
/*!\brief Konstruktor der Klasse
 *
 * Hier werden einige interne Variablen initialisert, die ID wird auf "UNKN" gesetzt, Version
 * auf 0 und Kompression abgeschaltet
 *
 * \since Version 6.1.0
 */
{
	id=L"UNKN";
	mainversion=subversion=0;
	comp=Compression::Algo_NONE;
}

PFPFile::~PFPFile()
/*!\brief Destruktor der Klasse
 *
 * Der Destruktor sorgt dafür, dass sämtlicher von der Klasse allokierter Speicher einschließlich
 * aller geladener Chunks freigegeben wird.
 *
 * \since Version 6.1.0
 */
{
	clear();
}

void PFPFile::clear()
/*!\brief Inhalt der Klasse löschen
 *
 * \desc
 * Mit dieser Funktion werden alle Chunks im Speicher freigegeben und die Klasse auf den
 * Ursprungszustand zurückgesetzt, das heisst sie ist anschließend leer
 *
 */
{
	List<PFPChunk*>::Iterator it;
	Chunks.reset(it);
	while (Chunks.getNext(it)) {
		delete it.value();
	}
	Chunks.clear();
	id=L"UNKN";
	mainversion=subversion=0;
	comp=Compression::Algo_NONE;
}

void PFPFile::setVersion(int main, int sub)
/*!\brief Version setzen
 *
 * \desc
 * Mit dieser Funktion wird die Version des PFP-Files gesetzt.
 *
 * \param main Hauptversion, Wert zwischen 0 und 255
 * \param sub Unterversion, Wert zwischen 0 und 255
 * \exception IllegalArgumentException Wird geworfen, wenn \p main oder \p sub ausserhalb des gültigen Bereichs liegen.
 *
 * \remarks
 * Haupt- und Unterversion werden jeweils in einem einzelnen Byte gespeichert. Daher darf die
 * Version nicht größer als 255 werden.
 *
 */
{
	if (main<0 || main>255 || sub<0 || sub>255) throw IllegalArgumentException();
	mainversion=(uint8_t)(main&0xff);
	subversion=(uint8_t)(sub&0xff);
}

void PFPFile::setId(const String &id)
/*!\brief ID des PFP-Files setzen
 *
 * Mit dieser Version wird die ID des PFP-Files festgelegt. Eine ID muss zwingend 4 Byte lang
 * sein und darf nur US-ASCII-Zeichen enthalten.
 *
 * \param id Pointer auf einen 4-Byte langen String, der mit 0 terminiert ist.
 * \exception IllegalArgumentException Wird geworfen, wenn die \p id einen ungültigen Wert enthält
 *
 * \since Version 6.1.0
 */
{
	if (id.len()!=4) throw IllegalArgumentException();
	for (size_t i=0;i<4;i++) {
		wchar_t c=id[i];
		if (c<32 || c>127) throw IllegalArgumentException();
	}
	this->id=id;
}

void PFPFile::setCompression(Compression::Algorithm type)
/*!\brief Kompression einstellen
 *
 * \desc
 * Mit dieser Funktion wird festgelegt ob und welche Kompression beim Speichern verwendet werden
 * soll.
 *
 * \param type Ein Wert, der die Art der Kompression angibt. Mögliche Werte sind:
 * - CCompression::Algo_NONE - Keine Komprimierung
 * - CCompression::Algo_ZLIB - Komprimierung mit Zlib
 * - CCompression::Algo_BZIP2 - Komprimierung mit Bzip2
 *
 * \exception UnknownCompressionMethodException Wird geworfen, wenn \p type einen ungültigen Wert enthält.
 */
{
	if (type>2 || type<0) throw UnknownCompressionMethodException();
	comp=type;
}

void PFPFile::setParam(const String &chunkname, const String &data)
/*!\brief Interne Funktion zum Speichern von vordefinierten Chunks
 *
 * \desc
 * Diese Funktion wird intern verwendet, um die vordefinierten Text-Chunks zu speichern. Sie
 * stellt sicher, dass jeder Chunk nur einmal vorkommt.
 *
 * \param chunkname Pointer auf den Namen des Chunks.
 * \param data Pointer auf den zu setzenden Text-String
 * \returns Bei Erfolg gibt die Funktion true (1) zurück, sonst false (0).
 * \see Die Funktion wird intern von folgenden Funktionen aufgerufen:
 * - PFPFile::SetName
 * - PFPFile::SetAuthor
 * - PFPFile::SetDescription
 * - PFPFile::SetCopyright
 *
 * \since Version 6.1.0
 */
{
	deleteChunk(chunkname);
	PFPChunk *c=new PFPChunk;
	try {
		c->setName(chunkname);
		c->setData(data);
		addChunk(c);
	} catch (...) {
		delete c;
		throw;
	}
}

void PFPFile::setAuthor(const String &author)
/*!\brief Author setzen
 *
 * Mit dieser Funktion wird automatisch ein Author-Chunk ("AUTH") angelegt. Dabei ist sichergestellt,
 * dass der Chunk nur ein einziges mal in der Datei vorkommt.
 *
 * \param author Pointer auf einen Null-terminierten String mit dem Namen des Authors
 * \see
 * - PFPFile::SetAuthor
 * - PFPFile::SetCopyright
 * - PFPFile::SetDescription
 * - PFPFile::SetName
 *
 */
{
	setParam("AUTH",author);
}

void PFPFile::setCopyright(const String &copy)
/*!\brief Copyright setzen
 *
 * Mit dieser Funktion wird automatisch ein Copyright-Chunk ("COPY") angelegt. Dabei ist sichergestellt,
 * dass der Chunk nur ein einziges mal in der Datei vorkommt.
 *
 * \param copy Pointer auf einen Null-terminierten String mit dem Copyright-Text
 *
 * \see
 * - PFPFile::SetAuthor
 * - PFPFile::SetCopyright
 * - PFPFile::SetDescription
 * - PFPFile::SetName
 *
 */
{
	setParam("COPY",copy);
}

void PFPFile::setDescription(const String &descr)
/*!\brief Description setzen
 *
 * Mit dieser Funktion wird automatisch ein Description-Chunk ("DESC") angelegt. Dabei ist sichergestellt,
 * dass der Chunk nur ein einziges mal in der Datei vorkommt.
 *
 * \param descr Pointer auf einen Null-terminierten String mit der Beschreibung
 * \see
 * - PFPFile::SetAuthor
 * - PFPFile::SetCopyright
 * - PFPFile::SetDescription
 * - PFPFile::SetName
 *
 */
{
	setParam("DESC",descr);
}

void PFPFile::setName(const String &name)
/*!\brief Name setzen
 *
 * \desc
 * Mit dieser Funktion wird automatisch ein Namens-Chunk ("NAME") angelegt. Dabei ist sichergestellt,
 * dass der Chunk nur ein einziges mal in der Datei vorkommt.
 *
 * \param name Pointer auf einen Null-terminierten String mit dem Namen des Files
 * \see
 * - PFPFile::SetAuthor
 * - PFPFile::SetCopyright
 * - PFPFile::SetDescription
 * - PFPFile::SetName
 */
{
	setParam("NAME",name);
}

String PFPFile::getName() const
/*!\brief Namen holen
 *
 * \desc
 * Diese Funktion liefert einen Pointer auf den Namen des Files zurück.
 * \returns Pointer auf den Namen oder NULL, wenn es keinen "NAME"-Chunk in der Datei gibt.
 * \see
 * - PFPFile::GetAuthor
 * - PFPFile::GetCopyright
 * - PFPFile::GetDescription
 * - PFPFile::GetName
 *
 * \since Version 6.1.0
 */
{
	Iterator it;
	PFPChunk *chunk=findFirstChunk(it,"NAME");
	if (!chunk) return String();
	return String((char*)chunk->chunkdata);
}

String PFPFile::getDescription() const
/*!\brief Pointer auf die Description holen
 *
 * Diese Funktion liefert einen Pointer auf die Beschreibung zurück.
 * \returns Pointer auf die Beschreibung oder NULL, wenn es keinen "DESC"-Chunk in der Datei gibt.
 *
 * \see
 * - PFPFile::GetAuthor
 * - PFPFile::GetCopyright
 * - PFPFile::GetDescription
 * - PFPFile::GetName
 *
 * \since Version 6.1.0
 */
{
	Iterator it;
	PFPChunk *chunk=findFirstChunk(it,"DESC");
	if (!chunk) return String();
	return String((char*)chunk->chunkdata);
}

String PFPFile::getAuthor() const
/*!\brief Pointer auf den Author holen
 *
 * Diese Funktion liefert einen Pointer auf den Author zurück
 * \returns Pointer auf den Author oder NULL, wenn es keinen "AUTH"-Chunk in der Datei gibt.
 * \see
 * - PFPFile::GetAuthor
 * - PFPFile::GetCopyright
 * - PFPFile::GetDescription
 * - PFPFile::GetName
 *
 * \since Version 6.1.0
 */
{
	Iterator it;
	PFPChunk *chunk=findFirstChunk(it,"AUTH");
	if (!chunk) return String();
	return String((char*)chunk->chunkdata);
}

String PFPFile::getCopyright() const
/*!\brief Pointer auf den Copyright-String holen
 *
 * Diese Funktion liefert einen Pointer auf den Copyright-Text des Files zurück.
 * \returns Pointer auf das Copyright oder NULL, wenn es keinen "COPY"-Chunk in der Datei gibt.
 * \see
 * - PFPFile::GetAuthor
 * - PFPFile::GetCopyright
 * - PFPFile::GetDescription
 * - PFPFile::GetName
 *
 * \since Version 6.1.0
 */
{
	Iterator it;
	PFPChunk *chunk=findFirstChunk(it,"COPY");
	if (!chunk) return String();
	return String((char*)chunk->chunkdata);
}

void PFPFile::getVersion(int *main, int *sub) const
/*!\brief Version auslesen
 *
 * Mit dieser Funktion wird die Version der Datei in die beiden Parameter kopiert
 * \param[out] main Pointer auf eine Integer-Variable, in der die Hauptversion geschrieben
 * werden soll
 * \param[out] sub Pointer auf eine Integer-Variable, in der die Unterversion geschrieben
 * werden soll
 *
 * \since Version 6.1.0
 *
 */
{
	if (main) *main=mainversion;
	if (sub) *sub=subversion;
}

const String &PFPFile::getID() const
/*!\brief ID auslesen
 *
 * Diese Funktion liefert einen Pointer auf die ID der Datei zurück
 * \returns Pointer auf die ID der Datei. Diese ist immer 4 Byte groß und mit einem 0-Byte
 * terminiert
 *
 * \since Version 6.1.0
 */
{
	return id;
}

int PFPFile::getMainVersion() const
/*!\brief Hauptversion auslesen
 *
 * Mit dieser Funktion wird die Hauptversion der Datei ausgelesen.
 * \returns Hauptversion als Interger
 *
 * \since Version 6.1.0
 */
{
	return mainversion;
}

int PFPFile::getSubVersion() const
/*!\brief Unterversion auslesen
 *
 * Mit dieser Funktion wird die Unterversion der Datei ausgelesen.
 * \returns Unterversion als Interger
 *
 * \since Version 6.1.0
 *
 */
{
	return subversion;
}

Compression::Algorithm PFPFile::getCompression() const
/*!\brief Kompressionsverfahren auslesen
 *
 * Mit dieser Funktion wird das eingestellte Kompressionsverfahren
 * ausgelesen.
 * \returns ID des Kompressionsverfahrens
 *
 * \see PFPFile::SetCompression
 * \since Version 6.1.0
 *
 */
{
	return comp;
}


void PFPFile::saveChunk(char *buffer, size_t &pp, PFPChunk *chunk)
{
	for (int i=0;i<4;i++) Poke8(buffer+pp+i,(unsigned int)(chunk->chunkname[i]));
	Poke32(buffer+pp+4,(int)chunk->chunksize+8);
	pp+=8;
	if (chunk->chunksize) {
		memcpy(buffer+pp,chunk->chunkdata,chunk->chunksize);
		pp+=chunk->chunksize;
	}
}

void PFPFile::save(const String &filename)
/*!\brief PFP-File speichern
 *
 * Mit dieser Funktion wird der Inhalt der PFPFile-Klasse in eine Datei geschrieben.
 * Dabei wird der Header und sämtliche Chunks zusammengefasst, gegebenenfalls komprimiert
 * (siehe PFPFile::SetCompression) und im Filesystem gespeichert. Der genaue Aufbau der Datei
 * wird weiter unten beschrieben.
 *
 * \param filename Die Funktion bekommt als einzigen Parameter einen Pointer auf den Dateinamen.
 * Es ist zu beachten, dass eine eventuell vorhandene gleichnamige Datei überschrieben wird.
 * \exception EmptyFileException Das File enthält keine Chunks, es gibt nichts zu speichern
 *
 * \remarks
 * Die Funktion stellt sicher, dass die Chunks in einer bestimmten Reihenfolge geschrieben
 * werden. Die vordefinierten Chunks mit Name, Author, Copyright und Beschreibung werden in
 * jedem Fall zuerst gespeichert, dann die restlichen Chunks.
 *
 * \par Aufbau der PFP-Datei
 *
 * \copydoc PFPFileVersion3
 *
 * \since Version 6.1.0
 */
{
	// Gespeichert wird nur, wenn die Datei Chunks enthält
	if (Chunks.num()==0) throw EmptyFileException();
	File ff;
	// Wir benötigen zuerst die Gesamtgröße aller Chunks
	size_t size=24;
	Iterator it;
	Chunks.reset(it);
	PFPChunk *chunk;
	try {
		while(Chunks.getNext(it)) {
			size+=8;
			size+=it.value()->chunksize;
		}
	} catch (...) {

	}
	// plus ENDF-Chunk
	size+=8;

	// Datei zusammenbauen
	char *p=(char*)malloc(size);
	if (!p) throw OutOfMemoryException();
	size_t hsize=24;
	strcpy(p,"PFP-File");
	Poke8(p+8,3);
	Poke8(p+9,(int)hsize);
	for (int i=0;i<4;i++) Poke8((p+10+i),(unsigned int)id[i]);
	Poke8(p+15,mainversion);
	Poke8(p+14,subversion);
	Poke8(p+16,comp);
	Poke8(p+17,0);
	Poke8(p+18,0);
	Poke8(p+19,0);
	Poke32(p+20,(uint32_t)GetTime());

	size_t pp=hsize;
	// Chunks zusammenfassen
	// Zuerst die vordefinierten, die wir am Anfang des Files wollen
	Chunks.reset(it);
	chunk=findFirstChunk(it,"NAME");
	if (chunk) saveChunk(p,pp,chunk);
	chunk=findFirstChunk(it,"AUTH");
	if (chunk) saveChunk(p,pp,chunk);
	chunk=findFirstChunk(it,"DESC");
	if (chunk) saveChunk(p,pp,chunk);
	chunk=findFirstChunk(it,"COPY");
	if (chunk) saveChunk(p,pp,chunk);
	// Restliche Chunks
	Chunks.reset(it);
	try {
		while(Chunks.getNext(it)) {
			PFPChunk *chunk=it.value();
			String cn=chunk->chunkname;
			// Vordefinierte Chunks müssen übergangen werden, da diese weiter oben schon
			// ausgelesen wurden
			if (cn!="NAME" && cn!="AUTH" && cn!="DESC" && cn!="COPY") {
				saveChunk(p,pp,chunk);
			}
		}
	} catch (...) {

	}
	strncpy(p+pp,"ENDF",4);
	Poke32(p+pp+4,0);
	pp+=8;

	void *save=NULL;
	size_t savesize=pp-hsize;
	// Komprimierung?
	Compression c;
	if (comp) {
		size_t dstlen=savesize+64;
		save=malloc(dstlen);
		if (!save) {
			free(p);
			throw OutOfMemoryException();
		}
		try {
			c.init(comp,Compression::Level_High);
			c.compress(save,&dstlen,p+hsize,savesize);
		} catch (...) {
			free(save);
			free(p);
			throw;
		}
		savesize=dstlen;
	}
	try {
		ff.open(filename,File::WRITE);
	} catch (...) {
		if (save) free(save);
		free(p);
		throw;
	}
	ff.write(p,hsize);
	if (comp) {
		char t[8];
		Poke32(t,(int)(pp-hsize));
		Poke32(t+4,(int)savesize);
		ff.write(t,8);
		ff.write(save,savesize);
	} else {
		ff.write(p+hsize,pp-hsize);
	}
	ff.close();
	if (save) free(save);
	free(p);
}

void PFPFile::addChunk(PFPChunk *chunk)
/*!\brief Chunk hinzufügen
 *
 * Mit dieser Funktion wird ein neuer Chunk in die Klasse hinzugefügt. Der Chunk muss von der
 * Anwendung mit "new" erstellt worden sein, einen Namen haben. Ist dies nicht der Fall,
 * gibt die Funktion eine Fehlermeldung zurück.
 *
 * Sobald der Chunk mit AddChunk an die PFPFile-Klasse übergeben wurde, wird er von der Klasse
 * verwaltet und gegebenenfalls auch gelöscht. Die Anwendung braucht kein "delete" darauf zu
 * machen.
 *
 * \param chunk Pointer auf den hinzuzufügenden Chunk
 * \returns Bei Erfolg gibt die Funktion true (1) zurück, sonst false (0). Ein passender Fehlercode
 * wird gesetzt.
 * \remarks Es ist möglich mehrere Chunks mit gleichem Namen hinzuzufügen. Der Chunk wird nur in der
 * Klasse hinzugefügt, nicht aber in die Datei geschrieben. Zum Speichern muss explizit die Funktion
 * PFPFile::Save aufgerufen werden.
 *
 * \example
 * \code
 * void *ptr=xxxxx;    // Pointer auf die Daten
 * int size=xxxx;      // Größe der Daten in Byte
 * ppl6::PFPFile file;
 * ppl6::PFPChunk *chunk=new ppl6::PFPChunk;
 * chunk->SetName("DATA");
 * chunk->SetData(ptr,size);
 * file.AddChunk(chunk);
 * \endcode
 *
 * \since Version 6.1.0
 */
{
	if (!chunk) throw NullPointerException();
	if (chunk->chunkname=="UNKN") throw IllegalArgumentException();
	Chunks.add(chunk);
}

void PFPFile::deleteChunk(PFPChunk *chunk)
/*!\brief Bestimmten Chunk löschen
 *
 * Mit dieser Funktion wird ein bestimmter Chunk aus der Klasse gelöscht.
 *
 * \param chunk Pointer auf den zu löschenden Chunk
 * \returns Die Funktion liefert bei Erfolg true (1) zurück, sonst false. Ein Fehler kann nur
 * auftreten, wenn der Chunk garnicht im PFPFile vorhanden war.
 * \remarks
 * Alternativ kann auch einfach ein "delete" auf den Chunk gemacht werden.
 *
 * \see PFPFile::DeleteChunk(const char *name)
 *
 * \since Version 6.1.0
 */
{
	if (!chunk) throw NullPointerException();
	Chunks.erase (chunk);
}

void PFPFile::deleteChunk(const String &chunkname)
/*!\brief Chunk nach Namen löschen
 *
 * Mit dieser Funktion werden alle Chunks gelöscht, die den angegebenen Namen haben
 *
 * \param chunkname Pointer auf den Namen des Chunks
 * \returns Die Funktion liefert true (1) zurück, wenn mindestens 1 Chunk gelöscht
 * wurde, sonst false (0).
 * \remarks
 *
 * \see PFPFile::DeleteChunk(PFPChunk *chunk)
 *
 * \since Version 6.1.0
 */
{
	if (chunkname.len()!=4) throw IllegalArgumentException();
	String s=chunkname;
	s.upperCase();
	for (size_t i=0;i<4;i++) {
		wchar_t c=s[i];
		if (c<32 || c>127) throw IllegalArgumentException();
	}
	Iterator it;
	PFPChunk *chunk;
	while ((chunk=findFirstChunk(it,s))) {
		Chunks.erase(chunk);
		delete chunk;
	}
}

PFPChunk *PFPFile::findFirstChunk(Iterator &it, const String &chunkname) const
/*!\brief Ersten Chunk mit einem bestimmten Namen finden
 *
 * Mit dieser und der Funktion PFPFile::FindNextChunk kann man sich durch alle Chunks mit einem
 * bestimmten Namen durchhangeln.
 *
 * \param chunkname Pointer auf den Namen des Chunks
 * \returns Bei Erfolg liefert die Funktion einen Pointer auf den ersten gefundenen Chunk zurück.
 * Wurde kein passender Chunk gefunden, wird NULL zurückgegeben und ein entsprechender Fehlercode
 * gesetzt.
 * \see
 * - PFPFile::FindFirstChunk
 * - PFPFile::FindNextChunk
 * - PFPFile::Reset
 * - PFPFile::GetFirst
 * - PFPFile::GetNext
 *
 */
{
	Chunks.reset(it);
	return findNextChunk(it,chunkname);
}

PFPChunk *PFPFile::findNextChunk(Iterator &it, const String &chunkname) const
/*!\brief Nächsten Chunk mit einem bestimmten Namen finden
 *
 * Mit dieser und der Funktion PFPFile::FindFirstChunk kann man sich durch alle Chunks mit einem
 * bestimmten Namen durchhangeln.
 *
 * \param chunkname Optionaler Pointer auf den Namen des Chunks. Wurde zuvor bereits
 * PFPFile::GetFirstChunk aufgerufen, muss kein Name angegeben werden.
 * \returns Bei Erfolg liefert die Funktion einen Pointer auf den nächsten gefundenen Chunk zurück.
 * Wurde kein passender Chunk gefunden, wird NULL zurückgegeben und ein entsprechender Fehlercode
 * gesetzt.
 * \see
 * - PFPFile::FindFirstChunk
 * - PFPFile::FindNextChunk
 * - PFPFile::Reset
 * - PFPFile::GetFirst
 * - PFPFile::GetNext
 *
 * \since Version 6.1.0
 */
{
	if (chunkname.notEmpty()) {
		it.findchunk=chunkname;
	}
	if (it.findchunk.len()!=4) throw IllegalArgumentException();
	while (Chunks.getNext(it)) {
		PFPChunk *chunk=it.value();
		if (it.findchunk==chunk->chunkname) return chunk;
	}
	return NULL;
}

void PFPFile::reset(Iterator &it) const
/*!\brief Zeiger zum Durchwandern der Chunks zurücksetzen
 *
 * Mit dieser Funktion wird der Zeiger, der beim Durchwandern der Chunks mit den Funktionen
 * FindNextChunk und GetNext verwendet wird, wieder auf den Anfang gesetzt.
 *
 * \see
 * - PFPFile::FindFirstChunk
 * - PFPFile::FindNextChunk
 * - PFPFile::Reset
 * - PFPFile::GetFirst
 * - PFPFile::GetNext
 *
 * \since Version 6.1.0
 */
{
	Chunks.reset(it);
}

PFPChunk *PFPFile::getFirst(Iterator &it) const
/*!\brief Pointer auf ersten Chunk holen
 *
 * Diese Funktion liefert einen Pointer auf den ersten Chunk in der Datei zurück.
 *
 * \returns Pointer auf den ersten Chunk oder NULL, wenn es keine Chunks gibt.
 * \see
 * - PFPFile::FindFirstChunk
 * - PFPFile::FindNextChunk
 * - PFPFile::Reset
 * - PFPFile::GetFirst
 * - PFPFile::GetNext
 *
 * \since Version 6.1.0
 */
{
	Chunks.reset(it);
	return getNext(it);
}

PFPChunk *PFPFile::getNext(Iterator &it) const
/*!\brief Pointer auf nächsten Chunk holen
 *
 * Diese Funktion liefert einen Pointer auf den nächsten Chunk in der Datei zurück.
 *
 * \returns Pointer auf den nächsten Chunk oder NULL, wenn es keine weiteren Chunks gibt.
 *
 * \see
 * - PFPFile::FindFirstChunk
 * - PFPFile::FindNextChunk
 * - PFPFile::Reset
 * - PFPFile::GetFirst
 * - PFPFile::GetNext
 *
 * \since Version 6.1.0
 */
{
	if (Chunks.getNext(it)) return it.value();
	return NULL;
}

void PFPFile::list() const
/*!\brief Chunks auf STDOUT auflisten
 *
 * Diese Funktion listet die Namen und Größen aller Chunks auf STDOUT aus.
 *
 * \since Version 6.1.0
 */
{
	printf("PFP-File Version 3 ============================================\n");
	printf("ID: %s, Version: %i.%i, Komprimierung: ",(const char*)id,mainversion,subversion);
	switch(comp) {
		case 0: printf ("keine\n");
			break;
		case 1: printf ("Zlib\n");
			break;
		case 2: printf ("Bzip2\n");
			break;
		default: printf ("unbekannt\n");
			break;
	}
	String Tmp;
	Tmp=getName();        if (Tmp.notEmpty()) printf("Name:        %s\n",(const char*)Tmp);
	Tmp=getAuthor();      if (Tmp.notEmpty()) printf("Author:      %s\n",(const char*)Tmp);
	Tmp=getDescription(); if (Tmp.notEmpty()) printf("Description: %s\n",(const char*)Tmp);
	Tmp=getCopyright();   if (Tmp.notEmpty()) printf("Copyright:   %s\n",(const char*)Tmp);

	Iterator it;
	Chunks.reset(it);
	printf ("\nChunks:\n");
	while(Chunks.getNext(it)) {
		PFPChunk *chunk=it.value();
		printf ("  %s: %zu Bytes\n",(const char*)chunk->chunkname,chunk->chunksize);
	}
	printf("===============================================================\n");
}


/*!\brief Prüfen, ob es sich um ein PFP-File handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei der Datei mit dem Namen \p file um eine Datei
 * im \ref PFPFileVersion3 PFP-Format Version 3 handelt. Ist dies der Fall, wird deren
 * ID und Version eingelesen.
 *
 * @param file Dateiname
 * @return Gibt \c true zurück, wenn es sich um eine Datei im PFP-Format handelt. Deren
 * ID kann anschließend mit PFPFile::getID ausgelesen werden, Version mit PFPFile::getVersion bzw.
 * PFPFile::getMainVersion und PFPFile::getSubVersion. Handelt es sich nicht um eine Datei
 * im PFP-Format, gibt die Funktion \c false zurück. Es wird keine Exception geworfen.
 */
bool PFPFile::ident(const String &file)
{
	File ff;
	try {
		ff.open(file,File::READ);
	} catch (...) {
		return false;
	}
	return ident(ff);
}

/*!\brief Prüfen, ob es sich um ein PFP-File handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei der geöffneten Datei \p ff um eine Datei
 * im \ref PFPFileVersion3 PFP-Format Version 3 handelt. Ist dies der Fall, wird deren
 * ID und Version eingelesen.
 *
 * @param ff Referenz auf eine geöffnete Datei
 * @return Gibt \c true zurück, wenn es sich um eine Datei im PFP-Format handelt. Deren
 * ID kann anschließend mit PFPFile::getID ausgelesen werden, Version mit PFPFile::getVersion bzw.
 * PFPFile::getMainVersion und PFPFile::getSubVersion. Handelt es sich nicht um eine Datei
 * im PFP-Format, gibt die Funktion \c false zurück. Es wird keine Exception geworfen.
 */
bool PFPFile::ident(FileObject &ff)
{
	try {
		const char *p;
		p=ff.map(0,24);
		if (strncmp(p,"PFP-File",8)!=0) return false;
		if (Peek8(p+8)!=3) return false;
		id.set(p+10,4);
		mainversion=Peek8(p+15);
		subversion=Peek8(p+14);
		comp=(Compression::Algorithm)Peek8(p+16);
		return true;
	} catch (...) {
		return false;
	}
	return false;
}



void PFPFile::load(const String &file)
/*!\brief PFP-File laden
 *
 * Mit dieser Funktion wird ein PFP-File in die Klasse geladen. Dabei wird zuerst der Header geladen
 * und überprüft, ob es sich um ein gültiges PFP-File handelt. Anschließend wird geprüft, ob der
 * Datenbereich komprimiert ist und gegebenenfalls dekomprimiert. Erst danach werden die
 * einzelnen Chunks eingelesen. Kommt es dabei zu Fehlern durch ungültige Chunks, werden diese
 * ignoriert und die Funktion gibt den Fehlercode 434 zurück.
 *
 * \param file Pointer auf den Namen der Datei, die geladen werden soll.
 * \returns Konnte die Datei fehlerfrei eingelesen werden, gibt die Funktion true (1) zurück,
 * im Fehlerfall false (0). Ein entsprechender Fehlercode wird gesetzt.
 *
 * \remarks
 * Vor dem Laden der Datei wird die Funktion PFPFile::Clear aufgerufen, so dass eventuell vorher
 * vorhandene Daten verloren gehen.
 *
 * \since Version 6.1.0
 */
{
	File ff;
	ff.open(file,File::READ);
	load(ff);
}

void PFPFile::load(FileObject &ff)
/*!\brief PFP-File laden
 *
 * Mit dieser Funktion wird ein PFP-File in die Klasse geladen. Dabei wird zuerst der Header geladen
 * und überprüft, ob es sich um ein gültiges PFP-File handelt. Dann wird die virtuelle Funktion
 * PFPFile::LoadRequest mit ID, Haupt- und Unterversion als Parameter aufgerufen. Liefert diese
 * nicht true (1) zurück, wird der Ladevorgang abgebrochen. Andernfalls wird fortgeführt
 * und geprüft, ob der
 * Datenbereich komprimiert ist und gegebenenfalls dekomprimiert. Erst danach werden die
 * einzelnen Chunks eingelesen. Kommt es dabei zu Fehlern durch ungültige Chunks, werden diese
 * ignoriert und die Funktion gibt den Fehlercode 434 zurück.
 *
 * \param ff Pointer auf eine CFile-Klasse, mit der die einzulesende Datei geöffnet wurde.
 * \returns Konnte die Datei fehlerfrei eingelesen werden, gibt die Funktion true (1) zurück,
 * im Fehlerfall false (0). Ein entsprechender Fehlercode wird gesetzt.
 *
 * \remarks
 * Vor dem Laden der Datei wird die Funktion PFPFile::Clear aufgerufen, so dass eventuell vorher
 * vorhandene Daten verloren gehen.
 *
 * \since Version 6.1.0
 */
{
	const char *p;
	try {
		p=ff.map(0,24);
	} catch (OverflowException &) {
		throw InvalidFormatException();
	}
	if (strncmp(p,"PFP-File",8)!=0) throw InvalidFormatException();
	if (Peek8(p+8)!=3) throw InvalidFormatException();
	size_t z,fsize;
	// Wir haben ein gültiges PFP-File, aber dürfen wir es auch laden?
	char tmpid[5];
	tmpid[4]=0;
	strncpy(tmpid,p+10,4);
	int t1,t2;
	t1=Peek8(p+15);
	t2=Peek8(p+14);
	if (!loadRequest(tmpid,t1,t2)) {
		throw AccessDeniedByInstanceException();
	}
	clear();
	id.set(p+10,4);
	mainversion=Peek8(p+15);
	subversion=Peek8(p+14);
	comp=(Compression::Algorithm)Peek8(p+16);
	size_t hsize=Peek8(p+9);
	char *u=NULL;
	if (comp) {
		p=(char*)ff.map(hsize,8);
		if (!p) throw ReadException();
		size_t sizeunk=Peek32(p);
		size_t sizecomp=Peek32(p+4);
		p=ff.map(hsize+8,sizecomp);
		if (!p) throw ReadException();
		u=(char*)malloc(sizeunk+1);
		if (!u) throw OutOfMemoryException();
		size_t dstlen=sizeunk;
		Compression c;
		try {
			c.init(comp);
			c.uncompress(u,&dstlen,p,sizecomp);
		} catch (...) {
			free(u);
			clear();
			throw;
		}
		if (dstlen!=sizeunk) {
			free(u);
			clear();
			throw DecompressionFailedException();
		}
		u[dstlen]=0;
		p=u;
		fsize=dstlen;
	} else {
		p=ff.map();
		p+=hsize;
		fsize=ff.size()-hsize;
	}
	// Wir haben nun den ersten Chunk ab Pointer p
	z=0;
	String Chunkname;
	try {
		size_t size=0;
		while ((z+=size)<fsize) {
			size=Peek32(p+z+4);
			if (strncmp(p+z,"ENDF",4)==0) break;
			if (!size) break;
			// Falls z+size über das Ende der Datei geht, stimmt mit diesem Chunk was nicht
			if (z+size>fsize) break;
			PFPChunk *chunk=new PFPChunk;
			if (!chunk) throw OutOfMemoryException();
			Chunkname.set(p+z,4);
			chunk->setName(Chunkname);
			chunk->setData(p+z+8,size-8);
			addChunk(chunk);
		}
	} catch (...) {
		if (u) free(u);
		clear();
		throw;
	}
	if (u) free(u);
}

int PFPFile::loadRequest(const String  &id, int mainversion ,int subversion)
/*!\brief Ladevorgang bestätigen
 *
 * Diese Funktion wird bei jedem Ladevorgang aufgerufen. Falls die Anwendung eine Klasse definiert
 * hat, die von PFPFile abgeleitet ist, kann sie an dieser Stelle den Ladevorgang abbrechen,
 * wenn die Datei nicht dem unterstützten Format entspricht.
 *
 * \param id Pointer auf die 4-Byte-ID des PFP-Files
 * \param mainversion Die Hauptversion der Datei
 * \param subversion Die Unterversion der Datei
 * \returns Die Funktion muss true (1) zurückliefern, wenn der Ladevorgang fortgesetzt werden
 * darf, oder false (0), wenn er abgebrochen werden soll. Optional kann auch ein Fehlercode
 * gesetzt werden. Wird dies nicht gemacht, setzt die Ladefunktion den Fehlercode 435.
 *
 */
{
	return 1;
}

}	// EOF namespace ppl7
