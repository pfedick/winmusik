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
#include "wm_musicalkeys.h"


TrackInfo::TrackInfo()
{

}

void TrackInfo::clear()
{
	Ti.Clear();
	Version.clear();
	Genre.clear();
	Label.clear();
	RecordingSource.clear();
	RecordingDevice.clear();
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
 * - Version 5: EnergyLevel hinzugekommen
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
						Bit 4: Music-Key ist verifiziert                           (16)
						\endverbatim
						 *
						 */

						 /*!\var DataTitle::Key
						  * \brief Tonart
						  *
						  * Gibt die initiale Tonart des Songs im Quintenzirkel an.
						  *
						  * \verbatim
						 ID: Note OpenKey Name
						 0:  -    -       unbekannt
						 1:  g#m  6m      A-Flat Minor
						 2:  B    6d      B Major
						 3:  d#m  7m      E-Flat Minor
						 4:  F#   7d      F-Sharp Major
						 5:  a#m  8m      B-Flat Minor
						 6:  C#   8d      D-Flat Major
						 7:  fm   9m      F Minor
						 8:  G#   9d      A-Flat Major
						 9:  cm   10m     C Minor
						 10: D#   10d     E-Flat Major
						 11: gm   11m     G Minor
						 12: A#   11d     B-Flat Major
						 13: dm   12m     D Minor
						 14: F    12d     F Major
						 15: am   1m      A Minor
						 16: C    1d      C Major
						 17: em   2m      E Minor
						 18: G    2d      G Major
						 19: bm   3m      B Minor
						 20: D    3d      D Major
						 21: f#m  4m      F-Sharp Minor
						 22: A    4d      A Major
						 23: c#m  5m      D-Flat Minor
						 24: E    5d      E Major
						 25: none
						 \endverbatim
						  *
						  */

						  /*!\var DataTitle::EnergyLevel
						   * \brief Tonart
						   *
						   * Wert zwischen 1 und 10, der die Tanzbarkeit des Titels angibt. Niedrige Werte
						   * stehen für eher ruhigere Titel, hohe Werte für Floorfiller.
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
	Clear();
	formatversion=5;
}

DataTitle::DataTitle(const DataTitle& other)
	: CStorageItem(other)
{
	formatversion=5;
	CopyFrom(other);
}

DataTitle::~DataTitle()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Gibt sämtlichen durch diese Klasse allokierten Speicher
 * wieder frei.
 */
{
}




DataTitle& DataTitle::operator=(const DataTitle& other)
{
	CopyFrom(other);
	return *this;
}

bool DataTitle::operator==(const DataTitle& other) const
{
	if (Artist != other.Artist) return false;
	if (Title != other.Title) return false;
	if (Remarks != other.Remarks) return false;
	if (Album != other.Album) return false;
	if (Tags != other.Tags) return false;
	if (CoverPreview != other.CoverPreview) { printf("changes: CoverPreview \n"); return false; }
	if (TitleId != other.TitleId) return false;
	if (DeviceId != other.DeviceId) return false;
	if (Length != other.Length) return false;
	if (VersionId != other.VersionId) return false;
	if (LabelId != other.LabelId) return false;
	if (BPM != other.BPM) return false;
	if (RecordDate != other.RecordDate) return false;
	if (ReleaseDate != other.ReleaseDate) return false;
	if (ImportData != other.ImportData) return false;
	if (Size != other.Size) return false;
	if (RecordSourceId != other.RecordSourceId) return false;
	if (Track != other.Track) return false;
	if (Bitrate != other.Bitrate) return false;
	if (GenreId != other.GenreId) return false;
	if (RecordDeviceId != other.RecordDeviceId) return false;
	if (DeviceType != other.DeviceType) return false;
	if (Page != other.Page) return false;
	if (Channels != other.Channels) return false;
	if (Quality != other.Quality) return false;
	if (Rating != other.Rating) return false;
	if (Flags != other.Flags) return false;
	if (Key != other.Key) return false;
	if (EnergyLevel != other.EnergyLevel) return false;
	return true;
}

bool DataTitle::operator!=(const DataTitle& other) const
{
	if (*this == other) return false;
	return true;
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
	Artist.clear();
	Title.clear();
	Remarks.clear();
	Album.clear();
	Tags.clear();
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
	CoverPreview.clear();
	CStorageItem::Clear();
	formatversion=5;
	Key=0;
	EnergyLevel=0;
}


void DataTitle::CopyFrom(const DataTitle& other)
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
	Clear();
	CopyDataFrom(other);
	CopyStorageFrom(other);
}

