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
	min=0;
	max=0;
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
	min=0;
	max=0;
	this->storage=storage;
	DeviceId=0;
	DeviceType=0;
	Page=0;
}


CTrackList::CTrackList(const CTrackList& other)
/*!\brief Copy Konstruktor der Klasse
 */
{
	min=0;
	max=0;
	storage=NULL;
	DeviceId=0;
	DeviceType=0;
	Page=0;
	copy(other);
}

CTrackList::CTrackList(const CTrackList* other)
/*!\brief Copy Konstruktor der Klasse
 */
{
	min=0;
	max=0;
	storage=NULL;
	DeviceId=0;
	DeviceType=0;
	Page=0;
	if (other) copy(*other);
}

void CTrackList::copy(const CTrackList& other)
{
	Clear();
	min=other.min;
	max=other.max;
	storage=other.storage;
	DeviceId=other.DeviceId;
	DeviceType=other.DeviceType;
	Page=other.Page;
	std::map<int, DataTrack>::const_iterator it;
	for (it=other.Tracks.begin();it != other.Tracks.end();++it) {
		Tracks.insert(std::pair<int, DataTrack>((*it).first, (*it).second));
	}
}

CTrackList::~CTrackList()
/*!\brief Destruktor der Klasse
 *
 * Der Destruktor löscht den internen binären Suchbaum und gibt dessen Speicher wieder frei.
 */
{
	Clear();
}

void CTrackList::Clear()
/*!\brief Daten löschen
 *
 * Diese Funktion löscht den internen binären Suchbaum und gibt dessen Speicher wieder frei.
 * Alle Variablen werden auf 0 gesetzt.
 */
{
	Tracks.clear();
	min=0;
	max=0;
}

void CTrackList::Add(int track, const DataTrack& entry)
/*!\brief Track hinzufügen
 *
 * Diese Funktion wird intern von CTrackStore::GetTracklist aufgerufen, um initial Tracks in die
 * Liste einzutragen.
 *
 * \param[in] track Nummer des Tracks
 * \param[in] entry Pointer auf die Daten des Tracks
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 */
{
	if (track < 1 || track >65535) {
		throw InvalidTrackNumberException("Track must be between 1 and 65535");
	}
	Tracks.insert(std::pair<int, DataTrack>(track, entry));
	if (min == 0 || track < min) min=track;
	if (track > max) max=track;
}

int CTrackList::GetMin()
/*!\brief Kleinste Tracknummer
 *
 * Diese Funktion liefert die kleinste Tracknummer der Liste zurück
 *
 * \returns Track-Nummer
 */
{
	return min;
}

int CTrackList::Num()
{
	return (int)Tracks.size();
}

int CTrackList::GetMax()
/*!\brief Größte Tracknummer
 *
 * Diese Funktion liefert die größte Tracknummer der Liste zurück
 *
 * \returns Track-Nummer
 */
{
	return max;
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

bool CTrackList::Exists(int track)
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
	if (min == 0 || t.Track < min) min=t.Track;
	if (t.Track > max) max=t.Track;
	return &new_it.first->second;
}

void CTrackList::SaveToStorage(DataTrack& t)
{
	if (storage) {
		storage->Put(t);
	}
}

