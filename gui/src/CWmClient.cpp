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

#ifdef _WIN32
#include <windows.h>
#endif


#include <locale.h>
#include "winmusik3.h"
#include "version.h"

#include "src/menue/menue.h"
#include "src/editor/edit.h"
#include "src/search/search.h"
#include "src/firststart/firststart.h"
#include "langselect.h"
#include "splashscreen.h"
#include "coverprinter.h"
#include "src/playlist/playlist.h"
#include "devicelist.h"
#include "src/searchlists/searchlists.h"
#include "src/searchlists/searchlistdialog.h"
#include "coverviewer.h"
#include <QMessageBox>
#include <QLocale>
#include <QDesktopWidget>
#include <QMenu>

CWmClient* wm_main=NULL;

/*!\class CWmClient
 * \brief Hauptklasse der Client GUI
 *
 * Dies ist die Hauptklasse der Client GUI. Sie sorgt dafür, dass alle Daten geladen
 * werden und die entprechenden Fenster angezeigt werden (Menue, Setup, Edit, Preferences, Search,...)
 */

CWmClient::CWmClient()
{
	wm_main=this;
	InitErrors();
	MainMenue=NULL;
	CoverViewerWindow=NULL;
	LatestPurchaseDate=QDate::currentDate();
	Hashes.wm=this;
	initLetterReplacements();
	initFilenameLetterReplacements();
}

CWmClient::~CWmClient()
{
	Hashes.Clear();
	ID3TagSaver.threadStop();
	CloseDatabase();
	if (CoverViewerWindow) {
		delete (CoverViewer*)CoverViewerWindow;
		CoverViewerWindow=NULL;
	}
	if (MainMenue) delete (Menue*)MainMenue;
	MainMenue=NULL;
	if (wmlog) delete wmlog;
	wmlog=NULL;
}

void CWmClient::Init(int argc, char** argv, QApplication* app)
{
	this->app=app;
	this->argc=argc;
	this->argv=argv;
	ppl7::String Tmp;
	Tmp=ppl7::GetArgv(argc, argv, "-c");
	if (Tmp.notEmpty()) {
		conf.setConfigFile(Tmp);
	}
	InitStorage();
}

int CWmClient::RaiseError()
{
	QString m=tr("The following error occured:");
	return RaiseError(NULL, m);
}

int CWmClient::RaiseError(QWidget* object, QString msg)
{
	int err=ppl6::GetErrorCode();
	ppl7::String descr=ppl6::GetError();
	ppl7::String sub=ppl6::GetExtendedError();

	ppl7::String m=msg;
	m.trim();
	ppl7::String a=tr("Errorcode");
	m.appendf("\n\n%s: %u\n", (const char*)a, err);
	a=tr("Description");
	m.appendf("%s: %s\n", (const char*)a, (const char*)descr);
	if (sub.size() > 0) {
		a=tr("Extended Description");
		m.appendf("%s: %s\n", (const char*)a, (const char*)sub);
	}
	return QMessageBox::critical(object, tr("WinMusik"),
		m,
		QMessageBox::Ok);
}

int CWmClient::Start()
{
	ppl7::String filename;
	ppl7::String path=app->applicationDirPath();

	try {
		conf.load();
	} catch (...) {
		if (!FirstStartDialog()) {
			return 0;
		}
		try {
			conf.load();
		} catch (const ppl7::Exception& exp) {
			ShowException(exp, tr("could not load configuration file"));
			return 0;
		}
	}
	LoadTranslation();
	InitLogging();

	InitDataPath();

	if (!LoadDatabase()) {
		return 0;
	}

	RegExpCapture.load();

	Mutex.lock();
	if (MainMenue) delete (Menue*)MainMenue;
	MainMenue=NULL;
	Menue* w=new Menue(NULL, this);
	//w->setWindowFlags(Qt::Window|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint);
	w->show();
	MainMenue=w;
	Mutex.unlock();
	Hashes.threadStart();
	return 1;
}

void CWmClient::InitLogging()
{
	Mutex.lock();
	if (conf.bEnableDebug) {
		if (!wmlog) {
			wmlog=new ppl6::CLog;
		}
		wmlog->SetLogfile(ppl6::LOG::DEBUG, (const char*)conf.Logfile);
		wmlog->SetLogLevel(ppl6::LOG::DEBUG, conf.Debuglevel);
		wmlog->SetLogRotate((ppluint64)conf.LogfileSize * 1024 * 1024, conf.LogfileGenerations);
		wmlog->Printf(ppl6::LOG::DEBUG, 1, "CWmClient", "InitLogging", __FILE__, __LINE__,
			"Logfile initialized, Debuglevel: %i, Maxsize: %i MB, Generations: %i",
			conf.Debuglevel, conf.LogfileSize, conf.LogfileGenerations);

		wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "initFilenameLetterReplacements", __FILE__, __LINE__, "Letter Replacements for Filenames:");
		std::map<wchar_t, wchar_t>::const_iterator it;
		for (it=filenameLetterReplacements.begin();it != filenameLetterReplacements.end();it++) {
			wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "initFilenameLetterReplacements", __FILE__, __LINE__, "%d => %d (\"%lc\" => \"%lc\")", it->first, it->second, it->first, it->second);
		}
	} else {
		if (wmlog) wmlog->Terminate();
	}

	Mutex.unlock();

}

void CWmClient::UpdateMenue()
{
	if (MainMenue) ((Menue*)MainMenue)->UpdateMenue();
}


QByteArray CWmClient::GetGeometry(const char* name)
{
	ppl7::String File=WM_APPNAME;
	File+="-Geometry";
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, WM_ORGANISATION, File);
	settings.beginGroup("geometry");
	return settings.value(name).toByteArray();
}

void CWmClient::SaveGeometry(const char* name, QByteArray Geometry)
{
	ppl7::String File=WM_APPNAME;
	File+="-Geometry";
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
	Storage.RegisterStorageType(&TitleStore);
	Storage.RegisterStorageType(&VersionStore);
	Storage.RegisterStorageType(&RecordSourceStore);
	Storage.RegisterStorageType(&LabelStore);
	Storage.RegisterStorageType(&PurchaseSourceStore);
	Storage.RegisterStorageType(&RecordDeviceStore);
	Storage.RegisterStorageType(&GenreStore);
	Storage.RegisterStorageType(&ShortcutStore);
	Storage.RegisterStorageType(&TrackStore);
	Storage.RegisterStorageType(&DeviceStore);
	Storage.RegisterStorageType(&OimpDataStore);
}

