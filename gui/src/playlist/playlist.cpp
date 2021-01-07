/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: Playlist.cpp,v 1.1 2010/11/14 13:20:11 pafe Exp $
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


#include <QClipboard>
#include <QMenu>
#include <QList>
#include <QUrl>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>
#include <QKeyEvent>
#include <QDomDocument>
#include <QBuffer>
#include <QDate>
#include <QDir>
#include <vector>

#include "playlisttracks.h"
#include "playlist.h"
#include "playlistedit.h"
#include "traktor.h"
#include "setbpmplayed.h"
#include "playlistexport.h"
#include "version.h"
#include "musickey.h"
#include "editstring.h"
#include <stdio.h>
#include <ppl6-sound.h>

/*
class Playlist;

class PlaylistTab : public QWidget
{
		Q_OBJECT
	public:
		PlaylistTab(QWidget *parent = 0);
		~PlaylistTab();
		PlaylistTracks *tracks;

		void setPlaylist(Playlist *p);
	private:


};

PlaylistTab::PlaylistTab(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *layout=new QVBoxLayout;
	layout->setContentsMargins(2,2,2,2);

	tracks=new PlaylistTracks;
	tracks->setAcceptDrops(true);
	tracks->sortByColumn(0,Qt::AscendingOrder);
	tracks->setMouseTracking(false);
	tracks->setContextMenuPolicy(Qt::CustomContextMenu);
	tracks->setAcceptDrops(true);
	tracks->setDropIndicatorShown(true);
	tracks->setDragEnabled(true);
	tracks->setDragDropOverwriteMode(false);
	tracks->setDragDropMode(QAbstractItemView::DragOnly);
	tracks->setDefaultDropAction(Qt::IgnoreAction);
	tracks->setAlternatingRowColors(true);
	tracks->setSelectionMode(QAbstractItemView::ExtendedSelection);
	tracks->setSelectionBehavior(QAbstractItemView::SelectRows);
	tracks->setIconSize(QSize(64,16));
	tracks->setRootIsDecorated(false);
	tracks->setUniformRowHeights(true);
	tracks->setItemsExpandable(false);
	tracks->setSortingEnabled(true);
	tracks->setExpandsOnDoubleClick(false);

	layout->addWidget(tracks);
	setLayout(layout);
}

PlaylistTab::~PlaylistTab()
{

}

void PlaylistTab::setPlaylist(Playlist *p)
{
	tracks->setPlaylist(p);
}

*/

Playlist::Playlist(QWidget *parent, CWmClient *wm)
    : QMainWindow(parent)
{
	ui.setupUi(this);
    ui.filterFrame->setVisible(false);
    this->setStatusBar(nullptr);
	this->wm=wm;
	harmonicsHighlighted=false;
    currentTreeItem=nullptr;
    searchWindow=nullptr;
    saveWidget=saveAsWidget=nullptr;
	setAttribute(Qt::WA_DeleteOnClose,true);
	ui.tracks->setPlaylist(this);
	ui.tracks->setAcceptDrops(true);

    ui.filterFrame->setAcceptDrops(true);
    ui.currentTrackGroupBox->setAcceptDrops(true);
    ui.ct_cover->setAcceptDrops(true);
    ui.filterFrame->installEventFilter(this);
    ui.currentTrackGroupBox->installEventFilter(this);
    ui.ct_cover->installEventFilter(this);

	this->setWindowTitle(tr("WinMusik Playlist"));

	ui.tracks->installEventFilter(this);
	ui.playlistName->installEventFilter(this);
    ui.issueDate->setDate(QDate::currentDate());

	createMenue();
	createToolbar();
	createStatusBar();

    playlistView=static_cast<playlistViewType>(wm->conf.playlistView);
    musicKeyDisplay=wm->conf.musicKeyDisplay;

	recreatePlaylist();
	changed=true;
	setChanged(false);

	ui.tracks->sortByColumn(0,Qt::AscendingOrder);

	restoreGeometry(wm->GetGeometry("playlist"));
	ui.tracks->setFocus();

	//ui.tracks->setStyleSheet("QTreeWidget::item { border-left-color: rgb(158, 158, 158)};");
}


Playlist::~Playlist()
{
	if (wm) {
		wm->PlaylistClosed(this);
	}
}


void Playlist::createMenue()
{
	QMenu	*menu;
	menu=menuBar()->addMenu(tr("&File"));

	menu->addAction(QIcon(":/icons/resources/filenew.png"),tr("&new Playlist"),this,SLOT(on_menuNew_triggered()));
	menu->addAction(QIcon(":/icons/resources/fileopen.png"),tr("&load Playlist"),this,SLOT(on_menuOpen_triggered()));
	menu->addAction(QIcon(":/icons/resources/filesave.png"),tr("&save Playlist"),this,SLOT(on_menuSave_triggered()));
	menu->addAction(QIcon(":/icons/resources/filesaveas.png"),tr("save Playlist &as"),this,SLOT(on_menuSaveAs_triggered()));
	menu->addSeparator();
    menu->addAction(QIcon(":/icons/resources/export.png"),tr("e&xport Playlist"),this,SLOT(on_menuExport_triggered()));
    menu->addAction(QIcon(":/icons/resources/printer16.png"),tr("&Print"),this,SLOT(on_printPlaylist_triggered()));
    menu->addSeparator();

	menuRecentPlaylists=menu->addMenu(QIcon(":/icons/resources/fileopen.png"),tr("&recent Playlists"));
	updateRecentPlaylistsMenu();

	menu=menuBar()->addMenu(tr("&View"));
	menu->addAction(QIcon(":/icons/resources/view_playlist.png"),tr("&Playlist"),this,SLOT(on_viewPlaylist_triggered()));
	menu->addAction(QIcon(":/icons/resources/view_dj.png"),tr("&DJ"),this,SLOT(on_viewDJ_triggered()));
    menu->addAction(QIcon(":/icons/resources/view_filter.png"),tr("&Filter"),this,SLOT(on_viewFilter_triggered()));

}

void Playlist::createToolbar()
{
	QToolBar *tb=new QToolBar();
	QAction *action;
	tb->setFloatable(false);
	tb->setMovable(false);

	tb->addAction(QIcon(":/icons/resources/filenew.png"),tr("&new Playlist"),this,SLOT(on_menuNew_triggered()));
	tb->addAction(QIcon(":/icons/resources/fileopen.png"),tr("&load Playlist"),this,SLOT(on_menuOpen_triggered()));
	action=tb->addAction(QIcon(":/icons/resources/filesave.png"),tr("&save Playlist"),this,SLOT(on_menuSave_triggered()));
	saveWidget=tb->widgetForAction(action);
	action=tb->addAction(QIcon(":/icons/resources/filesaveas.png"),tr("save Playlist &as"),this,SLOT(on_menuSaveAs_triggered()));
	saveAsWidget=tb->widgetForAction(action);
	tb->addSeparator();
    tb->addAction(QIcon(":/icons/resources/export.png"),tr("e&xport Playlist"),this,SLOT(on_menuExport_triggered()));
    tb->addAction(QIcon(":/icons/resources/printer16.png"),tr("&Print"),this,SLOT(on_printPlaylist_triggered()));
    tb->addAction(QIcon(":/icons/resources/randomize.png"),tr("shuffle Playlist"),this,SLOT(on_shufflePlaylist_triggered()));
    tb->addSeparator();
	tb->addAction(QIcon(":/icons/resources/view_playlist.png"),tr("view &Playlist"),this,SLOT(on_viewPlaylist_triggered()));
	tb->addAction(QIcon(":/icons/resources/view_dj.png"),tr("view &DJ"),this,SLOT(on_viewDJ_triggered()));
    tb->addAction(QIcon(":/icons/resources/view_filter.png"),tr("view &Filter"),this,SLOT(on_viewFilter_triggered()));

    this->addToolBar(tb);

}

void Playlist::createStatusBar()
{
	statusbar=new PlaylistStatusBar;
	statusbar->setMusicKeySelectionEnabled(true);
	ui.centralwidget->layout()->addWidget(statusbar);
	//this->layout()->addWidget(statusbar);
	//this->statusBar()->addWidget(statusbar);
	connect(statusbar,SIGNAL(musicKeySelectionChanged(int)),
				this, SLOT(on_statusbar_musicKeySelectionChanged(int)));

	connect(statusbar,SIGNAL(searchTriggered()),
				this, SLOT(on_searchTriggered()));

}

void Playlist::updateRecentPlaylistsMenu()
{
	menuRecentPlaylists->clear();
	if (wm->conf.LastPlaylists[0].NotEmpty()) menuRecentPlaylists->addAction(QIcon(":/icons/resources/fileopen.png"),wm->conf.LastPlaylists[0],this,SLOT(on_menuOpenRecent0_triggered()));
	if (wm->conf.LastPlaylists[1].NotEmpty()) menuRecentPlaylists->addAction(QIcon(":/icons/resources/fileopen.png"),wm->conf.LastPlaylists[1],this,SLOT(on_menuOpenRecent1_triggered()));
	if (wm->conf.LastPlaylists[2].NotEmpty()) menuRecentPlaylists->addAction(QIcon(":/icons/resources/fileopen.png"),wm->conf.LastPlaylists[2],this,SLOT(on_menuOpenRecent2_triggered()));
	if (wm->conf.LastPlaylists[3].NotEmpty()) menuRecentPlaylists->addAction(QIcon(":/icons/resources/fileopen.png"),wm->conf.LastPlaylists[3],this,SLOT(on_menuOpenRecent3_triggered()));
	if (wm->conf.LastPlaylists[4].NotEmpty()) menuRecentPlaylists->addAction(QIcon(":/icons/resources/fileopen.png"),wm->conf.LastPlaylists[4],this,SLOT(on_menuOpenRecent4_triggered()));

}

