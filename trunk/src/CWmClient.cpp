/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.15 $
 * $Date: 2011/05/24 18:55:11 $
 * $Id: CWmClient.cpp,v 1.15 2011/05/24 18:55:11 pafe Exp $
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


#ifdef _WIN32
#include <windows.h>
#endif


#include <locale.h>
#include "winmusik3.h"
#include "version.h"

#include "menue.h"
#include "src/editor/edit.h"
#include "search.h"
#include "firststart.h"
#include "langselect.h"
#include "splashscreen.h"
#include "coverprinter.h"
#include "src/playlist/playlist.h"
#include "updater.h"
#include "devicelist.h"
#include "src/searchlists/searchlists.h"
#include "src/searchlists/searchlistdialog.h"
#include "coverviewer.h"
#include <QMessageBox>
#include <QLocale>
#include <QDesktopWidget>
#include <ppl6-crypt.h>

CWmClient *wm_main=NULL;

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
	UpdateChecker=new CUpdateChecker(NULL,this);
	initLetterReplacements();
}

CWmClient::~CWmClient()
{
	Background.triggerShutdown();
	delete UpdateChecker;
	Hashes.Clear();
	ID3TagSaver.ThreadStop();
	CloseDatabase();
	if (CoverViewerWindow) {
		delete (CoverViewer*)CoverViewerWindow;
		CoverViewerWindow=NULL;
	}
	if (MainMenue) delete (Menue*)MainMenue;
	MainMenue=NULL;
	Background.shutdown();
	if (wmlog) delete wmlog;
	wmlog=NULL;
}

int CWmClient::Init(int argc, char **argv, QApplication *app)
{
	this->app=app;
	this->argc=argc;
	this->argv=argv;
	ppl6::CString Tmp;
	Tmp=ppl6::getargv(argc,argv,"-c");
	if (Tmp.NotEmpty()) {
		if (!conf.setConfigFile(Tmp)) return 0;
	}
	if (!InitStorage()) return 0;
	return 1;
}

int CWmClient::RaiseError()
{
	QString m=tr("The following error occured:");
	return RaiseError(NULL,m);
}

int CWmClient::RaiseError(QWidget *object, QString msg)
{
	ppluint32 err=ppl6::GetErrorCode();
	ppl6::CString descr=ppl6::GetError();
	ppl6::CString sub=ppl6::GetExtendedError();

	ppl6::CString m=msg;
	m.Trim();
	ppl6::CString a=tr("Errorcode");
	m.Concatf("\n\n%s: %u\n",(const char*)a,err);
	a=tr("Description");
	m.Concatf("%s: %s\n",(const char*)a,(const char*)descr);
	if (sub.Len()>0) {
		a=tr("Extended Description");
		m.Concatf("%s: %s\n",(const char*)a,(const char*)sub);
	}
	return QMessageBox::critical(object, tr("WinMusik"),
			m,
            QMessageBox::Ok);
}


int CWmClient::Start()
{
	ppl6::CString filename;
	ppl6::CString path=app->applicationDirPath();

	if (!conf.Load()) {
		if (!FirstStartDialog()) {
			ppl6::SetError(0);
			return 0;
		}
		if (!conf.Load()) {
			return 0;
		}
	}
	LoadTranslation();
	InitLogging();

	InitDataPath();

	/*
	ppl6::CString oldfile,newfile;
	oldfile=conf.DataPath;
	oldfile+="/data0001.wm3";
	newfile=conf.DataPath;
	newfile+="/data0001.neu";

	CWMFile f1,f2;
	if (!f1.Open(oldfile)) {
		return 0;
	}
	if (!f2.Open(newfile)) {
		return 0;
	}
	//f2.EnableCompression(true);
	CWMFile::CopyDatabase(f1,f2);
	return 0;
	*/


	if (!LoadDatabase()) {
		return 0;
	}

	RegExpCapture.load();

	//ImportDatabaseWM20();

	Mutex.Lock();
	if (MainMenue) delete (Menue*)MainMenue;
	MainMenue=NULL;
	Menue *w=new Menue(NULL,this);
	//w->setWindowFlags(Qt::Window|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint);
	w->show();
	MainMenue=w;
	Mutex.Unlock();
	Hashes.ThreadStart();
	if (conf.bCheckForUpdatesOnStartup) UpdateChecker->ThreadStart();
	return 1;
}

void CWmClient::InitLogging()
{
	Mutex.Lock();
	if (conf.bEnableDebug) {
		if (!wmlog) {
			wmlog=new ppl6::CLog;
		}
		wmlog->SetLogfile(ppl6::LOG::DEBUG,(const char*)conf.Logfile);
		wmlog->SetLogLevel(ppl6::LOG::DEBUG,conf.Debuglevel);
		wmlog->SetLogRotate((ppluint64)conf.LogfileSize*1024*1024,conf.LogfileGenerations);
		wmlog->Printf(ppl6::LOG::DEBUG,1,"CWmClient","InitLogging",__FILE__,__LINE__,
				"Logfile initialized, Debuglevel: %i, Maxsize: %i MB, Generations: %i",
				conf.Debuglevel,conf.LogfileSize,conf.LogfileGenerations);
	} else {
		if (wmlog) wmlog->Terminate();
	}

	Mutex.Unlock();

}

void CWmClient::UpdateMenue()
{
	if (MainMenue) ((Menue*)MainMenue)->UpdateMenue();
}


QByteArray CWmClient::GetGeometry(const char *name)
{
	ppl6::CString File=WM_APPNAME;
	File+="-Geometry";
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,WM_ORGANISATION,File);
	settings.beginGroup("geometry");
	return settings.value(name).toByteArray();
}

void CWmClient::SaveGeometry(const char *name, QByteArray Geometry)
{
	ppl6::CString File=WM_APPNAME;
	File+="-Geometry";
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,WM_ORGANISATION,File);
	settings.beginGroup("geometry");
	settings.setValue(name,Geometry);
}

void CWmClient::InitDataPath()
{
	Storage.Init(conf.DataPath);
}

