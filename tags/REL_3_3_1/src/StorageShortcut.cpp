/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: StorageShortcut.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "winmusik3.h"


/*!\class DataShortcut
 * \ingroup WM3DataTypes
 * \brief Klasse zum Speichern von Interpreten-Abkürzungen
 *
 * In dieser Datenklasse werden Abkürzungen zu Interpretennamen gespeichert.
 * Sie ist relativ einfach aufgebaut und enthält nur zwei Werte:
 * - Abkürzung
 * - Interpret
 *
 * Die Abkürzung dient als Schlüssel und wird von der Storage-Klasse CShortcutStore
 * in einem Tree gespeichert. Der Datentyp ist daher neben CStorageItem auch von
 * ppl6::CTreeItem abgeleitet.
 *
 */


/*!\var DataShortcut::shortcut
 * \brief Pointer auf String mit der Abkürzung
 */

/*!\var DataShortcut::artist
 * \brief Pointer auf String mit dem Interpreten
 */


DataShortcut::DataShortcut()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse
 */
{
	shortcut=NULL;
	artist=NULL;
	formatversion=1;
}

DataShortcut::DataShortcut(const DataShortcut &other)
{
	shortcut=NULL;
	artist=NULL;
	formatversion=1;
	CopyFrom(&other);
}

DataShortcut & DataShortcut::operator = (const DataShortcut &other)
{
	CopyFrom(&other);
	return *this;
}



DataShortcut::~DataShortcut()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Er sorgt dafür, dass der durch den Wert allokierte Speicher wieder freigegeben wird.
 */
{
	Clear();
}

void DataShortcut::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion wird der durch diese Klasse allokierte Speicher wieder freigegeben und
 * alle internen Variablen deinitialisiert.
 */
{
	if (shortcut) free(shortcut);
	if (artist) free(artist);
	shortcut=NULL;
	artist=NULL;
	CStorageItem::Clear();
	formatversion=1;
}

int DataShortcut::CompareNode(CTreeItem *item)
/*!\brief Elemente vergleichen
 *
 * \desc
 * Funktion zum Vergleichen zweier Baumelemente.
 * Bei dieser Funktion wird als Parameter ein Pointer auf ein Baum-Element erwartet.
 *
 * \param[in] item Pointer auf ein Baum-Element, mit dem dieses Element verglichen werden soll
 *
 * \returns Die Funktion liefert einen der folgenden Werte zurück:
 * - 0: Der Wert in \p item ist identisch mit dem Wert dieses Elements
 * - +1: Der Wert in \p item ist größer als der Wert dieses Elements
 * - -1: Der Wert in \p item ist kleiner als der Wert dieses Elements
 */
{
	DataShortcut *n=(DataShortcut *)item;
	int ret=strcmp(n->shortcut,shortcut);
	if (ret<0) return -1;
	if (ret>0) return 1;
	return 0;
}

int DataShortcut::CompareValue(void *value)
/*!\brief Elemente vergleichen
 *
 * \desc
 * Funktion zum Vergleichen zweier Baumelemente.
 * Bei dieser Funktion wird als Parameter ein void Pointer auf einen beliebigen
 * Wert erwartet.
 *
 * \param[in] value Pointer auf einen String.
 *
 * \returns Die Funktion liefert einen der folgenden Werte zurück:
 * - 0: Der Wert in \p value ist identisch mit dem Wert dieses Elements
 * - +1: Der Wert in \p value ist größer als der Wert dieses Elements
 * - -1: Der Wert in \p value ist kleiner als der Wert dieses Elements
 */
{
	const char *v=(const char *)value;
	int ret=strcmp(v,shortcut);
	if (ret<0) return -1;
	if (ret>0) return 1;
	return 0;
}

int DataShortcut::CopyFrom(const DataShortcut *t)
/*!\brief Daten kopieren
 *
 * Mit dieser Funktion werden die Daten einers anderen DataShortcut Datensatzes in diesen hineinkopiert.
 * Bereits vorhandene Daten werden dabei überschrieben.
 *
 * \param[in] t Pointer auf einen anderen Datensatz vom Typ DataShortcut
 *
 * \returns Konnten die Daten erfolgreich kopiert werden, liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \attention
 * Die Funktion ruft die Clear-Funktion auf, durch die auch die StorageDaten gelöscht werden!
 */
{
	if (!t) {
		ppl6::SetError(194,"int DataShortcut::CopyFrom(==> DataShortcut *t <==)");
		return 0;
	}
	Clear();
	if (t->shortcut) {
		shortcut=strdup(t->shortcut);
		if (!shortcut) {	// Out of Memory
			ppl6::SetError(2);
			return 0;
		}
	}
	if (t->artist) {
		artist=strdup(t->artist);
		if (!artist) {		// Out of Memory
			ppl6::SetError(2);
			return 0;
		}
	}
	CopyStorageFrom(t);
	return 1;
}

