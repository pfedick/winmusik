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
#include <QEvent>
#include <map>
#include <set>
#include <list>
#include <queue>

#define WITH_QT		// Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#define PPL_QT_STRING_UTF8

#include "ppl7.h"
#include "exceptions.h"

#include "wm_id3tagsaver.h"
#include "libwinmusik3.h"

using namespace de::pfp::winmusik;


#define WM_COPYRIGHT	"(c) Copyright by Patrick Fedick in 2022"
 // Die folgenden Werte werden für die Registry bzw. Ermitteln des Application Data
 // Verzeichnisses verwendet:
#define WM_ORGANISATION	"Patrick F.-Productions"
#define WM_APPNAME		"WinMusik3"
#define WM_REGISTRATION_URL	"https://www.pfp.de/winmusik/interface.html"
#define MAX_DEVICE_TYPES	10


typedef struct tagWindowListItem {
	struct tagWindowListItem* next, * previous;
	void* object;
} WINDOWLISTITEM;

class WMTreeItem : public QTreeWidgetItem
{
public:
	uint32_t	Id;
	uint32_t	Track;
};

extern ppl7::Logger* wmlog;

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

class CWmClient;
extern CWmClient* wm_main;

class CCallback
{
public:
	int progress;

	virtual ~CCallback();
	virtual void Update();
};



#include "regexpcapture.h"


/*******************************************************
 * Global Functions                                    *
 *******************************************************/
uint32_t Time2Int(const ppl7::String& Time);
ppl7::String Int2Time(uint32_t Time);
ppl7::String Long2Date(const ppl7::String& Format, uint32_t Date);

void getHarmonicKeys(std::set<int>& harmonics, int key);
void saveCover(const ppl7::String& filename, const QPixmap& Cover);
void getIconFromCover(ppl7::ByteArray& bin, const QPixmap& Cover);
void NormalizeImportString(ppl7::String& Buffer);
void setReadableLength(QLabel* label, int length);
void loadCoverToClipboard(const ppl7::String& Filename);


void SetWindowGeometry(QWidget* widget, const ppl7::String& name);
void SaveWindowGeometry(QWidget* widget, const ppl7::String& name);

ppl7::String getReadableTimeFromSeconds(int seconds);


/*******************************************************
 * Global Variablen                                    *
 *******************************************************/







class TrackInfo
{
public:
	DataTitle		Ti;
	ppl7::String	Version;
	ppl7::String	Genre;
	ppl7::String	Label;
	ppl7::String	RecordingSource;
	ppl7::String	RecordingDevice;
	ppl7::ByteArray	Cover;

	void clear();

};

bool getTrackInfoFromFile(TrackInfo& info, const ppl7::String& Filename, int preferedId3Version=2);
uint32_t findTitleIdByFilename(const ppl7::String& Filename);



/*******************************************************
 * Configuration Class                                 *
 *******************************************************/

#define WM_NUM_LASTPLAYLISTS	5
class Config
{
	Q_DECLARE_TR_FUNCTIONS(Config)

private:
	ppl7::String	ConfigFile;

public:
	Config();
	void load();
	void save();
	void trySave();
	void setConfigFile(const ppl7::String& filename);

	ppl7::String	LastCoverPath;
	ppl7::String	DataPath;
	ppl7::String	TmpPath;
	//ppl7::String	MP3Path;
	ppl7::String	MP3Player;
	ppl7::String	AIFFPlayer;
	ppl7::String	Locale;
	ppl7::String	UserName;
	ppl7::String	UserCompany;
	ppl7::String	Serial;
	ppl7::String	Currency;
	ppl7::String	PrinterName;
	ppl7::String	PrinterFont;
	ppl7::String	Logfile;
	ppl7::String	DefaultPrintPath;
	ppl7::String	CoverPath;
	ppl7::String	LastPlaylistPath;
	ppl7::String	LastPlaylists[WM_NUM_LASTPLAYLISTS];

	ppl7::String	DevicePath[MAX_DEVICE_TYPES];	// Muss erweitert werden, wenn neue Device-Typen hinzukommen!

	std::list<ppl7::String>	DirectorySearch;
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
	ppl7::String	serverHost;
	int				serverPort;
	ppl7::String	serverSSLKeyfile;
	ppl7::String	serverSSLPassword;

	ppl7::String	customMusicKey[26];
	ppl7::String	customMusicKeyName;
	MusicKeyType	musicKeyDisplay;
	MusicKeyType	musicKeyTag;
	int				playlistView;

