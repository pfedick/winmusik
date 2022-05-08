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


#include "winmusik3.h"

/*!\typedef DATADEVICE
 * \brief Behelfsdatentyp zum Suchen nach Tonträgern
 */

/*!\class DataDevice
 * \ingroup WM3DataTypes
 * \brief Klasse zum Speichern der Stammdaten von Tonträgern
 *
 * Diese Klasse wird verwendet, um die Stammdaten eines einzelnen Tonträgers zu speichern.
 *
 * Die Tonträger werden innerhalb der Storage Klasse CDeviceStore in einem binären Baum verwaltet.
 * Als Schlüssel wird ein zusammengesetzter Schlüssel aus DeviceType und DeviceId
 * verwendet,
 * Der Datentyp ist daher neben CStorageItem auch von ppl6::CTreeItem abgeleitet.
 *
 */


/*!\var DataDevice::Title
 * \brief Haupttitel des Tonträgers
 *
 * Diese Variable enthält den Titel des Tonträgers oder NULL, wenn kein Titel gesetzt wurde.
 * Er muss mit der Funktion DataDevice::SetTitle gesetzt werden.
 */

/*!\var DataDevice::SubTitle
 * \brief Untertitel des Tonträgers
 *
 * Diese Variable enthält den Untertitel des Tonträgers oder NULL, wenn kein Untertitel
 * gesetzt wurde.
 * Er muss mit der Funktion DataDevice::SetSubTitle gesetzt werden.
 */

/*!\var DataDevice::DeviceType
 * \brief Art des Tonträgers
 *
 * Diese Variable gibt an, um welche Art Tonträger es sich handelt.
 *
 * \copydoc WM3File_Devices
 */

/*!\var DataDevice::DeviceId
 * \brief ID des Tonträgers
 *
 * In WinMusik werden alle Tonträger zur Verwaltung durchnummeriert. Diese Variable gibt an,
 * um welchen Tonträger es sich innerhalb einer Art handelt.
 */

/*!\var DataDevice::Length
 * \brief Maximallänge des Tonträgers
 *
 * Diese Variable gibt die Gesamt-Länge bzw. maximale Aufnahmelänge des Tonträgers in Sekunden an.
 * Bei einigen Tonträgern ist diese Länge von vorne herein bekannt (z.B. eine 90 Minuten Musikkassette
 * oder eine brennbare Audio-CD mit 74 Minuten Länge), bei anderen hängt es von der
 * Komprimierung der einzelnen Tracks ab (z.B. MP3-Files). Der Wert kann daher auch 0 sein.
 */

/*!\var DataDevice::Recorded
 * \brief Bespielte Länge des Tonträgers
 *
 * Gibt die bespielte Länge des Tonträgers in Sekunden an und entspricht der Summe der Längen
 * aller Tracks des Tonträgers.
 */

/*!\var DataDevice::PurchaseDate
 * \brief Kaufdatum des Tonträgers
 *
 * Diese Variable enthält das Kaufdatum des Tonträgers als Integer im Format yyyymmdd.
 * Der 22. Mai 2009 wäre z.B. 20090522. Der Wert 0 bedeutet, dass das Kaufdatum unbekannt ist.
 */

/*!\var DataDevice::DateCreated
 * \brief Datum, wann der Tonträger angelegt wurde
 *
 * Diese Variable enthält das Datum, wann der Tonträger in der Datenbank angelegt wurde.
 * Es ist als Integer um Format yyyymmdd gespeichert
 */

/*!\var DataDevice::FirstDate
 * \brief Aufnahmedatum des ältesten Tracks auf dem Tonträger
 *
 * Enthält das Aufnahmedatum des ältesten Tracks auf dem Tonträger als Integer im Format
 * yyyymmdd.
 */

