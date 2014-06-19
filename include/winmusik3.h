/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.21 $
 * $Date: 2011/10/30 20:14:30 $
 * $Id: winmusik3.h,v 1.21 2011/10/30 20:14:30 pafe Exp $
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

#ifndef WINMUSIK3_H_
#define WINMUSIK3_H_
#include <QString>
#include <QSettings>
#include <QComboBox>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStatusBar>
#include <QMessageBox>
#include <QString>
#include <QLabel>
#include <QFileDialog>
#include <QTranslator>
#include <QApplication>
#include <QDate>
#include <QPixmap>
#include <map>
#include <set>

#define WITH_QT		// Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann

#include <ppl6.h>


#define WM_COPYRIGHT	"(c) Copyright by Patrick Fedick in 2014"
// Die folgenden Werte werden für die Registry bzw. Ermitteln des Application Data
// Verzeichnisses verwendet:
#define WM_ORGANISATION	"Patrick F.-Productions"
#define WM_APPNAME		"WinMusik3"
#define WM_REGISTRATION_URL	"https://www.pfp.de/winmusik/interface.html"
#define MAX_DEVICE_TYPES	10


typedef struct tagWindowListItem{
	struct tagWindowListItem *next, *previous;
	void *object;
} WINDOWLISTITEM;

class WMTreeItem : public QTreeWidgetItem
{
	public:
		ppluint32	Id;
		ppluint32	Track;
};

extern ppl6::CLog *wmlog;

#define WMLOG(facility,debuglevel) if(wmlog) wmlog->Printf(facility,debuglevel,__FILE__,__LINE__,
#define WMLOGERROR() if(wmlog) wmlog->LogError(__FILE__,__LINE__);

/*******************************************************
 * Structures                                          *
 *******************************************************/

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

class CUpdateChecker;
class CWmClient;
extern CWmClient *wm_main;

class CCallback
{
	public:
		int progress;

		virtual ~CCallback();
		virtual void Update();
};


class CWMFileChunk
{
	friend class CWMFile;
	friend class CStorage;
	private:
		char		chunkname[5];
		ppluint32	filepos;
		ppluint32	size;
		ppluint32	timestamp;
		ppluint32	version;
		ppluint32	datasize;
		ppluint8	formatversion;
		const char	*data;

	public:
		CWMFileChunk();
		~CWMFileChunk();
		void		Clear();
		ppluint32	GetChunkDataSize();
		const char	*GetChunkData();
		const char	*GetChunkName();
		int			SetChunkData(const char *chunkname, const char *data, ppluint32 size, ppluint32 oldfilepos=0, ppluint32 version=0, ppluint8 formatversion=1);
		void		SetFormatVersion(ppluint8 v);
		ppluint32	GetFilepos();
		ppluint32	GetTimestamp();
		ppluint32	GetVersion();
		ppluint8	GetFormatVersion();
		void		HexDump(ppl6::CLog *log=NULL);
		// int			SetChunkData(const char *chunkname, ppl6::CBinary *bin, CStorageItem *item);
};

class CWMFile
{
	private:
		ppl6::CFile ff;
		ppluint8	version, subversion;
		ppluint32	timestamp, lastchange;
		ppluint32	pos, first, eof;
		bool		enableCompression;

	public:
		CWMFile();
		~CWMFile();
		bool IsOpen() const;
		int Open(const char *filename);
		void Close();
		void Reset();
		int GetNextChunk(CWMFileChunk *chunk);
		int GetChunk(CWMFileChunk *chunk, ppluint32 filepos);
		int SaveChunk(CWMFileChunk *chunk);
		int DeleteChunk(CWMFileChunk *chunk);
		void ListChunks();
		void EnableCompression(bool flag);
		int IsValidChunkName(const char *name);
		ppluint32 GetFileSize();
		ppluint32 GetFilePosition();
		static int CopyDatabase(CWMFile &oldfile, CWMFile &newfile, CCallback *callback=NULL);
};


enum MusicKeyType
{
	musicKeyTypeNone=0,
	musicKeyTypeMusicalSharps,
	musicKeyTypeOpenKey,
	musicKeyTypeCustom=99
};



#include "wmstorage.h"
#include "regexpcapture.h"
#include "backgroundjobs.h"


