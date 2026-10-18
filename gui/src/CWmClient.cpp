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

#ifdef _WIN32
#include <windows.h>
#endif

#include <locale.h>
#include "winmusik3.h"
#include <pplib/audio/id3tag.h>
#include "version.h"

#include "src/menue/menue.h"
#include "src/editor/edit.h"
#include "src/search/search.h"
#include "src/firststart/firststart.h"
#include "src/firststart/langselect.h"
#include "src/splashscreen/splashscreen.h"
#include "src/coverprinter/coverprinter.h"
#include "src/playlist/playlist.h"
#include "src/devicelist/devicelist.h"
#include "src/searchlists/searchlists.h"
#include "src/searchlists/searchlistdialog.h"
#include "src/coverviewer/coverviewer.h"
#include <QMessageBox>
#include <QLocale>
#include <QMenu>
#include "wm_coverdownload.h"

CWmClient* wm_main = NULL;

/*!\class CWmClient
 * \brief Hauptklasse der Client GUI
 *
 * Dies ist die Hauptklasse der Client GUI. Sie sorgt dafür, dass alle Daten geladen
 * werden und die entprechenden Fenster angezeigt werden (Menue, Setup, Edit, Preferences, Search,...)
 */

CWmClient::CWmClient()
{
    wm_main = this;
    MainMenue = NULL;
    CoverViewerWindow = NULL;
    LatestPurchaseDate = QDate::currentDate();
    Hashes.wm = this;
}

CWmClient::~CWmClient()
{
    Hashes.Clear();
    ID3TagSaver.threadStop();
    CloseDatabase();
    if (CoverViewerWindow) {
        delete (CoverViewer*)CoverViewerWindow;
        CoverViewerWindow = NULL;
    }
    if (MainMenue) delete (Menue*)MainMenue;
    MainMenue = NULL;
    if (wmlog) delete wmlog;
    wmlog = NULL;
}

void CWmClient::Init(int argc, char** argv, QApplication* app)
{
    this->app = app;
    this->argc = argc;
    this->argv = argv;
    pplib::String Tmp;
    Tmp = pplib::GetArgv(argc, argv, "-c");
    if (Tmp.notEmpty()) {
        conf.setConfigFile(Tmp);
    }
    InitStorage();
}

int CWmClient::RaiseError(QWidget* parent, const QString& error)
{
    QString msg = tr("The following error occured:");
    return RaiseError(parent, msg, error);
}

int CWmClient::RaiseError(QWidget* parent, const QString& msg, const QString& error)
{
    QString m = "<b>" + msg + "</b>";
    if (!error.isEmpty()) {
        m += "\n\n" + error;
    }
    return QMessageBox::critical(parent, tr("WinMusik"), m, QMessageBox::Ok);
}

int CWmClient::Start()
{
    pplib::String filename;
    pplib::String path = app->applicationDirPath();

    try {
        conf.load();
    }
    catch (...) {
        if (!FirstStartDialog()) {
            return 0;
        }
        try {
            conf.load();
        }
        catch (const pplib::Exception& exp) {
            ShowException(exp, tr("could not load configuration file"));
            return 0;
        }
    }
    SetBeatportSession(conf.beatport.clearance, conf.beatport.userAgent);
    LoadTranslation();
    InitMusicKeys();
    InitLogging();

    InitDataPath();

    if (!LoadDatabase()) {
        return 0;
    }

    RegExpCapture.load();

    Mutex.lock();
    if (MainMenue) delete (Menue*)MainMenue;
    MainMenue = NULL;
    Menue* w = new Menue(NULL, this);
    // w->setWindowFlags(Qt::Window|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint);
    w->show();
    MainMenue = w;
    Mutex.unlock();
    Hashes.threadStart();
    return 1;
}

void CWmClient::InitMusicKeys()
{
    MusicKeys.setCustomName(conf.customMusicKeyName);
    for (int i = 0; i < 26; i++)
        MusicKeys.setCustomKeyName(i, conf.customMusicKey[i]);
}

void CWmClient::InitLogging()
{
    Mutex.lock();
    if (conf.bEnableDebug) {
        if (!wmlog) {
            wmlog = new pplib::Logger;
        }
        wmlog->setLogfile(pplib::Logger::DEBUG, (const char*)conf.Logfile);
        wmlog->setLogLevel(pplib::Logger::DEBUG, conf.Debuglevel);
        wmlog->setLogRotate((uint64_t)conf.LogfileSize * 1024 * 1024, conf.LogfileGenerations);
        wmlog->printf(pplib::Logger::DEBUG, 1, "CWmClient", "InitLogging", __FILE__, __LINE__,
                      "Logfile initialized, Debuglevel: %i, Maxsize: %i MB, Generations: %i", conf.Debuglevel, conf.LogfileSize,
                      conf.LogfileGenerations);
        ID3TagSaver.SetLogger(wmlog);
    } else {
        if (wmlog) {
            wmlog->terminate();
            ID3TagSaver.SetLogger(NULL);
        }
    }
    Mutex.unlock();
}

void CWmClient::UpdateMenue()
{
    if (MainMenue) ((Menue*)MainMenue)->UpdateMenue();
}

QByteArray CWmClient::GetGeometry(const char* name)
{
    pplib::String File = WM_APPNAME;
    File += "-Geometry";
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, WM_ORGANISATION, File);
    settings.beginGroup("geometry");
    return settings.value(name).toByteArray();
}

void CWmClient::SaveGeometry(const char* name, QByteArray Geometry)
{
    pplib::String File = WM_APPNAME;
    File += "-Geometry";
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, WM_ORGANISATION, File);
    settings.beginGroup("geometry");
    settings.setValue(name, Geometry);
}

void CWmClient::InitDataPath()
{
    Storage.Init(conf.DataPath);
}

