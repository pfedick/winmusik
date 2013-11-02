/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.27 $
 * $Date: 2012/01/13 19:45:04 $
 * $Id: edit.cpp,v 1.27 2012/01/13 19:45:04 pafe Exp $
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

#include "winmusik3.h"
#include <ppl6-grafix.h>
#include "edit.h"
#include "editdevice.h"
#include "tablesearch.h"
#include "shortcutdialog.h"
#include "massimport.h"
#include <QString>
#include <QScrollBar>
#include <QClipboard>
#include <QCompleter>
#include <QMenu>
#include <QUrl>
#include <QPoint>
#include <QTimer>
#include <QPixmap>
#include <QBuffer>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressDialog>


AsynchronousTrackUpdate::AsynchronousTrackUpdate()
{
	edit=NULL;
}

AsynchronousTrackUpdate::~AsynchronousTrackUpdate()
{

}

void AsynchronousTrackUpdate::ThreadMain(void *)
{

}

CTitleList::CTitleList(QWidget * parent)
	: QTreeWidget(parent)
{
	edit=NULL;
	wm=NULL;
}


void CTitleList::mousePressEvent ( QMouseEvent * event )
{
	//printf ("mousePressEvent\n");
	edit->on_trackList_MousePress(event);
	QTreeWidget::mousePressEvent(event);
}

void CTitleList::mouseReleaseEvent ( QMouseEvent * event )
{
	//printf ("mouseReleaseEvent\n");
	edit->on_trackList_MouseRelease(event);
	QTreeWidget::mouseReleaseEvent(event);
}


void CTitleList::mouseMoveEvent ( QMouseEvent * event )
{
	//printf ("mouseMoveEvent\n");
	edit->on_trackList_MouseMove(event);
	//QTreeWidget::mouseMoveEvent(event);
}

void CTitleList::focusInEvent ( QFocusEvent * event )
{
	QTreeWidget::focusInEvent(event);
	edit->FixFocus();
}

/*!\class Edit
 * \brief Editor-Fenster zur Titeleingabe
 * \ingroup WM3GUI
 *
 */

/*!\var Edit::position
 * \brief Positions-Variable
 *
 * Diese Variable wird in Abhängigkeit des Feldes gesetzt, das gerade den Eingabefocus
 * hat. Folgende Werte sind möglich:
 *
 * \image html eingabepositionen.png
 */

Edit::Edit(QWidget *parent, CWmClient *wm, int typ)
    : QWidget(parent)
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor der Klasse baut das Interface auf und übersetzt alle Text. Ausserdem werden diverse
 * Variablen initialisiert.
 */
{
	ui.setupUi(this);
	asyncTrackUpdate.edit=this;
	this->wm=wm;
	setAttribute(Qt::WA_DeleteOnClose,true);
	DeviceType=typ;
	DeviceId=0;
	Page=0;
	TrackList=NULL;
	position=0;
	oldposition=0;
	oimpInfo=NULL;
	titleCompleter=NULL;
	artistCompleter=NULL;
	albumCompleter=NULL;
	currentTrackListItem=NULL;
	searchWindow=NULL;
	DupeCheck=false;
	DupeTimer=NULL;
	SetupTrackList();
	DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";


	ui.releaseDate->setDate(QDate::currentDate());
	ui.recordDate->setDate(QDate::currentDate());

	// Events capturen
	InstallFilter(ui.index,1);
	InstallFilter(ui.page,2);
	InstallFilter(ui.track,3);
	InstallFilter(ui.artist,4);
	InstallFilter(ui.title,5);
	InstallFilter(ui.versionId,6);
	InstallFilter(ui.version,7);
	InstallFilter(ui.genreId,8);
	InstallFilter(ui.genre,9);
	InstallFilter(ui.length,10);
	InstallFilter(ui.bpm,11);
	InstallFilter(ui.releaseDate,12);
	InstallFilter(ui.recordDate,13);
	InstallFilter(ui.album,14);
	InstallFilter(ui.labelId,15);
	InstallFilter(ui.labelName,16);
	InstallFilter(ui.recordSourceId,17);
	InstallFilter(ui.recordSource,18);
	InstallFilter(ui.recordDeviceId,19);
	InstallFilter(ui.recordDevice,20);
	InstallFilter(ui.remarks,21);
	InstallFilter(ui.tags,22);
	InstallFilter(ui.complete,23);
	InstallFilter(ui.realTitle,24);
	InstallFilter(ui.interrupted,25);
	InstallFilter(ui.channels,26);
	InstallFilter(ui.quality,27);
	InstallFilter(ui.rating,28);
	InstallFilter(ui.bitrate,29);
	InstallFilter(ui.musickey,30);

	InstallFilter(ui.cover,100);


	/*
	 * 	Label.Title=tr("Producer / Label");
	Label.Init(this,wm,ui.labelId,ui.labelText,&wm->LabelStore);
	Label.SetNextWidget(ui.purchaseId);
	 *
	 */
	TCVersion.Title=tr("Version");
	TCVersion.Init(this,wm,ui.versionId,ui.version,&wm->VersionStore);
	TCVersion.SetNextWidget(ui.genreId);

	TCGenre.Title=tr("Genre");
	TCGenre.Init(this,wm,ui.genreId,ui.genre,&wm->GenreStore);
	TCGenre.SetNextWidget(ui.length);

	TCLabel.Title=tr("Label");
	TCLabel.Init(this,wm,ui.labelId,ui.labelName,&wm->LabelStore);
	TCLabel.SetNextWidget(ui.recordSourceId);

	TCRecordSource.Title=tr("Record Source");
	TCRecordSource.Init(this,wm,ui.recordSourceId,ui.recordSource,&wm->RecordSourceStore);
	TCRecordSource.SetNextWidget(ui.recordDeviceId);

	TCRecordDevice.Title=tr("Record Device");
	TCRecordDevice.Init(this,wm,ui.recordDeviceId,ui.recordDevice,&wm->RecordDeviceStore);
	TCRecordDevice.SetNextWidget(ui.remarks);



	ui.deviceIcon->setPixmap(wm->GetDevicePixmap(typ));
	setWindowIcon(wm->GetDeviceIcon(typ));

	switch (typ) {
		case 1:	// Cassette
			setWindowTitle(tr("Edit Music Cassette"));
			break;
		case 2:	// CD
			setWindowTitle(tr("Edit Audio CD"));
			break;
		case 3:	// Data CD
			setWindowTitle(tr("Edit Data CD"));
			break;
		case 4:	// Vinyl Record
			setWindowTitle(tr("Edit Vinyl Record"));
			break;
		case 5:	// VHS
			setWindowTitle(tr("Edit Video Tape"));
			break;
		case 6:	// Other
			setWindowTitle(tr("Edit Other Media Type"));
			break;
		case 7:	// MP3
			setWindowTitle(tr("Edit MP3 CD"));
			break;
		case 8:	// Tape
			setWindowTitle(tr("Edit Music Tape"));
			break;
		case 9:	// DVD
			setWindowTitle(tr("Edit Audio DVD"));
			break;
	}

	ppluint32 highest=wm->GetHighestDeviceId(typ);
	if (highest) {
		ppl6::CString Tmp;
		Tmp.Setf("%u",highest);
		ui.index->setText(Tmp);
	}
	ui.index->setFocus();
	ppl6::CString Name;
	Name.Setf("edit_type_%i",typ);
	this->restoreGeometry(wm->GetGeometry(Name));

	hideEditor();
}

Edit::~Edit()
{
	if (DupeTimer) delete DupeTimer;
	if (oimpInfo) delete oimpInfo;
	if (wm) wm->EditorClosed(this);
	if (TrackList) delete TrackList;
	if (titleCompleter) delete titleCompleter;
	if (artistCompleter) delete artistCompleter;
	if (albumCompleter) delete albumCompleter;
	//if (searchWindow) delete searchWindow;
}

void Edit::OpenTrack(ppluint32 deviceId, ppluint8 page, ppluint16 track)
{
	if (TrackList) delete TrackList;
	TrackList=NULL;
	Page=0;
	DeviceId=0;
	TrackNum=0;
	ui.page->setEnabled(true);
	ui.titleEdit->setEnabled(false);
	ui.index->deselect();
	ui.index->selectAll();
	ClearEditFields();
	DeviceId=deviceId;
	Page=page;
	ui.index->setText(ppl6::ToString("%i",deviceId));
	if (!Page) Page=1;
	ui.page->setText(ppl6::ToString("%i",Page));
	if (wm->LoadDevice(DeviceType,DeviceId,&datadevice)) {
		UpdateDevice();
		if (datadevice.Pages==1) {
			ui.page->setEnabled(false);
		}
		TrackList=wm->GetTracklist(DeviceType,DeviceId,Page);
		UpdateTrackListing();
		ui.titleEdit->setEnabled(true);
		ui.track->setFocus();
		if (track>0) {
			showEditor();
			ui.track->setText(ppl6::ToString("%i",track));
			ui.artist->setFocus();
		}

	}

}