int CWmClient::SelectLanguage()
{
	conf.Locale=QLocale::system().name();
	conf.Locale.cut("_");
	conf.Locale.lowerCase();
	LangSelect w;
	w.Select(conf.Locale);
	w.show();
	int ret=w.exec();
	if (!ret) return 0;
	conf.Locale=w.GetSelected();
	return 1;
}

int CWmClient::LoadTranslation()
{
	ppl7::String filename;
	//ppl7::String path=app->applicationDirPath();
	//printf ("conf.Locale=%s\n",(const char*)conf.Locale);
	if (conf.Locale == "de") {
		setlocale(LC_CTYPE, "de_DE.UTF-8");
	} else {
		setlocale(LC_CTYPE, "en_US.UTF-8");
	}
	filename=":/translation/qt_" + conf.Locale;
	qtTranslator.load(filename);
	app->installTranslator(&qtTranslator);
	filename=":/translation/winmusik_" + conf.Locale;
	if (!wmTranslator.load(filename)) {
		//printf ("Übersetzung nicht geladen!\n");
	}
	app->installTranslator(&wmTranslator);

	Str_Unknown=tr("unknown");
	return 1;
}

void CWmClient::ReloadTranslation()
{
	LoadTranslation();
	Mutex.lock();
	// Alle Fenster aktualisieren
	if (MainMenue) ((Menue*)MainMenue)->ReloadTranslation();

	Edit* edit;
	EditorWindows.Reset();
	while ((edit=(Edit*)EditorWindows.GetNext())) {
		edit->ReloadTranslation();
	}
	Search* search;
	SearchWindows.Reset();
	while ((search=(Search*)SearchWindows.GetNext())) {
		search->ReloadTranslation();
	}
	CoverPrinter* cover;
	CoverPrinterWindows.Reset();
	while ((cover=(CoverPrinter*)CoverPrinterWindows.GetFirst())) {
		cover->ReloadTranslation();
	}
	Playlist* playlist;
	PlaylistWindows.Reset();
	while ((playlist=(Playlist*)PlaylistWindows.GetFirst())) {
		playlist->ReloadTranslation();
	}
	/*
	PlaylistEditor *playlisteditor;
	PlaylistEditWindows.Reset();
	while ((playlisteditor=(PlaylistEditor *)PlaylistEditWindows.GetFirst())) {
		playlisteditor->ReloadTranslation();
	}
	*/
	DeviceList* devicelist;
	DeviceListWindows.Reset();
	while ((devicelist=(DeviceList*)DeviceListWindows.GetFirst())) {
		devicelist->ReloadTranslation();
	}

	{
		Searchlists* list;
		SearchlistOverviewWindows.Reset();
		while ((list=(Searchlists*)SearchlistOverviewWindows.GetFirst())) {
			list->ReloadTranslation();
		}
	}

	{
		SearchlistDialog* list;
		SearchlistWindows.Reset();
		while ((list=(SearchlistDialog*)SearchlistWindows.GetFirst())) {
			list->ReloadTranslation();
		}
	}

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
	int ret=s.exec();
	if (!ret) return 0;

	// If the user has not aborted, we try to save the configuration
	try {
		conf.save();
	} catch (const ppl7::Exception& exp) {
		ShowException(exp, tr("Could not load the WinMusik configuration!"));
		return 0;
	}
	// Everything is fine
	return 1;
}

bool CWmClient::isValidDataPath(const ppl7::String& Path)
{
	// Der Pfad muss existieren
	if (!ppl7::Dir::exists(Path)) return false;
	ppl7::String s=Path + "/winmusik.dat";
	if (!ppl7::File::exists(s)) return false;
	// TODO: Es muss geprüft werden, ob es sich um eine gültige WinMusik-Daten-Datei handelt
	CWMFile ff;
	if (!ff.Open(s)) return false;
	return true;

}

int CWmClient::CloseDatabase()
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
	Edit* edit;
	while ((edit=(Edit*)EditorWindows.GetFirst())) {
		Mutex.unlock();
		delete edit;
		Mutex.lock();
	}
	Search* search;
	while ((search=(Search*)SearchWindows.GetFirst())) {
		Mutex.unlock();
		delete search;
		Mutex.lock();
	}
	CoverPrinter* cover;
	while ((cover=(CoverPrinter*)CoverPrinterWindows.GetFirst())) {
		Mutex.unlock();
		delete cover;
		Mutex.lock();
	}
	Playlist* playlist;
	while ((playlist=(Playlist*)PlaylistWindows.GetFirst())) {
		Mutex.unlock();
		delete playlist;
		Mutex.lock();
	}
	/*
	PlaylistEditor *playlisteditor;
	while ((playlisteditor=(PlaylistEditor *)PlaylistEditWindows.GetFirst())) {
		Mutex.unlock();
		delete playlisteditor;
		Mutex.lock();
	}
	*/
	DeviceList* devicelist;
	while ((devicelist=(DeviceList*)DeviceListWindows.GetFirst())) {
		Mutex.unlock();
		delete devicelist;
		Mutex.lock();
	}
	Searchlists* searchlists;
	while ((searchlists=(Searchlists*)SearchlistOverviewWindows.GetFirst())) {
		Mutex.unlock();
		delete searchlists;
		Mutex.lock();
	}

	SearchlistDialog* searchlistdialog;
	while ((searchlistdialog=(SearchlistDialog*)SearchlistWindows.GetFirst())) {
		Mutex.unlock();
		delete searchlistdialog;
		Mutex.lock();
	}

	if (CoverViewerWindow) {
		Mutex.unlock();
		delete (CoverViewer*)CoverViewerWindow;
		CoverViewerWindow=NULL;
		Mutex.lock();
	}

	Mutex.unlock();
	return 1;
}

void CWmClient::UpdateSearchlistOverviews()
{
	Mutex.lock();
	Searchlists* list;
	SearchlistOverviewWindows.Reset();
	while ((list=(Searchlists*)SearchlistOverviewWindows.GetNext())) {
		list->Update();
	}
	Mutex.unlock();
}

void CWmClient::MainMenueClosed()
{
	CloseDatabase();
	Mutex.lock();
	MainMenue=NULL;
	Mutex.unlock();
}

void CWmClient::OpenEditor(int devicetype, int deviceId, int page, int track)
{
	if (!devicetype) return;
	Edit* edit=new Edit((Menue*)MainMenue, this, devicetype);
	edit->setWindowFlags(Qt::Window);
	edit->show();
	Mutex.lock();
	EditorWindows.Add(edit);
	Mutex.unlock();
	if (deviceId > 0) edit->OpenTrack((unsigned int)deviceId, (unsigned char)page, (unsigned short)track);
}