void DataTitle::CopyDataFrom(const DataTitle& other)
{
	Artist=other.Artist;
	Title=other.Title;
	Remarks=other.Remarks;
	Album=other.Album;
	Tags=other.Tags;
	TitleId=other.TitleId;
	DeviceId=other.DeviceId;
	Length=other.Length;
	VersionId=other.VersionId;
	GenreId=other.GenreId;
	RecordSourceId=other.RecordSourceId;
	RecordDeviceId=other.RecordDeviceId;
	LabelId=other.LabelId;
	BPM=other.BPM;
	RecordDate=other.RecordDate;
	ReleaseDate=other.ReleaseDate;
	Track=other.Track;
	Bitrate=other.Bitrate;
	DeviceType=other.DeviceType;
	Page=other.Page;
	Channels=other.Channels;
	Quality=other.Quality;
	Rating=other.Rating;
	Flags=other.Flags;
	ImportData=other.ImportData;
	CoverPreview=other.CoverPreview;
	Size=other.Size;
	Key=other.Key;
	EnergyLevel=other.EnergyLevel;
}

void DataTitle::SetTitle(const char* title)
/*!\brief Songtitel setzen
 *
 * Mit dieser Funktion wird der Titel des Songs gesetzt.
 *
 * \param[in] title Pointer auf einen String mit dem Titel.
 */
{
	Title.set(title);
}

void DataTitle::SetArtist(const char* artist)
/*!\brief Interpreten setzen
 *
 * Mit dieser Funktion wird der Interpret des Songs gesetzt.
 *
 * \param[in] artist Pointer auf einen String mit dem Namen des Interpreten.
 */
{
	Artist.set(artist);
}

void DataTitle::SetRemarks(const char* remarks)
/*!\brief Bemerkung setzen
 *
 * Mit dieser Funktion kann eine Bemerkung zu dem Titel hinterlegt werden.
 *
 * \param[in] remarks Pointer auf einen String mit der Bemerkung.
  */

{
	Remarks.set(remarks);
}

void DataTitle::SetTags(const char* tags)
/*!\brief Suchtags setzen
 *
 * Mit dieser Funktion können optionale Suchtags gesetzt werden.
 *
 * \param[in] tags Pointer auf einen String mit den Suchtags
  */

{
	Tags.set(tags);
}



void DataTitle::SetAlbum(const char* album)
/*!\brief Album setzen
 *
 * Mit dieser Funktion kann der Name des Albums gesetzt werden, von dem dieser Titel stammt.
 *
 * \param[in] album Pointer auf einen String mit dem Namen des Albums.
 */
{
	Album.set(album);
}

void DataTitle::SetKey(const ppl7::String& key)
{
	Key=DataTitle::keyId(key);
}


void DataTitle::SetTitle(const ppl7::String& title)
/*!\brief Songtitel setzen
 *
 * Mit dieser Funktion wird der Titel des Songs gesetzt.
 *
 * \param[in] title Pointer auf einen String mit dem Titel.
 */
{
	Title.set(title);
}

void DataTitle::SetArtist(const ppl7::String& artist)
/*!\brief Interpreten setzen
 *
 * Mit dieser Funktion wird der Interpret des Songs gesetzt.
 *
 * \param[in] artist Pointer auf einen String mit dem Namen des Interpreten.
 */
{
	Artist.set(artist);
}

void DataTitle::SetRemarks(const ppl7::String& remarks)
/*!\brief Bemerkung setzen
 *
 * Mit dieser Funktion kann eine Bemerkung zu dem Titel hinterlegt werden.
 *
 * \param[in] remarks Pointer auf einen String mit der Bemerkung.
 */
{
	Remarks.set(remarks);
}