/*******************************************************
 * Global Functions                                    *
 *******************************************************/
ppluint32 Time2Int(ppl6::CString &Time);
ppl6::CString Int2Time(ppluint32 Time);
ppl6::CString Long2Date(ppl6::CString &Format, ppluint32 Date);

void getHarmonicKeys(std::set<int> &harmonics, int key);
bool saveCover(const ppl6::CString &filename, const QPixmap &Cover);
void getIconFromCover(ppl6::CBinary &bin, const QPixmap &Cover);
void NormalizeImportString(ppl6::CString &Buffer);
void setReadableLength(QLabel *label, int length);

/*******************************************************
 * Global Variablen                                    *
 *******************************************************/






class CTrackListItem : ppl6::CTreeItem
{
	friend class CTrackList;
	private:
		int id;
		DataTrack *track;
	public:
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);
};

class CTrackList
{
	friend class CTrackStore;
	private:
		ppl6::CMutex Mutex;
		ppl6::CTree	Tracks;
		int min, max;
		CTrackStore *storage;
		int Add(int track, DataTrack *entry);
		ppluint32	DeviceId;
		ppluint8	DeviceType;
		ppluint8	Page;

	public:
		CTrackList();
		CTrackList(const CTrackList &other);
		CTrackList(const CTrackList *other);
		~CTrackList();
		void copy(const CTrackList &other);
		void Clear();
		int GetMin();
		int GetMax();
		int Num();
		DataTrack *Get(int track);
		int GetCopy (int track,DataTrack *t);
		int Put(DataTrack *entry);
		int Delete(int track);
		int DeleteShift(int track, CTitleStore *tistore);
		int InsertShift(int track, CTitleStore *tistore);
		void Reset();
		DataTrack *GetFirst();
		DataTrack *GetNext();
};




class TrackInfo
{
	public:
		DataTitle		Ti;
		ppl6::CString	Version;
		ppl6::CString	Genre;
		ppl6::CString	Label;
		ppl6::CString	RecordingSource;
		ppl6::CString	RecordingDevice;
		ppl6::CBinary	Cover;

		TrackInfo();
		void clear();

};

bool getTrackInfoFromFile(TrackInfo &info, const ppl6::CString &Filename, int preferedId3Version=2);
ppluint32 findTitleIdByFilename(const ppl6::CString &Filename);



/*******************************************************
 * Configuration Class                                 *
 *******************************************************/

#define WM_NUM_LASTPLAYLISTS	5
class Config
{
	Q_DECLARE_TR_FUNCTIONS(Config)

	private:
		ppl6::CString	ConfigFile;

	public:
		Config();
		int Load();
		int Save();
		int setConfigFile(const ppl6::CString &filename);

		ppl6::CString	LastCoverPath;
		ppl6::CString	DataPath;
		ppl6::CString	TmpPath;
		//ppl6::CString	MP3Path;
		ppl6::CString	MP3Player;
		ppl6::CString	AIFFPlayer;
		ppl6::CString	Locale;
		ppl6::CString	UserName;
		ppl6::CString	UserCompany;
		ppl6::CString	Serial;
		ppl6::CString	Currency;
		ppl6::CString	PrinterName;
		ppl6::CString	PrinterFont;
		ppl6::CString	Logfile;
		ppl6::CString	DefaultPrintPath;
		ppl6::CString	CoverPath;
		ppl6::CString	LastPlaylistPath;
		ppl6::CString	LastPlaylists[WM_NUM_LASTPLAYLISTS];

		ppl6::CString	DevicePath[MAX_DEVICE_TYPES];	// Muss erweitert werden, wenn neue Device-Typen hinzukommen!

		ppl6::CArray	DirectorySearch;
		int				LogfileSize;
		int				LogfileGenerations;
		int				Debuglevel;
		bool			bPrintColors;
		bool			bShowSplashScreen;
		bool			bCheckForUpdatesOnStartup;
		bool			bAutomaticArtistSuggestion;
		bool			bAutomaticTitleSuggestion;
		int				ReadId3Tag;
		bool			bWriteID3Tags;
		bool			bRemoveOriginalId3Tags;
		int				ID3v2Padding;
		bool			bSaveOriginalMp3Tags;
		bool			bSaveOriginalMp3TagsOnAutoImport;
		bool			bNormalizeFilename;
		int				TagSaverRetryIntervall;
		bool			SearchDevice[MAX_DEVICE_TYPES];
		bool			SearchFast;
		bool			VisibleDevice[MAX_DEVICE_TYPES];
		bool			bEnableDebug;
		bool			bCacheDirectorySearch;
		bool			bAutomaticEditSearch;
		int				MaxFilenameLength;