int DataShortcut::CopyFrom(const DataShortcut &t)
{
	return CopyFrom(&t);
}

int DataShortcut::SetValue(const char *shortcut, const char *artist)
/*!\brief Wert setzen
 *
 * Mit dieser Funktion wird der Wert der Klasse verändert.
 *
 * \param[in] shortcut Pointer auf den String, der die Abkürzung enthält
 * \param[in] artist Pointer auf den String, der den Interpreten enthält
 * \returns Im Erfolgsfall wird 1 zurückgegeben, sonst 0
 *
 * \attention Der Wert darf nur geändert werden, wenn die Klasse noch nicht Teil eines Trees ist,
 * da sonst die Sortierung durcheinander gerät.
 */
{
	if (this->shortcut) free(this->shortcut);
	if (this->artist) free(this->artist);
	this->shortcut=NULL;
	this->artist=NULL;
	if (shortcut) {
		// Der Shortcut wird immer in Kleinbuchstaben gespeichert
		ppl6::CWString s=shortcut;
		s.LCase();
		this->shortcut=strdup(s);
		if (!this->shortcut) {
			ppl6::SetError(2);
			return 0;
		}
	}
	if (artist) {
		this->artist=strdup(artist);
		if (!this->artist) {
			ppl6::SetError(2);
			return 0;
		}
	}
	return 1;
}

const char *DataShortcut::GetShortcut()
/*!\brief Abkürzung auslesen
 *
 * Mit dieser Funktion wird der String der Abkürzung ausgelesen.
 *
 * \returns Pointer auf einen 0-terminierten String mit der Abkürzung
 */
{
	return shortcut;
}

const char *DataShortcut::GetArtist()
/*!\brief Interpreten auslesen
 *
 * Mit dieser Funktion wird der String des Interpreten ausgelesen.
 *
 * \returns Pointer auf einen 0-terminierten String mit dem Namen des Interpreten
 */
{
	return artist;
}

ppl6::CBinary *DataShortcut::Export()
/*!\brief Binäre Exportfunktion
 *
 * Mit dieser Funktion werden die Daten der Klasse in binärer Form exportiert. Das Format ist
 * folgendermassen:
 *
 * \copydoc WM3File_Chunk_SHRT
 *
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf eine neue ppl6::CBinary Klasse zurück.
 * Diese muss nach Gebrauch vom aufrufenden Programm selbst gelöscht werden. Im Fehlerfall wird NULL
 * zurückgegeben.
 */
{
	// Zunächst den benötigten Speicher berechnen
	int size=4;		// 2*2 Byte für Längenangabe
	int lenShortcut=0;
	int lenArtist=0;

	if (shortcut) lenShortcut=strlen(shortcut);
	if (artist) lenArtist=strlen(artist);
	size=size+lenShortcut+lenArtist;
	char *a=(char*)malloc(size);
	if (!a) {
		ppl6::SetError(2);
		return NULL;
	}
	ppl6::Poke16(a,lenShortcut);
	if (shortcut) strncpy(a+2,shortcut,lenShortcut);
	ppl6::Poke16(a+2+lenShortcut,lenArtist);
	if (artist) strncpy(a+4+lenShortcut,artist,lenArtist);
	ppl6::CBinary *bin=new ppl6::CBinary;
	if (!bin) {
		ppl6::SetError(2);
		return NULL;
	}
	bin->Set(a,size);
	// CBinary ist verantwortlich den Speicher wieder freizugeben
	bin->ManageMemory();
	return bin;
}