int CWmClient::InitStorage()
/*!\brief Storage Klassen registrieren
 *
 * Diese Funktion sort dafür, dass alle bekannte Storage Klassen registriert werden. Sie wird
 * im Rahmen der Funktion CWmClient::Init aufgerufen.
 *
 * \returns Liefert gegenwärtig immer 1 zurück.
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
	return 1;
}

int CWmClient::SelectLanguage()
{
	conf.Locale=QLocale::system().name();
	conf.Locale.Cut("_");
	conf.Locale.LCase();
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
	ppl6::CString filename;
	//ppl6::CString path=app->applicationDirPath();
	//printf ("conf.Locale=%s\n",(const char*)conf.Locale);
	if (conf.Locale=="de") {
		setlocale(LC_CTYPE,"de_DE.UTF-8");
	} else {
		setlocale(LC_CTYPE,"en_US.UTF-8");
	}
	filename=":/translation/qt_"+conf.Locale;
	qtTranslator.load(filename);
	app->installTranslator(&qtTranslator);
	filename=":/translation/winmusik_"+conf.Locale;
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
	Mutex.Lock();
	// Alle Fenster aktualisieren
	if (MainMenue) ((Menue*)MainMenue)->ReloadTranslation();

	Edit *edit;
	EditorWindows.Reset();
	while ((edit=(Edit *)EditorWindows.GetNext())) {
		edit->ReloadTranslation();
	}
	Search *search;
	SearchWindows.Reset();
	while ((search=(Search *)SearchWindows.GetNext())) {
		search->ReloadTranslation();
	}
	CoverPrinter *cover;
	CoverPrinterWindows.Reset();
	while ((cover=(CoverPrinter *)CoverPrinterWindows.GetFirst())) {
		cover->ReloadTranslation();
	}
	Playlist *playlist;
	PlaylistWindows.Reset();
	while ((playlist=(Playlist *)PlaylistWindows.GetFirst())) {
		playlist->ReloadTranslation();
	}
	/*
	PlaylistEditor *playlisteditor;
	PlaylistEditWindows.Reset();
	while ((playlisteditor=(PlaylistEditor *)PlaylistEditWindows.GetFirst())) {
		playlisteditor->ReloadTranslation();
	}
	*/
	DeviceList *devicelist;
	DeviceListWindows.Reset();
	while ((devicelist=(DeviceList *)DeviceListWindows.GetFirst())) {
		devicelist->ReloadTranslation();
	}

	{
		Searchlists *list;
		SearchlistOverviewWindows.Reset();
		while ((list=(Searchlists *)SearchlistOverviewWindows.GetFirst())) {
			list->ReloadTranslation();
		}
	}

	{
		SearchlistDialog *list;
		SearchlistWindows.Reset();
		while ((list=(SearchlistDialog *)SearchlistWindows.GetFirst())) {
			list->ReloadTranslation();
		}
	}

	Mutex.Unlock();

}


int CWmClient::FirstStartDialog()
{
	// The user should select his preferred language first
	if (!SelectLanguage()) return 0;

	// If he has not aborted, we can load the translation files
	LoadTranslation();

	// Now we can show the "FirstStart" Dialog
	FirstStart s(NULL,this);
	s.show();
	int ret=s.exec();
	if (!ret) return 0;

	// If the user has not aborted, we try to load the configuration again
	if (!conf.Load()) {
		// If this fails something is terribly wrong
		ppl6::CString ee;
		ee=ppl6::Error();
		QString e=tr("Could not load the WinMusik configuration!");
		QString e2=ee;
		e+=e2;
		QMessageBox::critical(NULL, tr("WinMusik - Error"),e);
		return 0;
	}
	// Everything is fine
	return 1;
}


int CWmClient::isValidDataPath(ppl6::CString &Path)
{
	// Der Pfad muss existieren
	if (!ppl6::IsDir(Path)) return 0;
	ppl6::CString s;
	s.Setf("%s/winmusik.dat",(const char*)Path);
	if (!ppl6::FileExists(s)) return 0;
	// TODO: Es muss geprüft werden, ob es sich um eine gültige WinMusik-Daten-Datei handelt
	CWMFile ff;
	if (!ff.Open(s)) return 0;
	return 1;

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
	Mutex.Lock();
	Edit *edit;
	while ((edit=(Edit *)EditorWindows.GetFirst())) {
		Mutex.Unlock();
		delete edit;
		Mutex.Lock();
	}
	Search *search;
	while ((search=(Search *)SearchWindows.GetFirst())) {
		Mutex.Unlock();
		delete search;
		Mutex.Lock();
	}
	CoverPrinter *cover;
	while ((cover=(CoverPrinter *)CoverPrinterWindows.GetFirst())) {
		Mutex.Unlock();
		delete cover;
		Mutex.Lock();
	}
	Playlist *playlist;
	while ((playlist=(Playlist *)PlaylistWindows.GetFirst())) {
		Mutex.Unlock();
		delete playlist;
		Mutex.Lock();
	}
	/*
	PlaylistEditor *playlisteditor;
	while ((playlisteditor=(PlaylistEditor *)PlaylistEditWindows.GetFirst())) {
		Mutex.Unlock();
		delete playlisteditor;
		Mutex.Lock();
	}
	*/
	DeviceList *devicelist;
	while ((devicelist=(DeviceList *)DeviceListWindows.GetFirst())) {
		Mutex.Unlock();
		delete devicelist;
		Mutex.Lock();
	}
	Searchlists *searchlists;
	while ((searchlists=(Searchlists *)SearchlistOverviewWindows.GetFirst())) {
		Mutex.Unlock();
		delete searchlists;
		Mutex.Lock();
	}

	SearchlistDialog *searchlistdialog;
	while ((searchlistdialog=(SearchlistDialog *)SearchlistWindows.GetFirst())) {
		Mutex.Unlock();
		delete searchlistdialog;
		Mutex.Lock();
	}

	if (CoverViewerWindow) {
		Mutex.Unlock();
		delete (CoverViewer*) CoverViewerWindow;
		CoverViewerWindow=NULL;
		Mutex.Lock();
	}

	Mutex.Unlock();
	return 1;
}

void CWmClient::UpdateSearchlistOverviews()
{
	Mutex.Lock();
	Searchlists *list;
	SearchlistOverviewWindows.Reset();
	while ((list=(Searchlists *)SearchlistOverviewWindows.GetNext())) {
		list->Update();
	}
	Mutex.Unlock();
}

void CWmClient::MainMenueClosed()
{
	CloseDatabase();
	Mutex.Lock();
	MainMenue=NULL;
	Mutex.Unlock();
}

void CWmClient::OpenEditor(int devicetype,int deviceId, int page, int track)
{
	Edit *edit=new Edit((Menue*)MainMenue,this,devicetype);
	edit->setWindowFlags(Qt::Window);
	edit->show();
	Mutex.Lock();
	EditorWindows.Add(edit);
	Mutex.Unlock();
	if (deviceId>0) edit->OpenTrack(deviceId,page,track);
}

void CWmClient::OpenCoverPrinter()
{
	CoverPrinter *w=new CoverPrinter((Menue*)MainMenue,this);
	w->show();
	Mutex.Lock();
	CoverPrinterWindows.Add(w);
	Mutex.Unlock();
}

void CWmClient::CoverPrinterClosed(void *object)
{
	Mutex.Lock();
	CoverPrinterWindows.Delete(object);
	Mutex.Unlock();
}

void CWmClient::OpenPlaylistDialog()
{
	//printf ("Open Playlists\n");
	Playlist *w=new Playlist((Menue*)MainMenue,this);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.Lock();
	PlaylistWindows.Add(w);
	Mutex.Unlock();
}

void CWmClient::PlaylistClosed(void *object)
{
	Mutex.Lock();
	PlaylistWindows.Delete(object);
	Mutex.Unlock();

}

