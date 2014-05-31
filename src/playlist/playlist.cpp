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
#include <QToolBar>
#include <QKeyEvent>
#include <QDomDocument>
#include <QBuffer>

#include "playlisttracks.h"
#include "playlist.h"
#include "playlistedit.h"
#include "traktor.h"
#include <stdio.h>


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
	this->setStatusBar(NULL);
	this->wm=wm;
	currentTreeItem=NULL;
	searchWindow=NULL;
	saveWidget=saveAsWidget=NULL;
	setAttribute(Qt::WA_DeleteOnClose,true);
	ui.tracks->setPlaylist(this);
	ui.tracks->setAcceptDrops(true);
	this->setWindowTitle(tr("WinMusik Playlist"));

	ui.tracks->installEventFilter(this);
	ui.playlistName->installEventFilter(this);

	createMenue();
	createToolbar();
	createStatusBar();

	playlistView=(playlistViewType)wm->conf.playlistView;
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

	menuRecentPlaylists=menu->addMenu(QIcon(":/icons/resources/fileopen.png"),tr("&recent Playlists"));
	updateRecentPlaylistsMenu();

	menu=menuBar()->addMenu(tr("&View"));
	menu->addAction(QIcon(":/icons/resources/view_playlist.png"),tr("&Playlist"),this,SLOT(on_viewPlaylist_triggered()));
	menu->addAction(QIcon(":/icons/resources/view_dj.png"),tr("&DJ"),this,SLOT(on_viewDJ_triggered()));

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
	tb->addAction(QIcon(":/icons/resources/view_playlist.png"),tr("view &Playlist"),this,SLOT(on_viewPlaylist_triggered()));
	tb->addAction(QIcon(":/icons/resources/view_dj.png"),tr("view &DJ"),this,SLOT(on_viewDJ_triggered()));

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
			currentTreeItem=(PlaylistItem*)ui.tracks->currentItem();
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
				editTrack(currentTreeItem);
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

bool Playlist::on_tracks_MouseMove(QMouseEvent *event)
{
	return false;
	//printf ("on_tracks_MouseMove\n");
	if (!(event->buttons() == Qt::LeftButton)) return false;

	int distance=(event->pos()-startPos).manhattanLength();
	if (distance<QApplication::startDragDistance()) return false;

	QList<QTreeWidgetItem*> Items=ui.tracks->selectedItems();
	QList<QUrl> list;
    QPixmap Icon;

    //printf ("DragStart\n");

    ppl6::CString xml;
    xml="<winmusikTracklist>\n";
    xml+="<tracks>\n";
	for (int i=0;i<Items.size();i++) {
		PlaylistItem *item=(PlaylistItem *)Items[i];
		if (Icon.isNull()) {
			Icon.loadFromData((const uchar*)item->CoverPreview.GetPtr(),item->CoverPreview.GetSize());
		}
		xml+=item->exportAsXML(0);

#ifdef _WIN32
			list.append(QUrl::fromLocalFile(item->File));
#else
		list.append(QUrl::fromLocalFile(item->File));
#endif
	}
	xml+="</tracks>\n";
	xml+="</winmusikTracklist>\n";

	xml.Print(true);

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    if (Icon.isNull()) Icon.load(":/devices48/resources/tr48x48-0007.png");
    drag->setPixmap(Icon);
    mimeData->setText(xml);
    mimeData->setUrls(list);
    drag->setMimeData(mimeData);
    // start drag
    drag->exec(Qt::CopyAction | Qt::MoveAction);
	startPos.setX(0);
	startPos.setY(0);
	return true;
}

void Playlist::on_tracks_itemSelectionChanged ()
{
	updateLengthStatus();
}