		int				JpegQualityCover;
		int				JpegQualityPreview;

		// Server
		bool			bserverEnabled;
		bool			bserverEnableSSL;
		ppl6::CString	serverHost;
		int				serverPort;
		ppl6::CString	serverSSLKeyfile;
		ppl6::CString	serverSSLPassword;

		ppl6::CString	customMusicKey[26];
		ppl6::CString	customMusicKeyName;
		MusicKeyType	musicKeyDisplay;
		MusicKeyType	musicKeyTag;
		int				playlistView;

		class CDDB {
			public:
				CDDB();
				ppl6::CString	cddevice;
				ppl6::CString	server;
				int				port;

		};
		CDDB			cddb;

};

class CID3TagSaver : public ppl6::CThread
{
	private:
		ppl6::CMutex Mutex;
		ppl6::CAssocArray Todo;
		int PaddingSize;
		int RetryIntervall;

	public:
		CID3TagSaver();
		~CID3TagSaver();

		void SetPaddingSize(int bytes);
		void SetRetryIntervall(int seconds);

		virtual void ThreadMain(void *param);
		void Add(const char *filename, ppl6::CAssocArray *Tags, bool cleartag=false, bool writev1=true, bool writev2=true);
		int UpdateNow(const char *filename, ppl6::CAssocArray *Tags, bool cleartag=false);
};

class CStringCounterItem : public ppl6::CTreeItem
{
	public:
		ppl6::CWString	Name;
		ppluint32		Count;
        virtual int CompareNode(CTreeItem *item);
        virtual int CompareValue(void *value);

};

class CHashes : public ppl6::CThread
{
	friend class CWmClient;
	public:
		typedef std::set<ppluint32> TitleTree;
	private:
		typedef std::map<ppl6::CString,TitleTree >	WordTree;

		ppl6::CMutex	Mutex;
		CWmClient		*wm;
		WordTree		Artist;
		WordTree		Title;
		WordTree		Version;
		WordTree		Genre;
		WordTree		Tags;
		WordTree		Remarks;
		WordTree		Album;
		WordTree		Label;
		std::map<ppluint8,TitleTree>		MusicKeys;
		//CWordTree		Global;
		ppl6::CLog		*log;

		int AddTitleInternal(ppluint32 TitleId, const DataTitle *title=NULL);
		void AddWords(WordTree &Tree, ppl6::CArray &words,const DataTitle *title);
		void FindWords(const WordTree &Tree, ppl6::CArray &words, TitleTree &Result);
		void FindSingleWord(const WordTree &Tree, const ppl6::CString &Word, TitleTree &Result);
		void RemoveWords(WordTree &Tree, ppl6::CArray &words, const DataTitle *title);
		void Union(TitleTree &Result, const TitleTree &Tree1, const TitleTree &Tree2);
		void Copy(TitleTree &Result, const TitleTree &src);
		void Add(TitleTree &Result, const TitleTree &src);
	public:
		enum SearchFlags {
			SearchArtist=1,
			SearchTitle=2,
			SearchVersion=4,
			SearchGenre=8,
			SearchTags=16,
			SearchRemarks=32,
			SearchAlbum=64,
			SearchLabel=128,
			SearchAll=255
		};

		CHashes();
		~CHashes();
		virtual void ThreadMain(void *param);

		void Clear();
		int GetTags(const ppl6::CString &str, ppl6::CArray &words);
		int AddTitle(ppluint32 TitleId, const DataTitle *title=NULL);
		int RemoveTitle(ppluint32 TitleId, const DataTitle *title=NULL);

		int Find(const ppl6::CString &Artist, const ppl6::CString &Title, TitleTree &Result);
		int Find(const ppl6::CString &Artist, const ppl6::CString &Title, const ppl6::CString &Version, const ppl6::CString &Genre, const ppl6::CString &Tags, const ppl6::CString &Label, TitleTree &Result);
		int FindGlobal(const ppl6::CString &Query, TitleTree &Result, int Flags=SearchAll);
		int CheckDupes(const ppl6::CString &Artist, const ppl6::CString &Title, ppluint32 Version, ppluint32 Ignore=0);

};