void CWmClient::OpenSearchlistOverview()
{
	//printf ("Open Playlists\n");
	Searchlists *w=new Searchlists((Menue*)MainMenue,this);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.Lock();
	SearchlistOverviewWindows.Add(w);
	Mutex.Unlock();
}

void CWmClient::SearchlistOverviewClosed(void *object)
{
	Mutex.Lock();
	SearchlistOverviewWindows.Delete(object);
	Mutex.Unlock();

}

void CWmClient::OpenSearchlistDialog(const ppl6::CString Filename)
{
	//printf ("Open Playlists\n");
	SearchlistDialog *w=new SearchlistDialog((Menue*)MainMenue,this,Filename);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.Lock();
	SearchlistWindows.Add(w);
	Mutex.Unlock();
}

void CWmClient::SearchlistDialogClosed(void *object)
{
	Mutex.Lock();
	SearchlistWindows.Delete(object);
	Mutex.Unlock();

}

QWidget *CWmClient::OpenSearch(const char *artist, const char *title)
{
	Search *w=new Search((Menue*)MainMenue,this);
	w->setWindowFlags(Qt::Window);
	w->show();
	Mutex.Lock();
	SearchWindows.Add(w);
	Mutex.Unlock();
	if (artist!=NULL || title!=NULL) w->FastSearch(artist,title);
	return w;
}

QWidget *CWmClient::OpenOrReuseSearch(QWidget *q, const char *artist, const char *title)
{
	// Wir schauen, ob es q in der Liste gibt
	Search *w;
	if (q) {
		Mutex.Lock();
		if (SearchWindows.HasObject(q)) {
			Mutex.Unlock();
			w=(Search*)q;
			w->setFocus();
			if (artist!=NULL || title!=NULL) w->FastSearch(artist,title);
			return w;
		}
		Mutex.Unlock();
	}
	w=new Search((Menue*)MainMenue,this);
	w->setWindowFlags(Qt::Window);
	w->show();
	SearchWindows.Add(w);
	Mutex.Unlock();
	if (artist!=NULL || title!=NULL) w->FastSearch(artist,title);
	return w;
}

void CWmClient::EditorClosed(void *object)
{
	Mutex.Lock();
	EditorWindows.Delete(object);
	Mutex.Unlock();
}

void CWmClient::CoverViewerClosed()
{
	Mutex.Lock();
	CoverViewerWindow=NULL;
	Mutex.Unlock();
}

void CWmClient::OpenCoverViewer(const QPixmap &pix)
{
	Mutex.Lock();
	if (CoverViewerWindow==NULL) {
		CoverViewerWindow=new CoverViewer(NULL,this);
		if (!CoverViewerWindow) {
			Mutex.Unlock();
			return;
		}
		((CoverViewer*)CoverViewerWindow)->show();
	}
	((CoverViewer*)CoverViewerWindow)->setCover(pix);
	Mutex.Unlock();
}

void CWmClient::UpdateCoverViewer(const QPixmap &pix)
{
	Mutex.Lock();
	if (CoverViewerWindow!=NULL) {
		((CoverViewer*)CoverViewerWindow)->setCover(pix);
	}
	Mutex.Unlock();
}

bool CWmClient::IsCoverViewerVisible() const
{
	if (CoverViewerWindow!=NULL) return true;
	return false;
}



void CWmClient::SearchClosed(void *object)
{
	Mutex.Lock();
	SearchWindows.Delete(object);
	Mutex.Unlock();
}

