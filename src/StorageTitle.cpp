/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.4 $
 * $Date: 2010/11/13 21:38:58 $
 * $Id: StorageTitle.cpp,v 1.4 2010/11/13 21:38:58 pafe Exp $
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


TrackInfo::TrackInfo()
{

}

void TrackInfo::clear()
{
	Ti.Clear();
	Version.Clear();
	Genre.Clear();
	Label.Clear();
	RecordingSource.Clear();
	RecordingDevice.Clear();
}

/*!\class DataTitle
 * \ingroup WM3DataTypes
 * \brief Speicherklasse für einen Titel
 *
 * Diese Klasse repräsentiert einen Musiktitel im Hauptspeicher.
 * Mit CTitleStore::Get kann ein vorhandener Titel gelesen, mit CTitleStore::Put gespeichert werden.
 * \par Formatversionen
 * - Version 1: Initiale Version
 * - Version 2: Cover-Icon und Dateigröße hinzugekommen
 * - Version 3: Tags hinzugekommen
 * - Version 4: Tonart (Key) hinzugekommen
 */

/*!\var DataTitle::Artist
 * \brief Interpret
 */

/*!\var DataTitle::Title
 * \brief Titel
 */

/*!\var DataTitle::Remarks
 * \brief Optionale Bemerkung
 */

/*!\var DataTitle::Tags
 * \brief Optionale Suchtags
 */

/*!\var DataTitle::Album
 * \brief Optionaler Name des Albums
 */

/*!\var DataTitle::TitleId
 * \brief TitelID
 */

/*!\var DataTitle::DeviceId
 * \brief ID des Tonträgers
 */

/*!\var DataTitle::Length
 * \brief Länge des Titels in Sekunden
 */

/*!\var DataTitle::VersionId
 * \brief Pointer auf ID in der Version-Tabelle
 */

/*!\var DataTitle::LabelId
 * \brief Pointer auf ID in der Label-Tabelle
 */

/*!\var DataTitle::BPM
 * \brief Beats per Minute mit 1000 multipliziert
 */

/*!\var DataTitle::RecordDate
 * \brief Aufnahmedatum als Integer im Format yyyymmdd
 */


/*!\var DataTitle::ReleaseDate
 * \brief Veröffentlichungsdatum als Integer im Format yyyymmdd
 *
 * Das Veröffentlichungsdatum als Integer im Format yyyymmdd.
 * Ist kein genaues Datum bekannt, kann hier auch nur das Jahr oder Jahr und Monat gespeichert sein. Die
 * unbekannten Teile haben dann den Wert 0.
 *
 * Beispiel 1: Das Jahr ist 2008, der Rest ist unbekannt. Wert: 20080000
 *
 * Beispiel 2: Das Jahr ist 2008, der Monat 8, Tag unbekannt. Wert: 20080800
 *
 */

/*!\var DataTitle::RecordSourceId
 * \brief Verweis auf Tabelle der Aufnahmequellen
 */

/*!\var DataTitle::Track
 * \brief Track-Nummer
 *
 * Track-Nummer, es sind maximal 65535 Tracks pro Seite möglich.
 */

/*!\var DataTitle::Bitrate
 * \brief Bitrate bei MP3-Dateien in Kbit
 *
 * Bitrate bei MP3-Dateien in Kbit. Kann 0 sein, wenn die Bitrate unbekannt oder variable ist.
 * Ist nur bei MP3-Dateien gesetzt, sonst 0.
 *
 */

/*!\var DataTitle::GenreId
 * \brief Pointer auf ID in der Genre-Tabelle
 */

/*!\var DataTitle::RecordDeviceId
 * \brief Pinter auf ID in der Geräte-Tabelle
 */

/*!\var DataTitle::DeviceType
 * \brief Typ des Tonträgers
 */

/*!\var DataTitle::Page
 * \brief Seite auf dem Tonträger
 */

/*!\var DataTitle::Channels
 * \brief Index auf Anzahl/Typ der Kanäle
 *
 * Index auf Anzahl/Typ der Kanäle
 * \verbatim
0 = unbekannt
1 = Mono
2 = Stereo
3 = Dolby Stereo
4 = Dolby Surround
5 = Dolby Digital 5.1
6 = Dolby Digital 5.1EX
7 = DTS
\endverbatim
 *
 */

/*!\var DataTitle::Quality
 * \brief Klangqualität des Titels
 *
 * Klangqualität des Titels
 *
 * \verbatim
0 = unbekannt
1 = sehr gut
2 = gut
3 = mittelmäßig
4 = schlecht
5 = sehr schlecht
\endverbatim
 *
 */