class CWmServer : private ppl6::CTCPSocket, ppl6::CThread
{
	private:
		ppl6::CMutex			GlobalMutex;
		ppl6::CString			MyHost;
		int						MyPort;
		ppl6::CHostsAllow		*HostsAllow;
		ppl6::CLog				*Log;
		ppl6::CThreadPool	ConnectionPool;

		void ThreadMain(void *param);
		int ReceiveConnect(ppl6::CTCPSocket *socket, const char *host, int port);

	public:
		CWmServer();
		~CWmServer();
		int SetLogfile(ppl6::CLog *log);
		int Init(const ppl6::CString &host, int port);
		int SetHostsAllow(ppl6::CHostsAllow *allow);
		int Start();
		int Stop();




};

/*******************************************************
 * Main Client Class                                   *
 *******************************************************/
class CWmClient
{
	Q_DECLARE_TR_FUNCTIONS(CWmClient)

	private:
		ppl6::CMutex Mutex;
		ppl6::CString Str_Unknown;
		QApplication *app;
		int argc;
		char **argv;

		//ppl6::CpplThreadPool Threads;
		void InitErrors();

		QTranslator qtTranslator;
		QTranslator wmTranslator;

		int FirstStartDialog();
		int SelectLanguage();
		int LoadTranslation();
		int PrintTracklistTableHeader(QFont &Font, QPainter &painter,int x, int y);
		int PrintTracklistHeader(QFont &Font, QPainter &painter,int x, int y);
		int PrintTracklistDeviceHeader(QFont &Font, QPainter &painter,int x, int y,DataDevice *device);
		int PrintTracklistDisclaimer(QFont &Font, QPainter &painter);
		void initLetterReplacements();
		void initFilenameLetterReplacements();
		void addLetterReplacement(wchar_t letter, wchar_t replacement);
		void addLetterReplacement(const ppl6::CWString &letters, wchar_t replacement);
		void addFilenameLetterReplacement(const ppl6::CWString &letters, wchar_t replacement);
		void *MainMenue;
		ppl6::CGenericList EditorWindows;
		ppl6::CGenericList SearchWindows;
		ppl6::CGenericList CoverPrinterWindows;
		ppl6::CGenericList PlaylistWindows;
		ppl6::CGenericList SearchlistOverviewWindows;
		ppl6::CGenericList SearchlistWindows;
		ppl6::CGenericList DeviceListWindows;
		void *CoverViewerWindow;

		QDate					LatestPurchaseDate;

		std::map<wchar_t, wchar_t> letterReplacements;
		std::map<wchar_t, wchar_t> filenameLetterReplacements;


	public:
		Config conf;

		// Storage
		CStorage				Storage;
		CTitleStore				TitleStore;
		CVersionStore			VersionStore;
		CRecordSourceStore		RecordSourceStore;
		CLabelStore				LabelStore;
		CPurchaseSourceStore	PurchaseSourceStore;
		CRecordDeviceStore		RecordDeviceStore;
		CGenreStore				GenreStore;
		CShortcutStore			ShortcutStore;
		CTrackStore				TrackStore;
		CDeviceStore			DeviceStore;
		COimpDataStore			OimpDataStore;

		CID3TagSaver			ID3TagSaver;
		BackgroundJobs			Background;
		RegularExpressionCapture	RegExpCapture;
		CHashes					Hashes;
		CUpdateChecker			*UpdateChecker;

		CWmClient();
		~CWmClient();
		int Init(int argc, char **argv, QApplication *app);

		static int RaiseError();
		static int RaiseError(QWidget *object, QString msg);

		int Start();
		int InitStorage();
		void InitDataPath();
		void InitLogging();
		void UpdateMenue();

		int isValidDataPath(ppl6::CString &Path);

		int CloseDatabase();
		int LoadDatabase();
		int ImportDatabaseWM20();
		int CreateInitialDatabase();