int CWmClient::LoadDatabase()
{
	SplashScreen *splash=NULL;
	if (conf.bShowSplashScreen) {
		splash=new SplashScreen();
		//splash->setWindowFlags(Qt::Dialog|Qt::SplashScreen|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
		splash->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
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
	DeviceList *edit=new DeviceList((Menue*)MainMenue,this,devicetype);
	edit->setWindowFlags(Qt::Window);
	edit->show();
	Mutex.Lock();
	DeviceListWindows.Add(edit);
	Mutex.Unlock();
}

void CWmClient::DeviceListClosed(void *object)
{
	Mutex.Lock();
	DeviceListWindows.Delete(object);
	Mutex.Unlock();

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

ppl6::CString CWmClient::GetDeviceName(int DeviceType)
{
	ppl6::CString s;
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

ppl6::CString CWmClient::GetDeviceNameShort(ppluint8 DeviceType)
{
	ppl6::CString s;
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


int CWmClient::LoadDevice(ppluint8 DeviceType, ppluint32 DeviceId, DataDevice *data)
{
	data->Clear();
	DeviceStore.Update(DeviceType,DeviceId);
	return DeviceStore.GetCopy(DeviceType,DeviceId,data);
}

CTrackList *CWmClient::GetTracklist(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page)
{
	return TrackStore.GetTracklist(Device,DeviceId,Page);
}

DataTitle *CWmClient::GetTitle(ppluint32 TitleId)
{
	return TitleStore.Get(TitleId);
}

DataVersion *CWmClient::GetVersion(ppluint32 Id)
{
	return (DataVersion *)VersionStore.Get(Id);
}

const char *CWmClient::GetVersionText(ppluint32 Id)
{
	DataVersion *v=(DataVersion*)VersionStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}

DataGenre *CWmClient::GetGenre(ppluint32 Id)
{
	return (DataGenre *)GenreStore.Get(Id);
}

const char *CWmClient::GetGenreText(ppluint32 Id)
{
	DataGenre *v=(DataGenre*)GenreStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}

DataLabel *CWmClient::GetLabel(ppluint32 Id)
{
	return (DataLabel *)LabelStore.Get(Id);
}

const char *CWmClient::GetLabelText(ppluint32 Id)
{
	DataLabel *v=(DataLabel*)LabelStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}


DataRecordSource *CWmClient::GetRecordSource(ppluint32 Id)
{
	return (DataRecordSource *)RecordSourceStore.Get(Id);
}

const char *CWmClient::GetRecordSourceText(ppluint32 Id)
{
	DataRecordSource *v=(DataRecordSource*)RecordSourceStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}


DataRecordDevice *CWmClient::GetRecordDevice(ppluint32 Id)
{
	return (DataRecordDevice *)RecordDeviceStore.Get(Id);
}

const char *CWmClient::GetRecordDeviceText(ppluint32 Id)
{
	DataRecordDevice *v=(DataRecordDevice*)RecordDeviceStore.Get(Id);
	if (v) return v->Value;
	return Str_Unknown;
}

QString CWmClient::Unknown()
{
	return Str_Unknown;
}


ppl6::CString CWmClient::MP3Filename(ppluint32 DeviceId, ppluint8 Page, ppluint32 Track)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","MP3Filename",__FILE__,__LINE__,"MP3-Datei suchen: DeviceId=%u, Page=%u, Track=%u",DeviceId,Page,Track);
	ppl6::CString Path, Pattern;
	ppl6::CString MP3Path=conf.DevicePath[7];

	if (MP3Path.IsEmpty()) return Path;
	Path=MP3Path;
	Path.RTrim("/");
	Path.RTrim("\\");
	Path.Concatf("/%02u/%03u/",(ppluint32)(DeviceId/100),DeviceId);
	Pattern.Setf("%03u-*.mp3",Track);
	ppl6::CDir Dir;
	ppl6::CDirEntry *de;
	if (Dir.Open(Path,ppl6::CDir::Sort_Filename_IgnoreCase)) {
		if ((de=Dir.GetFirstPattern(Pattern,true))) {
			Path=de->File;
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","MP3Filename",__FILE__,__LINE__,"Gefunden: %s",(const char*)Path);
			return Path;
		}

	}
	Pattern.Setf("%03u.mp3",Track);
	if ((de=Dir.GetFirstPattern(Pattern,true))) {
		Path=de->File;
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","MP3Filename",__FILE__,__LINE__,"Gefunden: %s",(const char*)Path);
		return Path;
	}
	Pattern.Setf("/^%03u\\-.*\\.mp3$/i8",Track);
	if ((de=Dir.GetFirstRegExp(Pattern))) {
		Path=de->File;
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","MP3Filename",__FILE__,__LINE__,"Gefunden: %s",(const char*)Path);
		return Path;
	}



	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","MP3Filename",__FILE__,__LINE__,"Nicht gefunden");
	Path.Clear();
	return Path;
}

ppl6::CDirEntry CWmClient::StatMP3File(ppluint32 DeviceId, ppluint8 Page, ppluint32 Track)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","StatMP3File",__FILE__,__LINE__,"MP3-Datei suchen: DeviceId=%u, Page=%u, Track=%u",DeviceId,Page,Track);
	ppl6::CDirEntry ret;
	ppl6::CString Path, Pattern;
	ppl6::CString MP3Path=conf.DevicePath[7];

	if (MP3Path.IsEmpty()) return ret;
	Path=MP3Path;
	Path.RTrim("/");
	Path.RTrim("\\");
	Path.Concatf("/%02u/%03u/",(ppluint32)(DeviceId/100),DeviceId);
	Pattern.Setf("%03u-*.mp3",Track);
	ppl6::CDir Dir;
	ppl6::CDirEntry *de;
	if (Dir.Open(Path,ppl6::CDir::Sort_Filename_IgnoreCase)) {
		if ((de=Dir.GetFirstPattern(Pattern,true))) {
			Path=de->File;
			ret=*de;
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","StatMP3File",__FILE__,__LINE__,"Gefunden: %s",(const char*)Path);
			return ret;
		}

	}
	Pattern.Setf("%03u.mp3",Track);
	if ((de=Dir.GetFirstPattern(Pattern,true))) {
		Path=de->File;
		ret=*de;
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","StatMP3File",__FILE__,__LINE__,"Gefunden: %s",(const char*)Path);
		return ret;
	}
	Pattern.Setf("/^%03u\\-.*\\.mp3$/i8",Track);
	if ((de=Dir.GetFirstRegExp(Pattern))) {
		Path=de->File;
		ret=*de;
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","StatMP3File",__FILE__,__LINE__,"Gefunden: %s",(const char*)Path);
		return ret;
	}

	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","StatMP3File",__FILE__,__LINE__,"Nicht gefunden");
	return ret;
}

ppl6::CString CWmClient::NextMP3File(ppluint32 DeviceId, ppluint8 Page, ppluint32 Track)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","NextMP3File",__FILE__,__LINE__,"Nächste MP3-Datei suchen: DeviceId=%u, Page=%u, Track=%u",DeviceId,Page,Track);
	ppl6::CString Path, Filename, Pattern;
	ppl6::CString MP3Path=conf.DevicePath[7];

	if (MP3Path.IsEmpty()) return Path;
	Path=MP3Path;
	Path.RTrim("/");
	Path.RTrim("\\");
	Path.Concatf("/%02u/%03u/",(ppluint32)(DeviceId/100),DeviceId);
	Pattern.Setf("*.mp3");
	ppl6::CDir Dir;
	ppl6::CDirEntry *entry;
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"CWMClient","NextMP3File",__FILE__,__LINE__,"Öffne Verzeichnis: %s",(const char*)Path);
	if (Dir.Open(Path,ppl6::CDir::Sort_Filename_IgnoreCase)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"CWMClient","NextMP3File",__FILE__,__LINE__,"%i Dateien vorhanden, suche nach Pattern...",Dir.Num());
		while ((entry=Dir.GetNextPattern(Pattern,true))) {
			Filename=entry->Filename;
			// Der Dateiname darf nicht mit drei Ziffern und Bindestrich beginnen
			if (!Filename.PregMatch("/^[0-9]{3}\\-.*/")) {
				// Muss aber mit .mp3 enden und Daten enthalten (beim Download per Firefox wird eine leere Datei als Platzhalter angelegt)
				if (Filename.PregMatch("/^.*\\.mp3$/i")==true && entry->Size>256) {
					// Sehr schön. Nun benennen wir die Datei um und hängen die Track-Nummer davor
					if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,8,"CWMClient","NextMP3File",__FILE__,__LINE__,"Datei passt auf Pattern: %s",(const char*)Filename);
					ppl6::CString newFilename;
					newFilename.Setf("%s/%03u-%s",(const char*)entry->Path,Track,(const char*)Filename);
					if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","NextMP3File",__FILE__,__LINE__,"Rename %s => %s",(const char*)entry->File, (const char*)newFilename);
					// Wir versuchen sie umzubenennen
					if (ppl6::CFile::RenameFile(entry->File,newFilename)) {
						if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,3,"CWMClient","NextMP3File",__FILE__,__LINE__,"Erfolgreich. Datei: %s",(const char*)newFilename);
						return newFilename;
					}
					if (wmlog) wmlog->LogError();
					if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,8,"CWMClient","NextMP3File",__FILE__,__LINE__,"Error, versuche nächste Datei");
					// Fehlgeschlagen, vielleicht gibt's ja noch andere Dateien
				}
			}
		}
	}
	// Nichts passendes gefunden, wir geben einen leeren String zurück
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,8,"CWMClient","NextMP3File",__FILE__,__LINE__,"Nichts passendes gefunden");
	Path.Clear();
	return Path;
}