void CWmClient::OpenCoverPrinter()
{
	CoverPrinter* w=new CoverPrinter((Menue*)MainMenue, this);
	w->show();
	Mutex.lock();
	CoverPrinterWindows.Add(w);
	Mutex.unlock();
}

void CWmClient::CoverPrinterClosed(void* object)
{
	Mutex.lock();
	CoverPrinterWindows.Delete(object);
	Mutex.unlock();
}

void CWmClient::OpenPlaylistDialog()
{
	//printf ("Open Playlists\n");
	Playlist* w=new Playlist((Menue*)MainMenue, this);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.lock();
	PlaylistWindows.Add(w);
	Mutex.unlock();
}

void CWmClient::PlaylistClosed(void* object)
{
	Mutex.lock();
	PlaylistWindows.Delete(object);
	Mutex.unlock();

}

void CWmClient::OpenSearchlistOverview()
{
	//printf ("Open Playlists\n");
	Searchlists* w=new Searchlists((Menue*)MainMenue, this);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.lock();
	SearchlistOverviewWindows.Add(w);
	Mutex.unlock();
}

void CWmClient::SearchlistOverviewClosed(void* object)
{
	Mutex.lock();
	SearchlistOverviewWindows.Delete(object);
	Mutex.unlock();

}

void CWmClient::OpenSearchlistDialog(const ppl7::String Filename)
{
	//printf ("Open Playlists\n");
	SearchlistDialog* w=new SearchlistDialog((Menue*)MainMenue, this, Filename);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.lock();
	SearchlistWindows.Add(w);
	Mutex.unlock();
}

void CWmClient::SearchlistDialogClosed(void* object)
{
	Mutex.lock();
	SearchlistWindows.Delete(object);
	Mutex.unlock();

}

QWidget* CWmClient::OpenSearch(const char* artist, const char* title)
{
	Search* w=new Search((Menue*)MainMenue, this);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.lock();
	SearchWindows.Add(w);
	Mutex.unlock();
	if (artist != NULL || title != NULL) w->FastSearch(artist, title);
	return w;
}

QWidget* CWmClient::OpenOrReuseSearch(QWidget* q, const char* artist, const char* title)
{
	// Wir schauen, ob es q in der Liste gibt
	Search* w;
	if (q) {
		Mutex.lock();
		if (SearchWindows.HasObject(q)) {
			Mutex.unlock();
			w=(Search*)q;
			w->setFocus();
			if (artist != NULL || title != NULL) w->FastSearch(artist, title);
			return w;
		}
		Mutex.unlock();
	}
	w=new Search((Menue*)MainMenue, this);
	w->setWindowFlags(Qt::Window);
	w->show();
	SearchWindows.Add(w);
	Mutex.unlock();
	if (artist != NULL || title != NULL) w->FastSearch(artist, title);
	return w;
}

void CWmClient::EditorClosed(void* object)
{
	Mutex.lock();
	EditorWindows.Delete(object);
	Mutex.unlock();
}

void CWmClient::CoverViewerClosed()
{
	Mutex.lock();
	CoverViewerWindow=NULL;
	Mutex.unlock();
}