void Playlist::setChanged(bool flag)
{
	if (changed!=flag) {
		changed=flag;
		if (saveWidget) saveWidget->setEnabled(changed);
		//if (saveAsWidget) saveAsWidget->setEnabled(changed);

	}

}

void Playlist::closeEvent(QCloseEvent *event)
{
	if (saveFirst()!=QMessageBox::Ok) {
		event->ignore();
		return;
	}
	if (wm) {
		wm->SaveGeometry("playlist",saveGeometry());
	}
	QMainWindow::closeEvent(event);
}

QMessageBox::StandardButton Playlist::saveFirst()
{
	if (!changed) return QMessageBox::Ok;

	QMessageBox::StandardButton ret=QMessageBox::question(this,tr("Save playlist"),
			tr("This playlist was modified since the last save.\nDo you want to save your changes?"),
			QMessageBox::No|QMessageBox::Yes|QMessageBox::Abort, QMessageBox::Abort);
	if (ret==QMessageBox::Abort) return QMessageBox::Abort;
	if (ret==QMessageBox::No) return QMessageBox::Ok;

	if (PlaylistFileName.IsEmpty()) {
		ppl6::CString Filename=PlaylistFileName;
		if (Filename.IsEmpty()) Filename=wm->conf.LastPlaylistPath+"/playlist.wmp";
		ppl6::CString Tmp=QFileDialog::getSaveFileName (this, tr("Save WinMusik Playlist"), Filename,
				tr("Playlists (*.wmp)"));
		if (Tmp.IsEmpty()) return QMessageBox::Abort;
		PlaylistFileName=Tmp;
	}

    ui.tracks->setName(ui.playlistName->text());
    ui.tracks->setSubName(ui.playlistSubName->text());
    ui.tracks->setIssueNumber(ui.issueNumber->value());
    ui.tracks->setIssueDate(ppl6::CDateTime(ui.issueDate->date().toString()));

	if (!ui.tracks->save(PlaylistFileName)) return QMessageBox::Abort;
	wm->conf.LastPlaylistPath=ppl6::GetPath(PlaylistFileName);
	updateLastPlaylist();
	updateRecentPlaylistsMenu();
	wm->conf.Save();
	setChanged(false);
	ppl6::CString Title=tr("WinMusik Playlist");
	Title+=" - "+PlaylistFileName;
	this->setWindowTitle(Title);
	return QMessageBox::Ok;
}

bool Playlist::eventFilter(QObject *target, QEvent *event)
{
	if (consumeEvent(target,event)) return true;
	return QMainWindow::eventFilter(target,event);
}

bool Playlist::consumeEvent(QObject *target, QEvent *event)
{
	int type=event->type();
	//printf ("Event: %i\n",type);
	if (type==QEvent::KeyPress) {
		QKeyEvent *e=static_cast<QKeyEvent *>(event);
		if (target==ui.tracks) {
            currentTreeItem=static_cast<PlaylistItem*>(ui.tracks->currentItem());
			//printf ("Key: %i\n",e->key());
			if (e->key()==Qt::Key_Delete) {
				QList<QTreeWidgetItem *>selected=ui.tracks->selectedItems();
				if (selected.count()>1) {
					if (QMessageBox::question (this, tr("Delete Tracks"),
							tr("Do you want to delete all selected tracks from this playlist?"),
							QMessageBox::No|QMessageBox::Yes,
							QMessageBox::No) != QMessageBox::Yes) return false;
				}
				ui.tracks->deleteSelectedItems();
				setChanged(true);
				updateLengthStatus();
				renumberTracks();
				return true;
			} else if (e->key()==Qt::Key_E) {
				if(!currentTreeItem) return false;
                if (e->modifiers()==Qt::ShiftModifier) {
                    wm->OpenEditor(currentTreeItem->DeviceType,
                                   currentTreeItem->DeviceId,
                                   currentTreeItem->DevicePage,
                                   currentTreeItem->DeviceTrack);
                } else {
                    editTrack(currentTreeItem);
                }
				return true;
            } else if (e->key()==Qt::Key_C) {
                if(!currentTreeItem) return false;
                on_contextEditComment_triggered();
                return true;
			} else if (e->key()==Qt::Key_P) {
				if(!currentTreeItem) return false;
				wm->PlayFile((currentTreeItem)->File);
				return true;
			} else if (e->key()==Qt::Key_F && e->modifiers()==Qt::ControlModifier) {
				statusbar->setFocusOnSearch();
				return true;
			}
			return false;
		} else if (target==ui.playlistName && e->key()==Qt::Key_F && e->modifiers()==Qt::ControlModifier) {
			statusbar->setFocusOnSearch();
			return true;
		}
    } else if (type==QEvent::DragEnter) {
        if (target==ui.ct_cover
                || target==ui.currentTrackGroupBox
                || target==ui.filterFrame) {
            QDragEnterEvent *e=static_cast<QDragEnterEvent *>(event);
            e->accept();
            return true;
        }
    } else if (type==QEvent::Drop) {
        if (target==ui.ct_cover
                || target==ui.currentTrackGroupBox
                || target==ui.filterFrame) {
            QDropEvent *e=static_cast<QDragEnterEvent *>(event);
            handleFilterDropEvent(e);
            return true;
        }
    }
	return false;
}

bool Playlist::on_tracks_MouseButtonPress(QMouseEvent * event)
{
	//printf ("on_tracks_MouseButtonPress\n");
	if (event->buttons() == Qt::LeftButton) startPos=event->pos();
	ratePos=event->pos();
	return false;
}

bool Playlist::on_tracks_MouseButtonRelease(QMouseEvent *)
{
	//printf ("on_tracks_MouseButtonRelease\n");
	startPos.setX(0);
	startPos.setY(0);
	return false;
}

void Playlist::on_tracks_itemSelectionChanged ()
{
	updateLengthStatus();
}

void Playlist::handleDropEvent(QDropEvent *event)
{
    if (isFilterEnabled()) {
        event->ignore();
        return;
    }
    //printf("Playlist::handleDropEvent\n");
	event->accept();
	ui.tracks->unselectItems();
	//if (event->source()==this) printf ("Quelle identisch\n"); else printf ("Fremdquelle\n");
	const QMimeData *mime=event->mimeData();
	if (!mime) return;
	// QList<QTreeWidgetItem *>selected=ui.tracks->selectedItems();
	//QPoint p=(static_cast<QDragMoveEvent *>(event))->pos()-ui.tracks->geometry().topLeft();
	QPoint p=(static_cast<QDragMoveEvent *>(event))->pos();
	QTreeWidgetItem *insertItem=ui.tracks->itemAt(p);
	handleDropEvent(mime,insertItem);
	/*
	if (event->source()==this) ui.tracks->deleteItems(selected);
	updateLengthStatus();
	renumberTracks();
	*/
}

void Playlist::handleDropEvent(const QMimeData *mime, QTreeWidgetItem *insertItem)
{
    if (isFilterEnabled()) return;
	//ui.tracks->unselectItems();
	QApplication::processEvents();

	ui.tracks->setSortingEnabled(false);
	if (mime->hasFormat("application/winmusik+xml")) {
		QByteArray ba=mime->data("application/winmusik+xml");
		ppl6::CString xml;
		xml.Set(ba.constData(), ba.size());
		if (xml.Left(18)=="<winmusikTracklist") {
			//printf ("XML-Drop\n");
			handleXMLDrop(xml,insertItem);
		} else {
			QList<QUrl>	list=mime->urls();
			handleURLDrop(list,insertItem);
		}
	} else {
		QList<QUrl>	list=mime->urls();
		handleURLDrop(list,insertItem);
	}
	updateLengthStatus();
	renumberTracks();
	ui.tracks->setSortingEnabled(true);
}

void Playlist::handleXMLDrop(const ppl6::CString &xml, QTreeWidgetItem *insertItem)
{
//	printf ("Playlist::handleXMLDrop\n");
	//xml.Print(true);
    if (isFilterEnabled()) return;
	QDomDocument doc("winmusikTracklist");
	if (doc.setContent(xml)) {
		QDomElement root=doc.documentElement();
		if (root.tagName()=="winmusikTracklist") {
			QDomNode tracks=root.namedItem("tracks");
			if (tracks.isNull()==false) {
				QDomElement e=tracks.firstChildElement("item");
				while (!e.isNull()) {
					PlaylistItem *item=new PlaylistItem;
					item->importFromXML(e);
					item->updateFromDatabase();
					renderTrack(item);
					if (insertItem) ui.tracks->insertTopLevelItem(ui.tracks->indexOfTopLevelItem(insertItem),item);
					else ui.tracks->addTopLevelItem(item);
					setChanged(true);
					//item->setSelected(true);
					e=e.nextSiblingElement("item");
				}
			}
		}
	}
}

void Playlist::handleURLDrop(const QList<QUrl> &list, QTreeWidgetItem *insertItem)
{
	//printf ("Playlist::handleURLDrop\n");
    if (isFilterEnabled()) return;
	for (int i=0;i<list.size();i++) {
		QUrl url=list[i];
		QString file=url.toLocalFile();
		PlaylistItem *item=new PlaylistItem;
		item->File=file;
		ppluint32 titleId=findTitleIdByFilename(file);
		if (titleId) loadTrackFromDatabase(item,titleId);
		else loadTrackFromFile(item,file);
		renderTrack(item);
		setChanged(true);
		if (insertItem) ui.tracks->insertTopLevelItem(ui.tracks->indexOfTopLevelItem(insertItem),item);
		else ui.tracks->addTopLevelItem(item);
		//item->setSelected(true);
	}
}