ppl6::CString CWmClient::NormalizeFilename(ppluint32 DeviceId, ppluint8 Page, ppluint32 Track, DataTitle &Ti)
{
	ppl6::CString Filename;
	ppl6::CString MP3Path=conf.DevicePath[7];

	if (MP3Path.IsEmpty()) return Filename;
	Filename=MP3Path;
	Filename.RTrim("/");
	Filename.RTrim("\\");
	Filename.Concatf("/%02u/%03u/",(ppluint32)(DeviceId/100),DeviceId,Track);
	ppl6::CString Tmp;
	Tmp.Setf("%03u-",Track);
	if (Ti.Artist) Tmp+=Ti.Artist;
	else Tmp+="unknown";
	Tmp+=" - ";
	if (Ti.Title) Tmp+=Ti.Title;
	else Tmp+="unknown";
	// Version holen
	ppl6::CString Version=GetVersionText(Ti.VersionId);
	if (Version!="Single") {
		Tmp+=" ("+Version+")";
	}
	Tmp+=".mp3";
	// Problematische Zeichen rausfiltern
	Tmp.Replace("/"," ");
	Tmp.Replace("\\"," ");
	Tmp.Replace("?"," ");
	Tmp.Replace("*"," ");
	Tmp.Replace(":","-");
	Tmp.Replace("&","+");
	Tmp.Replace("ß","ss");
	Tmp.Replace("'","'");
	Tmp.Replace("`","'");
	Tmp.Replace("\"","''");
	Tmp.Trim();
	// Wir müssen mit Unicode arbeiten
	ppl6::CWString w=Tmp;
	if (w.Len()>conf.MaxFilenameLength) {
		w.Cut(conf.MaxFilenameLength-4);
		w+=".mp3";
		Tmp=w;
	}


	Filename+=Tmp;
	return Filename;
}

int CWmClient::SaveID3Tags(ppluint32 DeviceId, ppluint8 Page, ppluint32 Track, DataTitle &Ti, const ppl6::CString &Filename)
{
	ppl6::CString InternalFilename;
	ppl6::CString Tmp;
	if (Filename.NotEmpty()) InternalFilename=Filename;
	else InternalFilename=MP3Filename(DeviceId, Page, Track);
	if (InternalFilename.IsEmpty()) {
		Tmp=tr("Track: %i");
		ppl6::SetError(20022,Tmp,Track);
		return 0;
	}

	ppl6::CAssocArray Job;

	if (conf.bNormalizeFilename) {
		// Unter Windows würde ein rename an dieser Stelle fehlschlagen, wenn die Datei
		// bereits geöffnet ist (z.B. im MP3-Player). Daher geben wir die Aufgabe an den
		// TagSaver
		Tmp=NormalizeFilename(DeviceId,Page,Track,Ti);
		Job.Set("renamefile",Tmp);
	}

	ID3TagSaver.SetPaddingSize(conf.ID3v2Padding);
	ID3TagSaver.SetRetryIntervall(conf.TagSaverRetryIntervall);

	ppl6::CString comment,version;
	version=GetVersionText(Ti.VersionId);

	Job.Set("artist",Ti.Artist);
	if (ppl6::LCase(version)!="single")	Job.Setf("title","%s (%s)",Ti.Title,(const char*)version);
	else Job.Setf("title","%s",Ti.Title);
	Job.Set("album",Ti.Album);
	comment=Ti.Remarks;
	//if (comment.Len()>0) comment+=" - ";
	//comment+=version;
	Job.Set("comment",comment);
	Job.Set("version",version);
	Job.Setf("track","%u",Track);
	if (Ti.BPM>0) Job.Setf("bpm","%u",Ti.BPM);
	else Job.Setf("bpm","");
	Job.Set("key",Ti.getKeyName());
	Job.Set("genre",GetGenreText(Ti.GenreId));
	Job.Set("publisher",GetLabelText(Ti.LabelId));
	Tmp.Setf("%u",Ti.ReleaseDate);
	Tmp.Cut(4);
	Job.Set("year",Tmp);
	ID3TagSaver.Add(InternalFilename,&Job,conf.bRemoveOriginalId3Tags,
			conf.bWriteId3v1,conf.bWriteId3v2);
	return 1;
}

int CWmClient::SaveOriginalMP3Info(ppl6::CString &File, DataOimp &oimp)
{
	ppl6::CFile ff;
	ppl6::CString Tmp;
	if (!ff.Open(File,"rb")) return 0;
	if (ff.Size()<256) {
		ppl6::SetError(20017,"%s",(const char*)File);
		return 0;
	}
	// Alte Daten löschen
	oimp.Filename=ppl6::GetFilename(File);
	memset(&oimp.ID3v1,0,sizeof(ID3TAG));
	oimp.ID3v2.Clear();
	// ID3v1-Tag einlesen falls vorhanden
	const char *buffer=ff.Map(ff.Lof()-128,128);
	if (buffer[0]=='T' && buffer[1]=='A' && buffer[2]=='G') {
		ID3TAG *tag=(ID3TAG*)buffer;
		Tmp.Strncpy(tag->Artist,30);
		Tmp.Trim();
		oimp.ID3v1.Set("artist",Tmp);
		Tmp.Strncpy(tag->SongName,30);
		Tmp.Trim();
		oimp.ID3v1.Set("title",Tmp);

		Tmp.Strncpy(tag->Album,30);
		Tmp.Trim();
		oimp.ID3v1.Set("album",Tmp);

		Tmp.Strncpy(tag->Year,4);
		Tmp.Trim();
		oimp.ID3v1.Set("year",Tmp);

		Tmp.Strncpy(tag->Comment,29);
		Tmp.Trim();
		oimp.ID3v1.Set("comment",Tmp);

		Tmp=ppl6::GetID3GenreName(tag->Genre);
		oimp.ID3v1.Set("genre",Tmp);

	}
	ppl6::CID3Tag Tag;
	if (Tag.Load(&File)) {
		oimp.ID3v2.Set("artist",Tag.GetArtist());
		oimp.ID3v2.Set("title",Tag.GetTitle());
		oimp.ID3v2.Set("album",Tag.GetAlbum());
		oimp.ID3v2.Set("year",Tag.GetYear());
		oimp.ID3v2.Set("comment",Tag.GetComment());
		oimp.ID3v2.Set("genre",Tag.GetGenre());
		oimp.ID3v2.Set("remixer",Tag.GetRemixer());
	}
	/*
	printf ("Folgende Daten werden gespeichert:\n");
	printf ("Filename: %s\n",(char*)oimp.Filename);
	printf ("ID3v1-Tag:\n");
	oimp.ID3v1.List("ID3v1");
	printf ("ID3v2-Tag:\n");
	oimp.ID3v2.List("ID3v2");
	*/
	return OimpDataStore.Put(&oimp);
}