void CWmClient::OpenCoverViewer(const QPixmap& pix)
{
	Mutex.lock();
	if (CoverViewerWindow == NULL) {
		CoverViewerWindow=new CoverViewer(NULL, this);
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


void CWmClient::SearchClosed(void* object)
{
	Mutex.lock();
	SearchWindows.Delete(object);
	Mutex.unlock();
}

int CWmClient::LoadDatabase()
{
	SplashScreen* splash=NULL;
	if (conf.bShowSplashScreen) {
		splash=new SplashScreen();
		//splash->setWindowFlags(Qt::Dialog|Qt::SplashScreen|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
		splash->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
		splash->show();
		app->processEvents();
		splash->setMessage(tr("Loading Database..."));
	}
	if (!Storage.LoadDatabase(splash)) {
		ppl6::PushError();
		if (splash) delete splash;
		ppl6::PopError();
		return 0;
	}
	if (splash) delete splash;
	return 1;
}

void CWmClient::OpenDeviceList(int devicetype)
{
	DeviceList* edit=new DeviceList((Menue*)MainMenue, this, devicetype);
	edit->setWindowFlags(Qt::Window);
	edit->show();
	Mutex.lock();
	DeviceListWindows.Add(edit);
	Mutex.unlock();
}

void CWmClient::DeviceListClosed(void* object)
{
	Mutex.lock();
	DeviceListWindows.Delete(object);
	Mutex.unlock();

}

ppluint32 CWmClient::GetHighestDeviceId(int DeviceType)
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

ppl7::String CWmClient::GetDeviceName(int DeviceType)
{
	ppl7::String s;
	switch (DeviceType) {
	case 1: s=tr("Music Cassette");
		break;
	case 2: s=tr("Audio CD");
		break;
	case 3: s=tr("Data Storage");
		break;
	case 4: s=tr("Record");
		break;
	case 5: s=tr("VHS-Tape");
		break;
	case 6: s=tr("Other");
		break;
	case 7: s=tr("MP3-CD");
		break;
	case 8: s=tr("Tape");
		break;
	case 9: s=tr("Audio DVD");
		break;
	}

	return s;
}

QPixmap CWmClient::GetDevicePixmap(int DeviceType)
{
	QPixmap pix;
	switch (DeviceType) {
	case 1: pix.load(":/devices48/resources/tr48x48-0001.png");
		break;
	case 2: pix.load(":/devices48/resources/tr48x48-0002.png");
		break;
	case 3: pix.load(":/devices48/resources/tr48x48-0003.png");
		break;
	case 4: pix.load(":/devices48/resources/tr48x48-0004.png");
		break;
	case 5: pix.load(":/devices48/resources/tr48x48-0005.png");
		break;
	case 6: pix.load(":/devices48/resources/tr48x48-0006.png");
		break;
	case 7: pix.load(":/devices48/resources/tr48x48-0007.png");
		break;
	case 8: pix.load(":/devices48/resources/tr48x48-0008.png");
		break;
	case 9: pix.load(":/devices48/resources/tr48x48-0009.png");
		break;
	}

	return pix;
}

QIcon CWmClient::GetDeviceIcon(int DeviceType)
{
	QIcon pix;
	switch (DeviceType) {
	case 1: pix.addFile(":/devices48/resources/tr48x48-0001.png");
		break;
	case 2: pix.addFile(":/devices48/resources/tr48x48-0002.png");
		break;
	case 3: pix.addFile(":/devices48/resources/tr48x48-0003.png");
		break;
	case 4: pix.addFile(":/devices48/resources/tr48x48-0004.png");
		break;
	case 5: pix.addFile(":/devices48/resources/tr48x48-0005.png");
		break;
	case 6: pix.addFile(":/devices48/resources/tr48x48-0006.png");
		break;
	case 7: pix.addFile(":/devices48/resources/tr48x48-0007.png");
		break;
	case 8: pix.addFile(":/devices48/resources/tr48x48-0008.png");
		break;
	case 9: pix.addFile(":/devices48/resources/tr48x48-0009.png");
		break;
	}
	return pix;
}

ppl7::String CWmClient::GetDeviceNameShort(ppluint8 DeviceType)
{
	ppl7::String s;
	switch (DeviceType) {
	case 1: s=tr("CAS");
		break;
	case 2: s=tr("CD");
		break;
	case 3: s=tr("DATA");
		break;
	case 4: s=tr("RECORD");
		break;
	case 5: s=tr("VHS");
		break;
	case 6: s=tr("OTHER");
		break;
	case 7: s=tr("MP3");
		break;
	case 8: s=tr("TAPE");
		break;
	case 9: s=tr("DVD");
		break;
	default: s="";
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
	LatestPurchaseDate=Date;
}


int CWmClient::LoadDevice(ppluint8 DeviceType, ppluint32 DeviceId, DataDevice* data)
{
	data->Clear();
	//DeviceStore.Update(DeviceType,DeviceId);
	return DeviceStore.GetCopy(DeviceType, DeviceId, data);
}

void CWmClient::UpdateDevice(ppluint8 DeviceType, ppluint32 DeviceId)
{
	DeviceStore.Update(DeviceType, DeviceId);
}

CTrackList* CWmClient::GetTracklist(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page)
{
	return TrackStore.GetTracklist(Device, DeviceId, Page);
}

DataTrack* CWmClient::GetTrack(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track)
{
	return TrackStore.Get(Device, DeviceId, Page, Track);
}

DataTitle* CWmClient::GetTitle(ppluint32 TitleId)
{
	return TitleStore.Get(TitleId);
}

DataVersion* CWmClient::GetVersion(ppluint32 Id)
{
	return (DataVersion*)VersionStore.Get(Id);
}

const char* CWmClient::GetVersionText(ppluint32 Id)
{
	DataVersion* v=(DataVersion*)VersionStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}

DataGenre* CWmClient::GetGenre(ppluint32 Id)
{
	return (DataGenre*)GenreStore.Get(Id);
}

const char* CWmClient::GetGenreText(ppluint32 Id)
{
	DataGenre* v=(DataGenre*)GenreStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}

DataLabel* CWmClient::GetLabel(ppluint32 Id)
{
	return (DataLabel*)LabelStore.Get(Id);
}

const char* CWmClient::GetLabelText(ppluint32 Id)
{
	DataLabel* v=(DataLabel*)LabelStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}

ppl7::String CWmClient::getXmlTitle(ppluint32 TitleId)
{
	ppl7::String r;
	DataTitle* ti=TitleStore.Get(TitleId);
	if (!ti) return r;
	r.setf("<titleId>%u</titleId>\n", TitleId);
	r.appendf("<deviceType>%i</deviceType>\n", ti->DeviceType);
	r.appendf("<deviceId>%i</deviceId>\n", ti->DeviceId);
	r.appendf("<devicePage>%i</devicePage>\n", ti->Page);
	r.appendf("<trackNum>%i</trackNum>\n", ti->Track);
	r+="<Artist>" + ppl7::EscapeHTMLTags(ti->Artist) + "</Artist>\n";
	r+="<Title>" + ppl7::EscapeHTMLTags(ti->Title) + "</Title>\n";
	r+="<Version>" + ppl7::EscapeHTMLTags(GetVersionText(ti->VersionId)) + "</Version>\n";
	r+="<Genre>" + ppl7::EscapeHTMLTags(GetGenreText(ti->GenreId)) + "</Genre>\n";
	r+="<Label>" + ppl7::EscapeHTMLTags(GetLabelText(ti->LabelId)) + "</Label>\n";
	r+="<Album>" + ppl7::EscapeHTMLTags(ti->Album) + "</Album>\n";
	r.appendf("<bpm>%i</bpm>\n", (int)ti->BPM);
	r.appendf("<bitrate>%i</bitrate>\n", (int)ti->Bitrate);
	r.appendf("<rating>%i</rating>\n", (int)ti->Rating);
	r.appendf("<trackLength>%i</trackLength>\n", (int)ti->Length);
	r.appendf("<energyLevel>%i</energyLevel>\n", (int)ti->EnergyLevel);
	r+="<musicKey verified=\"";
	if (ti->Flags & 16) r+="true"; else r+="false";
	r+="\">" + ti->getKeyName(musicKeyTypeMusicalSharps) + "</musicKey>\n";
	return r;
}


DataRecordSource* CWmClient::GetRecordSource(ppluint32 Id)
{
	return (DataRecordSource*)RecordSourceStore.Get(Id);
}

const char* CWmClient::GetRecordSourceText(ppluint32 Id)
{
	DataRecordSource* v=(DataRecordSource*)RecordSourceStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}


DataRecordDevice* CWmClient::GetRecordDevice(ppluint32 Id)
{
	return (DataRecordDevice*)RecordDeviceStore.Get(Id);
}

const char* CWmClient::GetRecordDeviceText(ppluint32 Id)
{
	DataRecordDevice* v=(DataRecordDevice*)RecordDeviceStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}

QString CWmClient::Unknown()
{
	return Str_Unknown;
}


ppl7::String CWmClient::GetAudioPath(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page)
{
	ppl7::String DevicePath=conf.DevicePath[DeviceType];
	ppl7::String Path;
	if (DevicePath.isEmpty()) return Path;
	Path=DevicePath;
	Path.trimRight("/");
	Path.trimRight("\\");
	Path.appendf("/%02u/%03u/", (ppluint32)(DeviceId / 100), DeviceId);

	DataDevice data;
	if (LoadDevice(DeviceType, DeviceId, &data)) {
		if (data.Pages > 1) {
			Path.appendf("%01u/", Page);
		}
	}
	return Path;
}

ppl7::String CWmClient::GetAudioFilename(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Search for audio file: DeviceId=%u, Page=%u, Track=%u", DeviceId, Page, Track);
	ppl7::String Pattern;
	ppl7::String Path=GetAudioPath(DeviceType, DeviceId, Page);
	if (Path.isEmpty()) return Path;
	Pattern.setf("%03u-*.(mp3|aiff)", Track);
	ppl6::CDir Dir;
	const ppl6::CDirEntry* de;
	if (Dir.Open(Path, ppl6::CDir::Sort_Filename_IgnoreCase)) {
		if ((de=Dir.GetFirstPattern(Pattern, true))) {
			Path=de->File;
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Gefunden: %s", (const char*)Path);
			return Path;
		}

	}
	Pattern.setf("/^%03u\\.(mp3|aiff)$/i8", Track);
	if ((de=Dir.GetFirstRegExp(Pattern))) {
		Path=de->File;
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Gefunden: %s", (const char*)Path);
		return Path;
	}
	Pattern.setf("/^%03u\\-.*\\.(mp3|aiff)$/i8", Track);
	if ((de=Dir.GetFirstRegExp(Pattern))) {
		Path=de->File;
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Gefunden: %s", (const char*)Path);
		return Path;
	}



	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "GetAudioFilename", __FILE__, __LINE__, "Nicht gefunden");
	Path.clear();
	return Path;
}

ppl7::String CWmClient::NextAudioFile(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 3, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Find next audio file: DeviceId=%u, Page=%u, Track=%u", DeviceId, Page, Track);
	ppl7::String Filename;
	ppl7::String Pattern;
	ppl7::String Path=GetAudioPath(DeviceType, DeviceId, Page);
	if (Path.isEmpty()) return Path;
	ppl6::CDir Dir;
	const ppl6::CDirEntry* entry;
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 5, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Öffne Verzeichnis: %s", (const char*)Path);
	if (Dir.Open(Path, ppl6::CDir::Sort_Filename_IgnoreCase)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 5, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "%i Dateien vorhanden, suche nach Pattern...", Dir.Num());
		while ((entry=Dir.GetNext())) {
			Filename=entry->Filename;
			// Der Dateiname darf nicht mit drei Ziffern und Bindestrich beginnen
			if (!Filename.pregMatch("/^[0-9]{3}\\-.*/")) {
				// Muss aber mit .mp3 oder .aiff enden und Daten enthalten (beim Download per Firefox wird eine leere Datei als Platzhalter angelegt)
				if (entry->Size > 256) {
					if (Filename.pregMatch("/^.*\\.(mp3|aiff|aif|wav)$/i") == true) {
						// Sehr schön. Nun benennen wir die Datei um und hängen die Track-Nummer davor
						if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 8, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Datei passt auf Pattern: %s", (const char*)Filename);
						ppl7::String newFilename;
						newFilename.setf("%s/%03u-%s", (const char*)entry->Path, Track, (const char*)Filename);
						if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Rename %s => %s", (const char*)entry->File, (const char*)newFilename);
						// Wir versuchen sie umzubenennen
						try {
							ppl7::File::rename(entry->File, newFilename);
							return newFilename;
						} catch (...) {}
						//if (wmlog) wmlog->LogError();
						if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 8, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Error, versuche nächste Datei");
						// Fehlgeschlagen, vielleicht gibt's ja noch andere Dateien
					}
				}
			}
		}
	}
	// Nichts passendes gefunden, wir geben einen leeren String zurück
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 8, "CWMClient", "NextAudioFile", __FILE__, __LINE__, "Nichts passendes gefunden");
	Path.clear();
	return Path;
}

