/*
 * WinMusik Strukturen
 */

typedef struct {
  char	InfoByte[2];		// 0
  char	interpret[80];		// 2
  char	titel[80];			// 82
  dd    version;			// 162
  dd	musikart;			// 166
  float	Laenge;				// 170
  dw	bpm;				// 174
  dd	Quelle;				// 176
  dw	Erscheinungsjahr;	// 180
  dd	Aufnahmedatum;		// 182
  char	Bewertung[2];		// 186
  char	Qualitaet[2];		// 188
  dd	Bemerkung;			// 190
  dd	Aufnahmegeraet;		// 194
  dd	Soundfile;			// 198
  db	traeger;			// 202
  dd	Index;				// 203
  db	Seite;				// 207
  dw	track;				// 208
  dd	Nummer;				// 210
} TITELDATEI;			

typedef struct {
	dw		Tracks;
	dw		TrackLength;
	dd		TraegerTitel;
	dw		TraegerTyp;
	dd		TragerIndex;
	dd		TabStart;
} WMTHEADER;
  
typedef struct {
	TITELDATEI	ti;
	char		version[80];	// 214
	char		musikart[80];	// 294
} WMTENTRY;						// 374

