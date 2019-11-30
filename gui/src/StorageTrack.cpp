/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: StorageTrack.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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

DataTrack::~DataTrack()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Da kein Speicher innerhalb dieser Klasse dynamisch allokiert wird,
 * hat er gegenwärtig keine Funktion.
 */
{
	CStorageItem::Clear();
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

int DataTrack::CompareNode(CTreeItem *item)
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
	DataTrack *v=(DataTrack *)item;
	// Device vergleichen
	if (v->Device > Device) return 1;
	else if (v->Device < Device) return -1;
	// DeviceId vergleichen
	if (v->DeviceId > DeviceId) return 1;
	else if (v->DeviceId < DeviceId) return -1;
	// Seite vergleichen
	if (v->Page > Page) return 1;
	else if (v->Page < Page) return -1;
	// Track vergleichen
	if (v->Track > Track) return 1;
	else if (v->Track < Track) return -1;

	return 0;
}

int DataTrack::CompareValue(void *value)
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
	DataTrack *v=(DataTrack*)value;
	// Device vergleichen
	if (v->Device > Device) return 1;
	else if (v->Device < Device) return -1;
	// DeviceId vergleichen
	if (v->DeviceId > DeviceId) return 1;
	else if (v->DeviceId < DeviceId) return -1;
	// Seite vergleichen
	if (v->Page > Page) return 1;
	else if (v->Page < Page) return -1;
	// Track vergleichen
	if (v->Track > Track) return 1;
	else if (v->Track < Track) return -1;
	return 0;
}

int DataTrack::CopyFrom(DataTrack *t)
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
	if (!t) {
		ppl6::SetError(194,"int DataTrack::CopyFrom(==> DataTrack *t <==)");
		return 0;
	}
	Clear();
	DeviceId=t->DeviceId;
	TitleId=t->TitleId;
	Track=t->Track;
	Device=t->Device;
	Page=t->Page;
	CopyStorageFrom(t);
	return 1;
}


int DataTrack::SetValue(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track, ppluint32 TitleId)
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
	return 1;
}

ppl6::CBinary *DataTrack::Export()
/*!\brief Binäre Exportfunktion
 *
 * Mit dieser Funktion werden die Daten der Klasse in binärer Form exportiert. Das Format ist
 * folgendermassen:
 *
 * \copydoc WM3File_Chunk_TRAK
 *
 * Da keine Daten mit variabler Länge enthalten sind, ist der Datensatz immer 12 Byte groß.
 *
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf eine neue ppl6::CBinary Klasse zurück.
 * Diese muss nach Gebrauch vom aufrufenden Programm selbst gelöscht werden. Im Fehlerfall wird NULL
 * zurückgegeben.
 */
{
	// 12 Byte allokieren
	char *a=(char*)malloc(12);
	if (!a) {
		ppl6::SetError(2);
		return NULL;
	}
	ppl6::Poke8(a,Device);
	ppl6::Poke32(a+1,DeviceId);
	ppl6::Poke8(a+5,Page);
	ppl6::Poke16(a+6,Track);
	ppl6::Poke32(a+8,TitleId);
	ppl6::CBinary *bin=new ppl6::CBinary;
	if (!bin) {
		ppl6::SetError(2);
		return NULL;
	}
	bin->Set(a,12);
	// CBinary ist verantwortlich den Speicher wieder freizugeben
	bin->ManageMemory();
	return bin;
}


int DataTrack::Import(ppl6::CBinary *bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion werden binäre gespeicherte Daten in die Klasse importiert. Eine Beschreibung des
 * Binären Formats ist in der Funktion DataTrack::Export zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (!bin) {
		ppl6::SetError(194,"int DataTrack::Import(==> ppl6::CBinary *bin <==)");
		return 0;
	}
	if (version<1 || version>1) {
		ppl6::SetError(20023,"%i",version);
		return 0;
	}
	int size=bin->Size();
	const char *a=(char*)bin->GetPtr();
	// Die Größe muss genau 12 4 Byte betragen
	if (size!=12 || a==NULL) {
		ppl6::SetError(20017);
		return 0;
	}
	Clear();
	Device=ppl6::Peek8(a);
	DeviceId=ppl6::Peek32(a+1);
	Page=ppl6::Peek8(a+5);
	Track=ppl6::Peek16(a+6);
	TitleId=ppl6::Peek32(a+8);
	return 1;
}