/*!\var DataDevice::LastDate
 * \brief Aufnahmedatum des neusten Tracks auf dem Tonträger
 *
 * Enthält das Aufnahmedatum des neusten Tracks auf dem Tonträger als Integer im Format
 * yyyymmdd.
 */

/*!\var DataDevice::NumTracks
 * \brief Anzahl Tracks auf dem Tonträger
 *
 * Diese Variable enthält die Anzahl Tracks auf dem Tonträger insgesammt (alle Seiten).
 */

/*!\var DataDevice::PurchaseId
 * \brief ID der Kaufquelle
 *
 * Diese Variable stellt eine Verknüpfung zur Tabelle der Kaufquellen her (CPurchaseSourceStore).
 */

/*!\var DataDevice::PurchasePrice
 * \brief Kaufpreis des Tonträgers
 *
 * Diese Variable enthält den Kaufpreis des Tonträgers oder 0, wenn keiner angegeben wurde.
 */

/*!\var DataDevice::Pages
 * \brief Anzahl Seiten des Tonträgers
 *
 * Manche Tonträger haben mehrere bespielbare Seiten. Eine Musikkassette besteht z.B. in der Regel
 * aus 2 Seiten, eine CD hingegen aus nur einer. Es wäre aber auch denkbar, dass man eine einzelne
 * CD-Hülle mit mehreren CD's als eine CD mit mehreren Seiten ansieht.
 *
 * Diese Variable gibt an, aus wievielen Seiten der Tonträger besteht. Die Nummerierung beginnt
 * bei 1 und bedeutet eine Seite. Ein Tonträger kann aus maximal 255 Seiten bestehen.
 */


DataDevice::DataDevice()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse. Er setzt alle Variablen auf 0.
 */
{
	DeviceId=0;
	Length=0;
	Recorded=0;
	PurchaseDate=0;
	DateCreated=0;
	FirstDate=0;
	LastDate=0;
	NumTracks=0;
	PurchaseId=0;
	DeviceType=0;
	Pages=0;
	PurchasePrice=0.0f;
	formatversion=1;
}

DataDevice::~DataDevice()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Gibt sämtlichen durch diese Klasse allokierten Speicher
 * wieder frei.
 */
{
	Clear();
}

void DataDevice::Clear()
/*!\brief Variablen auf 0 setzen.
 *
 * Mit dieser Funktion werden alle Variablen auf 0 gesetzt und der durch die Klasse
 * allokierte Speicher wieder freigegeben. Außerdem wird die Clear-Funktion der
 * Storage-Klasse CStorageItem::Clear aufgerufen.
 */
{
	Title.clear();
	SubTitle.clear();
	DeviceId=0;
	Length=0;
	Recorded=0;
	LabelId=0;
	PurchaseDate=0;
	DateCreated=0;
	FirstDate=0;
	LastDate=0;
	NumTracks=0;
	PurchasePrice=0.0f;
	PurchaseId=0;
	DeviceType=0;
	Pages=0;
	CStorageItem::Clear();
	formatversion=1;
}

void DataDevice::CopyFrom(const DataDevice& other)
{
	Clear();
	CopyDataFrom(other);
	CopyStorageFrom(other);
}

void DataDevice::CopyDataFrom(const DataDevice& other)
/*!\brief Daten kopieren
 *
 * Mit dieser Funktion werden die Daten einers anderen DataDevice Datensatzes in diesen hineinkopiert.
 * Bereits vorhandene Daten werden dabei überschrieben.
 *
 * \param[in] t Pointer auf einen anderen Datensatz vom Typ DataDevice
 *
 * \returns Konnten die Daten erfolgreich kopiert werden, liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \attention
 * Auch die Storage-Datenm werden von \p t übernommen.
 */
{

	Title=other.Title;
	SubTitle=other.SubTitle;
	DeviceId=other.DeviceId;
	Length=other.Length;
	Recorded=other.Recorded;
	LabelId=other.LabelId;
	PurchaseDate=other.PurchaseDate;
	DateCreated=other.DateCreated;
	FirstDate=other.FirstDate;
	LastDate=other.LastDate;
	NumTracks=other.NumTracks;
	PurchasePrice=other.PurchasePrice;
	PurchaseId=other.PurchaseId;
	DeviceType=other.DeviceType;
	Pages=other.Pages;
}