/*!\var DataTitle::Rating
 * \brief Bewertung
 *
 * Bewertung des Titels
 * \verbatim
0 = keine Bewertung
1 = 1 Stern (sehr schlecht)
2 = 2 Sterne (schlecht)
3 = 3 Sterne (mittelmäßig)
4 = 4 Sterne (gut)
5 = 5 Sterne (sehr gut)
6 = 6 Sterne (absoluter Knaller)
\endverbatim
 *
 */

/*!\var DataTitle::Flags
 * \brief Verschiedene Flags
 *
 * Die Variable Flags enthält ein Bitfeld mit verschiedenen Flags:
 *
 * \verbatim
Bit 0: Titel ist komplett                                  (1)
Bit 1: Es handelt sich um einen Musiktitel                 (2)
Bit 2: MP3-Datei mit variabler Bitrate                     (4)
Bit 3: Titel enthält Jingle oder es wurde reingeredet      (8)
\endverbatim
 *
 */

/*!\var DataTitle::Key
 * \brief Tonart
 *
 * Gibt die initiale Tonart des Songs im Quintenzirkel an.
 *
 * \verbatim
0: unbekannt
1: A-Flat Minor (G#m)
2: B Major
3: E-Flat Minor (D#m)
4: F-Sharp Major
5: B-Flat Minor (A#m)
6: D-Flat Major (C#)
7: F Minor
8: A-Flat Major
9: C Minor
10: E-Flat Major
11: G Minor
12: B-Flat Major
13: D Minor
14: F Major
15: A Minor
16: C Major
17: E Minor
18: G Major
19: B Minor
20: D Major
21: F-Sharp Minor
22: A Major
23: D-Flat Minor (C#m)
24: E Major
\endverbatim
 *
 */

/*!\var DataTitle::ImportData
 * \brief Verweis auf Original-Importdaten (nur MP3)
 *
 * Beim Import von ID3-Tags bei MP3-Dateien können die Originaldaten gesichert werden. Dabei
 * werden der Original ID3v2- und ID3v1-Tag sowie der Dateinamen gespeichert.
 *
 * In dieser Variable wird die ID dieses Datensatzen gespeichert, so dass später leicht
 * nochmal auf die Originaldaten zugegriffen werden kann.
 *
 */

/*!\var DataTitle::Size
 * \brief Größe der Datei bei MP3-Dateien in Bytes
 *
 * Größe der Datei bei MP3-Dateien in Bytes. Ist 0, wenn auf die Datei nicht zugegriffen
 * werden kann, oder es sich nicht um eine MP3-Datei handelt.
 */

/*!\var DataTitle::CoverPreview
 * \brief Preview-Image des Covers
 *
 * Nur bei MP3-Dateien: Ein Preview des Covers in der Größe 64 x 64 im JPEG-Format.
 */

DataTitle::DataTitle()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse. Er setzt alle Variablen auf 0.
 */
{
	Artist=NULL;
	Title=NULL;
	Remarks=NULL;
	Album=NULL;
	Tags=NULL;
	Clear();
	formatversion=4;
}

DataTitle::DataTitle(const DataTitle &other)
: CStorageItem(other)
{
	Artist=NULL;
	Title=NULL;
	Remarks=NULL;
	Album=NULL;
	Tags=NULL;
	formatversion=4;
	CopyFrom(&other);
}

DataTitle::~DataTitle()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Gibt sämtlichen durch diese Klasse allokierten Speicher
 * wieder frei.
 */
{
	if (Artist) free(Artist);
	if (Title) free(Title);
	if (Remarks) free(Remarks);
	if (Album) free(Album);
	if (Tags) free(Tags);
}

#ifdef OWNHEAP
void *DataTitle::operator new (size_t size)
/*!\brief Speicher allokieren
 *
 * Der New-Operator dieser Klasse wurde überschrieben, damit der Speicher nicht mehr aus dem
 * Hauptspeicher allokiert wird und diesen somit schnell fragmentiert, sondern aus dem internen
 * Heap.
 *
 * \param[in] size Größe des benötigten Speicherblocks. Dieser muss identisch sein mit
 * sizeof(CString)
 *
 * \returns bei Erfolg gibt die Funktion einen Pointer auf den Beginn des Speicherblocks zurück,
 * im Fehlerfall wird eine Exception generiert
 *
 */
{
	//printf ("Alloc %i Bytes for DataTitle\n",size);
	void *mem=malloc(size);
	return mem;
}