bool Playlist::loadTrackFromDatabase(PlaylistItem *item, ppluint32 titleId)
{
	DataTitle *ti=wm->GetTitle(titleId);
	if (!ti) return false;
	item->DeviceId=ti->DeviceId;
	item->DeviceTrack=ti->Track;
	item->DeviceType=ti->DeviceType;
	item->DevicePage=ti->Page;

	item->titleId=titleId;
	item->Artist=ti->Artist;
	item->Title=ti->Title;
	item->Version=wm->GetVersionText(ti->VersionId);
	item->Genre=wm->GetGenreText(ti->GenreId);
	item->Label=wm->GetLabelText(ti->LabelId);
	item->Album=ti->Album;
	item->musicKey=ti->Key;
	item->keyVerified=(ti->Flags>>4)&1;
	item->energyLevel=ti->EnergyLevel;
    item->bpm=ti->BPM;
    item->bitrate=ti->Bitrate;
	item->bpmPlayed=0;
	item->rating=ti->Rating;
	item->trackLength=ti->Length;
	item->mixLength=ti->Length;
	item->startPositionSec=0;
	item->endPositionSec=ti->Length;
	for (int z=0;z<5;z++) {
		item->cutStartPosition[z]=0;
		item->cutEndPosition[z]=0;
	}
	item->CoverPreview=ti->CoverPreview;
	item->File=wm->GetAudioFilename(ti->DeviceType,ti->DeviceId,ti->Page,ti->Track);
	item->useTraktorCues(item->File);
	return true;
}


void Playlist::loadTrackFromFile(PlaylistItem *item, const ppl6::CString &file)
{
	item->File=file;
	TrackInfo info;

	if (!getTrackInfoFromFile(info,file)) return;
	//printf ("Playlist::loadTrackFromFile: %s\n",(const char*)file);
	item->Artist=info.Ti.Artist;
	item->Title=info.Ti.Title;
	item->Album=info.Ti.Album;
	item->Version=info.Version;
	item->Genre=info.Genre;
	item->Label=info.Label;
	item->trackLength=info.Ti.Length;
	item->mixLength=item->trackLength;
	item->endPositionSec=item->trackLength;
	item->bpm=info.Ti.BPM;
	item->rating=info.Ti.Rating;
    item->bitrate=info.Ti.Bitrate;
	item->musicKey=info.Ti.Key;
	item->energyLevel=info.Ti.EnergyLevel;
	item->CoverPreview=info.Ti.CoverPreview;

	item->useTraktorCues(file);
}


void Playlist::Resize()
{
	int w=ui.tracks->width();
	ui.tracks->setColumnWidth(columnTrack,40);
	w-=44;
	ui.tracks->setColumnWidth(columnCover,64);
	w-=68;
	ui.tracks->setColumnWidth(columnLength,60);
	w-=64;
	ui.tracks->setColumnWidth(columnRating,50);
	w-=53;
	ui.tracks->setColumnWidth(columnSource,65);
	w-=69;
    ui.tracks->setColumnWidth(columnBitrate,30);
    w-=34;


	if (playlistView==playlistViewNormal) {
		ui.tracks->setColumnWidth(columnTitle,w*80/100);
		ui.tracks->setColumnWidth(columnGenre,w*15/100);

	} else if (playlistView==playlistViewDJ) {
		ui.tracks->setColumnWidth(columnBpm,35);
		w-=39;
		ui.tracks->setColumnWidth(columnBpmPlayed,45);
		w-=49;
        ui.tracks->setColumnWidth(columnMusicKey,55);
        w-=59;
		ui.tracks->setColumnWidth(columnEnergyLevel,30);
		w-=34;
        ui.tracks->setColumnWidth(columnStart,50);
        w-=54;
        ui.tracks->setColumnWidth(columnEnd,50);
        w-=54;
        ui.tracks->setColumnWidth(columnCuts,50);
        w-=54;
        ui.tracks->setColumnWidth(columnTotalLength,50);
        w-=54;

		ui.tracks->setColumnWidth(columnTitle,w*65/100);
		ui.tracks->setColumnWidth(columnGenre,w*15/100);
		ui.tracks->setColumnWidth(columnComment,w*20/100);
	}



}

void Playlist::recreatePlaylist()
{
	QTreeWidgetItem *item=ui.tracks->headerItem();

	columnTrack=0;
	columnCover=1;
	columnTitle=2;
	columnGenre=3;
	columnComment=4;
	columnBpm=5;
	columnBpmPlayed=6;
	columnMusicKey=7;
	columnEnergyLevel=8;
	columnRating=9;
	columnStart=10;
	columnEnd=11;
	columnCuts=12;
	columnLength=13;
	columnTotalLength=14;
    columnBitrate=15;
    columnSource=16;

	if (playlistView==playlistViewNormal) {
        ui.tracks->setColumnCount(8);
		columnLength=4;
		columnRating=5;
        columnBitrate=6;
        columnSource=7;

		item->setText(columnTrack,tr("Track"));
		item->setText(columnCover,tr("Cover"));
		item->setText(columnTitle,tr("Artist - Title (Version)"));
		item->setText(columnGenre,tr("Genre"));
		item->setText(columnLength,tr("Length"));
		item->setText(columnRating,tr("Rating"));
        item->setText(columnBitrate,tr("Bitrate"));
		item->setText(columnSource,tr("Source"));

	} else if (playlistView==playlistViewDJ) {
		ui.tracks->setColumnCount(16);
		item->setText(columnTrack,tr("Track"));
		item->setText(columnCover,tr("Cover"));
		item->setText(columnTitle,tr("Artist - Title (Version)"));
		item->setText(columnGenre,tr("Genre"));
		item->setText(columnComment,tr("Comment"));
		item->setText(columnBpm,tr("Bpm"));
		item->setText(columnBpmPlayed,tr("MixBpm"));
		item->setText(columnMusicKey,tr("Key"));
		item->setText(columnEnergyLevel,tr("Energy"));
		item->setText(columnRating,tr("Rating"));
		item->setText(columnStart,tr("Start"));
		item->setText(columnEnd,tr("End"));
		item->setText(columnCuts,tr("Cuts"));
		item->setText(columnLength,tr("Length"));
		item->setText(columnTotalLength,tr("Total"));
        item->setText(columnBitrate,tr("Bitrate"));
		item->setText(columnSource,tr("Source"));
	}
	Resize();
	updatePlaylist();
}

void Playlist::updatePlaylist()
{
	for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		renderTrack(item);
		item->setText(0,ppl6::ToString("%5d",i+1));
	}
	updateLengthStatus();
}

void Playlist::renumberTracks()
{
	for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		item->setText(0,ppl6::ToString("%5d",i+1));
	}
}

void Playlist::updateLengthStatus()
{
	ppl6::CString Tmp;
	int trackLength=0.0f;
	float mixLength=0.0f;
	int selectedLength=0.0f;
	float selectedMixLength=0.0f;
	int selectedTracks=0;
	for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		trackLength+=item->trackLength;
		mixLength+=item->mixLength;
		if (playlistView==playlistViewDJ) {
            item->setText(columnTotalLength,getReadableTimeFromSeconds(mixLength));
		}
		if (item->isSelected()) {
			selectedTracks++;
			selectedLength+=item->trackLength;
			selectedMixLength+=item->mixLength;
		}
	}
	statusbar->setTotalLength((int)trackLength);
	statusbar->setMixLength((int)mixLength);
	statusbar->setTotalTracks(ui.tracks->topLevelItemCount());

	statusbar->setSelectedLength((int)selectedLength);
	statusbar->setSelectedMixLength((int)selectedMixLength);
	statusbar->setSelectedTracks(selectedTracks);
}

void Playlist::renderTrack(PlaylistItem *item)
{
	QColor color(0,0,0);
	QBrush b=item->background(0);
    item->setBackground(columnMusicKey,b);
	for (int i=1;i<item->columnCount();i++) {
		item->setText(i,"");
		item->setIcon(i,QIcon());
        //item->setTextColor(i,color);
	}
	QFont f=item->font(columnMusicKey);
	f.setBold(false);
	item->setFont(columnMusicKey,f);


    //QRect rr=ui.tracks->visualItemRect(item);

    int ch=16;
    //if (rr.height()>ch) ch=rr.height();
    //if (ch>64) ch=64;

	if (item->CoverPreview.Size()>0) {
		QPixmap pix, icon;
		pix.loadFromData((const uchar*)item->CoverPreview.GetPtr(),item->CoverPreview.Size());
		icon=pix.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        item->setIcon(columnCover,icon.copy(0,0,64,ch));
        //ui.tracks->setIconSize(QSize(64,ch));

	}
	ppl6::CString Tmp;
	Tmp=item->Artist+" - "+item->Title + " ("+item->Version+")";
	item->setText(columnTitle,Tmp);
	item->setText(columnGenre,item->Genre);

	// Rating
	switch (item->rating) {
		case 0: item->setIcon(columnRating,QIcon(":/bewertung/resources/rating-0.png"));
		item->setText(columnRating,"0");
		break;
		case 1: item->setIcon(columnRating,QIcon(":/bewertung/resources/rating-1.png"));
		item->setText(columnRating,"1");
		break;
		case 2: item->setIcon(columnRating,QIcon(":/bewertung/resources/rating-2.png"));
		item->setText(columnRating,"2");
		break;
		case 3: item->setIcon(columnRating,QIcon(":/bewertung/resources/rating-3.png"));
		item->setText(columnRating,"3");
		break;
		case 4: item->setIcon(columnRating,QIcon(":/bewertung/resources/rating-4.png"));
		item->setText(columnRating,"4");
		break;
		case 5: item->setIcon(columnRating,QIcon(":/bewertung/resources/rating-5.png"));
		item->setText(columnRating,"5");
		break;
		case 6: item->setIcon(columnRating,QIcon(":/bewertung/resources/rating-6.png"));
		item->setText(columnRating,"6");
		break;
	}

    // Bitrate
    Tmp.Setf("%d",(int)(item->bitrate));
    item->setText(columnBitrate,Tmp);

	// Tonträger
	if (item->titleId) {
		const char Seite[]="?ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        Tmp.Setf("%u %c-%03u", item->DeviceId,Seite[(item->DevicePage<10?item->DevicePage:0)],
                item->DeviceTrack);
		item->setText(columnSource,Tmp);
		QIcon Icon;
		Tmp.Setf(":/devices16/resources/tr16x16-%04i.png",item->DeviceType);
		Icon.addFile(Tmp);
		item->setIcon(columnSource,Icon);
	} else {
		item->setText(columnSource,item->File);
	}


	if (playlistView==playlistViewNormal) {
		renderTrackViewPlaylist(item);
	} else if (playlistView==playlistViewDJ) {
		renderTrackViewDJ(item);
	}
}

