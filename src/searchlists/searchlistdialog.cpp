/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: SearchlistDialog.cpp,v 1.1 2010/11/14 13:20:11 pafe Exp $
 *
 *
 * Copyright (c) 2012 Patrick Fedick
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
#include <QMimeData>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStatusBar>
#include <QMessageBox>
#include "searchlistdialog.h"
#include "searchlisttrackdialog.h"
#include "csearchlist.h"

SearchlistDialog::SearchlistDialog(QWidget *parent, CWmClient *wm, const ppl6::CString &Filename)
    : QWidget(parent)
{
	dupeCheckThread=new DupeCheckThread (this);
	statusbar=NULL;
	ui.setupUi(this);
	this->wm=wm;
	List.setWmClient(wm);
	ui.trackList->installEventFilter(this);
	currentTrackListItem=NULL;
	searchWindow=NULL;
	haveCopyItem=false;
	ClipBoardTimer.setParent(this);
	connect(&ClipBoardTimer, SIGNAL(timeout()), this, SLOT(on_ClipBoardTimer_update()));

	setAttribute(Qt::WA_DeleteOnClose,true);
	QString Style="QTreeView::item {\n"
	    		"border-right: 1px solid #b9b9b9;\n"
	    		"border-bottom: 1px solid #b9b9b9;\n"
	    		"}\n"
	    		"QTreeView::item:selected {\n"
	    		//"border-top: 1px solid #80c080;\n"
	    		//"border-bottom: 1px solid #80c080;\n"
	    		"background: #000070;\n"
	    		"color: rgb(255, 255, 255);\n"
	    		"}\n"
	    		"";
	ui.trackList->setStyleSheet(Style);


	// Suchliste laden
	this->Filename=Filename;
	if (!List.load(Filename)) {
		// Hmm, es könnte eine neue Liste sein
		//return;
	}
	ui.searchlistName->setText(List.name());
	SearchlistTreeItem *item;
	ui.trackList->setSortingEnabled(false);
	for (size_t i=0;i<List.size();i++) {
		item=new SearchlistTreeItem;
		item->Track=List[i];
		item->dupePresumption=0;
		renderTrack(item);
		ui.trackList->addTopLevelItem(item);
	}
	ui.trackList->setSortingEnabled(true);
	ui.trackList->sortItems(SL_COLUMN_ARTIST,Qt::AscendingOrder);

	ui.trackList->setDragEnabled(true);
	ui.trackList->setDropIndicatorShown(true);
	//ui.trackList->viewport()->setAcceptDrops(true);
	ui.trackList->setAcceptDrops(true);
	ui.trackList->setDragDropMode(QAbstractItemView::DragDrop);

	setupStatusBar();

	this->show();
	this->restoreGeometry(wm_main->GetGeometry("SearchlistDialog"));


	// Das müssen wir irgendwie asynchron hinbekommen
	/*
	for (int i=0;i<ui.trackList->topLevelItemCount();i++) {
		item=(SearchlistTreeItem*)ui.trackList->topLevelItem(i);
		dupeCheckOnTrack(item);
	}
	*/

	dupeCheckThread->setTracklist(ui.trackList);
	connect(dupeCheckThread, SIGNAL(updateItem(QTreeWidgetItem *, int)), this, SLOT(updateDupeCheckItem(QTreeWidgetItem *, int)));
	dupeCheckThread->start();
	ClipBoardTimer.start(200);
}

SearchlistDialog::~SearchlistDialog()
{
	dupeCheckThread->stopThread();
	if (wm) {
		wm->SearchlistDialogClosed(this);
	}
}