void CWmClient::InitStorage()
/*!\brief Storage Klassen registrieren
 *
 * Diese Funktion sort dafür, dass alle bekannte Storage Klassen registriert werden. Sie wird
 * im Rahmen der Funktion CWmClient::Init aufgerufen.
 */
{
    Storage.RegisterStorageClass(&TitleStore);
    Storage.RegisterStorageClass(&VersionStore);
    Storage.RegisterStorageClass(&RecordSourceStore);
    Storage.RegisterStorageClass(&LabelStore);
    Storage.RegisterStorageClass(&PurchaseSourceStore);
    Storage.RegisterStorageClass(&RecordDeviceStore);
    Storage.RegisterStorageClass(&GenreStore);
    Storage.RegisterStorageClass(&ShortcutStore);
    Storage.RegisterStorageClass(&TrackStore);
    Storage.RegisterStorageClass(&DeviceStore);
}

int CWmClient::SelectLanguage()
{
    conf.Locale = QLocale::system().name();
    conf.Locale.cut("_");
    conf.Locale.lowerCase();
    LangSelect w;
    w.Select(conf.Locale);
    w.show();
    int ret = w.exec();
    if (!ret) return 0;
    conf.Locale = w.GetSelected();
    return 1;
}

int CWmClient::LoadTranslation()
{
    pplib::String filename;
    // pplib::String path=app->applicationDirPath();
    // printf ("conf.Locale=%s\n",(const char*)conf.Locale);
    if (conf.Locale == "de") {
        setlocale(LC_CTYPE, "de_DE.UTF-8");
    } else {
        setlocale(LC_CTYPE, "en_US.UTF-8");
    }
    filename = ":/translation/qt_" + conf.Locale;
    if (qtTranslator.load(filename)) {
        app->installTranslator(&qtTranslator);
    }
    filename = ":/translation/winmusik_" + conf.Locale;
    if (wmTranslator.load(filename)) {
        // printf ("Übersetzung nicht geladen!\n");
        app->installTranslator(&wmTranslator);
    }

    Str_Unknown = tr("unknown");
    return 1;
}

void CWmClient::ReloadTranslation()
{
    LoadTranslation();
    Mutex.lock();
    // Alle Fenster aktualisieren
    std::map<WindowType, std::set<QWidget*>>::iterator type_it;
    std::set<QWidget*>::iterator win_it;
    for (type_it = WindowsList.begin(); type_it != WindowsList.end(); ++type_it) {
        std::set<QWidget*>& widgets = type_it->second;
        for (win_it = widgets.begin(); win_it != widgets.end(); ++win_it) {
            QEvent* event = new QEvent((QEvent::Type)WinMusikEvent::retranslateUi);
            QCoreApplication::postEvent((*win_it), event);
        }
    }
    if (MainMenue) ((Menue*)MainMenue)->ReloadTranslation();
    Mutex.unlock();
}

int CWmClient::FirstStartDialog()
{
    // The user should select his preferred language first
    if (!SelectLanguage()) return 0;

    // If he has not aborted, we can load the translation files
    LoadTranslation();

    // Now we can show the "FirstStart" Dialog
    FirstStart s(NULL, this);
    s.show();
    int ret = s.exec();
    if (!ret) return 0;

    // If the user has not aborted, we try to save the configuration
    try {
        conf.save();
    }
    catch (const pplib::Exception& exp) {
        ShowException(exp, tr("Could not load the WinMusik configuration!"));
        return 0;
    }
    // Everything is fine
    return 1;
}

bool CWmClient::isValidDataPath(const pplib::String& Path)
{
    // Der Pfad muss existieren
    if (!pplib::Dir::exists(Path)) return false;
    pplib::String s = Path + "/winmusik.dat";
    if (!pplib::File::exists(s)) return false;
    // TODO: Es muss geprüft werden, ob es sich um eine gültige WinMusik-Daten-Datei handelt
    CWMFile ff;
    try {
        ff.Open(s);
        return true;
    }
    catch (...) {
        return false;
    }
}

void CWmClient::CloseDatabase()
/*!\brief Close the database and all active windows
 *
 * This function closes all currently active windows and the main database.
 * It assures that all editor windows are saving their unsaved data first.
 *
 * It is save to change the database path in the configuration class after
 * calling this function.
 *
 * \returns On success this function returns 1, otherweise 0. You should not change
 * the datapath variable if a call to this function fails!
 */
{
    Hashes.Clear();
    Mutex.lock();
    std::map<WindowType, std::set<QWidget*>>::iterator type_it;
    std::set<QWidget*>::iterator win_it;
    for (type_it = WindowsList.begin(); type_it != WindowsList.end(); ++type_it) {
        std::set<QWidget*>& widgets = type_it->second;
        for (win_it = widgets.begin(); win_it != widgets.end(); ++win_it) {
            (*win_it)->setAttribute(Qt::WA_DeleteOnClose);
            (*win_it)->close();
        }
    }
    Mutex.unlock();
    if (CoverViewerWindow) {
        delete (CoverViewer*)CoverViewerWindow;
        CoverViewerWindow = NULL;
    }
}

void CWmClient::UpdateSearchlistOverviews()
{
    Mutex.lock();
    std::set<QWidget*>& widgets = WindowsList[WindowType::SearchlistOverview];
    std::set<QWidget*>::iterator win_it;
    for (win_it = widgets.begin(); win_it != widgets.end(); ++win_it) {
        (static_cast<Searchlists*>(*win_it))->Update();
    }
    Mutex.unlock();
}

void CWmClient::MainMenueClosed()
{
    CloseDatabase();
    Mutex.lock();
    MainMenue = NULL;
    Mutex.unlock();
}