int CWmClient::WritePlaylist(ppluint32 DeviceId, ppluint8 Page, CTrackList *list, DataDevice *device)
{
	ppl6::CString Path, Filename, Tmp, Minuten, FilePath;
	DataTrack *track;
	DataTitle *Ti;
	if (!DeviceId) {
		ppl6::SetError(20040);
		return 0;
	}
	if (!list) {
		ppl6::SetError(20041);
		return 0;
	}
	ppl6::CString MP3Path=conf.DevicePath[7];
	if (MP3Path.IsEmpty()) {
		ppl6::SetError(20042);
		return 0;
	}
	Minuten=tr("min","Minutes in Tracklisting of Playlist");
	Path=MP3Path;
	Path.RTrim("/");
	Path.RTrim("\\");
	Path.Concatf("/%02u/%03u",(ppluint32)(DeviceId/100),DeviceId);


	ppl6::CFile m3u;
	if (!m3u.Openf("%s/000index.m3u","wb",(const char*)Path)) return 0;

	ppl6::CFile pls;
	if (!pls.Openf("%s/000index.pls","wb",(const char*)Path)) return 0;

	ppl6::CFile txt;
	if (!txt.Openf("%s/000index.txt","wb",(const char*)Path)) return 0;

	ppl6::CFile xspf;
	if (!xspf.Openf("%s/000index.xspf","wb",(const char*)Path)) return 0;

	m3u.Puts("#EXTM3U\n");
	pls.Puts("[playlist]\n");

	Tmp=tr("Tracklisting MP3-Medium","Subject of Playlist");
	Tmp.Concatf(" %u\r\n",DeviceId);
	txt.Puts(Tmp);
	if (device!=NULL && device->Title!=NULL) {
		Tmp=device->Title;
		Tmp.Trim();
		txt.Puts(Tmp);
		txt.Puts("\r\n");
	}
	txt.Puts("\r\n");

	xspf.Puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	xspf.Puts("<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">\n");
	Tmp=tr("Playlist created by WinMusik");
	Tmp.Trim();
	Tmp.Concatf(" %s",WM_VERSION);
	xspf.Putsf("<creator>%s</creator>\n",(const char*)Tmp);
	xspf.Puts("<trackList>\n");
	int count=0;
	int min=list->GetMin();
	int max=list->GetMax();
	for (int i=min;i<=max;i++) {
		track=list->Get(i);
		if (track) {
			Ti=GetTitle(track->TitleId);
			if (Ti) {
				count++;
				if (Ti->Artist) Tmp=Ti->Artist;
				else Tmp="unknown";
				Tmp+=" - ";
				if (Ti->Title) Tmp+=Ti->Title;
				else Tmp+="unknown";
				Tmp+=" (";
				// Version holen
				Tmp+=GetVersionText(Ti->VersionId);
				Tmp+=")";

				FilePath=MP3Filename(DeviceId,Page,i);
				Filename=ppl6::GetFilename(FilePath);
				m3u.Putsf("#EXTINF:%u,%s\n",Ti->Length,(const char*)Tmp);
				m3u.Putsf("%s\n",(const char*)Filename);

				pls.Putsf("File%i=%s\n",count,(const char*)Filename);
				pls.Putsf("Title%i=%s\n",count,(const char*)Tmp);
				pls.Putsf("Length%i=%u\n",count,Ti->Length);

				xspf.Putsf("  <track>\n");
				xspf.Putsf("     <trackNum>%u</trackNum>\n",i);
				ppl6::CString TmpTxt=Tmp;
				TmpTxt.Chop(1);
				txt.Putsf("%3u. %s, %0i:%02i %s)\r\n",i,(const char*)TmpTxt,(int)(Ti->Length/60),Ti->Length%60,(const char*)Minuten);

				//Tmp=ppl6::UrlEncode(FilePath);
				//Tmp.Replace("+","%20");
				xspf.Putsf("     <location>file://%s</location>\n",(const char*)ppl6::EscapeHTMLTags(FilePath));
				if (Ti->Artist) xspf.Putsf("     <creator>%s</creator>\n",(const char*)ppl6::EscapeHTMLTags(Ti->Artist));
				if (Ti->Title) xspf.Putsf("     <title>%s - %s (%s)</title>\n",
						(const char*)ppl6::EscapeHTMLTags(Ti->Artist),
						(const char*)ppl6::EscapeHTMLTags(Ti->Title),
						(const char*)ppl6::EscapeHTMLTags(GetVersionText(Ti->VersionId)));
				//xspf.Putsf("     <annotation>%s</annotation>\n",GetVersionText(Ti->VersionId));
				if (Ti->Album) xspf.Putsf("     <album>%s</album>\n",(const char*)ppl6::EscapeHTMLTags(Ti->Album));
				xspf.Putsf("     <duration>%u</duration>\n",Ti->Length*1000);  // Bringt VLC zum Absturz
				xspf.Putsf("  </track>\n");


			}
		}
	}
	pls.Putsf("NumberOfEntries=%i\nVersion=2\n",count);

	xspf.Putsf("</trackList>\n</playlist>\n");


	return 1;
}

int CWmClient::UpdateID3Tags(ppluint32 DeviceId, ppluint8 Page, CTrackList *list)
{
	ppl6::CString Path, Filename, Tmp, Minuten;
	DataTrack *track;
	DataTitle *Ti;
	if (!DeviceId) {
		ppl6::SetError(20040);
		return 0;
	}
	if (!list) {
		ppl6::SetError(20041);
		return 0;
	}
	ppl6::CString MP3Path=conf.DevicePath[7];
	if (MP3Path.IsEmpty()) {
		ppl6::SetError(20042);
		return 0;
	}
	int min=list->GetMin();
	int max=list->GetMax();
	for (int i=min;i<=max;i++) {
		track=list->Get(i);
		if (track) {
			Ti=GetTitle(track->TitleId);
			if (Ti) {
				if (!SaveID3Tags(DeviceId, Page, track->Track, *Ti)) return 0;
			}
		}
	}
	return 1;
}


int CWmClient::PlayFile(ppl6::CString &Filename)
{
#ifdef _WIN32
	ppl6::CWString f=Filename;
	// Windows mag keine Vorwärts-Slashes
	f.Replace("/","\\");
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CWMClient","PlayFile",__FILE__,__LINE__,"Datei abspielen: %s",(const char*)f);
	if (conf.MP3Player.IsEmpty()) {
		ShellExecuteW(NULL,L"open", (const wchar_t*)f,
			L"",L"", SW_SHOWNORMAL);
	} else {
		f="\""+f;
		f+="\"";
		ppl6::CWString prog=conf.MP3Player;
		ShellExecuteW(NULL,L"open", (const wchar_t*)prog,(const wchar_t*)f,
			L"", SW_SHOWNORMAL);
	}
#else
	if (conf.MP3Player.IsEmpty()) {
		QMessageBox::warning(NULL, tr("WinMusik: Attention"),
				tr("There is no MP3 player specified.\nPlease go to preferences and open the MP3 page. There you can specify your favorite player."),
				QMessageBox::Ok,QMessageBox::Ok);
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CWMClient","PlayFile",__FILE__,__LINE__,"Datei abspielen: %s",(const char*)Filename);
	ppl6::CString cmd;
	cmd.Setf("%s \"%s\" > /dev/null 2>&1 &",(const char*)conf.MP3Player,(const char*)Filename);
	system ((const char*)cmd);
#endif
	return 1;
}

int CWmClient::TrashMP3File(ppluint32 DeviceId, ppluint8 Page, ppluint32 Track)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Datei löschen: DeviceId=%u, Page=%u, Track=%u",DeviceId,Page,Track);
	ppl6::CString MP3Path=conf.DevicePath[7];
	if (MP3Path.IsEmpty()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,6,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Kein MP3-Pfad angegeben");
		return 0;
	}
	ppl6::CString Path;
	Path=MP3Path;
	Path.RTrim("/");
	Path.RTrim("\\");
	Path.Concatf("/Trash");
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Pfad für gelöschte Dateien: %s",(const char*)Path);
	if (!ppl6::CFile::Exists(Path)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Pfad existiert nicht und wird angelegt");
		if (!ppl6::MkDir(Path,1)) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Anlegen fehlgeschlagen");
			if (wmlog) wmlog->LogError("CWMClient","TrashMP3File",__FILE__,__LINE__);
			return 0;
		}
		if (!ppl6::CFile::Exists(Path)) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Pfad wirde zwar angelegt, existiert aber trotzdem nicht");
			return 0;
		}
	}
	if (!ppl6::IsDir(Path)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Pfad ist kein Verzeichnis: %s",(const char*)Path);
		return 0;
	}
	ppl6::CString old=MP3Filename(DeviceId,Page,Track);
	if (old.IsEmpty()) return 1;
	ppl6::CString file=Path;
	file.Concatf("/%03u-%02u-",DeviceId,Page);
	file+=ppl6::GetFilename(old);
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Umbenennung: %s ==> %s",(const char*)old, (const char*)file);
	if (!ppl6::CFile::RenameFile(old,file)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Rename fehlgeschlagen");
		if (wmlog) wmlog->LogError("CWMClient","TrashMP3File",__FILE__,__LINE__);
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CWMClient","TrashMP3File",__FILE__,__LINE__,"Erfolgreich");
	return 1;
}