		void MainMenueClosed();
		void OpenEditor(int devicetype, int deviceId=0, int page=0, int track=0);
		void OpenDeviceList(int devicetype);
		void OpenCoverPrinter();
		void OpenPlaylistDialog();
		void OpenSearchlistOverview();
		void OpenSearchlistDialog(const ppl6::CString Filename);

		QWidget *OpenSearch(const char *artist=NULL, const char *title=NULL);
		QWidget *OpenOrReuseSearch(QWidget *q, const char *artist=NULL, const char *title=NULL);
		void EditorClosed(void *object);
		void SearchClosed(void *object);
		void SearchlistOverviewClosed(void *object);
		void UpdateSearchlistOverviews();
		void SearchlistDialogClosed(void *object);
		void CoverPrinterClosed(void *object);
		void PlaylistClosed(void *object);
		void DeviceListClosed(void *object);
		void OpenCoverViewer(const QPixmap &pix);
		void UpdateCoverViewer(const QPixmap &pix);
		void CoverViewerClosed();
		bool IsCoverViewerVisible() const;

		int CurlTalk(ppl6::CCurl &Curl, ppl6::CAssocArray &cmd, ppl6::CAssocArray &answer);
		ppl6::CString GetOperatingSystem();
		ppluint32 GetHighestDeviceId(int DeviceType);
		ppl6::CString GetDeviceName(int DeviceType);
		ppl6::CString GetDeviceNameShort(ppluint8 DeviceType);
		QPixmap GetDevicePixmap(int DeviceType);
		QIcon GetDeviceIcon(int DeviceType);
		QDate GetLatestPurchaseDate();
		void SetLatestPurchaseDate(QDate Date);
		int LoadDevice(ppluint8 DeviceType, ppluint32 DeviceId, DataDevice *data);
		CTrackList *GetTracklist(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page);
		DataTrack *GetTrack(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track);
		DataTitle *GetTitle(ppluint32 TitleId);
		DataVersion *GetVersion(ppluint32 Id);
		DataGenre *GetGenre(ppluint32 Id);
		DataLabel *GetLabel(ppluint32 Id);
		DataRecordSource *GetRecordSource(ppluint32 Id);
		DataRecordDevice *GetRecordDevice(ppluint32 Id);

		const char *GetVersionText(ppluint32 Id);
		const char *GetGenreText(ppluint32 Id);
		const char *GetLabelText(ppluint32 Id);
		const char *GetRecordSourceText(ppluint32 Id);
		const char *GetRecordDeviceText(ppluint32 Id);

		ppl6::CString getXmlTitle(ppluint32 TitleId);


		QByteArray GetGeometry(const char *name);
		void SaveGeometry(const char *name, QByteArray Geometry);
		void ReloadTranslation();
		QString Unknown();
		ppl6::CString GetAudioPath(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page);
		ppl6::CString GetAudioFilename(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track);
		ppl6::CDirEntry StatAudioFile(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track);

		int TrashAudioFile(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track);
		int RenameAudioFile(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 OldTrack, ppluint32 NewTrack);
		ppl6::CString NextAudioFile(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track);
		int SaveID3Tags(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track, DataTitle &Ti, const ppl6::CString &Filename="");
		ppl6::CString NormalizeFilename(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track, DataTitle &Ti, const ppl6::CString &Suffix);

		int SaveOriginalAudioInfo(ppl6::CString &File, DataOimp &oimp);
		int WritePlaylist(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, CTrackList *list, DataDevice *device=NULL);
		int UpdateID3Tags(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, CTrackList *list);

		int PlayFile(const ppl6::CString &Filename);

		// Printing
		int PrintCoverDialog(QWidget *parent, int DeviceType, ppluint32 DeviceId);
		int PrintMP3Cover(QWidget *parent, int DeviceType, ppluint32 start, ppluint32 end);
		int PrintTracklist(QWidget *parent, int DeviceType, ppluint32 start, ppluint32 end);


		void NormalizeTerm(ppl6::CString &term);
		void NormalizeLetters(const std::map<wchar_t, wchar_t> &letters, ppl6::CWString &term);
		int GetWords(const ppl6::CString &str, ppl6::CArray &words);

		ppl6::CString DeCrypt(const ppl6::CString &str);
		ppl6::CString Crypt(const ppl6::CString &str);

		int StartServer();
		int StopServer();

};


#endif /* WINMUSIK3_H_ */
