/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.3 $
 * $Date: 2010/09/30 19:07:09 $
 * $Id: CTrackList.cpp,v 1.3 2010/09/30 19:07:09 pafe Exp $
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

/*!\class CTrackListItem
 * \brief Datenelement der Klasse CTrackList
 *
 * Dies ist ein Datenelement der Klasse CTrackList
 */


/*!\var CTrackListItem::id
 * \brief Track-Nummer des Datensatzes
 */

/*!\var CTrackListItem::track
 * \brief Pointer auf den eigentlichen Datensatz vom Typ DataTrack
 */


int CTrackListItem::CompareNode(CTreeItem *item)
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
	CTrackListItem *v=(CTrackListItem *)item;
	// Device vergleichen
	if (v->id > id) return 1;
	else if (v->id < id) return -1;
	return 0;
}

int CTrackListItem::CompareValue(void *value)
/*!\brief Elemente vergleichen
 *
 * \desc
 * Funktion zum Vergleichen zweier Baumelemente.
 * Bei dieser Funktion wird als Parameter ein void Pointer auf einen beliebigen
 * Wert erwartet.
 *
 * \param[in] value Pointer auf eine Vergleichsklasse vom Typ DataTrack
 *
 * \returns Die Funktion liefert einen der folgenden Werte zurück:
 * - 0: Der Wert in \p value ist identisch mit dem Wert dieses Elements
 * - +1: Der Wert in \p value ist größer als der Wert dieses Elements
 * - -1: Der Wert in \p value ist kleiner als der Wert dieses Elements
 */
{
	int *v=(int*)value;
	// Device vergleichen
	if (*v > id) return 1;
	else if (*v < id) return -1;
	return 0;
}

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

CTrackList::CTrackList(const CTrackList &other)
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

CTrackList::CTrackList(const CTrackList *other)
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

void CTrackList::copy(const CTrackList &other)
{
	Mutex.Lock();
	min=other.min;
	max=other.max;
	storage=other.storage;
	DeviceId=other.DeviceId;
	DeviceType=other.DeviceType;
	Page=other.Page;
	Tracks.Clear(true);
	ppl6::CTreeWalker walk;
	other.Tracks.Reset(walk);
	CTrackListItem *t;
	while ((t=(CTrackListItem *)other.Tracks.GetNext(walk))) {
		Tracks.Add(t);
	}

	Mutex.Unlock();

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
	Mutex.Lock();
	Tracks.Clear(true);
	min=0;
	max=0;
	Mutex.Unlock();
}

int CTrackList::Add(int track, DataTrack *entry)
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
	if (!entry) {
		ppl6::SetError(194,"int CTrackList::Add(int track, ==> DataTrack *entry <==)");
		return 0;
	}
	if (track<1 || track >65535) {
		ppl6::SetError(20017);
		return 0;
	}
	CTrackListItem *item=new CTrackListItem;
	if (!item) {
		ppl6::SetError(2);
		return 0;
	}
	Mutex.Lock();
	item->id=track;
	item->track=entry;
	if (!Tracks.Add(item)) {
		ppl6::PushError();
		Mutex.Unlock();
		delete item;
		ppl6::PopError();
		return 0;
	}
	if (min==0 || track<min) min=track;
	if (track>max) max=track;
	Mutex.Unlock();
	return 1;

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
	return Tracks.Num();
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

DataTrack *CTrackList::Get(int track)
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
	if (track<1 || track>65535) {
		ppl6::SetError(20017);
		return NULL;
	}
	Mutex.Lock();
	if (track<min || track>max) {
		Mutex.Unlock();
		ppl6::SetError(20018);
		return NULL;
	}
	CTrackListItem *t=(CTrackListItem*)Tracks.Find(&track);
	Mutex.Unlock();
	if (!t) {
		ppl6::SetError(20018);
		return NULL;
	}
	if (t->track->TitleId>0) return t->track;
	ppl6::SetError(20018);
	return NULL;
}