void SearchlistDialog::setupStatusBar()
{
	statusbar=new QStatusBar;
	ui.searchlistLayout->addWidget(statusbar);

	QLabel *label=new QLabel(tr("Tracks:"));
	statusbar->addWidget(label);

	statusbarTrackNumber=new QLabel("0");
	statusbarTrackNumber->setFrameStyle(QFrame::Panel|QFrame::Sunken);
	statusbar->addWidget(statusbarTrackNumber);

	label=new QLabel(tr("Selected:"));
	statusbar->addWidget(label);

	statusbarTracksSelected=new QLabel("0");
	statusbarTracksSelected->setFrameStyle(QFrame::Panel|QFrame::Sunken);
	statusbar->addWidget(statusbarTracksSelected);

	label=new QLabel(tr("Pre-selected:"));
	statusbar->addWidget(label);

	statusbarTracksPreSelected=new QLabel("0");
	statusbarTracksPreSelected->setFrameStyle(QFrame::Panel|QFrame::Sunken);
	statusbar->addWidget(statusbarTracksPreSelected);

	label=new QLabel(tr("Done:"));
	statusbar->addWidget(label);

	statusbarTracksDone=new QLabel("0");
	statusbarTracksDone->setFrameStyle(QFrame::Panel|QFrame::Sunken);
	statusbar->addWidget(statusbarTracksDone);

	updateStatusBar();
}

void SearchlistDialog::updateStatusBar()
{
	ppl6::CString Tmp;
	Tmp.Setf("%i",ui.trackList->topLevelItemCount());
	statusbarTrackNumber->setText(Tmp);
	QList<QTreeWidgetItem *> list=ui.trackList->selectedItems();
	Tmp.Setf("%i",list.size());
	statusbarTracksSelected->setText(Tmp);

	int selected=0;
	int done=0;
	SearchlistTreeItem *item;
	for (int i=0;i<ui.trackList->topLevelItemCount();i++) {
		item=(SearchlistTreeItem*)ui.trackList->topLevelItem(i);
		if (item) {
			if (item->Track.selected==true) selected++;
			if (item->Track.found==true) done++;
		}
	}
	Tmp.Setf("%i",selected);
	statusbarTracksPreSelected->setText(Tmp);

	Tmp.Setf("%i",done);
	statusbarTracksDone->setText(Tmp);
}

bool SearchlistDialog::eventFilter(QObject *target, QEvent *event)
{
	if (target==ui.trackList) {
		if (event->type()==QEvent::KeyPress) {
			QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
			int key=keyEvent->key();
			int modifier=keyEvent->modifiers();
			SearchlistTreeItem *item=(SearchlistTreeItem*)ui.trackList->currentItem();
			if (key==Qt::Key_Delete && modifier==Qt::NoModifier) {
				if (item) on_deleteTrackButton_clicked();
				return true;
			} else if (key==Qt::Key_Return && modifier==Qt::NoModifier) {
				if (item) editTrack(item);
				return true;
			} else if (key==Qt::Key_C && modifier==Qt::ControlModifier) {
				if (item) {
					copyItem=item->Track;
					haveCopyItem=true;
				}
				return true;
			} else if (key==Qt::Key_V && modifier==Qt::ControlModifier) {
				if (haveCopyItem) addTrack(copyItem);
				return true;
			} else if (key==Qt::Key_N && modifier==Qt::ControlModifier) {
				on_newTrackButton_clicked();
				return true;
			} else if (modifier==Qt::NoModifier && key==Qt::Key_F4) {
				if (item) {
					searchWindow=wm->OpenOrReuseSearch(searchWindow,item->Track.Artist,item->Track.Title);
					this->setFocus();
				}
				return true;
			}
		}
	}
	return QWidget::eventFilter(target,event);
}


void SearchlistDialog::dupeCheckOnTrack(SearchlistTreeItem *item)
{
	int dupePresumption=dupeCheckThread->dupeCheckOnTrack(item);
	ppl6::CString Tmp;
	Tmp.Setf("%3i %%",dupePresumption);
	item->setText(SL_COLUMN_EXISTING,Tmp);
}