void CWmClient::OpenEditor(int devicetype, int deviceId, int page, int track)
{
    if (!devicetype) return;
    Edit* edit = new Edit((Menue*)MainMenue, this, devicetype);
    edit->setWindowFlags(Qt::Window);
    edit->show();
    if (deviceId > 0) edit->OpenTrack((unsigned int)deviceId, (unsigned char)page, (unsigned short)track);
}

void CWmClient::OpenCoverPrinter()
{
    CoverPrinter* w = new CoverPrinter((Menue*)MainMenue, this);
    w->show();
}

void CWmClient::OpenPlaylistDialog()
{
    // printf ("Open Playlists\n");
    Playlist* w = new Playlist((Menue*)MainMenue, this);
    w->setWindowFlags(Qt::Window);
    w->show();
}

void CWmClient::OpenSearchlistOverview()
{
    // printf ("Open Playlists\n");
    Searchlists* w = new Searchlists((Menue*)MainMenue, this);
    w->setWindowFlags(Qt::Window);
    w->show();
}

void CWmClient::OpenSearchlistDialog(const pplib::String Filename)
{
    // printf ("Open Playlists\n");
    SearchlistDialog* w = new SearchlistDialog((Menue*)MainMenue, this, Filename);
    w->setWindowFlags(Qt::Window);
    w->show();
}

QWidget* CWmClient::OpenSearch(const char* artist, const char* title)
{
    Search* w = new Search((Menue*)MainMenue, this);
    w->setWindowFlags(Qt::Window);
    w->show();
    // w->setFocus();

    if (artist != NULL || title != NULL) w->FastSearch(artist, title);
    return w;
}

QWidget* CWmClient::OpenOrReuseSearch(QWidget* q, const char* artist, const char* title)
{
    // Wir schauen, ob es q in der Liste gibt
    Search* w;
    if (q) {
        Mutex.lock();
        std::set<QWidget*>::iterator it = WindowsList[WindowType::Search].find(q);
        if (it != WindowsList[WindowType::Search].end()) {
            Mutex.unlock();
            w = (Search*)q;
            w->setFocus();
            if (artist != NULL || title != NULL) w->FastSearch(artist, title);
            return w;
        }
        Mutex.unlock();
    }
    w = new Search((Menue*)MainMenue, this);
    w->setWindowFlags(Qt::Window);
    w->show();
    // w->setFocus();
    if (artist != NULL || title != NULL) w->FastSearch(artist, title);
    return w;
}

void CWmClient::CoverViewerClosed()
{
    Mutex.lock();
    CoverViewerWindow = NULL;
    Mutex.unlock();
}

void CWmClient::OpenCoverViewer(const QPixmap& pix)
{
    Mutex.lock();
    if (CoverViewerWindow == NULL) {
        CoverViewerWindow = new CoverViewer(NULL, this);
        if (!CoverViewerWindow) {
            Mutex.unlock();
            return;
        }
        ((CoverViewer*)CoverViewerWindow)->show();
    }
    ((CoverViewer*)CoverViewerWindow)->setCover(pix);
    Mutex.unlock();
}

void CWmClient::UpdateCoverViewer(const QPixmap& pix)
{
    Mutex.lock();
    if (CoverViewerWindow != NULL) {
        ((CoverViewer*)CoverViewerWindow)->setCover(pix);
    }
    Mutex.unlock();
}

bool CWmClient::IsCoverViewerVisible() const
{
    if (CoverViewerWindow != NULL) return true;
    return false;
}