	class CDDB {
	public:
		CDDB();
		ppl7::String	cddevice;
		ppl7::String	server;
		ppl7::String	proxy_server;
		ppl7::String	username;
		ppl7::String	hostname;
		ppl7::String	querypath;
		int				proxy_port;
		int				port;
		bool			useProxy;
	};
	CDDB			cddb;

	class PlaylistExport {
	public:
		PlaylistExport();
		ppl7::String	TargetPath;
		bool option_copy_files;
		bool option_prepend_tracknumber;
		bool export_m3u;
		bool export_pls;
		bool export_xspf;
		bool export_txt;
	};
	PlaylistExport playlist_export;

};

/*
class CStringCounterItem : public ppl6::CTreeItem
{
public:
	ppl7::WideString	Name;
	uint32_t			Count;
	virtual int CompareNode(CTreeItem* item);
	virtual int CompareValue(void* value);

};
*/

class ResultFilter;

class CHashes : public ppl7::Thread
{
	Q_DECLARE_TR_FUNCTIONS(CHashes)

		friend class CWmClient;
public:
	typedef std::set<uint32_t> TitleTree;
private:
	typedef std::map<ppl7::String, TitleTree >	WordTree;

	ppl7::Mutex		Mutex;
	CWmClient* wm;
	WordTree		Artist;
	WordTree		Title;
	WordTree		Version;
	WordTree		Genre;
	WordTree		Tags;
	WordTree		Remarks;
	WordTree		Album;
	WordTree		Label;
	std::map<uint8_t, TitleTree>		MusicKeys;
	//CWordTree		Global;
	ppl7::Logger* log;

	int AddTitleInternal(uint32_t TitleId, const DataTitle* title=nullptr);
	void AddWords(WordTree& Tree, const ppl7::Array& words, const DataTitle* title);
	void FindWords(const WordTree& Tree, const ppl7::Array& words, TitleTree& Result);
	void FindSingleWord(const WordTree& Tree, const ppl7::String& Word, TitleTree& Result, const ResultFilter& filter);
	void RemoveWords(WordTree& Tree, const ppl7::Array& words, const DataTitle* title);
	void Union(TitleTree& Result, const TitleTree& Tree1, const TitleTree& Tree2);
	void Copy(TitleTree& Result, const TitleTree& src);
	uint32_t Add(TitleTree& Result, const TitleTree& src, const ResultFilter& filter);
	uint32_t Add(TitleTree& Result, const TitleTree& src);
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
	virtual void run();

	void Clear();
	int GetTags(const ppl7::String& str, ppl7::Array& words);
	int AddTitle(uint32_t TitleId, const DataTitle* title=nullptr);
	int RemoveTitle(uint32_t TitleId, const DataTitle* title=nullptr);

	int Find(const ppl7::String& Artist, const ppl7::String& Title, TitleTree& Result);
	int Find(const ppl7::String& Artist, const ppl7::String& Title, const ppl7::String& Version, const ppl7::String& Genre, const ppl7::String& Tags, const ppl7::String& Label, TitleTree& Result);
	int FindGlobal(const ppl7::String& Query, TitleTree& Result, int Flags, const ResultFilter& filter);
	int CheckDupes(const ppl7::String& Artist, const ppl7::String& Title, uint32_t Version, uint32_t Ignore=0);

};

enum class WinMusikEvent
{
	retranslateUi=(QEvent::Type)(QEvent::User + 1)
};

enum class WindowType
{
	Misc=0,
	Editor=1,
	Search,
	CoverPrinter,
	Playlist,
	SearchlistOverview,
	Searchlist,
	DeviceList

};


/*******************************************************
 * Main Client Class                                   *
 *******************************************************/
class CWmClient
{
	Q_DECLARE_TR_FUNCTIONS(CWmClient)

private:
	ppl7::Mutex Mutex;
	ppl7::String Str_Unknown;
	QApplication* app;
	int argc;
	char** argv;

	//ppl6::CpplThreadPool Threads;


	QTranslator qtTranslator;
	QTranslator wmTranslator;

	void InitMusicKeys();
	int FirstStartDialog();
	int SelectLanguage();
	int LoadTranslation();
	int PrintTracklistTableHeader(QFont& Font, QPainter& painter, int x, int y);
	int PrintTracklistHeader(QFont& Font, QPainter& painter, int x, int y);
	int PrintTracklistDeviceHeader(QFont& Font, QPainter& painter, int x, int y, const DataDevice* device);
	int PrintTracklistDisclaimer(QFont& Font, QPainter& painter);
	void* MainMenue;
	std::map<WindowType, std::set<QWidget*>> WindowsList;
	void* CoverViewerWindow;

