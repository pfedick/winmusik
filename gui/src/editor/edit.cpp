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

#include "winmusik3.h"
#include <stdio.h>
#include "edit.h"
#include "editdevice.h"
#include "tablesearch.h"
#include "shortcutdialog.h"
#include "massimport.h"
#include "csearchlist.h"
#include "musickey.h"
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
#include <QMimeData>
#include <QDrag>
#include <QDomDocument>
#include <list>




CTitleList::CTitleList(QWidget* parent)
	: QTreeWidget(parent)
{
	edit=NULL;
	wm=NULL;
	this->setAcceptDrops(true);
	this->setDropIndicatorShown(true);
}


void CTitleList::mousePressEvent(QMouseEvent* event)
{
	//printf ("mousePressEvent\n");
	edit->on_trackList_MousePress(event);
	QTreeWidget::mousePressEvent(event);
}

void CTitleList::mouseReleaseEvent(QMouseEvent* event)
{
	//printf ("mouseReleaseEvent\n");
	edit->on_trackList_MouseRelease(event);
	QTreeWidget::mouseReleaseEvent(event);
}


void CTitleList::mouseMoveEvent(QMouseEvent* event)
{
	//printf ("mouseMoveEvent\n");
	edit->on_trackList_MouseMove(event);
	//QTreeWidget::mouseMoveEvent(event);
}

void CTitleList::focusInEvent(QFocusEvent* event)
{
	QTreeWidget::focusInEvent(event);
	edit->FixFocus();
}

/*
void CTitleList::dragEnterEvent ( QDragEnterEvent * event )
{
	if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
		event->acceptProposedAction();
	}
}
*/

QStringList CTitleList::mimeTypes() const
{
	QStringList qstrList;
	// list of accepted mime types for drop
	qstrList.append("text/uri-list");
	return qstrList;
}


Qt::DropActions CTitleList::supportedDropActions() const
{
	// returns what actions are supported when dropping
	return Qt::CopyAction | Qt::MoveAction;
}

/*
bool CTitleList::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{
	printf("CTitleList::dropMimeData\n");
	//edit->handleTrackListDropEvent(data,parent);
	return true;
}
*/