int CWmClient::LoadDatabase()
{
    if (!Storage.DatabaseExists()) {
        if (QMessageBox::Yes == QMessageBox::question(NULL, tr("WinMusik"),
                                                      tr("Database file does not exist or is empty.\n\nShould I create a new Database?"),
                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {

            CreateInitialDatabase();
        } else {
            return 0;
        }
    }

    SplashScreen* splash = NULL;
    if (conf.bShowSplashScreen) {
        splash = new SplashScreen();
        // splash->setWindowFlags(Qt::Dialog|Qt::SplashScreen|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
        splash->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        splash->show();
        app->processEvents();
        splash->setMessage(tr("Loading Database..."));
    }
    try {
        Storage.LoadDatabase(splash);
    }
    catch (const pplib::Exception& exp) {
        ShowException(exp, tr("Could not load database"));
        return 0;
    }
    if (splash) delete splash;
    return 1;
}

void CWmClient::OpenDeviceList(int devicetype)
{
    DeviceList* edit = new DeviceList((Menue*)MainMenue, this, devicetype);
    edit->setWindowFlags(Qt::Window);
    edit->show();
}

uint32_t CWmClient::GetHighestDeviceId(int DeviceType)
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
    return DeviceStore.GetHighestDevice(DeviceType);
}

pplib::String CWmClient::GetDeviceName(int DeviceType)
{
    pplib::String s;
    switch (DeviceType) {
    case 1:
        s = tr("Music Cassette");
        break;
    case 2:
        s = tr("Audio CD");
        break;
    case 3:
        s = tr("Data Storage");
        break;
    case 4:
        s = tr("Record");
        break;
    case 5:
        s = tr("VHS-Tape");
        break;
    case 6:
        s = tr("Other");
        break;
    case 7:
        s = tr("MP3-CD");
        break;
    case 8:
        s = tr("Tape");
        break;
    case 9:
        s = tr("Audio DVD");
        break;
    }

    return s;
}

QPixmap CWmClient::GetDevicePixmap(int DeviceType)
{
    QPixmap pix;
    switch (DeviceType) {
    case 1:
        pix.load(":/devices48/resources/tr48x48-0001.png");
        break;
    case 2:
        pix.load(":/devices48/resources/tr48x48-0002.png");
        break;
    case 3:
        pix.load(":/devices48/resources/tr48x48-0003.png");
        break;
    case 4:
        pix.load(":/devices48/resources/tr48x48-0004.png");
        break;
    case 5:
        pix.load(":/devices48/resources/tr48x48-0005.png");
        break;
    case 6:
        pix.load(":/devices48/resources/tr48x48-0006.png");
        break;
    case 7:
        pix.load(":/devices48/resources/tr48x48-0007.png");
        break;
    case 8:
        pix.load(":/devices48/resources/tr48x48-0008.png");
        break;
    case 9:
        pix.load(":/devices48/resources/tr48x48-0009.png");
        break;
    }

    return pix;
}

QIcon CWmClient::GetDeviceIcon(int DeviceType)
{
    QIcon pix;
    switch (DeviceType) {
    case 1:
        pix.addFile(":/devices48/resources/tr48x48-0001.png");
        break;
    case 2:
        pix.addFile(":/devices48/resources/tr48x48-0002.png");
        break;
    case 3:
        pix.addFile(":/devices48/resources/tr48x48-0003.png");
        break;
    case 4:
        pix.addFile(":/devices48/resources/tr48x48-0004.png");
        break;
    case 5:
        pix.addFile(":/devices48/resources/tr48x48-0005.png");
        break;
    case 6:
        pix.addFile(":/devices48/resources/tr48x48-0006.png");
        break;
    case 7:
        pix.addFile(":/devices48/resources/tr48x48-0007.png");
        break;
    case 8:
        pix.addFile(":/devices48/resources/tr48x48-0008.png");
        break;
    case 9:
        pix.addFile(":/devices48/resources/tr48x48-0009.png");
        break;
    }
    return pix;
}

pplib::String CWmClient::GetDeviceNameShort(uint8_t DeviceType)
{
    pplib::String s;
    switch (DeviceType) {
    case 1:
        s = tr("CAS");
        break;
    case 2:
        s = tr("CD");
        break;
    case 3:
        s = tr("DATA");
        break;
    case 4:
        s = tr("RECORD");
        break;
    case 5:
        s = tr("VHS");
        break;
    case 6:
        s = tr("OTHER");
        break;
    case 7:
        s = tr("MP3");
        break;
    case 8:
        s = tr("TAPE");
        break;
    case 9:
        s = tr("DVD");
        break;
    default:
        s = "";
        break;
    }
    return s;
}

QDate CWmClient::GetLatestPurchaseDate()
{
    return LatestPurchaseDate;
}

void CWmClient::SetLatestPurchaseDate(QDate Date)
{
    LatestPurchaseDate = Date;
}

int CWmClient::LoadDevice(uint8_t DeviceType, uint32_t DeviceId, DataDevice* data)
{
    data->Clear();
    const DataDevice* dd = DeviceStore.GetPtr(DeviceType, DeviceId);
    if (dd) {
        data->CopyFrom(*dd);
        return 1;
    }
    return 0;
}

void CWmClient::UpdateDevice(uint8_t DeviceType, uint32_t DeviceId)
{
    DeviceStore.Update(DeviceType, DeviceId);
}

CTrackList CWmClient::GetTracklist(uint8_t Device, uint32_t DeviceId, uint8_t Page)
{
    return TrackStore.GetTracklist(Device, DeviceId, Page);
}

const DataTrack* CWmClient::GetTrack(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track)
{
    return TrackStore.GetPtr(Device, DeviceId, Page, Track);
}

const DataTitle* CWmClient::GetTitle(uint32_t TitleId) const
{
    return TitleStore.GetPtr(TitleId);
}

const DataVersion* CWmClient::GetVersion(uint32_t Id)
{
    return (const DataVersion*)VersionStore.GetPtr(Id);
}

const pplib::String& CWmClient::GetVersionText(uint32_t Id)
{
    const DataVersion* v = (const DataVersion*)VersionStore.GetPtr(Id);
    if (v) return v->Value;
    return Str_Unknown;
}

const DataGenre* CWmClient::GetGenre(uint32_t Id)
{
    return (const DataGenre*)GenreStore.GetPtr(Id);
}

const pplib::String& CWmClient::GetGenreText(uint32_t Id)
{
    const DataGenre* v = (const DataGenre*)GenreStore.GetPtr(Id);
    if (v) return v->Value;
    return Str_Unknown;
}

const DataLabel* CWmClient::GetLabel(uint32_t Id)
{
    return (const DataLabel*)LabelStore.GetPtr(Id);
}

const pplib::String& CWmClient::GetLabelText(uint32_t Id)
{
    const DataLabel* v = (const DataLabel*)LabelStore.GetPtr(Id);
    if (v) return v->Value;
    return Str_Unknown;
}

pplib::String CWmClient::getXmlTitle(uint32_t TitleId)
{
    pplib::String r;
    if (!TitleStore.Exists(TitleId)) return r;
    const DataTitle ti = TitleStore.Get(TitleId);
    r.setf("<titleId>%u</titleId>\n", TitleId);
    r.appendf("<deviceType>%i</deviceType>\n", ti.DeviceType);
    r.appendf("<deviceId>%i</deviceId>\n", ti.DeviceId);
    r.appendf("<devicePage>%i</devicePage>\n", ti.Page);
    r.appendf("<trackNum>%i</trackNum>\n", ti.Track);
    r += "<Artist>" + pplib::EscapeHTMLTags(ti.Artist) + "</Artist>\n";
    r += "<Title>" + pplib::EscapeHTMLTags(ti.Title) + "</Title>\n";
    r += "<Version>" + pplib::EscapeHTMLTags(GetVersionText(ti.VersionId)) + "</Version>\n";
    r += "<Genre>" + pplib::EscapeHTMLTags(GetGenreText(ti.GenreId)) + "</Genre>\n";
    r += "<Label>" + pplib::EscapeHTMLTags(GetLabelText(ti.LabelId)) + "</Label>\n";
    r += "<Album>" + pplib::EscapeHTMLTags(ti.Album) + "</Album>\n";
    r.appendf("<bpm>%i</bpm>\n", (int)ti.BPM);
    r.appendf("<bitrate>%i</bitrate>\n", (int)ti.Bitrate);
    r.appendf("<rating>%i</rating>\n", (int)ti.Rating);
    r.appendf("<trackLength>%i</trackLength>\n", (int)ti.Length);
    r.appendf("<energyLevel>%i</energyLevel>\n", (int)ti.EnergyLevel);
    r += "<musicKey verified=\"";
    if (ti.Flags & 16)
        r += "true";
    else
        r += "false";
    r += "\">" + MusicKeys.keyName(ti.Key, musicKeyTypeMusicalSharps) + "</musicKey>\n";
    return r;
}

const DataRecordSource* CWmClient::GetRecordSource(uint32_t Id)
{
    return (const DataRecordSource*)RecordSourceStore.GetPtr(Id);
}

const pplib::String& CWmClient::GetRecordSourceText(uint32_t Id)
{
    const DataRecordSource* v = (const DataRecordSource*)RecordSourceStore.GetPtr(Id);
    if (v) return v->Value;
    return Str_Unknown;
}

const DataRecordDevice* CWmClient::GetRecordDevice(uint32_t Id)
{
    return (const DataRecordDevice*)RecordDeviceStore.GetPtr(Id);
}

const pplib::String& CWmClient::GetRecordDeviceText(uint32_t Id)
{
    const DataRecordDevice* v = (const DataRecordDevice*)RecordDeviceStore.GetPtr(Id);
    if (v) return v->Value;
    return Str_Unknown;
}

QString CWmClient::Unknown()
{
    return Str_Unknown;
}

pplib::String CWmClient::GetAudioPath(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page)
{
    pplib::String DevicePath = conf.DevicePath[DeviceType];
    pplib::String Path;
    if (DevicePath.isEmpty()) return Path;
    Path = DevicePath;
    Path.trimRight("/");
    Path.trimRight("\\");
    Path.appendf("/%02u/%03u/", (uint32_t)(DeviceId / 100), DeviceId);

    DataDevice data;
    if (LoadDevice(DeviceType, DeviceId, &data)) {
        if (data.Pages > 1) {
            Path.appendf("%01u/", Page);
        }
    }
    return Path;
}

pplib::String CWmClient::GetAudioFilename(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track)
{
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__,
                      "Search for audio file: DeviceId=%u, Page=%u, Track=%u", DeviceId, Page, Track);
    pplib::String Pattern;
    pplib::String Path = GetAudioPath(DeviceType, DeviceId, Page);
    if (Path.isEmpty()) return Path;
    pplib::Dir Dir;
    try {
        Dir.open(Path, pplib::Dir::Sort::FilenameIgnoreCase);
    }
    catch (...) {
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "could not open directory: %s",
                          (const char*)Path);
        return pplib::String();
    }
    Pattern.setf("/^%03u-.*\\.(mp3|aiff|aif)$/i", Track);
    if (auto entry = Dir.findRegExp(Pattern); entry.has_value()) {
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Gefunden: %s",
                          (const char*)entry->File);
        return entry->File;
    }

    Pattern.setf("/^%03u\\.(mp3|aiff|aif)$/i", Track);
    if (auto entry = Dir.findRegExp(Pattern); entry.has_value()) {
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Gefunden: %s",
                          (const char*)entry->File);
        return entry->File;
    }
    Pattern.setf("/^%03u-.*\\.(mp3|aiff|aif)$/i8", Track);
    if (auto entry = Dir.findRegExp(Pattern); entry.has_value()) {
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Gefunden: %s",
                          (const char*)entry->File);
        return entry->File;
    }
    if (wmlog) wmlog->printf(pplib::Logger::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Nicht gefunden");
    return pplib::String();
}

