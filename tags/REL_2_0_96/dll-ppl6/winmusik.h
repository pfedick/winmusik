using namespace ppl6;
/*
Type Titeldatei
  InfoByte As Integer
  interpret As String * 80
  titel As String * 80
  version As Long
  musikart As Long
  Laenge As Single
  bpm As Integer
  Quelle As Long
  Erscheinungsjahr As Integer
  Aufnahmedatum As Long
  Bewertung As String * 2
  Qualitaet As String * 2
  Bemerkung As Long
  Aufnahmegeraet As Long
  Soundfile As Long
  traeger As Byte
  Index As Long
  Seite As Byte
  track As Integer
  Nummer As Long
End Type
*/

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
} TITELDATEI;

typedef struct {
  ppldw	InfoByte;
  char	interpret[80];
  char	titel[80];
  ppldw	fill1;
  ppldd version;
  ppldd	musikart;
  float	Laenge;
  ppldw	bpm;
  ppldw	fill2;
  ppldd	Quelle;
  ppldw	Erscheinungsjahr;
  ppldw	fill3;
  ppldd	Aufnahmedatum;
  char	Bewertung[2];
  char	Qualitaet[2];
  ppldd	Bemerkung;
  ppldd	Aufnahmegeraet;
  ppldd	Soundfile;
  ppldb	traeger;
  char	fill4[3];
  ppldd	Index;
  ppldb	Seite;
  char	fill5[1];
  ppldw	track;
  //ppldw	fill6;
  ppldd	Nummer;
} TITELDATEIALIGN;

typedef struct {
	ppldd	ID;
	ppldd	Status;
	char	Interpret[80];
	char	Titel[80];
	ppldd	Version;
	ppldd	Musikart;
	float	Laenge;
	ppldd	Quelle;
	ppldd	Aufnahmedatum;
	ppldd	Bemerkung;
	ppldd	Aufnahmegeraet;
	ppldd	Soundfile;
	ppldd	Index;
	ppldw	BPM;
	ppldw	Erscheinungsjahr;
	ppldw	Track;
	ppldb	Traeger;
	ppldb	Seite;
	char	Bewertung[2];
	char	Qualitaet[2];
} TITELDATEI21;

typedef struct {
	char	Filename[256];
} ORIGINALFILENAME;

typedef struct {
	ppldd	status;
	ppldd	framesize;
	ppldd	filelength;
	ppldd	length;
	ppldd	mslength;
	ppldd	bitrate;
	ppldd	frequency;
	ppldd	datastart;
	ppldd	dataend;
	ppldd	datasize;
	char	version;
	char	layer;
	char	protection;
	char	bitrateindex;
	char	frequencyindex;
	char	padding;
	char	privat;
	char	channelmode;
	char	modeextension;
	char	copyright;
	char	original;
	char	emphasis;
} MP3HEADER;

/*	status:	Bit 0:	MP3-File erkannt
			Bit 1:  ID3v1-Tag vorhanden
			Bit 2:  ID3v2-Tag vorhanden
			Bit 3:  Titel aus Filename
*/

typedef struct {
  char	tag[3];
  char	SongName[30];
  char	Artist[30];
  char	Album[30];
  char	Year[4];
  char	Comment[29];
  char	track;
  char	Genre;
} ID3TAG;

typedef struct {
  char	SongName[256];
  char	Artist[256];
  char	Album[256];
  char	Year[4];
  char	Comment[256];
  char	Genre[256];
  char	Version[256];
} ID3v2TAG;


#pragma pack()		// Byte aligning wieder abschalten

typedef struct {
	ppldd size;					// Laenge des Datensatzes in Bytes
	ppldw	InfoByte;
	char	*interpret;			// Ptr auf Interpret
	char	*titel;				// Ptr auf Titel
	ppldd	version;
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
} TITEL_CACHE;


typedef struct tagTitelIndex {
	TITEL_CACHE *ti;
	ppldd		size;
} TITELINDEX;

typedef struct tagMemBlocks {
	ppldd	size;
	ppldd	free;
	ppldd	allocated;
	char	*start;
	char	*freestart;
	struct tagMemBlocks *next, *previous;
} MEMBLOCK;


class CTitelCache {
	private:
		ppldd			startsize;
		CpplMemSpace	MemSpace;
		MEMBLOCK		*first, *last;

	public:
		CTitelCache(ppldd startsize=0);
		~CTitelCache();
		void SetStartBufferSize(ppldd size);
		void Clear();
		void *Malloc(ppldd size);
		
};

class CTitel {
	private:
		char			*nameTiteldatei;
		CpplFile		*tf;
		CTitelCache		cache;
		ppldd			cachefragments;
		ppldd			lostcachesize;
		ppldd			HighestID;
		ppldd			MaxTitelIndex;
		TITELINDEX		*tii;
		ppluint64		lastaccess;
		CpplMutex		iosection;
		ppldd			mon_thread_ID;
		HANDLE			mon_thread;


		void	CopyFromCache(TITELDATEIALIGN *dst, TITEL_CACHE *src);
		void	CopyToCache(TITEL_CACHE *dst, TITELDATEI *src);
		ppldd	CalcLength(TITELDATEI *Ti);
		ppldd	StrLenRight(char *str, ppldd max);
		void	IncreaseCache();
		void	Tia2Ti(TITELDATEI *dst, TITELDATEIALIGN *src);
		int		ReadFromDisk(ppldd id, TITELDATEI *Ti);
		int		WriteToDisk(ppldd id, TITELDATEI *Ti);

	public:
		ppldd	err;
		bool			mon_thread_is_running;
		bool			mon_thread_stop;

		CTitel (char * filename);
		~CTitel();
		int		Get (ppldd id, TITELDATEIALIGN *Ti);
		int		Put (ppldd id, TITELDATEIALIGN *Ti);
		void	Update(ppldd id);
		void	ClearCache();
		void	CloseTitelDatei();

};

class CTagSaver : public ppl6::CpplThread
{
	private:
		ppl6::CpplMutex Mutex;
		ppl6::CpplAssocArray Todo;
		ppl6::CpplLog Log;

	public:
		CTagSaver();
		~CTagSaver();

		virtual void ThreadMain(void *param);

		void Add(char *filename, ppl6::CpplAssocArray *Tags, bool cleartag=false);

		int UpdateNow(char *filename, ppl6::CpplAssocArray *Tags, int cleartag=0);

};


// Funktionen

int GetMP3Info_internal (CpplFile *file, MP3HEADER *mp3, ID3TAG *id3, ID3v2TAG *v2);
int ChopID3Tags_internal (char *filename);

ppldds FindMp3Start(CpplFile *ff);
ppldd FindMp3End(CpplFile *ff);