void SearchlistDialog::renderTrack(SearchlistTreeItem *item)
{
	ppl6::CString Tmp;
	Tmp=item->Track.Artist+" - "+item->Track.Title;
	item->setText(SL_COLUMN_ARTIST,Tmp);
	item->setText(SL_COLUMN_VERSION,item->Track.Version);
	item->setText(SL_COLUMN_GENRE,item->Track.Genre);
	item->setTextAlignment(SL_COLUMN_LENGTH,Qt::AlignRight);
	if (item->Track.Length>0) {
		Tmp.Setf("%i:%02i",item->Track.Length/60,item->Track.Length%60);
		item->setText(SL_COLUMN_LENGTH,Tmp);
	}
	item->setText(SL_COLUMN_DATEADDED,item->Track.DateAdded.get("%Y-%m-%d"));
	item->setText(SL_COLUMN_DATERELEASE,item->Track.ReleaseDate.get("%Y-%m-%d"));

	if (item->Track.found==true) {
		item->setIcon(SL_COLUMN_DONE,QIcon(":/icons/resources/button_ok.png"));
		item->setText(SL_COLUMN_DONE,tr("yes"));
	} else {
		item->setIcon(SL_COLUMN_DONE,QIcon(":/icons/resources/edit-find.png"));
		item->setText(SL_COLUMN_DONE,tr("no"));
	}

	if (item->Track.selected==true) {
		item->setIcon(SL_COLUMN_SELECTED,QIcon(":/icons/resources/button_ok.png"));
		item->setText(SL_COLUMN_SELECTED,tr("yes"));
	} else {
		item->setIcon(SL_COLUMN_SELECTED,QIcon(":/icons/resources/edit-delete.png"));
		item->setText(SL_COLUMN_SELECTED,tr("no"));
	}

	// Rating
	switch (item->Track.Rating) {
		case 0: item->setIcon(SL_COLUMN_RATING,QIcon(":/bewertung/resources/rating-0.png"));
			item->setText(SL_COLUMN_RATING,"0");
			break;
		case 1: item->setIcon(SL_COLUMN_RATING,QIcon(":/bewertung/resources/rating-1.png"));
			item->setText(SL_COLUMN_RATING,"1");
			break;
		case 2: item->setIcon(SL_COLUMN_RATING,QIcon(":/bewertung/resources/rating-2.png"));
			item->setText(SL_COLUMN_RATING,"2");
			break;
		case 3: item->setIcon(SL_COLUMN_RATING,QIcon(":/bewertung/resources/rating-3.png"));
			item->setText(SL_COLUMN_RATING,"3");
			break;
		case 4: item->setIcon(SL_COLUMN_RATING,QIcon(":/bewertung/resources/rating-4.png"));
			item->setText(SL_COLUMN_RATING,"4");
			break;
		case 5: item->setIcon(SL_COLUMN_RATING,QIcon(":/bewertung/resources/rating-5.png"));
			item->setText(SL_COLUMN_RATING,"5");
			break;
		case 6: item->setIcon(SL_COLUMN_RATING,QIcon(":/bewertung/resources/rating-6.png"));
			item->setText(SL_COLUMN_RATING,"6");
			break;
	}


}

void SearchlistDialog::Resize()
{
	int s=ui.trackList->width();
	ui.trackList->setColumnWidth(SL_COLUMN_DONE,60);
	ui.trackList->setColumnWidth(SL_COLUMN_SELECTED,60);
	ui.trackList->setColumnWidth(SL_COLUMN_EXISTING,60);
	ui.trackList->setColumnWidth(SL_COLUMN_DATEADDED,90);
	ui.trackList->setColumnWidth(SL_COLUMN_LENGTH,70);
	ui.trackList->setColumnWidth(SL_COLUMN_GENRE,100);

	ui.trackList->setColumnWidth(SL_COLUMN_RATING,64);

	s=s-62-62-92-72-108-66-62;
	if (s<300) s=300;
	ui.trackList->setColumnWidth(SL_COLUMN_VERSION,s*30/100);
	ui.trackList->setColumnWidth(SL_COLUMN_ARTIST,s*70/100);
}

void SearchlistDialog::showEvent(QShowEvent * event)
{
	Resize();
	QWidget::showEvent(event);
}
void SearchlistDialog::resizeEvent(QResizeEvent * event)
{
	Resize();
	QWidget::resizeEvent(event);
}