void DataTitle::operator delete (void *ptr, size_t size)
/*!\brief Speicher freigeben
 *
 * Die Klasse verwendet Speicher aus dem internen Heap, der durch Aufruf dieses überladenen
 * delete-Operators wieder freigegeben wird
 *
 * \param[in] ptr Zeiger auf den freizugebenden Speicherblock
 * \param[in] size Größe des Speicherblocks.
 */
{
	//printf ("Delete\n");
	free(ptr);
}
#endif


DataTitle & DataTitle::operator=(const DataTitle &other)
{
	CopyFrom(&other);
	return *this;
}

void DataTitle::Clear()
/*!\brief Inhalt der Klasse löschen
 *
 * Mit dieser Funktion wird der Inhalt der Klasse gelöscht und zusätzlich allokierter Speicher
 * freigegeben.
 * \attention
 * Sie darf nicht für Titel aufgerufen werden, die über CTitleStore::Get ausgelesen
 * wurden, da diese von CTitleStore verwaltet werden. Ggfs. kann CTitleStore::GetCopy verwendet
 * werden, wodurch eine Kopie angefertigt wird.
 */
{
	if (Artist) free(Artist);
	if (Title) free(Title);
	if (Remarks) free(Remarks);
	if (Album) free(Album);
	if (Tags) free(Tags);
	Artist=NULL;
	Title=NULL;
	Remarks=NULL;
	Album=NULL;
	Tags=NULL;
	TitleId=0;
	DeviceId=0;
	Length=0;
	VersionId=0;
	LabelId=0;
	BPM=0;
	RecordDate=0;
	ReleaseDate=0;
	RecordSourceId=0;
	Track=0;
	Bitrate=0;
	GenreId=0;
	RecordDeviceId=0;
	DeviceType=0;
	Page=0;
	Channels=0;
	Quality=0;
	Rating=0;
	Flags=0;
	ImportData=0;
	CoverPreview.Clear();
	CStorageItem::Clear();
	formatversion=4;
	Key=0;
}


int DataTitle::CopyFrom(const DataTitle &other)
{
	return CopyFrom(&other);
}

int DataTitle::CopyFrom(const DataTitle *t)
/*!\brief Daten kopieren
 *
 * Mit dieser Funktion werden die Daten einers anderen DataTitle Datensatzes in diesen hineinkopiert.
 * Bereits vorhandene Daten werden dabei überschrieben.
 *
 * \param[in] t Pointer auf einen anderen Datensatz vom Typ DataTitle
 *
 * \returns Konnten die Daten erfolgreich kopiert werden, liefert die Funktion 1 zurück, im Fehlerfall 0.
 *
 * \attention
 * Auch die Storage-Daten werden von \p t übernommen.
 */
{
	if (!t) {
		return 0;
	}
	Clear();
	if (t->Artist) Artist=strdup(t->Artist);
	if (t->Title) Title=strdup(t->Title);
	if (t->Remarks) Remarks=strdup(t->Remarks);
	if (t->Album) Album=strdup(t->Album);
	if (t->Tags) Tags=strdup(t->Tags);
	TitleId=t->TitleId;
	DeviceId=t->DeviceId;
	Length=t->Length;
	VersionId=t->VersionId;
	GenreId=t->GenreId;
	RecordSourceId=t->RecordSourceId;
	RecordDeviceId=t->RecordDeviceId;
	LabelId=t->LabelId;
	BPM=t->BPM;
	RecordDate=t->RecordDate;
	ReleaseDate=t->ReleaseDate;
	Track=t->Track;
	Bitrate=t->Bitrate;
	DeviceType=t->DeviceType;
	Page=t->Page;
	Channels=t->Channels;
	Quality=t->Quality;
	Rating=t->Rating;
	Flags=t->Flags;
	ImportData=t->ImportData;
	CoverPreview=t->CoverPreview;
	Size=t->Size;
	Key=t->Key;
	CopyStorageFrom(t);
	return 1;
}

void DataTitle::SetTitle(const char *title)
/*!\brief Songtitel setzen
 *
 * Mit dieser Funktion wird der Titel des Songs gesetzt.
 *
 * \param[in] title Pointer auf einen String mit dem Titel.
 * \returns Liefert 1 zurück, wenn der Titel erfolgreich gesetzt werden konnte.
 */
{
	if (Title) free(Title);
	Title=NULL;
	if (title!=NULL && strlen(title)>0) Title=strdup(title);
}

