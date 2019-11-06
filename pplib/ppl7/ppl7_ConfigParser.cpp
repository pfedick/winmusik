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
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif

#include "ppl7.h"

namespace ppl7 {

typedef struct tagSections {
	struct tagSections *last, *next;
	String name;
	AssocArray values;
} SECTION;

/*!\class ConfigParser
 * \ingroup PPLGroupFileIO
 * \brief Lesen und Schreiben von Konfigurationsdateien
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Klasse können Konfigurationsdateien mit mehreren Sektionen gelesen und
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
root=42ee4d3cde1583d93b48923067696142
patrick=690669aaf38453f0c5676a4f16fe9d20

[allowedhosts]
# In dieser Sektion befinden sich IP-Adressen oder Hostnamen von den Rechnern,
# denen der Zugriff auf das Programm erlaubt ist.
192.168.0.1
192.168.0.2
192.168.0.3
192.168.0.4
\endcode
\par
Die Klasse kann folgendermassen verwendet werden:
\code
#include <ppl7.h>
int main(int argc, char **argv) {
	ppl7::ConfigParser conf;
	ppl7::String ip, path;
	int port;

	try {
		conf.load("my.conf");
		conf.selectSection("server");
		ip=conf.get("ip");
		port=conf.getInt("port",8080);
		// alternativ: port=conf.get("port","8080").toInt();
		path=conf.get("path");
	} catch (const ppl7::Exception &e) {
		cout << "Ein Fehler ist aufgetreten: " << e << "\n";
	}
	return 0;
}
\endcode
 */

ConfigParser::ConfigParser()
{
	setSeparator("=");
	first=last=section=NULL;
}

ConfigParser::ConfigParser(const String &filename)
{
	setSeparator("=");
	first=last=section=NULL;
	load(filename);
}

ConfigParser::ConfigParser(FileObject &file)
{
	setSeparator("=");
	first=last=section=NULL;
	load(file);
}

ConfigParser::~ConfigParser()
{
	unload();
}

void ConfigParser::unload()
{
	sections.clear();
	SECTION *s=(SECTION *)first;
	while (s) {
		s=s->next;
		delete ((SECTION *)first);
		first=s;
	}
	section=first=last=NULL;
}

void ConfigParser::setSeparator(const String &string)
{
	separator=string;
	if (separator.isEmpty()) separator="=";
}

const String &ConfigParser::getSeparator() const
{
	return separator;
}

void ConfigParser::selectSection(const String &sectionname)
{
	section=findSection(sectionname);
	if (!section) throw UnknownSectionException(sectionname);
}

void *ConfigParser::findSection(const String &sectionname) const
{
	SECTION *s=(SECTION *)first;
	while (s) {
		if (sectionname.strCaseCmp(s->name)==0) return (void *) s;
		s=s->next;
	}
	return NULL;
}

int ConfigParser::firstSection()
{
	section=first;
	if (section) return 1;
	return 0;
}

int ConfigParser::nextSection()
{
	SECTION *s=(SECTION *)section;
	if (!s) return 0;
	section=s->next;
	if (!section) return 0;
	return 1;
}

const String & ConfigParser::getSectionName() const
{
	if (!section) throw NoSectionSelectedException();
	return ((SECTION*)section)->name;
}


void ConfigParser::createSection(const String &sectionname)
{
	SECTION *s;
	s=(SECTION*)findSection(sectionname);
	if (s) {			// Section existiert bereits
		section=s;
		return;
	}

	s=new SECTION;
	if (!s) {
		throw OutOfMemoryException();
	}
	s->name=sectionname;
	s->next=NULL;
	s->last=(SECTION *) last;				// In die Kette haengen
	if (last) ((SECTION *)last)->next=s;
	last=s;
	if (!first) first=s;
	section=s;
}

void ConfigParser::deleteSection(const String &sectionname)
{
	SECTION *s=(SECTION *)findSection(sectionname);
	if (!s) return;
	if (s->last) s->last->next=s->next;		// aus der Kette nehmen
	if (s->next) s->next->last=s->last;
	if (s==(SECTION *)last) last=s->last;
	if (s==(SECTION *)first) first=s->next;
	delete(s);
	if(s==section) section=NULL;
}

void ConfigParser::reset()
{
	if (!section) return;
	return ((SECTION *)section)->values.reset(it);
}

bool ConfigParser::getFirst(String &key, String &value)
{
	if (!section) return false;
	return ((SECTION *)section)->values.getFirst(it,key,value);
}

bool ConfigParser::getNext(String &key, String &value)
{
	if (!section) return false;
	return ((SECTION *)section)->values.getNext(it,key,value);
}

void ConfigParser::add(const String &section, const String &key, const String &value)
{
	SECTION *s=(SECTION *)findSection(section);
	if (!s) {
		createSection(section);
		s=(SECTION *)findSection(section);
		if (!s) throw UnknownSectionException(section);
	}
	s->values.append(key,value,"\n");
}

void ConfigParser::add(const String &section, const String &key, const char *value)
{
	SECTION *s=(SECTION *)findSection(section);
	if (!s) {
		createSection(section);
		s=(SECTION *)findSection(section);
		if (!s) throw UnknownSectionException(section);
	}
	s->values.append(key,String(value),"\n");
}

void ConfigParser::add(const String &section, const String &key, int value)
{
	add(section,key,ToString("%i",value));
}

void ConfigParser::add(const String &section, const String &key, bool value)
{
	add(section,key,ToString("%s",(value?"yes":"no")));
}


void ConfigParser::add(const String &key, const String &value)
{
	if (!section) throw NoSectionSelectedException();
	add(((SECTION *)section)->name,key,value);
}

void ConfigParser::add(const String &key, const char *value)
{
	if (!section) throw NoSectionSelectedException();
	add(((SECTION *)section)->name,key,String(value));
}

void ConfigParser::add(const String &key, int value)
{
	if (!section) throw NoSectionSelectedException();
	add(((SECTION *)section)->name,key,value);
}

void ConfigParser::add(const String &key, bool value)
{
	if (!section) throw NoSectionSelectedException();
	add(((SECTION *)section)->name,key,value);
}

void ConfigParser::deleteKey(const String &key)
{
	if (!section) throw NoSectionSelectedException();
	return deleteKey(((SECTION *)section)->name,key);
}

void ConfigParser::deleteKey(const String &section, const String &key)
{
	SECTION *s=(SECTION *)findSection(section);
	if (!s) return;
	try {
		s->values.remove(key);
	} catch (const KeyNotFoundException &) {

	}
}

String ConfigParser::get(const String &key, const String &defaultvalue) const
{
	if (!section) throw NoSectionSelectedException();
	String ret;
	try {
		ret=((SECTION *)section)->values.getString(key);
	} catch (...) {
		return defaultvalue;
	}
	return ret;
}

String ConfigParser::getFromSection(const String &section, const String &key, const String &defaultvalue) const
{
	SECTION *s=(SECTION *)findSection(section);
	if (!s) return defaultvalue;
	String ret;
	try {
		ret=s->values.getString(key);
	} catch (...) {
		return defaultvalue;
	}
	return ret;
}

bool ConfigParser::getBoolFromSection(const String &section, const String &key, bool defaultvalue) const
{
	SECTION *s=(SECTION *)findSection(section);
	if (!s) return defaultvalue;
	String ret;
	try {
		ret=s->values.getString(key);
	} catch (...) {
		return defaultvalue;
	}
	return IsTrue(ret);
}

bool ConfigParser::getBool(const String &key, bool defaultvalue) const
{
	if (!section) throw NoSectionSelectedException();
	String ret;
	try {
		ret=((SECTION*)section)->values.getString(key);
	} catch (...) {
		return defaultvalue;
	}
	return IsTrue(ret);
}

int ConfigParser::getIntFromSection(const String &section, const String &key, int defaultvalue) const
{
	SECTION *s=(SECTION *)findSection(section);
	if (!s) return defaultvalue;
	String ret;
	try {
		ret=s->values.getString(key);
	} catch (...) {
		return defaultvalue;
	}
	return ret.toInt();
}

int ConfigParser::getInt(const String &key, int defaultvalue) const
{
	if (!section) throw NoSectionSelectedException();
	String ret;
	try {
		ret=((SECTION*)section)->values.getString(key);
	} catch (...) {
		return defaultvalue;
	}
	return ret.toInt();
}


const String& ConfigParser::getSection(const String &name) const
/*!\brief Inhalt einer Sektion als String
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird der komplette Inhalt einer Sektion als String
 * zurückgegeben, wie sie beim Laden der Konfiguration vorgefunden wurde, einschließlich
 * Kommentare. Zwischenzeitlich erfolgte Veränderungen, z.B. mit ConfigParser::add, sind
 * nicht enthalten.
 * \par
 * Die Funktion eignet sich gut, um Sektionen der Konfigdatei zu lesen, die keine
 * Key-Value-Paare enthält.
 *
 * \param section Der Name der Sektion ohne Eckige Klammern
 * \returns Im Erfolgsfall liefert die Funktion einen String mit dem Inhalt
 * der Sektion zurück. Im Fehlerfall wird eine Exception geworfen.
 *
 */
{
	//sections.List("configSections");
	return sections.getString(name);
}

/*!\brief Inhalt einer Sektion in einem AssocArray speichern
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird der Inhalt einer Sektion in ein AssocArray kopiert.
 *
 * \param target Assoziatives Array, in dem die Sektion gespeichert werden soll
 * \param section Der Name der Sektion ohne Eckige Klammern
 */
void ConfigParser::copySection(AssocArray &target, const String &section) const
{
	SECTION *s=(SECTION *)findSection(section);
	if (!s) throw UnknownSectionException(section);
	target.add(s->values);
}

/*!\brief Konfiguration auf STDOUT ausgeben
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird die derzeit vorhandene Konfiguration auf STDOUT ausgegeben.
 *
 */
void ConfigParser::print() const
{
	SECTION *s=(SECTION *)first;
	while (s) {
		printf ("[%s]\n",s->name.getPtr());
		s->values.list();
		printf ("\n");
		s=s->next;
	}
}

void ConfigParser::load(const String &filename)
/*!\brief Konfiguration aus einer Datei laden
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird eine Konfiguration aus einer Datei geladen.
 *
 * \param filename Dateiname
 */
{
	File ff;
	ff.open(filename,File::READ);
	load(ff);
}

/*!\brief Konfiguration aus dem Speicher laden
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird eine bereits im Speicher befindliche Konfiguration in das
 * ConfigParser-Objekt geladen.
 *
 * \param buffer Ein Pointer auf den Beginn des Speicherbereichs
 * \param bytes Die Größe des Speicherbereichs
 *
 */
void ConfigParser::loadFromMemory(const void *buffer, size_t bytes)
{
	if (!buffer) throw IllegalArgumentException("buffer");
	if (!bytes) throw IllegalArgumentException("bytes");
	MemFile ff;
	ff.open((void*)buffer,bytes);
	ff.load();
}

/*!\brief Konfiguration aus dem Speicher laden
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird eine bereits im Speicher befindliche Konfiguration in das
 * ConfigParser-Objekt geladen.
 *
 * \param ptr Referenz auf ein ByteArray oder ByteArrayPtr Objekt
 */
void ConfigParser::loadFromMemory(const ByteArrayPtr &ptr)
{
	MemFile ff;
	ff.open(ptr);
	ff.load();
}

void ConfigParser::loadFromString(const String &string)
/*!\brief Konfiguration aus einem String laden
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird eine in einem String enthaltene Konfiguration in das
 * ConfigParser-Objekt geladen.
 *
 * \param string Ein String, der die zu parsende Konfiguration enthält
 *
 */
{

	MemFile ff;
	ff.open((void*)string.getPtr(),string.size());
	ff.load();
}

/*!\brief Konfiguration aus einem FileObject-Objekt laden
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Funktion wird eine Konfiguration aus einem FileObject-Objekt geladen.
 * Die Funktion wird intern von den anderen Load-Funktionen verwendet.
 *
 * \param file Referenz auf eine FileObject-Klasse
 * \exception File::FileNotOpenException Wird geworfen, wenn das FileObject /p file keine
 * geöffnete Datei enthält
 * \exception Diverese Es können alle Exceptions auftreten, File::gets werfen kann
 *
 * \see loadFromString(const String &string)
 * \see load(const String &filename);
 * \see loadFromMemory(const void *buffer, size_t bytes)
 */
void ConfigParser::load(FileObject &file)
{
	unload();
	String key,value;
	String buffer,trimmedBuffer;
	String sectionname;

	size_t separatorLength=separator.length();

	if (!file.isOpen()) throw FileNotOpenException();
	//printf ("File open: %s, size: %tu\n",(const char*)file.filename(),file.size());

	try {
		while (!file.eof()) {			// Zeilen lesen, bis keine mehr kommt
			if (!file.gets(buffer,65535)) break;
			trimmedBuffer=buffer.trimmed();
			size_t l=trimmedBuffer.size();
			if (sectionname.notEmpty()) {
				if (l==0 || (l>0 && trimmedBuffer[0]!='[' && trimmedBuffer[l-1]!=']')) {
					sections.append(sectionname,buffer);
				}
			}


			if (l>0) {
				if (trimmedBuffer[0]=='[' && trimmedBuffer[l-1]==']') {	// Neue [Sektion] erkannt
					sectionname.clear();
					if (l<1024) {							// nur gültig, wenn < 1024 Zeichen
						sectionname=trimmedBuffer.mid(1,l-2);
						sections.append(sectionname,"","");
						createSection(sectionname);
					}
				} else if ((sectionname.notEmpty()) && trimmedBuffer[0]!='#' && trimmedBuffer[0]!=';') {	// Kommentare ignorieren
					size_t trenn=trimmedBuffer.instr(separator);			// Trennzeichen suchen
					if (trenn>0) {							// Wenn eins gefunden wurde, dann
						key=trimmedBuffer.left(trenn);				// Key ist alles vor dem Trennzeichen
						key.trim();
						value=trimmedBuffer.mid(trenn+separatorLength);	// Value der rest danach
						value.trim();
						add(sectionname,key,value);			// Und das ganze dann hinzufügen ins Array
					}
				}
			}
		}
	} catch (const ppl7::EndOfFileException) {
		section=NULL;
		return;
	}
	section=NULL;
}

/*!\brief Konfiguration in eine Datei speichern
 *
 * \header \#include <ppl7.h>
 * \desc
 * Der Inhalt des ConfigParser wird in die Datei \p filename gespeichert.
 *
 * \param filename Der Dateiname, unter dem die Konfiguration gespeichert werden soll
 * \exception Diverese Es können alle Exceptions auftreten, die File::open, File::puts und
 * File::putsf werfen kann.
 * \note
 * Eine eventuell schon vorhandene Datei wird überschrieben. Der ConfigParser speichert nur
 * die Sektionen mit den jeweiligen Key-Value-Paaren in alphabetischer Reihenfolge, ohne
 * Kommentare.
 *
 */
void ConfigParser::save(const String &filename)
{
	File ff;
	ff.open(filename,File::WRITE);
	save(ff);
}

/*!\brief Konfiguration in ein FileObject speichern
 *
 * \header \#include <ppl7.h>
 * \desc
 * Der Inhalt des ConfigParser wird in das FileObject \p file gespeichert.
 *
 * \param filename Der Dateiname, unter dem die Konfiguration gespeichert werden soll
 * \exception Diverese Es können alle Exceptions auftreten, File::puts und
 * File::putsf werfen kann.
 * \exception File::FileNotOpenException Wird geworfen, wenn das FileObject /p file keine
 * geöffnete Datei enthält
 *
 */
void ConfigParser::save(FileObject &file)
{
	AssocArray::Iterator it;
	String key, value;
	if (!file.isOpen()) throw FileNotOpenException();

	SECTION *s=(SECTION *)first;
	while (s) {
		if (s!=first) file.puts("\n");
		file.putsf("[%s]\n",s->name.getPtr());
		s->values.reset(it);
		while (s->values.getNext(it,key,value)) {
			if (value.instr("\n")>=0) {	// Value muss auf mehrere Zeilen aufgesplittet werden
				Array a;
				a.explode(value,"\n");
				for (size_t i=0;i<a.size();i++) {
					file.putsf("%s%s%s\n",(const char*)key,(const char*)separator,(const char*)a[i]);
				}

			} else {
				file.putsf("%s%s%s\n",(const char*)key,(const char*)separator,(const char*)value);
			}
		}
		s=s->next;
	}
}


} // end of namespace ppl7