int CWmClient::RenameMP3File(ppluint32 DeviceId, ppluint8 Page, ppluint32 OldTrack, ppluint32 NewTrack)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CWMClient","RenameMP3File",__FILE__,__LINE__,"Datei umbenennen: DeviceId=%u, Page=%u, OldTrack=%u, NewTrack=%u",DeviceId,Page,OldTrack,NewTrack);
	ppl6::CString MP3Path=conf.DevicePath[7];
	if (MP3Path.IsEmpty()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,6,"CWMClient","RenameMP3File",__FILE__,__LINE__,"Kein MP3-Pfad angegeben");
		return 0;
	}
	ppl6::CString Old=MP3Filename(DeviceId,Page,OldTrack);
	if (Old.IsEmpty()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,6,"CWMClient","RenameMP3File",__FILE__,__LINE__,"Alter Track %u ist nicht vorhanden",OldTrack);
		return 1;
	}

	ppl6::CString Path=ppl6::GetPath(Old);
	ppl6::CString Filename=ppl6::GetFilename(Old);
	Filename=Filename.Mid(4);
	ppl6::CString NewFile;
	NewFile.Setf("%s/%03u-%s",(const char *)Path,NewTrack,(const char*)Filename);
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","RenameMP3File",__FILE__,__LINE__,"Umbenennung: %s ==> %s",(const char*)Old, (const char*)NewFile);
	if (!ppl6::CFile::RenameFile(Old,NewFile)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"CWMClient","RenameMP3File",__FILE__,__LINE__,"Rename fehlgeschlagen");
		if (wmlog) wmlog->LogError("CWMClient","RenameMP3File",__FILE__,__LINE__);
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CWMClient","RenameMP3File",__FILE__,__LINE__,"Erfolgreich");
	return 1;
}

int CWmClient::CurlTalk(ppl6::CCurl &Curl, ppl6::CAssocArray &cmd, ppl6::CAssocArray &answer)
{
	Curl.AddPostVar(cmd,"cmd");
	if (!Curl.Post()) return 0;
	ppl6::CString result;
	result=Curl.GetResultBuffer();
	result+="\n";
	answer.Clear();
	answer.CreateFromTemplate((const char*)result,"\n",":","\n",true);
	result=answer.Get("Result");
	if (result=="SUCCESS") return 1;
	result=answer.Get("Errortext");
	ppl6::CString code=answer.Get("Errorcode");
	SetError(code.ToInt(),result);
	return 0;
}

ppl6::CString CWmClient::GetOperatingSystem()
{
	ppl6::CString ret;
	ppl6::CString ByteOrder="unknown;";
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
	ByteOrder="BE;";
#endif
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	ByteOrder="LE;";
#endif
	ppl6::CString Bits;
	Bits.Setf("%u;",QT_POINTER_SIZE*8);

#ifdef Q_WS_WIN
	ret="Win;";
	ret+=Bits;
	ret+=ByteOrder;
	switch (QSysInfo::WindowsVersion) {
		case QSysInfo::WV_32s: ret+="Windows 3.x;";	break;
		case QSysInfo::WV_95: ret+="Windows 95;";	break;
		case QSysInfo::WV_98: ret+="Windows 98;";	break;
		case QSysInfo::WV_Me: ret+="Windows Me;";	break;
		case QSysInfo::WV_NT: ret+="Windows NT;";	break;
		case QSysInfo::WV_2000: ret+="Windows 2000;";	break;
		case QSysInfo::WV_XP: ret+="Windows XP";	break;
		case QSysInfo::WV_2003: ret+="Windows Server 2003";	break;
		case QSysInfo::WV_VISTA: ret+="Windows Vista";	break;
		case 0x0090: ret+="Windows 7";	break;
		case QSysInfo::WV_CE: ret+="Windows CE";	break;
		case QSysInfo::WV_CENET: ret+="Windows CE .NET";	break;
		case QSysInfo::WV_CE_5: ret+="Windows CE 5.x";	break;
		case QSysInfo::WV_CE_6: ret+="Windows CE 6.x";	break;
		default:
			ret+="unknown;";
	}

#elif defined Q_WS_MAC
	ret="Mac;";
	ret+=Bits;
	ret+=ByteOrder;
	switch (QSysInfo::WindowsVersion) {
		case QSysInfo::MV_9: ret+="Mac OS 9;";	break;
		case QSysInfo::MV_10_0: ret+="Mac OS X 10.0;";	break;
		case QSysInfo::MV_10_1: ret+="Mac OS X 10.1;";	break;
		case QSysInfo::MV_10_2: ret+="Mac OS X 10.2;";	break;
		case QSysInfo::MV_10_3: ret+="Mac OS X 10.3;";	break;
		case QSysInfo::MV_10_4: ret+="Mac OS X 10.4;";	break;
		case QSysInfo::MV_10_5: ret+="Mac OS X 10.5;";	break;
		default:
			ret+="unknown;";
	}

#elif defined Q_OS_UNIX
	ret="Unix;";
	ret+=Bits;
	ret+=ByteOrder;
	ppl6::CFile ff;
	ppl6::CString Buffer;
	Buffer="";
	if (ff.Popen("uname -s","r")) {
		if (ff.Gets(Buffer,2048)) {
			Buffer.Trim();
		}
	}
	ret+=Buffer;
	ret+=";";
	Buffer="";
	if (ff.Popen("uname -m","r")) {
		if (ff.Gets(Buffer,2048)) {
			Buffer.Trim();
		}
	}
	ret+=Buffer;
	ret+=";";
	Buffer="";
	if (ff.Popen("uname -r","r")) {
		if (ff.Gets(Buffer,2048)) {
			Buffer.Trim();
		}
	}
	ret+=Buffer;
	ret+=";";
	Buffer="";
	if (ff.Popen("uname -v","r")) {
		if (ff.Gets(Buffer,2048)) {
			Buffer.Trim();
		}
	}
	ret+=Buffer;
	ret+=";";


#else	// Unknown system
	ret="unknown;";
	ret+=Bits;
	ret+=ByteOrder;
	ret+="unknown;";
#endif
	return ret;
}