pplib::String CWmClient::NextAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track)
{
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 3, "CWMClient", "NextAudioFile", __FILE__, __LINE__,
                      "Find next audio file: DeviceId=%u, Page=%u, Track=%u", DeviceId, Page, Track);
    pplib::String Filename;
    pplib::String Pattern;
    pplib::String Path = GetAudioPath(DeviceType, DeviceId, Page);
    if (Path.isEmpty()) return Path;
    pplib::Dir Dir;
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 5, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Öffne Verzeichnis: %s",
                      (const char*)Path);
    try {
        Dir.open(Path, pplib::Dir::Sort::FilenameIgnoreCase);
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 5, "CWMClient", "NextAudioFile", __FILE__, __LINE__,
                          "%zu Dateien vorhanden, suche nach Pattern...", Dir.size());
        try {
            for (const pplib::DirEntry& entry : Dir) {
                Filename = entry.Filename;
                // Der Dateiname darf nicht mit drei Ziffern und Bindestrich beginnen
                if (!pplib::RegEx::match("/^[0-9]{3}\\-.*/", Filename)) {
                    // Muss aber mit .mp3 oder .aiff enden und Daten enthalten (beim Download per Firefox wird eine leere Datei als
                    // Platzhalter angelegt)
                    if (entry.Size > 256) {
                        if (pplib::RegEx::match("/^.*\\.(mp3|aiff|aif|wav)$/i", Filename) == true) {
                            // Sehr schön. Nun benennen wir die Datei um und hängen die Track-Nummer davor
                            if (wmlog)
                                wmlog->printf(pplib::Logger::DEBUG, 8, "CWMClient", "NextAudioFile", __FILE__, __LINE__,
                                              "Datei passt auf Pattern: %s", (const char*)Filename);
                            pplib::String newFilename;
                            newFilename.setf("%s/%03u-%s", (const char*)entry.Path, Track, (const char*)Filename);
                            if (wmlog)
                                wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Rename %s => %s",
                                              (const char*)entry.File, (const char*)newFilename);
                            // Wir versuchen sie umzubenennen
                            try {
                                pplib::File::rename(entry.File, newFilename);
                                return newFilename;
                            }
                            catch (...) {
                            }
                            // if (wmlog) wmlog->LogError();
                            if (wmlog)
                                wmlog->printf(pplib::Logger::DEBUG, 8, "CWMClient", "NextAudioFile", __FILE__, __LINE__,
                                              "Error, versuche nächste Datei");
                            // Fehlgeschlagen, vielleicht gibt's ja noch andere Dateien
                        }
                    }
                }
            }
        }
        catch (...) {
        }
    }
    catch (...) {
    }

    // Nichts passendes gefunden, wir geben einen leeren String zurück
    if (wmlog) wmlog->printf(pplib::Logger::DEBUG, 8, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Nichts passendes gefunden");
    Path.clear();
    return Path;
}