void Edit::SetupTrackList()
/*!\brief Trackliste einrichten/
 *
 * Die Trackliste besteht aus einem QTreeWidget. Dieses hat das Problem, wenn es mit dem Designer erstellt wurde,
 * dass die Mouse-Events nicht abgefangen werden können. Daher müssen wir eine eigene Instanz ableiten (siehe
 * CTitleList) und die Methoden selbst implementieren. Das erfordert wiederum, dass das Widget zur
 * Laufzeit erstellt und an die richtige Position gesetzt wird.
 * \par
 * Im Formular gibt es bereits ein rahmenloses Widget als Platzhalter: Edit::ui::listWidget.
 */
{
	QLayout *w=ui.listWidget->layout();
	trackList=new CTitleList(ui.listWidget);
	w->addWidget(trackList);
	trackList->edit=this;
	trackList->wm=wm;

	//trackList->installEventFilter(this);
	//trackList->setMouseTracking(true);
    trackList->setDragEnabled(false);
    trackList->setDragDropMode(QAbstractItemView::DragOnly);


    trackList->setObjectName(QString::fromUtf8("trackList"));
    trackList->setAlternatingRowColors(true);
    trackList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    trackList->setRootIsDecorated(false);
    trackList->setItemsExpandable(false);
    trackList->setSortingEnabled(false);
    trackList->setContextMenuPolicy(Qt::CustomContextMenu);

    trackList->headerItem()->setText(TRACKLIST_TRACK_ROW, tr("Track","trackList"));
    trackList->headerItem()->setText(TRACKLIST_COVER_ROW, tr("Cover","trackList"));
    trackList->headerItem()->setText(TRACKLIST_NAME_ROW, tr("Artist - Title","trackList"));
    trackList->headerItem()->setText(TRACKLIST_VERSION_ROW, tr("Version","trackList"));
    trackList->headerItem()->setText(TRACKLIST_GENRE_ROW, tr("Genre","trackList"));
    trackList->headerItem()->setText(TRACKLIST_LENGTH_ROW, tr("Length","trackList"));
    trackList->headerItem()->setText(TRACKLIST_BPM_ROW, tr("BPM","trackList"));
    trackList->headerItem()->setText(TRACKLIST_KEY_ROW, tr("Key","trackList"));
    trackList->headerItem()->setText(TRACKLIST_RATING_ROW, tr("Rating","trackList"));

    connect(trackList,SIGNAL(customContextMenuRequested(const QPoint &)),
    		this,SLOT(on_trackList_customContextMenuRequested(const QPoint &)));
    connect(trackList,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
    		this,SLOT(on_trackList_itemDoubleClicked(QTreeWidgetItem *, int)));
    connect(trackList,SIGNAL(itemClicked(QTreeWidgetItem *, int)),
    		this,SLOT(on_trackList_itemClicked(QTreeWidgetItem *, int)));

    /*
    QPalette pal=trackList->palette();
    pal.setColor(QPalette::Base,QColor(252,246,239,255));
    pal.setColor(QPalette::AlternateBase,QColor(241,234,227,255));
    trackList->setPalette(pal);
    QTreeWidgetItem *header=trackList->headerItem();
    int cols=header->columnCount();
    for (int i=0;i<cols;i++) {
    	QBrush brush=header->background(i);
    	brush.setColor(QColor(189,175,156,150));
    	header->setBackground(i,brush);
    }
    */
    QString Style="QTreeView::item {\n"
    		"border-right: 1px solid #b9b9b9;\n"
    		"border-bottom: 1px solid #b9b9b9;\n"
    		"}\n"
    		"QTreeView::item:selected {\n"
    		//"border-top: 1px solid #80c080;\n"
    		//"border-bottom: 1px solid #80c080;\n"
    		"background: #d0d0ff;\n"
    		"color: #000000;\n"
    		"}\n"
    		"";
    trackList->setStyleSheet(Style);

}

void Edit::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void Edit::InstallFilter(QObject *object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id",id);
}

bool Edit::eventFilter(QObject *target, QEvent *event)
/*!\brief Event Handler
 *
 * Dies ist eine überladene Funktion, die von Qt immer dann aufgerufen wird, wenn bei einem Widget, bei dem ein
 * Filter installiert wurde (Edit::InstallFilter) ein beliebiger Event auftritt. Die Funktion ruft lediglich
 * die Funktion Edit::consumeEvent auf. Liefert diese \c true zurück, wird davon ausgegangen, dass der
 * Event behandelt wurde und der Default-Eventhandler (QWidget::eventFilter) nicht aufgerufen werden muss.
 *
 * \param[in] target Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] event Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Liefert \c true zurück, wenn der Event von Edit::consumeEvent verarbeitet wurde. Andernfalls
 * wird die übergeordnete Basisfunktion QWidget::eventFilter aufgerufen und deren Returncode zurückgeliefert.
 */
{
	if (consumeEvent(target,event)) return true;
	return QWidget::eventFilter(target,event);
}