ppl7::String CWmClient::NormalizeFilename(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track, DataTitle& Ti, const ppl7::String& Suffix)
{
	// TODO: Übergabeparameter für Audio-Format oder Suffix
	ppl7::String Filename=GetAudioPath(DeviceType, DeviceId, Page);
	if (Filename.isEmpty()) return Filename;
	ppl7::String Tmp;
	Tmp.setf("%03u-", Track);
	if (Ti.Artist.notEmpty()) Tmp+=Ti.Artist;
	else Tmp+="unknown";
	Tmp+=" - ";
	if (Ti.Title.notEmpty()) Tmp+=Ti.Title;
	else Tmp+="unknown";
	// Version holen
	ppl7::String Version=GetVersionText(Ti.VersionId);
	if (Version != "Single") {
		Tmp+=" (" + Version + ")";
	}
	Tmp+=".";
	Tmp+=Suffix;
	// Problematische Zeichen rausfiltern
	Tmp.replace("ß", "ss");
	Tmp.trim();
	// Wir müssen mit Unicode arbeiten
	ppl7::WideString w=Tmp;
	NormalizeLetters(filenameLetterReplacements, w);
	if (w.size() > (size_t)conf.MaxFilenameLength) {
		w.cut(conf.MaxFilenameLength - Suffix.size() - 1);
		w+=".";
		w+=ppl7::WideString(Suffix);
	}
	Tmp=w;
	Filename+=Tmp;
	return Filename;
}

int CWmClient::SaveID3Tags(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track, DataTitle& Ti, const ppl7::String& Filename)
{
	if (conf.bWriteID3Tags == false) return 0;
	ppl7::String InternalFilename;
	ppl7::String Tmp;
	if (Filename.notEmpty()) InternalFilename=Filename;
	else InternalFilename=GetAudioFilename(DeviceType, DeviceId, Page, Track);
	if (InternalFilename.isEmpty()) {
		Tmp=tr("Track: %i");
		ppl6::SetError(20022, Tmp, Track);
		return 0;
	}

	ppl7::AssocArray Job;

	if (conf.bNormalizeFilename) {
		// Unter Windows würde ein rename an dieser Stelle fehlschlagen, wenn die Datei
		// bereits geöffnet ist (z.B. im MP3-Player). Daher geben wir die Aufgabe an den
		// TagSaver
		ppl7::Array matches;
		if (InternalFilename.pregMatch("/.*\\.(.*?)$/", matches)) {
			Tmp=NormalizeFilename(DeviceType, DeviceId, Page, Track, Ti, matches.get(1));
			Job.set("renamefile", Tmp);
		}
	}

	ID3TagSaver.SetPaddingSize(conf.ID3v2Padding);
	ID3TagSaver.SetRetryIntervall(conf.TagSaverRetryIntervall);

	ppl7::String comment, version;
	version=GetVersionText(Ti.VersionId);

	Job.set("artist", (const char*)Ti.Artist);
	if (ppl7::LowerCase(version) != "single")	Job.setf("title", "%s (%s)", (const char*)Ti.Title, (const char*)version);
	else Job.setf("title", "%s", (const char*)Ti.Title);
	Job.set("album", (const char*)Ti.Album);
	comment=Ti.Remarks;
	//if (comment.Len()>0) comment+=" - ";
	//comment+=version;
	Job.set("comment", comment);
	int r=Ti.Rating;
	if (Ti.Rating > 1) r=Ti.Rating - 1;
	Job.setf("rating", "%d", r * 255 / 5);
	Job.set("version", version);
	Job.setf("track", "%u", Track);
	if (Ti.EnergyLevel > 0) Job.setf("EnergyLevel", "%d", Ti.EnergyLevel);
	if (Ti.BPM > 0) Job.setf("bpm", "%u", Ti.BPM);
	else Job.setf("bpm", "");
	if (conf.musicKeyTag != musicKeyTypeNone) Job.set("key", Ti.getKeyName(conf.musicKeyTag));
	Job.set("genre", GetGenreText(Ti.GenreId));
	Job.set("publisher", GetLabelText(Ti.LabelId));
	Tmp.setf("%u", Ti.ReleaseDate);
	Tmp.cut(4);
	Job.set("year", Tmp);
	ID3TagSaver.Add(InternalFilename, Job, conf.bRemoveOriginalId3Tags);
	return 1;
}

