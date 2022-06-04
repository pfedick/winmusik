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
#include "wm_normalizer.h"

namespace de {
namespace pfp {
namespace winmusik {

/*!\class DataVersion
 * \brief Datentyp für Musikversion
 * \ingroup WM3DataTypes
 */

/*!\class DataRecordSource
 * \brief Datentyp für Aufnahmequellen
 * \ingroup WM3DataTypes
 */

/*!\class DataLabel
 * \brief Datentyp für Plattenlabels
 * \ingroup WM3DataTypes
 */

/*!\class DataPurchaseSource
 * \brief Datentyp für Kaufquellen
 * \ingroup WM3DataTypes
 */

/*!\class DataRecordDevice
 * \brief Datentyp für Aufnahmegeräte
 * \ingroup WM3DataTypes
 */

/*!\class DataGenre
 * \brief Datentyp für Musik Genres
 * \ingroup WM3DataTypes
 */


/*!\class CSimpleTable
 * \brief Basisklasse für verschiedene "Index-Wert" Datentypen
 *
 * Dies ist die Basisklasse für verschiedene "Index-Wert" Datentypen. Dazu zählen:
 * - CVersion
 * - CRecordSource
 * - CLabel
 * - CPurchaseSource
 * - CRecordDevice
 * - CGenre
 *
 * Es handelt sich hier um einen einfachen Datentyp, der nur aus einem nummerischen Index und einem String
 * als Wert besteht. Der Datentyp ist von ppl6::CTreeItem abgeleitet und kann somit in einem Binären Baum
 * verwaltet werden. Als Schlüssel dient dazu der Wert des Strings.
 */


/*!\var CSimpleTable::Id
 * \brief ID des Datensatzes
 */

/*!\var CSimpleTable::References
 * \brief Anzahl Referenzierungen auf diesen Datensatz
 */

/*!\var CSimpleTable::Value
 * \brief Pointer auf den Text dieses Datensatzes
 */

CSimpleTable::CSimpleTable()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse
 */
{
	Id=0;
	References=0;
	formatversion=1;
}

CSimpleTable::CSimpleTable(uint32_t id, const ppl7::String& value, uint32_t references)
{
	formatversion=1;
	this->Id=id;
	this->Value=value;
	this->References=references;
}

CSimpleTable::CSimpleTable(const CSimpleTable& other)
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse
 */
	: CStorageItem(other)
{
	CopyDataFrom(other);
}

CSimpleTable& CSimpleTable::operator = (const CSimpleTable& other)
{
	formatversion=1;
	CopyFrom(other);
	return *this;
}

CSimpleTable::~CSimpleTable()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Er sorgt dafür, dass der durch den Wert allokierte Speicher wieder freigegeben wird.
 */
{
	Clear();
}


void CSimpleTable::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion wird der durch diese Klasse allokierte Speicher wieder freigegeben und
 * alle internen Variablen deinitialisiert.
 */
{
	Id=0;
	References=0;
	Value.clear();
	CStorageItem::Clear();
	formatversion=1;
}

void CSimpleTable::CopyFrom(const CSimpleTable& other)
/*!\brief Daten kopieren
 *
 * Mit dieser Funktion werden die Daten einers anderen CSimpleTable Datensatzes in diesen hineinkopiert.
 * Bereits vorhandene Daten werden dabei überschrieben.
 *
 * \param[in] t Pointer auf einen anderen Datensatz vom Typ CSimpleTable
 *
 * \returns Konnten die Daten erfolgreich kopiert werden, liefert die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	Clear();
	CopyDataFrom(other);
	CopyStorageFrom(other);
}

void CSimpleTable::CopyDataFrom(const CSimpleTable& other)
{
	Id=other.Id;
	References=other.References;
	Value=other.Value;
}


void CSimpleTable::SetValue(const ppl7::String& value)
/*!\brief Wert setzen
 *
 * Mit dieser Funktion wird der Wert der Klasse verändert.
 *
 * \param[in] value Pointer auf den String, der übernommen werden soll
 * \returns Im Erfolgsfall wird 1 zurückgegeben, sonst 0
 *
 * \attention Der Wert darf nur geändert werden, wenn die Klasse noch nicht Teil eines Trees ist,
 * da sonst die Sortierung durcheinander gerät.
 */
{
	this->Value=value;
}

const ppl7::String& CSimpleTable::GetValue() const
{
	return Value;
}

void CSimpleTable::Export(ppl7::ByteArray& bin) const
/*!\brief Binäre Exportfunktion
 *
 * Mit dieser Funktion werden die Daten der Klasse in binärer Form exportiert. Das Format ist
 * folgendermassen:
 *
 * \copydoc WM3File_Chunk_TABL
 *
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf eine neue ppl6::CBinary Klasse zurück.
 * Diese muss nach Gebrauch vom aufrufenden Programm selbst gelöscht werden. Im Fehlerfall wird NULL
 * zurückgegeben.
 */
{
	// Zunächst den benötigten Speicher berechnen
	int size=9;
	int lenValue=Value.size();
	if (lenValue) size+=lenValue;
	char* a=(char*)bin.malloc(size);
	ppl7::Poke32(a, Id);
	ppl7::Poke32(a + 4, References);
	if (lenValue) strcpy(a + 8, Value);
	else ppl7::Poke8(a + 8, 0);
}

void CSimpleTable::Import(const ppl7::ByteArrayPtr& bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion werden binäre gespeicherte Daten in die Klasse importiert. Eine Beschreibung des
 * Binären Formats ist in der Funktion CSimpleTable::Export zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (version < 1 || version>1) {
		throw UnknownRecordVersionException("SimpleTable, got version %d", version);
	}
	int size=bin.size();
	const char* a=(char*)bin.ptr();
	// Die Größe muss mindestens 9 Byte betragen
	if (size < 9 || a == NULL) {
		throw InvalidRecordException();
	}
	Clear();
	Id=ppl7::Peek32(a);
	References=ppl7::Peek32(a + 4);
	if (ppl7::Peek8(a + 8) != 0) Value.set(a + 8, size - 8);
}


/*!\class CTableStore
 * \brief Klasse zum Verwalten von CSimpleTable Datensätzen
 *
 * Klasse zum Verwalten von CSimpleTable Datensätzen
 *
 */

/*!\var CTableStore::Mutex
 * \brief Mutex der Klasse
 *
 * Die Klasse verwendet einen Mutex bei allen Zugriffen und ist somit Threadsicher.
 */

/*!\var CTableStore::Tree
 * \brief Interner Binary Tree zur Optimierung der Textsuche
 *
 * Interner Binary Tree zur Optimierung der Textsuche
 */

/*!\var CTableStore::TableIndex
 * \brief Tabelle mit Pointern auf die eigentlichen Datensätze.
 *
 * Tabelle mit Pointern auf die eigentlichen Datensätze vom Typ CSimpleTable.
 * Diese Tabelle wird intern von der Klasse verwaltet und automatisch erweitert.
 * Jede ID entspricht einem Eintrag in der Tabelle. Ein NULL-Wert bedeutet, dass
 * es zu dieser ID noch keinen Datensatz gibt.
 */

/*!\var CTableStore::max
 * \brief Größe der internen Tabelle CTableStore::TableIndex
 *
 * Dieser Wert gibt an, wieviele Einträge die interne Pointer-Tabelle maximal enthalten kann.
 * Der Wert ist somit identisch mit der höchst möglichen ID. Soll ein Datensatz mit einer höheren
 * ID gespeichert werden, wird die Tabelle automatisch erweitert.
 */

/*!\var CTableStore::highestId
 * \brief Enthält die höchste bisher verwendete ID in der Tabelle.
 *
 * Enthält die höchste bisher verwendete ID in der Tabelle.
 */

CTableStore::CTableStore()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse
 */
{
	TableIndex=NULL;
	max=0;
	highestId=0;
	size=0;
}

CTableStore::~CTableStore()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Stellt sicher, dass sämtlicher durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen wird. Dazu wird die Member-Funktion CTableStore::Clear
 * aufgerufen.
 */
{
	Clear();
}

void CTableStore::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion kann der komplette durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen werden.
 */
{
	if (TableIndex) {
		for (uint32_t i=0;i < max;i++) {
			if (TableIndex[i] != NULL) delete TableIndex[i];
		}
		free(TableIndex);
	}
	TableIndex=NULL;
	max=0;
	highestId=0;
	size=0;
	Words.clear();
	Tree.clear();
}

const char* CTableStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return NULL;
}

void CTableStore::Increase(uint32_t maxid)
{
	uint32_t h=maxid + 10000;
	CSimpleTable** t=(CSimpleTable**)calloc(sizeof(CSimpleTable*), h);
	if (!t) throw ppl7::OutOfMemoryException();
	if (TableIndex) {
		memcpy(t, TableIndex, max * sizeof(CSimpleTable*));
		free(TableIndex);
	}
	TableIndex=t;
	max=h;
}

void CTableStore::SaveToStorage(CSimpleTable& t)
{
	CStorage& store=getStorage();
	ppl7::ByteArray bin;
	t.Export(bin);
	store.Save(this, &t, bin);
}


CSimpleTable* CTableStore::SaveToMemory(const CSimpleTable& t)
{
	uint32_t id=0;

	if (t.Id == 0) {
		// Wir haben einen neuen Eintrag und vergeben eine Id
		id=highestId + 1;
	} else {
		id=t.Id;
	}
	if (id >= max) {
		Increase(id);
	}

	// Gibt's den Eintrag schon?
	if (TableIndex[id]) {
		removeFromWordTree(id);
		removeFromTree(id);
	} else {
		TableIndex[id]=new CSimpleTable;
		if (!TableIndex[id]) {
			throw ppl7::OutOfMemoryException();
		}
		size++;
	}
	if (id > highestId) highestId=id;
	TableIndex[id]->CopyDataFrom(t);
	TableIndex[id]->Id=id;
	addToWordTree(id);
	ppl7::String search=ppl7::LowerCase(ppl7::Trim(t.Value));
	Tree.insert(std::pair<ppl7::String, uint32_t>(search, id));
	return TableIndex[id];
}

const CSimpleTable& CTableStore::Put(const CSimpleTable& entry)
/*!\brief Datensatz speichern
 *
 * Mit dieser Funktion wird ein veränderter oder neuer Datensatz im Speicher der Anwendung und
 * auf Festplatte gespeichert.
 *
 * \param[in] entry Pointer auf den gewünschten Datensatz vom Typ CSimpleTable. Dieser wird nicht
 * selbst gespeichert, sondern es wird eine Kopie erstellt. Der Wert muss daher von der Anwendung
 * nach Gebrauch wieder freigegeben werden. Ist der Wert CSimpleTable::Id == 0 wird automatisch eine
 * freie ID vergeben. Andernfalls wird der vorhandene Datensatz überschrieben.
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 * \note Es ist sichergestellt, dass der interne Suchbaum stets sortiert ist, auch wenn sich der
 * Text des Elements geändert hat.
 */
{
	CSimpleTable* new_entry=SaveToMemory(entry);
	SaveToStorage(*new_entry);
	return *new_entry;
}

const CSimpleTable& CTableStore::Get(uint32_t id) const
/*!\brief Datensatz auslesen
 *
 * Mit dieser Funktion kann der zu \p id zugehörige Datensatz ausgelesen werden.
 *
 * \param[in] id ID des gewünschten Datensatzes
 * \returns Liefert eine Referenz auf den gewünschten Datensatz zurück
 * \throws RecordDoesNotExistException Wird geworfen, wenn der Datensatz nicht vorhanden ist
 */
{
	if (id > highestId || TableIndex == NULL || TableIndex[id] == NULL) {
		throw RecordDoesNotExistException();
	}
	return *TableIndex[id];
}

const CSimpleTable* CTableStore::GetPtr(uint32_t id) const
/*!\brief Datensatz auslesen
 *
 * Mit dieser Funktion kann der zu \p id zugehörige Datensatz ausgelesen werden.
 *
 * \param[in] id ID des gewünschten Datensatzes
 * \returns Liefert eine Referenz auf den gewünschten Datensatz zurück
 * oder NULL, wenn der Datensatz nicht vorhanden ist
 */
{
	if (id > highestId || TableIndex == NULL || TableIndex[id] == NULL) {
		return NULL;
	}
	return TableIndex[id];
}

const ppl7::String& CTableStore::GetValue(uint32_t id) const
/*!\brief Datensatz auslesen
 *
 * Mit dieser Funktion kann der zu \p id zugehörige Datensatz ausgelesen werden.
 *
 * \param[in] id ID des gewünschten Datensatzes
 * \returns Liefert eine Referenz auf den gewünschten Datensatz zurück
 * \throws RecordDoesNotExistException Wird geworfen, wenn der Datensatz nicht vorhanden ist
 */
{
	if (id > highestId || TableIndex == NULL || TableIndex[id] == NULL) {
		return unknown;
	}
	return TableIndex[id]->Value;
}

void CTableStore::Delete(uint32_t id)
{
	CSimpleTable* t=(CSimpleTable*)GetPtr(id);
	if (!t) return;
	removeFromWordTree(id);
	removeFromTree(id);
	TableIndex[id]=NULL;
	getStorage().Delete(this, t);
	size--;
	delete t;
	if (id != highestId) return;
	uint32_t new_highestId=0;
	for (uint32_t i=1;i < highestId;i++) {
		if (TableIndex[i]) new_highestId=i;
	}
	highestId=new_highestId;
}


uint32_t CTableStore::MaxId() const
{
	return highestId;
}

uint32_t CTableStore::Capacity() const
{
	return max;
}

uint32_t CTableStore::Size() const
{
	return size;
}



const CSimpleTable* CTableStore::Find(const ppl7::String& value) const
/*!\brief Datensatz finden
 *
 * Mit dieser Funktion kann ein Datensatz anhand seines Textwertes gefunden werden.
 *
 * \param[in] value Pointer auf den zu suchenden String
 * \returns Wurde der angegebene String \p value gefunden, wird ein Pointer auf den
 * CSimpleTable Datensatz zurückgeliefert, der den Text enthält. Wurde er nicht
 * gefunden, wird NULL zurückgegeben.
 */
{
	std::map<ppl7::String, uint32_t>::const_iterator it;
	ppl7::String search=ppl7::LowerCase(ppl7::Trim(value));
	it=Tree.find(search);
	if (it == Tree.end()) return NULL;
	return GetPtr(it->second);
}

uint32_t CTableStore::FindOrAdd(const ppl7::String& value)
{
	std::map<ppl7::String, uint32_t>::const_iterator it;
	ppl7::String search=ppl7::LowerCase(ppl7::Trim(value));
	it=Tree.find(search);
	if (it != Tree.end()) return it->second;
	// Neu anlegen
	CSimpleTable item;
	item.Id=0;
	item.References=1;
	item.SetValue(value);
	return Put(item).Id;
}

uint32_t CTableStore::GetId(const ppl7::String& value) const
{
	std::map<ppl7::String, uint32_t>::const_iterator it;
	ppl7::String search=ppl7::LowerCase(ppl7::Trim(value));
	it=Tree.find(search);
	if (it == Tree.end()) return 0;
	return it->second;
}

size_t CTableStore::FindAll(const ppl7::String& value, IndexTree& Result)
{
	ppl7::String search=ppl7::LowerCase(ppl7::Trim(value));
	Result.clear();
	std::map<ppl7::String, uint32_t>::const_iterator it;
	for (it=Tree.begin();it != Tree.end();++it) {
		if (it->first.instr(search) >= 0) Result.insert(it->second);
	}
	return Result.size();
}


void CTableStore::LoadChunk(const CWMFileChunk& chunk)
{
	CSimpleTable data;
	ppl7::ByteArrayPtr bin(chunk.GetChunkData(), chunk.GetChunkDataSize());
	data.Import(bin, chunk.GetFormatVersion());
	data.CopyStorageFrom(chunk);
	SaveToMemory(data);
}


void CTableStore::removeFromTree(uint32_t id)
{
	const CSimpleTable* t=GetPtr(id);
	if (!t) return;
	ppl7::String search=ppl7::LowerCase(ppl7::Trim(TableIndex[id]->Value));
	std::map<ppl7::String, uint32_t>::iterator it=Tree.find(search);
	if (it != Tree.end() && it->second == id) Tree.erase(it);
}


void CTableStore::removeFromWordTree(uint32_t id)
{
	const CSimpleTable* t=GetPtr(id);
	if (t == NULL || t->Value.isEmpty() == true) return;
	ppl7::Array words;
	if (GetNormalizer().GetWords(t->Value, words)) {
		for (size_t i=0;i < words.size();i++) {
			Words[words[i]].erase(id);
		}
	}
}

void CTableStore::addToWordTree(uint32_t id)
{
	const CSimpleTable* t=GetPtr(id);
	if (t == NULL || t->Value.isEmpty() == true) return;

	ppl7::Array words;
	if (GetNormalizer().GetWords(t->Value, words)) {
		for (size_t i=0;i < words.size();i++) {
			Words[words[i]].insert(id);
		}
	}
}

void CTableStore::makeUnion(IndexTree& Result, const IndexTree& Tree1, const IndexTree& Tree2)
{
	Result.clear();
	// Nur was in Tree1 und Tree2 vorhanden ist, wandert in Result
	const IndexTree* small, * big;
	if (Tree1.size() < Tree2.size()) {
		small=&Tree1;
		big=&Tree2;
	} else {
		small=&Tree2;
		big=&Tree1;
	}
	IndexTree::const_iterator it;
	for (it=small->begin();it != small->end();it++) {
		if (big->find(*it) != big->end()) Result.insert(*it);
	}
}

void CTableStore::copy(IndexTree& Result, const IndexTree& src)
{
	Result.clear();
	IndexTree::const_iterator it;
	for (it=src.begin();it != src.end();it++) {
		Result.insert(*it);
	}
}

size_t CTableStore::FindWords(const ppl7::String& words, IndexTree& Result)
{
	ppl7::Array w;
	if (GetNormalizer().GetWords(words, w)) {
		for (size_t i=0;i < w.size();i++) {
			std::map<ppl7::String, IndexTree >::const_iterator it=Words.find(w[i]);
			if (it != Words.end()) {
				if (!Result.size()) copy(Result, it->second);
				else {
					IndexTree res2;
					makeUnion(res2, Result, it->second);
					copy(Result, res2);
				}
			} else {
				Result.clear();
				return 0;
			}
		}
	}
	return Result.size();
}


/*!\class CVersionStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Musikversionen
 *
 * Klasse zum Verwalten von Musikversionen
 */

const char* CVersionStore::GetChunkName()
{
	return "VERS";
}

/*!\class CRecordSourceStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Aufnahmequellen
 *
 * Klasse zum Verwalten von Aufnahmequellen
 */

const char* CRecordSourceStore::GetChunkName()
{
	return "RSRC";
}

/*!\class CLabelStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Plattenlabels
 *
 * Klasse zum Verwalten von Plattenlabels
 */

const char* CLabelStore::GetChunkName()
{
	return "LABL";
}

/*!\class CPurchaseSourceStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Kaufquellen
 *
 * Klasse zum Verwalten von Kaufquellen
 */

const char* CPurchaseSourceStore::GetChunkName()
{
	return "PCHS";
}

/*!\class CRecordDeviceStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Aufnahmegeräten
 *
 * Klasse zum Verwalten von Aufnahmegeräten
 */

const char* CRecordDeviceStore::GetChunkName()
{
	return "RDEV";
}

/*!\class CGenreStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Musik Genres
 *
 * Klasse zum Verwalten von Musik Genres
 */

const char* CGenreStore::GetChunkName()
{
	return "GENR";
}


}
}
}