bool Edit::consumeEvent(QObject *target, QEvent *event)
/*!\brief Event verarbeiten
 *
 * Diese Funktion wird von Edit::eventFilter aufgerufen, wenn Qt einen Event für ein Widget registriert hat,
 * für das ein Filter installiert wurde (Edit::InstallFilter).
 *
 * Die Funktion prüft, ob für das angegebene \p target ein Handler existiert und ruft diesen auf. Der
 * spezielle Handler muss \c true zurückgeben, wenn er den Event verarbeitet hat und der Standardhandler
 * von Qt nicht mehr aufgerufen werden muss. Die Rückgabe von \c false bewirkt, dass der
 * Standardhandler QWidget::eventFilter aufgerufen wird.
 *
 * \param[in] target Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] event Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Gibt \c true zurück, wenn der Event verarbeit wurde, sonst \c false
 */
{
	ppl6::CString Tmp;
	QKeyEvent *keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;
	QFocusEvent *focusEvent=NULL;

	// Id auslesen
	int id=target->property("id").toInt();
	int type=event->type();
	if (type==QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent *>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (on_KeyPress(target,key,modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	} else if (type==QEvent::FocusIn || type==QEvent::FocusOut) {
		focusEvent=static_cast<QFocusEvent *>(event);
		if (type==QEvent::FocusIn) {
			if (id>3 && ui.track->text().toInt()<1) { // Das erlauben wir nur, wenn eine Tracknummer angegeben ist
				ui.track->setFocus();
				return true;
			}
			position=id;
			UpdateFkeys();
			if (position>7 && DupeCheck==false) {
				CheckDupes();
			} else if (position<8 && DupeCheck==true) DupeCheck=false;
		}
		else if (type==QEvent::FocusOut) {
			oldposition=id;
			if (oldposition==10) on_length_FocusOut();
		}
	}

	if (target==ui.index) {
		if (type==QEvent::FocusIn) return on_index_FocusIn();
		if (type==QEvent::KeyPress) return on_index_KeyPress(keyEvent,key,modifier);
	} else if (target==ui.page) {
		if (type==QEvent::FocusIn) return on_page_FocusIn();
	} else if (target==ui.track) {
		if (type==QEvent::FocusIn) return on_track_FocusIn();
	} else if (target==ui.artist) {
		if (type==QEvent::FocusIn) {
			int reason=focusEvent->reason();
			/*
			printf ("Artist::FocusIn, Reason=");
			if (reason==Qt::MouseFocusReason) printf ("MouseFocusReason\n");
			if (reason==Qt::TabFocusReason) printf ("TabFocusReason\n");
			if (reason==Qt::BacktabFocusReason) printf ("BacktabFocusReason\n");
			if (reason==Qt::ActiveWindowFocusReason) printf ("ActiveWindowFocusReason\n");
			if (reason==Qt::PopupFocusReason) printf ("PopupFocusReason\n");
			if (reason==Qt::ShortcutFocusReason) printf ("ShortcutFocusReason\n");
			if (reason==Qt::MenuBarFocusReason) printf ("MenuBarFocusReason\n");
			if (reason==Qt::OtherFocusReason) printf ("OtherFocusReason\n");
			*/
			// Bei einem PopUp-Event (ausgelöst durch den Completer) darf die Focus-Funktion
			// nicht aufgerufen werden, da der Completer sonst während der Benutzung
			// gelöscht wird
			if (reason!=Qt::PopupFocusReason) return on_artist_FocusIn();
			return false;
		}
		if (type==QEvent::FocusOut) {
			return on_artist_FocusOut();
		}
	} else if (target==ui.title) {
		if (type==QEvent::FocusIn && focusEvent->reason()!=Qt::PopupFocusReason) return on_title_FocusIn();
		if (type==QEvent::FocusOut) return on_title_FocusOut();
	} else if (target==ui.version || target==ui.versionId) {
		return TCVersion.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.genre || target==ui.genreId) {
		return TCGenre.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.labelName || target==ui.labelId) {
		return TCLabel.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.recordSource || target==ui.recordSourceId) {
		return TCRecordSource.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.recordDevice || target==ui.recordDeviceId) {
		return TCRecordDevice.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.album) {
		if (type==QEvent::FocusIn && focusEvent->reason()!=Qt::PopupFocusReason) return on_album_FocusIn();
		if (type==QEvent::FocusOut) return on_album_FocusOut();
	} else if (target==ui.length) {
		if (type==QEvent::FocusIn) return on_FocusIn(ui.length);
	} else if (target==ui.bpm) {
		if (type==QEvent::FocusIn) return on_FocusIn(ui.bpm);
	} else if (target==ui.musickey) {
		if (type==QEvent::FocusIn) return on_FocusIn(ui.musickey);
	} else if (target==ui.bitrate) {
		if (type==QEvent::FocusIn) return on_FocusIn(ui.bitrate);
	} else if (target==ui.album) {
		if (type==QEvent::FocusIn) return on_FocusIn(ui.album);
	} else if (target==ui.remarks) {
		if (type==QEvent::FocusIn) return on_FocusIn(ui.remarks);
	} else if (target==ui.tags) {
		if (type==QEvent::FocusIn) return on_FocusIn(ui.tags);
	} else if (target==ui.cover && type==QEvent::MouseButtonDblClick) {
		wm->OpenCoverViewer(Cover);	
	} else if (target==ui.cover && type==QEvent::MouseButtonPress) {
		wm->OpenCoverViewer(Cover);	
	}
	return false;
}

void Edit::CheckDupes()
{
	DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";
	DupeCheck=true;
	TCVersion.Finish();
	ppl6::CString Tmp, Artist, Title;
	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.Trim();
	Title=ui.title->text();
	Title.Trim();
	ppluint32 Version=ui.versionId->text().toInt();
	if (wm->Hashes.CheckDupes(Artist,Title,Version,Ti.TitleId)) {
		if (!DupeTimer) {
			DupeTimer=new QTimer(this);
			connect(DupeTimer, SIGNAL(timeout()), this, SLOT(on_DupeTimer_update()));
		}
		DupeTimer->start(300);
	} else if (DupeTimer) DupeTimer->stop();
}

void Edit::on_DupeTimer_update()
{
	static int count=0;
	count++;
	if (count&1) DupeCheckIcon=":/fkeys/resources/fkeys/f-key-3005.png";
	else DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";
	if (position>7) {
		ui.fkeys->setFkey(5,DupeCheckIcon,"");
	}
}

void Edit::resizeEvent ( QResizeEvent * event )
/*!\brief Größenänderung des Fensters
 *
 * Diese Funktion wird durch Qt aufgerufen, wenn sich die Größe
 * des Fensters ändert, sowie von Edit::UpdateTrackListing, wenn sich der
 * Inhalt der Trackliste ändert.
 */
{
	int w=trackList->width();
	trackList->setColumnWidth(TRACKLIST_TRACK_ROW,60);
	w-=62;
	trackList->setColumnWidth(TRACKLIST_COVER_ROW,64);
	w-=66;
	trackList->setColumnWidth(TRACKLIST_LENGTH_ROW,60);
	w-=62;
	trackList->setColumnWidth(TRACKLIST_BPM_ROW,35);
	w-=37;
	trackList->setColumnWidth(TRACKLIST_KEY_ROW,40);
	w-=42;
	trackList->setColumnWidth(TRACKLIST_RATING_ROW,75);
	w-=77;
	trackList->setColumnWidth(TRACKLIST_NAME_ROW,w*55/100);
	trackList->setColumnWidth(TRACKLIST_VERSION_ROW,w*30/100);
	trackList->setColumnWidth(TRACKLIST_GENRE_ROW,w*15/100);
	QWidget::resizeEvent(event);
}

void Edit::closeEvent(QCloseEvent *event)
{
	ppl6::CString Name;
	Name.Setf("edit_type_%i",DeviceType);
	if (wm) {
		wm->SaveGeometry(Name,this->saveGeometry());
	}
	if (oimpInfo) {
		delete oimpInfo;
		oimpInfo=NULL;
	}
    QWidget::closeEvent(event);
}

void Edit::moveEvent(QMoveEvent *event)
{
	/*
	if (oimpInfo) {
		delete oimpInfo;
		oimpInfo=NULL;
	}
	*/
	QWidget::moveEvent(event);
}


void Edit::SetFkey(QToolButton *button,const char *Icon, QString Text, bool enabled)
{
	button->setIcon(QIcon(Icon));
	if (Text.size()) button->setText(Text);
	button->setEnabled(enabled);
}


void Edit::UpdateFkeys()
{
	ui.fkeys->setFkey(0,":/fkeys/resources/fkeys/f-key-0000.png"," ",false);
	ui.fkeys->setFkey(1,":/fkeys/resources/fkeys/f-key-0001.png"," ",false);
	ui.fkeys->setFkey(2,":/fkeys/resources/fkeys/f-key-0002.png"," ",false);
	ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-0003.png"," ",false);
	ui.fkeys->setFkey(4,":/fkeys/resources/fkeys/f-key-0004.png"," ",false);
	ui.fkeys->setFkey(5,":/fkeys/resources/fkeys/f-key-0005.png"," ",false);
	ui.fkeys->setFkey(6,":/fkeys/resources/fkeys/f-key-0006.png"," ",false);
	ui.fkeys->setFkey(7,":/fkeys/resources/fkeys/f-key-0007.png"," ",false);
	ui.fkeys->setFkey(8,":/fkeys/resources/fkeys/f-key-0008.png"," ",false);
	ui.fkeys->setFkey(9,":/fkeys/resources/fkeys/f-key-0009.png"," ",false);
	ui.fkeys->setFkey(10,":/fkeys/resources/fkeys/f-key-0010.png"," ",false);
	ui.fkeys->setFkey(11,":/fkeys/resources/fkeys/f-key-0011.png"," ",false);
	ui.fkeys->setFkey(12,":/fkeys/resources/fkeys/f-key-0012.png"," ",false);

	QString t[20];
	t[0]=tr("close");
	t[1]=tr("new");
	t[2]=tr("back");
	t[3]=tr("master data");
	t[4]=tr("search");
	t[5]=tr("print");
	t[6]=tr("save");
	t[7]=tr("delete");
	t[8]=tr("insert");
	t[9]=tr("first letter");
	t[10]=tr("read ID3");
	t[11]=tr("shortcut");
	t[12]=tr("original Tags");
	t[13]=tr("dupes?");
	t[14]=tr("playlist");
	t[15]=tr("autoimport");
	t[16]=tr("save all ID3");
	t[17]=tr("list devices");
	t[18]=tr("mass import");

	switch (position) {
		case 1:		// Device Index
			ui.fkeys->setFkey(0,":/fkeys/resources/fkeys/f-key-1000.png",t[0]);
			ui.fkeys->setFkey(2,":/fkeys/resources/fkeys/f-key-1002.png",t[1]);
			ui.fkeys->setFkey(4,":/fkeys/resources/fkeys/f-key-2004.png",t[17]);
			break;
		case 2:		// Device Page
			break;
		case 3:		// Device Track
			//if (DeviceType==7) SetFkey(ui.f6,":/fkeys/resources/fkeys/f-key-2006.png",t[15]);
			if (wm->conf.DevicePath[DeviceType].NotEmpty()==true) ui.fkeys->setFkey(9,":/fkeys/resources/fkeys/f-key-2009.png",t[16]);
			if (wm->conf.DevicePath[DeviceType].NotEmpty()==true) ui.fkeys->setFkey(6,":/fkeys/resources/fkeys/f-key-3006.png",t[18]);

			break;
		case 4:		// Interpret
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			ui.fkeys->setFkey(5,":/fkeys/resources/fkeys/f-key-1005.png",t[11]);
			break;
		case 5:		// Titel
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
		case 6:		// VersionId
			break;
		case 7:		// Version
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
		case 9:		// Genre
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
		case 14:		// Album
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
		case 16:		// Label
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
		case 18:		// Aufnahmequelle
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
		case 20:		// Aufnahmegerät
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
		case 21:		// Bemerkung
			ui.fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
			break;
	}
	if (position>1) {
		ui.fkeys->setFkey(0,":/fkeys/resources/fkeys/f-key-2000.png",t[2]);
		ui.fkeys->setFkey(2,":/fkeys/resources/fkeys/f-key-2002.png",t[3]);
	}
	if (position>2) {
		ui.fkeys->setFkey(11,":/fkeys/resources/fkeys/f-key-1011.png",t[5]);
		if (wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
			ui.fkeys->setFkey(10,":/fkeys/resources/fkeys/f-key-1010.png",t[14]);
		}
	}
	if (position>3) {
		ui.fkeys->setFkey(7,":/fkeys/resources/fkeys/f-key-1007.png",t[7]);
		ui.fkeys->setFkey(8,":/fkeys/resources/fkeys/f-key-1008.png",t[8]);
		ui.fkeys->setFkey(12,":/fkeys/resources/fkeys/f-key-1012.png",t[6]);
		if (wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
			ui.fkeys->setFkey(6,":/fkeys/resources/fkeys/f-key-1006.png",t[10]);
			if (Ti.ImportData>0) ui.fkeys->setFkey(9,":/fkeys/resources/fkeys/f-key-1009.png",t[12]);
		}
	}
	if (position>7) {
		// DupeCheck
		ui.fkeys->setFkey(5,DupeCheckIcon,t[13]);
	}

	// Suchbutton
	if (position==4 || position==5 || position==6 || position==8
			||position==15 || position==17 || position==19) {
		ui.fkeys->setFkey(4,":/fkeys/resources/fkeys/f-key-1004.png",t[4]);
	}
}

void Edit::MoveToNextWidget()
/*!\brief zum nächsten Eingabefeld wechseln
 *
 * Diese Funktion prüft anhand der Variablen \c position in welchem Eingabefeld sich der Focus
 * gerade befindet und setzt den Focus auf das nachfolgende Eingabefeld.
 */
{
	switch (position) {
		case 1: ui.page->setFocus(); break;
		case 2: ui.track->setFocus(); break;
		case 3: ui.artist->setFocus(); break;
		case 4: ui.title->setFocus(); break;
		case 5: ui.versionId->setFocus(); break;
		case 6: ui.version->setFocus(); break;
		case 7: ui.genreId->setFocus(); break;
		case 8: ui.genre->setFocus(); break;
		case 9: ui.length->setFocus(); break;
		case 10: ui.bpm->setFocus(); break;
		case 11: ui.musickey->setFocus(); break;
		case 30: ui.bitrate->setFocus(); break;
		case 29: ui.releaseDate->setFocus(); break;
		case 12: ui.recordDate->setFocus(); break;
		case 13: ui.album->setFocus(); break;
		case 14: ui.labelId->setFocus(); break;
		case 15: ui.labelName->setFocus(); break;
		case 16: ui.recordSourceId->setFocus(); break;
		case 17: ui.recordSource->setFocus(); break;
		case 18: ui.recordDeviceId->setFocus(); break;
		case 19: ui.recordDevice->setFocus(); break;
		case 20: ui.remarks->setFocus(); break;
		case 21: ui.tags->setFocus(); break;
		case 22: ui.artist->setFocus(); break;
		case 23: ui.realTitle->setFocus(); break;
		case 24: ui.interrupted->setFocus(); break;
		case 25: ui.channels->setFocus(); break;
		case 26: ui.quality->setFocus(); break;
		case 27: ui.rating->setFocus(); break;
		case 28: ui.artist->setFocus(); break;
	};
}

QWidget *Edit::GetWidgetFromPosition(int position)
{
	switch (position) {
		case 1: return ui.index;
		case 2: return ui.page;
		case 3: return ui.track;
		case 4: return ui.artist;
		case 5: return ui.title;
		case 6: return ui.versionId;
		case 7: return ui.version;
		case 8: return ui.genreId;
		case 9: return ui.genre;
		case 10: return ui.length;
		case 11: return ui.bpm;
		case 12: return ui.releaseDate;
		case 13: return ui.recordDate;
		case 14: return ui.album;
		case 15: return ui.labelId;
		case 16: return ui.labelName;
		case 17: return ui.recordSourceId;
		case 18: return ui.recordSource;
		case 19: return ui.recordDeviceId;
		case 20: return ui.recordDevice;
		case 21: return ui.remarks;
		case 22: return ui.tags;
		case 23: return ui.complete;
		case 24: return ui.realTitle;
		case 25: return ui.interrupted;
		case 26: return ui.channels;
		case 27: return ui.quality;
		case 28: return ui.rating;
		case 29: return ui.bitrate;
		case 30: return ui.musickey;
	}
	return NULL;
}


// ********************************************************************************************************
// *** EVENTS                                                                                           ***
// ********************************************************************************************************

// Globale Events, Fkeys
bool Edit::on_KeyPress(QObject *target, int key, int modifier)
/*!\brief Globale KeyPress Events behandeln
 *
 * Diese Funktion behandelt globale KeyPress Events, als Beispielsweise das Drücken der ESC-Taste,
 * F1-F12 aber auch die Return/Enter-Taste und weitere Shortcuts.
 *
 * \param[in] target Das QObject, auf das sich der Event bezieht
 * \param[in] key Code der Taste (Qt::Key)
 * \param[in] modifier Modifizierung dazu (Qt::KeyboardModifiers)
 *
 * \returns Die Funktion gibt \b true zurück, wenn der Event behandelt wurde und das System sonst
 * nichts mehr machen muss, andernfalls \b false.
 */
{
	// ******************************************************************************* ESC
	if (key==Qt::Key_Escape) {
		on_esc_clicked();
		return true;
		// *************************************************************************** Return/Enter
	} else if (key==Qt::Key_Return || key==Qt::Key_Enter) {
		MoveToNextWidget();
		return true;
		// *************************************************************************** Page down
	} else if (position>2 && key==Qt::Key_PageDown) {
		if (modifier==Qt::NoModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
		else if (modifier==Qt::ShiftModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
		return true;
		// *************************************************************************** Page up
	} else if (position>2 && key==Qt::Key_PageUp) {
		if (modifier==Qt::NoModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
		else if (modifier==Qt::ShiftModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
		return true;
		// *************************************************************************** F2
	} else if (key==Qt::Key_F2 && modifier==Qt::NoModifier && position>1) {
		ppluint32 ret=EditDeviceDialog(DeviceId);
		((QWidget*)target)->setFocus();
		if (ret==DeviceId) UpdateDevice();
		return true;
		// *************************************************************************** F3
	} else if (key==Qt::Key_F3 && modifier==Qt::NoModifier && ui.fkeys->isEnabled(3)==true) {
		QLineEdit *LineEdit=(QLineEdit*)target;
		ppl6::CWString Tmp=LineEdit->text().toLower();
		Tmp.UCWords();
		LineEdit->setText(Tmp);
		return true;
		// *************************************************************************** F4 im Index
	} else if (key==Qt::Key_F4 && position==1) {
		wm->OpenDeviceList(DeviceType);
		return true;
		// *************************************************************************** F4
	} else if (key==Qt::Key_F4 && position>=4 && position<=5) {
		return on_f4_Pressed(position);
		// *************************************************************************** F5
	} else if (key==Qt::Key_F5 && position==4) {
		return on_f5_ShortCut(modifier);
	} else if (key==Qt::Key_F5 && position>7) {
		return on_f5_CheckDupes(target);
		// *************************************************************************** F6
	} else if (DeviceType==7 && key==Qt::Key_F6 && modifier==Qt::ControlModifier && ui.fkeys->isEnabled(6)==true && position>3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		wm->TrashMP3File(DeviceId,Page,Track.Track);
		return on_f6_Pressed(target,Qt::NoModifier);

	} else if (DeviceType==7 && key==Qt::Key_F6 && ui.fkeys->isEnabled(6)==true && position>3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f6_Pressed(target,modifier);

	} else if (DeviceType==7 && key==Qt::Key_F6 && ui.fkeys->isEnabled(6)==true && position==3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f6_MassImport();
		// *************************************************************************** F7
	} else if (key==Qt::Key_F7 && position>3 && modifier==Qt::NoModifier) {
		return on_f7_DeleteTrack();
		// *************************************************************************** F8
	} else if (key==Qt::Key_F8 && position>3 && modifier==Qt::NoModifier) {
		return on_f8_InsertTrack();
		// *************************************************************************** F9
	} else if (DeviceType==7 && key==Qt::Key_F9 && modifier==Qt::NoModifier && position>3 && Ti.ImportData>0) {
		if (oimpInfo) {
			delete oimpInfo;
			oimpInfo=NULL;
		} else if (wm->OimpDataStore.GetCopy(Ti.ImportData,&Oimp)) {
			ShowOimpInfo();
		}
		return true;
		// *************************************************************************** F9
	} else if (DeviceType==7 && key==Qt::Key_F9 && modifier==Qt::NoModifier && position==3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f9_UpdateAllID3Tags();
		// *************************************************************************** F10
	} else if (key==Qt::Key_F10 && position>2 && modifier==Qt::NoModifier && DeviceType==7 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f10_WritePlaylist();
		// *************************************************************************** F11 => Printing
	} else if (key==Qt::Key_F11 && position>2 && modifier==Qt::NoModifier) {
		wm->PrintCoverDialog(this,DeviceType,DeviceId);
		return true;
		// *************************************************************************** F12
	} else if (key==Qt::Key_F12 && modifier==Qt::NoModifier && position>3) {
		SaveTrack();
		return true;
		// *************************************************************************** Alt & b
	} else if (key==Qt::Key_B && modifier==Qt::AltModifier && position>3) {
		ui.remarks->setFocus();
		return true;
		// *************************************************************************** Alt & a
	} else if (key==Qt::Key_A && modifier==Qt::AltModifier && position>3) {
		ui.album->setFocus();
		return true;
		// *************************************************************************** Alt & d
	} else if (key==Qt::Key_D && modifier==Qt::AltModifier && position>3) {
		ui.recordDate->setFocus();
		return true;
		// *************************************************************************** Alt & l
	} else if (key==Qt::Key_L && modifier==Qt::AltModifier && position>3) {
		ui.labelId->setFocus();
		return true;
		// *************************************************************************** Alt & t
	} else if (key==Qt::Key_T && modifier==Qt::AltModifier && position>3) {
		ui.tags->setFocus();
		return true;
		// *************************************************************************** Alt & y
	} else if (key==Qt::Key_Y && modifier==Qt::AltModifier && position>3) {
		ui.releaseDate->setFocus();
		return true;
		// *************************************************************************** Alt & j
	} else if (key==Qt::Key_J && modifier==Qt::AltModifier && position>3) {
		ui.releaseDate->setFocus();
		return true;
	}
	return false;
}

// EVENT: index
bool Edit::on_index_FocusIn()
{
	asyncTrackUpdate.ThreadStop();
	if (TrackList) delete TrackList;
	TrackList=NULL;
	Page=0;
	DeviceId=0;
	TrackNum=0;
	ui.page->setEnabled(true);
	ui.titleEdit->setEnabled(false);
	ui.index->deselect();
	ui.index->selectAll();
	ClearEditFields();
	return false;
}

bool Edit::on_index_KeyPress(__attribute__ ((unused)) QKeyEvent *event,int key,int modifier)
{
	if (key==Qt::Key_F2 && modifier==Qt::NoModifier) {
		ppluint32 ret=EditDeviceDialog(0);
		if (ret) {
			ppl6::CString Tmp;
			Tmp.Setf("%u",ret);
			ui.index->setText(Tmp);
			ui.page->setFocus();
			return false;
		}
		ui.index->setFocus();
		return false;
	}
	return false;
}

// *****************************************************************************************************
// EVENT: page
bool Edit::on_page_FocusIn()
{
	// Wenn wir von Index kommen, laden wir den Tonträger
	asyncTrackUpdate.ThreadStop();
	Page=0;
	TrackNum=0;
	if (TrackList) delete TrackList;
	ClearEditFields();
	TrackList=NULL;
	ppl6::CString Tmp;
	if (oldposition<2) {
		DeviceId=ui.index->text().toInt();
		if (ui.index->text()=="?") {
			DeviceId=wm->DeviceStore.GetHighestDevice(DeviceType);
			Tmp.Setf("%u",DeviceId);
			ui.index->setText(Tmp);
		}
		if (ui.index->text()=="*") {
			DeviceId=EditDeviceDialog(0);
			if (!DeviceId) {
				ui.index->setFocus();
				return true;
			}
			Tmp.Setf("%u",DeviceId);
			ui.index->setText(Tmp);
		}
		if (DeviceId==0) {
			ui.index->setFocus();
			return true;
		}
		if (!wm->LoadDevice(DeviceType,DeviceId,&datadevice)) {
			DeviceId=EditDeviceDialog(DeviceId);
			if (!DeviceId) {
				ui.index->setFocus();
				return true;
			}
			if (!wm->LoadDevice(DeviceType,DeviceId,&datadevice)) {
				ui.index->setFocus();
				return true;
			}
		}
		UpdateDevice();
		Tmp="1";
		ui.track->setText(Tmp);
		if (datadevice.Pages==1) {
			ui.titleEdit->setEnabled(true);
			ui.track->setFocus();
			ui.page->setEnabled(false);
			return true;
		}
	}
	ui.titleEdit->setEnabled(true);
	ui.page->deselect();
	ui.page->selectAll();
	return false;
}


// *****************************************************************************************************
// EVENT: track
bool Edit::on_track_FocusIn()
{
	DupeCheck=false;

	TrackNum=0;
	if (!Page) {
		Page=(ppluint8)ui.page->text().toInt();
		if (Page<1 || Page>datadevice.Pages) {
			ui.page->setFocus();
			return true;
		}
		// Trackliste laden
		if (TrackList) delete TrackList;
		TrackList=wm->GetTracklist(DeviceType,DeviceId,Page);
		if (!TrackList) {
			// TODO: Ein Bug, darf nicht vorkommen
			ui.index->setFocus();
			return true;
		}
		UpdateTrackListing();
		//asyncTrackUpdate.ThreadStop();
		//asyncTrackUpdate.ThreadStart();

		ppl6::CString a;
		a.Setf("%u",TrackList->GetMax()+1);
		ui.track->setText(a);
	}
	UpdateFkeys();
	UpdateCompleters();
	if (oimpInfo) {
		delete oimpInfo;
		oimpInfo=NULL;
	}
	ui.track->deselect();
	ui.track->selectAll();
	return false;

}

void Edit::ReloadTracks()
{
	if (TrackList) delete TrackList;
	TrackList=wm->GetTracklist(DeviceType,DeviceId,Page);
	if (!TrackList) {
		// TODO: Ein Bug, darf nicht vorkommen
		ui.index->setFocus();
		return;
	}
	UpdateTrackListing();
	ppl6::CString a;
	a.Setf("%u",TrackList->GetMax()+1);
	ui.track->setText(a);
	UpdateFkeys();
	UpdateCompleters();
	if (oimpInfo) {
		delete oimpInfo;
		oimpInfo=NULL;
	}
	this->setFocus();
	ui.track->setFocus();
	ui.track->deselect();
	ui.track->selectAll();
}

// *****************************************************************************************************
// EVENT: artist
bool Edit::on_artist_FocusIn()
{
	showEditorWithoutFocusChange();
	ui.artist->setFocus();
	ppl6::CString Tmp;
	if (!TrackNum) {
		if (!EditTrack()) {
			ui.track->setFocus();
			return true;
		}
	}
	if (artistCompleter) {
		//printf ("Artist::FocusIn , delete artistCompleter\n");
		delete artistCompleter;
	}
	artistCompleter=new QCompleter(Artists, this);
	artistCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	artistCompleter->setCompletionMode(QCompleter::PopupCompletion);
	artistCompleter->setModelSorting(QCompleter::UnsortedModel);
	ui.artist->setCompleter(artistCompleter);
	QTreeWidgetItem * w;
	int i=TrackNum;
	while (NULL==(w=trackList->topLevelItem(i-1)) && i>0) i--;
	if (w) {
		trackList->scrollToItem(w);
	}

	ui.artist->deselect();
	ui.artist->selectAll();
	return false;
}

bool Edit::on_artist_FocusOut()
{
	//printf ("Artist::FocusOut, setCompleter(NULL)\n");
	ui.artist->setCompleter(NULL);
	if (artistCompleter) {
		//printf ("Artist::FocusOut, delete artistCompleter\n");
		delete artistCompleter;
		artistCompleter=NULL;
	}
	return false;
}

// *****************************************************************************************************
// EVENT: title
bool Edit::on_title_FocusIn()
{
	if (titleCompleter) delete titleCompleter;
	titleCompleter=new QCompleter(Titles, this);
	titleCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	titleCompleter->setCompletionMode(QCompleter::PopupCompletion);
	ui.title->setCompleter(titleCompleter);

	ui.title->deselect();
	ui.title->selectAll();
	return false;
}

bool Edit::on_title_FocusOut()
{
	ui.title->setCompleter(NULL);
	if (titleCompleter) delete titleCompleter;
	titleCompleter=NULL;
	return false;
}

// *****************************************************************************************************
// EVENT: album
bool Edit::on_album_FocusIn()
{
	if (albumCompleter) delete albumCompleter;
	albumCompleter=new QCompleter(Albums, this);
	albumCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	albumCompleter->setCompletionMode(QCompleter::PopupCompletion);
	ui.album->setCompleter(albumCompleter);
	ui.album->deselect();
	ui.album->selectAll();
	return false;
}

bool Edit::on_album_FocusOut()
{
	ui.album->setCompleter(NULL);
	if (albumCompleter) delete albumCompleter;
	albumCompleter=NULL;
	return false;
}

bool Edit::on_length_FocusOut()
{
	ppl6::CString Tmp;
	ppluint32 l=Time2Int(Tmp=ui.length->text());
	if (l>0) Tmp.Setf("%0i:%02i",(int)(l/60),l%60); else Tmp.Clear();
	ui.length->setText(Tmp);
	return false;
}


// *****************************************************************************************************
// EVENT: LineEdit FocusIn
bool Edit::on_FocusIn(QLineEdit *widget)
{
	widget->deselect();
	widget->selectAll();
	return false;
}


bool Edit::on_f4_Pressed(int position)
{
	ppl6::CString Artist, Title;

	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.Trim();
	Title=ui.title->text();
	Title.Trim();
	if (position==4) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow,Artist);
		this->setFocus();
		ui.artist->setFocus();
	} else if (position==5) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow,Artist,Title);
		this->setFocus();
		ui.title->setFocus();
	}


	return true;
}

bool Edit::on_f5_ShortCut(int modifier)
{
	ppl6::CString Artist;
	Artist=ui.artist->text();
	Artist.Trim();

	if (modifier==Qt::NoModifier) {
		DataShortcut sc;
		if (wm->ShortcutStore.GetCopy(Artist, &sc)) {
			Artist=sc.GetArtist();
			ui.artist->setText(Artist);
			return true;
		}
	}
	ShortcutDialog Dialog(this,wm);
	Dialog.setModal(true);
	Dialog.SetShortcut(Artist);
	if (Dialog.exec()) {
		ui.artist->setText(Dialog.GetArtist());
	}
	return true;
}

bool Edit::on_f5_CheckDupes(QObject *target)
{
	ppl6::CString Artist, Title;

	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.Trim();
	Title=ui.title->text();
	Title.Trim();
	searchWindow=wm->OpenOrReuseSearch(searchWindow,Artist,Title);
	qApp->processEvents();
	qApp->processEvents();
	this->setFocus();
	if (target) ((QWidget*)target)->setFocus();
	return true;
}

// *****************************************************************************************************
// EVENT: F6 - MP3 ID3-Tag einlesen
bool Edit::on_f6_Pressed(__unused__ QObject *target, int modifier)
{
	ppl6::CString Tmp;
	ppl6::CString Path=wm->MP3Filename(DeviceId,Page,TrackNum);
	if (Path.IsEmpty()) {
		// Vielleicht gibt es noch einen Titel ohne Index
		Path=wm->NextMP3File(DeviceId,Page,TrackNum);
		if (Path.IsEmpty()) {
			QMessageBox::information(this, tr("WinMusik: Notice"),
					tr("There are no further titles without an index in the directory of this device"));
			return false;
		}
	}

	// Den Dateinamen nehmen wir in die Zwischenablage
	ppl6::CString Songname=ppl6::GetFilename(Path);
	ui.filename->setText(Songname);
	ppl6::CDirEntry de;
	if (ppl6::CFile::Stat(Path,de)) {
		Tmp.Setf("%0.1f",(double)de.Size/1048576.0);
		ui.filesize->setText(Tmp);
	}

	Songname.PregReplace("/\\.mp3$/i","");
	Songname.PregReplace("/^[0-9]+-/","");
	Songname.Replace("_"," ");
	Songname.Replace("\t"," ");
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(Songname);
	if (modifier==Qt::NoModifier) {
		if (wm->conf.bSaveOriginalMp3Tags) {
			if (wm->SaveOriginalMP3Info(Path,Oimp)) {
				Ti.ImportData=Oimp.Id;
				//ShowOimpInfo();
			}
		}
		TrackInfo tinfo;
		bool ret=getTrackInfoFromFile(tinfo,Path);
		if (ret) {
			CopyFromTrackInfo(tinfo);
			searchWindow=wm->OpenOrReuseSearch(searchWindow,tinfo.Ti.Artist,tinfo.Ti.Title);
			qApp->processEvents();
			qApp->setActiveWindow(this);
			this->setFocus();
			FixFocus();

		}
		return ret;
	}
	if (modifier==Qt::ShiftModifier) {
		wm->PlayFile(Path);
	}

	return false;

}

bool Edit::on_f7_DeleteTrack()
{
	if (Track.Track>0) {
		if (Track.TitleId) {
			TrackList->Delete(Track.Track);
			UpdateTrackListing();
			EditTrack();
			ui.artist->setFocus();
		} else {
			// Track löschen
			if (DeviceType==7) {
				wm->TrashMP3File(DeviceId,Page,Track.Track);
			}
			// Nachfolgende Tracks nach oben rücken
			TrackList->DeleteShift(Track.Track,&wm->TitleStore);
			UpdateTrackListing();
			QTreeWidgetItem * w=trackList->topLevelItem(Track.Track);
			if (w) {
				trackList->scrollToItem(w);
			}
			ClearEditFields();
			ui.track->setFocus();
		}
	}
	return false;
}

bool Edit::on_f8_InsertTrack()
{
	if (Track.Track>0) {
		TrackList->InsertShift(Track.Track,&wm->TitleStore);
		UpdateTrackListing();
		QTreeWidgetItem * w=trackList->topLevelItem(Track.Track);
		if (w) {
			trackList->scrollToItem(w);
		}
		ClearEditFields();
		EditTrack();
		ui.artist->setFocus();
	}
	return false;
}

bool Edit::on_f10_WritePlaylist()
{
	if (wm->WritePlaylist(DeviceId,Page,TrackList,&datadevice)) {
		QMessageBox::information(this, tr("WinMusik: Notice"),
				tr("Playlists wurden erfolgreich erstellt"));
		return true;
	}
	wm->RaiseError(this,tr("Could not create playlists"));
	return true;
}


void Edit::on_fkeys_clicked(int num)
{
	switch (num) {
		case 0: on_esc_clicked();
			break;
		case 1: on_f1_clicked();
			break;
		case 2: on_f2_clicked();
			break;
		case 3: on_f3_clicked();
			break;
		case 4: on_f4_clicked();
			break;
		case 5: on_f5_clicked();
			break;
		case 6: on_f6_clicked();
			break;
		case 7: on_f7_clicked();
			break;
		case 8: on_f8_clicked();
			break;
		case 9: on_f9_clicked();
			break;
		case 10: on_f10_clicked();
			break;
		case 11: on_f11_clicked();
			break;
		case 12: on_f12_clicked();
			break;
	}
}


void Edit::on_esc_clicked()
{
	if (position<2) {
		position=0;
		close();
		return;
	} else if (position==2) ui.index->setFocus();
	else if (position==3 && datadevice.Pages==1) ui.index->setFocus();
	else if (position==3 && datadevice.Pages>1) ui.page->setFocus();
	else if (position==4) ui.track->setFocus();
	else if (position>4) ui.artist->setFocus();
}

void Edit::on_f1_clicked()
{

}

void Edit::on_f2_clicked()
{
	QWidget *target=GetWidgetFromPosition(position);
	if (position>1 && target!=NULL) {
		ppluint32 ret=EditDeviceDialog(DeviceId);
		((QWidget*)target)->setFocus();
		if (ret==DeviceId) UpdateDevice();
	} else if (position==1) {
		ppluint32 ret=EditDeviceDialog(0);
		if (ret) {
			ppl6::CString Tmp;
			Tmp.Setf("%u",ret);
			ui.index->setText(Tmp);
			ui.page->setFocus();
			return;
		} else {
			ui.index->setFocus();
		}
	}
}
void Edit::on_f3_clicked()
{
	if (position==4 || position==5 || position==7 || position==9 || position==14
			|| position==16 || position==18 || position==20 || position==21) {
		QWidget *target=GetWidgetFromPosition(position);
		if (!target) return;
		QLineEdit *LineEdit=(QLineEdit*)target;
		ppl6::CWString Tmp=LineEdit->text().toLower();
		Tmp.UCWords();
		LineEdit->setText(Tmp);
	}
}
void Edit::on_f4_clicked()
{
	if (position>=4 && position<=5) on_f4_Pressed(position);
}
void Edit::on_f5_clicked()
{
	if (position==4) on_f5_ShortCut(Qt::NoModifier);
	else if(position>7) on_f5_CheckDupes(NULL);
}
void Edit::on_f6_clicked()
{
	if (DeviceType==7 && position==3) on_f6_MassImport();
	else if (DeviceType==7 && position>3) on_f6_Pressed(GetWidgetFromPosition(position),Qt::NoModifier);
}

void Edit::on_f7_clicked()
{
	if (position>3) on_f7_DeleteTrack();
}

void Edit::on_f8_clicked()
{
	if (position>3) on_f8_InsertTrack();
}

void Edit::on_f9_clicked()
{
	if (DeviceType==7 && position==3) on_f9_UpdateAllID3Tags();
	else if (DeviceType==7 && position>3 && Ti.ImportData>0) {
		if (oimpInfo) {
			delete oimpInfo;
			oimpInfo=NULL;
		} else if (wm->OimpDataStore.GetCopy(Ti.ImportData,&Oimp)) {
			ShowOimpInfo();
		}
		return;
	}
}

void Edit::on_f10_clicked()
{
	if (position>2 && DeviceType==7) on_f10_WritePlaylist();
}

void Edit::on_f11_clicked()
{
	wm->PrintCoverDialog(this,DeviceType,DeviceId);
}

void Edit::on_f12_clicked()
{
	SaveTrack();
}

bool Edit::on_trackList_MousePress(QMouseEvent * event)
{
	//printf ("MousePress\n");
	if (event->buttons() == Qt::LeftButton) startPos=event->pos();
	ratePos=event->pos();
	return false;
	/*
	QTreeWidget::mousePressEvent(event);
	QClipboard *clipboard = QApplication::clipboard();
	QList<QTreeWidgetItem*> Items=selectedItems();
	CTreeItem *item;
	TITEL *t;
	ppl6::CString Text, File;
	for (int i=0;i<Items.size();i++) {
		item=(CTreeItem *)Items[i];
		t=wm->GetTitel(item->TitelId);
		if (t!=NULL) {
			File.Setf("%s - %s (%s, %0.2f min, %s) [%s %u %s-%i]\n",t->Interpret, t->Titel,
					wm->GetVersion(t->Version), t->Laenge,wm->GetGenre(t->Genre),
					wm->GetTraegerShort(t->Traeger), t->TraegerIndex, wm->GetSeite(t->Seite), t->Track);
			Text+=File;
		}
	}
	clipboard->setText(Text,QClipboard::Clipboard);
	clipboard->setText(Text,QClipboard::Selection);
	*/
}

bool Edit::on_trackList_MouseRelease(QMouseEvent *)
{
	//printf ("MouseRelease\n");
	startPos.setX(0);
	startPos.setY(0);
	return false;
}

bool Edit::on_trackList_MouseMove(QMouseEvent *event)
{
	//printf ("MouseMove\n");
	if (!(event->buttons() == Qt::LeftButton)) {
		//QTreeWidget::mouseMoveEvent(event);
		return false;
	}
	if (DeviceType!=7) return false;
	if (event->pos().x()> trackList->columnViewportPosition (TRACKLIST_RATING_ROW)) return false;
	//QTreeWidgetItem *	itemAt ( const QPoint & p ) const

	int distance=(event->pos()-startPos).manhattanLength();
	//printf ("distance=%i\n", distance);
	if (distance<QApplication::startDragDistance()) {
		//QTreeWidget::mouseMoveEvent(event);
		return false;
	}

	WMTreeItem *item=(WMTreeItem*) trackList->currentItem();
	if (!item) return false;
	QList<QTreeWidgetItem*> Items=trackList->selectedItems();
	QList<QUrl> list;
	ppl6::CString File;
	QString qFile;
    QPixmap Icon;


	for (int i=0;i<Items.size();i++) {
		item=(WMTreeItem *)Items[i];
		if (Icon.isNull()) {
			DataTitle *ti=wm->GetTitle(item->Id);
			if (ti!=NULL &&ti->CoverPreview.Size()>0) {
				Icon.loadFromData((const uchar*)ti->CoverPreview.GetPtr(),ti->CoverPreview.GetSize());
			}
		}
		File=wm->MP3Filename(DeviceId,Page,item->Track);
		if (File.NotEmpty()) {
#ifdef _WIN32
			list.append(QUrl::fromLocalFile(File));
#else
			list.append(QUrl::fromLocalFile(File));
#endif
		}
	}
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    if (Icon.isNull()) Icon.load(":/devices48/resources/tr48x48-0007.png");
    drag->setPixmap(Icon);
    mimeData->setUrls(list);
    drag->setMimeData(mimeData);
    // start drag
    drag->exec(Qt::CopyAction | Qt::MoveAction);
	startPos.setX(0);
	startPos.setY(0);
	event->accept();

	return false;
}

bool Edit::on_f9_UpdateAllID3Tags()
{
	if (QMessageBox::question(this, tr("WinMusik: update all ID3-Tags"),
		tr("Update ID3-Tags of all tracks?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)
		==QMessageBox::No) return true;

	if (wm->UpdateID3Tags(DeviceId,Page,TrackList)) {
		QMessageBox::information(this, tr("WinMusik: Notice"),
				tr("Update of ID3-Tags on all tracks has been started"));
		return true;
	}
	wm->RaiseError(this,tr("Update of ID3-Tags failed"));
	return true;
}



void Edit::FixFocus()
{
	QWidget *widget=this->GetWidgetFromPosition(position);
	if (widget) widget->setFocus();
}

void Edit::on_trackList_itemClicked (QTreeWidgetItem * item, int column )
{
	DataTitle *t=wm->GetTitle(((WMTreeItem*)item)->Id);
	if (t) {
		QClipboard *clipboard = QApplication::clipboard();
		ppl6::CString Text;
		Text.Setf("%s - %s (%s, %0i:%02i min, %s)",t->Artist,t->Title,
			wm->GetVersionText(t->VersionId), t->Length/60,t->Length%60, wm->GetGenreText(t->GenreId));
		Text.Concatf(" [%s %u %c-%i]",(const char*)wm->GetDeviceNameShort(t->DeviceType),
				t->DeviceId,(t->Page+'A'-1),t->Track);
		clipboard->setText(Text,QClipboard::Clipboard);
		clipboard->setText(Text,QClipboard::Selection);

		if (column==TRACKLIST_RATING_ROW) {
			int x=ratePos.x()-trackList->columnViewportPosition (TRACKLIST_RATING_ROW);
			if (x<0) x=0;
			int r=x/10.666666;
			if (r!=t->Rating) {
				DataTitle tUpdate=*t;
				tUpdate.Rating=r;
				if (!wm->TitleStore.Put(&tUpdate)) {
					wm->RaiseError(this,tr("Could not save Title in TitleStore"));
					return;
				}
				switch (r) {
					case 0: item->setIcon(TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/sterne64x16-0.png"));
						item->setText(TRACKLIST_RATING_ROW,"0");
						break;
					case 1: item->setIcon(TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/sterne64x16-1.png"));
						item->setText(TRACKLIST_RATING_ROW,"1");
						break;
					case 2: item->setIcon(TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/sterne64x16-2.png"));
						item->setText(TRACKLIST_RATING_ROW,"2");
						break;
					case 3: item->setIcon(TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/sterne64x16-3.png"));
						item->setText(TRACKLIST_RATING_ROW,"3");
						break;
					case 4: item->setIcon(TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/sterne64x16-4.png"));
						item->setText(TRACKLIST_RATING_ROW,"4");
						break;
					case 5: item->setIcon(TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/sterne64x16-5.png"));
						item->setText(TRACKLIST_RATING_ROW,"5");
						break;
					case 6: item->setIcon(TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/sterne64x16-6.png"));
						item->setText(TRACKLIST_RATING_ROW,"6");
						break;
				}
			}
			//printf ("Rating: %i, %i\n",ratePos.x(),x);
		} else if (column==TRACKLIST_COVER_ROW && wm->IsCoverViewerVisible()==true) {
			ppl6::CID3Tag Tag;
			ppl6::CString File=wm->MP3Filename(DeviceId,Page,((WMTreeItem*)item)->Track);
			if (Tag.Load(&File)) {
				ppl6::CBinary cover;
				if (Tag.GetPicture(3,cover)) {
					QPixmap trackCover;
					trackCover.loadFromData((const uchar*)cover.GetPtr(),cover.GetSize());
					wm->UpdateCoverViewer(trackCover);
				}
			}
		}


		/* // Zu langsam
		if (position==3) {
			ppl6::CString Tmp;
			Tmp.Setf("%i",((WMTreeItem*)item)->Track);
			ui.track->setText(Tmp);
			EditTrack();
		}
		*/
	}
	//FixFocus();

}

void Edit::on_trackList_itemDoubleClicked ( QTreeWidgetItem * item, int column )
{
	if (DeviceType==7) {
		if (column==TRACKLIST_RATING_ROW) return;
		else if (column==TRACKLIST_COVER_ROW) {
			ppl6::CID3Tag Tag;
			ppl6::CString File=wm->MP3Filename(DeviceId,Page,((WMTreeItem*)item)->Track);
			if (Tag.Load(&File)) {
				ppl6::CBinary cover;
				if (Tag.GetPicture(3,cover)) {
					QPixmap trackCover;
					trackCover.loadFromData((const uchar*)cover.GetPtr(),cover.GetSize());
					wm->OpenCoverViewer(trackCover);
				}
			}
		} else {
			ppl6::CString Path=wm->MP3Filename(DeviceId,Page,((WMTreeItem*)item)->Track);
			if (Path.IsEmpty()) return;
			//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (const char*)Path);
			wm->PlayFile(Path);
		}
	}
	//FixFocus();
}


void Edit::on_trackList_customContextMenuRequested ( const QPoint & pos )
/*!\brief Kontext-Menue der Trackliste
 *
 * Diese Funktion wird aufgerufen, wenn der Anwender mit der rechten Maustaste
 * in die Trackliste klickt. Sie generiert ein Context-abhängiges Popup-Menu
 * an der Position des Klicks.
 *
 * \param[in] pos Mausposition des Klicks
 */
{
    QPoint p=ui.listWidget->mapToGlobal(pos);
    currentTrackListItem=(WMTreeItem*)trackList->itemAt(pos);
    if (!currentTrackListItem) return;
    //printf ("Custom Context %i\n",currentTrackListItem->Track);

    QMenu *m=new QMenu(this);
    m->setTitle("Ein Titel");
    QAction *a=m->addAction (QIcon(":/icons/resources/findmore.png"),tr("Find other versions","trackList Context Menue"),this,SLOT(on_contextFindMoreVersions_triggered()));
    m->addAction (QIcon(":/icons/resources/findmore-artist.png"),tr("Find more of artist","trackList Context Menue"),this,SLOT(on_contextFindMoreArtist_triggered()));
    m->addAction (QIcon(":/icons/resources/findmore-title.png"),tr("Find other artists of this title","trackList Context Menue"),this,SLOT(on_contextFindMoreTitle_triggered()));
    if (DeviceType==7) m->addAction (QIcon(":/icons/resources/play.png"),tr("Play Track","trackList Context Menue"),this,SLOT(on_contextPlayTrack_triggered()));
    m->addAction (QIcon(":/icons/resources/edit.png"),tr("Edit Track","trackList Context Menue"),this,SLOT(on_contextEditTrack_triggered()));
    m->addAction (QIcon(":/icons/resources/copytrack.png"),tr("Copy Artist and Title","trackList Context Menue"),this,SLOT(on_contextCopyTrack_triggered()));
    if (DeviceType==7) m->addAction (QIcon(":/icons/resources/copyfile.png"),tr("Copy MP3-File","trackList Context Menue"),this,SLOT(on_contextCopyFile_triggered()));
    m->addSeparator();
    m->addAction (QIcon(":/icons/resources/delete-track.png"),tr("Delete Track","trackList Context Menue"),this,SLOT(on_contextDeleteTrack_triggered()));
    m->addAction (QIcon(":/icons/resources/insert-track.png"),tr("Insert Track","trackList Context Menue"),this,SLOT(on_contextInsertTrack_triggered()));
    if (DeviceType==7 && TrackList != NULL &&(
    		trackList->currentColumn()==TRACKLIST_BPM_ROW
    		|| trackList->currentColumn()==TRACKLIST_KEY_ROW )) {
    	m->addSeparator();
    	m->addAction (QIcon(":/icons/resources/edit.png"),tr("Read BPM and Key from ID3-Tag","trackList Context Menue"),this,SLOT(on_contextReadBpmAndKey_triggered()));

    }
    m->popup(p,a);
    //FixFocus();
}

void Edit::on_contextReadBpmAndKey_triggered()
{
	if (!TrackList) return;
	// Höchste Tracknummer
	int max=TrackList->GetMax();

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QProgressDialog progress(tr("Reading ID3-Tags from Files..."), tr("Abort"), 0, max, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setWindowTitle(QString(WM_APPNAME)+QString(": ")+tr("Reading ID3-Tags from Files..."));
	progress.setMinimumWidth(500);
	progress.setMaximumWidth(500);
	progress.show();

	QCoreApplication::processEvents();
	for (int i=1;i<=max;i++) {
		progress.setValue(i);
		QCoreApplication::processEvents();
		if (progress.wasCanceled())	break;
		DataTrack *track=TrackList->Get(i);
		if (track) {
			// Titel holen
			DataTitle *title=wm->GetTitle(track->TitleId);
			ppl6::CString Path=wm->MP3Filename(track->DeviceId,track->Page,track->Track);
			if (title!=NULL && Path.NotEmpty()==true) {
				//printf ("Path: %s\n",(const char*)Path);
				progress.setLabelText(Path);
				TrackInfo tinfo;
				if (getTrackInfoFromFile(tinfo,Path)) {
					if (tinfo.Ti.Key != title->Key || tinfo.Ti.BPM != title->BPM) {
						title->Key=tinfo.Ti.Key;
						title->BPM=tinfo.Ti.BPM;
						if (!wm->TitleStore.Put(title)) {
							wm->RaiseError(this,tr("Could not save Title in TitleStore"));
							break;
						}
					}
				}
			}
		}
	}


	QApplication::restoreOverrideCursor();
	UpdateTrackListing();
}


void Edit::on_contextFindMoreVersions_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow,t->Artist,t->Title);
	}
}

void Edit::on_contextFindMoreArtist_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow,t->Artist);
	}
}

void Edit::on_contextFindMoreTitle_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow,NULL,t->Title);
	}
}

void Edit::on_contextPlayTrack_triggered()
{
	ppl6::CString Path=wm->MP3Filename(DeviceId,Page,currentTrackListItem->Track);
	if (Path.IsEmpty()) return;
	//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (const char*)Path);
	wm->PlayFile(Path);
}

void Edit::on_contextEditTrack_triggered()
{
	showEditor();
	ppl6::CString Tmp;
	Tmp.Setf("%i",currentTrackListItem->Track);
	TrackNum=0;
	ui.track->setFocus();
	ui.track->setText(Tmp);
	ui.artist->setFocus();
}

void Edit::on_contextCopyTrack_triggered()
{
	on_trackList_itemClicked(currentTrackListItem,0);
}

void Edit::on_contextCopyFile_triggered()
{
	if (DeviceType==7 && currentTrackListItem->Track>0) {
		ppl6::CString Path=wm->MP3Filename(DeviceId,Page,currentTrackListItem->Track);
		if (Path.IsEmpty()) return;
		QClipboard *clipboard = QApplication::clipboard();
		QList<QUrl> list;
		QString qf="file:://";
		qf+=(const char*)Path;
		list.append(QUrl(qf));
	    QMimeData *mimeData = new QMimeData;
	    mimeData->setUrls(list);
	    //printf ("Adding to Clipboard\n");
	    clipboard->setMimeData(mimeData,QClipboard::Clipboard);
	}
}

void Edit::on_contextDeleteTrack_triggered()
{
	if (!currentTrackListItem->Track) return;
	//printf ("currentTrackListItem->Track=%u, currentTrackListItem->Id=%u\n",currentTrackListItem->Track,currentTrackListItem->Id);
	if (currentTrackListItem->Id) {
		TrackList->Delete(currentTrackListItem->Track);
	} else {
		// Nachfolgende Tracks nach oben rücken
		TrackList->DeleteShift(currentTrackListItem->Track,&wm->TitleStore);
	}
	UpdateTrackListing();
	QTreeWidgetItem * w=trackList->topLevelItem(currentTrackListItem->Track);
	if (w) {
		trackList->scrollToItem(w);
	}
	ppl6::CString Tmp;
	Tmp.Setf("%u",currentTrackListItem->Track);
	ui.track->setText(Tmp);
	ui.track->setFocus();
}

void Edit::on_contextInsertTrack_triggered()
{
	if (!currentTrackListItem->Track) return;
	TrackList->InsertShift(currentTrackListItem->Track,&wm->TitleStore);
	UpdateTrackListing();
	QTreeWidgetItem * w=trackList->topLevelItem(currentTrackListItem->Track);
	if (w) {
		trackList->scrollToItem(w);
	}
	ppl6::CString Tmp;
	Tmp.Setf("%u",currentTrackListItem->Track);
	ui.track->setText(Tmp);
	ui.track->setFocus();
}

void Edit::on_coverCopyButton_clicked()
{
	if (position<3) return;
	QClipboard *clipboard = QApplication::clipboard();
	if (!clipboard) return;
	clipboard->setPixmap(Cover);
	FixFocus();
}

void Edit::on_coverInsertButton_clicked()
{
	if (position<3) return;
	QClipboard *clipboard = QApplication::clipboard();
	if (!clipboard) return;
	if (clipboard->pixmap().isNull()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	Cover=clipboard->pixmap();
	UpdateCover();
	FixFocus();
	QApplication::restoreOverrideCursor();
}

void Edit::on_coverDeleteButton_clicked()
{
	FixFocus();
	if (position<3) return;
	if (Cover.isNull()) return;
	if (QMessageBox::question(this, tr("WinMusik: delete MP3-Cover"),
		tr("Do you want to remove the cover from the mp3 file?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)
		==QMessageBox::No) return;
	Cover=QPixmap();
	ui.cover->setPixmap(Cover);
	wm->UpdateCoverViewer(Cover);
	if (DeviceType==7) {
		ppl6::CString Path=wm->MP3Filename(DeviceId,Page,TrackNum);
		if (Path.NotEmpty()) {
			ppl6::CID3Tag Tag;
			Tag.Load(&Path);
			Tag.RemovePicture(3);
			Tag.Save(wm->conf.bWriteId3v1,wm->conf.bWriteId3v2);
		}
	}


}

void Edit::on_coverLoadButton_clicked()
{
	FixFocus();
	if (position<3) return;

	ppl6::CString Dir=wm->conf.LastCoverPath+"/";
	if (Dir.IsEmpty()) {
		Dir=QDir::homePath();
	}
	QString newfile = QFileDialog::getOpenFileName(this, tr("Select cover image"),
			Dir,
			tr("Images (*.png *.bmp *.jpg)"));
	if (newfile.isNull()) return;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	wm->conf.LastCoverPath=ppl6::GetPath(newfile);
	wm->conf.Save();
	if (!Cover.load(newfile)) {
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this,tr("Error: could not load Cover"),
				tr("The soecified file could not be loaded.\nPlease check if the file exists, is readable and contains an image format, which is supported by WinMusik (.png, .jpg or .bmp)")
				);
		return;
	} else {
		UpdateCover();
	}
	QApplication::restoreOverrideCursor();
}

void Edit::on_coverSaveButton_clicked()
{
	FixFocus();
	if (position<3) return;
	if (Cover.isNull()) return;
	ppl6::CString Dir=wm->conf.LastCoverPath+"/";
	Dir+=ppl6::Mid(ppl6::GetFilename(wm->NormalizeFilename(Track.DeviceId,Page,Track.Track,Ti)),4);
	Dir.Cut(Dir.Length()-4);
	Dir+=".jpg";
	Dir.Print(true);

	QString newfile = QFileDialog::getSaveFileName(this, tr("Save cover to file"),
				Dir,
				tr("Images (*.png *.bmp *.jpg)"));
	if (newfile.isNull()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	wm->conf.LastCoverPath=ppl6::GetPath(newfile);
	wm->conf.Save();
	if (!Cover.save (newfile)) {
		/*
		 * StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton ) [static]
		 *
		 */
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this,tr("Error: could not save Cover"),
				tr("The cover of this track could not be saved.\nPlease check if the target directory exists and is writable.\nPlease also check the file extension. WinMusik only supports .png, .jpg and .bmp")
				);
		return;
	}
	QApplication::restoreOverrideCursor();
}



void Edit::on_coverSearchAmazon_clicked()
{
	FixFocus();
	if (position<3) return;
	ppl6::CString Url;
	// Interpret und Titel
	ppl6::CString Artist=ui.artist->text();
	Artist.Trim();
	ppl6::CString Title=ui.title->text();
	Title.Trim();

	// Version

	// http://www.amazon.de/s/ref=nb_sb_ss_i_0_16?__mk_de_DE=%C5M%C5Z%D5%D1&url=search-alias%3Ddigital-music&field-keywords=armin+van+buuren+mirage&x=0&y=0&sprefix=Armin+van+Buuren%2Cdigital-music%2C148
	// http://www.google.de/search?q=Armin+van+buuren+mirage&hl=de&client=firefox-a&hs=3fV&rls=org.mozilla:de-DE:official&prmd=imvnsol&source=lnms&tbm=isch&ei=K-ERT8m9KMrsOdDn2YYD&sa=X&oi=mode_link&ct=mode&cd=2&ved=0CCcQ_AUoAQ&biw=971&bih=532#hl=de&client=firefox-a&hs=m0&rls=org.mozilla:de-DE%3Aofficial&tbm=isch&sa=1&q=Armin+van+buuren+mirage+cover&pbx=1&oq=Armin+van+buuren+mirage+cover&aq=f&aqi=g1&aql=&gs_sm=e&gs_upl=1037l2111l0l2973l6l6l0l3l3l0l314l610l1.1.0.1l3l0&bav=on.2,or.r_gc.r_pw.,cf.osb&fp=89a7740a921e9709&biw=971&bih=532
	// http://www.google.de/search?q=Armin+van+buuren+mirage&hl=de&client=firefox-a&rls=org.mozilla:de-DE:official&tbm=vid&source=lnms&ei=E-IRT5TMHIqfOpnU1dQG&sa=X&oi=mode_link&ct=mode&cd=4&ved=0CDEQ_AUoAw&biw=971&bih=532
	//s/?ie=UTF8&keywords=amazon+suchen&tag=googhydr08-21&index=aps&hvadid=9259989001&ref=pd_sl_4uh4d1aa33_b

	//Url="http://www.google.com/search?q=cover+"+ppl6::UrlEncode(Artist)+"+"+ppl6::UrlEncode(Title)+"&tbm=isch";

	Url="http://www.amazon.de/s/?ie=UTF8&index=digital-music&keywords="+ppl6::UrlEncode(Artist)+"+"+ppl6::UrlEncode(Title);

	//Url="http://www.pfp.de/";
	ppl6::CString Cmd;
	Cmd="firefox \""+Url+"\" &";
	system(Cmd);
	//QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

void Edit::on_coverSearchGoogle_clicked()
{
	FixFocus();
	if (position<3) return;
	ppl6::CString Url;
	// Interpret und Titel
	ppl6::CString Artist=ui.artist->text();
	Artist.Trim();
	ppl6::CString Title=ui.title->text();
	Title.Trim();

	Url="https://www.google.com/search?num=10&site=imghp&tbm=isch&q="+ppl6::UrlEncode(Artist)+"+"+ppl6::UrlEncode(Title);
	ppl6::CString Cmd;
	Cmd="firefox \""+Url+"\" &";
	system(Cmd);
	//QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

void Edit::on_coverSearchBeatport_clicked()
{
	FixFocus();
	if (position<3) return;
	ppl6::CString Url;
	// Interpret und Titel
	ppl6::CString Artist=ui.artist->text();
	Artist.Trim();
	ppl6::CString Title=ui.title->text();
	Title.Trim();

	Url="http://www.beatport.com/search?query="+ppl6::UrlEncode(Artist)+"+"+ppl6::UrlEncode(Title);

	ppl6::CString Cmd;
	Cmd="firefox \""+Url+"\" &";
	system(Cmd);
	//QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

void Edit::on_coverSearchSoundcloud_clicked()
{
	FixFocus();
	if (position<3) return;
	ppl6::CString Url;
	// Interpret und Titel
	ppl6::CString Artist=ui.artist->text();
	Artist.Trim();
	ppl6::CString Title=ui.title->text();
	Title.Trim();

	Url="https://soundcloud.com/search?q="+ppl6::UrlEncode(Artist)+"+"+ppl6::UrlEncode(Title);

	ppl6::CString Cmd;
	Cmd="firefox \""+Url+"\" &";
	system(Cmd);
	//QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

bool Edit::on_f6_MassImport()
{
	MassImport Import(this,wm);
	Import.setSearchWindow(searchWindow);
	Import.show();
	if (!Import.load(DeviceType,DeviceId,Page,TrackList->GetMax()+1)) {
		return true;
	}
	Import.exec();
	searchWindow=Import.getSearchWindow();
	ReloadTracks();


	return true;
}

void Edit::on_hideEditor_clicked()
{
	if (ui.titleEdit->isVisible()) {
		hideEditor();
	} else {
		showEditor();
	}
}