void DataTitle::SetTags(const ppl7::String& tags)
/*!\brief Suchtags setzen
 *
 * Mit dieser Funktion können optionale Suchtags gesetzt werden.
 *
 * \param[in] tags Pointer auf einen String mit den Suchtags
 */
{
	Tags.set(tags);
}

void DataTitle::SetAlbum(const ppl7::String& album)
/*!\brief Album-Name setzen
 *
 * Mit dieser Funktion kann der Name des Albums gesetzt werden.
 *
 * \param[in] album String mit dem Namen des Albums
 */
{
	Album.set(album);
}


ppl7::String DataTitle::getKeyName(MusicKeyType type) const
{
	return DataTitle::keyName(Key, type);
}

static inline ppl7::String keyNameSharps(int id)
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
	case 25: return "none";
	default: return "";
	}
}

static inline ppl7::String keyNameOpenKey(int id)
{
	switch (id) {
	case 1: return " 6m";	// G#m
	case 2: return " 6d";	// B
	case 3: return " 7m";	// D#m
	case 4: return " 7d";	// F#
	case 5: return " 8m";	// A#m
	case 6: return " 8d";	// C#
	case 7: return " 9m";	// Fm
	case 8: return " 9d";	// G#
	case 9: return "10m";	// Cm
	case 10: return "10d";	// D#
	case 11: return "11m";	// Gm
	case 12: return "11d";	// A#
	case 13: return "12m";	// Dm
	case 14: return "12d";	// F
	case 15: return " 1m";	// Am
	case 16: return " 1d";	// C
	case 17: return " 2m";	// Em
	case 18: return " 2d";	// G
	case 19: return " 3m";	// Bm
	case 20: return " 3d";	// D
	case 21: return " 4m";	// F#m
	case 22: return " 4d";	// A
	case 23: return " 5m";	// C#m
	case 24: return " 5d";	// E
	case 25: return "none";	// E
	default: return "";		// ""
	}
}

ppl7::String DataTitle::keyName(ppluint8 id, MusicKeyType type)
{
	if (type == musicKeyTypeMusicalSharps) return keyNameSharps(id);
	else if (type == musicKeyTypeOpenKey) return keyNameOpenKey(id);
	else if (type == musicKeyTypeCustom) return wm_main->conf.customMusicKey[id];
	return "";

}


ppluint8 DataTitle::keyId(const ppl7::String& name)
{
	ppl7::String k=ppl7::LowerCase(ppl7::Trim(name));
	k.replace("♯", "#");
	k.replace("♭", "b");
	if (k.isEmpty()) return 0;
	if (wm_main->conf.musicKeyDisplay == musicKeyTypeCustom) {
		for (ppluint8 i=1;i < 26;i++) {
			ppl7::String tmp=ppl7::LowerCase(ppl7::Trim(wm_main->conf.customMusicKey[i]));
			if (tmp == k) return i;
		}
	}


	if (k == "g#m" || k == "g#min" || k == "abm" || k == "1a" || k == "6m") return 1;
	else if (k == "b" || k == "bmaj" || k == "1b" || k == "6d") return 2;
	else if (k == "d#m" || k == "d#min" || k == "ebm" || k == "2a" || k == "7m") return 3;
	else if (k == "f#" || k == "f#maj" || k == "gb" || k == "2b" || k == "7d") return 4;
	else if (k == "a#m" || k == "a#min" || k == "bbm" || k == "3a" || k == "8m") return 5;
	else if (k == "c#" || k == "c#maj" || k == "db" || k == "3b" || k == "8d") return 6;
	else if (k == "fm" || k == "fmin" || k == "4a" || k == "6m" || k == "9m") return 7;
	else if (k == "g#" || k == "g#maj" || k == "ab" || k == "4b" || k == "9d") return 8;
	else if (k == "cm" || k == "cmin" || k == "5a" || k == "10m") return 9;
	else if (k == "d#" || k == "d#maj" || k == "eb" || k == "5b" || k == "10d") return 10;
	else if (k == "gm" || k == "gmin" || k == "6a" || k == "11m") return 11;
	else if (k == "a#" || k == "a#maj" || k == "bb" || k == "6b" || k == "11d") return 12;
	else if (k == "dm" || k == "dmin" || k == "7a" || k == "12m") return 13;
	else if (k == "f" || k == "fmaj" || k == "7b" || k == "12d") return 14;
	else if (k == "am" || k == "amin" || k == "8a" || k == "1m") return 15;
	else if (k == "c" || k == "cmaj" || k == "8b" || k == "1d") return 16;
	else if (k == "em" || k == "emin" || k == "9a" || k == "2m") return 17;
	else if (k == "g" || k == "gmaj" || k == "2b" || k == "2d") return 18;
	else if (k == "bm" || k == "bmin" || k == "10a" || k == "3m") return 19;
	else if (k == "d" || k == "dmaj" || k == "10b" || k == "3d") return 20;
	else if (k == "f#m" || k == "f#min" || k == "gbm" || k == "11a" || k == "4m") return 21;
	else if (k == "a" || k == "amaj" || k == "11b" || k == "4d") return 22;
	else if (k == "c#m" || k == "c#min" || k == "dbm" || k == "12a" || k == "5m") return 23;
	else if (k == "e" || k == "emaj" || k == "12b" || k == "5d") return 24;

	return 0;
}


