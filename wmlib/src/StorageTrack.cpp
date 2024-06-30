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


#include "wm_storage.h"
#include "wm_tracklist.h"
#include "wm_exceptions.h"
#include <map>


namespace de {
namespace pfp {
namespace winmusik {



/*!\class DataTrack
 * \ingroup WM3DataTypes
 * \brief Klasse zum Speichern von Track-Zuordnungen
 *
 * Jeder Tonträger enthält in der Regel mehrere Titel, die in einer bestimmten Reihenfolge
 * aufeinanderfolgen, die sogenannten \b Tracks.
 *
 * In dieser Datenklasse werden die für einen Track nötigen Informationen gespeichert.
 * Dabei handelt es sich um:
 * - Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * - Tonträger-ID (die Nummer des Tonträgers)
 * - Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * - TrackNummer (Die Position des Titels innerhalb der Seite des Tonträgers)
 * - Titel-ID (Referenz auf den eigentlichen Titel (DataTitle))
 *
 * Die Tracks werden innerhalb der Storage Klasse CTrackStore in einem binären Baum verwaltet.
 * Als Schlüssel wird ein zusammengesetzter Schlüssel aus Device, DeviceId, Page und Track
 * verwendet,
 * Der Datentyp ist daher neben CStorageItem auch von ppl6::CTreeItem abgeleitet.
 *
 *
 *\todo Eventuell könnte man einen 64-bittigen zusammengesetzten Schlüssel verwenden,
 * wodurch der Vergleich schneller und einfacher geht:
 * \code
 *	Key=((ppluint64)DeviceId<<56);
 *	Key|=((ppluint64)Device<<48);
 *	Key|=((ppluint64)Page<<16);
 *	Key|=Track;
 * \endcode
 * Die Frage ist, ob ein 64-Bit Shift in einer 32-Bit Umgebung funktioniert?
 */


/*!\var DataTrack::Device
 * \brief Art des Tonträgers
 *
 * Diese Variable gibt an, um welche Art Tonträger es sich handelt.
 *
 * \copydoc WM3File_Devices
 */

/*!\var DataTrack::DeviceId
 * \brief ID des Tonträgers
 *
 * In WinMusik werden alle Tonträger zur Verwaltung durchnummeriert. Diese Variable gibt an,
 * um welchen Tonträger es sich innerhalb einer Art handelt.
 */

/*!\var DataTrack::Page
 * \brief Seite des Tonträgers
 *
 * Manche Tonträger haben mehrere bespielbare Seiten. Eine Musikkassette besteht z.B. in der Regel
 * aus 2 Seiten, eine CD hingegen aus nur einer.
 *
 * Diese Variable gibt an, auf welcher Seite des Tonträgers sich der Titel befindet.
 */

/*!\var DataTrack::Track
 * \brief Position des Titels
 *
 * Diese Variable gibt an, an welcher Position sich der Titel innerhalb der Seite des Tonträgers
 * befindet.
 */

/*!\var DataTrack::TitleId
 * \brief Referenz auf den Titel-Datensatz (DataTitle)
 *
 * Diese Variable enthält die ID des eigentlichen Titels innerhalb der Klasse CStorageTitle.
 */

DataTrack::DataTrack()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse. Er setzt alle Variablen auf 0.
 */
{
	DeviceId=0;
	TitleId=0;
	Track=0;
	Device=0;
	Page=0;
	formatversion=1;
}

DataTrack::DataTrack(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track, uint32_t TitleId)
{
	formatversion=1;
	this->DeviceId=DeviceId;
	this->TitleId=TitleId;
	this->Track=Track;
	this->Device=Device;
	this->Page=Page;
}

void DataTrack::Clear()
/*!\brief Variablen auf 0 setzen.
 *
 * Mit dieser Funktion werden alle Variablen auf 0 gesetzt. Außerdem wird die Clear-Funktion der
 * Storage-Klasse CStorageItem::Clear aufgerufen.
 */
{
	DeviceId=0;
	TitleId=0;
	Track=0;
	Device=0;
	Page=0;
	CStorageItem::Clear();
	formatversion=1;
}


void DataTrack::CopyFrom(const DataTrack& other)
/*!\brief Daten kopieren
 *
 * Mit dieser Funktion werden die Daten einers anderen DataTrack Datensatzes in diesen hineinkopiert.
 * Bereits vorhandene Daten werden dabei überschrieben.
 *
 * \param[in] t Pointer auf einen anderen Datensatz vom Typ DataTrack
 *
 * \returns Konnten die Daten erfolgreich kopiert werden, liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \attention
 * Die Funktion ruft die Clear-Funktion auf, durch die auch die StorageDaten gelöscht werden!
 */
{
	Clear();
	CopyDataFrom(other);
	CopyStorageFrom(other);
}

void DataTrack::CopyDataFrom(const DataTrack& other)
{
	DeviceId=other.DeviceId;
	TitleId=other.TitleId;
	Track=other.Track;
	Device=other.Device;
	Page=other.Page;
}


void DataTrack::SetValue(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track, uint32_t TitleId)
/*!\brief Wert setzen
 *
 * Mit dieser Funktion wird der Wert der Klasse verändert.
 *
 * \param[in] Device ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in] Page Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * \param[in] Track TrackNummer (Die Position des Titels innerhalb der Seite des Tonträgers)
 * \param[in] TitleId Titel-ID (Referenz auf den eigentlichen Titel (DataTitle))
 * \returns Liefert immer 1 zurück
 *
 * \attention Der Wert darf nur geändert werden, wenn die Klasse noch nicht Teil eines Trees ist,
 * da sonst die Sortierung durcheinander gerät.
 */
{
	this->DeviceId=DeviceId;
	this->TitleId=TitleId;
	this->Track=Track;
	this->Device=Device;
	this->Page=Page;
}

void DataTrack::Export(ppl7::ByteArray& bin) const
{
	char* ptr=(char*)bin.malloc(12);
	ppl7::Poke8(ptr, Device);
	ppl7::Poke32(ptr + 1, DeviceId);
	ppl7::Poke8(ptr + 5, Page);
	ppl7::Poke16(ptr + 6, Track);
	ppl7::Poke32(ptr + 8, TitleId);
}


void DataTrack::Import(const ppl7::ByteArrayPtr& bin, int version)
{
	if (version < 1 || version>1) {
		throw UnknownRecordVersionException("%d", version);
	}
	// Die Größe muss genau 12 Byte betragen
	int size=(int)bin.size();
	if (size != 12) {
		throw InvalidRecordException("expected %d bytes, but got %d bytes", 12, size);
	}
	const char* ptr=(const char*)bin.ptr();
	Clear();
	Device=ppl7::Peek8(ptr);
	DeviceId=ppl7::Peek32(ptr + 1);
	Page=ppl7::Peek8(ptr + 5);
	Track=ppl7::Peek16(ptr + 6);
	TitleId=ppl7::Peek32(ptr + 8);
}



/*!\class CTrackStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Tracks
 *
 * Klasse zum Verwalten von DataTrack Datensätzen
 *
 */


/*!\var CTrackStore::Mutex
 * \brief Mutex der Klasse
 *
 * Die Klasse verwendet einen Mutex bei allen Zugriffen und ist somit Threadsicher.
 */

CTrackStore::CTrackStore()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse. Hat gegenwärtig keine Funktion.
 */
{
}

CTrackStore::~CTrackStore()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Stellt sicher, dass sämtlicher durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen wird. Dazu wird die Member-Funktion CTrackStore::Clear
 * aufgerufen.
 */
{
	Clear();
}

inline static uint64_t getKey(uint8_t Device, uint32_t DeviceId, uint8_t Page)
{
	return ((uint64_t)Device << 40) + ((uint64_t)DeviceId << 8) + Page;
}

void CTrackStore::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion kann der komplette durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen werden.
 */
{
	// Erfreulicherweise bringt die Tree-Klasse der PPL-Library schon eine Clear-Funktion mit,
	// die alle Datensätze löscht.
	Tracks.clear();
}

const char* CTrackStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return "TRAK";
}

void CTrackStore::SaveToStorage(DataTrack& t)
{
	ppl7::ByteArray bin;
	t.Export(bin);
	getStorage().Save(this, &t, bin);
}

DataTrack* CTrackStore::SaveToMemory(const DataTrack& t)
{
	uint64_t key=getKey(t.Device, t.DeviceId, t.Page);
	std::map<uint64_t, DataTrackMap>::iterator it;
	it=Tracks.find(key);
	if (it == Tracks.end()) {
		DataTrackMap trackmap;
		std::pair<std::map<uint64_t, DataTrackMap>::iterator, bool>newit=Tracks.insert(std::pair<uint64_t, DataTrackMap>(key, trackmap));
		it=newit.first;
	}
	DataTrackMap::iterator dtm_it=it->second.find(t.Track);
	if (dtm_it == it->second.end()) {
		std::pair<DataTrackMap::iterator, bool> newtrack_it;
		newtrack_it=it->second.insert(std::pair < uint16_t, DataTrack>(t.Track, t));
		return &newtrack_it.first->second;
	}
	dtm_it->second.CopyDataFrom(t);
	return &dtm_it->second;
}

bool CTrackStore::Exists(uint8_t Device, uint32_t DeviceId, uint8_t Page) const
{
	uint64_t key=getKey(Device, DeviceId, Page);
	std::map<uint64_t, DataTrackMap>::const_iterator it;
	it=Tracks.find(key);
	if (it == Tracks.end()) return false;
	return true;
}

DataTrack* CTrackStore::Find(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const
{
	uint64_t key=getKey(Device, DeviceId, Page);
	std::map<uint64_t, DataTrackMap>::const_iterator it;
	it=Tracks.find(key);
	if (it == Tracks.end()) return NULL;
	DataTrackMap::const_iterator dtm_it=it->second.find(Track);
	if (dtm_it == it->second.end()) return NULL;
	return (DataTrack*)&dtm_it->second;
}

bool CTrackStore::Exists(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const
{
	DataTrack* track=Find(Device, DeviceId, Page, Track);
	if (track) return true;
	return false;
}


void CTrackStore::Delete(const DataTrack& entry)
{
	Delete(entry.Device, entry.DeviceId, entry.Page, entry.Track);
}

void CTrackStore::Delete(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track)
{
	uint64_t key=getKey(Device, DeviceId, Page);
	std::map<uint64_t, DataTrackMap>::iterator it;
	it=Tracks.find(key);
	if (it == Tracks.end()) return;
	DataTrackMap::iterator dtm_it=it->second.find(Track);
	if (dtm_it == it->second.end()) return;
	getStorage().Delete(this, &dtm_it->second);
	it->second.erase(dtm_it);
}

const  DataTrack& CTrackStore::Put(const DataTrack& entry)
/*!\brief Datensatz speichern
 *
 * Mit dieser Funktion wird ein veränderter oder neuer Datensatz im Speicher der Anwendung und
 * auf Festplatte gespeichert.
 *
 * \param[in] entry Pointer auf den gewünschten Datensatz vom Typ DataTrack. Dieser wird nicht
 * selbst gespeichert, sondern es wird eine Kopie erstellt. Der Wert muss daher von der Anwendung
 * nach Gebrauch wieder freigegeben werden.
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 * \note Es ist sichergestellt, dass der interne Suchbaum stets sortiert ist, auch wenn sich der
 * Text des Elements geändert hat.
 */
{
	DataTrack* t=SaveToMemory(entry);
	SaveToStorage(*t);
	return *t;
}

const DataTrack* CTrackStore::GetPtr(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const
/*!\brief Datensatz finden
 *
 * Mit dieser Funktion kann ein bestimmer Track ausgelesen werden
 *
 * \param[in] Device ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in] Page Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * \param[in] Track TrackNummer (Die Position des Titels innerhalb der Seite des Tonträgers)
 * \returns Wurde der gewünschte Track gefunden, wird ein Pointer auf den
 * DataTrack Datensatz zurückgeliefert. Wurde er nicht
 * gefunden, wird NULL zurückgegeben.
 */
{
	return Find(Device, DeviceId, Page, Track);
}

const DataTrack& CTrackStore::Get(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const
/*!\brief Datensatz finden
 *
 * Mit dieser Funktion kann ein bestimmer Track ausgelesen werden
 *
 * \param[in] Device ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in] Page Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * \param[in] Track TrackNummer (Die Position des Titels innerhalb der Seite des Tonträgers)
 * \returns TODO
 */
{
	const DataTrack* t=Find(Device, DeviceId, Page, Track);
	if (!t) {
		throw RecordDoesNotExistException("Device=%d, DeviceId=%d, Page=%d, Track=%d",
			Device, DeviceId, Page, Track);
	}
	return (*t);
}

CTrackList CTrackStore::GetTracklist(uint8_t Device, uint32_t DeviceId, uint8_t Page) const
/*!\brief Trackliste für einen bestimmten Datenträger erstellen
 *
 * Diese Funktion erstellt eine Trackliste für den gewünschten Datenträger. Dabei werden
 * die Original Datensätze referenziert, es werden keine Kopien gemacht.
 * \param[in] Device ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in] Page Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * \returns TODO
 */
{
	CTrackList list((CTrackStore*)this);
	list.DeviceType=Device;
	list.DeviceId=DeviceId;
	list.Page=Page;
	uint64_t key=getKey(Device, DeviceId, Page);
	std::map<uint64_t, DataTrackMap>::const_iterator it;
	it=Tracks.find(key);
	if (it != Tracks.end()) {
		DataTrackMap::const_iterator dtm_it;
		for (dtm_it=it->second.begin();dtm_it != it->second.end();++dtm_it) {
			list.Add(dtm_it->first, dtm_it->second);
		}
	}
	return list;
}


void CTrackStore::LoadChunk(const CWMFileChunk& chunk)
{
	DataTrack data;
	ppl7::ByteArrayPtr bin(chunk.GetChunkData(), chunk.GetChunkDataSize());
	data.Import(bin, chunk.GetFormatVersion());
	data.CopyStorageFrom(chunk);
	SaveToMemory(data);
}



}
}
}