pplib::String CWmClient::NormalizeFilename(
    uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track, const DataTitle& Ti, const pplib::String& Suffix)
{
    // TODO: Übergabeparameter für Audio-Format oder Suffix
    pplib::String Filename = GetAudioPath(DeviceType, DeviceId, Page);
    if (Filename.isEmpty()) return Filename;
    pplib::String Tmp;
    Tmp.setf("%03u-", Track);
    if (Ti.Artist.notEmpty())
        Tmp += Ti.Artist;
    else
        Tmp += "unknown";
    Tmp += " - ";
    if (Ti.Title.notEmpty())
        Tmp += Ti.Title;
    else
        Tmp += "unknown";
    // Version holen
    pplib::String Version = GetVersionText(Ti.VersionId);
    if (Version != "Single") {
        Tmp += " (" + Version + ")";
    }
    Tmp += ".";
    Tmp += Suffix;
    normalizer.NormalizeFilename(Tmp);
    pplib::WideString s = Tmp.toWideString();
    if (s.size() > (size_t)conf.MaxFilenameLength) {
        s.cut(conf.MaxFilenameLength - Suffix.size() - 1);
        Tmp = s;
        Tmp += ".";
        Tmp += pplib::String(Suffix);
    }
    Filename += Tmp;
    return Filename;
}

int CWmClient::SaveID3Tags(
    uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track, const DataTitle& Ti, const pplib::String& Filename)
{
    if (conf.bWriteID3Tags == false) return 0;
    pplib::String InternalFilename;
    pplib::String Tmp;
    if (Filename.notEmpty())
        InternalFilename = Filename;
    else
        InternalFilename = GetAudioFilename(DeviceType, DeviceId, Page, Track);
    if (InternalFilename.isEmpty()) {
        Tmp = tr("Track: %i");
        // ppl6::SetError(20022, Tmp, Track);
        return 0;
    }

    pplib::AssocArray Job;

    if (conf.bNormalizeFilename) {
        // Unter Windows würde ein rename an dieser Stelle fehlschlagen, wenn die Datei
        // bereits geöffnet ist (z.B. im MP3-Player). Daher geben wir die Aufgabe an den
        // TagSaver
        std::vector<pplib::String> matches;
        if (pplib::RegEx::capture("/.*\\.(.*?)$/", InternalFilename, matches)) {
            Tmp = NormalizeFilename(DeviceType, DeviceId, Page, Track, Ti, matches.at(1));
            Job.set("renamefile", Tmp);
        }
    }

    ID3TagSaver.SetPaddingSize(conf.ID3v2Padding);
    ID3TagSaver.SetRetryIntervall(conf.TagSaverRetryIntervall);

    pplib::String comment, version;
    version = GetVersionText(Ti.VersionId);

    Job.set("artist", (const char*)Ti.Artist);
    if (pplib::LowerCase(version) != "single")
        Job.setf("title", "%s (%s)", (const char*)Ti.Title, (const char*)version);
    else
        Job.setf("title", "%s", (const char*)Ti.Title);
    Job.set("album", Ti.Album);
    comment = Ti.Remarks;
    // if (comment.Len()>0) comment+=" - ";
    // comment+=version;
    Job.set("comment", comment);
    int r = Ti.Rating;
    if (Ti.Rating > 1) r = Ti.Rating - 1;
    Job.setf("rating", "%d", r * 255 / 5);
    Job.set("version", version);
    Job.setf("track", "%u", Track);
    if (Ti.EnergyLevel > 0) Job.setf("EnergyLevel", "%d", Ti.EnergyLevel);
    if (Ti.BPM > 0)
        Job.setf("bpm", "%u", Ti.BPM);
    else
        Job.set("bpm", "");
    if (conf.musicKeyTag != musicKeyTypeNone) Job.set("key", MusicKeys.keyName(Ti.Key, conf.musicKeyTag));
    Job.set("genre", GetGenreText(Ti.GenreId));
    Job.set("publisher", GetLabelText(Ti.LabelId));
    Tmp.setf("%u", Ti.ReleaseDate);
    Tmp.cut(4);
    Job.set("year", Tmp);
    ID3TagSaver.Add(InternalFilename, Job, conf.bRemoveOriginalId3Tags);
    return 1;
}