void Playlist::renderTrackViewPlaylist(PlaylistItem *item)
{
	ppl6::CString Tmp;
	Tmp.Setf("%02i:%02i",(int)(item->trackLength/60),(int)item->trackLength%60);
	item->setText(columnLength,Tmp);
}

void Playlist::renderTrackViewDJ(PlaylistItem *item)
{
	ppl6::CString Tmp;
	item->setText(columnComment,item->Remarks);
	Tmp.Setf("%i",item->bpm);
	item->setText(columnBpm,Tmp);
	Tmp.Setf("%i",(item->bpmPlayed>0?item->bpmPlayed:item->bpm));
	item->setText(columnBpmPlayed,Tmp);
    if (item->keyModification!=0) {
        Tmp=MusicKey(item->musicKey).addSemitone(item->keyModification).name();
        if (item->keyModification>0) Tmp.Concatf(" ▲ +%d",item->keyModification);
        else Tmp.Concatf(" ▼ %d",item->keyModification);
        item->setText(columnMusicKey,Tmp);
    } else {
        item->setText(columnMusicKey,DataTitle::keyName(item->musicKey,musicKeyDisplay));
    }
	QFont f=item->font(columnMusicKey);
	if ((item->keyVerified)) {
        //item->setTextColor(columnMusicKey,QColor(0,0,0));
		f.setBold(true);
        f.setWeight(QFont::Black);
	} else {
        //item->setTextColor(columnMusicKey,QColor(192,192,192));
		f.setBold(false);
        f.setWeight(QFont::ExtraLight);
	}
	item->setFont(columnMusicKey,f);

	Tmp.Setf("%i",item->energyLevel);
	item->setText(columnEnergyLevel,Tmp);

	Tmp.Setf("%02i:%02i",(int)(item->startPositionSec/60),(int)item->startPositionSec%60);
	item->setText(columnStart,Tmp);
	Tmp.Setf("%02i:%02i",(int)(item->endPositionSec/60),(int)item->endPositionSec%60);
	item->setText(columnEnd,Tmp);
	Tmp.Setf("%02i:%02i",(int)(item->mixLength/60),(int)item->mixLength%60);
	item->setText(columnLength,Tmp);

	int cuts=0;
	for (int i=0;i<5;i++) {
		cuts+=(item->cutEndPosition[i]-item->cutStartPosition[i]);
	}
	Tmp.Setf("%02i:%02i",(int)(cuts/60),(int)cuts%60);
	item->setText(columnCuts,Tmp);


    //QColor grey(192,192,192);
    QColor basecolor=QApplication::palette().base().color();
    QBrush grey=item->foreground(columnStart);
    if (basecolor.lightness()<127) {
        grey.setColor(QColor(128,128,128));
    } else {
        grey.setColor(QColor(192,192,192));
    }

    if (item->startPositionSec==0) item->setForeground(columnStart,grey);
    if (item->endPositionSec==item->trackLength) item->setForeground(columnEnd,grey);
    if (cuts==0) item->setForeground(columnCuts,grey);


}

void Playlist::calcMixLength(PlaylistItem *item)
{
	item->updateMixLength();
}

void Playlist::showEvent(QShowEvent * event)
{
	Resize();
	QWidget::showEvent(event);
}
void Playlist::resizeEvent(QResizeEvent * event)
{
	Resize();
	QWidget::resizeEvent(event);
}



void Playlist::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void Playlist::loadPlaylist(ppl6::CString &Filename)
{
    ui.targetTrackGroupBox->setChecked(false);
	ui.tracks->setSortingEnabled(false);
	if (ui.tracks->load(Filename)) {
		PlaylistFileName=Filename;
		updateLastPlaylist();
		updateRecentPlaylistsMenu();
		ui.playlistName->setText(ui.tracks->getName());
		ui.playlistSubName->setText(ui.tracks->getSubName());
		ui.issueNumber->setValue(ui.tracks->getIssueNumber());
		ppl6::CDateTime d=ui.tracks->getIssueDate();
		ui.issueDate->setDate(QDate(d.year(),d.month(),d.day()));
		updatePlaylist();
		setChanged(false);
		ppl6::CString Title=tr("WinMusik Playlist");
		Title+=" - "+Filename;
		this->setWindowTitle(Title);
	}
	ui.tracks->setSortingEnabled(true);
	ui.tracks->sortByColumn(0,Qt::AscendingOrder);

}


void Playlist::editTrack(PlaylistItem *item)
{
	PlaylistEdit edit(this,wm);
	edit.filloutFields(item);
	if (edit.exec()==1) {
		edit.storeFileds(item);
		saveTitle(item);
		setChanged(true);
		renderTrack(item);
		updateLengthStatus();
	}
}

void Playlist::saveTitle(PlaylistItem *item)
{
	if (item->titleId==0) return;
	DataTitle *ti=wm_main->GetTitle(item->titleId);
	if (!ti) return;

	DataTitle Ti, OldTi;
	Ti.CopyFrom(ti);
	OldTi.CopyFrom(ti);

	Ti.SetArtist(item->Artist);
	Ti.SetTitle(item->Title);
	Ti.SetAlbum(item->Album);
	Ti.VersionId=wm_main->VersionStore.FindOrAdd(item->Version);
	Ti.GenreId=wm_main->GenreStore.FindOrAdd(item->Genre);
	Ti.LabelId=wm_main->LabelStore.FindOrAdd(item->Label);
	Ti.Key=item->musicKey;
	Ti.BPM=item->bpm;
	Ti.EnergyLevel=item->energyLevel;
	Ti.Rating=item->rating;
	if (Ti.CoverPreview.Size()==0 || item->CoverPreview.Size()>0) Ti.CoverPreview=item->CoverPreview;

	// Titel speichern
	if (Ti!=OldTi) {
		//printf ("Changed detected, saving...\n");
		if (Ti.TitleId>0) wm_main->Hashes.RemoveTitle(Ti.TitleId);
		if (!wm_main->TitleStore.Put(&Ti)) {
			wm->RaiseError(this,tr("Could not save Title in TitleStore"));
			if (Ti.TitleId>0) wm_main->Hashes.AddTitle(Ti.TitleId);
		}
	}

	if (!wm->SaveID3Tags(ti->DeviceType,ti->DeviceId, ti->Page, ti->Track,Ti)) {
		wm->RaiseError(this,tr("Could not save ID3 Tags"));
	}
}

void Playlist::copyTracks(const QList<QTreeWidgetItem *> items)
{
	QList<QUrl> list;
	ppl6::CString File;
	ppl6::CString xml;
	xml="<winmusikTracklist>\n";
	xml+="<tracks>\n";
	for (int i=0;i<items.size();i++) {
		PlaylistItem *item=(PlaylistItem *)items[i];
		xml+=item->exportAsXML(0);

#ifdef _WIN32
		list.append(QUrl::fromLocalFile(item->File));
#else
		list.append(QUrl::fromLocalFile(item->File));
#endif
		//printf("File: %s\n",(const char*)item->File);
	}
	xml+="</tracks>\n";
	xml+="</winmusikTracklist>\n";
	QClipboard *clipboard = QApplication::clipboard();
	QMimeData *mimeData = new QMimeData;
	mimeData->setText(xml);
	mimeData->setUrls(list);
	clipboard->setMimeData(mimeData);
}

void Playlist::on_menuNew_triggered()
{
	if (saveFirst()!=QMessageBox::Ok) return;
    ui.targetTrackGroupBox->setChecked(false);
	PlaylistFileName.Clear();
	ui.tracks->setName("");
    ui.tracks->setSubName("");
    ui.tracks->setIssueNumber(1);
    ui.tracks->setIssueDate(ppl6::CDateTime::currentTime());

	ui.playlistName->setText("");
    ui.playlistSubName->setText("");
    ui.issueNumber->setValue(1);
    ui.issueDate->setDate(QDate::currentDate());
	ui.tracks->clear();
	updatePlaylist();
	setChanged(false);
	this->setWindowTitle(tr("WinMusik Playlist"));
}

void Playlist::on_menuOpen_triggered()
{
	if (saveFirst()!=QMessageBox::Ok) return;

	ppl6::CString Tmp=QFileDialog::getOpenFileName (this, tr("Load Playlist"), wm->conf.LastPlaylistPath,
				tr("Playlists (*.wmp)"));
	if (Tmp.IsEmpty()) return;
	wm->conf.LastPlaylistPath=ppl6::GetPath(Tmp);
	wm->conf.Save();
	loadPlaylist(Tmp);
}

void Playlist::on_menuOpenRecent0_triggered()
{
	loadPlaylist(wm->conf.LastPlaylists[0]);
}

void Playlist::on_menuOpenRecent1_triggered()
{
	loadPlaylist(wm->conf.LastPlaylists[1]);
}

void Playlist::on_menuOpenRecent2_triggered()
{
	loadPlaylist(wm->conf.LastPlaylists[2]);
}

void Playlist::on_menuOpenRecent3_triggered()
{
	loadPlaylist(wm->conf.LastPlaylists[3]);
}

void Playlist::on_menuOpenRecent4_triggered()
{
	loadPlaylist(wm->conf.LastPlaylists[4]);
}