void Playlist::handleDropEvent(QDropEvent *event)
{
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
	//ui.tracks->unselectItems();
	QApplication::processEvents();

	ui.tracks->setSortingEnabled(false);
	ppl6::CString xml=mime->text();
	if (xml.Left(18)=="<winmusikTracklist") {
		handleXMLDrop(xml,insertItem);
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
//	printf ("Playlist::handleURLDrop\n");

	for (int i=0;i<list.size();i++) {
		QUrl url=list[i];
		QString file=url.toLocalFile();
		PlaylistItem *item=new PlaylistItem;
		item->File=file;
		ppluint32 titleId=findTitleIdByFilename(file);
		if (titleId) loadTrackFromDatabase(item,titleId);
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


	if (playlistView==playlistViewNormal) {
		ui.tracks->setColumnWidth(columnTitle,w*80/100);
		ui.tracks->setColumnWidth(columnGenre,w*15/100);

	} else if (playlistView==playlistViewDJ) {
		ui.tracks->setColumnWidth(columnBpm,35);
		w-=39;
		ui.tracks->setColumnWidth(columnBpmPlayed,45);
		w-=49;
		ui.tracks->setColumnWidth(columnMusicKey,50);
		w-=54;
		ui.tracks->setColumnWidth(columnEnergyLevel,30);
		w-=34;
		ui.tracks->setColumnWidth(columnStart,60);
		w-=64;
		ui.tracks->setColumnWidth(columnEnd,60);
		w-=64;
		ui.tracks->setColumnWidth(columnCuts,60);
		w-=64;
		ui.tracks->setColumnWidth(columnTotalLength,60);
		w-=64;

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
	columnSource=15;

	if (playlistView==playlistViewNormal) {
		ui.tracks->setColumnCount(7);
		columnLength=4;
		columnRating=5;
		columnSource=6;

		item->setText(columnTrack,tr("Track"));
		item->setText(columnCover,tr("Cover"));
		item->setText(columnTitle,tr("Artist - Title (Version)"));
		item->setText(columnGenre,tr("Genre"));
		item->setText(columnLength,tr("Length"));
		item->setText(columnRating,tr("Rating"));
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
	int trackLength=0;
	int mixLength=0;
	int selectedLength=0;
	int selectedMixLength=0;
	int selectedTracks=0;
	for (int i=0;i<ui.tracks->topLevelItemCount();i++) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		trackLength+=item->trackLength;
		mixLength+=item->mixLength;
		if (playlistView==playlistViewDJ) {
			Tmp.Setf("%02i:%02i",(int)(mixLength/60),(int)mixLength%60);
			item->setText(columnTotalLength,Tmp);
		}
		if (item->isSelected()) {
			selectedTracks++;
			selectedLength+=item->trackLength;
			selectedMixLength+=item->mixLength;
		}
	}
	statusbar->setTotalLength(trackLength);
	statusbar->setMixLength(mixLength);
	statusbar->setTotalTracks(ui.tracks->topLevelItemCount());

	statusbar->setSelectedLength(selectedLength);
	statusbar->setSelectedMixLength(selectedMixLength);
	statusbar->setSelectedTracks(selectedTracks);
}

void Playlist::renderTrack(PlaylistItem *item)
{
	QColor color(0,0,0);
	QBrush b=item->background(0);
	setItemBackground(item,b);
	for (int i=1;i<item->columnCount();i++) {
		item->setText(i,"");
		item->setIcon(i,QIcon());
		item->setTextColor(i,color);
	}
	QFont f=item->font(columnMusicKey);
	f.setBold(false);
	item->setFont(columnMusicKey,f);


	if (item->CoverPreview.Size()>0) {
		QPixmap pix, icon;
		pix.loadFromData((const uchar*)item->CoverPreview.GetPtr(),item->CoverPreview.Size());
		icon=pix.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation);
		item->setIcon(columnCover,icon.copy(0,0,64,16));
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

	// Tonträger
	if (item->titleId) {
		const char Seite[]="?ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		Tmp.Setf("%u %c-%03u", item->DeviceId,Seite[(item->DevicePage<10?item->DevicePage:0)],item->DeviceTrack);
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
	item->setText(columnMusicKey,DataTitle::keyName(item->musicKey,musicKeyDisplay));
	QFont f=item->font(columnMusicKey);
	if ((item->keyVerified)) {
		item->setTextColor(columnMusicKey,QColor(0,0,0));
		f.setBold(true);
	} else {
		item->setTextColor(columnMusicKey,QColor(192,192,192));
		f.setBold(false);
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

	QColor grey(192,192,192);

	if (item->startPositionSec==0) item->setTextColor(columnStart,grey);
	if (item->endPositionSec==item->trackLength) item->setTextColor(columnEnd,grey);
	if (cuts==0) item->setTextColor(columnCuts,grey);


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
	ui.tracks->setSortingEnabled(false);
	if (ui.tracks->load(Filename)) {
		PlaylistFileName=Filename;
		updateLastPlaylist();
		updateRecentPlaylistsMenu();
		ui.playlistName->setText(ui.tracks->getName());
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

	DataTitle Ti;
	Ti.CopyFrom(ti);

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
	if (Ti.TitleId>0) wm_main->Hashes.RemoveTitle(Ti.TitleId);
	if (!wm_main->TitleStore.Put(&Ti)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		if (Ti.TitleId>0) wm_main->Hashes.AddTitle(Ti.TitleId);
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
	PlaylistFileName.Clear();
	ui.tracks->setName("");
	ui.playlistName->setText("");
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

void Playlist::on_tracks_itemDoubleClicked (QTreeWidgetItem * item, int )
{
	Qt::KeyboardModifiers key=QApplication::keyboardModifiers ();
	if (key&Qt::MetaModifier) {
		editTrack((PlaylistItem*)item);
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
		QClipboard *clipboard = QApplication::clipboard();
		ppl6::CString Text;
		if (key&Qt::MetaModifier) {
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

	DataTitle *t=NULL;
	t=wm->GetTitle(currentTreeItem->titleId);

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

void Playlist::on_statusbar_musicKeySelectionChanged(int newValue)
{
	musicKeyDisplay=(MusicKeyType)newValue;
	updatePlaylist();
}


void Playlist::setItemBackgroundColor(PlaylistItem *item, const QColor &c)
{
	item->setBackgroundColor(columnTitle,c);
	item->setBackgroundColor(columnMusicKey,c);
}

void Playlist::setItemBackground(PlaylistItem *item, const QBrush &b)
{
	item->setBackground(columnTitle,b);
	item->setBackground(columnMusicKey,b);
}


void Playlist::highlightHarmonicKeys(PlaylistItem *track)
{
	if (!track) {
		return;
	}
	std::set<int> harmonics;
	std::set<int>::const_iterator it;
	getHarmonicKeys(harmonics,track->musicKey);

	int count=ui.tracks->topLevelItemCount();
	for (int i=0;i<count;i++) {
		PlaylistItem *item=(PlaylistItem*)ui.tracks->topLevelItem(i);
		if (item) {
			QBrush b=item->background(columnTrack);
			setItemBackground(item,b);

			if (item->musicKey!=0 && track->musicKey!=0) {
				if (item->musicKey==track->musicKey) setItemBackgroundColor(item,QColor(170,255,170,255));
				else {
					it=harmonics.find(item->musicKey);
					if (it!=harmonics.end()) setItemBackgroundColor(item,QColor(218,255,192,255));
				}
			}


		}
	}

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