/*!\class DataTrackHint
 * \brief Klasse zum Speichern von Minimal und Maximal Tracknummer eines Tonträgers
 *
 * Diese Klasse wird intern von CTrackStore verwendet, um sich zu merken, was jeweils
 * die kleinste und höchste Tracknummer pro Seite und Tonträger ist. Sie wird zur Erstellung
 * einer Trackliste mit CTrackStore::GetTracklist benötigt, um die Suche im binären Baum
 * der Tracks einzugrenzen.
 *
 * Pro Tonträger und Seite gibt es genau einen Datensatz. Als Schlüssel wird ein
 * zusammengesetzter Schlüssel aus Device, DeviceId und Page verwendet,
 * Der Datentyp ist daher von ppl6::CTreeItem abgeleitet.
 *
 * Der Baum wird beim laden der Datenbank autumatisch aufgebaut und bei jedem Speichervorgang
 * eines Tracks aktualisiert.
 *
 */


/*!\var DataTrackHint::Device
 * \brief Art des Tonträgers
 *
 * Diese Variable gibt an, um welche Art Tonträger es sich handelt.
 *
 * \copydoc WM3File_Devices
 */

/*!\var DataTrackHint::DeviceId
 * \brief ID des Tonträgers
 *
 * In WinMusik werden alle Tonträger zur Verwaltung durchnummeriert. Diese Variable gibt an,
 * um welchen Tonträger es sich innerhalb einer Art handelt.
 */

/*!\var DataTrackHint::Page
 * \brief Seite des Tonträgers
 *
 * Manche Tonträger haben mehrere bespielbare Seiten. Eine Musikkassette besteht z.B. in der Regel
 * aus 2 Seiten, eine CD hingegen aus nur einer.
 *
 * Diese Variable gibt an, auf welcher Seite des Tonträgers sich der Titel befindet.
 */

/*!\var DataTrackHint::min
 * \brief Kleinste Trackposition
 *
 * Diese Variable enthält die kleinste Tracknummer auf dieser Tonträgerseite.
 */

/*!\var DataTrackHint::max
 * \brief Größte Trackposition
 *
 * Diese Variable enthält die größte Tracknummer auf dieser Tonträgerseite.
 */

DataTrackHint::DataTrackHint()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse. Er setzt alle Variablen auf 0.
 */
{
	DeviceId=0;
	Device=0;
	Page=0;
	min=0;
	max=0;
}

DataTrackHint::~DataTrackHint()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Da kein Speicher innerhalb dieser Klasse dynamisch allokiert wird,
 * hat er gegenwärtig keine Funktion.
 */
{

}

int DataTrackHint::CompareNode(CTreeItem *item)
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
	DataTrackHint *v=(DataTrackHint *)item;
	// Device vergleichen
	if (v->Device > Device) return 1;
	else if (v->Device < Device) return -1;
	// DeviceId vergleichen
	if (v->DeviceId > DeviceId) return 1;
	else if (v->DeviceId < DeviceId) return -1;
	// Seite vergleichen
	if (v->Page > Page) return 1;
	else if (v->Page < Page) return -1;
	return 0;
}

int DataTrackHint::CompareValue(void *value)
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
	DataTrackHint *v=(DataTrackHint*)value;
	// Device vergleichen
	if (v->Device > Device) return 1;
	else if (v->Device < Device) return -1;
	// DeviceId vergleichen
	if (v->DeviceId > DeviceId) return 1;
	else if (v->DeviceId < DeviceId) return -1;
	// Seite vergleichen
	if (v->Page > Page) return 1;
	else if (v->Page < Page) return -1;
	return 0;
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