void Playlist::on_menuSave_triggered()
{
	if (PlaylistFileName.IsEmpty()) {
		on_menuSaveAs_triggered();
		return;
	}
	ui.tracks->setName(ui.playlistName->text());
    ui.tracks->setSubName(ui.playlistSubName->text());
    ui.tracks->setIssueNumber(ui.issueNumber->value());
    ui.tracks->setIssueDate(ppl6::CDateTime(ui.issueDate->date().toString(Qt::ISODate)));

	if (!ui.tracks->save(PlaylistFileName)) return;
	wm->conf.LastPlaylistPath=ppl6::GetPath(PlaylistFileName);
	updateLastPlaylist();
	updateRecentPlaylistsMenu();
	wm->conf.Save();
	setChanged(false);
	ppl6::CString Title=tr("WinMusik Playlist");
	Title+=" - "+PlaylistFileName;
	this->setWindowTitle(Title);
}

void Playlist::on_menuSaveAs_triggered()
{
	ppl6::CString Filename=PlaylistFileName;
	if (Filename.IsEmpty()) Filename=wm->conf.LastPlaylistPath+"/playlist.wmp";

	ppl6::CString Tmp=QFileDialog::getSaveFileName (this, tr("Save WinMusik Playlist"), Filename,
			tr("Playlists (*.wmp)"));
	if (Tmp.IsEmpty()) return;
	PlaylistFileName=Tmp;
	on_menuSave_triggered();
}

static void clearDir(const QString &dirName)
{
    QDir dir(dirName);
    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                clearDir(info.absoluteFilePath());
            }
            else {
                QFile::remove(info.absoluteFilePath());
            }
        }
    }
}

void Playlist::on_menuExport_triggered()
{
    PlaylistExport exportdialog(this,wm);
    if (exportdialog.exec()==1) {
        exportdialog.show();
        exportdialog.start(ui.tracks->topLevelItemCount());
        QCoreApplication::processEvents();
        clearDir(wm->conf.playlist_export.TargetPath);
        ppl6::MkDir(wm->conf.playlist_export.TargetPath);
        //QDir newdir;
        //newdir.mkpath(wm->conf.playlist_export.TargetPath);
        for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
            exportdialog.setTotalProgress(i);
            PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
            if (wm->conf.playlist_export.option_copy_files) {
                exportFile(exportdialog, i+1, item->File);
            }
            QCoreApplication::processEvents();
        }
        if (wm->conf.playlist_export.export_m3u) exportM3U();
        if (wm->conf.playlist_export.export_pls) exportPLS();
        if (wm->conf.playlist_export.export_xspf) exportXSPF();
        if (wm->conf.playlist_export.export_txt) exportTXT();
    }
}

void Playlist::updateLastPlaylist()
{
	ppl6::CString Tmp[WM_NUM_LASTPLAYLISTS];
	for (int i=0;i<WM_NUM_LASTPLAYLISTS;i++) Tmp[i]=wm->conf.LastPlaylists[i];
	wm->conf.LastPlaylists[0]=PlaylistFileName;
	int c=1;
	for (int i=0;i<WM_NUM_LASTPLAYLISTS && c<WM_NUM_LASTPLAYLISTS;i++) {
		if(Tmp[i]!=PlaylistFileName) {
			wm->conf.LastPlaylists[c]=Tmp[i];
			c++;
		}
	}
}

void Playlist::on_viewPlaylist_triggered()
{
	if (playlistView!=playlistViewNormal) {
		playlistView=playlistViewNormal;
		statusbar->setMusicKeySelectionEnabled(false);
		recreatePlaylist();
		wm->conf.playlistView=playlistView;
		wm->conf.Save();
	}
}

void Playlist::on_viewDJ_triggered()
{
	if (playlistView!=playlistViewDJ) {
		playlistView=playlistViewDJ;
		statusbar->setMusicKeySelectionEnabled(true);
		recreatePlaylist();
		wm->conf.playlistView=playlistView;
		wm->conf.Save();
	}
}

void Playlist::on_tracks_itemDoubleClicked (QTreeWidgetItem * item, int column)
{
	Qt::KeyboardModifiers key=QApplication::keyboardModifiers ();
	if (key&Qt::MetaModifier || key&Qt::ControlModifier) {
		editTrack((PlaylistItem*)item);
		return;
	} else if (column==columnComment) {
		currentTreeItem=static_cast<PlaylistItem*>(item);
		on_contextEditComment_triggered();
		return;
    } else if (column==columnSource) {
        currentTreeItem=static_cast<PlaylistItem*>(item);
        //on_contextEditComment_triggered();
        wm->OpenEditor(currentTreeItem->DeviceType,
                       currentTreeItem->DeviceId,
                       currentTreeItem->DevicePage,
                       currentTreeItem->DeviceTrack);
        return;
	} else if (column==columnBpmPlayed) {
		currentTreeItem=static_cast<PlaylistItem*>(item);
		on_contextSetBPMPlayed_triggered();
		return;
	}
	wm->PlayFile(((PlaylistItem*)item)->getExistingFilename());
	return;
}

void Playlist::on_tracks_itemClicked (QTreeWidgetItem * item, int column)
{
	Qt::KeyboardModifiers key=QApplication::keyboardModifiers ();

	PlaylistItem *t=(PlaylistItem*)item;
	if (column==columnMusicKey && playlistView==playlistViewDJ) {
		highlightHarmonicKeys(t);
	} else {
		if (harmonicsHighlighted) unHighlightHarmonicKeys();
		QClipboard *clipboard = QApplication::clipboard();
		ppl6::CString Text;
		if (key&(Qt::AltModifier|Qt::MetaModifier)) {
			Text.Setf("%s %s",(const char*)t->Artist,(const char*)t->Title);
		} else {
			Text.Setf("%s - %s (%s, %0i:%02i min, %s)",(const char*)t->Artist,(const char*)t->Title,
					(const char*)t->Version, t->trackLength/60,t->trackLength%60, (const char*)t->Genre);
			Text.Concatf(" [%s %u %c-%i]",(const char*)wm->GetDeviceNameShort(t->DeviceType),
					t->DeviceId,(t->DevicePage+'A'-1),t->DeviceTrack);
		}
		clipboard->setText(Text,QClipboard::Clipboard);
		clipboard->setText(Text,QClipboard::Selection);
	}
}

void Playlist::on_tracks_customContextMenuRequested ( const QPoint & pos )
{
	QPoint p=ui.tracks->mapToGlobal(pos);
	currentTreeItem=(PlaylistItem*)ui.tracks->itemAt(pos);
	if (!currentTreeItem) return;
	int column=ui.tracks->currentColumn();

    //DataTitle *t=NULL;
    //t=wm->GetTitle(currentTreeItem->titleId);

	QMenu *m=new QMenu(this);
	QAction *a=NULL;

	if (column==columnRating) {
		a=m->addAction (QIcon(":/bewertung/resources/rating-0.png"),"0",this,SLOT(on_contextRate0_clicked()));
		m->addAction (QIcon(":/bewertung/resources/rating-1.png"),"1",this,SLOT(on_contextRate1_clicked()));
		m->addAction (QIcon(":/bewertung/resources/rating-2.png"),"2",this,SLOT(on_contextRate2_clicked()));
		m->addAction (QIcon(":/bewertung/resources/rating-3.png"),"3",this,SLOT(on_contextRate3_clicked()));
		m->addAction (QIcon(":/bewertung/resources/rating-4.png"),"4",this,SLOT(on_contextRate4_clicked()));
		m->addAction (QIcon(":/bewertung/resources/rating-5.png"),"5",this,SLOT(on_contextRate5_clicked()));
		m->addAction (QIcon(":/bewertung/resources/rating-6.png"),"6",this,SLOT(on_contextRate6_clicked()));
	} else if (column==columnMusicKey) {
		if (currentTreeItem->keyVerified==false) a=m->addAction (QIcon(":/icons/resources/musicKeyOk.png"),tr("Music Key is verified","trackList Context Menue"),this,SLOT(on_contextMusicKeyVerified_triggered()));
		else a=m->addAction (QIcon(":/icons/resources/musicKeyNotOk.png"),tr("Music Key is not verified","trackList Context Menue"),this,SLOT(on_contextMusicKeyVerified_triggered()));
		QMenu *mk=m->addMenu ( QIcon(":/icons/resources/musicKey.png"),tr("Set Music-Key","trackList Context Menue") );
		createSetMusicKeyContextMenu(mk);
	} else if (column==columnEnergyLevel) {
		a=m->addAction(tr("Energy Level"));
		createSetEnergyLevelContextMenu(m);
	} else if (column==columnBpmPlayed) {
		a=m->addAction(QIcon(""),tr("Set BPM played","trackList Context Menue"),this,SLOT(on_contextSetBPMPlayed_triggered()));
    } else if (column==columnComment) {
        a=m->addAction(QIcon(""),tr("Edit Comment","trackList Context Menue"),this,SLOT(on_contextEditComment_triggered()));
	} else if (column==columnLength) {
		a=m->addAction(QIcon(""),tr("Reread Traktor IN and OUTs","trackList Context Menue"),this,SLOT(on_contextReReadInAndOuts_triggered()));

	} else if (column==columnCover && QApplication::clipboard()!=NULL && QApplication::clipboard()->pixmap().isNull()==false) {
		a=m->addAction (QIcon(":/icons/resources/copytrack.png"),tr("Paste Cover","trackList Context Menue"),this,SLOT(on_contextPasteCover_triggered()));
	} else {
		a=m->addAction (QIcon(":/icons/resources/edit.png"),tr("Edit Track","trackList Context Menue"),this,SLOT(on_contextEditTrack_triggered()));
		m->addAction (QIcon(":/icons/resources/copytrack.png"),tr("Copy","trackList Context Menue"),this,SLOT(on_contextCopyTrack_triggered()));
		m->addAction (QIcon(":/icons/resources/insert-track.png"),tr("Paste","trackList Context Menue"),this,SLOT(on_contextPasteTrack_triggered()));
		m->addAction (QIcon(":/icons/resources/delete-track.png"),tr("Delete","trackList Context Menue"),this,SLOT(on_contextDeleteTrack_triggered()));
		m->addSeparator();
		m->addAction (QIcon(":/icons/resources/findmore.png"),tr("Find other versions","trackList Context Menue"),this,SLOT(on_contextFindMoreVersions_triggered()));
		m->addAction (QIcon(":/icons/resources/findmore-artist.png"),tr("Find more of artist","trackList Context Menue"),this,SLOT(on_contextFindMoreArtist_triggered()));
		m->addAction (QIcon(":/icons/resources/findmore-title.png"),tr("Find other artists of this title","trackList Context Menue"),this,SLOT(on_contextFindMoreTitle_triggered()));
		m->addSeparator();
		m->addAction (QIcon(":/icons/resources/play.png"),tr("Play Track","trackList Context Menue"),this,SLOT(on_contextPlayTrack_triggered()));
	}
	m->popup(p,a);
}