void DataDevice::SetTitle(const ppl7::String& title)
/*!\brief Haupttitel setzen
 *
 * Mit dieser Funktion wird der Titel des Tonträgers gesetzt.
 *
 * \param[in] title Pointer auf einen String mit dem Titel.
 * \returns Liefert 1 zurück, wenn der Titel erfolgreich gesetzt werden konnte.
 */
{
	this->Title=title;
}

void DataDevice::SetSubTitle(const ppl7::String& subtitle)
/*!\brief Untertitel setzen
 *
 * Mit dieser Funktion wird der Untertitel des Tonträgers gesetzt.
 *
 * \param[in] subtitle Pointer auf einen String mit dem Untertitel.
 * \returns Liefert 1 zurück, wenn der Untertitel erfolgreich gesetzt werden konnte.
 */
{
	this->SubTitle=subtitle;
}

const ppl7::String& DataDevice::GetTitle() const
{
	return Title;
}

const ppl7::String& DataDevice::GetSubTitle() const
{
	return SubTitle;
}


void DataDevice::Export(ppl7::ByteArray& bin) const
/*!\brief Binäre Exportfunktion
 *
 * Mit dieser Funktion werden die Daten der Klasse in binärer Form exportiert. Das Format ist
 * folgendermassen:
 *
 * \copydoc WM3File_Chunk_DEVI
 *
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf eine neue ppl6::CBinary Klasse zurück.
 * Diese muss nach Gebrauch vom aufrufenden Programm selbst gelöscht werden. Im Fehlerfall wird NULL
 * zurückgegeben.
 */
{
	// Benötigte Bytes berechnen
	int size=48;
	int lenTitle=Title.size();
	int lenSubTitle=SubTitle.size();
	size=size + lenTitle + lenSubTitle;
	// Speicher allokieren
	char* a=(char*)bin.malloc(size);
	ppl7::Poke8(a, DeviceType);
	ppl7::Poke32(a + 1, DeviceId);
	ppl7::Poke8(a + 5, Pages);
	ppl7::Poke32(a + 6, Length);
	ppl7::Poke32(a + 10, Recorded);
	ppl7::Poke16(a + 14, PurchaseId);
	ppl7::Poke32(a + 16, LabelId);
	ppl7::Poke32(a + 20, PurchaseDate);
	ppl6::PokeFloat(a + 24, PurchasePrice);
	ppl7::Poke32(a + 28, DateCreated);
	ppl7::Poke32(a + 32, FirstDate);
	ppl7::Poke32(a + 36, LastDate);
	ppl7::Poke32(a + 40, NumTracks);
	ppl7::Poke16(a + 44, lenTitle);
	if (lenTitle) strncpy(a + 46, (const char*)Title, lenTitle);
	ppl7::Poke16(a + 46 + lenTitle, lenSubTitle);
	if (lenSubTitle) strncpy(a + 48 + lenTitle, (const char*)SubTitle, lenSubTitle);
}

