/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

#define WITH_QT // Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#define PPL_WITH_QT6
#define PPL_QT_STRING_UTF8

#include "ppl7.h"
#include "exceptions.h"

#include "wm_id3tagsaver.h"
#include "libwinmusik3.h"
#include "coverdownloadmanager.h"

using namespace de::pfp::winmusik;

#define WM_COPYRIGHT "(c) Copyright by Patrick Fedick in 2024"
// Die folgenden Werte werden für die Registry bzw. Ermitteln des Application Data
// Verzeichnisses verwendet:
#define WM_ORGANISATION "Patrick F.-Productions"
#define WM_APPNAME "WinMusik3"
#define MAX_DEVICE_TYPES 10

typedef struct tagWindowListItem
{
    struct tagWindowListItem *next, *previous;
    void* object;
} WINDOWLISTITEM;

class WMTreeItem : public QTreeWidgetItem
{
public:
    uint32_t Id;
    uint32_t Track;
};

extern pplib::Logger* wmlog;

/*******************************************************
 * Structures                                          *
 *******************************************************/

typedef struct
{
    char tag[3];
    char SongName[30];
    char Artist[30];
    char Album[30];
    char Year[4];
    char Comment[29];
    char track;
    char Genre;
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
uint32_t Time2Int(const pplib::String& Time);
pplib::String Int2Time(uint32_t Time);
pplib::String Long2Date(const pplib::String& Format, uint32_t Date);

void getHarmonicKeys(std::set<int>& harmonics, int key);
void saveCover(const pplib::String& filename, const QPixmap& Cover);
void saveCover(pplib::ID3Tag& Tag, const QPixmap& Cover);
void saveCoverToFile(const pplib::String& filename, const QPixmap& Cover);
void getIconFromCover(pplib::ByteArray& bin, const QPixmap& Cover);
void NormalizeImportString(pplib::String& Buffer);
void setReadableLength(QLabel* label, int length);
void loadCoverToClipboard(const pplib::String& Filename);

void SetWindowGeometry(QWidget* widget, const pplib::String& name);
void SaveWindowGeometry(QWidget* widget, const pplib::String& name);

pplib::String getReadableTimeFromSeconds(int seconds);

/*******************************************************
 * Global Variablen                                    *
 *******************************************************/

class TrackInfo
{
public:
    DataTitle Ti;
    pplib::String Version;
    pplib::String Genre;
    pplib::String Label;
    pplib::String RecordingSource;
    pplib::String RecordingDevice;
    pplib::String KeyText; // only used when reading from idv3-tag
    pplib::ByteArray Cover;
};

bool getTrackInfoFromFile(TrackInfo& info, const pplib::String& Filename, int preferedId3Version = 2);
uint32_t findTitleIdByFilename(const pplib::String& Filename);

/*******************************************************
 * Configuration Class                                 *
 *******************************************************/

#define WM_NUM_LASTPLAYLISTS 5
class Config
{
    Q_DECLARE_TR_FUNCTIONS(Config)

private:
    pplib::String ConfigFile;

public:
    Config();
    void load();
    void save();
    void trySave();
    void setConfigFile(const pplib::String& filename);

    pplib::String LastCoverPath;
    pplib::String DataPath;
    pplib::String TmpPath;
    // pplib::String	MP3Path;
    pplib::String MP3Player;
    pplib::String AIFFPlayer;
    pplib::String Locale;
    pplib::String UserName;
    pplib::String UserCompany;
    pplib::String Serial;
    pplib::String Currency;
    pplib::String PrinterName;
    pplib::String PrinterFont;
    pplib::String Logfile;
    pplib::String DefaultPrintPath;
    pplib::String CoverPath;
    pplib::String LastPlaylistPath;
    pplib::String LastPlaylists[WM_NUM_LASTPLAYLISTS];

    pplib::String DevicePath[MAX_DEVICE_TYPES]; // Muss erweitert werden, wenn neue Device-Typen hinzukommen!
    pplib::String SearchListCoverPath;

    std::list<pplib::String> DirectorySearch;
    int LogfileSize;
    int LogfileGenerations;
    int Debuglevel;
    bool bPrintColors;
    bool bShowSplashScreen;
    bool bCheckForUpdatesOnStartup;
    bool bAutomaticArtistSuggestion;
    bool bAutomaticTitleSuggestion;
    int ReadId3Tag;
    bool bWriteID3Tags;
    bool bRemoveOriginalId3Tags;
    int ID3v2Padding;
    bool bSaveOriginalMp3Tags;
    bool bSaveOriginalMp3TagsOnAutoImport;
    bool bNormalizeFilename;
    int TagSaverRetryIntervall;
    bool SearchDevice[MAX_DEVICE_TYPES];
    bool SearchFast;
    bool VisibleDevice[MAX_DEVICE_TYPES];
    bool bEnableDebug;
    bool bCacheDirectorySearch;
    bool bAutomaticEditSearch;
    int MaxFilenameLength;

    int JpegQualityCover;
    int JpegQualityPreview;

    // Server
    bool bserverEnabled;
    bool bserverEnableSSL;
    pplib::String serverHost;
    int serverPort;
    pplib::String serverSSLKeyfile;
    pplib::String serverSSLPassword;

    pplib::String customMusicKey[26];
    pplib::String customMusicKeyName;
    MusicKeyType musicKeyDisplay;
    MusicKeyType musicKeyTag;
    int playlistView;

    class CDDB
    {
    public:
        CDDB();
        pplib::String cddevice;
        pplib::String server;
        pplib::String proxy_server;
        pplib::String username;
        pplib::String hostname;
        pplib::String querypath;
        int proxy_port;
        int port;
        bool useProxy;
    };
    CDDB cddb;

    class PlaylistExport
    {
    public:
        PlaylistExport();
        pplib::String TargetPath;
        bool option_copy_files;
        bool option_prepend_tracknumber;
        bool export_m3u;
        bool export_pls;
        bool export_xspf;
        bool export_txt;
        bool with_start_times;
        bool export_json;
        bool with_cover_icons;
    };
    PlaylistExport playlist_export;

    class Beatport
    {
    public:
        pplib::String clearance;
        pplib::String userAgent;
    };
    Beatport beatport;
};

/*
class CStringCounterItem : public ppl6::CTreeItem
{
public:
    pplib::WideString	Name;
    uint32_t			Count;
    virtual int CompareNode(CTreeItem* item);
    virtual int CompareValue(void* value);

};
*/

class ResultFilter;

class CHashes : public pplib::Thread
{
    Q_DECLARE_TR_FUNCTIONS(CHashes)

    friend class CWmClient;

public:
    typedef std::set<uint32_t> TitleTree;

private:
    typedef std::map<pplib::String, TitleTree> WordTree;

    pplib::Mutex Mutex;
    CWmClient* wm;
    WordTree Artist;
    WordTree Title;
    WordTree Version;
    WordTree Genre;
    WordTree Tags;
    WordTree Remarks;
    WordTree Album;
    WordTree Label;
    std::map<uint8_t, TitleTree> MusicKeys;
    // CWordTree		Global;
    pplib::Logger* log;

    int AddTitleInternal(uint32_t TitleId, const DataTitle* title = nullptr);
    void AddWords(WordTree& Tree, const pplib::Array& words, const DataTitle* title);
    void FindWords(const WordTree& Tree, const pplib::Array& words, TitleTree& Result);
    void FindSingleWord(const WordTree& Tree, const pplib::String& Word, TitleTree& Result, const ResultFilter& filter);
    void RemoveWords(WordTree& Tree, const pplib::Array& words, const DataTitle* title);
    void Union(TitleTree& Result, const TitleTree& Tree1, const TitleTree& Tree2);
    void Copy(TitleTree& Result, const TitleTree& src);
    uint32_t Add(TitleTree& Result, const TitleTree& src, const ResultFilter& filter);
    uint32_t Add(TitleTree& Result, const TitleTree& src);

public:
    enum SearchFlags
    {
        SearchArtist = 1,
        SearchTitle = 2,
        SearchVersion = 4,
        SearchGenre = 8,
        SearchTags = 16,
        SearchRemarks = 32,
        SearchAlbum = 64,
        SearchLabel = 128,
        SearchAll = 255
    };

    CHashes();
    ~CHashes();
    virtual void run();

    void Clear();
    int GetTags(const pplib::String& str, pplib::Array& words);
    int AddTitle(uint32_t TitleId, const DataTitle* title = nullptr);
    int RemoveTitle(uint32_t TitleId, const DataTitle* title = nullptr);

    int Find(const pplib::String& Artist, const pplib::String& Title, TitleTree& Result);
    int Find(const pplib::String& Artist,
             const pplib::String& Title,
             const pplib::String& Version,
             const pplib::String& Genre,
             const pplib::String& Tags,
             const pplib::String& Label,
             TitleTree& Result);
    int FindGlobal(const pplib::String& Query, TitleTree& Result, int Flags, const ResultFilter& filter);
    int CheckDupes(const pplib::String& Artist, const pplib::String& Title, uint32_t Version, uint32_t Ignore = 0);
};

enum class WinMusikEvent
{
    retranslateUi = (QEvent::Type)(QEvent::User + 1)
};

enum class WindowType
{
    Misc = 0,
    Editor = 1,
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
    pplib::Mutex Mutex;
    pplib::String Str_Unknown;
    QApplication* app;
    int argc;
    char** argv;

    // ppl6::CpplThreadPool Threads;

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

    QDate LatestPurchaseDate;

public:
    Config conf;

    // Storage
    CTitleStore TitleStore;
    CVersionStore VersionStore;
    CRecordSourceStore RecordSourceStore;
    CLabelStore LabelStore;
    CPurchaseSourceStore PurchaseSourceStore;
    CRecordDeviceStore RecordDeviceStore;
    CGenreStore GenreStore;
    CShortcutStore ShortcutStore;
    CTrackStore TrackStore;
    CDeviceStore DeviceStore;
    CStorage Storage;

    de::pfp::winmusik::CID3TagSaver ID3TagSaver;
    RegularExpressionCapture RegExpCapture;
    CHashes Hashes;

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

    pplib::String GetWmVersion() const;

    bool isValidDataPath(const pplib::String& Path);

    void CloseDatabase();
    int LoadDatabase();
    int CreateInitialDatabase();

    void MainMenueClosed();

    void RegisterWindow(WindowType type, QWidget* widget);
    void UnRegisterWindow(WindowType type, QWidget* widget);

    void OpenEditor(int devicetype, int deviceId = 0, int page = 0, int track = 0);
    void OpenDeviceList(int devicetype);
    void OpenCoverPrinter();
    void OpenPlaylistDialog();
    void OpenSearchlistOverview();
    void OpenSearchlistDialog(const pplib::String Filename);

    QWidget* OpenSearch(const char* artist = nullptr, const char* title = nullptr);
    QWidget* OpenOrReuseSearch(QWidget* q, const char* artist = nullptr, const char* title = nullptr);
    void UpdateSearchlistOverviews();

    void OpenCoverViewer(const QPixmap& pix);
    void UpdateCoverViewer(const QPixmap& pix);
    void CoverViewerClosed();
    bool IsCoverViewerVisible() const;

    uint32_t GetHighestDeviceId(int DeviceType);
    pplib::String GetDeviceName(int DeviceType);
    pplib::String GetDeviceNameShort(uint8_t DeviceType);
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

    const pplib::String& GetVersionText(uint32_t Id);
    const pplib::String& GetGenreText(uint32_t Id);
    const pplib::String& GetLabelText(uint32_t Id);
    const pplib::String& GetRecordSourceText(uint32_t Id);
    const pplib::String& GetRecordDeviceText(uint32_t Id);

    pplib::String getXmlTitle(uint32_t TitleId);

    QByteArray GetGeometry(const char* name);
    void SaveGeometry(const char* name, QByteArray Geometry);
    void ReloadTranslation();
    QString Unknown();
    pplib::String GetAudioPath(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page);
    pplib::String GetAudioFilename(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track);

    int TrashAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track);
    int RenameAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t OldTrack, uint32_t NewTrack);
    pplib::String NextAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track);
    int SaveID3Tags(
        uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track, const DataTitle& Ti, const pplib::String& Filename = "");
    pplib::String NormalizeFilename(
        uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track, const DataTitle& Ti, const pplib::String& Suffix);

    int WritePlaylist(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, CTrackList* list, DataDevice* device = nullptr);
    int UpdateID3Tags(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, CTrackList* list);

    int PlayFile(const pplib::String& Filename);

    // Printing
    int PrintCoverDialog(QWidget* parent, int DeviceType, uint32_t DeviceId);
    int PrintMP3Cover(QWidget* parent, int DeviceType, uint32_t start, uint32_t end);
    int PrintTracklist(QWidget* parent, int DeviceType, uint32_t start, uint32_t end);
};

#endif /* WINMUSIK3_H_ */