void Playlist::createSetMusicKeyContextMenu(QMenu *m)
{
	m->addAction(tr("unknown","trackList Context Menue"),this,SLOT(on_contextMusicKey0_triggered()));
	m->addAction(DataTitle::keyName(22,musicKeyDisplay),this,SLOT(on_contextMusicKey22_triggered()));
	m->addAction(DataTitle::keyName(12,musicKeyDisplay),this,SLOT(on_contextMusicKey12_triggered()));
	m->addAction(DataTitle::keyName(5,musicKeyDisplay),this,SLOT(on_contextMusicKey5_triggered()));
	m->addAction(DataTitle::keyName(15,musicKeyDisplay),this,SLOT(on_contextMusicKey15_triggered()));
	m->addAction(DataTitle::keyName(2,musicKeyDisplay),this,SLOT(on_contextMusicKey2_triggered()));
	m->addAction(DataTitle::keyName(19,musicKeyDisplay),this,SLOT(on_contextMusicKey19_triggered()));
	m->addAction(DataTitle::keyName(16,musicKeyDisplay),this,SLOT(on_contextMusicKey16_triggered()));
	m->addAction(DataTitle::keyName(6,musicKeyDisplay),this,SLOT(on_contextMusicKey6_triggered()));
	m->addAction(DataTitle::keyName(23,musicKeyDisplay),this,SLOT(on_contextMusicKey23_triggered()));
	m->addAction(DataTitle::keyName(9,musicKeyDisplay),this,SLOT(on_contextMusicKey9_triggered()));
	m->addAction(DataTitle::keyName(20,musicKeyDisplay),this,SLOT(on_contextMusicKey20_triggered()));
	m->addAction(DataTitle::keyName(10,musicKeyDisplay),this,SLOT(on_contextMusicKey10_triggered()));
	m->addAction(DataTitle::keyName(3,musicKeyDisplay),this,SLOT(on_contextMusicKey3_triggered()));
	m->addAction(DataTitle::keyName(13,musicKeyDisplay),this,SLOT(on_contextMusicKey13_triggered()));
	m->addAction(DataTitle::keyName(24,musicKeyDisplay),this,SLOT(on_contextMusicKey24_triggered()));
	m->addAction(DataTitle::keyName(17,musicKeyDisplay),this,SLOT(on_contextMusicKey17_triggered()));
	m->addAction(DataTitle::keyName(14,musicKeyDisplay),this,SLOT(on_contextMusicKey14_triggered()));
	m->addAction(DataTitle::keyName(4,musicKeyDisplay),this,SLOT(on_contextMusicKey4_triggered()));
	m->addAction(DataTitle::keyName(21,musicKeyDisplay),this,SLOT(on_contextMusicKey21_triggered()));
	m->addAction(DataTitle::keyName(7,musicKeyDisplay),this,SLOT(on_contextMusicKey7_triggered()));
	m->addAction(DataTitle::keyName(18,musicKeyDisplay),this,SLOT(on_contextMusicKey18_triggered()));
	m->addAction(DataTitle::keyName(8,musicKeyDisplay),this,SLOT(on_contextMusicKey8_triggered()));
	m->addAction(DataTitle::keyName(1,musicKeyDisplay),this,SLOT(on_contextMusicKey1_triggered()));
	m->addAction(DataTitle::keyName(11,musicKeyDisplay),this,SLOT(on_contextMusicKey11_triggered()));
	m->addAction(DataTitle::keyName(25,musicKeyDisplay),this,SLOT(on_contextMusicKey25_triggered()));
}

void Playlist::createSetEnergyLevelContextMenu(QMenu *m)
{
	m->addAction(tr("unknown","trackList Context Menue"),this,SLOT(on_contextEnergyLevel0_triggered()));
	m->addAction(tr("1","trackList Context Menue"),this,SLOT(on_contextEnergyLevel1_triggered()));
	m->addAction(tr("2","trackList Context Menue"),this,SLOT(on_contextEnergyLevel2_triggered()));
	m->addAction(tr("3","trackList Context Menue"),this,SLOT(on_contextEnergyLevel3_triggered()));
	m->addAction(tr("4","trackList Context Menue"),this,SLOT(on_contextEnergyLevel4_triggered()));
	m->addAction(tr("5","trackList Context Menue"),this,SLOT(on_contextEnergyLevel5_triggered()));
	m->addAction(tr("6","trackList Context Menue"),this,SLOT(on_contextEnergyLevel6_triggered()));
	m->addAction(tr("7","trackList Context Menue"),this,SLOT(on_contextEnergyLevel7_triggered()));
	m->addAction(tr("8","trackList Context Menue"),this,SLOT(on_contextEnergyLevel8_triggered()));
	m->addAction(tr("9","trackList Context Menue"),this,SLOT(on_contextEnergyLevel9_triggered()));
	m->addAction(tr("10","trackList Context Menue"),this,SLOT(on_contextEnergyLevel10_triggered()));
}

void Playlist::on_contextMusicKeyVerified_triggered()
{
	if (!currentTreeItem) return;
	setChanged(true);
	currentTreeItem->keyVerified=!currentTreeItem->keyVerified;
	DataTitle *t=wm->GetTitle(currentTreeItem->titleId);
	if (t) {
		DataTitle tUpdate=*t;
		if (tUpdate.Flags&16) tUpdate.Flags-=16;
		else tUpdate.Flags|=16;
		currentTreeItem->keyVerified=(tUpdate.Flags&16)>>4;
		if (!wm->TitleStore.Put(&tUpdate)) {
			wm->RaiseError(this,tr("Could not save Title in TitleStore"));
			return;
		}
	}
	renderTrack(currentTreeItem);
	updateLengthStatus();
}

void Playlist::on_contextSetMusicKey(int k)
{
	if (!currentTreeItem) return;
	setChanged(true);
	currentTreeItem->musicKey=k;
	renderTrack(currentTreeItem);

	DataTitle *t=wm->GetTitle(currentTreeItem->titleId);
	if (!t) return;

	DataTitle tUpdate=*t;
	tUpdate.Key=k;

	if (!wm->TitleStore.Put(&tUpdate)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		return;
	}
	if (!wm->SaveID3Tags(t->DeviceType,t->DeviceId, t->Page, t->Track,tUpdate)) {
		wm->RaiseError(this,tr("Could not save ID3 Tags"));
	}
}

void Playlist::on_contextSetEnergyLevel(int v)
{
	if (!currentTreeItem) return;
	setChanged(true);
	currentTreeItem->energyLevel=v;
	renderTrack(currentTreeItem);

	DataTitle *t=wm->GetTitle(currentTreeItem->titleId);
	if (!t) return;

	DataTitle tUpdate=*t;
	tUpdate.EnergyLevel=v;
	if (!wm->TitleStore.Put(&tUpdate)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		return;
	}
}

void Playlist::rateCurrentTrack(int value)
{
	if (!currentTreeItem) return;
	setChanged(true);
	currentTreeItem->rating=value;
	renderTrack(currentTreeItem);

	DataTitle *t=wm->GetTitle(currentTreeItem->titleId);
	if (!t) return;
	if (value==t->Rating) return;

	DataTitle tUpdate=*t;
	tUpdate.Rating=value;
	if (!wm->TitleStore.Put(&tUpdate)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		return;
	}
}

void Playlist::on_contextRate0_clicked()
{
	rateCurrentTrack(0);
}

void Playlist::on_contextRate1_clicked()
{
	rateCurrentTrack(1);
}

void Playlist::on_contextRate2_clicked()
{
	rateCurrentTrack(2);
}

void Playlist::on_contextRate3_clicked()
{
	rateCurrentTrack(3);
}

void Playlist::on_contextRate4_clicked()
{
	rateCurrentTrack(4);
}

void Playlist::on_contextRate5_clicked()
{
	rateCurrentTrack(5);
}

void Playlist::on_contextRate6_clicked()
{
	rateCurrentTrack(6);
}