int CWmClient::SaveOriginalAudioInfo(ppl7::String& File, DataOimp& oimp)
{
	ppl6::CFile ff;
	ppl7::String Tmp;
	if (!ff.Open(File, "rb")) return 0;
	if (ff.Size() < 256) {
		ppl6::SetError(20017, "%s", (const char*)File);
		return 0;
	}
	// Alte Daten löschen
	oimp.Filename=ppl7::File::getFilename(File);
	oimp.ID3v1.clear();
	oimp.ID3v2.clear();
	// ID3v1-Tag einlesen falls vorhanden
	const char* buffer=ff.Map(ff.Lof() - 128, 128);
	if (buffer[0] == 'T' && buffer[1] == 'A' && buffer[2] == 'G') {
		ID3TAG* tag=(ID3TAG*)buffer;
		Tmp.set(tag->Artist, 30);
		Tmp.trim();
		oimp.ID3v1.set("artist", Tmp);
		Tmp.set(tag->SongName, 30);
		Tmp.trim();
		oimp.ID3v1.set("title", Tmp);

		Tmp.set(tag->Album, 30);
		Tmp.trim();
		oimp.ID3v1.set("album", Tmp);

		Tmp.set(tag->Year, 4);
		Tmp.trim();
		oimp.ID3v1.set("year", Tmp);

		Tmp.set(tag->Comment, 29);
		Tmp.trim();
		oimp.ID3v1.set("comment", Tmp);

		Tmp=ppl7::GetID3GenreName(tag->Genre);
		oimp.ID3v1.set("genre", Tmp);
	}
	ppl7::ID3Tag Tag;
	try {
		Tag.load(File);
		oimp.ID3v2.set("artist", Tag.getArtist());
		oimp.ID3v2.set("title", Tag.getTitle());
		oimp.ID3v2.set("album", Tag.getAlbum());
		oimp.ID3v2.set("year", Tag.getYear());
		oimp.ID3v2.set("comment", Tag.getComment());
		oimp.ID3v2.set("genre", Tag.getGenre());
		oimp.ID3v2.set("remixer", Tag.getRemixer());
	} catch (...) {}
	return OimpDataStore.Put(&oimp);
}


