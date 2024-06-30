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




/*!\class CTrackList
 * \brief Tracklisting
 *
 * Diese Klasse enthält eine Liste sämtlicher Tracks einer Seite eines Datenträgers. Sie wird bei Aufruf
 * der Funktion CTrackStore::GetTracklist erstellt. Sie ist fest mit den Storage-Klassen
 * verknüpft, eine Änderung eines Tracks in der Trackliste wirkt sich auch auf die gespeicherten
 * Daten aus.
 *
 * Die Aufrufende Anwendung muss die Klasse nach Gebrauch selbst mit delete freigeben.
 *
 */

/*!\var CTrackList::Mutex
 * \brief Mutex der Klasse
 *
 * Die Klasse verwendet einen Mutex bei allen Zugriffen und ist somit Threadsicher.
 */

/*!\var CTrackList::Tracks
 * \brief Binärer Baum mit den einzelnen Datensätzen
 */

/*!\var CTrackList::min
 * \brief Kleinste Tracknummer in dieser Liste
 */

/*!\var CTrackList::max
 * \brief Höchste Tracknummer in dieser Liste
 */

/*!\var CTrackList::storage
 * \brief Pointer auf die CTrackStore Klasse, von der diese Liste erstellt wurde
 */


CTrackList::CTrackList()
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor setzt alle Variablen auf 0.
 */
{
	storage=NULL;
	DeviceId=0;
	DeviceType=0;
	Page=0;
}

CTrackList::CTrackList(CTrackStore* storage)
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor setzt alle Variablen auf 0.
 */
{
	this->storage=storage;
	DeviceId=0;
	DeviceType=0;
	Page=0;
}


CTrackList::CTrackList(const CTrackList& other)
/*!\brief Copy Konstruktor der Klasse
 */
{
	storage=NULL;
	DeviceId=0;
	DeviceType=0;
	Page=0;
	copy(other);
}

void CTrackList::copy(const CTrackList& other)
{
	Clear();
	storage=other.storage;
	DeviceId=other.DeviceId;
	DeviceType=other.DeviceType;
	Page=other.Page;
	Tracks=other.Tracks;
}

CTrackList::~CTrackList()
/*!\brief Destruktor der Klasse
 *
 * Der Destruktor löscht den internen binären Suchbaum und gibt dessen Speicher wieder frei.
 */
{
	Clear();
}

CTrackList& CTrackList::operator=(const CTrackList& other)
{
	copy(other);
	return *this;
}

void CTrackList::Clear()
/*!\brief Daten löschen
 *
 * Diese Funktion löscht den internen binären Suchbaum und gibt dessen Speicher wieder frei.
 * Alle Variablen werden auf 0 gesetzt.
 */
{
	Tracks.clear();
}

int CTrackList::GetMin() const
/*!\brief Kleinste Tracknummer
 *
 * Diese Funktion liefert die kleinste Tracknummer der Liste zurück
 *
 * \returns Track-Nummer
 */
{
	if (Tracks.empty()) return 0;
	return Tracks.begin()->first;
}

int CTrackList::Num() const
{
	return (int)Tracks.size();
}

size_t CTrackList::Size() const
{
	return Tracks.size();
}

int CTrackList::GetMax() const
/*!\brief Größte Tracknummer
 *
 * Diese Funktion liefert die größte Tracknummer der Liste zurück
 *
 * \returns Track-Nummer
 */
{
	if (Tracks.empty()) return 0;
	return Tracks.rbegin()->first;
}

const DataTrack& CTrackList::Get(int track) const
{
	if (track < 1 || track >65535) {
		throw InvalidTrackNumberException("Track must be between 1 and 65535");
	}
	std::map<int, DataTrack>::const_iterator it;
	it=Tracks.find(track);
	if (it == Tracks.end()) throw RecordDoesNotExistException();
	return it->second;
}

const DataTrack* CTrackList::GetPtr(int track) const
/*!\brief Datensatz finden
 *
 * Mit dieser Funktion kann ein bestimmer Track ausgelesen werden
 *
 * \param[in] track TrackNummer (Die Position des Titels innerhalb der Seite des Tonträgers)
 * \returns Ist der gewünschte Track in der Trackliste enthalten, wird ein Pointer auf den
 * DataTrack Datensatz zurückgeliefert. Wurde er nicht
 * gefunden, wird NULL zurückgegeben.
 */
{
	if (track < 1 || track >65535) {
		return NULL;
	}
	std::map<int, DataTrack>::const_iterator it;
	it=Tracks.find(track);
	if (it == Tracks.end()) return NULL;
	return &(*it).second;
}

bool CTrackList::Exists(int track) const
{
	const DataTrack* t=GetPtr(track);
	if (t) return true;
	return false;
}


CTrackList::iterator CTrackList::begin()
{
	return Tracks.begin();
}

CTrackList::iterator CTrackList::end()
{
	return Tracks.end();
}

CTrackList::const_iterator CTrackList::begin() const
{
	return Tracks.begin();
}

CTrackList::const_iterator CTrackList::end() const
{
	return Tracks.end();
}

DataTrack* CTrackList::SaveToMemory(const DataTrack& t)
{
	std::pair<std::map<int, DataTrack>::iterator, bool> new_it;
	new_it=Tracks.insert(std::pair<int, DataTrack>(t.Track, t));
	return &new_it.first->second;
}

void CTrackList::Add(uint16_t Track, const DataTrack& t)
{
	Tracks.insert(std::pair<int, DataTrack>(Track, t));
}

void CTrackList::SaveToStorage(DataTrack& t)
{
	if (storage) {
		storage->Put(t);
	}
}

const DataTrack& CTrackList::Put(const DataTrack& entry)
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
 * \note Die Funktion ruft die gleichnamige Funktion CTrackStore::Put auf, womit die Daten
 * sowohl im Speicher als auch auf der Platte gespeichert werden.
 *
 */
{
	if (entry.Track < 1 || entry.Track >65535) {
		throw InvalidTrackNumberException("Track must be between 1 and 65535");
	}
	if (storage) {
		const DataTrack& memtrack=storage->Put(entry);
		return *SaveToMemory(memtrack);
	} else {
		return *SaveToMemory(entry);
	}
}

void CTrackList::Delete(int track)
/*!\brief Track löschen
 *
 * Mit dieser Funktion wird ein bestimmter Track in der Liste und auf dem Datenträger
 * gelöscht. Dadurch entsteht eine Lücke in der Liste. Der Titel, auf den dieser
 * Track gezeigt hat, wird nicht gelöscht!
 *
 * \param[in] track Nummer des Tracks
 */
{
	if (track < 1 || track>65535) return;
	if (storage) {
		const DataTrack* t=GetPtr(track);
		if (t) {
			storage->Delete(*t);
		}
	}
	Tracks.erase(track);
}



}
}
}
