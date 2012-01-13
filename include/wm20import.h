/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:29 $
 * $Id: wm20import.h,v 1.1 2010/05/16 12:23:29 pafe Exp $
 *
 *
 * Copyright (c) 2009 Patrick Fedick
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


class ImportProgress
{

	public:
		ImportProgress();
		virtual ~ImportProgress();
		int progress;
		int current;
		bool import_shortcuts;
		bool import_titeldatei;
		bool import_traegertitel;
		bool import_versionen;
		bool import_musikarten;
		bool import_quellen;
		bool import_kaufquellen;
		bool import_label;
		bool import_geraete;
		bool import_kassetten;
		bool import_cds;
		bool import_platten;
		bool import_mp3;
		bool import_daten;
		bool import_video;
		bool import_band;
		bool import_sonstige;
		virtual void Update();

};


namespace wm20 {

#pragma pack(1)		// Der Compiler soll die Variablen in den folgenden
					// Structures auf Bytes alignen

typedef struct TITELDATEI{
  ppldw	InfoByte;
  char	interpret[80];
  char	titel[80];
  ppldd version;
  ppldd	musikart;
  float	Laenge;
  ppldw	bpm;
  ppldd	Quelle;
  ppldw	Erscheinungsjahr;
  ppldd	Aufnahmedatum;
  char	Bewertung[2];
  char	Qualitaet[2];
  ppldd	Bemerkung;
  ppldd	Aufnahmegeraet;
  ppldd	Soundfile;
  ppldb	traeger;
  ppldd	Index;
  ppldb	Seite;
  ppldw	track;
  ppldd	Nummer;
} TITELDATEI;
/*
' Infobyte:
'   Bit 0:  Gültiger Titel                                  1
'   Bit 1:  ?                                               2
'   Bit 2:  Stereo                                          4
'   Bit 3:  Dolby                                           8
'   Bit 4:  Surround                                        16
'   Bit 5:  vollständig                                     32
'   Bit 6:  echter Titel                                    64
'   Bit 7:  Digital 5.1                                     128
'   Bit 8:  Referenziert durch Tonträger                    256 (neu 14.11.2001)
*/

typedef struct TraegerVerzeichnis {
	ppluint16	InfoByte;			// InfoByte As Integer
	ppluint8	Seiten;				// Seiten As Byte
	float		Laenge;				// Laenge As Single
	float		Rest;				// rest As Single
	ppluint32	Bezeichnung;		// Bezeichnung As Long
	ppluint32	Label;				// Label As Long
	ppluint32	Kaufquelle;			// Kaufquelle As Long
	ppluint32	Kaufdatum;			// Kaufdatum As Long
	float		Preis;				// Preis As Single
	ppluint32	Erstelldatum;		// Erstelldatum As Long
	ppluint32	Aufnahmestart;		// Aufnahmestart As Long
	ppluint32	Aufnahmeende;		// Aufnahmeende As Long
	ppluint16	Tracks;				// Tracks As Integer
	ppluint16	Singles;			// Singles As Integer
	ppluint16	Mixe;				// Mixe As Integer
	ppluint32	Cover;				// cover As Long
	ppluint32	Coverbild;			// coverbild As Long
	ppluint32	ErsterDatensatz;	// ErsterDatensatz As Long
} TraegerVerzeichnis;

typedef struct TraegerDatensatz {
	ppluint32	LetzterDatensatz;	// LetzterDatensatz As Long
	ppluint32	NaechsterDatensatz;	// NaechsterDatensatz As Long
	ppluint32	TraegerIndex;		// traegerindex As Long
	ppluint32	Titelnummer;		// Titelnummer As Long
	ppluint8	Seite;				// Seite As Byte
	ppluint16	Track;				// track As Integer
} TraegerDatensatz;

typedef struct TraegerTitel {
	char		Titel1[80];			// Titel1 As String * 80
	char		Titel2[80];			// Titel2 As String * 80
} TraegerTitel;


typedef struct Abkuerzungen {
	char		abk[10];
	char		interpret[80];
} Abkuerzungen;

#pragma pack()		// Byte aligning wieder abschalten


class Cwm20TraegerTitel
{
	private:
		ppl6::CFile ff;
		int num;
		TraegerTitel *titel;
	public:
		Cwm20TraegerTitel();
		int Load(const char *path);
		int Get(int index, ppl6::CString *Titel1, ppl6::CString *Titel2);
};

class Cwm20TraegerVerzeichnis
{
	private:
		ppl6::CFile ff;
		int num;
		TraegerVerzeichnis *vz;

	public:
		Cwm20TraegerVerzeichnis();
		int Load(const char *path, const char *file);
		TraegerVerzeichnis *Get(int index);
};

class Cwm20TraegerDaten
{
	private:
		ppl6::CFile ff;
		int num;
		TraegerDatensatz *td;

	public:
		Cwm20TraegerDaten();
		int Load(const char *path, const char *file);
		TraegerDatensatz *Get(int index);
};




class Cwm20Import
{
	public:
		/*
		Cwm20Titel Ti;
		Cwm20TraegerTitel TraegerTitel;
		Cwm20TraegerVerzeichnis	TrCD;
		Cwm20TraegerVerzeichnis	TrKassette;
		Cwm20TraegerVerzeichnis	TrMP3;
		Cwm20TraegerVerzeichnis	TrPlatte;
		Cwm20TraegerVerzeichnis	TrSonstige;
		Cwm20TraegerVerzeichnis	TrDaten;
		Cwm20TraegerVerzeichnis	TrBand;
		Cwm20TraegerVerzeichnis	TrVideo;

		Cwm20TraegerDaten	TrDataCD;
		Cwm20TraegerDaten	TrDataKassette;
		Cwm20TraegerDaten	TrDataMP3;
		Cwm20TraegerDaten	TrDataPlatte;
		Cwm20TraegerDaten	TrDataSonstige;
		Cwm20TraegerDaten	TrDataDaten;
		Cwm20TraegerDaten	TrDataBand;
		Cwm20TraegerDaten	TrDataVideo;
		*/

		const char *charset;
		ImportProgress *progress;

		CStorage	*Storage;

	public:
		Cwm20Import();
		~Cwm20Import();
		void SetCharset(const char *charset);

		int Load(const char *path, CStorage *storage, ImportProgress *progress);

		int CalcOccurances();

		int ImportShortcuts(const char *path);
		int ImportTable(const char *path, const char *file, int width, const char *cunktype);
		int ImportTitle(const char *path);
		int ImportDevices(const char *path, const char *fileprefix, ppluint8 DeviceType);
		int ImportTracks(const char *path, const char *fileprefix, DataDevice *device, ppluint32 first);

};

}	// EOF namespace wm40