void SearchlistDialog::closeEvent(QCloseEvent *event)
{
	wm_main->SaveGeometry("SearchlistDialog",this->saveGeometry());
	QWidget::closeEvent(event);
}


void SearchlistDialog::ReloadTranslation()
{
	ui.retranslateUi(this);
}


void SearchlistDialog::on_trackList_customContextMenuRequested ( const QPoint & pos )
{
    QPoint p=ui.trackList->mapToGlobal(pos);
    currentTrackListItem=(SearchlistTreeItem*)ui.trackList->itemAt(pos);
    if (!currentTrackListItem) return;
    //printf ("Custom Context %i\n",currentTrackListItem->Track);
    int column=ui.trackList->currentColumn();

    QMenu *m=new QMenu(this);
    QAction *a=NULL;
    if (column==SL_COLUMN_RATING) {
    	a=m->addAction (QIcon(":/bewertung/resources/rating-0.png"),"0",this,SLOT(on_contextRate0_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-1.png"),"1",this,SLOT(on_contextRate1_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-2.png"),"2",this,SLOT(on_contextRate2_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-3.png"),"3",this,SLOT(on_contextRate3_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-4.png"),"4",this,SLOT(on_contextRate4_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-5.png"),"5",this,SLOT(on_contextRate5_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-6.png"),"6",this,SLOT(on_contextRate6_clicked()));
    } else {
    	a=m->addAction (QIcon(":/icons/resources/edit.png"),tr("Edit Track","trackList Context Menue"),this,SLOT(on_contextEditTrack_triggered()), tr("Return"));
    	m->addAction (QIcon(":/icons/resources/delete-track.png"),tr("Delete Track","trackList Context Menue"),this,SLOT(on_contextDeleteTrack_triggered()), tr("Del"));
    	m->addAction (QIcon(":/icons/resources/insert-track.png"),tr("Insert Track","trackList Context Menue"),this,SLOT(on_contextInsertTrack_triggered()), tr("Ctrl+N"));
    	m->addSeparator();
    	m->addAction (QIcon(":/icons/resources/findmore.png"),tr("Search track in Database","trackList Context Menue"),this,SLOT(on_contextFind_triggered()), tr("F4"));
    }
    /*
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
    */
    m->popup(p,a);

}

void SearchlistDialog::on_trackList_itemClicked ( QTreeWidgetItem * item, int column )
{
	if (!item) return;
	if (column<SL_COLUMN_SELECTED) {
		SearchlistItem it=((SearchlistTreeItem*)item)->Track;
		QClipboard *clipboard = QApplication::clipboard();
		ppl6::CString Text;
		Text=it.Artist+" "+it.Title;
		LastClipboardString=Text;		// Verhindern, dass eine Suche ausgelöst wird
		clipboard->setText(Text,QClipboard::Clipboard);
		clipboard->setText(Text,QClipboard::Selection);

	} else if (column==SL_COLUMN_DONE) {
		SearchlistItem it=((SearchlistTreeItem*)item)->Track;
		if (it.found==true) it.found=false;
		else it.found=true;
		((SearchlistTreeItem*)item)->Track=it;
		renderTrack((SearchlistTreeItem*)item);
		QCoreApplication::processEvents();
		dupeCheckOnTrack((SearchlistTreeItem*)item);
		save();
		updateStatusBar();
	} else if (column==SL_COLUMN_SELECTED) {
		SearchlistItem it=((SearchlistTreeItem*)item)->Track;
		if (it.selected==true) it.selected=false;
		else it.selected=true;
		((SearchlistTreeItem*)item)->Track=it;
		renderTrack((SearchlistTreeItem*)item);
		QCoreApplication::processEvents();
		dupeCheckOnTrack((SearchlistTreeItem*)item);
		save();
		updateStatusBar();
	}
}

void SearchlistDialog::on_trackList_itemDoubleClicked ( QTreeWidgetItem * item, int column)
{
	if (!item) return;
	if (column<SL_COLUMN_EXISTING) {
		editTrack((SearchlistTreeItem*)item);
	} else if (column==SL_COLUMN_EXISTING) {
		currentTrackListItem=(SearchlistTreeItem*)item;
		on_contextFind_triggered();
	}

}