/*
static void SubstituteLetter(ppl6::CWString &Text, const ppl6::CWString &Letters, const ppl6::CWString &Replacement)
{
	wchar_t letter;
	for (int i=0;i<Text.Length();i++) {
		letter=Text[i];
		if (Letters.In)
	}
}
*/

void CWmClient::initLetterReplacements()
{
	addLetterReplacement(ppl6::CWString(L"&+()_,"),L' ');
	addLetterReplacement(ppl6::CWString(L".:''`"),0);
	addLetterReplacement(ppl6::CWString(L"°"),L'o');
	addLetterReplacement(ppl6::CWString(L"àáâãäåāăąæ"),L'a');
	addLetterReplacement(ppl6::CWString(L"þ"),L'b');
	addLetterReplacement(ppl6::CWString(L"çćĉċč"),L'c');
	addLetterReplacement(ppl6::CWString(L"ďđ"),L'd');
	addLetterReplacement(ppl6::CWString(L"èéêëēĕėęě"),L'e');
	addLetterReplacement(ppl6::CWString(L"ĝğġģ"),L'g');
	addLetterReplacement(ppl6::CWString(L"ĥħ"),L'h');
	addLetterReplacement(ppl6::CWString(L"ìíîïĩīĭįı"),L'i');
	addLetterReplacement(ppl6::CWString(L"ĵ"),L'j');
	addLetterReplacement(ppl6::CWString(L"ķĸ"),L'k');
	addLetterReplacement(ppl6::CWString(L"ĺļľŀł"),L'l');
	addLetterReplacement(ppl6::CWString(L"ñńņňŉŋ"),L'n');
	addLetterReplacement(ppl6::CWString(L"òóôõöøōŏőœ"),L'o');
	addLetterReplacement(ppl6::CWString(L"ŕŗř"),L'r');
	addLetterReplacement(ppl6::CWString(L"śŝşš"),L's');
	addLetterReplacement(ppl6::CWString(L"ţťŧ"),L't');
	addLetterReplacement(ppl6::CWString(L"ùúûüũūŭůűų"),L'u');
	addLetterReplacement(ppl6::CWString(L"ŵ"),L'w');
	addLetterReplacement(ppl6::CWString(L"ýÿŷ"),L'y');
	addLetterReplacement(ppl6::CWString(L"źżž"),L'z');
}

void CWmClient::addLetterReplacement(wchar_t letter, wchar_t replacement)
{
	letterReplacements[letter]=replacement;
}

void CWmClient::addLetterReplacement(const ppl6::CWString &letters, wchar_t replacement)
{
	for (size_t i=0;i<letters.Size();i++) {
		addLetterReplacement(letters[i],replacement);
	}
}

static inline void ReplaceIfExists(ppl6::CWString &s, const wchar_t *search, const ppl6::CWString &replace)
{
	if (s.IsEmpty()) return;
	wchar_t *buffer=(wchar_t*)s.GetBuffer();
	if (!buffer) return;
	if (wcsstr(buffer,search)) {
		ppl6::CWString ss;
		ss.Set(search);
		s.Replace(ss,replace);
	}
}

void CWmClient::NormalizeTerm(ppl6::CString &term)
{
	if (term.IsEmpty()) return;
	ppl6::CWString s=term;
	ppl6::CWString search,replace;
	s.LCase();
	replace.Set(L" ");
	wchar_t *buffer;
	ReplaceIfExists(s,L" versus ",replace);
	ReplaceIfExists(s,L" pres. ",replace);
	ReplaceIfExists(s,L" presents ",replace);
	ReplaceIfExists(s,L" vs. ",replace);
	ReplaceIfExists(s,L" ft. ",replace);
	ReplaceIfExists(s,L" feat. ",replace);
	ReplaceIfExists(s,L" featuring ",replace);
	ReplaceIfExists(s,L" und ",replace);
	ReplaceIfExists(s,L" and ",replace);
	ReplaceIfExists(s,L" - ",replace);
	ReplaceIfExists(s,L"DJ ",replace);
	s.Trim();
	std::map<wchar_t,wchar_t>::iterator it;
	size_t ss=s.Len();
	size_t target=0;
	buffer=(wchar_t*)s.GetBuffer();
	wchar_t c;
	for (size_t i=0;i<ss;i++) {
		c=buffer[i];
		if (c<L'A' || c>L'z' || (c>L'Z' && c<L'a')) {
			it=letterReplacements.find(c);
			if (it!=letterReplacements.end()) {
				if (it->second!=(wchar_t)0) {
					buffer[target++]=it->second;
				}
			} else {
				buffer[target++]=c;
			}
		} else {
			buffer[target++]=c;
		}
	}
	//printf ("ss=%zi, target=%zi\n",ss,target);
	s.Cut(target);
	term=s;
}

int CWmClient::GetWords(const ppl6::CString &str, ppl6::CArray &words)
{
	words.Clear();
	ppl6::CString s=str;
	s.Trim();
	if (s.IsEmpty()) return 0;
	// Bestimmte Zeichen filtern wir raus
	NormalizeTerm(s);
	words.Explode(s," ",0,true);
	return 1;
}

const char *key="HJnD8Kj$/SDbu910LqAb§doHqc";

ppl6::CString CWmClient::DeCrypt(const ppl6::CString &str)
{
	ppl6::CBinary bin=FromBase64(str);
	if (!ppl6::CMCrypt::Decrypt(bin,key,ppl6::CMCrypt::Algo_TWOFISH,ppl6::CMCrypt::Mode_CFB)) {
		throw ppl6::Exception();
	}
	return bin;
}

ppl6::CString CWmClient::Crypt(const ppl6::CString &str)
{
	ppl6::CBinary bin=str;
	if (!ppl6::CMCrypt::Crypt(bin,key,ppl6::CMCrypt::Algo_TWOFISH,ppl6::CMCrypt::Mode_CFB)) {
		throw ppl6::Exception();
	}
	return ToBase64(bin);
}

int CWmClient::StartServer()
{
	return 1;
}

int CWmClient::StopServer()
{
	return 1;
}