void DataTitle::Export(ppl7::ByteArray& bin)
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
	int size=70;
	int p=0;
	int lenArtist=0;
	int lenTitle=0;
	int lenRemarks=0;
	int lenAlbum=0;
	int lenTags=0;
	formatversion=5;

	lenArtist=static_cast<int>(Artist.size());
	lenTitle=static_cast<int>(Title.size());
	lenRemarks=static_cast<int>(Remarks.size());
	lenTags=static_cast<int>(Tags.size());
	lenAlbum=static_cast<int>(Album.size());
	size=size + lenArtist + lenTitle + lenRemarks + lenAlbum + lenTags + static_cast<int>(CoverPreview.size());
	//size=size+lenArtist+lenTitle+lenRemarks+lenAlbum;
	char* a=(char*)bin.malloc(size);
	ppl7::Poke32(a + 0, TitleId);
	ppl7::Poke8(a + 4, Flags);
	ppl7::Poke32(a + 5, DeviceId);
	ppl7::Poke32(a + 9, Length);
	ppl7::Poke32(a + 13, VersionId);
	ppl7::Poke32(a + 17, LabelId);
	ppl7::Poke32(a + 21, BPM);
	ppl7::Poke32(a + 25, RecordDate);
	ppl7::Poke32(a + 29, ReleaseDate);
	ppl7::Poke16(a + 33, RecordSourceId);
	ppl7::Poke16(a + 35, Track);
	ppl7::Poke16(a + 37, Bitrate);
	ppl7::Poke16(a + 39, GenreId);
	ppl7::Poke16(a + 41, RecordDeviceId);
	ppl7::Poke8(a + 43, DeviceType);
	ppl7::Poke8(a + 44, Page);
	ppl7::Poke8(a + 45, Channels);
	ppl7::Poke8(a + 46, Quality);
	ppl7::Poke8(a + 47, Rating);
	ppl7::Poke32(a + 48, ImportData);
	ppl7::Poke32(a + 52, Size);
	ppl7::Poke8(a + 56, Key);
	ppl7::Poke8(a + 57, EnergyLevel);
	p=58;
	ppl7::Poke16(a + p, lenArtist & 0xffff);
	if (lenArtist) strncpy(a + p + 2, (const char*)Artist, lenArtist);
	p+=lenArtist + 2;
	ppl7::Poke16(a + p, lenTitle);
	if (lenTitle) strncpy(a + p + 2, (const char*)Title, lenTitle);
	p+=lenTitle + 2;
	ppl7::Poke16(a + p, lenRemarks);
	if (lenRemarks) strncpy(a + p + 2, (const char*)Remarks, lenRemarks);
	p+=lenRemarks + 2;
	ppl7::Poke16(a + p, lenAlbum);
	if (lenAlbum) strncpy(a + p + 2, (const char*)Album, lenAlbum);
	p+=lenAlbum + 2;
	ppl7::Poke16(a + p, lenTags);
	if (lenTags) strncpy(a + p + 2, (const char*)Tags, lenTags);
	p+=lenTags + 2;

	ppl7::Poke16(a + p, CoverPreview.size());
	if (CoverPreview.size()) memcpy(a + p + 2, CoverPreview.ptr(), CoverPreview.size());
	p+=CoverPreview.size() + 2;
}