void SearchlistDialog::on_trackList_itemSelectionChanged()
{
	updateStatusBar();
}

void SearchlistDialog::on_trackList_itemDropped(SearchlistItem *item)
{
	SearchlistTreeItem *insert=new SearchlistTreeItem();
	insert->Track=(*item);
	ui.trackList->addTopLevelItem(insert);
	renderTrack((SearchlistTreeItem*)insert);
	dupeCheckOnTrack(insert);
	delete (item);
}

void SearchlistDialog::on_trackList_changed()
{
	updateStatusBar();
	save();
}



void SearchlistDialog::rateCurrentTrack(int value)
{
	if (!currentTrackListItem) return;
	if (value!=currentTrackListItem->Track.Rating) {
		currentTrackListItem->Track.Rating=value;
		renderTrack(currentTrackListItem);
		dupeCheckOnTrack(currentTrackListItem);
		save();
	}
}

void SearchlistDialog::on_contextRate0_clicked()
{
	rateCurrentTrack(0);
}

void SearchlistDialog::on_contextRate1_clicked()
{
	rateCurrentTrack(1);
}

void SearchlistDialog::on_contextRate2_clicked()
{
	rateCurrentTrack(2);
}

void SearchlistDialog::on_contextRate3_clicked()
{
	rateCurrentTrack(3);
}

void SearchlistDialog::on_contextRate4_clicked()
{
	rateCurrentTrack(4);
}

void SearchlistDialog::on_contextRate5_clicked()
{
	rateCurrentTrack(5);
}

void SearchlistDialog::on_contextRate6_clicked()
{
	rateCurrentTrack(6);
}


void SearchlistDialog::on_contextEditTrack_triggered()
{
	if (!currentTrackListItem) return;
	editTrack(currentTrackListItem);
}

void SearchlistDialog::editTrack(SearchlistTreeItem *item)
{
	if (!item) return;
	SearchlistTrackDialog dialog;
	dialog.set(item->Track);
	int ret=dialog.exec();
	if (ret==1) {
		item->Track=dialog.get();
		renderTrack(item);
		dupeCheckOnTrack(item);
		save();
	}
}

void SearchlistDialog::on_contextInsertTrack_triggered()
{
	on_newTrackButton_clicked();
}

void SearchlistDialog::addTrack(const SearchlistItem &track)
{
	SearchlistTreeItem *item=new SearchlistTreeItem;
	item->Track=track;
	item->dupePresumption=0;
	renderTrack(item);
	dupeCheckOnTrack(item);
	dupeCheckThread->wait();
	ui.trackList->addTopLevelItem(item);
	save();
	updateStatusBar();
}

void SearchlistDialog::on_newTrackFromClipboardButton_clicked()
{
	SearchlistTrackDialog dialog;
	dialog.setFromClipboard();
	int ret=dialog.exec();
	if (ret==1) {
		addTrack(dialog.get());
	}
}

void SearchlistDialog::on_contextDeleteTrack_triggered()
{
	on_deleteTrackButton_clicked();
}