/*!\var CTrackStore::Tree
 * \brief Interner Binary Tree zur Verwaltung der Datensätze
 *
 * Interner Binary Tree zur Verwaltung der Datensätze
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

void CTrackStore::Clear()
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
	TrackHints.Clear(true);
	Mutex.Unlock();
}

const char *CTrackStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return "TRAK";
}

int CTrackStore::Save(DataTrack *t)
/*!\brief Datensatz auf Festplatte schreiben
 *
 * Diese Funktion speichert den übergebenen Datensatz auf die Festplatte. Dazu wird der Inhalt
 * zunächst als Binary exportiert (DataTrack::Export) und damit CStorage::Save aufgerufen.
 * Die Funktion wird intern von CTrackStore::Put verwendet.
 *
 * \param[in] t Pointer auf einen DataTrack Datensatz
 * \returns Konnte der Datensatz erfolgreich gespeichert werden, liefert die Funktion 1 zurück, sonst
 * 0.
 */
{
	if (!Storage) {
		return 0;
	}
	if (!t) {
		ppl6::SetError(194,"int CTrackStore::Save(==> DataTrack *t <==)");
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

int CTrackStore::Delete(DataTrack *entry)
{
	if (!entry) {
		ppl6::SetError(194,"int CTrackStore::Put(==> DataTrack *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014,"CTrackStore");
		return 0;
	}
	Mutex.Lock();
	// Gibt's den Track überhaupt?
	DataTrack *t;
	t=(DataTrack *)Tree.Find((void *)entry);
	if (t) {	// Jepp
		Storage->Delete(this,t);
		Tree.Delete(t);
		delete t;
	}
	return 1;
}

int CTrackStore::Put(DataTrack *entry)
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
	if (!entry) {
		ppl6::SetError(194,"int CTrackStore::Put(==> DataTrack *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014,"CTrackStore");
		return 0;
	}
	Mutex.Lock();
	// Gibt's den Track schon?
	DataTrack *t;
	t=(DataTrack *)Tree.Find((void *)entry);
	if (t) {	// Jepp, gibt's schon. Wir machen ein Update
		t->DeviceId=entry->DeviceId;
		t->TitleId=entry->TitleId;
		t->Track=entry->Track;
		t->Device=entry->Device;
		t->Page=entry->Page;
		if (!Save(t)) {
			Mutex.Unlock();
			return 0;
		}
		// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
		entry->CopyStorageFrom(t);
		UpdateHints(t);
		Mutex.Unlock();
		return 1;
	}
	// Nein, neuer Track
	t=new DataTrack;
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
	UpdateHints(t);
	Mutex.Unlock();
	return 1;
}

DataTrack *CTrackStore::Get(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track)
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
	DataTrack search;
	search.SetValue(Device,DeviceId,Page,Track,0);
	DataTrack *t;
	Mutex.Lock();
	t=(DataTrack *)Tree.Find(&search);
	Mutex.Unlock();
	return t;
}

int CTrackStore::GetCopy(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track, DataTrack *t)
/*!\brief Kopie eines Datensatzes erstellen
 *
 * Mit dieser Funktion kann eine Kopie eines vorhandenen Datensatzes erstellt werden.
 *
 * \param[in] Device ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in] Page Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * \param[in] Track TrackNummer (Die Position des Titels innerhalb der Seite des Tonträgers)
 * \param[in,out] t Pointer auf eine Klasse vom Typ DataTrack
 *
 * \returns Ist der Datensatz vorhanden und konnte kopiert werden, liefert die Funktion 1
 * zurück, sonst 0.
 */
{
	DataTrack search;
	search.SetValue(Device,DeviceId,Page,Track,0);
	DataTrack *res;
	Mutex.Lock();
	res=(DataTrack *)Tree.Find(&search);
	if (res) t->CopyFrom(res);
	Mutex.Unlock();
	if (res) return 1;
	return 0;
}

DataTrackHint *CTrackStore::GetHints(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page)
/*!\brief Hint-Eintrag für den gewünschten Datenträger suchen
 *
 * Diese interne Funktion sucht den gewünschten Hint-Eintrag heraus, sofern vorhanden.
 *
 * \param[in] Device ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in] Page Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * \returns Ist bereits ein Hint-Eintrag vorhanden, wird der Pointer darauf zurückgeliefert,
 * ansonsten NULL.
 *
 * \note Die Funktion wird intern von CTrackStore::UpdateHints und CTrackStore::GetTracklist
 * aufgerufen und verwendet selbst keinen Mutex. Die Aufrufenden Funktionen müssen daher
 * die Thread-Sicherheit garantieren.
 */
{
	DataTrackHint search;
	search.DeviceId=DeviceId;
	search.Device=Device;
	search.Page=Page;
	return (DataTrackHint*)TrackHints.Find(&search);
}

int CTrackStore::UpdateHints(DataTrack *t)
/*!\brief TrackHints aktualisieren
 *
 * Mit dieser internen Funktion wird die Hint-Tabelle für den minimal- und maximal-Wert
 * der Trackposition für einen bestimmten Tonträger und bestimmte Seite aktualisiert.
 *
 * \param[in] t Pointer auf den gerade aktualisierten Datensatz
 * \returns Bei Erfolg wird 1 zurückgeliefert, sonst 0. Ein Fehler kann nur auftreten, wenn der
 * Speicher aufgebraucht ist.
 */
{
	DataTrackHint *s;
	s=GetHints(t->Device, t->DeviceId, t->Page);
	if (!s) {
		s=new DataTrackHint;
		if (!s) {
			ppl6::SetError(2);
			return 0;
		}
		s->DeviceId=t->DeviceId;
		s->Device=t->Device;
		s->Page=t->Page;
		if (!TrackHints.Add(s)) {
			delete s;
			return 0;
		}
	}
	if (t->Track<s->min || s->min==0) s->min=t->Track;
	if (t->Track>s->max) s->max=t->Track;
	return 1;
}

CTrackList *CTrackStore::GetTracklist(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page)
/*!\brief Trackliste für einen bestimmten Datenträger erstellen
 *
 * Diese Funktion erstellt eine Trackliste für den gewünschten Datenträger. Dabei werden
 * die Original Datensätze referenziert, es werden keine Kopien gemacht.
 * \param[in] Device ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in] Page Seite (z.B. bei einer Musikkassette, die 2 Seiten hat)
 * \returns Bei Erfolg liefert die Funktion einen Pointer auf eine neue CTrackList Klasse zurück.
 * Die Aufrufende Anwendung ist dafür verantwortlich die Klasse nach Gebrauch zu
 * löschen! Im Fehlerfall (out of memory) wird NULL zurückgegeben.
 *
 * \todo Prüfen, ob durch die blosse Referenzierung der Tracks Probleme in der Threadsicherheit entstehen.
 */
{
	CTrackList *list=new CTrackList;
	list->storage=this;
	list->DeviceType=Device;
	list->Page=Page;
	list->DeviceId=DeviceId;
	DataTrack *res, search;
	DataTrackHint *hint;
	search.SetValue(Device,DeviceId,Page,0,0);
	Mutex.Lock();
	// Hints laden
	hint=GetHints(Device, DeviceId, Page);
	if (hint) {
		// Wir haben Hints gefunden und hangeln uns nun von min zu max
		for (int i=hint->min;i<=hint->max;i++) {
			search.Track=i;
			res=(DataTrack *)Tree.Find(&search);
			if (res) {
				// Track gefunden, wir können ihn in die TrackListe hängen
				list->Add(i,res);
			}
		}
	}
	Mutex.Unlock();
	return list;
}


int CTrackStore::LoadChunk(CWMFileChunk *chunk)
{
	DataTrack data;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk->GetChunkData(),chunk->GetChunkDataSize())) return 0;
	if (!data.Import(&bin,chunk->GetFormatVersion())) return 0;
	data.CopyStorageFrom(chunk);
	return Put(&data);
}