void DataDevice::Import(const ppl7::ByteArrayPtr& bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion werden binäre gespeicherte Daten in die Klasse importiert. Eine Beschreibung des
 * Binären Formats ist in der Funktion DataDevice::Export zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (version < 1 || version>1) {
		throw UnknownRecordVersion("Device %d", version);
	}
	size_t size=bin.size();
	const char* a=(const char*)bin.ptr();
	// Die Größe muss mindestens 48 Byte betragen
	if (size < 48 || a == NULL) {
		throw InvalidRecord();
	}
	Clear();
	DeviceType=ppl7::Peek8(a);
	DeviceId=ppl7::Peek32(a + 1);
	Pages=ppl7::Peek8(a + 5);
	Length=ppl7::Peek32(a + 6);
	Recorded=ppl7::Peek32(a + 10);
	PurchaseId=ppl7::Peek16(a + 14);
	LabelId=ppl7::Peek32(a + 16);
	PurchaseDate=ppl7::Peek32(a + 20);
	PurchasePrice=ppl6::PeekFloat(a + 24);
	DateCreated=ppl7::Peek32(a + 28);
	FirstDate=ppl7::Peek32(a + 32);
	LastDate=ppl7::Peek32(a + 36);
	NumTracks=ppl7::Peek32(a + 40);
	int lenTitle=ppl7::Peek16(a + 44);
	int lenSubTitle=ppl7::Peek16(a + 46 + lenTitle);
	if (lenTitle) Title=ppl6::strndup(a + 46, lenTitle);
	if (lenSubTitle) SubTitle=ppl6::strndup(a + 48 + lenTitle, lenSubTitle);
}


/*!\class CDeviceStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Tonträgern
 *
 * Klasse zum Verwalten von DataDevice Datensätzen
 *
 */


/*!\var CDeviceStore::Mutex
 * \brief Mutex der Klasse
 *
 * Die Klasse verwendet einen Mutex bei allen Zugriffen und ist somit Threadsicher.
 */

/*!\var CDeviceStore::Tree
 * \brief Interner Binary Tree zur Verwaltung der Datensätze
 *
 * Interner Binary Tree zur Verwaltung der Datensätze
 */

CDeviceStore::CDeviceStore()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse. Initialisiert die internen Variablen.
 */
{
	for (int i=0;i <= MAX_DEVICE_TYPES;i++) highest[i]=0;
}

CDeviceStore::~CDeviceStore()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Stellt sicher, dass sämtlicher durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen wird. Dazu wird die Member-Funktion CDeviceStore::Clear
 * aufgerufen.
 */
{
	Clear();
}

void CDeviceStore::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion kann der komplette durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen werden.
 */
{
	// Erfreulicherweise bringt die Tree-Klasse der PPL-Library schon eine Clear-Funktion mit,
	// die alle Datensätze löscht.
	Mutex.lock();
	Tree.clear();
	Mutex.unlock();
}

const char* CDeviceStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return "DEVI";
}