void DataTitle::SetArtist(const char *artist)
/*!\brief Interpreten setzen
 *
 * Mit dieser Funktion wird der Interpret des Songs gesetzt.
 *
 * \param[in] artist Pointer auf einen String mit dem Namen des Interpreten.
 * \returns Liefert 1 zurück, wenn der Interpret erfolgreich gesetzt werden konnte.
 */
{
	if (Artist) free(Artist);
	Artist=NULL;
	if (artist!=NULL && strlen(artist)>0) Artist=strdup(artist);
}

void DataTitle::SetRemarks(const char *remarks)
/*!\brief Bemerkung setzen
 *
 * Mit dieser Funktion kann eine Bemerkung zu dem Titel hinterlegt werden.
 *
 * \param[in] remarks Pointer auf einen String mit der Bemerkung.
 * \returns Liefert 1 zurück, wenn die Bemerkung erfolgreich gesetzt werden konnte.
 */

{
	if (Remarks) free(Remarks);
	Remarks=NULL;
	if (remarks!=NULL && strlen(remarks)>0) Remarks=strdup(remarks);
}

void DataTitle::SetTags(const char *tags)
/*!\brief Suchtags setzen
 *
 * Mit dieser Funktion können optionale Suchtags gesetzt werden.
 *
 * \param[in] tags Pointer auf einen String mit den Suchtags
 * \returns Liefert 1 zurück, wenn die Tags erfolgreich gesetzt werden konnte.
 */

{
	if (Tags) free(Tags);
	Tags=NULL;
	if (tags!=NULL && strlen(tags)>0) Tags=strdup(tags);
}



void DataTitle::SetAlbum(const char *album)
/*!\brief Album setzen
 *
 * Mit dieser Funktion kann der Name des Albums gesetzt werden, von dem dieser Titel stammt.
 *
 * \param[in] album Pointer auf einen String mit dem Namen des Albums.
 * \returns Liefert 1 zurück, wenn das Album erfolgreich gesetzt werden konnte.
 */
{
	if (Album) free(Album);
	Album=NULL;
	if (album!=NULL && strlen(album)>0) Album=strdup(album);
}

void DataTitle::SetTitle(const ppl6::CString &title)
/*!\brief Songtitel setzen
 *
 * Mit dieser Funktion wird der Titel des Songs gesetzt.
 *
 * \param[in] title Pointer auf einen String mit dem Titel.
 * \returns Liefert 1 zurück, wenn der Titel erfolgreich gesetzt werden konnte.
 */
{
	if (Title) free(Title);
	Title=NULL;
	if (title.NotEmpty()) Title=strdup((const char*)title);
}

void DataTitle::SetArtist(const ppl6::CString &artist)
/*!\brief Interpreten setzen
 *
 * Mit dieser Funktion wird der Interpret des Songs gesetzt.
 *
 * \param[in] artist Pointer auf einen String mit dem Namen des Interpreten.
 * \returns Liefert 1 zurück, wenn der Interpret erfolgreich gesetzt werden konnte.
 */
{
	if (Artist) free(Artist);
	Artist=NULL;
	if (artist.NotEmpty()) Artist=strdup((const char*)artist);
}

void DataTitle::SetRemarks(const ppl6::CString &remarks)
/*!\brief Bemerkung setzen
 *
 * Mit dieser Funktion kann eine Bemerkung zu dem Titel hinterlegt werden.
 *
 * \param[in] remarks Pointer auf einen String mit der Bemerkung.
 * \returns Liefert 1 zurück, wenn die Bemerkung erfolgreich gesetzt werden konnte.
 */

{
	if (Remarks) free(Remarks);
	Remarks=NULL;
	if (remarks.NotEmpty()) Remarks=strdup((const char*)remarks);
}

void DataTitle::SetTags(const ppl6::CString &tags)
/*!\brief Suchtags setzen
 *
 * Mit dieser Funktion können optionale Suchtags gesetzt werden.
 *
 * \param[in] tags Pointer auf einen String mit den Suchtags
 * \returns Liefert 1 zurück, wenn die Tags erfolgreich gesetzt werden konnte.
 */

{
	if (Tags) free(Tags);
	Tags=NULL;
	if (tags.NotEmpty()) Tags=strdup((const char*)tags);
}


void DataTitle::SetKey(const ppl6::CString &key)
{
	Key=DataTitle::keyId(key);
}

ppl6::CString DataTitle::getKeyName()
{
	return DataTitle::keyName(Key);
}