int DataShortcut::Import(ppl6::CBinary *bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion werden binäre gespeicherte Daten in die Klasse importiert. Eine Beschreibung des
 * Binären Formats ist in der Funktion DataShortcut::Export zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (!bin) {
		ppl6::SetError(194,"int DataShortcut::Import(==> ppl6::CBinary *bin <==)");
		return 0;
	}
	if (version<1 || version>1) {
		ppl6::SetError(20023,"%i",version);
		return 0;
	}
	int size=bin->Size();
	const char *a=(const char*)bin->GetPtr();
	// Die Größe muss mindestens 4 Byte betragen
	if (size<4 || a==NULL) {
		ppl6::SetError(20026);
		return 0;
	}
	Clear();
	int lenShortcut=ppl6::Peek16(a);
	int lenArtist=ppl6::Peek16(a+2+lenShortcut);
	if (lenShortcut) shortcut=ppl6::strndup(a+2,lenShortcut);
	if (lenArtist) artist=ppl6::strndup(a+4+lenShortcut,lenArtist);
	return 1;
}

/*!\class CShortcutStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von DataShortcut Datensätzen
 *
 * Klasse zum Verwalten von DataShortcut Datensätzen mit Interpreten-Abkürzungen.
 *
 */


/*!\var CShortcutStore::Mutex
 * \brief Mutex der Klasse
 *
 * Die Klasse verwendet einen Mutex bei allen Zugriffen und ist somit Threadsicher.
 */

/*!\var CShortcutStore::Tree
 * \brief Interner Binary Tree zur Verwaltung der Datensätze
 *
 * Interner Binary Tree zur Verwaltung der Datensätze
 */

CShortcutStore::CShortcutStore()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse. Hat gegenwärtig keine Funktion.
 */
{
}

CShortcutStore::~CShortcutStore()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Stellt sicher, dass sämtlicher durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen wird. Dazu wird die Member-Funktion CShortcutStore::Clear
 * aufgerufen.
 */
{
	Clear();
}

void CShortcutStore::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion kann der komplette durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen werden.
 */
{
	// Erfreulicherweise bringt die Tree-Klasse der PPL-Library schon eine Clear-Funktion mit,
	// die alle Datensätze löscht.
	Mutex.Lock();
	Tree.Clear(true);
	Mutex.Unlock();
}

const char *CShortcutStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return "SHRT";
}

int CShortcutStore::Save(DataShortcut *t)
/*!\brief Datensatz auf Festplatte schreiben
 *
 * Diese Funktion speichert den übergebenen Datensatz auf die Festplatte. Dazu wird der Inhalt
 * zunächst als Binary exportiert (DataShortcut::Export) und damit CStorage::Save aufgerufen.
 * Die Funktion wird intern von CShortcutStore::Put verwendet.
 *
 * \param[in] t Pointer auf einen DataShortcut Datensatz
 * \returns Konnte der Datensatz erfolgreich gespeichert werden, liefert die Funktion 1 zurück, sonst
 * 0.
 */
{
	if (!Storage) {
		return 0;
	}
	if (!t) {
		ppl6::SetError(194,"int CShortcutStore::Save(==> DataShortcut *t <==)");
		return 0;
	}
	if (Storage->isDatabaseLoading()) return 1;
	ppl6::CBinary *bin=t->Export();
	if (!bin) return 0;
	if (!Storage->Save(this,t,bin)) {
		ppl6::PushError();
		delete bin;
		ppl6::PopError();
		return 0;
	}
	delete bin;
	return 1;
}