int CWmClient::WritePlaylist(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, CTrackList* list, DataDevice* device)
{
    pplib::String Filename, Tmp, Minuten, FilePath;
    const DataTrack* track;
    const DataTitle* Ti;
    if (!DeviceId) {
        // ppl6::SetError(20040);
        return 0;
    }
    if (!list) {
        // ppl6::SetError(20041);
        return 0;
    }
    pplib::String Path = GetAudioPath(DeviceType, DeviceId, Page);
    if (Path.isEmpty()) {
        // ppl6::SetError(20042);
        return 0;
    }

    Minuten = tr("min", "Minutes in Tracklisting of Playlist");

    try {
        pplib::File m3u;
        m3u.open(pplib::ToString("%s/000index.m3u", (const char*)Path), pplib::File::FileMode::WRITE);

        pplib::File pls;
        pls.open(pplib::ToString("%s/000index.pls", (const char*)Path), pplib::File::FileMode::WRITE);

        pplib::File txt;
        txt.open(pplib::ToString("%s/000index.txt", (const char*)Path), pplib::File::FileMode::WRITE);

        pplib::File xspf;
        xspf.open(pplib::ToString("%s/000index.xspf", (const char*)Path), pplib::File::FileMode::WRITE);

        m3u.puts("#EXTM3U\n");
        pls.puts("[playlist]\n");

        Tmp = tr("Tracklisting MP3-Medium", "Subject of Playlist");
        Tmp.appendf(" %u\r\n", DeviceId);
        txt.puts(Tmp);
        if (device != NULL && device->Title != NULL) {
            Tmp = device->Title;
            Tmp.trim();
            txt.puts(Tmp);
            txt.puts("\r\n");
        }
        txt.puts("\r\n");

        xspf.puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        xspf.puts("<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">\n");
        Tmp = tr("Playlist created by WinMusik");
        Tmp.trim();
        Tmp.appendf(" %s", WM_VERSION);
        xspf.putsf("<creator>%s</creator>\n", (const char*)Tmp);
        xspf.puts("<trackList>\n");
        int count = 0;
        int min = list->GetMin();
        int max = list->GetMax();
        for (int i = min; i <= max; i++) {
            track = list->GetPtr(i);
            if (track) {
                Ti = GetTitle(track->TitleId);
                if (Ti) {
                    count++;
                    if (Ti->Artist.notEmpty())
                        Tmp = Ti->Artist;
                    else
                        Tmp = "unknown";
                    Tmp += " - ";
                    if (Ti->Title.notEmpty())
                        Tmp += Ti->Title;
                    else
                        Tmp += "unknown";
                    Tmp += " (";
                    // Version holen
                    Tmp += GetVersionText(Ti->VersionId);
                    Tmp += ")";

                    FilePath = GetAudioFilename(DeviceType, DeviceId, Page, i);
                    Filename = pplib::File::getFilename(FilePath);
                    m3u.putsf("#EXTINF:%u,%s\n", Ti->Length, (const char*)Tmp);
                    m3u.putsf("%s\n", (const char*)Filename);

                    pls.putsf("File%i=%s\n", count, (const char*)Filename);
                    pls.putsf("Title%i=%s\n", count, (const char*)Tmp);
                    pls.putsf("Length%i=%u\n", count, Ti->Length);

                    xspf.putsf("  <track>\n");
                    xspf.putsf("     <trackNum>%u</trackNum>\n", i);
                    pplib::String TmpTxt = Tmp;
                    TmpTxt.chop(1);
                    txt.putsf("%3u. %s, %0i:%02i %s)\r\n", i, (const char*)TmpTxt, (int)(Ti->Length / 60), Ti->Length % 60,
                              (const char*)Minuten);

                    // Tmp=ppl6::UrlEncode(FilePath);
                    // Tmp.Replace("+","%20");
                    xspf.putsf("     <location>file://%s</location>\n", (const char*)pplib::EscapeHTMLTags(FilePath));
                    if (Ti->Artist.notEmpty()) xspf.putsf("     <creator>%s</creator>\n", (const char*)pplib::EscapeHTMLTags(Ti->Artist));
                    if (Ti->Title.notEmpty())
                        xspf.putsf("     <title>%s - %s (%s)</title>\n", (const char*)pplib::EscapeHTMLTags(Ti->Artist),
                                   (const char*)pplib::EscapeHTMLTags(Ti->Title),
                                   (const char*)pplib::EscapeHTMLTags(GetVersionText(Ti->VersionId)));
                    // xspf.Putsf("     <annotation>%s</annotation>\n",GetVersionText(Ti->VersionId));
                    if (Ti->Album.notEmpty()) xspf.putsf("     <album>%s</album>\n", (const char*)pplib::EscapeHTMLTags(Ti->Album));
                    xspf.putsf("     <duration>%u</duration>\n", Ti->Length * 1000); // Bringt VLC zum Absturz
                    xspf.putsf("  </track>\n");
                }
            }
        }
        pls.putsf("NumberOfEntries=%i\nVersion=2\n", count);

        xspf.putsf("</trackList>\n</playlist>\n");

        return 1;
    }
    catch (...) {
        return 0;
    }
}

int CWmClient::UpdateID3Tags(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, CTrackList* list)
{
    pplib::String Path, Filename, Tmp, Minuten;
    const DataTrack* track;
    const DataTitle* Ti;
    if (!DeviceId) {
        // ppl6::SetError(20040);
        return 0;
    }
    if (!list) {
        // ppl6::SetError(20041);
        return 0;
    }
    pplib::String DevicePath = conf.DevicePath[DeviceType];
    if (DevicePath.isEmpty()) {
        // ppl6::SetError(20042);
        return 0;
    }
    int min = list->GetMin();
    int max = list->GetMax();
    for (int i = min; i <= max; i++) {
        track = list->GetPtr(i);
        if (track) {
            Ti = GetTitle(track->TitleId);
            if (Ti) {
                if (!SaveID3Tags(DeviceType, DeviceId, Page, track->Track, *Ti)) return 0;
            }
        }
    }
    return 1;
}

int CWmClient::PlayFile(const pplib::String& Filename)
{
    pplib::String Player = conf.MP3Player;
    std::vector<pplib::String> matches;
    if (pplib::RegEx::capture("/\\.aif[f]{0,2}$/i", Filename, matches)) Player = conf.AIFFPlayer;
    if (Player.isEmpty()) Player = conf.MP3Player;

#ifdef _WIN32
    pplib::WideString f = Filename;
    // Windows mag keine Vorwärts-Slashes
    f.replace(L"/", L"\\");
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 1, "CWMClient", "PlayFile", __FILE__, __LINE__, "Datei abspielen: %ls", (const wchar_t*)f);
    if (Player.isEmpty()) {
        ShellExecuteW(NULL, L"open", (const wchar_t*)f, L"", L"", SW_SHOWNORMAL);
    } else {
        f = L"\"" + f;
        f += L"\"";
        pplib::WideString prog = Player;
        ShellExecuteW(NULL, L"open", (const wchar_t*)prog, (const wchar_t*)f, L"", SW_SHOWNORMAL);
    }
#else
    if (Player.isEmpty()) {
        QMessageBox::warning(NULL, tr("WinMusik: Attention"),
                             tr("There is no Audio player specified.\nPlease go to preferences and open the MP3 page. You can specify your "
                                "favorite player there."),
                             QMessageBox::Ok, QMessageBox::Ok);
        return 0;
    }
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 1, "CWMClient", "PlayFile", __FILE__, __LINE__, "Datei abspielen: %s", (const char*)Filename);
    pplib::String cmd;
    cmd.setf("%s \"%s\" > /dev/null 2>&1 &", (const char*)Player, (const char*)Filename);
    system((const char*)cmd);