int CDeviceStore::Save(DataDevice* t)
/*!\brief Datensatz auf Festplatte schreiben
 *
 * Diese Funktion speichert den übergebenen Datensatz auf die Festplatte. Dazu wird der Inhalt
 * zunächst als Binary exportiert (DataDevice::Export) und damit CStorage::Save aufgerufen.
 * Die Funktion wird intern von CDeviceStore::Put verwendet.
 *
 * \param[in] t Pointer auf einen DataDevice Datensatz
 * \returns Konnte der Datensatz erfolgreich gespeichert werden, liefert die Funktion 1 zurück, sonst
 * 0.
 */
{
	if (!Storage) {
		return 0;
	}
	if (!t) {
		ppl6::SetError(194, "int CDeviceStore::Save(==> DataDevice *t <==)");
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


int CDeviceStore::Renumber(ppluint8 DeviceType, ppluint32 oldId, ppluint32 newId)
{
	DATADEVICE dd;
	Mutex.Lock();

	// Neue ID darf nicht existieren
	dd.DeviceId=newId;
	dd.DeviceType=DeviceType;
	DataDevice* t=(DataDevice*)Tree.Find((void*)&dd);
	if (t) {
		printf("ERROR: Neue DeviceId bereits vorhanden!\n");
		Mutex.Unlock();
		return 0;
	}

	// Alte ID muss vorhanden sein
	dd.DeviceId=oldId;
	dd.DeviceType=DeviceType;
	t=(DataDevice*)Tree.Find((void*)&dd);
	if (!t) {
		printf("ERROR: Alte DeviceId nicht gefunden!\n");
		Mutex.Unlock();
		return 0;
	}

	// Alten Datensatz aus dem Suchbaum enfernen
	Tree.Delete(t);

	// Datensatz mit neuer ID speichern
	t->DeviceId=newId;
	if (!Save(t)) {
		printf("ERROR: Speichern fehlgeschlagen!\n");
		t->DeviceId=oldId;
		Tree.Add(t);
		Mutex.Unlock();
		return 0;
	}
	if (t->DeviceId > highest[t->DeviceType]) highest[t->DeviceType]=t->DeviceId;
	// Datensatz mit neuer ID in den Suchbaum aufnehmen
	Tree.Add(t);
	Mutex.Unlock();
	return 1;
}

int CDeviceStore::Put(DataDevice* entry)
/*!\brief Datensatz speichern
 *
 * Mit dieser Funktion wird ein veränderter oder neuer Datensatz im Speicher der Anwendung und
 * auf Festplatte gespeichert.
 *
 * \param[in] entry Pointer auf den gewünschten Datensatz vom Typ DataDevice. Dieser wird nicht
 * selbst gespeichert, sondern es wird eine Kopie erstellt. Der Wert muss daher von der Anwendung
 * nach Gebrauch wieder freigegeben werden.
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 * \note Es ist sichergestellt, dass der interne Suchbaum stets sortiert ist, auch wenn sich der
 * Text des Elements geändert hat.
 */
{
	if (!entry) {
		ppl6::SetError(194, "int CDeviceStore::Put(==> DataDevice *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014, "CDeviceStore");
		return 0;
	}
	Mutex.Lock();
	// Gibt's den Tonträger schon?
	DataDevice* t;
	DATADEVICE dd;
	dd.DeviceId=entry->DeviceId;
	dd.DeviceType=entry->DeviceType;
	t=(DataDevice*)Tree.Find((void*)&dd);
	if (t) {	// Jepp, gibt's schon. Wir machen ein Update
		// CopyFrom führt ein Clear aus, daher müssen wir die Storage Daten retten
		CStorageItem ssave;
		ssave.CopyStorageFrom(t);
		// Nun können wir die Daten kopieren
		if (!t->CopyFrom(entry)) {
			Mutex.Unlock();
			return 0;
		}
		// StorageDaten wieder herstellen
		t->CopyStorageFrom(&ssave);
		if (!Save(t)) {
			Mutex.Unlock();
			return 0;
		}
		// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
		entry->CopyStorageFrom(t);
		if (entry->DeviceId > highest[entry->DeviceType]) highest[entry->DeviceType]=entry->DeviceId;
		Mutex.Unlock();
		return 1;
	}
	// Nein, neuer Track
	t=new DataDevice;
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
	if (t->DeviceId == 0) {
		t->DeviceId=GetHighestDevice(t->DeviceType) + 1;
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
	if (t->DeviceId > highest[t->DeviceType]) highest[t->DeviceType]=t->DeviceId;
	entry->CopyFrom(t);
	Mutex.Unlock();
	return 1;
}

DataDevice* CDeviceStore::Get(ppluint8 DeviceType, ppluint32 DeviceId)
/*!\brief Datensatz finden
 *
 * Mit dieser Funktion kann ein bestimmer Tonträger ausgelesen werden
 *
 * \param[in] DeviceType ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \returns Wurde der gewünschte Tonträger gefunden, wird ein Pointer auf den
 * DataDevice Datensatz zurückgeliefert. Wurde er nicht
 * gefunden, wird NULL zurückgegeben.
 */
{
	DATADEVICE dd;
	dd.DeviceType=DeviceType;
	dd.DeviceId=DeviceId;
	DataDevice* t;
	Mutex.Lock();
	t=(DataDevice*)Tree.Find((void*)&dd);
	Mutex.Unlock();
	return t;
}

int CDeviceStore::GetCopy(ppluint8 DeviceType, ppluint32 DeviceId, DataDevice* t)
/*!\brief Kopie eines Datensatzes erstellen
 *
 * Mit dieser Funktion kann eine Kopie eines vorhandenen Datensatzes erstellt werden.
 *
 * \param[in] DeviceType ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \param[in] DeviceId Tonträger-ID (die Nummer des Tonträgers)
 * \param[in,out] t Pointer auf eine Klasse vom Typ DataDevice
 *
 * \returns Ist der Datensatz vorhanden und konnte kopiert werden, liefert die Funktion 1
 * zurück, sonst 0.
 */
{
	DATADEVICE dd;
	dd.DeviceType=DeviceType;
	dd.DeviceId=DeviceId;
	DataDevice* found;
	Mutex.Lock();
	found=(DataDevice*)Tree.Find((void*)&dd);
	if (found) t->CopyFrom(found);
	Mutex.Unlock();
	if (found) return 1;
	return 0;
}

int CDeviceStore::LoadChunk(CWMFileChunk* chunk)
{
	DataDevice data;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk->GetChunkData(), chunk->GetChunkDataSize())) return 0;
	if (!data.Import(&bin, chunk->GetFormatVersion())) return 0;
	data.CopyStorageFrom(chunk);
	return Put(&data);
}

int CDeviceStore::Update(ppluint8 DeviceType, ppluint32 DeviceId)
{
	DATADEVICE dd;
	dd.DeviceType=DeviceType;
	dd.DeviceId=DeviceId;
	DataDevice* t;
	CTrackList* tracks;
	DataTrack* dt;
	const DataTitle* ti;
	Mutex.Lock();
	t=(DataDevice*)Tree.Find((void*)&dd);
	if (t) {
		DataDevice previous;
		previous.NumTracks=t->NumTracks;
		previous.Recorded=t->Recorded;
		previous.FirstDate=t->FirstDate;
		previous.LastDate=t->LastDate;

		t->NumTracks=0;
		t->Recorded=0;
		t->FirstDate=0;
		t->LastDate=0;

		for (ppluint8 page=1;page <= t->Pages;page++) {
			tracks=wm_main->GetTracklist(DeviceType, DeviceId, page);
			if (tracks) {
				tracks->Reset();
				while ((dt=tracks->GetNext())) {
					ti=wm_main->GetTitle(dt->TitleId);
					if (ti) {
						t->NumTracks++;
						t->Recorded+=ti->Length;
						if (ti->RecordDate > 0) {
							if (ti->RecordDate > t->LastDate) t->LastDate=ti->RecordDate;
							if (ti->RecordDate < t->FirstDate || t->FirstDate == 0) t->FirstDate=ti->RecordDate;
						}
					}
				}
				delete tracks;
			}
		}
		// Save only, if something changed
		if (previous.NumTracks != t->NumTracks ||
			previous.Recorded != t->Recorded ||
			previous.FirstDate != t->FirstDate ||
			previous.LastDate != t->LastDate
			) Save(t);
	}
	Mutex.Unlock();
	return 1;
}

ppluint32 CDeviceStore::GetHighestDevice(int DeviceType)
/*!\brief Höchste verwendete ID eines Tonträgers
 *
 * Diese Funktion liefert die Nummer des höchsten verwendeten Tonträges für einen
 * bestimmten Typ zurück.
 *
 * \param[in] DeviceType ID für die Art des Tonträgers (z.B. CD, Kassette, MP3-CD, etc.)
 * \returns Nummer des höchsten Tonträgers oder 0, wenn noch kein Tonträger angelegt
 * ist oder ein ungültiger \p DeviceType angegeben wurde.
 */
{
	if (DeviceType<1 || DeviceType>MAX_DEVICE_TYPES) return 0;
	return highest[DeviceType];
}