ppl6::CString DataTitle::keyName(int id)
{
	switch (id) {
		case 1: return "G#m";
		case 2: return "B";
		case 3: return "D#m";
		case 4: return "F#";
		case 5: return "A#m";
		case 6: return "C#";
		case 7: return "Fm";
		case 8: return "G#";
		case 9: return "Cm";
		case 10: return "D#";
		case 11: return "Gm";
		case 12: return "A#";
		case 13: return "Dm";
		case 14: return "F";
		case 15: return "Am";
		case 16: return "C";
		case 17: return "Em";
		case 18: return "G";
		case 19: return "Bm";
		case 20: return "D";
		case 21: return "F#m";
		case 22: return "A";
		case 23: return "C#m";
		case 24: return "E";
		default: return "";
	}
}

int DataTitle::keyId(const ppl6::CString &name)
{
	ppl6::CString k=ppl6::LCase(ppl6::Trim(name));
	if (k.IsEmpty()) return 0;
	if (k=="g#m") return 1;
	else if (k=="b") return 2;
	else if (k=="d#m") return 3;
	else if (k=="f#") return 4;
	else if (k=="a#m") return 5;
	else if (k=="c#") return 6;
	else if (k=="fm") return 7;
	else if (k=="g#") return 8;
	else if (k=="cm") return 9;
	else if (k=="d#") return 10;
	else if (k=="gm") return 11;
	else if (k=="a#") return 12;
	else if (k=="dm") return 13;
	else if (k=="f") return 14;
	else if (k=="am") return 15;
	else if (k=="c") return 16;
	else if (k=="em") return 17;
	else if (k=="g") return 18;
	else if (k=="bm") return 19;
	else if (k=="d") return 20;
	else if (k=="f#m") return 21;
	else if (k=="a") return 22;
	else if (k=="c#m") return 23;
	else if (k=="e") return 24;
	return 0;
}


void DataTitle::SetAlbum(const ppl6::CString &album)
/*!\brief Album setzen
 *
 * Mit dieser Funktion kann der Name des Albums gesetzt werden, von dem dieser Titel stammt.
 *
 * \param[in] album Pointer auf einen String mit dem Namen des Albums.
 * \returns Liefert 1 zurück, wenn das Album erfolgreich gesetzt werden konnte.
 */
{
	if (Album) free(Album);
	Album=NULL;
	if (album.NotEmpty()) Album=strdup((const char*)album);
}

ppl6::CBinary *DataTitle::Export()
/*!\brief Binäre Exportfunktion
 *
 * Mit dieser Funktion werden die Daten der Klasse in binärer Form exportiert. Das Format ist
 * folgendermassen:
 *
 * \copydoc WM3File_Chunk_TITL
 *
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf eine neue ppl6::CBinary Klasse zurück.
 * Diese muss nach Gebrauch vom aufrufenden Programm selbst gelöscht werden. Im Fehlerfall wird NULL
 * zurückgegeben.
 */
{
	// Zunächst den benötigten Speicher berechnen
	int size=69;
	int p=0;
	int lenArtist=0;
	int lenTitle=0;
	int lenRemarks=0;
	int lenAlbum=0;
	int lenTags=0;
	formatversion=4;

	if (Artist) lenArtist=strlen(Artist);
	if (Title) lenTitle=strlen(Title);
	if (Remarks) lenRemarks=strlen(Remarks);
	if (Tags) lenTags=strlen(Tags);
	if (Album) lenAlbum=strlen(Album);
	size=size+lenArtist+lenTitle+lenRemarks+lenAlbum+lenTags+CoverPreview.Size();
	//size=size+lenArtist+lenTitle+lenRemarks+lenAlbum;
	char *a=(char*)malloc(size);
	if (!a) {
		ppl6::SetError(2);
		return NULL;
	}
	ppl6::Poke32(a+0,TitleId);
	ppl6::Poke8(a+4,Flags);
	ppl6::Poke32(a+5,DeviceId);
	ppl6::Poke32(a+9,Length);
	ppl6::Poke32(a+13,VersionId);
	ppl6::Poke32(a+17,LabelId);
	ppl6::Poke32(a+21,BPM);
	ppl6::Poke32(a+25,RecordDate);
	ppl6::Poke32(a+29,ReleaseDate);
	ppl6::Poke16(a+33,RecordSourceId);
	ppl6::Poke16(a+35,Track);
	ppl6::Poke16(a+37,Bitrate);
	ppl6::Poke16(a+39,GenreId);
	ppl6::Poke16(a+41,RecordDeviceId);
	ppl6::Poke8(a+43,DeviceType);
	ppl6::Poke8(a+44,Page);
	ppl6::Poke8(a+45,Channels);
	ppl6::Poke8(a+46,Quality);
	ppl6::Poke8(a+47,Rating);
	ppl6::Poke32(a+48,ImportData);
	ppl6::Poke32(a+52,Size);
	ppl6::Poke8(a+56,Key);
	p=57;
	ppl6::Poke16(a+p,lenArtist);
	if (lenArtist) strncpy(a+p+2,Artist,lenArtist);
	p+=lenArtist+2;
	ppl6::Poke16(a+p,lenTitle);
	if (lenTitle) strncpy(a+p+2,Title,lenTitle);
	p+=lenTitle+2;
	ppl6::Poke16(a+p,lenRemarks);
	if (lenRemarks) strncpy(a+p+2,Remarks,lenRemarks);
	p+=lenRemarks+2;
	ppl6::Poke16(a+p,lenAlbum);
	if (lenAlbum) strncpy(a+p+2,Album,lenAlbum);
	p+=lenAlbum+2;
	ppl6::Poke16(a+p,lenTags);
	if (lenTags) strncpy(a+p+2,Tags,lenTags);
	p+=lenTags+2;

	ppl6::Poke16(a+p,CoverPreview.Size());
	if (CoverPreview.Size()) memcpy(a+p+2,CoverPreview.GetPtr(),CoverPreview.Size());
	p+=CoverPreview.Size()+2;


	/*
	ppluint32 crc=ppl6::crc32(a+4,size-4);
	ppl6::Poke32(a,crc);
	*/

	ppl6::CBinary *bin=new ppl6::CBinary;
	bin->Set(a,size);
	bin->ManageMemory();
	return bin;
}