void DataTitle::Import(const ppl7::ByteArrayPtr& bin, int version)
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
	if (version < 1 || version>5) {
		throw UnknownRecordVersion("%d", version);
	}
	size_t size=bin.size();
	const char* a=(const char*)bin.ptr();
	// Die Größe muss mindestens 60 Byte betragen
	if (size < 60 || a == NULL) {
		throw InvalidRecord();
	}
	Clear();
	int p=0;
	int len;
	TitleId=ppl7::Peek32(a + 0);
	Flags=ppl7::Peek8(a + 4);
	DeviceId=ppl7::Peek32(a + 5);
	Length=ppl7::Peek32(a + 9);
	VersionId=ppl7::Peek32(a + 13);
	LabelId=ppl7::Peek32(a + 17);
	BPM=ppl7::Peek32(a + 21);
	RecordDate=ppl7::Peek32(a + 25);
	ReleaseDate=ppl7::Peek32(a + 29);
	if (ReleaseDate < 10000000) {			// Korrektur fehlerhafter Timestamps
		if (ReleaseDate > 1900 && ReleaseDate < 2015) {
			ReleaseDate=ReleaseDate * 10000;
		} else {
			ReleaseDate=RecordDate;
			if (ReleaseDate > 1900 && ReleaseDate < 2015) {
				ReleaseDate=ReleaseDate * 10000;
			}
		}
	}
	if (RecordDate < 10000000) {			// Korrektur fehlerhafter Timestamps
		if (RecordDate > 1900 && RecordDate < 2015) {
			RecordDate=RecordDate * 10000 + 0101;
		} else {
			RecordDate=ReleaseDate;
		}
	}

	RecordSourceId=ppl7::Peek16(a + 33);
	Track=ppl7::Peek16(a + 35);
	Bitrate=ppl7::Peek16(a + 37);
	GenreId=ppl7::Peek16(a + 39);
	RecordDeviceId=ppl7::Peek16(a + 41);
	DeviceType=ppl7::Peek8(a + 43);
	Page=ppl7::Peek8(a + 44);
	Channels=ppl7::Peek8(a + 45);
	Quality=ppl7::Peek8(a + 46);
	Rating=ppl7::Peek8(a + 47);
	ImportData=ppl7::Peek32(a + 48);
	if (version == 1) {
		p=52;
		Size=0;
	} else if (version >= 2) {
		Size=ppl7::Peek32(a + 52);
		p=56;
	}
	if (version >= 4) {
		Key=ppl7::Peek8(a + 56);
		p=57;
	}
	if (version >= 5) {
		EnergyLevel=ppl7::Peek8(a + 57);
		p=58;
	}
	len=ppl7::Peek16(a + p);
	if (len) Artist.set(a + p + 2, len);
	p+=len + 2;
	len=ppl7::Peek16(a + p);
	if (len) Title.set(a + p + 2, len);
	p+=len + 2;
	len=ppl7::Peek16(a + p);
	if (len) Remarks.set(a + p + 2, len);
	p+=len + 2;
	len=ppl7::Peek16(a + p);
	if (len) Album.set(a + p + 2, len);
	p+=len + 2;
	if (version >= 3) {
		len=ppl7::Peek16(a + p);
		if (len) Tags.set(a + p + 2, len);
		p+=len + 2;
	}
	if (version >= 2) {
		len=ppl7::Peek16(a + p);
		if (len) CoverPreview.copy(a + p + 2, len);
	}
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
}