void CTrackList::Put(const DataTrack& entry)
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
		storage->Put(entry);
		const DataTrack& memtrack=storage->Get(entry.Device, entry.DeviceId, entry.Page, entry.Track);
		SaveToMemory(memtrack);
	} else {
		SaveToMemory(entry);
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

#ifdef OLDCODE
void CTrackList::DeleteShift(int track, CTitleStore* tistore)
/*!\brief Track löschen, nachfolgende nach vorne schieben
 *
 * Mit dieser Funktion wird ein bestimmter Track in der Liste gelöscht und die nachfolgenden
 * Tracks nach vorne gezogen. Die Änderung wird außerdem auch auf dem Datenträger vorgenommen.
 * Der Wert max wird automatisch korrigiert.
 *
 * \param[in] track Nummer des Tracks
 * \param[in] tistore Optionaler Pointer auf den TitelStore. Nur wenn er angegeben ist, werden
 * auch die Daten des Titels aktualisiert (empfohlen)
 */
{
	// Zuerst löschen wir den Track, falls er noch vorhanden ist
	if (!Delete(track)) return 0;
	// Wir brauchen Track nicht weiter zu prüfen, da Delete dies schon getan hat
	Mutex.Lock();
	CTrackListItem* t;
	DataTitle* Ti;
	for (int i=track + 1;i <= max;i++) {
		t=(CTrackListItem*)Tracks.Find(&i);
		if (t) {
			// Erst nehmen wir den Track aus dem Tree
			Tracks.Delete(t);
			// Track-Nummer verkleinern
			t->track->Track=i - 1;
			t->id=i - 1;
			// Track speichern
			storage->Put(t->track);
			// Wieder in den Tree einfügen
			Tracks.Add(t);
			// Titel korrigieren
			if (tistore) {
				Ti=tistore->Get(t->track->TitleId);
				if (Ti != NULL && Ti->DeviceId == t->track->DeviceId
					&& Ti->DeviceType == t->track->Device
					&& Ti->Page == t->track->Page) {
					Ti->Track=t->track->Track;
					tistore->Put(Ti);
				}
			}
			if (wm_main != NULL) {
				// File umbenennen
				wm_main->RenameAudioFile(DeviceType, DeviceId, Page, i, i - 1);
			}
		}
	}
	int oldmax=max;
	max=0;
	min=oldmax;
	for (int i=0;i <= oldmax;i++) {
		CTrackListItem* t=(CTrackListItem*)Tracks.Find(&i);
		if (t) {
			if (i > max) max=i;
			if (i < min) min=i;
		}
	}
	Mutex.Unlock();
	return 1;
}

int CTrackList::InsertShift(int track, CTitleStore* tistore)
/*!\brief Tracks nach hinten verschieben
 *
 * Mit dieser Funktion wird an der angegebenen Track-Positionen eine Lück eingefügt und alle
 * nachfolgenden Tracks nach hinten verschoben. Die Änderung wird auch auf dem Datenträger vorgenommen.
 * Der Wert max wird automatisch korrigiert.
 *
 * \param[in] track Nummer des Tracks
 * \param[in] tistore Optionaler Pointer auf den TitelStore. Nur wenn er angegeben ist, werden
 * auch die Daten des Titels aktualisiert (empfohlen)
 * \returns Bei Erfolg gibt die Funktion 1 zurück, sonst 0.
 */
{
	if (track < 1 || track>65535) {
		ppl6::SetError(20017);
		return 0;
	}
	Mutex.Lock();
	if (track<min || track>max) {
		ppl6::SetError(20018);
		Mutex.Unlock();
		return 0;
	}
	CTrackListItem* t;
	DataTitle* Ti;
	for (int i=max;i >= track;i--) {				// Von hinten nach vorne
		t=(CTrackListItem*)Tracks.Find(&i);
		if (t) {
			// Erst nehmen wir den Track aus dem Tree
			Tracks.Delete(t);
			// Track-Nummer vergrößern
			t->track->Track=i + 1;
			t->id=i + 1;
			// Track speichern
			storage->Put(t->track);
			// Wieder in den Tree einfügen
			Tracks.Add(t);
			// Titel korrigieren
			if (tistore) {
				Ti=tistore->Get(t->track->TitleId);
				if (Ti != NULL && Ti->DeviceId == t->track->DeviceId
					&& Ti->DeviceType == t->track->Device
					&& Ti->Page == t->track->Page) {
					Ti->Track=t->track->Track;
					tistore->Put(Ti);
				}
			}
			if (wm_main != NULL) {
				// File umbenennen
				wm_main->RenameAudioFile(DeviceType, DeviceId, Page, i, i + 1);
			}
		}
	}
	max++;
	Mutex.Unlock();
	return 1;
}
#endif



}
}
}