int DataTitle::Import(ppl6::CBinary *bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion werden binäre gespeicherte Daten in die Klasse importiert. Eine Beschreibung des
 * binären Formats ist in der Funktion DataTitle::Export zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (!bin) {
		ppl6::SetError(194,"int DataTitle::Import(==> ppl6::CBinary *bin <==)");
		return 0;
	}
	if (version<1 || version>4) {
		ppl6::SetError(20023,"%i",version);
		return 0;
	}
	int size=bin->Size();
	const char *a=(const char*)bin->GetPtr();
	// Die Größe muss mindestens 60 Byte betragen
	if (size<60 || a==NULL) {
		ppl6::SetError(20011);
		return 0;
	}
	/*
	ppluint32 crc=ppl6::crc32(a+4,size-4);
	if (crc!=ppl6::Peek32(a)) {
		ppl6::SetError(20012);
		return 0;
	}
	*/
	Clear();
	int p=0;
	int len;
	TitleId=ppl6::Peek32(a+0);
	Flags=ppl6::Peek8(a+4);
	DeviceId=ppl6::Peek32(a+5);
	Length=ppl6::Peek32(a+9);
	VersionId=ppl6::Peek32(a+13);
	LabelId=ppl6::Peek32(a+17);
	BPM=ppl6::Peek32(a+21);
	RecordDate=ppl6::Peek32(a+25);
	ReleaseDate=ppl6::Peek32(a+29);
	RecordSourceId=ppl6::Peek16(a+33);
	Track=ppl6::Peek16(a+35);
	Bitrate=ppl6::Peek16(a+37);
	GenreId=ppl6::Peek16(a+39);
	RecordDeviceId=ppl6::Peek16(a+41);
	DeviceType=ppl6::Peek8(a+43);
	Page=ppl6::Peek8(a+44);
	Channels=ppl6::Peek8(a+45);
	Quality=ppl6::Peek8(a+46);
	Rating=ppl6::Peek8(a+47);
	ImportData=ppl6::Peek32(a+48);
	if (version==1) {
		p=52;
		Size=0;
	} else if (version>=2) {
		Size=ppl6::Peek32(a+52);
		p=56;
	}
	if (version >= 4) {
		Key=ppl6::Peek8(a+56);
		p=57;
	}
	len=ppl6::Peek16(a+p);
	if (len) Artist=ppl6::strndup(a+p+2,len);
	p+=len+2;
	len=ppl6::Peek16(a+p);
	if (len) Title=ppl6::strndup(a+p+2,len);
	p+=len+2;
	len=ppl6::Peek16(a+p);
	if (len) Remarks=ppl6::strndup(a+p+2,len);
	p+=len+2;
	len=ppl6::Peek16(a+p);
	if (len) Album=ppl6::strndup(a+p+2,len);
	p+=len+2;
	if (version>=3) {
		len=ppl6::Peek16(a+p);
		if (len) Tags=ppl6::strndup(a+p+2,len);
		p+=len+2;
	}
	if (version>=2) {
		len=ppl6::Peek16(a+p);
		if (len) CoverPreview.Copy(a+p+2,len);
	}
	return 1;
}