CTitleStore::~CTitleStore()
{
	Clear();
}

void CTitleStore::Clear()
{
	for (ppluint32 i=0;i < max;i++) {
		if (TitleIndex[i] != NULL) delete TitleIndex[i];
	}
	free(TitleIndex);
	TitleIndex=NULL;
	max=0;
	highestId=0;
	Artists.clear();
}

const char* CTitleStore::GetChunkName()
{
	return "TITL";
}

void CTitleStore::Increase(ppluint32 maxid)
{
	ppluint32 h=maxid + 10000;
	DataTitle** t=(DataTitle**)calloc(sizeof(DataTitle*), h);
	if (!t) throw ppl7::OutOfMemoryException();
	if (TitleIndex) {
		memcpy(t, TitleIndex, max * sizeof(DataTitle*));
		free(TitleIndex);
	}
	TitleIndex=t;
	max=h;
}

void CTitleStore::SaveToStorage(DataTitle& t)
{
	CStorage& store=getStorage();
	ppl7::ByteArray bin;
	t.Export(bin);
	store.Save(this, &t, bin);
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
uint32_t CTitleStore::Put(const DataTitle& title)
{
	Mutex.lock();
	try {
		DataTitle* new_title=SaveToMemory(title);
		SaveToStorage(*new_title);
		Mutex.unlock();
		return new_title->TitleId;
	} catch (...) {
		Mutex.unlock();
		throw;
	}
}


DataTitle* CTitleStore::SaveToMemory(const DataTitle& title)
{
	uint32_t save_highestId=highestId;
	uint32_t id=0;

	if (title.TitleId == 0) {
		// Wir haben einen neuen Titel und vergeben eine Id
		id=highestId + 1;
	} else {
		id=title.TitleId;
	}
	if (id >= max) {
		Increase(id);
	}
	if (id > highestId) highestId=id;

	// Gibt's den Titel schon?
	if (TitleIndex[id]) {
		TitleIndex[id]->CopyDataFrom(title);
		return TitleIndex[id];
	} else {
		TitleIndex[id]=new DataTitle;
		if (!TitleIndex[id]) {
			throw ppl7::OutOfMemoryException();
		}
		TitleIndex[id]->CopyDataFrom(title);
	}
	return TitleIndex[id];
}

void CTitleStore::Delete(uint32_t id)
{
	CStorage& store=getStorage();
	// Gibt's den Track überhaupt?
	Mutex.lock();
	try {
		if (id <= highestId && TitleIndex[id] != NULL) {
			store.Delete(this, (CStorageItem*)TitleIndex[id]);
			delete TitleIndex[id];
			TitleIndex[id]=NULL;
			if (id == highestId) highestId--;	// ID kann wiederverwendet werden
		}
	} catch (...) {
		Mutex.unlock();
		throw;
	}
	Mutex.unlock();
}


uint32_t CTitleStore::MaxId() const
{
	return highestId;
}

const DataTitle& CTitleStore::Get(uint32_t id) const
{
	if (id > highestId || TitleIndex == NULL || TitleIndex[id] == NULL) {
		throw RecordDoesNotExist();
	}
	return *TitleIndex[id];
}

const DataTitle* CTitleStore::GetPtr(uint32_t id) const
{
	if (id > highestId || TitleIndex == NULL || TitleIndex[id] == NULL) {
		throw RecordDoesNotExist();
	}
	return TitleIndex[id];
}

bool CTitleStore::Exists(uint32_t id) const
{
	if (id > highestId || TitleIndex == NULL || TitleIndex[id] == NULL) {
		return false;
	}
	return true;
}


void CTitleStore::LoadChunk(const CWMFileChunk& chunk)
{
	DataTitle data;
	ppl7::ByteArrayPtr bin(chunk.GetChunkData(), chunk.GetChunkDataSize());
	data.Import(bin, chunk.GetFormatVersion());
	data.CopyStorageFrom(chunk);
	SaveToMemory(data);
}