void CTitleList::dropEvent(QDropEvent* event)
{
	if (event->source() == this || event->source() == edit) {
		event->ignore();
		return;
	}
	//printf ("CTitleList::dropEvent, action: %i\n",event->dropAction());
	QList<QUrl> urlList=event->mimeData()->urls();
	edit->handleDropOnTracklist(urlList, event->dropAction());
	event->acceptProposedAction();
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

Edit::Edit(QWidget* parent, CWmClient* wm, int typ)
	: QWidget(parent)
	/*!\brief Konstruktor der Klasse
	 *
	 * Der Konstruktor der Klasse baut das Interface auf und übersetzt alle Text. Ausserdem werden diverse
	 * Variablen initialisiert.
	 */
{
	ui.setupUi(this);
	this->wm=wm;
	wm->RegisterWindow(WindowType::Editor, this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	DeviceType=typ;
	DeviceId=0;
	Page=0;
	TrackList=NULL;
	position=0;
	oldposition=0;
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
	InstallFilter(ui.index, 1);
	InstallFilter(ui.page, 2);
	InstallFilter(ui.track, 3);
	InstallFilter(ui.artist, 4);
	InstallFilter(ui.title, 5);
	InstallFilter(ui.versionId, 6);
	InstallFilter(ui.version, 7);
	InstallFilter(ui.genreId, 8);
	InstallFilter(ui.genre, 9);
	InstallFilter(ui.length, 10);
	InstallFilter(ui.bpm, 11);
	InstallFilter(ui.releaseDate, 12);
	InstallFilter(ui.recordDate, 13);
	InstallFilter(ui.album, 14);
	InstallFilter(ui.labelId, 15);
	InstallFilter(ui.labelName, 16);
	InstallFilter(ui.recordSourceId, 17);
	InstallFilter(ui.recordSource, 18);
	InstallFilter(ui.recordDeviceId, 19);
	InstallFilter(ui.recordDevice, 20);
	InstallFilter(ui.remarks, 21);
	InstallFilter(ui.tags, 22);
	InstallFilter(ui.complete, 23);
	InstallFilter(ui.realTitle, 24);
	InstallFilter(ui.interrupted, 25);
	InstallFilter(ui.channels, 26);
	InstallFilter(ui.quality, 27);
	InstallFilter(ui.rating, 28);
	InstallFilter(ui.bitrate, 29);
	InstallFilter(ui.musickey, 30);
	InstallFilter(ui.energyLevel, 31);


	ui.deviceTitle->installEventFilter(this);
	ui.titleEdit->installEventFilter(this);

	/*
	 * 	Label.Title=tr("Producer / Label");
	Label.Init(this,wm,ui.labelId,ui.labelText,&wm->LabelStore);
	Label.SetNextWidget(ui.purchaseId);
	 *
	 */
	TCVersion.Title=tr("Version");
	TCVersion.Init(this, wm, ui.versionId, ui.version, &wm->VersionStore);
	TCVersion.SetNextWidget(ui.genreId);

	TCGenre.Title=tr("Genre");
	TCGenre.Init(this, wm, ui.genreId, ui.genre, &wm->GenreStore);
	TCGenre.SetNextWidget(ui.length);

	TCLabel.Title=tr("Label");
	TCLabel.Init(this, wm, ui.labelId, ui.labelName, &wm->LabelStore);
	TCLabel.SetNextWidget(ui.recordSourceId);

	TCRecordSource.Title=tr("Record Source");
	TCRecordSource.Init(this, wm, ui.recordSourceId, ui.recordSource, &wm->RecordSourceStore);
	TCRecordSource.SetNextWidget(ui.recordDeviceId);

	TCRecordDevice.Title=tr("Record Device");
	TCRecordDevice.Init(this, wm, ui.recordDeviceId, ui.recordDevice, &wm->RecordDeviceStore);
	TCRecordDevice.SetNextWidget(ui.remarks);



	ui.deviceIcon->setIcon(wm->GetDeviceIcon(typ));
	setWindowIcon(wm->GetDeviceIcon(typ));

	QString WmTitle=wm->GetWmVersion() + " - ";
	switch (typ) {
		case 1:	// Cassette
			setWindowTitle(WmTitle + tr("Edit Music Cassette"));
			break;
		case 2:	// CD
			setWindowTitle(WmTitle + tr("Edit Audio CD"));
			break;
		case 3:	// Data CD
			setWindowTitle(WmTitle + tr("Edit Data CD"));
			break;
		case 4:	// Vinyl Record
			setWindowTitle(WmTitle + tr("Edit Vinyl Record"));
			break;
		case 5:	// VHS
			setWindowTitle(WmTitle + tr("Edit Video Tape"));
			break;
		case 6:	// Other
			setWindowTitle(WmTitle + tr("Edit Other Media Type"));
			break;
		case 7:	// MP3
			setWindowTitle(WmTitle + tr("Edit MP3 CD"));
			break;
		case 8:	// Tape
			setWindowTitle(WmTitle + tr("Edit Music Tape"));
			break;
		case 9:	// DVD
			setWindowTitle(WmTitle + tr("Edit Audio DVD"));
			break;
	}

	// MusicKey
	musicKeyDisplay=wm->conf.musicKeyDisplay;
	switch (musicKeyDisplay) {
		case musicKeyTypeMusicalSharps: ui.displayMusicKey->setCurrentIndex(0); break;
		case musicKeyTypeMusicalFlats: ui.displayMusicKey->setCurrentIndex(1); break;
		case musicKeyTypeOpenKey: ui.displayMusicKey->setCurrentIndex(2); break;
		case musicKeyTypeCustom: ui.displayMusicKey->setCurrentIndex(3); break;
		default: ui.displayMusicKey->setCurrentIndex(1); break;
	}
	ui.displayMusicKey->setItemText(3, wm->conf.customMusicKeyName);


	uint32_t highest=wm->GetHighestDeviceId(typ);
	if (highest) {
		ppl7::String Tmp;
		Tmp.setf("%u", highest);
		ui.index->setText(Tmp);
	}
	ui.index->setFocus();
	hideEditor();
}

Edit::~Edit()
{
	if (DupeTimer) delete DupeTimer;
	if (titleCompleter) delete titleCompleter;
	if (artistCompleter) delete artistCompleter;
	if (albumCompleter) delete albumCompleter;
	//if (searchWindow) delete searchWindow;
	wm->UnRegisterWindow(WindowType::Editor, this);
}

void Edit::setTracklistCaptions()
{
	trackList->headerItem()->setText(TRACKLIST_TRACK_ROW, tr("Track", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_COVER_ROW, tr("Cover", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_NAME_ROW, tr("Artist - Title", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_VERSION_ROW, tr("Version", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_GENRE_ROW, tr("Genre", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_LENGTH_ROW, tr("Length", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_BPM_ROW, tr("BPM", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_KEY_ROW, tr("Key", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_ENERGYLEVEL_ROW, tr("Energy", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_YEAR, tr("Year", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_BITRATE_ROW, tr("Bitrate", "trackList"));
	trackList->headerItem()->setText(TRACKLIST_RATING_ROW, tr("Rating", "trackList"));

}

void Edit::customEvent(QEvent* event)
{
	if (event->type() == (QEvent::Type)WinMusikEvent::retranslateUi) {
		ui.retranslateUi(this);
		setTracklistCaptions();
		UpdateFkeys();
		event->accept();
	}
}

void Edit::show()
{
	ppl7::String Name;
	Name.setf("edit_type_%i", DeviceType);
	SetWindowGeometry(this, Name);
	QWidget::show();
}

void Edit::OpenTrack(uint32_t deviceId, uint8_t page, uint16_t track)
{
	TrackList.Clear();
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
	ui.index->setText(ppl7::ToString("%i", deviceId));
	if (!Page) Page=1;
	ui.page->setText(ppl7::ToString("%i", Page));
	wm->UpdateDevice(DeviceType, DeviceId);
	if (wm->LoadDevice(DeviceType, DeviceId, &datadevice)) {
		UpdateDevice();
		if (datadevice.Pages == 1) {
			ui.page->setEnabled(false);
		}
		TrackList=wm->GetTracklist(DeviceType, DeviceId, Page);
		UpdateTrackListing();
		ui.titleEdit->setEnabled(true);
		ui.track->setFocus();
		if (track > 0 || TrackList.Num() == 0) {
			showEditor();
			ui.track->setText(ppl7::ToString("%i", track));
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
	QLayout* w=ui.listWidget->layout();
	trackList=new CTitleList(ui.listWidget);
	w->addWidget(trackList);
	trackList->edit=this;
	trackList->wm=wm;

	//trackList->installEventFilter(this);
	//trackList->setMouseTracking(true);
	trackList->setDragEnabled(false);
	trackList->setDragDropMode(QAbstractItemView::DragDrop);
	//trackList->setDropIndicatorShown(true);


	trackList->setObjectName(QString::fromUtf8("trackList"));
	trackList->setAlternatingRowColors(true);
	trackList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	trackList->setRootIsDecorated(false);
	trackList->setItemsExpandable(false);
	trackList->setSortingEnabled(false);
	trackList->setContextMenuPolicy(Qt::CustomContextMenu);

	setTracklistCaptions();
	connect(trackList, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(on_trackList_customContextMenuRequested(const QPoint&)));
	connect(trackList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(on_trackList_itemDoubleClicked(QTreeWidgetItem*, int)));
	connect(trackList, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
		this, SLOT(on_trackList_itemClicked(QTreeWidgetItem*, int)));

	/*
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
			*/
	QString Style="QTreeWidget::item {\n"
		"margin: 0px;\n"
		"padding-top: 0px;\n"
		"padding-bottom: 0px;\n"
		"border: 0px;\n"
		"}\n";
	//trackList->setStyleSheet(Style);

}


void Edit::InstallFilter(QObject* object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id", id);
}

bool Edit::eventFilter(QObject* target, QEvent* event)
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
	if (consumeEvent(target, event)) return true;
	return QWidget::eventFilter(target, event);
}

bool Edit::consumeEvent(QObject* target, QEvent* event)
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
	QKeyEvent* keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;
	QFocusEvent* focusEvent=NULL;

	// Id auslesen
	int id=target->property("id").toInt();
	int type=event->type();
	if (type == QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent*>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (on_KeyPress(target, key, modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	} else if (type == QEvent::FocusIn || type == QEvent::FocusOut) {
		focusEvent=static_cast<QFocusEvent*>(event);
		if (type == QEvent::FocusIn) {
			if (id > 3 && ui.track->text().toInt() < 1) { // Das erlauben wir nur, wenn eine Tracknummer angegeben ist
				ui.track->setFocus();
				return true;
			}
			position=id;
			UpdateFkeys();
			if (position > 7 && DupeCheck == false) {
				CheckDupes();
			} else if (position < 8 && DupeCheck == true) DupeCheck=false;
		} else if (type == QEvent::FocusOut) {
			oldposition=id;
			if (oldposition == 10) on_length_FocusOut();
		}
	}

	if (target == ui.index) {
		if (type == QEvent::FocusIn) return on_index_FocusIn();
		if (type == QEvent::KeyPress) return on_index_KeyPress(keyEvent, key, modifier);
	} else if (target == ui.page) {
		if (type == QEvent::FocusIn) return on_page_FocusIn();
	} else if (target == ui.track) {
		if (type == QEvent::FocusIn) return on_track_FocusIn();
	} else if (target == ui.artist) {
		if (type == QEvent::FocusIn) {
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
			if (reason != Qt::PopupFocusReason) return on_artist_FocusIn();
			return false;
		}
		if (type == QEvent::FocusOut) {
			return on_artist_FocusOut();
		}
	} else if (target == ui.title) {
		if (type == QEvent::FocusIn && focusEvent->reason() != Qt::PopupFocusReason) return on_title_FocusIn();
		if (type == QEvent::FocusOut) return on_title_FocusOut();
	} else if (target == ui.version || target == ui.versionId) {
		return TCVersion.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.genre || target == ui.genreId) {
		return TCGenre.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.labelName || target == ui.labelId) {
		return TCLabel.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.recordSource || target == ui.recordSourceId) {
		return TCRecordSource.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.recordDevice || target == ui.recordDeviceId) {
		return TCRecordDevice.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.album) {
		if (type == QEvent::FocusIn && focusEvent->reason() != Qt::PopupFocusReason) return on_album_FocusIn();
		if (type == QEvent::FocusOut) return on_album_FocusOut();
	} else if (target == ui.length) {
		if (type == QEvent::FocusIn) return on_FocusIn(ui.length);
	} else if (target == ui.bpm) {
		if (type == QEvent::FocusIn) return on_FocusIn(ui.bpm);
	} else if (target == ui.musickey) {
		if (type == QEvent::FocusIn) return on_FocusIn(ui.musickey);
	} else if (target == ui.bitrate) {
		if (type == QEvent::FocusIn) return on_FocusIn(ui.bitrate);
	} else if (target == ui.album) {
		if (type == QEvent::FocusIn) return on_FocusIn(ui.album);
	} else if (target == ui.remarks) {
		if (type == QEvent::FocusIn) return on_FocusIn(ui.remarks);
	} else if (target == ui.tags) {
		if (type == QEvent::FocusIn) return on_FocusIn(ui.tags);
	} else if (target == ui.titleEdit && type == QEvent::Drop) {
		handleDropEvent(static_cast<QDropEvent*>(event));
		return true;
	} else if ((target == ui.artist || ui.title) && type == QEvent::Drop) {
		if (handleDropFromSearchlist(static_cast<QDropEvent*>(event))) {
			return true;
		}
	} else if ((target == ui.deviceTitle || ui.titleEdit) && type == QEvent::DragEnter) {
		//(static_cast<QDragEnterEvent *>(event))->accept();
		if (handleDragEnterEvent(static_cast<QDragEnterEvent*>(event))) {
			return true;
		}
	}
	return false;
}

void Edit::CheckDupes()
{
	DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";
	DupeCheck=true;
	TCVersion.Finish();
	ppl7::String Tmp, Artist, Title;
	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.trim();
	Title=ui.title->text();
	Title.trim();
	uint32_t Version=ui.versionId->text().toInt();
	if (wm->Hashes.CheckDupes(Artist, Title, Version, Ti.TitleId)) {
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
	if (count & 1) DupeCheckIcon=":/fkeys/resources/fkeys/f-key-3005.png";
	else DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";
	if (position > 7) {
		ui.fkeys->setFkey(5, DupeCheckIcon, "");
	}
}

void Edit::resizeEvent(QResizeEvent* event)
/*!\brief Größenänderung des Fensters
 *
 * Diese Funktion wird durch Qt aufgerufen, wenn sich die Größe
 * des Fensters ändert, sowie von Edit::UpdateTrackListing, wenn sich der
 * Inhalt der Trackliste ändert.
 */
{
	int w=trackList->width();
	int space=8;
	trackList->setColumnWidth(TRACKLIST_TRACK_ROW, 50);
	w-=(50 + space);
	trackList->setColumnWidth(TRACKLIST_COVER_ROW, 64);
	w-=(60 + space);
	trackList->setColumnWidth(TRACKLIST_LENGTH_ROW, 60);
	w-=(60 + space);
	trackList->setColumnWidth(TRACKLIST_BPM_ROW, 35);
	w-=(35 + space);
	trackList->setColumnWidth(TRACKLIST_KEY_ROW, 40);
	w-=(40 + space);
	trackList->setColumnWidth(TRACKLIST_ENERGYLEVEL_ROW, 30);
	w-=(30 + space);
	trackList->setColumnWidth(TRACKLIST_YEAR, 40);
	w-=(40 + space);
	trackList->setColumnWidth(TRACKLIST_BITRATE_ROW, 30);
	w-=(30 + space);

	trackList->setColumnWidth(TRACKLIST_RATING_ROW, 55);
	w-=(55 + space);
	trackList->setColumnWidth(TRACKLIST_NAME_ROW, w * 55 / 100 - 6);
	trackList->setColumnWidth(TRACKLIST_VERSION_ROW, w * 30 / 100 - 6);
	trackList->setColumnWidth(TRACKLIST_GENRE_ROW, w * 15 / 100 - 6);
	QWidget::resizeEvent(event);
}

void Edit::closeEvent(QCloseEvent* event)
{
	ppl7::String Name;
	Name.setf("edit_type_%i", DeviceType);
	if (wm) {
		wm->SaveGeometry(Name, this->saveGeometry());
	}
	QWidget::closeEvent(event);
}

void Edit::moveEvent(QMoveEvent* event)
{
	QWidget::moveEvent(event);
}


void Edit::SetFkey(QToolButton* button, const char* Icon, QString Text, bool enabled)
{
	button->setIcon(QIcon(Icon));
	if (Text.size()) button->setText(Text);
	button->setEnabled(enabled);
}


void Edit::UpdateFkeys()
{
	ui.fkeys->setFkey(0, ":/fkeys/resources/fkeys/f-key-0000.png", " ", false);
	ui.fkeys->setFkey(1, ":/fkeys/resources/fkeys/f-key-0001.png", " ", false);
	ui.fkeys->setFkey(2, ":/fkeys/resources/fkeys/f-key-0002.png", " ", false);
	ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-0003.png", " ", false);
	ui.fkeys->setFkey(4, ":/fkeys/resources/fkeys/f-key-0004.png", " ", false);
	ui.fkeys->setFkey(5, ":/fkeys/resources/fkeys/f-key-0005.png", " ", false);
	ui.fkeys->setFkey(6, ":/fkeys/resources/fkeys/f-key-0006.png", " ", false);
	ui.fkeys->setFkey(7, ":/fkeys/resources/fkeys/f-key-0007.png", " ", false);
	ui.fkeys->setFkey(8, ":/fkeys/resources/fkeys/f-key-0008.png", " ", false);
	ui.fkeys->setFkey(9, ":/fkeys/resources/fkeys/f-key-0009.png", " ", false);
	ui.fkeys->setFkey(10, ":/fkeys/resources/fkeys/f-key-0010.png", " ", false);
	ui.fkeys->setFkey(11, ":/fkeys/resources/fkeys/f-key-0011.png", " ", false);
	ui.fkeys->setFkey(12, ":/fkeys/resources/fkeys/f-key-0012.png", " ", false);

	QString t[20];
	t[0]=tr("close");
	t[1]=tr("new");
	t[2]=tr("back");
	t[3]=tr("edit device");
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

	switch (position) {
		case 1:		// Device Index
			ui.fkeys->setFkey(0, ":/fkeys/resources/fkeys/f-key-1000.png", t[0]);
			ui.fkeys->setFkey(2, ":/fkeys/resources/fkeys/f-key-1002.png", t[1]);
			ui.fkeys->setFkey(4, ":/fkeys/resources/fkeys/f-key-2004.png", tr("list devices"));
			break;
		case 2:		// Device Page
			break;
		case 3:		// Device Track
			if (wm->conf.DevicePath[DeviceType].notEmpty() == true) {
				ui.fkeys->setFkey(7, ":/fkeys/resources/fkeys/f-key-2007.png", tr("synchronize Tag"));
				ui.fkeys->setFkey(8, ":/fkeys/resources/fkeys/f-key-2008.png", tr("import cover"));
				ui.fkeys->setFkey(9, ":/fkeys/resources/fkeys/f-key-2009.png", tr("save all ID3"));
				ui.fkeys->setFkey(6, ":/fkeys/resources/fkeys/f-key-3006.png", tr("mass import"));
			}
			if (ppl7::AudioCD::isSupported() == true && ppl7::CDDB::isSupported() == true) {
				ui.fkeys->setFkey(5, ":/fkeys/resources/fkeys/f-key-4005.png", tr("cddb import"));
			}
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-2003.png", tr("renumber"));
			break;
		case 4:		// Interpret
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			ui.fkeys->setFkey(5, ":/fkeys/resources/fkeys/f-key-1005.png", t[11]);
			break;
		case 5:		// Titel
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
		case 6:		// VersionId
			break;
		case 7:		// Version
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
		case 9:		// Genre
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
		case 14:		// Album
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
		case 16:		// Label
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
		case 18:		// Aufnahmequelle
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
		case 20:		// Aufnahmegerät
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
		case 21:		// Bemerkung
			ui.fkeys->setFkey(3, ":/fkeys/resources/fkeys/f-key-1003.png", t[9]);
			break;
	}
	if (position > 1) {
		ui.fkeys->setFkey(0, ":/fkeys/resources/fkeys/f-key-2000.png", t[2]);
		ui.fkeys->setFkey(2, ":/fkeys/resources/fkeys/f-key-2002.png", t[3]);
	}
	if (position > 2) {
		ui.fkeys->setFkey(11, ":/fkeys/resources/fkeys/f-key-1011.png", t[5]);
		if (wm->conf.DevicePath[DeviceType].notEmpty() == true) {
			ui.fkeys->setFkey(10, ":/fkeys/resources/fkeys/f-key-1010.png", tr("playlist"));
		}
	}
	if (position > 3) {
		ui.fkeys->setFkey(7, ":/fkeys/resources/fkeys/f-key-1007.png", t[7]);
		ui.fkeys->setFkey(8, ":/fkeys/resources/fkeys/f-key-1008.png", t[8]);
		ui.fkeys->setFkey(12, ":/fkeys/resources/fkeys/f-key-1012.png", t[6]);
		if (wm->conf.DevicePath[DeviceType].notEmpty() == true) {
			ui.fkeys->setFkey(6, ":/fkeys/resources/fkeys/f-key-1006.png", t[10]);
		}
	}
	if (position > 7) {
		// DupeCheck
		ui.fkeys->setFkey(5, DupeCheckIcon, t[13]);
	}

	// Suchbutton
	if (position == 4 || position == 5 || position == 6 || position == 8
		|| position == 15 || position == 17 || position == 19) {
		ui.fkeys->setFkey(4, ":/fkeys/resources/fkeys/f-key-1004.png", t[4]);
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
		case 30: ui.energyLevel->setFocus(); break;
		case 31: ui.bitrate->setFocus(); break;
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

QWidget* Edit::GetWidgetFromPosition(int position)
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
		case 31: return ui.energyLevel;
	}
	return NULL;
}


// ********************************************************************************************************
// *** EVENTS                                                                                           ***
// ********************************************************************************************************



static bool isDropFromSearchlist(const QMimeData* mime)
{
	if (!mime->hasText()) return false;
	//printf ("we have text\n");
	QDomDocument doc("winmusikSearchlist");
	if (!doc.setContent(mime->text())) return false;
	//printf ("we have xml\n");
	QDomElement root=doc.documentElement();
	//QByteArray ba = root.tagName().toUtf8();
	//printf ("tagName=%s\n",(const char*)ba.data());
	//ba = root.attribute("version").toUtf8();
	//printf ("version=%s\n",(const char*)ba.data());
	if (root.tagName() != "winmusikSearchlist" || root.attribute("version") != "1") {
		return false;
	}
	return true;
}

static bool isDropWithAudioFile(const QMimeData* mime)
{
	if (!mime->hasUrls()) return false;
	QList<QUrl>	list=mime->urls();
	if (list.count() != 1) return false;
	QUrl url=list.first();
	/*
#if QT_VERSION >= 0x050000
	QString file=url.path(QUrl::FullyDecoded);
#else
	QString file=url.encodedPath();
#endif
	*/
	return true;
}
bool Edit::handleDragEnterEvent(QDragEnterEvent* event)
{
	const QMimeData* mime=event->mimeData();
	if (!mime) {
		//event->ignore();
		return false;
	}
	if (isDropFromSearchlist(mime)) {
		event->accept();
		return true;
	}
	if (position > 3 && isDropWithAudioFile(mime)) {
		event->accept();
		return true;
	}

	//event->ignore();
	return false;
}

void Edit::handleDropEvent(QDropEvent* event)
{
	event->accept();
	const QMimeData* mime=event->mimeData();
	if (!mime) return;
	if (handleDropFromSearchlist(event)) {
		event->accept();
		return;
	}
	if (position > 3) {
		handleFileDropEvent(event);
	}
	return;
	if (!mime->hasUrls()) return;
	QList<QUrl>	list=mime->urls();
	QUrl url=list.first();
#if QT_VERSION >= 0x050000
	QString file=url.path(QUrl::FullyDecoded);
#else
	QString file=url.encodedPath();
#endif
	ppl7::String f=file;
	ppl7::String path=wm->conf.DevicePath[DeviceType];
	int p=f.instr(path);
	if (p < 0) return;
	f=f.mid(p);
	f.replace(path, "");
	std::vector<ppl7::String> matches;
	if (ppl7::RegEx::capture("/\\/([0-9]+)\\/([0-9]{3})-.*$/", f, matches)) {
		int myDeviceId=matches[1].toInt();
		int myTrack=matches[2].toInt();
		OpenTrack(myDeviceId, 0, myTrack);
		QApplication::processEvents();
		ui.artist->setFocus();
		this->activateWindow();
		QApplication::processEvents();
		position=4;
		FixFocus();
		on_artist_FocusIn();
	}
	event->accept();

	//printf ("lala: %s, path: %s\n",(const char*)f, (const char*)path);

}

void Edit::handleFileDropEvent(QDropEvent* event)
{
	ppl7::String Tmp;
	event->accept();
	const QMimeData* mime=event->mimeData();
	if (!mime) return;
	if (!mime->hasUrls()) return;
	QList<QUrl>	list=mime->urls();
	QUrl url=list.first();

	QString file=url.toLocalFile();
	ppl7::String f=file;
	ppl7::DirEntry de;
	if (!ppl7::File::tryStatFile(f, de)) return;

	int tn=ui.track->text().toInt();
	if (tn < 1) return;
	ppl7::String ExistingFile=wm->GetAudioFilename(DeviceType, DeviceId, Page, tn);
	if (ExistingFile.notEmpty()) {
		if (QMessageBox::question(this, tr("WinMusik: overwrite existing file"),
			tr("Do you want to overwrite the exiting file?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
			== QMessageBox::No) return;
	}
	ppl7::String path=wm->GetAudioPath(DeviceType, DeviceId, Page);
	if (path.isEmpty()) {
		QMessageBox::information(this, tr("WinMusik: copy file"),
			tr("File copy is not supported on this device.\nYou have to configure a path for this device in the settings."));
		return;
	}
	if (!QDir(path).exists()) {
		QDir().mkpath(path);
	}
	path.appendf("/%03d-", ui.track->text().toInt());
	f=ppl7::File::getFilename(f);
	if (ppl7::RegEx::match("/^[0-9]{3}-/",f)) {
		f=f.mid(4);
	}
	path+=ppl7::File::getFilename(f);
	if (ExistingFile.notEmpty()) {
		QFile::remove(ExistingFile);
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (!QFile::copy(file, path)) {
		QApplication::restoreOverrideCursor();
		Tmp=tr("Could not copy file:");
		Tmp+="\n";
		Tmp+=file;
		Tmp+=" => ";
		Tmp+=path;
		QMessageBox::critical(this, tr("Error: could not copy file"), Tmp);
		return;
	}
	QApplication::restoreOverrideCursor();
	ui.filename->setText(path);
	ui.filename->setStyleSheet("");


	Tmp.setf("%0.1f", (double)de.Size / 1048576.0);
	ui.filesize->setText(Tmp);
	ppl7::ID3Tag Tag;
	if (Tag.loaded(path)) {
		// Cover?
		ppl7::ByteArray cover;
		if (Tag.getPicture(3, cover)) {
			Cover.loadFromData((const uchar*)cover.ptr(), cover.size());
			ui.coverwidget->setPixmap(Cover);
			wm->UpdateCoverViewer(Cover);
		}
	}
	QCoreApplication::processEvents();
	this->activateWindow();
	this->FixFocus();
}

bool Edit::handleDropFromSearchlist(QDropEvent* event)
{
	const QMimeData* mime=event->mimeData();
	if (!mime) return false;

	//printf ("Edit::handleDropFromSearchlist\n");
	if (!mime->hasText()) return false;
	//printf ("we have text\n");
	QDomDocument doc("winmusikSearchlist");
	if (!doc.setContent(mime->text())) return false;
	//printf ("we have xml\n");
	QDomElement root=doc.documentElement();
	//QByteArray ba = root.tagName().toUtf8();
	//printf ("tagName=%s\n",(const char*)ba.data());
	//ba = root.attribute("version").toUtf8();
	//printf ("version=%s\n",(const char*)ba.data());
	if (root.tagName() != "winmusikSearchlist" || root.attribute("version") != "1") {
		return false;
	}
	ppl7::Array rows;
	rows.explode(mime->text(), "<searchlistitem>");
	//printf ("we have winmusikSearchlist, %d rows\n",(int)rows.Size() );
	//ba = mime->text().toUtf8();
	//printf ("%s\n",ba.data());
	if (rows.size() < 2) return false;
	SearchlistItem item;
	item.importXML(rows.get(1));
	ui.artist->setText(item.Artist);
	ui.title->setText(item.Title);
	ui.versionId->setText("*");
	ui.version->setText(item.Version);
	ui.genreId->setText("*");
	ui.genre->setText(item.Genre);
	ui.labelId->setText("*");
	ui.labelName->setText(item.Comment);
	ui.releaseDate->setDate(QDate::currentDate());
	ui.recordDate->setDate(QDate::currentDate());

	if (item.Length > 0) ui.length->setText(ppl7::ToString("%0i:%02i", (int)(item.Length / 60), item.Length % 60));

	ppl7::DateTime date=ppl7::DateTime::currentTime();
	if (item.ReleaseDate.notEmpty()) {
		date=item.ReleaseDate;
	} else if (item.DateAdded.notEmpty()) {
		date=item.DateAdded;
	}
	QDate qdate(date.year(), date.month(), date.day());
	ui.releaseDate->setDate(qdate);
	ui.recordDate->setDate(qdate);
	ui.rating->setCurrentIndex(item.Rating);

	QApplication::processEvents();
	ui.artist->setFocus();
	this->activateWindow();
	QApplication::processEvents();
	position=4;
	FixFocus();
	on_artist_FocusIn();
	return true;
}


static void addFileIfSuitable(const ppl7::String& filename, std::list<ppl7::String>& fileList)
{
	ppl7::String lowerFile=filename;
	lowerFile.lowerCase();
	if (lowerFile.right(4) == ".mp3")
		fileList.push_back(filename);
	else if (lowerFile.right(4) == ".aif")
		fileList.push_back(filename);
	else if (lowerFile.right(5) == ".aiff")
		fileList.push_back(filename);
	else if (lowerFile.right(5) == ".wav")
		fileList.push_back(filename);

}

static void traverseDirectoryForFiles(const ppl7::String& path, std::list<ppl7::String>& fileList)
{
	ppl7::Dir dir;
	if (dir.tryOpen(path, ppl7::Dir::SORT_FILENAME)) {
		ppl7::DirEntry entry;
		ppl7::Dir::Iterator it;
		dir.reset(it);
		while ((dir.getNext(entry, it))) {
			//printf ("DIREntgry: >>%s<<\n",(const char*)entry->Filename);
			if (entry.Filename == "." || entry.Filename == "..") continue;
			if (entry.isDir()) {
				traverseDirectoryForFiles(entry.File, fileList);
			} else if (entry.isFile()) {
				addFileIfSuitable(entry.File, fileList);
			}
		}
	}
}

static int getHighestIdOfDirectory(const ppl7::String& path)
{
	int max=0;
	ppl7::Dir dir;
	if (dir.tryOpen(path, ppl7::Dir::SORT_FILENAME)) {
		std::vector<ppl7::String> matches;
		ppl7::Dir::Iterator it;
		dir.reset(it);
		ppl7::DirEntry entry;
		while ((dir.getNext(entry, it))) {
			if (ppl7::RegEx::capture("/^([0-9]{3}_.*$", entry.Filename, matches)) {
				int v=matches.at(1).toInt();
				if (v > max) max=v;
				//printf("match: %d\n", v);
			}
		}
	}
	return max;
}

void Edit::handleDropOnTracklist(const QList<QUrl>& urlList, int dropAction)
{
	if (position < 3) return;
	std::list<ppl7::String> fileList, filteredFileList;
	foreach(QUrl url, urlList) {
		ppl7::String filename=url.toLocalFile();
		ppl7::DirEntry file;
		if (ppl7::File::tryStatFile(filename, file)) {
			if (file.isDir()) {
				traverseDirectoryForFiles(filename, fileList);
			} else if (file.isFile()) {
				addFileIfSuitable(filename, fileList);
			}
		}
	}

	std::list<ppl7::String>::const_iterator it;
	ppl7::String TargetPath=wm->conf.DevicePath[DeviceType];
	if (TargetPath.isEmpty()) return;
	TargetPath.trimRight("/");
	TargetPath.trimRight("\\");
	TargetPath.appendf("/%02u/%03u/", (uint32_t)(DeviceId / 100), DeviceId);

	for (it=fileList.begin();it != fileList.end();++it) {
		if ((*it).left(TargetPath.size()) != TargetPath) filteredFileList.push_back((*it));
	}
	if (filteredFileList.size() == 0) return;

	int highestId=getHighestIdOfDirectory(TargetPath);
	//printf ("Target: %s\n",(const char*)TargetPath);

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QProgressDialog progress(tr("Copy Files into WinMusik directory..."), tr("Abort"), 0, fileList.size(), this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setWindowTitle(QString(WM_APPNAME) + QString(": ") + tr("Copy Files into WinMusik directory..."));
	progress.setMinimumWidth(500);
	progress.setMaximumWidth(500);
	progress.show();

	QCoreApplication::processEvents();
	int i=0;

	ppl7::String NewFile;
	for (it=filteredFileList.begin();it != filteredFileList.end();++it) {
		progress.setValue(i);
		QCoreApplication::processEvents();
		if (progress.wasCanceled())	break;
		i++;
		progress.setLabelText(ppl7::File::getFilename((*it)));
		highestId++;
		NewFile=TargetPath + ppl7::ToString("%03d_", highestId) + ppl7::File::getFilename((*it));
		//printf (" ==> %s\n",(const char*)NewFile);
		try {
			if (dropAction == Qt::MoveAction)
				ppl7::File::move((*it), NewFile);
			else
				ppl7::File::copy((*it), NewFile);
		} catch (...) {}
	}
	QApplication::restoreOverrideCursor();
	progress.close();

	if (filteredFileList.size() > 1) {
		on_f6_MassImport();
		return;
	}
	ppl7::String Tmp, FinalFile;

	TrackNum=TrackList.GetMax() + 1;
	FinalFile.setf("%s/%03u-%s", (const char*)TargetPath, TrackNum, (const char*)ppl7::File::getFilename(NewFile));
	try {
		ppl7::File::rename(NewFile, FinalFile);
	} catch (...) {}
	Tmp.setf("%u", TrackNum);
	ui.track->setFocus();
	ui.track->setText(Tmp);
	EditTrack();
	//position=4;
	ui.artist->setFocus();

	TrackInfo tinfo;
	//printf ("DEBUG: %s\n",(const char*)FinalFile);
	if (getTrackInfoFromFile(tinfo, FinalFile)) {
		//printf ("CopyFromTrackInfo\n");
		CopyFromTrackInfo(tinfo);

	}
}


// Globale Events, Fkeys
bool Edit::on_KeyPress(QObject* target, int key, int modifier)
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
	if (key == Qt::Key_Escape) {
		on_esc_clicked();
		return true;
		// *************************************************************************** Return/Enter
	} else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
		MoveToNextWidget();
		return true;
		// *************************************************************************** Page down
	} else if (position > 2 && key == Qt::Key_PageDown) {
		if (modifier == Qt::NoModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
		else if (modifier == Qt::ShiftModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
		return true;
		// *************************************************************************** Page up
	} else if (position > 2 && key == Qt::Key_PageUp) {
		if (modifier == Qt::NoModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
		else if (modifier == Qt::ShiftModifier) trackList->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
		return true;
		// *************************************************************************** F2
	} else if (key == Qt::Key_F2 && modifier == Qt::NoModifier && position > 1) {
		uint32_t ret=EditDeviceDialog(DeviceId);
		((QWidget*)target)->setFocus();
		if (ret == DeviceId) UpdateDevice();
		return true;
		// *************************************************************************** F3
	} else if (key == Qt::Key_F3 && modifier == Qt::NoModifier && position == 3) {
		renumber();
		return true;
	} else if (key == Qt::Key_F3 && modifier == Qt::NoModifier && position > 3 && ui.fkeys->isEnabled(3) == true) {
		QLineEdit* LineEdit=(QLineEdit*)target;
		ppl7::WideString Tmp=LineEdit->text().toLower();
		Tmp.upperCaseWords();
		LineEdit->setText(Tmp);
		return true;
		// *************************************************************************** F4 im Index
	} else if (key == Qt::Key_F4 && position == 1) {
		wm->OpenDeviceList(DeviceType);
		return true;
		// *************************************************************************** F4
	} else if (key == Qt::Key_F4 && position >= 4 && position <= 5) {
		return on_f4_Pressed(position);
		// *************************************************************************** F5
	} else if (key == Qt::Key_F5 && position == 4) {
		return on_f5_ShortCut(modifier);
	} else if (key == Qt::Key_F5 && position > 7) {
		return on_f5_CheckDupes(target);
	} else if (key == Qt::Key_F5 && position == 3) {
		importFromCddb();
		return true;
		// *************************************************************************** F6
	} else if (key == Qt::Key_F6 && modifier == Qt::ControlModifier && ui.fkeys->isEnabled(6) == true && position > 3 && wm->conf.DevicePath[DeviceType].notEmpty() == true) {
		wm->TrashAudioFile(DeviceType, DeviceId, Page, Track.Track);
		return on_f6_Pressed(target, Qt::NoModifier);

	} else if (key == Qt::Key_F6 && ui.fkeys->isEnabled(6) == true && position > 3 && wm->conf.DevicePath[DeviceType].notEmpty() == true) {
		return on_f6_Pressed(target, modifier);

	} else if (key == Qt::Key_F6 && ui.fkeys->isEnabled(6) == true && position == 3 && wm->conf.DevicePath[DeviceType].notEmpty() == true) {
		return on_f6_MassImport();
		// *************************************************************************** F7
	} else if (key == Qt::Key_F7 && position > 3 && modifier == Qt::NoModifier) {
		return on_f7_DeleteTrack();
	} else if (key == Qt::Key_F7 && position == 3 && modifier == Qt::NoModifier) {
		on_contextSynchronizeKeys_triggered();
		return true;
		// *************************************************************************** F8
	} else if (key == Qt::Key_F8 && position > 3 && modifier == Qt::NoModifier) {
		return on_f8_InsertTrack();
	} else if (key == Qt::Key_F8 && position == 3 && modifier == Qt::NoModifier) {
		on_contextLoadCoverAllTracks_triggered();
		return true;
		// *************************************************************************** F9
	} else if (key == Qt::Key_F9 && modifier == Qt::NoModifier && position == 3 && wm->conf.DevicePath[DeviceType].notEmpty() == true) {
		return on_f9_UpdateAllID3Tags();
		// *************************************************************************** F10
	} else if (key == Qt::Key_F10 && position > 2 && modifier == Qt::NoModifier && wm->conf.DevicePath[DeviceType].notEmpty() == true) {
		return on_f10_WritePlaylist();
		// *************************************************************************** F11 => Printing
	} else if (key == Qt::Key_F11 && position > 2 && modifier == Qt::NoModifier) {
		wm->PrintCoverDialog(this, DeviceType, DeviceId);
		return true;
		// *************************************************************************** F12
	} else if (key == Qt::Key_F12 && modifier == Qt::NoModifier && position > 3) {
		SaveEditorTrack();
		return true;
		// *************************************************************************** Alt & b
	} else if (key == Qt::Key_B && modifier == Qt::AltModifier && position > 3) {
		ui.remarks->setFocus();
		return true;
		// *************************************************************************** Alt & a
	} else if (key == Qt::Key_A && modifier == Qt::AltModifier && position > 3) {
		ui.album->setFocus();
		return true;
		// *************************************************************************** Alt & q
	} else if (key == Qt::Key_Q && modifier == Qt::AltModifier && position > 3) {
		ui.recordSourceId->setFocus();
		return true;
		// *************************************************************************** Alt & d
	} else if (key == Qt::Key_D && modifier == Qt::AltModifier && position > 3) {
		ui.recordDate->setFocus();
		return true;
		// *************************************************************************** Alt & l
	} else if (key == Qt::Key_L && modifier == Qt::AltModifier && position > 3) {
		ui.labelId->setFocus();
		return true;
		// *************************************************************************** Alt & t
	} else if (key == Qt::Key_T && modifier == Qt::AltModifier && position > 3) {
		ui.tags->setFocus();
		return true;
		// *************************************************************************** Alt & y
	} else if (key == Qt::Key_Y && modifier == Qt::AltModifier && position > 3) {
		ui.releaseDate->setFocus();
		return true;
		// *************************************************************************** Alt & j
	} else if (key == Qt::Key_J && modifier == Qt::AltModifier && position > 3) {
		ui.releaseDate->setFocus();
		return true;
		// *************************************************************************** Ctrl & Shift & v im Feld Artist
	} else if (key == Qt::Key_V && modifier == (int)(Qt::ShiftModifier | Qt::ControlModifier) && position == 4) {
		RegExpMatch match;
		RegExpClipboard clip;
		copyFromClipboard(clip);
		if (wm_main->RegExpCapture.match(clip, match)) {
			ui.artist->setText(match.Artist);
			ui.title->setText(match.Title);
			if (match.Version.notEmpty()) {
				ui.versionId->setText("*");
				ui.version->setText(match.Version);
			}
			if (match.Genre.notEmpty()) {
				ui.genreId->setText("*");
				ui.genre->setText(match.Genre);
			}
		}
		return true;
	}
	return false;
}

// EVENT: index
bool Edit::on_index_FocusIn()
{
	ui.coverwidget->setEnabled(false);
	TrackList.Clear();
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

bool Edit::on_index_KeyPress(__attribute__((unused)) QKeyEvent* event, int key, int modifier)
{
	if (key == Qt::Key_F2 && modifier == Qt::NoModifier) {
		uint32_t ret=EditDeviceDialog(0);
		if (ret) {
			ppl7::String Tmp;
			Tmp.setf("%u", ret);
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
	ui.coverwidget->setEnabled(false);
	Page=0;
	TrackNum=0;
	ClearEditFields();
	TrackList.Clear();
	ppl7::String Tmp;
	if (oldposition < 2) {
		DeviceId=ui.index->text().toInt();
		if (ui.index->text() == "?") {
			DeviceId=wm->DeviceStore.GetHighestDevice(DeviceType);
			Tmp.setf("%u", DeviceId);
			ui.index->setText(Tmp);
		}
		if (ui.index->text() == "*") {
			DeviceId=EditDeviceDialog(0);
			if (!DeviceId) {
				ui.index->setFocus();
				return true;
			}
			Tmp.setf("%u", DeviceId);
			ui.index->setText(Tmp);
		}
		if (DeviceId == 0) {
			ui.index->setFocus();
			return true;
		}
		wm->UpdateDevice(DeviceType, DeviceId);
		if (!wm->LoadDevice(DeviceType, DeviceId, &datadevice)) {
			DeviceId=EditDeviceDialog(DeviceId);
			if (!DeviceId) {
				ui.index->setFocus();
				return true;
			}
			wm->UpdateDevice(DeviceType, DeviceId);
			if (!wm->LoadDevice(DeviceType, DeviceId, &datadevice)) {
				ui.index->setFocus();
				return true;
			}
			showEditor();
		}
		UpdateDevice();
		Tmp="1";
		ui.track->setText(Tmp);
		if (datadevice.Pages == 1) {
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
	ui.coverwidget->setEnabled(false);

	TrackNum=0;
	if (!Page) {
		Page=(uint8_t)ui.page->text().toInt();
		if (Page<1 || Page>datadevice.Pages) {
			ui.page->setFocus();
			return true;
		}
		// Trackliste laden
		TrackList=wm->GetTracklist(DeviceType, DeviceId, Page);
		UpdateTrackListing();
		//asyncTrackUpdate.ThreadStop();
		//asyncTrackUpdate.ThreadStart();

		ppl7::String a;
		a.setf("%u", TrackList.GetMax() + 1);
		ui.track->setText(a);
	}
	UpdateFkeys();
	UpdateCompleters();
	if (TrackList.Num() == 0) showEditor();
	ui.track->deselect();
	ui.track->selectAll();
	return false;

}

void Edit::ReloadTracks()
{
	TrackList=wm->GetTracklist(DeviceType, DeviceId, Page);
	UpdateTrackListing();
	ppl7::String a;
	a.setf("%u", TrackList.GetMax() + 1);
	ui.track->setText(a);
	UpdateFkeys();
	UpdateCompleters();
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
	ui.coverwidget->setEnabled(true);
	ui.artist->setFocus();
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
	QTreeWidgetItem* w;
	int i=TrackNum;
	while (NULL == (w=trackList->topLevelItem(i - 1)) && i > 0) i--;
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
	ppl7::String Tmp;
	uint32_t l=Time2Int(Tmp=ui.length->text());
	if (l > 0) Tmp.setf("%0i:%02i", (int)(l / 60), l % 60); else Tmp.clear();
	ui.length->setText(Tmp);
	return false;
}


// *****************************************************************************************************
// EVENT: LineEdit FocusIn
bool Edit::on_FocusIn(QLineEdit* widget)
{
	widget->deselect();
	widget->selectAll();
	return false;
}


bool Edit::on_f4_Pressed(int position)
{
	ppl7::String Artist, Title;

	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.trim();
	Title=ui.title->text();
	Title.trim();
	if (position == 4) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow, Artist);
		this->setFocus();
		ui.artist->setFocus();
	} else if (position == 5) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow, Artist, Title);
		this->setFocus();
		ui.title->setFocus();
	}


	return true;
}

bool Edit::on_f5_ShortCut(int modifier)
{
	ppl7::String Artist;
	Artist=ui.artist->text();
	Artist.trim();

	if (modifier == Qt::NoModifier) {
		const DataShortcut* sc=wm->ShortcutStore.GetPtr(Artist);
		if (sc) {
			ui.artist->setText(sc->artist);
			return true;
		}
	}
	ShortcutDialog Dialog(this, wm);
	Dialog.setModal(true);
	Dialog.SetShortcut(Artist);
	if (Dialog.exec()) {
		ui.artist->setText(Dialog.GetArtist());
	}
	return true;
}

bool Edit::on_f5_CheckDupes(QObject* target)
{
	ppl7::String Artist, Title;

	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.trim();
	Title=ui.title->text();
	Title.trim();
	searchWindow=wm->OpenOrReuseSearch(searchWindow, Artist, Title);
	qApp->processEvents();
	qApp->processEvents();
	this->setFocus();
	if (target) ((QWidget*)target)->setFocus();
	return true;
}

// *****************************************************************************************************
// EVENT: F6 - MP3 ID3-Tag einlesen
bool Edit::on_f6_Pressed(QObject*, int modifier)
{
	ppl7::String Tmp;
	ppl7::String Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, TrackNum);
	if (Path.isEmpty()) {
		// Vielleicht gibt es noch einen Titel ohne Index
		Path=wm->NextAudioFile(DeviceType, DeviceId, Page, TrackNum);
		if (Path.isEmpty()) {
			QMessageBox::information(this, tr("WinMusik: Notice"),
				tr("There are no further titles without an index in the directory of this device"));
			return true;
		}
	}

	// Den Dateinamen nehmen wir in die Zwischenablage
	ppl7::String Songname=ppl7::File::getFilename(Path);
	ui.filename->setText(Songname);
	ppl7::DirEntry de;
	if (ppl7::File::tryStatFile(Path, de)) {
		Tmp.setf("%0.1f", (double)de.Size / 1048576.0);
		ui.filesize->setText(Tmp);
	} else {
		ui.filesize->setText("");
	}
	Songname=ppl7::RegEx::replace("/\\.mp3$/i", Songname, "");
	Songname=ppl7::RegEx::replace("/\\.aiff$/i", Songname, "");
	Songname=ppl7::RegEx::replace("/\\.aif$/i", Songname, "");
	Songname=ppl7::RegEx::replace("/\\.wav$/i", Songname, "");
	Songname=ppl7::RegEx::replace("/^[0-9]+-/", Songname, "");
	Songname.replace("_", " ");
	Songname.replace("\t", " ");
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(Songname);
	if (modifier == Qt::NoModifier) {
		TrackInfo tinfo;
		bool ret=getTrackInfoFromFile(tinfo, Path);
		if (ret) {
			CopyFromTrackInfo(tinfo);
			searchWindow=wm->OpenOrReuseSearch(searchWindow, tinfo.Ti.Artist, tinfo.Ti.Title);
			qApp->processEvents();
			this->activateWindow();
			this->setFocus();
			FixFocus();

		}
		return ret;
	}
	if (modifier == Qt::ShiftModifier) {
		wm->PlayFile(Path);
	}

	return true;

}

bool Edit::on_f7_DeleteTrack()
{
	if (Track.Track > 0) {
		if (Track.TitleId) {
			const DataTitle* Ti=NULL;
			Ti=wm->GetTitle(Track.TitleId);
			if (Ti != NULL && Ti->DeviceType == DeviceType && Ti->DeviceId == DeviceId
				&& Ti->Page == Page) {
				wm->Hashes.RemoveTitle(Ti->TitleId);
				wm->TitleStore.Delete(Ti->TitleId);
			}
			TrackList.Delete(Track.Track);
			UpdateTrackListing();
			EditTrack();
			ui.artist->setFocus();
		} else {
			// Track löschen
			wm->TrashAudioFile(DeviceType, DeviceId, Page, Track.Track);
			// Nachfolgende Tracks nach oben rücken
			// TODO
			DeleteShift(Track.Track); //, &wm->TitleStore
			UpdateTrackListing();
			QTreeWidgetItem* w=trackList->topLevelItem(Track.Track);
			if (w) {
				trackList->scrollToItem(w);
			}
			ClearEditFields();
			ui.titleEdit->setEnabled(true);
			ui.track->setFocus();
			ui.artist->setFocus();
		}
	}
	return true;
}

bool Edit::on_f8_InsertTrack()
{
	if (Track.Track > 0) {
		// TODO
		InsertShift(Track.Track);
		UpdateTrackListing();
		QTreeWidgetItem* w=trackList->topLevelItem(Track.Track);
		if (w) {
			trackList->scrollToItem(w);
		}
		ClearEditFields();
		EditTrack();
		ui.artist->setFocus();
		return true;
	}
	return false;
}

bool Edit::on_f10_WritePlaylist()
{
	if (wm->WritePlaylist(DeviceType, DeviceId, Page, &TrackList, &datadevice)) {
		QMessageBox::information(this, tr("WinMusik: Notice"),
			tr("Playlists wurden erfolgreich erstellt"));
		return true;
	}
	wm->RaiseError(this, tr("Could not create playlists"));
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
	if (position < 2) {
		position=0;
		close();
		return;
	} else if (position == 2) ui.index->setFocus();
	else if (position == 3 && datadevice.Pages == 1) ui.index->setFocus();
	else if (position == 3 && datadevice.Pages > 1) ui.page->setFocus();
	else if (position == 4) ui.track->setFocus();
	else if (position > 4) ui.artist->setFocus();
}

void Edit::on_f1_clicked()
{

}

void Edit::on_f2_clicked()
{
	QWidget* target=GetWidgetFromPosition(position);
	if (position > 1 && target != NULL) {
		uint32_t ret=EditDeviceDialog(DeviceId);
		((QWidget*)target)->setFocus();
		if (ret == DeviceId) UpdateDevice();
	} else if (position == 1) {
		uint32_t ret=EditDeviceDialog(0);
		if (ret) {
			ppl7::String Tmp;
			Tmp.setf("%u", ret);
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
	if (position == 3) {
		renumber();
	} else if (position == 4 || position == 5 || position == 7 || position == 9 || position == 14
		|| position == 16 || position == 18 || position == 20 || position == 21) {
		QWidget* target=GetWidgetFromPosition(position);
		if (!target) return;
		QLineEdit* LineEdit=(QLineEdit*)target;
		ppl7::WideString Tmp=LineEdit->text().toLower();
		Tmp.upperCaseWords();
		LineEdit->setText(Tmp);
	}
}
void Edit::on_f4_clicked()
{
	if (position >= 4 && position <= 5) on_f4_Pressed(position);
}
void Edit::on_f5_clicked()
{
	if (position == 3) importFromCddb();
	if (position == 4) on_f5_ShortCut(Qt::NoModifier);
	else if (position > 7) on_f5_CheckDupes(NULL);
}
void Edit::on_f6_clicked()
{
	if (position == 3) on_f6_MassImport();
	else if (position > 3) on_f6_Pressed(GetWidgetFromPosition(position), Qt::NoModifier);
}

void Edit::on_f7_clicked()
{
	if (position == 3) on_contextSynchronizeKeys_triggered();
	if (position > 3) on_f7_DeleteTrack();
}

void Edit::on_f8_clicked()
{
	if (position == 3) on_contextLoadCoverAllTracks_triggered();
	else if (position > 3) on_f8_InsertTrack();
}

void Edit::on_f9_clicked()
{
	if (position == 3) on_f9_UpdateAllID3Tags();
}

void Edit::on_f10_clicked()
{
	if (position > 2) on_f10_WritePlaylist();
}

void Edit::on_f11_clicked()
{
	wm->PrintCoverDialog(this, DeviceType, DeviceId);
}

void Edit::on_f12_clicked()
{
	SaveEditorTrack();
}

bool Edit::on_trackList_MousePress(QMouseEvent* event)
{
	//printf ("MousePress\n");
	if (event->buttons() == Qt::LeftButton) startPos=event->pos();
	ratePos=event->pos();
	return false;
}

bool Edit::on_trackList_MouseRelease(QMouseEvent*)
{
	//printf ("MouseRelease\n");
	startPos.setX(0);
	startPos.setY(0);
	return false;
}

bool Edit::on_trackList_MouseMove(QMouseEvent* event)
{
	//printf ("MouseMove\n");
	if (!(event->buttons() == Qt::LeftButton)) {
		//QTreeWidget::mouseMoveEvent(event);
		return false;
	}
	if (event->pos().x() > trackList->columnViewportPosition(TRACKLIST_RATING_ROW)) return false;
	//QTreeWidgetItem *	itemAt ( const QPoint & p ) const

	int distance=(event->pos() - startPos).manhattanLength();
	//printf ("distance=%i\n", distance);
	if (distance < QApplication::startDragDistance()) {
		//QTreeWidget::mouseMoveEvent(event);
		return false;
	}

	WMTreeItem* item=(WMTreeItem*)trackList->currentItem();
	if (!item) return false;
	QList<QTreeWidgetItem*> Items=trackList->selectedItems();
	QList<QUrl> list;
	ppl7::String File;
	QString qFile;
	QPixmap Icon;

	ppl7::String xml;
	xml="<winmusikTracklist>\n";
	xml+="<tracks>\n";

	for (int i=0;i < Items.size();i++) {
		item=(WMTreeItem*)Items[i];
		if (Icon.isNull()) {
			const DataTitle* ti=wm->GetTitle(item->Id);
			if (ti != NULL && ti->CoverPreview.size() > 0) {
				Icon.loadFromData((const uchar*)ti->CoverPreview.ptr(), ti->CoverPreview.size());
			}
		}
		xml+="<item>\n";
		xml+=wm->getXmlTitle(item->Id);
		File=wm->GetAudioFilename(DeviceType, DeviceId, Page, item->Track);
		if (File.notEmpty()) {
			xml+="<File>" + ppl7::EscapeHTMLTags(File) + "</File>\n";

#ifdef _WIN32
			list.append(QUrl::fromLocalFile(File));
#else
			list.append(QUrl::fromLocalFile(File));
#endif
		}
		xml+="</item>";
	}
	xml+="</tracks>\n";
	xml+="</winmusikTracklist>\n";

	QDrag* drag = new QDrag(this);
	QMimeData* mimeData = new QMimeData;
	if (Icon.isNull()) Icon.load(":/devices48/resources/tr48x48-0007.png");
	drag->setPixmap(Icon);
	QByteArray ba((const char*)xml, xml.size());
	mimeData->setData("application/winmusik+xml", ba);
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
		tr("Update ID3-Tags of all tracks?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
		== QMessageBox::No) return true;

	if (wm->UpdateID3Tags(DeviceType, DeviceId, Page, &TrackList)) {
		QMessageBox::information(this, tr("WinMusik: Notice"),
			tr("Update of ID3-Tags on all tracks has been started"));
		return true;
	}
	wm->RaiseError(this, tr("Update of ID3-Tags failed"));
	return true;
}



void Edit::FixFocus()
{
	QWidget* widget=this->GetWidgetFromPosition(position);
	if (widget) widget->setFocus();
}

static void setItemBackground(WMTreeItem* item, const QBrush& c)
{
	item->setBackground(TRACKLIST_NAME_ROW, c);
	item->setBackground(TRACKLIST_VERSION_ROW, c);
	item->setBackground(TRACKLIST_GENRE_ROW, c);
	item->setBackground(TRACKLIST_LENGTH_ROW, c);
	item->setBackground(TRACKLIST_BPM_ROW, c);
	item->setBackground(TRACKLIST_KEY_ROW, c);
}

void Edit::on_trackList_itemClicked(QTreeWidgetItem* item, int column)
{
	Qt::KeyboardModifiers key=QApplication::keyboardModifiers();
	const DataTitle* t=wm->GetTitle(((WMTreeItem*)item)->Id);
	if (t) {
		QClipboard* clipboard = QApplication::clipboard();
		ppl7::String Text;
		if (key & (Qt::AltModifier | Qt::MetaModifier)) {
			Text.setf("%s %s", (const char*)t->Artist, (const char*)t->Title);
		} else {
			Text.setf("%s - %s (%s, %0i:%02i min, %s)", (const char*)t->Artist, (const char*)t->Title,
				(const char*)wm->GetVersionText(t->VersionId), t->Length / 60, t->Length % 60, (const char*)wm->GetGenreText(t->GenreId));
			Text.appendf(" [%s %u %c-%i]", (const char*)wm->GetDeviceNameShort(t->DeviceType),
				t->DeviceId, (t->Page + 'A' - 1), t->Track);
		}
		clipboard->setText(Text, QClipboard::Clipboard);
		clipboard->setText(Text, QClipboard::Selection);

		if (column == TRACKLIST_RATING_ROW) {
			int x=ratePos.x() - trackList->columnViewportPosition(TRACKLIST_RATING_ROW);
			if (x < 0) x=0;
			int r=x / 10.666666;
			if (r != t->Rating) {
				DataTitle tUpdate=*t;
				tUpdate.Rating=r;
				try {
					wm->TitleStore.Put(tUpdate);
				} catch (const ppl7::Exception& exp) {
					ShowException(exp, tr("Could not save Title in TitleStore"));
					return;
				}
				if (wm_main->conf.bWriteID3Tags == true) {
					ppl7::String Path=wm->GetAudioFilename(tUpdate.DeviceType,
						tUpdate.DeviceId,
						tUpdate.Page,
						tUpdate.Track);
					if (Path.notEmpty()) {
						if (!wm->SaveID3Tags(tUpdate.DeviceType,
							tUpdate.DeviceId,
							tUpdate.Page,
							tUpdate.Track,
							tUpdate)) {
							wm->RaiseError(this, tr("Could not save ID3 Tags"));
						}
					}
				}
				switch (r) {
					case 0: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-0.png"));
						item->setText(TRACKLIST_RATING_ROW, "0");
						break;
					case 1: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-1.png"));
						item->setText(TRACKLIST_RATING_ROW, "1");
						break;
					case 2: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-2.png"));
						item->setText(TRACKLIST_RATING_ROW, "2");
						break;
					case 3: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-3.png"));
						item->setText(TRACKLIST_RATING_ROW, "3");
						break;
					case 4: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-4.png"));
						item->setText(TRACKLIST_RATING_ROW, "4");
						break;
					case 5: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-5.png"));
						item->setText(TRACKLIST_RATING_ROW, "5");
						break;
					case 6: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-6.png"));
						item->setText(TRACKLIST_RATING_ROW, "6");
						break;
				}
			}
			//printf ("Rating: %i, %i\n",ratePos.x(),x);
		} else if (column == TRACKLIST_COVER_ROW && wm->IsCoverViewerVisible() == true) {
			ppl7::ID3Tag Tag;
			ppl7::String File=wm->GetAudioFilename(DeviceType, DeviceId, Page, ((WMTreeItem*)item)->Track);
			if (Tag.loaded(File)) {
				ppl7::ByteArray cover;
				if (Tag.getPicture(3, cover)) {
					QPixmap trackCover;
					trackCover.loadFromData((const uchar*)cover.ptr(), cover.size());
					wm->UpdateCoverViewer(trackCover);
				}
			}
		} else if (column == TRACKLIST_KEY_ROW && t->Key > 0) {
			std::map<int, HarmonicType> harmonics;
			std::map<int, HarmonicType>::const_iterator it;
			getHarmonicKeys(harmonics, t->Key);

			int count=trackList->topLevelItemCount();


			for (int i=0;i < count;i++) {
				WMTreeItem* item=(WMTreeItem*)trackList->topLevelItem(i);
				if (item) {
					const DataTitle* title=wm->GetTitle(item->Id);
					QBrush q=item->background(TRACKLIST_TRACK_ROW);
					setItemBackground(item, q);

					if (title != NULL && title->Key > 0) {
						if (title->Key == t->Key) item->setBackground(TRACKLIST_KEY_ROW, colorscheme.sameKey);
						else {
							it=harmonics.find(title->Key);
							if (it != harmonics.end()) {
								switch (it->second) {
									case harmonicSemitoneUp:
									case harmonicTwoSemitoneUp:
										item->setBackground(TRACKLIST_KEY_ROW, colorscheme.boostKey);
										break;
									case harmonicAvbBoost:
										item->setBackground(TRACKLIST_KEY_ROW, colorscheme.boostKey2);
										break;
									default:
										item->setBackground(TRACKLIST_KEY_ROW, colorscheme.relatedKey);
										break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void Edit::on_trackList_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
	if (column == TRACKLIST_RATING_ROW) return;
	else if (column == TRACKLIST_COVER_ROW) {
		ppl7::ID3Tag Tag;
		ppl7::String File=wm->GetAudioFilename(DeviceType, DeviceId, Page, ((WMTreeItem*)item)->Track);
		if (Tag.loaded(File)) {
			ppl7::ByteArray cover;
			if (Tag.getPicture(3, cover)) {
				QPixmap trackCover;
				trackCover.loadFromData((const uchar*)cover.ptr(), cover.size());
				wm->OpenCoverViewer(trackCover);
			}
		}
	} else {
		ppl7::String Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, ((WMTreeItem*)item)->Track);
		if (Path.isEmpty()) return;
		//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (const char*)Path);
		wm->PlayFile(Path);
	}
	//FixFocus();
}


void Edit::on_trackList_customContextMenuRequested(const QPoint& pos)
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
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);

	QMenu* m=new QMenu(this);
	QAction* a=NULL;
	//m->setTitle("Ein Titel");
	if (TrackList.Num() > 0 &&
		(trackList->currentColumn() == TRACKLIST_KEY_ROW
			|| trackList->currentColumn() == TRACKLIST_BPM_ROW
			|| trackList->currentColumn() == TRACKLIST_ENERGYLEVEL_ROW
			)) {
		a=m->addAction(QIcon(":/icons/resources/sync-keys.png"), tr("Synchronize Keys, BPM and Energy with ID3-Tag", "trackList Context Menue"), this, SLOT(on_contextSynchronizeKeys_triggered()));
		QMenu* mk=m->addMenu(QIcon(":/icons/resources/musicKey.png"), tr("Set Music-Key", "trackList Context Menue"));
		createSetMusicKeyContextMenu(mk);
		if (t != NULL && (t->Flags & 16) == 0) m->addAction(QIcon(":/icons/resources/musicKeyOk.png"), tr("Music Key is verified", "trackList Context Menue"), this, SLOT(on_contextMusicKeyVerified_triggered()));
		else if (t != NULL && (t->Flags & 16) == 16) m->addAction(QIcon(":/icons/resources/musicKeyNotOk.png"), tr("Music Key is not verified", "trackList Context Menue"), this, SLOT(on_contextMusicKeyVerified_triggered()));
	} else {
		if (trackList->currentColumn() == TRACKLIST_COVER_ROW) {
			a=m->addAction(QIcon(":/icons/resources/view_cover.png"), tr("Show cover", "trackList Context Menue"), this, SLOT(on_contextShowCover_triggered()));
			m->addAction(QIcon(":/icons/resources/load_cover.png"), tr("Load Cover for all Tracks", "trackList Context Menue"), this, SLOT(on_contextLoadCoverAllTracks_triggered()));

		} else {
			a=m->addAction(QIcon(":/icons/resources/findmore.png"), tr("Find other versions", "trackList Context Menue"), this, SLOT(on_contextFindMoreVersions_triggered()));
			m->addAction(QIcon(":/icons/resources/findmore-artist.png"), tr("Find more of artist", "trackList Context Menue"), this, SLOT(on_contextFindMoreArtist_triggered()));
			m->addAction(QIcon(":/icons/resources/findmore-title.png"), tr("Find other artists of this title", "trackList Context Menue"), this, SLOT(on_contextFindMoreTitle_triggered()));
		}
		m->addAction(QIcon(":/icons/resources/play.png"), tr("Play Track", "trackList Context Menue"), this, SLOT(on_contextPlayTrack_triggered()));
		m->addAction(QIcon(":/icons/resources/edit.png"), tr("Edit Track", "trackList Context Menue"), this, SLOT(on_contextEditTrack_triggered()));
		m->addAction(QIcon(":/icons/resources/copytrack.png"), tr("Copy Artist and Title", "trackList Context Menue"), this, SLOT(on_contextCopyTrack_triggered()));
		m->addAction(QIcon(":/icons/resources/copyfile.png"), tr("Copy MP3-File", "trackList Context Menue"), this, SLOT(on_contextCopyFile_triggered()));
		m->addAction(QIcon(":/icons/resources/copycover.png"), tr("Copy Cover", "trackList Context Menue"), this, SLOT(on_contextCopyCover_triggered()));
		m->addSeparator();
		m->addAction(QIcon(":/icons/resources/delete-track.png"), tr("Delete Track", "trackList Context Menue"), this, SLOT(on_contextDeleteTrack_triggered()));
		m->addAction(QIcon(":/icons/resources/insert-track.png"), tr("Insert Track", "trackList Context Menue"), this, SLOT(on_contextInsertTrack_triggered()));
		if (TrackList.Num() > 0 && trackList->currentColumn() == TRACKLIST_BPM_ROW) {
			m->addSeparator();
			m->addAction(QIcon(":/icons/resources/edit.png"), tr("Read BPM and Key from ID3-Tag", "trackList Context Menue"), this, SLOT(on_contextReadBpmAndKey_triggered()));
		}
		m->addAction(QIcon(":/icons/resources/eraser.png"), tr("Delete ID3-Tags from file", "trackList Context Menue"), this, SLOT(on_contextDeleteID3Tags_triggered()));
	}
	m->popup(p, a);
	//FixFocus();
}

void Edit::createSetMusicKeyContextMenu(QMenu* m)
{
	de::pfp::winmusik::MusicalKeys& keys=wm_main->MusicKeys;

	m->addAction(tr("unknown", "trackList Context Menue"), this, SLOT(on_contextMusicKey0_triggered()));
	std::map<QString, const char*> sortedkeys;
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(1, musicKeyDisplay), SLOT(on_contextMusicKey1_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(2, musicKeyDisplay), SLOT(on_contextMusicKey2_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(3, musicKeyDisplay), SLOT(on_contextMusicKey3_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(4, musicKeyDisplay), SLOT(on_contextMusicKey4_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(5, musicKeyDisplay), SLOT(on_contextMusicKey5_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(6, musicKeyDisplay), SLOT(on_contextMusicKey6_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(7, musicKeyDisplay), SLOT(on_contextMusicKey7_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(8, musicKeyDisplay), SLOT(on_contextMusicKey8_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(9, musicKeyDisplay), SLOT(on_contextMusicKey9_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(10, musicKeyDisplay), SLOT(on_contextMusicKey10_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(11, musicKeyDisplay), SLOT(on_contextMusicKey11_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(12, musicKeyDisplay), SLOT(on_contextMusicKey12_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(13, musicKeyDisplay), SLOT(on_contextMusicKey13_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(14, musicKeyDisplay), SLOT(on_contextMusicKey14_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(15, musicKeyDisplay), SLOT(on_contextMusicKey15_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(16, musicKeyDisplay), SLOT(on_contextMusicKey16_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(17, musicKeyDisplay), SLOT(on_contextMusicKey17_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(18, musicKeyDisplay), SLOT(on_contextMusicKey18_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(19, musicKeyDisplay), SLOT(on_contextMusicKey19_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(20, musicKeyDisplay), SLOT(on_contextMusicKey20_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(21, musicKeyDisplay), SLOT(on_contextMusicKey21_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(22, musicKeyDisplay), SLOT(on_contextMusicKey22_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(23, musicKeyDisplay), SLOT(on_contextMusicKey23_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(24, musicKeyDisplay), SLOT(on_contextMusicKey24_triggered())));
	sortedkeys.insert(std::pair<QString, const char*>(keys.keyName(25, musicKeyDisplay), SLOT(on_contextMusicKey25_triggered())));

	std::map<QString, const char*>::const_iterator it;
	for (it=sortedkeys.begin();it != sortedkeys.end();++it) {
		m->addAction(it->first, this, it->second);
	}
}

void Edit::on_contextMusicKeyVerified_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;
	DataTitle tUpdate=*t;
	if (tUpdate.Flags & 16) tUpdate.Flags-=16;
	else tUpdate.Flags|=16;
	try {
		wm->TitleStore.Put(tUpdate);
	} catch (const ppl7::Exception& exp) {
		ShowException(exp, tr("Could not save Title in TitleStore"));
		return;
	}
	RenderTrack(currentTrackListItem, tUpdate);

}

void Edit::on_contextSetMusicKey(int k)
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;
	DataTitle tUpdate=*t;
	tUpdate.Key=k;
	try {
		wm->TitleStore.Put(tUpdate);
	} catch (const ppl7::Exception& exp) {
		ShowException(exp, tr("Could not save Title in TitleStore"));
		return;
	}
	RenderTrack(currentTrackListItem, tUpdate);
}


void Edit::on_contextSynchronizeKeys_triggered()
{
	// Höchste Tracknummer
	int max=TrackList.GetMax();

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QProgressDialog progress(tr("Reading ID3-Tags from Files..."), tr("Abort"), 0, max, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setWindowTitle(QString(WM_APPNAME) + QString(": ") + tr("Reading ID3-Tags from Files..."));
	progress.setMinimumWidth(500);
	progress.setMaximumWidth(500);
	progress.show();

	QCoreApplication::processEvents();
	for (int i=1;i <= max;i++) {
		progress.setValue(i);
		QCoreApplication::processEvents();
		if (progress.wasCanceled())	break;
		const DataTrack* track=TrackList.GetPtr(i);
		if (track) {
			// Titel holen
			const DataTitle* title=wm->GetTitle(track->TitleId);
			ppl7::String Path=wm->GetAudioFilename(DeviceType, track->DeviceId, track->Page, track->Track);
			if (title != NULL && Path.notEmpty() == true) {
				//printf ("Path: %s\n",(const char*)Path);
				progress.setLabelText(Path);
				TrackInfo tinfo;
				if (getTrackInfoFromFile(tinfo, Path)) {
					DataTitle Ti;
					Ti.CopyFrom(*title);
					bool modified=false;
					bool modifyid3=false;
					if (tinfo.Ti.Key != title->Key && (title->Flags & 16) == 16) {
						tinfo.Ti.Key=Ti.Key;
						modifyid3=true;
					} else if (tinfo.Ti.Key != title->Key && (title->Flags & 16) == 0) {
						Ti.Key=tinfo.Ti.Key;
						modified=true;
					}
					ppl7::String MyKeyText= wm->MusicKeys.keyName(tinfo.Ti.Key, wm->conf.musicKeyTag);
					MyKeyText.replace("♯", "#");
					MyKeyText.replace("♭", "b");
					MyKeyText.replace("none", "o");
					MyKeyText.trim();
					if (tinfo.Ti.Key > 0 && tinfo.KeyText != MyKeyText) {
						if (wmlog) wmlog->printf("Key mismatch: File=%s, Wanted=%s", (const char*)tinfo.KeyText, (const char*)MyKeyText);
						modifyid3=true;
					}
					if (tinfo.Ti.Rating != Ti.Rating && Ti.Rating != 0) {
						//tinfo.Ti.Rating=Ti.Rating;
						//printf ("Rating will be saved: %d\n",track->Track);
						//modifyid3=true;
					} else if (tinfo.Ti.Rating > Ti.Rating) {
						Ti.Rating=tinfo.Ti.Rating;
						modified=true;
					}
					if (tinfo.Ti.EnergyLevel > 0 && Ti.EnergyLevel != tinfo.Ti.EnergyLevel) {
						Ti.EnergyLevel=tinfo.Ti.EnergyLevel;
						modified=true;
					}
					if (tinfo.Ti.BPM > 0 && Ti.BPM != tinfo.Ti.BPM) {
						Ti.BPM=tinfo.Ti.BPM;
						modified=true;
					}
					if (tinfo.Ti.Bitrate > 0 && tinfo.Ti.Bitrate != Ti.Bitrate) {
						Ti.Bitrate=tinfo.Ti.Bitrate;
						modified=true;
					}
					if (tinfo.Ti.Length > 0 && tinfo.Ti.Length != Ti.Length) {
						Ti.Length=tinfo.Ti.Length;
						modified=true;
					}
					if (tinfo.Ti.Size > 0 && tinfo.Ti.Size != Ti.Size) {
						Ti.Size=tinfo.Ti.Size;
						modified=true;
					}
					if (Ti.CoverPreview.size() == 0) {
						Ti.CoverPreview.copy(tinfo.Ti.CoverPreview);

					}
					if (modified) {
						try {
							wm->TitleStore.Put(Ti);
						} catch (const ppl7::Exception& exp) {
							ShowException(exp, tr("Could not save Title in TitleStore"));
							break;
						}
					}
					if (modifyid3) {
						if (!wm->SaveID3Tags(title->DeviceType, title->DeviceId, title->Page, title->Track, Ti)) {
							wm->RaiseError(this, tr("Could not save ID3 Tags"));
						}
					}
				}
			}
		}
	}
	QApplication::restoreOverrideCursor();
	UpdateTrackListing();
}

void Edit::on_contextShowCover_triggered()
{
	if (!currentTrackListItem) return;
	on_trackList_itemDoubleClicked(currentTrackListItem, TRACKLIST_COVER_ROW);
}

void Edit::on_contextLoadCoverAllTracks_triggered()
{
	ppl7::String Dir=wm->conf.LastCoverPath + "/";
	if (Dir.isEmpty()) {
		Dir=QDir::homePath();
	}
	QString newfile = QFileDialog::getOpenFileName(this, tr("Select cover image"),
		Dir,
		tr("Images (*.png *.bmp *.jpg)"));
	if (newfile.isNull()) return;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	wm->conf.LastCoverPath=ppl7::File::getPath(newfile);
	wm->conf.trySave();

	QPixmap GlobalCover;

	if (!GlobalCover.load(newfile)) {
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this, tr("Error: could not load Cover"),
			tr("The specified file could not be loaded.\nPlease check if the file exists, is readable and contains an image format, which is supported by WinMusik (.png, .jpg or .bmp)")
		);
		return;
	}
	QPixmap icon=GlobalCover.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	icon.save(&buffer, "JPEG", wm->conf.JpegQualityPreview);
	//Ti.CoverPreview.Copy(bytes.data(),bytes.size());

	for (int i=TrackList.GetMin();i <= TrackList.GetMax();i++) {
		const DataTrack* track=TrackList.GetPtr(i);
		if (track) {
			const DataTitle* ti=wm->GetTitle(track->TitleId);
			if (ti) {
				DataTitle Title;
				Title.CopyFrom(*ti);
				ppl7::String Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, i);
				saveCover(Path, GlobalCover);
				Title.CoverPreview.copy(bytes.data(), bytes.size());
				try {
					wm->TitleStore.Put(Title);
				} catch (const ppl7::Exception& exp) {
					ShowException(exp, tr("Could not save Title in TitleStore"));
					return;
				}
			}
		}
	}
	UpdateTrackListing();
	QApplication::restoreOverrideCursor();
}

void Edit::on_contextCopyCover_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t != NULL) {
		ppl7::String Path=wm->GetAudioFilename(t->DeviceType, t->DeviceId, t->Page, t->Track);
		if (Path.isEmpty()) return;
		loadCoverToClipboard(Path);
	}
}


void Edit::on_contextFindMoreVersions_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow, t->Artist, t->Title);
	}
}

void Edit::on_contextFindMoreArtist_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow, t->Artist);
	}
}

void Edit::on_contextFindMoreTitle_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		searchWindow=wm->OpenOrReuseSearch(searchWindow, NULL, t->Title);
	}
}

void Edit::on_contextPlayTrack_triggered()
{
	ppl7::String Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, currentTrackListItem->Track);
	if (Path.isEmpty()) return;
	//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (const char*)Path);
	wm->PlayFile(Path);
}

void Edit::on_contextEditTrack_triggered()
{
	showEditor();
	ppl7::String Tmp;
	Tmp.setf("%i", currentTrackListItem->Track);
	TrackNum=0;
	ui.track->setFocus();
	ui.track->setText(Tmp);
	ui.artist->setFocus();
}

void Edit::on_contextCopyTrack_triggered()
{
	on_trackList_itemClicked(currentTrackListItem, 0);
}

void Edit::on_contextCopyFile_triggered()
{
	if (currentTrackListItem->Track > 0) {
		ppl7::String Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, currentTrackListItem->Track);
		if (Path.isEmpty()) return;
		QClipboard* clipboard = QApplication::clipboard();
		QList<QUrl> list;
		QString qf="file:://";
		qf+=(const char*)Path;
		list.append(QUrl(qf));
		QMimeData* mimeData = new QMimeData;
		mimeData->setUrls(list);
		//printf ("Adding to Clipboard\n");
		clipboard->setMimeData(mimeData, QClipboard::Clipboard);
	}
}

void Edit::on_contextDeleteTrack_triggered()
{
	if (!currentTrackListItem->Track) return;
	// Track löschen und nachfolgende nach oben rücken
	DeleteShift(currentTrackListItem->Track);
	UpdateTrackListing();
	QTreeWidgetItem* w=trackList->topLevelItem(currentTrackListItem->Track);
	if (w) {
		trackList->scrollToItem(w);
	}
	ppl7::String Tmp;
	Tmp.setf("%u", currentTrackListItem->Track);
	ui.track->setText(Tmp);
	ui.track->setFocus();
}

void Edit::on_contextInsertTrack_triggered()
{
	if (!currentTrackListItem->Track) return;
	InsertShift(currentTrackListItem->Track);
	UpdateTrackListing();
	QTreeWidgetItem* w=trackList->topLevelItem(currentTrackListItem->Track);
	if (w) {
		trackList->scrollToItem(w);
	}
	ppl7::String Tmp;
	Tmp.setf("%u", currentTrackListItem->Track);
	ui.track->setText(Tmp);
	ui.track->setFocus();
}


void Edit::on_coverSearchAmazon_clicked()
{
	FixFocus();
	if (position < 3) return;
	ppl7::String Url;
	// Interpret und Titel
	ppl7::String Artist=ui.artist->text();
	Artist.trim();
	ppl7::String Title=ui.title->text();
	Title.trim();
	Url="http://www.amazon.de/s/?ie=UTF8&index=digital-music&keywords=" + ppl7::UrlEncode(Artist) + "+" + ppl7::UrlEncode(Title);
	QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

void Edit::on_coverSearchGoogle_clicked()
{
	FixFocus();
	if (position < 3) return;
	ppl7::String Url;
	// Interpret und Titel
	ppl7::String Artist=ui.artist->text();
	Artist.trim();
	ppl7::String Title=ui.title->text();
	Title.trim();
	Url="https://www.google.com/search?num=10&site=imghp&tbm=isch&q=" + ppl7::UrlEncode(Artist) + "+" + ppl7::UrlEncode(Title);
	QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

void Edit::on_coverSearchDiscogs_clicked()
{
	FixFocus();
	if (position < 3) return;
	ppl7::String Url;
	// Interpret und Titel
	ppl7::String Artist=ui.artist->text();
	Artist.trim();
	ppl7::String Title=ui.title->text();
	Title.trim();
	ppl7::String Version=ui.version->text();
	Version.trim();
	ppl7::String Query=Artist + " " + Title + " " + Version;
	Query.replace("&", " ");
	Url="http://www.discogs.com/search/?q=" + ppl7::UrlEncode(Query);
	QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}


void Edit::on_coverSearchBeatport_clicked()
{
	FixFocus();
	if (position < 3) return;
	ppl7::String Url;
	// Interpret und Titel
	ppl7::String Artist=ui.artist->text();
	Artist.trim();
	ppl7::String Title=ui.title->text();
	Title.trim();
	Url="http://www.beatport.com/search?query=" + ppl7::UrlEncode(Artist) + "+" + ppl7::UrlEncode(Title);
	QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

void Edit::on_coverSearchSoundcloud_clicked()
{
	FixFocus();
	if (position < 3) return;
	ppl7::String Url;
	// Interpret und Titel
	ppl7::String Artist=ui.artist->text();
	Artist.trim();
	ppl7::String Title=ui.title->text();
	Title.trim();
	Url="https://soundcloud.com/search?q=" + ppl7::UrlEncode(Artist) + "+" + ppl7::UrlEncode(Title);
	QDesktopServices::openUrl(QUrl(Url, QUrl::TolerantMode));
}

bool Edit::on_f6_MassImport()
{
	MassImport Import(this, wm);
	Import.setSearchWindow(searchWindow);
	Import.show();
	if (!Import.load(DeviceType, DeviceId, Page, TrackList.GetMax() + 1)) {
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

void Edit::on_deviceIcon_clicked()
{
	//printf ("Edit::on_deviceIcon_clicked\n");
	ppl7::String path=wm->GetAudioPath(DeviceType, DeviceId, Page);
	if (path.notEmpty()) {
		QDesktopServices::openUrl(QUrl::fromLocalFile(path));
	}
}

void Edit::on_displayMusicKey_currentIndexChanged(int)
{
	switch (ui.displayMusicKey->currentIndex()) {
		case 0: musicKeyDisplay=musicKeyTypeMusicalSharps; break;
		case 1: musicKeyDisplay=musicKeyTypeMusicalFlats; break;
		case 2: musicKeyDisplay=musicKeyTypeOpenKey; break;
		case 3: musicKeyDisplay=musicKeyTypeCustom; break;
		default: musicKeyDisplay=musicKeyTypeOpenKey; break;
	}
	UpdateTrackListing();
	FixFocus();
}

void Edit::on_contextDeleteID3Tags_triggered()
{
	QList<QTreeWidgetItem*> Items=trackList->selectedItems();
	if (Items.count() < 1) return;
	//ppl7::PrintDebug("Edit::on_contextDeleteID3Tags_triggered\n");
	if (QMessageBox::question(this, tr("WinMusik: delete ID3-Tags"),
		tr("Do you want to delete all ID3-Tags from the selected files?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
		== QMessageBox::No) return;
	for (int i=0;i < Items.size();i++) {
		WMTreeItem* item=(WMTreeItem*)Items[i];
		ppl7::String Filename=wm->GetAudioFilename(DeviceType, DeviceId, Page, item->Track);
		if (Filename.notEmpty()) {
			//ppl7::PrintDebug("File: %s\n", (const char*)Filename);
			ppl7::ID3Tag Tag;
			try {
				Tag.load(Filename);
				Tag.clearTags();
				Tag.save();

			} catch (...) {

			}
		}
	}

}