/*!\class CStringCounterItem
 * \brief Zählklasse für Strings
 *
 * Diese Klasse wird verwendet, um das Vorkommen von Strings zu zählen. Sie repräsentiert dabei
 * einen einzelnen Datensatz in einem binären AVL-Baum, weswegen sie von ppl6::CTreeItem abgeleitet ist.
 *
 * Die Klasse wird beispielsweise eingesetzt, um zu zählen, wie häufig der gleiche Interpret in
 * der Datenbank vorkommt.
 */

/*!\var CStringCounterItem::Name
 * \brief Enthält den String im Unicode-Format (Wide-Character)
 */

/*!\var CStringCounterItem::Count
 * \brief Zähler wie oft der String vorkommt
 */

int CStringCounterItem::CompareNode(CTreeItem *item)
{
	CStringCounterItem *s=(CStringCounterItem*)item;
	int ret=s->Name.StrCmp(Name);
	//printf ("Item:   %s\nString 2: %s\nResult: %i\n",(char*)s->Name,(char*)Name,ret);
	if (ret<0) return -1;
	if (ret>0) return 1;
	return 0;
}

int CStringCounterItem::CompareValue(void *value)
{
	ppl6::CWString *s=(ppl6::CWString*)value;
	int ret=s->StrCmp(Name);
	if (ret<0) return -1;
	if (ret>0) return 1;
	return 0;
}


/*!\class CTitleStore
 * \ingroup WM3StorageClasses
 * \brief Speicherklasse für Musiktitel-Datensätze
 */
CTitleStore::CTitleStore()
{
	TitleIndex=NULL;
	max=0;
	highestId=0;
	Storage=NULL;
	highestOimp=0;
}

CTitleStore::~CTitleStore()
{
	Clear();
}

void CTitleStore::Clear()
{
	for (ppluint32 i=0;i<max;i++) {
		if (TitleIndex[i].t!=NULL) delete TitleIndex[i].t;
	}
	free(TitleIndex);
	TitleIndex=NULL;
	max=0;
	highestId=0;
	highestOimp=0;
	Artists.Clear(true);
}

const char *CTitleStore::GetChunkName()
{
	return "TITL";
}

int CTitleStore::Increase(ppluint32 maxid)
{
	ppluint32 h=maxid+1000;
	void *t=calloc(sizeof(TITLE),h);
	if (!t) return 0;
	if (TitleIndex) {
		memcpy(t,TitleIndex,max*sizeof(TITLE));
		free(TitleIndex);
	}
	TitleIndex=(TITLE*)t;
	max=h;
	return 1;
}

int CTitleStore::Save(DataTitle *t)
{
	if (!Storage) {
		return 0;
	}
	if (!t) {
		ppl6::SetError(194,"int CTitelStore::Save(==> CTitle *t <==)");
		return 0;
	}
	if (!Storage->isDatabaseLoading()) {
		ppl6::CBinary *bin=t->Export();
		if (!bin) return 0;
		if (!Storage->Save(this,t,bin)) {
			ppl6::PushError();
			delete bin;
			ppl6::PopError();
			return 0;
		}
		delete bin;
	}
	CStringCounterItem *found, *item;
	if (t->Artist!=NULL) {
		item=new CStringCounterItem;
		item->Name=t->Artist;
		item->Count=1;
		found=(CStringCounterItem*)Artists.Find(item);
		if (found) {
			found->Count++;
			delete item;
		}else Artists.Add(item);
	}


	/*
	CArtist *a=new CArtist;
	a->Name=t->Artist;
	if (!Artists.Add(a)) delete a;
	*/

	return 1;
}

/*!\brief Kopie eines Titels speichern
 *
 * \desc
 * Diese Funktion geht davon aus, dass \p title entweder einen neuen Titel ohne ID enthält
 * oder einen bestehenden Titel. Zeigt \p title direkt in die Datenbank (Verwendung von
 * CTitleStore::Get) werden keine Daten kopiert, sondern nur dessen Daten in die Datenbank
 * geschrieben. Ist dies nicht der Fall, wird der bisherige Eintrag in der Datenbank mit
 * den neuen Daten überschrieben und gespeichert.
 *
 */