#endif
    return 1;
}

int CWmClient::TrashAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t Track)
{
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 1, "CWMClient", "TrashAudioFile", __FILE__, __LINE__,
                      "Delete track: DeviceId=%u, Page=%u, Track=%u", DeviceId, Page, Track);
    pplib::String Path = GetAudioPath(DeviceType, DeviceId, Page);
    if (Path.isEmpty()) {
        if (wmlog) wmlog->printf(pplib::Logger::DEBUG, 6, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Kein MP3-Pfad angegeben");
        return 0;
    }
    Path.append("/Trash");
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Path for deleted files: %s",
                      (const char*)Path);
    if (!pplib::File::exists(Path)) {
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__,
                          "Path does not exists and will be created");
        try {
            pplib::Dir::mkDir(Path, true);
        }
        catch (const pplib::Exception& exp) {
            if (wmlog) {
                wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "create path failed");
                wmlog->printException(__FILE__, __LINE__, "CWMClient", "TrashAudioFile", exp);
            }
        }
        if (!pplib::File::exists(Path)) {
            if (wmlog)
                wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__,
                              "Path was created, but its still doesn't exist");
            return 0;
        }
    }
    if (!pplib::File::isDir(Path)) {
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Path is not a directory: %s",
                          (const char*)Path);
        return 0;
    }
    pplib::String old = GetAudioFilename(DeviceType, DeviceId, Page, Track);
    if (old.isEmpty()) return 1;
    pplib::String file = Path;
    file.appendf("/%03u-%02u-", DeviceId, Page);
    file += pplib::File::getFilename(old);
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Umbenennung: %s ==> %s",
                      (const char*)old, (const char*)file);
    try {
        pplib::File::rename(old, file);
    }
    catch (const pplib::Exception& exp) {
        if (wmlog) {
            wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Rename fehlgeschlagen");
            wmlog->printException(__FILE__, __LINE__, "CWMClient", "TrashAudioFile", exp);
        }
        return 0;
    }
    if (wmlog) wmlog->printf(pplib::Logger::DEBUG, 1, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Erfolgreich");
    return 1;
}

int CWmClient::RenameAudioFile(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint32_t OldTrack, uint32_t NewTrack)
{
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 1, "CWMClient", "RenameAudioFile", __FILE__, __LINE__,
                      "Datei umbenennen: DeviceId=%u, Page=%u, OldTrack=%u, NewTrack=%u", DeviceId, Page, OldTrack, NewTrack);
    pplib::String DevicePath = conf.DevicePath[DeviceType];
    if (DevicePath.isEmpty()) {
        if (wmlog) wmlog->printf(pplib::Logger::DEBUG, 6, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Kein MP3-Pfad angegeben");
        return 0;
    }
    pplib::String Old = GetAudioFilename(DeviceType, DeviceId, Page, OldTrack);
    if (Old.isEmpty()) {
        if (wmlog)
            wmlog->printf(pplib::Logger::DEBUG, 6, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Alter Track %u ist nicht vorhanden",
                          OldTrack);
        return 1;
    }

    pplib::String Path = pplib::File::getPath(Old);
    pplib::String Filename = pplib::File::getFilename(Old);
    Filename = Filename.mid(4);
    pplib::String NewFile;
    NewFile.setf("%s/%03u-%s", (const char*)Path, NewTrack, (const char*)Filename);
    if (wmlog)
        wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Umbenennung: %s ==> %s",
                      (const char*)Old, (const char*)NewFile);
    try {
        pplib::File::rename(Old, NewFile);
    }
    catch (const pplib::Exception& exp) {
        if (wmlog) wmlog->printf(pplib::Logger::DEBUG, 9, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Rename fehlgeschlagen");
        wmlog->printException(__FILE__, __LINE__, "CWMClient", "RenameAudioFile", exp);
        return 0;
    }
    if (wmlog) wmlog->printf(pplib::Logger::DEBUG, 1, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Erfolgreich");
    return 1;
}

void CWmClient::RegisterWindow(WindowType type, QWidget* widget)
{
    Mutex.lock();
    WindowsList[type].insert(widget);
    Mutex.unlock();
}

void CWmClient::UnRegisterWindow(WindowType type, QWidget* widget)
{
    Mutex.lock();
    WindowsList[type].erase(widget);
    Mutex.unlock();
}

pplib::String CWmClient::GetWmVersion() const
{
    pplib::String Tmp = tr("WinMusik %v%");
    Tmp.replace("%v%", WM_VERSION);
    return Tmp;
}