int CTrackList::GetCopy (int track, DataTrack *t)
/*!\brief Kopie eines Datensatzes erstellen
 *
 * Mit dieser Funktion kann eine Kopie eines vorhandenen Datensatzes erstellt werden.
 *
 * \param[in] track TrackNummer (Die Position des Titels innerhalb der Seite des Tonträgers)
 * \param[in,out] t Pointer auf eine Klasse vom Typ DataTrack
 *
 * \returns Ist der Datensatz in der Trackliste vorhanden und konnte kopiert werden, liefert die Funktion 1
 * zurück, sonst 0.
 */
{
	if (!t) {
		ppl6::SetError(194,"int CTrackList::GetCopy (int track,==> DataTrack *t <==)");
		return 0;
	}
	DataTrack *res=Get(track);
	if (!res) return 0;
	t->CopyFrom(res);
	return 1;
}

int CTrackList::Put(DataTrack *entry)
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
	if (!storage) {
		ppl6::SetError(20019);
		return 0;
	}
	if (!storage->Put(entry)) return 0;
	DataTrack *n=storage->Get(entry->Device, entry->DeviceId, entry->Page, entry->Track);
	int track=entry->Track;
	Mutex.Lock();
	CTrackListItem *t=(CTrackListItem*)Tracks.Find(&track);
	Mutex.Unlock();
	if (t) t->track=n;
	else return Add(track,n);
	return 1;
}

int CTrackList::Delete(int track)
/*!\brief Track löschen
 *
 * Mit dieser Funktion wird ein bestimmter Track in der Liste und auf dem Datenträger
 * gelöscht. Dadurch entsteht eine Lücke in der Liste. Falls sich der Titel
 * auf dem Tonträger befindet, von dem diese Trackliste generiert wurde, wird
 * auch der Titel aus der Datenbank gelöscht.
 *
 * \param[in] track Nummer des Tracks
 * \returns Bei Erfolg gibt die Funktion 1 zurück, sonst 0.
 */

{
	if (track<1 || track>65535) {
		ppl6::SetError(20017);
		return 0;
	}
	Mutex.Lock();
	ppluint32 tmp;
	CTrackListItem *t=(CTrackListItem*)Tracks.Find(&track);
	if (t) {
		tmp=t->track->TitleId;
		t->track->TitleId=0;
		if (!storage->Delete(t->track)) {
			t->track->TitleId=tmp;
			Mutex.Unlock();
			return 0;
		}
		// Titel laden
		if (wm_main) {
			DataTitle *ti=wm_main->GetTitle(tmp);
			if (ti) {
				if (ti->DeviceType==DeviceType && ti->DeviceId==DeviceId && ti->Page==Page) {
					// Titel muss gelöscht werden
					wm_main->Hashes.RemoveTitle(ti->TitleId);
					wm_main->TitleStore.Delete(ti);
				}
			}
		}


		Tracks.Delete(t);
		delete(t);
		int oldmax=max;
		max=0;
		min=oldmax;
		for (int i=0;i<=oldmax;i++) {
			CTrackListItem *t=(CTrackListItem*)Tracks.Find(&i);
			if (t) {
				if (i>max) max=i;
				if (i<min) min=i;
			}
		}
		if (DeviceType==7 && wm_main!=NULL) {
			wm_main->TrashMP3File(DeviceId,Page,track);
		}

	}
	Mutex.Unlock();
	return 1;
}