int CWmClient::WritePlaylist(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, CTrackList* list, DataDevice* device)
{
	ppl7::String Filename, Tmp, Minuten, FilePath;
	DataTrack* track;
	DataTitle* Ti;
	if (!DeviceId) {
		ppl6::SetError(20040);
		return 0;
	}
	if (!list) {
		ppl6::SetError(20041);
		return 0;
	}
	ppl7::String Path=GetAudioPath(DeviceType, DeviceId, Page);
	if (Path.isEmpty()) {
		ppl6::SetError(20042);
		return 0;
	}

	Minuten=tr("min", "Minutes in Tracklisting of Playlist");

	ppl6::CFile m3u;
	if (!m3u.Openf("%s/000index.m3u", "wb", (const char*)Path)) return 0;

	ppl6::CFile pls;
	if (!pls.Openf("%s/000index.pls", "wb", (const char*)Path)) return 0;

	ppl6::CFile txt;
	if (!txt.Openf("%s/000index.txt", "wb", (const char*)Path)) return 0;

	ppl6::CFile xspf;
	if (!xspf.Openf("%s/000index.xspf", "wb", (const char*)Path)) return 0;

	m3u.Puts("#EXTM3U\n");
	pls.Puts("[playlist]\n");

	Tmp=tr("Tracklisting MP3-Medium", "Subject of Playlist");
	Tmp.appendf(" %u\r\n", DeviceId);
	txt.Puts(Tmp);
	if (device != NULL && device->Title != NULL) {
		Tmp=device->Title;
		Tmp.trim();
		txt.Puts(Tmp);
		txt.Puts("\r\n");
	}
	txt.Puts("\r\n");

	xspf.Puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	xspf.Puts("<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">\n");
	Tmp=tr("Playlist created by WinMusik");
	Tmp.trim();
	Tmp.appendf(" %s", WM_VERSION);
	xspf.Putsf("<creator>%s</creator>\n", (const char*)Tmp);
	xspf.Puts("<trackList>\n");
	int count=0;
	int min=list->GetMin();
	int max=list->GetMax();
	for (int i=min;i <= max;i++) {
		track=list->Get(i);
		if (track) {
			Ti=GetTitle(track->TitleId);
			if (Ti) {
				count++;
				if (Ti->Artist.notEmpty()) Tmp=Ti->Artist;
				else Tmp="unknown";
				Tmp+=" - ";
				if (Ti->Title.notEmpty()) Tmp+=Ti->Title;
				else Tmp+="unknown";
				Tmp+=" (";
				// Version holen
				Tmp+=GetVersionText(Ti->VersionId);
				Tmp+=")";

				FilePath=GetAudioFilename(DeviceType, DeviceId, Page, i);
				Filename=ppl7::File::getFilename(FilePath);
				m3u.Putsf("#EXTINF:%u,%s\n", Ti->Length, (const char*)Tmp);
				m3u.Putsf("%s\n", (const char*)Filename);

				pls.Putsf("File%i=%s\n", count, (const char*)Filename);
				pls.Putsf("Title%i=%s\n", count, (const char*)Tmp);
				pls.Putsf("Length%i=%u\n", count, Ti->Length);

				xspf.Putsf("  <track>\n");
				xspf.Putsf("     <trackNum>%u</trackNum>\n", i);
				ppl7::String TmpTxt=Tmp;
				TmpTxt.chop(1);
				txt.Putsf("%3u. %s, %0i:%02i %s)\r\n", i, (const char*)TmpTxt, (int)(Ti->Length / 60), Ti->Length % 60, (const char*)Minuten);

				//Tmp=ppl6::UrlEncode(FilePath);
				//Tmp.Replace("+","%20");
				xspf.Putsf("     <location>file://%s</location>\n", (const char*)ppl6::EscapeHTMLTags(FilePath));
				if (Ti->Artist.notEmpty()) xspf.Putsf("     <creator>%s</creator>\n", (const char*)ppl6::EscapeHTMLTags(Ti->Artist));
				if (Ti->Title.notEmpty()) xspf.Putsf("     <title>%s - %s (%s)</title>\n",
					(const char*)ppl6::EscapeHTMLTags(Ti->Artist),
					(const char*)ppl6::EscapeHTMLTags(Ti->Title),
					(const char*)ppl6::EscapeHTMLTags(GetVersionText(Ti->VersionId)));
				//xspf.Putsf("     <annotation>%s</annotation>\n",GetVersionText(Ti->VersionId));
				if (Ti->Album.notEmpty()) xspf.Putsf("     <album>%s</album>\n", (const char*)ppl6::EscapeHTMLTags(Ti->Album));
				xspf.Putsf("     <duration>%u</duration>\n", Ti->Length * 1000);  // Bringt VLC zum Absturz
				xspf.Putsf("  </track>\n");


			}
		}
	}
	pls.Putsf("NumberOfEntries=%i\nVersion=2\n", count);

	xspf.Putsf("</trackList>\n</playlist>\n");


	return 1;
}

int CWmClient::UpdateID3Tags(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, CTrackList* list)
{
	ppl7::String Path, Filename, Tmp, Minuten;
	DataTrack* track;
	DataTitle* Ti;
	if (!DeviceId) {
		ppl6::SetError(20040);
		return 0;
	}
	if (!list) {
		ppl6::SetError(20041);
		return 0;
	}
	ppl7::String DevicePath=conf.DevicePath[DeviceType];
	if (DevicePath.isEmpty()) {
		ppl6::SetError(20042);
		return 0;
	}
	int min=list->GetMin();
	int max=list->GetMax();
	for (int i=min;i <= max;i++) {
		track=list->Get(i);
		if (track) {
			Ti=GetTitle(track->TitleId);
			if (Ti) {
				if (!SaveID3Tags(DeviceType, DeviceId, Page, track->Track, *Ti)) return 0;
			}
		}
	}
	return 1;
}


int CWmClient::PlayFile(const ppl7::String& Filename)
{
	ppl7::String Player=conf.MP3Player;
	ppl7::Array matches;
	if (Filename.pregMatch("/\\.aif[f]{0,2}$/i", matches)) Player=conf.AIFFPlayer;
	if (Player.isEmpty()) Player=conf.MP3Player;

#ifdef _WIN32
	ppl7::WideString f=Filename;
	// Windows mag keine Vorwärts-Slashes
	f.replace("/", "\\");
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CWMClient", "PlayFile", __FILE__, __LINE__, "Datei abspielen: %s", (const char*)f);
	if (Player.isEmpty()) {
		ShellExecuteW(NULL, L"open", (const wchar_t*)f,
			L"", L"", SW_SHOWNORMAL);
	} else {
		f="\"" + f;
		f+="\"";
		ppl7::WideString prog=Player;
		ShellExecuteW(NULL, L"open", (const wchar_t*)prog, (const wchar_t*)f,
			L"", SW_SHOWNORMAL);
}
#else
	if (Player.isEmpty()) {
		QMessageBox::warning(NULL, tr("WinMusik: Attention"),
			tr("There is no Audio player specified.\nPlease go to preferences and open the MP3 page. You can specify your favorite player there."),
			QMessageBox::Ok, QMessageBox::Ok);
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CWMClient", "PlayFile", __FILE__, __LINE__, "Datei abspielen: %s", (const char*)Filename);
	ppl7::String cmd;
	cmd.setf("%s \"%s\" > /dev/null 2>&1 &", (const char*)Player, (const char*)Filename);
	system((const char*)cmd);
#endif
	return 1;
}

int CWmClient::TrashAudioFile(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 Track)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Delete track: DeviceId=%u, Page=%u, Track=%u", DeviceId, Page, Track);
	ppl7::String Path=GetAudioPath(DeviceType, DeviceId, Page);
	if (Path.isEmpty()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 6, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Kein MP3-Pfad angegeben");
		return 0;
	}
	Path.append("/Trash");
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Path for deleted files: %s", (const char*)Path);
	if (!ppl6::CFile::Exists(Path)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Path does not exists and will be created");
		if (!ppl6::MkDir(Path, 1)) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "create path failed");
			if (wmlog) wmlog->LogError("CWMClient", "TrashAudioFile", __FILE__, __LINE__);
			return 0;
		}
		if (!ppl6::CFile::Exists(Path)) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Path was created, but its still doesn't exist");
			return 0;
		}
	}
	if (!ppl6::IsDir(Path)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Path is not a directory: %s", (const char*)Path);
		return 0;
	}
	ppl7::String old=GetAudioFilename(DeviceType, DeviceId, Page, Track);
	if (old.isEmpty()) return 1;
	ppl7::String file=Path;
	file.appendf("/%03u-%02u-", DeviceId, Page);
	file+=ppl7::File::getFilename(old);
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Umbenennung: %s ==> %s", (const char*)old, (const char*)file);
	if (!ppl6::CFile::RenameFile(old, file)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Rename fehlgeschlagen");
		if (wmlog) wmlog->LogError("CWMClient", "TrashAudioFile", __FILE__, __LINE__);
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CWMClient", "TrashAudioFile", __FILE__, __LINE__, "Erfolgreich");
	return 1;
}


int CWmClient::RenameAudioFile(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint32 OldTrack, ppluint32 NewTrack)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Datei umbenennen: DeviceId=%u, Page=%u, OldTrack=%u, NewTrack=%u", DeviceId, Page, OldTrack, NewTrack);
	ppl7::String DevicePath=conf.DevicePath[DeviceType];
	if (DevicePath.isEmpty()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 6, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Kein MP3-Pfad angegeben");
		return 0;
	}
	ppl7::String Old=GetAudioFilename(DeviceType, DeviceId, Page, OldTrack);
	if (Old.isEmpty()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 6, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Alter Track %u ist nicht vorhanden", OldTrack);
		return 1;
	}

	ppl7::String Path=ppl7::File::getPath(Old);
	ppl7::String Filename=ppl7::File::getFilename(Old);
	Filename=Filename.mid(4);
	ppl7::String NewFile;
	NewFile.setf("%s/%03u-%s", (const char*)Path, NewTrack, (const char*)Filename);
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Umbenennung: %s ==> %s", (const char*)Old, (const char*)NewFile);
	if (!ppl6::CFile::RenameFile(Old, NewFile)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Rename fehlgeschlagen");
		if (wmlog) wmlog->LogError("CWMClient", "RenameAudioFile", __FILE__, __LINE__);
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CWMClient", "RenameAudioFile", __FILE__, __LINE__, "Erfolgreich");
	return 1;
}

