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
#include "../include/searchlistdialog.h"
#include "../include/searchlisttrackdialog.h"
#include "csearchlist.h"

SearchlistDialog::SearchlistDialog(QWidget *parent, CWmClient *wm, const ppl6::CString &Filename)
    : QWidget(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	List.setWmClient(wm);
	ui.trackList->installEventFilter(this);
	currentTrackListItem=NULL;
	searchWindow=NULL;
	haveCopyItem=false;
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
	for (size_t i=0;i<List.size();i++) {
		item=new SearchlistTreeItem;
		item->Track=List[i];
		renderTrack(item);
		ui.trackList->addTopLevelItem(item);
	}
}

SearchlistDialog::~SearchlistDialog()
{
	if (wm) {
		wm->SearchlistDialogClosed(this);
	}
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
				if (item) deleteTrack(item);
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




void SearchlistDialog::renderTrack(SearchlistTreeItem *item)
{
	ppl6::CString Tmp;
	Tmp=item->Track.Artist+" - "+item->Track.Title;
	item->setText(0,Tmp);
	item->setText(1,item->Track.Version);
	item->setText(2,item->Track.Genre);
	item->setTextAlignment(3,Qt::AlignRight);
	if (item->Track.Length>0) {
		Tmp.Setf("%i:%02i",item->Track.Length/60,item->Track.Length%60);
		item->setText(3,Tmp);
	}
	item->setText(4,item->Track.DateAdded.get("%Y-%m-%d"));

	// Titel in Datenbank suchen
	CTitleHashTree Result;
	int dupePresumption=0;
	wm->Hashes.Find(item->Track.Artist,item->Track.Title,item->Track.Version,"","",Result);
	if (Result.Num()>1) {
		dupePresumption=100;
	} else if (Result.Num()>0) {
		dupePresumption=90;
	} else {
		wm->Hashes.Find(item->Track.Artist,item->Track.Title,Result);
		if (Result.Num()>3) {
			dupePresumption=70;
		} else if (Result.Num()>0) {
			dupePresumption=40;
		}
	}
	Tmp.Setf("%3i %%",dupePresumption);
	item->setText(5,Tmp);


	if (item->Track.found==true) {
		item->setIcon(6,QIcon(":/icons/resources/button_ok.png"));
		item->setText(6,tr("yes"));
	} else {
		item->setIcon(6,QIcon(":/icons/resources/edit-find.png"));
		item->setText(6,tr("no"));
	}


}

void SearchlistDialog::Resize()
{
	int s=ui.trackList->width();
	ui.trackList->setColumnWidth(6,60);
	ui.trackList->setColumnWidth(5,60);
	ui.trackList->setColumnWidth(4,80);
	ui.trackList->setColumnWidth(3,80);
	ui.trackList->setColumnWidth(2,100);
	s=s-62-62-82-82-108;
	if (s<300) s=300;
	ui.trackList->setColumnWidth(1,s*30/100);
	ui.trackList->setColumnWidth(0,s*70/100);
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

    QMenu *m=new QMenu(this);
    QAction *a=NULL;
    a=m->addAction (QIcon(":/icons/resources/edit.png"),tr("Edit Track","trackList Context Menue"),this,SLOT(on_contextEditTrack_triggered()), tr("Return"));
    m->addAction (QIcon(":/icons/resources/delete-track.png"),tr("Delete Track","trackList Context Menue"),this,SLOT(on_contextDeleteTrack_triggered()), tr("Del"));
    m->addAction (QIcon(":/icons/resources/insert-track.png"),tr("Insert Track","trackList Context Menue"),this,SLOT(on_contextInsertTrack_triggered()), tr("Ctrl+N"));
    m->addSeparator();
    m->addAction (QIcon(":/icons/resources/findmore.png"),tr("Search track in Database","trackList Context Menue"),this,SLOT(on_contextFind_triggered()), tr("F4"));

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
	if (column<6) {
		SearchlistItem it=((SearchlistTreeItem*)item)->Track;
		QClipboard *clipboard = QApplication::clipboard();
		ppl6::CString Text;
		Text=it.Artist+" "+it.Title;
		clipboard->setText(Text,QClipboard::Clipboard);
		clipboard->setText(Text,QClipboard::Selection);
	} else if (column==6) {
		SearchlistItem it=((SearchlistTreeItem*)item)->Track;
		if (it.found==true) it.found=false;
		else it.found=true;
		((SearchlistTreeItem*)item)->Track=it;
		renderTrack((SearchlistTreeItem*)item);
		save();
	}
}

void SearchlistDialog::on_trackList_itemDoubleClicked ( QTreeWidgetItem * item, int column)
{
	if (!item) return;
	if (column<5) {
		editTrack((SearchlistTreeItem*)item);
	} else if (column==5) {
		currentTrackListItem=(SearchlistTreeItem*)item;
		on_contextFind_triggered();
	}

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
	renderTrack(item);
	ui.trackList->addTopLevelItem(item);
	save();
}

void SearchlistDialog::on_newTrackButton_clicked()
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

void SearchlistDialog::deleteTrack(SearchlistTreeItem *item)
{
	if (!item) return;
	int index=ui.trackList->indexOfTopLevelItem (item);
	if (index<0) return;
	item=(SearchlistTreeItem*) ui.trackList->takeTopLevelItem(index);
	if (item) delete item;
	save();
}

void SearchlistDialog::on_deleteTrackButton_clicked()
{
	currentTrackListItem=(SearchlistTreeItem*)ui.trackList->currentItem();
	deleteTrack(currentTrackListItem);
	currentTrackListItem=NULL;
	save();
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

void SearchlistDialog::on_newTrackButton2_clicked()
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