void Playlist::on_contextPasteCover_triggered()
{
	if (!currentTreeItem) return;
	DataTitle *t=wm->GetTitle(currentTreeItem->titleId);
	if (!t) return;

	QClipboard *clipboard = QApplication::clipboard();
	if (!clipboard) return;
	if (clipboard->pixmap().isNull()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QPixmap Cover=clipboard->pixmap();

	// Cover im File speichern
	if (currentTreeItem->File.NotEmpty()) saveCover(currentTreeItem->File, Cover);
	DataTitle Ti=*t;
	getIconFromCover(Ti.CoverPreview,Cover);
	currentTreeItem->CoverPreview=Ti.CoverPreview;
	QApplication::restoreOverrideCursor();

	// Titel speichern
	if (!wm_main->TitleStore.Put(&Ti)) {
		QApplication::restoreOverrideCursor();
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
	} else {
		this->renderTrack(currentTreeItem);
		QApplication::restoreOverrideCursor();
	}
}


void Playlist::on_contextEditTrack_triggered()
{
	if(!currentTreeItem) return;
	editTrack(currentTreeItem);
}

void Playlist::on_contextCopyTrack_triggered()
{
	if(!currentTreeItem) return;
	QList<QTreeWidgetItem *> items;
	items.push_back(currentTreeItem);
	copyTracks(items);
}

void Playlist::on_contextPasteTrack_triggered()
{

}

void Playlist::on_contextDeleteTrack_triggered()
{
	if(!currentTreeItem) return;
	PlaylistItem *item=(PlaylistItem*)ui.tracks->takeTopLevelItem(ui.tracks->indexOfTopLevelItem(currentTreeItem));
	if (item) delete item;
	setChanged(true);
	renumberTracks();
	updateLengthStatus();
}

void Playlist::on_contextFindMoreVersions_triggered()
{
	if(!currentTreeItem) return;
	searchWindow=wm->OpenOrReuseSearch(searchWindow,currentTreeItem->Artist,currentTreeItem->Title);
}

void Playlist::on_contextFindMoreArtist_triggered()
{
	if(!currentTreeItem) return;
	searchWindow=wm->OpenOrReuseSearch(searchWindow,currentTreeItem->Artist);
}

void Playlist::on_contextFindMoreTitle_triggered()
{
	if(!currentTreeItem) return;
	searchWindow=wm->OpenOrReuseSearch(searchWindow,NULL,currentTreeItem->Title);
}

void Playlist::on_contextPlayTrack_triggered()
{
	if(!currentTreeItem) return;
	wm->PlayFile(((PlaylistItem*)currentTreeItem)->File);
}

void Playlist::on_contextSetBPMPlayed_triggered()
{
	QList<QTreeWidgetItem *>selected=ui.tracks->selectedItems();
	if (selected.count()>0) {
		SetBPMPlayed dialog;
		int bpm=((PlaylistItem*)selected[0])->bpmPlayed;
		if (!bpm) bpm=((PlaylistItem*)selected[0])->bpm;
		dialog.setValue(bpm);
		if (dialog.exec()==1) {
			QList<QTreeWidgetItem *>selected=ui.tracks->selectedItems();
			bpm=dialog.getValue();
			for (int i=0;i<selected.count();i++) {
				PlaylistItem *item=(PlaylistItem*)selected[i];
				item->bpmPlayed=bpm;
				calcMixLength(item);
				renderTrack(item);
			}
			updateLengthStatus();
			setChanged(true);
		}
	}
}

void Playlist::on_contextEditComment_triggered()
{
    if (!currentTreeItem) return;
    EditString dialog;
    dialog.setTitle("Comment: "+currentTreeItem->Artist+": "+currentTreeItem->Title);
    dialog.setString(currentTreeItem->text(columnComment));
    if (dialog.exec()==1) {
        currentTreeItem->setText(columnComment,dialog.getString());
        currentTreeItem->Remarks=dialog.getString();
        setChanged(true);
    }
}


static void updateInAndOut(PlaylistItem *item)
{
	std::list <TraktorTagCue> cuelist;
	std::list <TraktorTagCue>::const_iterator it;
	if (!getTraktorCuesFromFile(cuelist, item->File)) return;
	if (cuelist.size()==0) return;
	float traktorIn=-1;
	float traktorOut=-1;
	for (it=cuelist.begin();it!=cuelist.end();it++) {
		float sec=(float)(it->start/1000.0);
		if (it->type==TraktorTagCue::IN) traktorIn=sec;
		if (it->type==TraktorTagCue::OUT) traktorOut=sec;
	}
	item->startPositionSec=0;
	item->endPositionSec=item->trackLength;

	if (traktorIn>=0.0f) item->startPositionSec=traktorIn;
	if (traktorOut>=0.0f) item->endPositionSec=traktorOut;
}


void Playlist::on_contextReReadInAndOuts_triggered()
{
	QList<QTreeWidgetItem *>selected=ui.tracks->selectedItems();
	if (selected.count()>0) {
		QList<QTreeWidgetItem *>selected=ui.tracks->selectedItems();
		for (int i=0;i<selected.count();i++) {
			PlaylistItem *item=(PlaylistItem*)selected[i];
			updateInAndOut(item);
			calcMixLength(item);
			renderTrack(item);
		}
		updateLengthStatus();
		setChanged(true);
	}
}



void Playlist::on_statusbar_musicKeySelectionChanged(int newValue)
{
	musicKeyDisplay=(MusicKeyType)newValue;
	updatePlaylist();
}

void Playlist::highlightHarmonicKeys(PlaylistItem *track)
{
	if (!track) {
		return;
	}
    std::map<int,HarmonicType> harmonics;
    std::map<int,HarmonicType>::const_iterator it;
	getHarmonicKeys(harmonics,track->musicKey);
    QBrush background;
    background.setStyle(Qt::NoBrush);
	int count=ui.tracks->topLevelItemCount();
    if (count==0) return;
    PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(0);
	for (int i=0;i<count;i++) {
        item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		if (item) {
            item->setBackground(columnMusicKey,background);
			if (item->musicKey!=0 && track->musicKey!=0) {
                if (item->musicKey==track->musicKey) item->setBackground(columnMusicKey,colorscheme.sameKey);
                else {
                    it=harmonics.find(item->musicKey);
                    if (it!=harmonics.end()) {
                        switch(it->second) {
                        case harmonicSemitoneUp:
                        case harmonicTwoSemitoneUp:
                            item->setBackground(columnMusicKey,colorscheme.boostKey);
                            //setItemBackground(item,boostkey);
                            break;
                        case harmonicAvbBoost:
                            item->setBackground(columnMusicKey,colorscheme.boostKey);
                            //setItemBackground(item,boostkey2);
                            break;
                        default:
                            item->setBackground(columnMusicKey,colorscheme.relatedKey);
                            //setItemBackground(item,relatedkey);
                            break;
                        }

                    }
                }
			}
		}
	}
	harmonicsHighlighted=true;
}

void Playlist::unHighlightHarmonicKeys()
{
    QBrush background;
    background.setStyle(Qt::NoBrush);
	int count=ui.tracks->topLevelItemCount();
	for (int i=0;i<count;i++) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		if (item) {
            item->setBackground(columnMusicKey,background);
		}
	}
	harmonicsHighlighted=false;
}

void Playlist::on_searchTriggered()
{
	ui.tracks->unselectItems();
	ppl6::CString Search=statusbar->searchText();
	ppl6::CString TrackWords;
	Search.Trim();
	Search.LCase();
	//printf ("Search triggered: %s\n",(const char*)Search);
	if (Search.IsEmpty()) return;
	ppl6::CArray Words(Search," ");
	int count=0;
	for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		TrackWords=item->Artist+" "+item->Title+" "+item->Version;
		TrackWords.LCase();
		int found=0;
		for (int w=0;w<Words.Num();w++) {
			if (TrackWords.Instr(Words[w])>=0) found++;
		}
		if (found==Words.Num()) {
			if (!count) ui.tracks->setCurrentItem(item);
			item->setSelected(true);
			count++;
		}


	}

}

void Playlist::on_playlistName_textChanged ( const QString & )
{
	setChanged(true);
}

void Playlist::on_playlistSubName_textChanged ( const QString & )
{
	setChanged(true);
}

void Playlist::on_issueNumber_valueChanged ( int  )
{
	setChanged(true);
}

void Playlist::on_issueDate_dateChanged(const QDate &)
{
	setChanged(true);
}

void Playlist::on_shufflePlaylist_triggered()
{
	int ret=QMessageBox::question(this,tr("shuffle playlist"),
			tr("Do you really want to shuffle the tracks of your playlist?"),
			QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel);
	if (ret!=QMessageBox::Yes) return;

	ui.tracks->setSortingEnabled(false);
	currentTreeItem=NULL;
	std::vector<PlaylistItem *> tmpPlaylist;
	tmpPlaylist.reserve(ui.tracks->topLevelItemCount()+1);
	while(ui.tracks->topLevelItemCount()>0) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->takeTopLevelItem(0);
		tmpPlaylist.push_back(item);
	}
	ui.tracks->clear();
	while (tmpPlaylist.size()>0) {
		int index=ppl6::rand(0,tmpPlaylist.size()-1);
		PlaylistItem *item=tmpPlaylist[index];
		ui.tracks->addTopLevelItem(item);
		tmpPlaylist.erase(tmpPlaylist.begin()+index);
	}
	updatePlaylist();
	ui.tracks->setSortingEnabled(true);
	setChanged(true);
	fflush(stdout);
}

ppl6::CString Playlist::getExportFilename(int track, const ppl6::CString &SourceFile)
{
    if (!wm->conf.playlist_export.option_copy_files) {
        return SourceFile;
    }
    ppl6::CString Tmp;
    if (wm->conf.playlist_export.option_prepend_tracknumber) {
        Tmp.Concatf("%03d-", track);
    }
    ppl6::CString Filename=ppl6::GetFilename(SourceFile);
    if (Filename.PregMatch("/^[0-9]{3}-/")) {
        Filename=Filename.Mid(4);
    }
    Tmp+=Filename;
    return Tmp;
}


void Playlist::exportFile(PlaylistExport &dialog, int track, const ppl6::CString &SourceFile)
{
    ppl6::CString Tmp=getExportFilename(track, SourceFile);
    dialog.setCurrentFile(Tmp,0);
    ppl6::CString TargetFile=wm->conf.playlist_export.TargetPath+"/"+Tmp;
    ppl6::CFile::CopyFile(SourceFile, TargetFile);
}