void SearchlistDialog::on_deleteTrackButton_clicked()
{
	dupeCheckThread->wait();
	QList<QTreeWidgetItem *> list=ui.trackList->selectedItems();
	if (list.size()>1) {
		int ret = QMessageBox::question(this, tr("delete tracks"),
		                                tr("You have selected more than one track.\n"
		                                   "Are you sure, you want to delete them?"),
		                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (ret!=QMessageBox::Yes) return;
	}
	for (int i=0;i<list.size();i++) {
		SearchlistTreeItem *item=(SearchlistTreeItem*)list[i];
		int index=ui.trackList->indexOfTopLevelItem (item);
		if (index>=0) {
			item=(SearchlistTreeItem*) ui.trackList->takeTopLevelItem(index);
			if (item) delete item;
		}
	}
	save();
	updateStatusBar();
}

void SearchlistDialog::on_saveExitButton_clicked()
{
	close();
}

void SearchlistDialog::on_saveButton_clicked()
{
	save();
}

int SearchlistDialog::save()
{
	if (Filename.IsEmpty()) {
		// Neue Suchliste
	}
	SearchlistTreeItem *item;
	List.clear();
	for (int i=0;i<ui.trackList->topLevelItemCount();i++) {
		item=(SearchlistTreeItem*)ui.trackList->topLevelItem(i);
		if (item) List.add(item->Track);
	}
	List.setName(ppl6::Trim(ppl6::CString(ui.searchlistName->text())));
	if (!List.save(Filename)) {
		// Fehler behandeln

		return 0;
	}
	wm->UpdateSearchlistOverviews();
	return 1;
}

void SearchlistDialog::on_contextFind_triggered()
{
	if (!currentTrackListItem) return;
	searchWindow=wm->OpenOrReuseSearch(searchWindow,currentTrackListItem->Track.Artist,currentTrackListItem->Track.Title);
	this->setFocus();
}

void SearchlistDialog::on_newTrackButton_clicked()
{
	ppl6::CString Term=ui.searchTerm->text();
	Term.Trim();
	if (Term.IsEmpty()) {
		QString originalText = QApplication::clipboard()->text();
		if (originalText.length()>0 && originalText.length()<512) {
			Term=originalText;
		}
	}
	SearchlistTrackDialog dialog;
	dialog.set(SearchlistItem(Term));
	int ret=dialog.exec();
	if (ret==1) {
		addTrack(dialog.get());
	}
}

void SearchlistDialog::on_searchButton_clicked()
{
	ppl6::CString Term=ui.searchTerm->text();
	Term.Trim();
	SearchlistItem Item(Term);

	searchWindow=wm->OpenOrReuseSearch(searchWindow,Item.Artist,Item.Title);
	this->setFocus();

}

static int matchWords(const ppl6::CArray &needle, const ppl6::CArray &stack)
{
	int matches=0;
	for (int i=0;i<needle.Num();i++) {
		ppl6::CString word=needle[i];
		for (int z=0;z<stack.Num();z++) {
			ppl6::CString comp=stack[z];
			if (word==comp) {
				matches++;
				break;
			}
		}
	}
	if (!matches) return 0;
	return matches*100/needle.Num();
}


void SearchlistDialog::on_ClipBoardTimer_update()
{
	RegExpClipboard clip;
	clip.copyFromClipboard();
	if (clip.PlainText==LastClipboardString) return;
	LastClipboardString=clip.PlainText;
	RegExpMatch match;
	ppl6::CString s;
	if (wm->RegExpCapture.match(clip,match)) {
		s=match.Artist+" "+match.Title;
	} else {
		s=clip.PlainText;
		if (s.PregMatch("/^.*? - .*? \\(.*?,.*?,.*?\\).*$/")) return;
		if (s.Instr("\n")>=0) return;
		s.Replace("\t"," ");
		s.PregReplace("/\\(.*?\\)/","");
	}
	s.LCase();
	ppl6::CArray searchwords;
	if (!wm->GetWords(s,searchwords)) return;

	ClipBoardTimer.stop();
	SearchlistTreeItem *item;
	for (int i=0;i<ui.trackList->topLevelItemCount();i++) {
		item=(SearchlistTreeItem*)ui.trackList->topLevelItem(i);
		if (item) {
			s=item->Track.Artist+" "+item->Track.Title;
			s.LCase();
			ppl6::CArray words;
			if (wm->GetWords(s,words)) {
				if (matchWords(searchwords,words)>70) ui.trackList->setCurrentItem(item);
			}
		}
	}

	ClipBoardTimer.start(200);

}

void SearchlistDialog::updateDupeCheckItem(QTreeWidgetItem *item, int dupePresumption)
{
	ppl6::CString Tmp;
	Tmp.Setf("%3i %%",dupePresumption);
	item->setText(SL_COLUMN_EXISTING,Tmp);
}

void SearchlistDialog::deleteSelectedTracks()
{
	on_deleteTrackButton_clicked();
}

