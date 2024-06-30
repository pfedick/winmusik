/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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
	: CStorageItem(other)
{
	shortcut=other.shortcut;
	artist=other.artist;
	formatversion=1;
}

DataShortcut::DataShortcut(const ppl7::String& shortcut, const ppl7::String& artist)
{
	formatversion=1;
	this->shortcut=shortcut;
	this->artist=artist;
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

DataShortcut& DataShortcut::operator = (const DataShortcut& other)
{
	CopyDataFrom(other);
	CopyStorageFrom(other);
	return *this;
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
	if (lenShortcut) strncpy(a + 2, shortcut, lenShortcut);
	ppl7::Poke16(a + 2 + lenShortcut, lenArtist);
	if (lenArtist) strncpy(a + 4 + lenShortcut, artist, lenArtist);
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
	Tree.clear();
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

void CShortcutStore::SaveToStorage(DataShortcut& t)
{
	CStorage& store=getStorage();
	ppl7::ByteArray bin;
	t.Export(bin);
	store.Save(this, &t, bin);
}

DataShortcut* CShortcutStore::SaveToMemory(const DataShortcut& t)
{
	// Gibt's die Abkürzung schon?
	ppl7::String search=ppl7::Trim(ppl7::LowerCase(t.shortcut));
	std::map<ppl7::String, DataShortcut>::iterator it;
	it=Tree.find(search);
	if (it != Tree.end()) {
		// Jepp, gibt's schon. Wir machen ein Update
		it->second.artist=t.artist;
		return &it->second;
	}
	std::pair<CShortcutStore::iterator, bool> new_it;
	new_it=Tree.insert(std::pair<ppl7::String, DataShortcut>(search, t));
	return &new_it.first->second;
}



void CShortcutStore::Put(const DataShortcut& entry)
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
	DataShortcut* t=SaveToMemory(entry);
	SaveToStorage(*t);
}

const DataShortcut& CShortcutStore::Get(const ppl7::String& shortcut) const
{
	// Make search string to lowercase
	ppl7::String search=ppl7::Trim(ppl7::LowerCase(shortcut));
	std::map<ppl7::String, DataShortcut>::const_iterator it;
	it=Tree.find(search);
	if (it == Tree.end())  throw RecordDoesNotExistException();
	return it->second;
}

const DataShortcut* CShortcutStore::GetPtr(const ppl7::String& shortcut) const
{
	// Make search string to lowercase
	ppl7::String search=ppl7::Trim(ppl7::LowerCase(shortcut));
	std::map<ppl7::String, DataShortcut>::const_iterator it;
	it=Tree.find(search);
	if (it == Tree.end()) return NULL;
	return &it->second;
}

size_t CShortcutStore::Size() const
{
	return Tree.size();
}

void CShortcutStore::LoadChunk(const CWMFileChunk& chunk)
{
	DataShortcut data;
	ppl7::ByteArrayPtr bin(chunk.GetChunkData(), chunk.GetChunkDataSize());
	data.Import(bin, chunk.GetFormatVersion());
	data.CopyStorageFrom(chunk);
	SaveToMemory(data);
}


void CShortcutStore::Delete(const ppl7::String& shortcut)
{
	ppl7::String search=ppl7::Trim(ppl7::LowerCase(shortcut));
	std::map<ppl7::String, DataShortcut>::iterator it;
	it=Tree.find(search);
	if (it != Tree.end()) {
		getStorage().Delete(this, &it->second);
		Tree.erase(it);
	}
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