	QDate					LatestPurchaseDate;


public:
	Config conf;

	// Storage
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
	CStorage				Storage;

	de::pfp::winmusik::CID3TagSaver		ID3TagSaver;
	RegularExpressionCapture	RegExpCapture;
	CHashes					Hashes;

	de::pfp::winmusik::MusicalKeys MusicKeys;
	de::pfp::winmusik::WMNormalizer normalizer;

	CWmClient();
	~CWmClient();
	void Init(int argc, char** argv, QApplication* app);

	static int RaiseError(QWidget* object, const QString& msg, const QString& error);
	static int RaiseError(QWidget* object, const QString& error);

	int Start();
	void InitStorage();
	void InitDataPath();
	void InitLogging();
	void UpdateMenue();

	ppl7::String GetWmVersion() const;

	bool isValidDataPath(const ppl7::String& Path);

	void CloseDatabase();
	int LoadDatabase();
	int CreateInitialDatabase();

	void MainMenueClosed();

	void RegisterWindow(WindowType type, QWidget* widget);
	void UnRegisterWindow(WindowType type, QWidget* widget);


	void OpenEditor(int devicetype, int deviceId=0, int page=0, int track=0);
	void OpenDeviceList(int devicetype);
	void OpenCoverPrinter();
	void OpenPlaylistDialog();
	void OpenSearchlistOverview();
	void OpenSearchlistDialog(const ppl7::String Filename);



	QWidget* OpenSearch(const char* artist=nullptr, const char* title=nullptr);
	QWidget* OpenOrReuseSearch(QWidget* q, const char* artist=nullptr, const char* title=nullptr);
	void UpdateSearchlistOverviews();

	void OpenCoverViewer(const QPixmap& pix);
	void UpdateCoverViewer(const QPixmap& pix);
	void CoverViewerClosed();
	bool IsCoverViewerVisible() const;

	uint32_t GetHighestDeviceId(int DeviceType);
	ppl7::String GetDeviceName(int DeviceType);
	ppl7::String GetDeviceNameShort(uint8_t DeviceType);
	QPixmap GetDevicePixmap(int DeviceType);
	QIcon GetDeviceIcon(int DeviceType);
	QDate GetLatestPurchaseDate();
	void SetLatestPurchaseDate(QDate Date);
	int LoadDevice(uint8_t DeviceType, uint32_t DeviceId, DataDevice* data);
	void UpdateDevice(uint8_t DeviceType, uint32_t DeviceId);
	CTrackList GetTracklist(uint8_t Device, uint32_t DeviceId, uint8_t Page);
	const DataTrack* GetTrack(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track);
	const DataTitle* GetTitle(uint32_t TitleId) const;
	const DataVersion* GetVersion(uint32_t Id);
	const DataGenre* GetGenre(uint32_t Id);
	const DataLabel* GetLabel(uint32_t Id);
	const DataRecordSource* GetRecordSource(uint32_t Id);
	const DataRecordDevice* GetRecordDevice(uint32_t Id);

	const ppl7::String& GetVersionText(uint32_t Id);
	const ppl7::String& GetGenreText(uint32_t Id);
	const ppl7::String& GetLabelText(uint32_t Id);
	const ppl7::String& GetRecordSourceText(uint32_t Id);
	const ppl7::String& GetRecordDeviceText(uint32_t Id);

	ppl7::String getXmlTitle(uint32_t TitleId);


	QByteArray GetGeometry(const char* name);
	void SaveGeometry(const char* name, QByteArray Geometry);
	void ReloadTranslation();
	QString Unknown();
	ppl7::String GetAudioPath(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page);
	ppl7::String GetAudioFilename(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track);

	int TrashAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track);
	int RenameAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t OldTrack, uint32_t NewTrack);
	ppl7::String NextAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track);
	int SaveID3Tags(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track, const DataTitle& Ti, const ppl7::String& Filename="");
	ppl7::String NormalizeFilename(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track, const DataTitle& Ti, const ppl7::String& Suffix);

	int WritePlaylist(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, CTrackList* list, DataDevice* device=nullptr);
	int UpdateID3Tags(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, CTrackList* list);

	int PlayFile(const ppl7::String& Filename);

	// Printing
	int PrintCoverDialog(QWidget* parent, int DeviceType, uint32_t DeviceId);
	int PrintMP3Cover(QWidget* parent, int DeviceType, uint32_t start, uint32_t end);
	int PrintTracklist(QWidget* parent, int DeviceType, uint32_t start, uint32_t end);


};


#endif /* WINMUSIK3_H_ */