int CShortcutStore::Put(DataShortcut *entry)
/*!\brief Datensatz speichern
 *
 * Mit dieser Funktion wird ein veränderter oder neuer Datensatz im Speicher der Anwendung und
 * auf Festplatte gespeichert.
 *
 * \param[in] entry Pointer auf den gewünschten Datensatz vom Typ DataShortcut. Dieser wird nicht
 * selbst gespeichert, sondern es wird eine Kopie erstellt. Der Wert muss daher von der Anwendung
 * nach Gebrauch wieder freigegeben werden.
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 * \note Es ist sichergestellt, dass der interne Suchbaum stets sortiert ist, auch wenn sich der
 * Text des Elements geändert hat.
 */
{
	if (!entry) {
		ppl6::SetError(194,"int CShortcutStore::Put(==> DataShortcut *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014,"CShortcutStore");
		return 0;
	}
	const char *shortcut=entry->GetShortcut();
	if (!shortcut) {
		ppl6::SetError(20038);
		return 0;
	}
	Mutex.Lock();
	// Gibt's die Abkürzung schon?

	DataShortcut *t;
	t=(DataShortcut *)Tree.Find((void *)shortcut);
	if (t) {	// Jepp, gibt's schon. Wir machen ein Update
		if (t->artist) free(t->artist);
		if (entry->artist) t->artist=strdup(entry->artist);
		if (!Save(t)) {
			Mutex.Unlock();
			return 0;
		}
		// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
		entry->CopyStorageFrom(t);
		Mutex.Unlock();
		return 1;
	}
	// Nein, neue Abkürzung
	t=new DataShortcut;
	if (!t) {
		Mutex.Unlock();
		ppl6::SetError(2);
		return 0;
	}
	if (!t->CopyFrom(entry)) {
		ppl6::PushError();
		delete t;
		Mutex.Unlock();
		ppl6::PopError();
		return 0;
	}
	if (!Save(t)) {
		ppl6::PushError();
		delete t;
		Mutex.Unlock();
		ppl6::PopError();
		return 0;
	}
	// Den internen Datensatz in den Tree hängen
	Tree.Add(t);
	// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
	entry->CopyStorageFrom(t);
	Mutex.Unlock();
	return 1;
}

DataShortcut *CShortcutStore::Get(const char *shortcut)
/*!\brief Datensatz finden
 *
 * Mit dieser Funktion kann nach einer Abkürzung gesucht werden.
 *
 * \param[in] shortcut Pointer auf die zu suchende Abkürzung
 * \returns Wurde die angegebene Abkürzung \p shortcut gefunden, wird ein Pointer auf den
 * DataShortcut Datensatz zurückgeliefert, der den Text enthält. Wurde er nicht
 * gefunden, wird NULL zurückgegeben.
 */
{
	// Make search string to lowercase
	ppl6::CWString s=shortcut;
	s.LCase();
	const char *search=s.GetPtr();
	DataShortcut *t;
	Mutex.Lock();
	t=(DataShortcut *)Tree.Find((void *)search);
	Mutex.Unlock();
	return t;
}

int CShortcutStore::GetCopy(const char *shortcut, DataShortcut *t)
/*!\brief Kopie eines Datensatzes erstellen
 *
 * Mit dieser Funktion kann eine Kopie eines vorhandenen Datensatzes erstellt werden.
 *
 * \param[in] shortcut Pointer auf die zu suchende Abkürzung
 * \param[in,out] t Pointer auf eine Klasse vom Typ DataShortcut
 *
 * \returns Ist der Datensatz vorhanden und konnte kopiert werden, liefert die Funktion 1
 * zurück, sonst 0.
 */
{
	// Make search string to lowercase
	ppl6::CWString s=shortcut;
	s.LCase();
	const char *search=s.GetPtr();

	DataShortcut *res;
	Mutex.Lock();
	res=(DataShortcut *)Tree.Find((void *)search);
	if (res) {
		t->CopyFrom(res);
	}
	Mutex.Unlock();
	if (res) return 1;
	return 0;
}


int CShortcutStore::LoadChunk(CWMFileChunk *chunk)
{
	DataShortcut data;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk->GetChunkData(),chunk->GetChunkDataSize())) return 0;
	if (!data.Import(&bin,chunk->GetFormatVersion())) return 0;
	data.CopyStorageFrom(chunk);
	return Put(&data);
}


void CShortcutStore::List()
{
	DataShortcut *res;
	Mutex.Lock();
	Tree.Reset();
	while ((res=(DataShortcut*)Tree.GetNext())) {
		printf ("Shortcut: >>%s<< => >>%s<<\n",res->shortcut,res->artist);
	}
	Mutex.Unlock();
}


void CShortcutStore::Reset()
{
	Tree.Reset();
}

DataShortcut *CShortcutStore::GetFirst()
{
	return (DataShortcut*)Tree.GetFirst();
}

DataShortcut *CShortcutStore::GetNext()
{
	return (DataShortcut*)Tree.GetNext();
}

int CShortcutStore::Delete(const char *shortcut)
{
	DataShortcut *sc;
	if (!Storage) {
		return 0;
	}
	Mutex.Lock();
	// Gibt's die Abkürzung?
	sc=(DataShortcut *)Tree.Find((void *)shortcut);
	if (!sc) {
		Mutex.Unlock();
		return 0;
	}
	if (!Storage->Delete(this,sc)) {
		Mutex.Unlock();
		return 0;
	}
	Mutex.Unlock();
	return 1;
}