void CWmClient::initLetterReplacements()
{
	addLetterReplacement(ppl7::WideString(L"&+()_,!?/"), L' ');
	addLetterReplacement(ppl7::WideString(L".:''`"), 0);
	addLetterReplacement(ppl7::WideString(L"°"), L'o');
	addLetterReplacement(ppl7::WideString(L"àáâãäåāăąæ"), L'a');
	addLetterReplacement(ppl7::WideString(L"þ"), L'b');
	addLetterReplacement(ppl7::WideString(L"çćĉċč"), L'c');
	addLetterReplacement(ppl7::WideString(L"ďđ"), L'd');
	addLetterReplacement(ppl7::WideString(L"èéêëēĕėęě"), L'e');
	addLetterReplacement(ppl7::WideString(L"ĝğġģ"), L'g');
	addLetterReplacement(ppl7::WideString(L"ĥħ"), L'h');
	addLetterReplacement(ppl7::WideString(L"ìíîïĩīĭįı"), L'i');
	addLetterReplacement(ppl7::WideString(L"ĵ"), L'j');
	addLetterReplacement(ppl7::WideString(L"ķĸ"), L'k');
	addLetterReplacement(ppl7::WideString(L"ĺļľŀł"), L'l');
	addLetterReplacement(ppl7::WideString(L"ñńņňŉŋ"), L'n');
	addLetterReplacement(ppl7::WideString(L"òóôõöøōŏőœ"), L'o');
	addLetterReplacement(ppl7::WideString(L"ŕŗř"), L'r');
	addLetterReplacement(ppl7::WideString(L"śŝşš"), L's');
	addLetterReplacement(ppl7::WideString(L"ţťŧ"), L't');
	addLetterReplacement(ppl7::WideString(L"ùúûüũūŭůűų"), L'u');
	addLetterReplacement(ppl7::WideString(L"ŵ"), L'w');
	addLetterReplacement(ppl7::WideString(L"ýÿŷ"), L'y');
	addLetterReplacement(ppl7::WideString(L"źżž"), L'z');
	addLetterReplacement(ppl7::WideString(L"–"), L' ');
}

void CWmClient::initFilenameLetterReplacements()
{
	addFilenameLetterReplacement(ppl7::WideString(L"&"), L'+');
	addFilenameLetterReplacement(ppl7::WideString(L"\"´`"), L'\'');
	addFilenameLetterReplacement(ppl7::WideString(L"{"), L'(');
	addFilenameLetterReplacement(ppl7::WideString(L"}"), L')');
	addFilenameLetterReplacement(ppl7::WideString(L"|%#;:<>*?\\/"), L' ');
	addFilenameLetterReplacement(ppl7::WideString(L"$"), L'S');
	addFilenameLetterReplacement(ppl7::WideString(L"°"), L'o');
	addFilenameLetterReplacement(ppl7::WideString(L"þ"), L'b');

}

void CWmClient::addLetterReplacement(wchar_t letter, wchar_t replacement)
{
	letterReplacements[letter]=replacement;
}

void CWmClient::addLetterReplacement(const ppl7::WideString& letters, wchar_t replacement)
{
	for (size_t i=0;i < letters.size();i++) {
		addLetterReplacement(letters[i], replacement);
	}
}

void CWmClient::addFilenameLetterReplacement(const ppl7::WideString& letters, wchar_t replacement)
{
	//printf ("Adding Letters: %ls (%i letters)",(const wchar_t*)letters,letters.Len());
	for (size_t i=0;i < letters.size();i++) {
		filenameLetterReplacements[letters[i]]=replacement;
	}
}

static void ReplaceIfExists(ppl7::WideString& s, const wchar_t* search, const ppl7::WideString& replace)
{
	if (s.isEmpty()) return;
	const wchar_t* buffer=(wchar_t*)s.getPtr();
	if (!buffer) return;
	if (wcsstr(buffer, search)) {
		ppl7::WideString ss;
		ss.set(search);
		s.replace(ss, replace);
	}
}

void CWmClient::NormalizeLetters(const std::map<wchar_t, wchar_t>& letters, ppl7::WideString& term)
{
	wchar_t* buffer;
	std::map<wchar_t, wchar_t>::const_iterator it;
	size_t ss=term.size();
	size_t target=0;
	buffer=(wchar_t*)term.getPtr();
	wchar_t c;
	for (size_t i=0;i < ss;i++) {
		c=buffer[i];
		if (c < L'A' || c>L'z' || (c > L'Z' && c < L'a')) {
			it=letters.find(c);
			if (it != letters.end()) {
				if (it->second != (wchar_t)0) {
					buffer[target++]=it->second;
				}
			} else {
				buffer[target++]=c;
			}
		} else {
			buffer[target++]=c;
		}
	}
	term.cut(target);
}

void CWmClient::NormalizeTerm(ppl7::String& term)
{
	if (term.isEmpty()) return;
	ppl7::WideString s=term;
	ppl7::WideString search, replace;
	s.lowerCase();
	replace.set(L" ");
	ReplaceIfExists(s, L" versus ", replace);
	ReplaceIfExists(s, L" pres. ", replace);
	ReplaceIfExists(s, L" presents ", replace);
	ReplaceIfExists(s, L" vs. ", replace);
	ReplaceIfExists(s, L" vs ", replace);
	ReplaceIfExists(s, L" ft. ", replace);
	ReplaceIfExists(s, L" ft ", replace);
	ReplaceIfExists(s, L" feat. ", replace);
	ReplaceIfExists(s, L" featuring ", replace);
	ReplaceIfExists(s, L" und ", replace);
	ReplaceIfExists(s, L" and ", replace);
	ReplaceIfExists(s, L" - ", replace);
	ReplaceIfExists(s, L" x ", replace);
	ReplaceIfExists(s, L" with ", replace);
	ReplaceIfExists(s, L" /\\ ", replace);
	ReplaceIfExists(s, L"DJ ", replace);
	ReplaceIfExists(s, L" ", replace);		// U+00A0, c2 a0, NO-BREAK SPACE
	s.trim();
	s.replace(L"  ", L" ");
	NormalizeLetters(letterReplacements, s);
	term=s;
}

int CWmClient::GetWords(const ppl7::String& str, ppl7::Array& words)
{
	words.clear();
	ppl7::String s=str;
	s.trim();
	if (s.isEmpty()) return 0;
	// Bestimmte Zeichen filtern wir raus
	NormalizeTerm(s);
	words.explode(s, " ", 0, true);
	return 1;
}
