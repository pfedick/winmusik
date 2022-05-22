/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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

#include "wm_cwmfile.h"
#include "wm_storage.h"
#include "wm_exceptions.h"

namespace de {
namespace pfp {
namespace winmusik {



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


DataShortcut::DataShortcut()
{
	formatversion=1;
}

DataShortcut::DataShortcut(const DataShortcut& other)
{
	shortcut=other.shortcut;
	artist=other.artist;
	formatversion=1;
}

void DataShortcut::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion wird der durch diese Klasse allokierte Speicher wieder freigegeben und
 * alle internen Variablen deinitialisiert.
 */
{
	shortcut.clear();
	artist.clear();
	CStorageItem::Clear();
	formatversion=1;
}

void DataShortcut::CopyDataFrom(const DataShortcut& other)
{
	shortcut=other.shortcut;
	artist=other.artist;
	formatversion=1;
}

void DataShortcut::CopyFrom(const DataShortcut& other)
{
	CopyDataFrom(other);
	CopyStorageFrom(other);
}

void DataShortcut::SetValue(const ppl7::String& shortcut, const ppl7::String& artist)
{
	this->shortcut=shortcut;
	this->artist=artist;
}

const ppl7::String& DataShortcut::GetShortcut() const
{
	return shortcut;
}

const ppl7::String& DataShortcut::GetArtist() const
{
	return artist;
}

void DataShortcut::Export(ppl7::ByteArray& bin) const
{
	// Benötigte Bytes berechnen
	int lenShortcut=shortcut.size();
	int lenArtist= artist.size();
	int size=4 + lenShortcut + lenArtist;
	// Speicher allokieren
	char* a=(char*)bin.malloc(size);
	ppl7::Poke16(a, lenShortcut);
	if (shortcut) strncpy(a + 2, shortcut, lenShortcut);
	ppl7::Poke16(a + 2 + lenShortcut, lenArtist);
	if (artist) strncpy(a + 4 + lenShortcut, artist, lenArtist);
}

void DataShortcut::Import(const ppl7::ByteArrayPtr& bin, int version)
{
	if (version < 1 || version>1) {
		throw UnknownRecordVersionException("Shortcut %d", version);
	}
	size_t size=bin.size();
	const char* a=(const char*)bin.ptr();
	// Die Größe muss mindestens 4 Byte betragen
	if (size < 4 || a == NULL) {
		throw InvalidRecordException();
	}
	Clear();
	int lenShortcut=ppl7::Peek16(a);
	int lenArtist=ppl7::Peek16(a + 2 + lenShortcut);
	if (lenShortcut) shortcut.set(a + 2, lenShortcut);
	if (lenArtist) artist.set(a + 4 + lenShortcut, lenArtist);
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

const char* CShortcutStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return "SHRT";
}

int CShortcutStore::Save(DataShortcut* t)
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
		ppl6::SetError(194, "int CShortcutStore::Save(==> DataShortcut *t <==)");
		return 0;
	}
	if (Storage->isDatabaseLoading()) return 1;
	ppl6::CBinary* bin=t->Export();
	if (!bin) return 0;
	if (!Storage->Save(this, t, bin)) {
		ppl6::PushError();
		delete bin;
		ppl6::PopError();
		return 0;
	}
	delete bin;
	return 1;
}

int CShortcutStore::Put(DataShortcut* entry)
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
		ppl6::SetError(194, "int CShortcutStore::Put(==> DataShortcut *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014, "CShortcutStore");
		return 0;
	}
	const char* shortcut=entry->GetShortcut();
	if (!shortcut) {
		ppl6::SetError(20038);
		return 0;
	}
	Mutex.Lock();
	// Gibt's die Abkürzung schon?

	DataShortcut* t;
	t=(DataShortcut*)Tree.Find((void*)shortcut);
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

DataShortcut* CShortcutStore::Get(const char* shortcut)
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
	const char* search=s.GetPtr();
	DataShortcut* t;
	Mutex.Lock();
	t=(DataShortcut*)Tree.Find((void*)search);
	Mutex.Unlock();
	return t;
}

int CShortcutStore::GetCopy(const char* shortcut, DataShortcut* t)
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
	const char* search=s.GetPtr();

	DataShortcut* res;
	Mutex.Lock();
	res=(DataShortcut*)Tree.Find((void*)search);
	if (res) {
		t->CopyFrom(res);
	}
	Mutex.Unlock();
	if (res) return 1;
	return 0;
}


int CShortcutStore::LoadChunk(CWMFileChunk* chunk)
{
	DataShortcut data;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk->GetChunkData(), chunk->GetChunkDataSize())) return 0;
	if (!data.Import(&bin, chunk->GetFormatVersion())) return 0;
	data.CopyStorageFrom(chunk);
	return Put(&data);
}


void CShortcutStore::Delete(const ppl7::String& shortcut)
{
	DataShortcut* sc;
	if (!Storage) {
		return 0;
	}
	Mutex.Lock();
	// Gibt's die Abkürzung?
	sc=(DataShortcut*)Tree.Find((void*)shortcut);
	if (!sc) {
		Mutex.Unlock();
		return 0;
	}
	if (!Storage->Delete(this, sc)) {
		Mutex.Unlock();
		return 0;
	}
	Mutex.Unlock();
	return 1;
}

CShortcutStore::iterator CShortcutStore::begin()
{
	return Tree.begin();
}

CShortcutStore::iterator CShortcutStore::end()
{
	return Tree.end();
}

CShortcutStore::const_iterator CShortcutStore::begin() const
{
	return Tree.begin();
}

CShortcutStore::const_iterator CShortcutStore::end() const
{
	return Tree.end();
}


}
}
}