int CTitleStore::Put(DataTitle *title)
{
	if (!title) {
		return 0;
	}
	if (!Storage) {
		return 0;
	}
	ppluint32 save_highestId=highestId;
	ppluint32 id=0;
	Mutex.Lock();
	if (title->TitleId==0) {
		// Wir haben einen neuen Titel und vergeben eine Id
		highestId++;
		id=highestId;
	} else {
		id=title->TitleId;
		if (id>highestId) highestId=id;
	}

	if (id>=max) {
		if (!Increase(id)) {
			highestId=save_highestId;
			Mutex.Unlock();
			return 0;
		}
	}
	// Gibt's den Titel schon?
	if (TitleIndex[id].t) {
		if (TitleIndex[id].t!=title) {
			// CopyFrom führt ein Clear aus, daher müssen wir die Storage Daten retten
			CStorageItem ssave;
			ssave.CopyStorageFrom(TitleIndex[id].t);
			// Nun können wir die Daten kopieren
			if (!TitleIndex[id].t->CopyFrom(title)) {
				highestId=save_highestId;
				Mutex.Unlock();
				return 0;
			}
			// StorageDaten wieder herstellen
			TitleIndex[id].t->CopyStorageFrom(&ssave);
			TitleIndex[id].t->TitleId=id;
			if (!Save(TitleIndex[id].t)) {
				highestId=save_highestId;
				Mutex.Unlock();
				return 0;
			}
			if (title->ImportData>highestOimp) highestOimp=title->ImportData;
			// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
			title->CopyStorageFrom(TitleIndex[id].t);
		} else {
			if (!Save(TitleIndex[id].t)) {
				highestId=save_highestId;
				Mutex.Unlock();
				return 0;
			}
		}
		Mutex.Unlock();
		return 1;
	}
	// Nein, neuer Titel
	TitleIndex[id].t=new DataTitle;
	if (!TitleIndex[id].t) {
		highestId=save_highestId;
		Mutex.Unlock();
		return 0;
	}
	if (!TitleIndex[id].t->CopyFrom(title)) {
		ppl6::PushError();
		delete TitleIndex[id].t;
		TitleIndex[id].t=NULL;
		highestId=save_highestId;
		Mutex.Unlock();
		ppl6::PopError();
		return 0;
	}
	TitleIndex[id].t->TitleId=id;
	if (!Save(TitleIndex[id].t)) {
		ppl6::PushError();
		delete TitleIndex[id].t;
		TitleIndex[id].t=NULL;
		highestId=save_highestId;
		Mutex.Unlock();
		ppl6::PopError();
		return 0;
	}
	if (title->ImportData>highestOimp) highestOimp=title->ImportData;
	title->CopyStorageFrom(TitleIndex[id].t);
	title->TitleId=id;
	Mutex.Unlock();
	return 1;
}

int CTitleStore::Delete(DataTitle *entry)
{
	if (!entry) {
		ppl6::SetError(194,"int CTitleStore::Delete(==> DataTitle *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014,"CTitleStore");
		return 0;
	}
	// Gibt's den Track überhaupt?
	ppluint32 id=entry->TitleId;
	DataTitle *t;
	t=Get(id);
	if (!t) return 0;
	Mutex.Lock();
	Storage->Delete(this,t);
	TitleIndex[id].t=NULL;
	delete t;
	if (id==highestId) highestId--;	// ID kann wiederverwendet werden
	return 1;
}


ppluint32 CTitleStore::GetHighestImportDataId()
{
	return highestOimp;
}

ppluint32 CTitleStore::MaxId()
{
	return highestId;
}

DataTitle *CTitleStore::Get(ppluint32 id)
{
	if (id>highestId) {
		ppl6::SetError(20010,"%u > %u",id,highestId);
		return NULL;
	}
	if (TitleIndex==NULL || TitleIndex[id].t==NULL) {
		ppl6::SetError(20010,"%u",id);
		return NULL;
	}
	return TitleIndex[id].t;
}

int CTitleStore::GetCopy(ppluint32 id, DataTitle *t)
{
	DataTitle *title=Get(id);
	if (!title) return 0;
	t->CopyFrom(title);
	return 1;
}

int CTitleStore::LoadChunk(CWMFileChunk *chunk)
{
	DataTitle data;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk->GetChunkData(),chunk->GetChunkDataSize())) return 0;
	if (!data.Import(&bin,chunk->GetFormatVersion())) return 0;
	data.CopyStorageFrom(chunk);
	//if (data.Track==0) return 1; 	// Workaround für einen vorherigen Bug
	return Put(&data);
}



class Artists : public ppl6::CTreeItem
{
	private:

	public:
		Artists();
		virtual ~Artists();
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);

};