int CTrackList::DeleteShift(int track, CTitleStore *tistore)
/*!\brief Track löschen, nachfolgende nach vorne schieben
 *
 * Mit dieser Funktion wird ein bestimmter Track in der Liste gelöscht und die nachfolgenden
 * Tracks nach vorne gezogen. Die Änderung wird außerdem auch auf dem Datenträger vorgenommen.
 * Der Wert max wird automatisch korrigiert.
 *
 * \param[in] track Nummer des Tracks
 * \param[in] tistore Optionaler Pointer auf den TitelStore. Nur wenn er angegeben ist, werden
 * auch die Daten des Titels aktualisiert (empfohlen)
 * \returns Bei Erfolg gibt die Funktion 1 zurück, sonst 0.
 */
{
	// Zuerst löschen wir den Track, falls er noch vorhanden ist
	if (!Delete(track)) return 0;
	// Wir brauchen Track nicht weiter zu prüfen, da Delete dies schon getan hat
	Mutex.Lock();
	CTrackListItem *t;
	DataTitle *Ti;
	for (int i=track+1;i<=max;i++) {
		t=(CTrackListItem*)Tracks.Find(&i);
		if (t) {
			// Erst nehmen wir den Track aus dem Tree
			Tracks.Delete(t);
			// Track-Nummer verkleinern
			t->track->Track=i-1;
			t->id=i-1;
			// Track speichern
			storage->Put(t->track);
			// Wieder in den Tree einfügen
			Tracks.Add(t);
			// Titel korrigieren
			if (tistore) {
				Ti=tistore->Get(t->track->TitleId);
				if (Ti!=NULL && Ti->DeviceId==t->track->DeviceId
						&& Ti->DeviceType==t->track->DeviceId
						&& Ti->Page==t->track->Page) {
					Ti->Track=t->track->Track;
					tistore->Put(Ti);
				}
			}
			if (DeviceType==7 && wm_main!=NULL) {
				// File umbenennen
				wm_main->RenameMP3File(DeviceId,Page,i,i-1);
			}
		}
	}
	int oldmax=max;
	max=0;
	min=oldmax;
	for (int i=0;i<=oldmax;i++) {
		CTrackListItem *t=(CTrackListItem*)Tracks.Find(&i);
		if (t) {
			if (i>max) max=i;
			if (i<min) min=i;
		}
	}
	Mutex.Unlock();
	return 1;
}

int CTrackList::InsertShift(int track, CTitleStore *tistore)
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
	if (track<1 || track>65535) {
		ppl6::SetError(20017);
		return 0;
	}
	Mutex.Lock();
	if (track<min || track>max) {
		ppl6::SetError(20018);
		Mutex.Unlock();
		return 0;
	}
	CTrackListItem *t;
	DataTitle *Ti;
	for (int i=max;i>=track;i--) {				// Von hinten nach vorne
		t=(CTrackListItem*)Tracks.Find(&i);
		if (t) {
			// Erst nehmen wir den Track aus dem Tree
			Tracks.Delete(t);
			// Track-Nummer vergrößern
			t->track->Track=i+1;
			t->id=i+1;
			// Track speichern
			storage->Put(t->track);
			// Wieder in den Tree einfügen
			Tracks.Add(t);
			// Titel korrigieren
			if (tistore) {
				Ti=tistore->Get(t->track->TitleId);
				if (Ti!=NULL && Ti->DeviceId==t->track->DeviceId
						&& Ti->DeviceType==t->track->DeviceId
						&& Ti->Page==t->track->Page) {
					Ti->Track=t->track->Track;
					tistore->Put(Ti);
				}
			}
			if (DeviceType==7 && wm_main!=NULL) {
				// File umbenennen
				wm_main->RenameMP3File(DeviceId,Page,i,i+1);
			}
		}
	}
	max++;
	Mutex.Unlock();
	return 1;
}


void CTrackList::Reset()
{
	Tracks.Reset();
}

DataTrack *CTrackList::GetFirst()
{
	CTrackListItem *t=(CTrackListItem*)Tracks.GetFirst();
	if (t) return t->track;
	return NULL;
}

DataTrack *CTrackList::GetNext()
{
	CTrackListItem *t=(CTrackListItem*)Tracks.GetNext();
	if (t) return t->track;
	return NULL;
}