void Playlist::exportM3U()
{
    ppl6::CFile m3u;
    ppl6::CString Tmp;
    if (!m3u.Openf("%s/000index.m3u","wb",(const char*)wm->conf.playlist_export.TargetPath)) return;
    m3u.Puts("#EXTM3U\n");
    for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
        PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
        Tmp=item->Artist + " - " + item->Title;
        Tmp+=" (";
        Tmp+=item->Version;
        Tmp+=")";
        m3u.Putsf("#EXTINF:%u,%s\n",item->trackLength,(const char*)Tmp);
        m3u.Putsf("%s\n",(const char*)getExportFilename(i+1,item->File));
    }
    m3u.Close();
}

void Playlist::exportPLS()
{
    ppl6::CFile pls;
    ppl6::CString Tmp;
    if (!pls.Openf("%s/000index.pls","wb",(const char*)wm->conf.playlist_export.TargetPath)) return;
    pls.Puts("[playlist]\n");
    for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
        PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
        Tmp=item->Artist + " - " + item->Title;
        Tmp+=" (";
        Tmp+=item->Version;
        Tmp+=")";

        pls.Putsf("File%i=%s\n",i+1,(const char*)getExportFilename(i+1,item->File));
        pls.Putsf("Title%i=%s\n",i+1,(const char*)Tmp);
        pls.Putsf("Length%i=%u\n",i+1,item->trackLength);
    }
    pls.Putsf("NumberOfEntries=%i\nVersion=2\n",ui.tracks->topLevelItemCount());
    pls.Close();
}

void Playlist::exportXSPF()
{
    ppl6::CFile xspf;
    ppl6::CString Tmp;
    if (!xspf.Openf("%s/000index.xspf","wb",(const char*)wm->conf.playlist_export.TargetPath)) return;
    xspf.Puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    xspf.Puts("<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">\n");
    Tmp=tr("Playlist created by WinMusik");
    Tmp.Trim();
    Tmp.Concatf(" %s",WM_VERSION);
    xspf.Putsf("<creator>%s</creator>\n",(const char*)Tmp);
    xspf.Puts("<trackList>\n");
    for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
        PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
        ppl6::CString Filename=wm->conf.playlist_export.TargetPath+"/"+getExportFilename(i+1,item->File);
        xspf.Putsf("  <track>\n");
        xspf.Putsf("     <trackNum>%u</trackNum>\n",i+1);
        xspf.Putsf("     <location>file://%s</location>\n",(const char*)ppl6::EscapeHTMLTags(Filename));
        xspf.Putsf("     <creator>%s</creator>\n",(const char*)ppl6::EscapeHTMLTags(item->Artist));
        xspf.Putsf("     <title>%s - %s (%s)</title>\n",
                                (const char*)ppl6::EscapeHTMLTags(item->Artist),
                                (const char*)ppl6::EscapeHTMLTags(item->Title),
                                (const char*)ppl6::EscapeHTMLTags(item->Version));
        xspf.Putsf("     <duration>%u</duration>\n",item->trackLength*1000);  // Bringt VLC zum Absturz
        xspf.Putsf("  </track>\n");
    }
    xspf.Putsf("</trackList>\n</playlist>\n");
    xspf.Close();
}

void Playlist::exportTXT()
{
    ppl6::CFile txt;
    ppl6::CString Tmp;
    ppl6::CString Minuten=tr("min","Minutes in Tracklisting of Playlist");
    if (!txt.Openf("%s/000index.txt","wb",(const char*)wm->conf.playlist_export.TargetPath)) return;
    Tmp=ui.tracks->getName();
    if (Tmp.IsEmpty()) {
        Tmp=tr("Playlist");
    }
    txt.Puts(Tmp);
    txt.Puts("\r\n");

    Tmp=ui.tracks->getSubName();
    if (Tmp.NotEmpty()) {
        txt.Puts(Tmp);
        txt.Puts("\r\n");
    }
    txt.Puts("\r\n");
    for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
        PlaylistItem *item=static_cast<PlaylistItem*>(ui.tracks->topLevelItem(i));
        Tmp=item->Artist + " - " + item->Title;
        Tmp+=" (";
        Tmp+=item->Version;
        Tmp+=")";
        ppl6::CString TmpTxt=Tmp;
        TmpTxt.Chop(1);
        txt.Putsf("%3u. %s, %0i:%02i %s)\r\n",i+1,static_cast<const char*>(TmpTxt),
                  static_cast<int>(item->trackLength/60),item->trackLength%60,
                  static_cast<const char*>(Minuten));

    }
    txt.Puts("\r\n");

}

void Playlist::on_viewFilter_triggered()
{
    if (ui.filterFrame->isVisible()) ui.filterFrame->setVisible(false);
    else ui.filterFrame->setVisible(true);
    filterChanged();
}

bool Playlist::isFilterEnabled() const
{
    if (ui.filterFrame->isVisible()) {
        if (ui.targetTrackGroupBox->isChecked()) {
            return true;
        }
    }
    return false;
}

void Playlist::handleFilterDropEvent(QDropEvent *event)
{
    event->accept();
    const QMimeData *mime=event->mimeData();
    if (!mime) return;
    if (mime->hasFormat("application/winmusik+xml")) {
        QByteArray ba=mime->data("application/winmusik+xml");
        ppl6::CString xml;
        xml.Set(ba.constData(), ba.size());
        if (xml.Left(18)=="<winmusikTracklist") {
            //handleXMLDrop(xml,insertItem);
            //return;
        }
    }
    QList<QUrl>	list=mime->urls();
    if (list.size()==0) return;
    QUrl url=list[0];
    QString file=url.toLocalFile();


    TrackInfo info;
    if (!getTrackInfoFromFile(info,file)) {
        return;
    }


    ui.ct_artist->setText(info.Ti.Artist);
    ui.ct_title->setText(info.Ti.Title);
    ui.ct_version->setText(info.Version);
    ui.ct_bpm->setText(ppl6::ToString("%d",info.Ti.BPM));
    ui.ct_energy->setText(ppl6::ToString("%d",info.Ti.EnergyLevel));
    ppl6::CString Tmp;
    Tmp.Setf("%02i:%02i",static_cast<int>(info.Ti.Length/60),static_cast<int>(info.Ti.Length%60));
    ui.ct_length->setText(Tmp);

    MusicKey key(info.Ti.Key);
    ui.ct_key->setText(key.name(musicKeyDisplay));
    MusicKey modified_key=key.addSemitone(ui.ct_modificationSpinBox->value());
    ui.ct_modifiedKey->setText(modified_key.name(musicKeyDisplay));

    if (info.Cover.Size()>0) {
        QPixmap pix, icon;
        pix.loadFromData(static_cast<const uchar*>(info.Cover.GetPtr()),static_cast<uint>(info.Cover.Size()));
        //icon=pix.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui.ct_cover->setPixmap(pix);
    }

    if (ui.tt_bpmStartSpinBox->value()==0 && info.Ti.BPM>5) ui.tt_bpmStartSpinBox->setValue(static_cast<int>(info.Ti.BPM)-5);
    if (ui.tt_bpmEndSpinBox->value()==0 && info.Ti.BPM>0) ui.tt_bpmEndSpinBox->setValue(static_cast<int>(info.Ti.BPM)+5);

    filterChanged();
}

void Playlist::filterChanged()
{
    MusicKey key(ui.ct_modifiedKey->text());
    ui.tt_sameKey->setText(key.name(musicKeyDisplay));
    ui.tt_nextKey->setText(key.nextKey().name(musicKeyDisplay));
    ui.tt_previousKey->setText(key.previousKey().name(musicKeyDisplay));
    ui.tt_minorMajorSwitch->setText(key.minorMajorSwitch().name(musicKeyDisplay));
    ui.tt_minorMajorJump3->setText(key.minorMajorJump3().name(musicKeyDisplay));
    ui.tt_minorMajorJump1->setText(key.minorMajorJump1().name(musicKeyDisplay));
    ui.tt_boostSemitone1->setText(key.plus1Semitone().name(musicKeyDisplay));
    ui.tt_boostSemitone2->setText(key.plus2Semitone().name(musicKeyDisplay));
    ui.tt_relatedKey->setText(key.avbBoost().name(musicKeyDisplay));

    if (!isFilterEnabled()) {
        for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
            PlaylistItem *item=static_cast<PlaylistItem*>(ui.tracks->topLevelItem(i));
            item->setHidden(false);
        }
        return;
    }
    for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
        PlaylistItem *item=static_cast<PlaylistItem*>(ui.tracks->topLevelItem(i));
        item->setHidden(true);
        if (item->bpm>=ui.tt_bpmStartSpinBox->value() || ui.tt_bpmCheckBox->isChecked()==false) {
            if (item->bpm<=ui.tt_bpmEndSpinBox->value() || ui.tt_bpmCheckBox->isChecked()==false) {

                MusicKey itemKey=MusicKey(item->musicKey).addSemitone(ui.tt_keyModificationSpinBox->value());
                if (itemKey==key && ui.tt_sameKeyCheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.nextKey() && ui.tt_nextKeyCheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.previousKey() && ui.tt_previousKeyCheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.minorMajorSwitch() && ui.tt_minorMajorSwitchCheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.minorMajorJump3() && ui.tt_minorMajorJump3CheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.minorMajorJump1() && ui.tt_minorMajorJump1CheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.plus1Semitone() && ui.tt_boostSemitone1CheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.plus2Semitone() && ui.tt_boostSemitone2CheckBox->isChecked()) item->setHidden(false);
                else if (itemKey==key.avbBoost() && ui.tt_relatedKeyCheckBox->isChecked()) item->setHidden(false);
            }
        }
    }
}

void Playlist::on_ct_modificationSpinBox_valueChanged(int i)
{
    MusicKey key(ui.ct_key->text());
    MusicKey modified_key=key.addSemitone(i);
    ui.ct_modifiedKey->setText(modified_key.name(musicKeyDisplay));
    filterChanged();
}

