using namespace ppl6;


#pragma pack(1)		// Der Compiler soll die Variablen in den folgenden
					// Structures auf Bytes alignen

typedef struct {
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
} TITELDATEI20;


typedef struct {
	ppluint32	crc;		// Checksumme über den restlichen Datensatz
	ppluint32	Id;
	ppluint32	Status;
	ppluint32	References;
	char		Interpret[81];
	char		Titel[81];
	ppluint32 Version;
	ppluint32	Musikart;
	float		Laenge;
	float		Bpm;
	ppluint32	Quelle;
	ppluint32	Aufnahmedatum;
	ppluint16	Erscheinungsjahr;
	ppluint8	Bewertung;
	ppluint8	Qualitaet;
	ppluint32	Bemerkung;
	ppluint32	Aufnahmegeraet;
	ppluint32	Index;
	ppluint8	TraegerArt;
	ppluint8	Seite;
	ppluint16	Track;
	ppluint64	Date_Created;		// Unix epoch
	ppluint64	Date_Changed;		// Unix epoch
} TITELDATEI21;


#pragma pack()		// Byte aligning wieder abschalten