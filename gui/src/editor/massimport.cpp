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



#include <QClipboard>
#include <QMenu>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QMouseEvent>
#include "massimport.h"
#include "../include/edittrackdialog.h"

MassImport::MassImport(QWidget* parent, CWmClient* wm)
	: QDialog(parent)
{
	ppl7::String Tmp;
	ui.setupUi(this);
	this->wm=wm;
	searchWindow=NULL;
	currentTrackListItem=NULL;
	position=oldposition=0;
	TrackList=NULL;

	ui.treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
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
	ui.treeWidget->setStyleSheet(Style);

	TCVersion.Title=tr("Version");
	TCVersion.Init(this, wm, ui.versionId, ui.version, &wm->VersionStore);
	TCVersion.SetNextWidget(ui.versionApplyButton);

	TCGenre.Title=tr("Genre");
	TCGenre.Init(this, wm, ui.genreId, ui.genre, &wm->GenreStore);
	TCGenre.SetNextWidget(ui.genreApplyButton);

	TCLabel.Title=tr("Label");
	TCLabel.Init(this, wm, ui.labelId, ui.labelName, &wm->LabelStore);
	TCLabel.SetNextWidget(ui.labelApplyButton);

	TCRecordSource.Title=tr("Record Source");
	TCRecordSource.Init(this, wm, ui.recordSourceId, ui.recordSource, &wm->RecordSourceStore);
	TCRecordSource.SetNextWidget(ui.recordingSourceApplyButton);

	TCRecordDevice.Title=tr("Record Device");
	TCRecordDevice.Init(this, wm, ui.recordDeviceId, ui.recordDevice, &wm->RecordDeviceStore);
	TCRecordDevice.SetNextWidget(ui.recordDeviceApplyButton);


	QDate Date=QDate::currentDate();
	Tmp=Date.toString("yyyyMMdd");
	ui.releaseDate->setDate(Date);
	ui.recordDate->setDate(Date);



	InstallFilter(ui.versionId, 6);
	InstallFilter(ui.version, 7);
	InstallFilter(ui.genreId, 8);
	InstallFilter(ui.genre, 9);
	InstallFilter(ui.labelId, 15);
	InstallFilter(ui.labelName, 16);
	InstallFilter(ui.recordSourceId, 17);
	InstallFilter(ui.recordSource, 18);
	//InstallFilter(ui.recordDeviceId,19);
	//InstallFilter(ui.recordDevice,20);



}

MassImport::~MassImport()
{
	if (TrackList) delete TrackList;
}

void MassImport::setSearchWindow(QWidget* widget)
{
	searchWindow=widget;
}

QWidget* MassImport::getSearchWindow() const
{
	return searchWindow;
}


void MassImport::Resize()
{
	int w=ui.treeWidget->width();
	ui.treeWidget->setColumnWidth(0, 60);
	ui.treeWidget->setColumnWidth(1, 64);
	ui.treeWidget->setColumnWidth(5, 80);	// Length
	ui.treeWidget->setColumnWidth(6, 50);	// Dupe
	ui.treeWidget->setColumnWidth(7, 40);	// Action
	w=w - 60 - 64 - 80 - 50 - 40 - 5 * 4;
	if (w < 200) w=200;
	ui.treeWidget->setColumnWidth(2, w * 55 / 100);
	ui.treeWidget->setColumnWidth(3, w * 30 / 100);
	ui.treeWidget->setColumnWidth(4, w * 15 / 100);
}

void MassImport::showEvent(QShowEvent* event)
{
	Resize();
	QDialog::showEvent(event);
}

void MassImport::resizeEvent(QResizeEvent* event)
{
	Resize();
	QDialog::resizeEvent(event);
}


void MassImport::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void MassImport::InstallFilter(QObject* object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id", id);
}

bool MassImport::eventFilter(QObject* target, QEvent* event)

{
	if (consumeEvent(target, event)) return true;
	return QDialog::eventFilter(target, event);
}

bool MassImport::consumeEvent(QObject* target, QEvent* event)
{
	//QKeyEvent *keyEvent=NULL;
	//int key=0;
	//int modifier=Qt::NoModifier;
	//QFocusEvent *focusEvent=NULL;

	// Id auslesen
	int id=target->property("id").toInt();
	int type=event->type();
	if (type == QEvent::KeyPress) {
		//keyEvent= static_cast<QKeyEvent *>(event);
		//key=keyEvent->key();
		//modifier=keyEvent->modifiers();
		//if (on_KeyPress(target,key,modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	} else if (type == QEvent::FocusIn || type == QEvent::FocusOut) {
		//focusEvent=static_cast<QFocusEvent *>(event);
		if (type == QEvent::FocusIn) {
			position=id;
		} else if (type == QEvent::FocusOut) {
			oldposition=id;
		}
	}

	if (target == ui.version || target == ui.versionId) {
		return TCVersion.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.genre || target == ui.genreId) {
		return TCGenre.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.labelName || target == ui.labelId) {
		return TCLabel.ConsumeEvent(target, event, oldposition, position);
	} else if (target == ui.recordSource || target == ui.recordSourceId) {
		return TCRecordSource.ConsumeEvent(target, event, oldposition, position);
	}
	return false;
}


int MassImport::load(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint16 StartTrack)
{

	this->DeviceType=DeviceType;
	this->DeviceId=DeviceId;
	this->Page=Page;
	this->StartTrack=StartTrack;

	TrackList=wm->GetTracklist(DeviceType, DeviceId, Page);


	ppl7::String Path, Filename, Pattern;
	ppl7::String MP3Path=wm->conf.DevicePath[DeviceType];

	if (MP3Path.isEmpty()) return 0;
	Path=MP3Path;
	Path.trimRight("/");
	Path.trimRight("\\");
	Path.appendf("/%02u/%03u/", (ppluint32)(DeviceId / 100), DeviceId);
	Pattern.setf("*.mp3");
	ppl6::CDir Dir;
	const ppl6::CDirEntry* entry;
	int count=0;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	qApp->processEvents();
	ui.treeWidget->setSortingEnabled(false);

	if (Dir.Open(Path, ppl6::CDir::Sort_Filename_IgnoreCase)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 5, "CWMClient", "NextMP3File", __FILE__, __LINE__, "%i Dateien vorhanden, suche nach Pattern...", Dir.Num());
		while ((entry=Dir.GetNextPattern(Pattern, true))) {
			Filename=entry->Filename;
			// Der Dateiname darf nicht mit drei Ziffern und Bindestrich beginnen
			if (!Filename.pregMatch("/^[0-9]{3}\\-.*/")) {
				// Muss aber mit .mp3 enden
				if (Filename.pregMatch("/^.*\\.mp3$/i")) {
					//printf ("%s\n",(const char*)Filename);
					addTrack(entry->File);
					count++;
				}
			}
		}
	}
	ui.treeWidget->sortByColumn(0, Qt::AscendingOrder);
	ui.treeWidget->setSortingEnabled(true);
	QApplication::restoreOverrideCursor();
	if (count) return 1;
	QMessageBox::information(this, tr("WinMusik: Notice"),
		tr("There are no tracks to import"));
	return 0;

}

void MassImport::addTrack(const ppl7::String Filename)
{
	//printf ("%s\n",(const char*)Filename);
	ppl7::String Tmp;
	TreeItem* item=new TreeItem;
	item->Filename=Filename;
	item->dupePresumption=0;
	item->import=true;
	getTrackInfoFromFile(item->info, Filename, wm->conf.ReadId3Tag);
	item->info.Ti.VersionId=wm->VersionStore.GetId(item->info.Version);
	item->info.Ti.GenreId=wm->GenreStore.GetId(item->info.Genre);
	item->info.Ti.LabelId=wm->LabelStore.GetId(item->info.Label);
	item->info.Ti.RecordSourceId=wm->RecordSourceStore.GetId(item->info.RecordingSource);
	item->info.Ti.RecordDeviceId=wm->RecordDeviceStore.GetId(item->info.RecordingDevice);

	checkDupes(item);
	Tmp.setf("%5i", ui.treeWidget->topLevelItemCount() + 1);
	item->setText(0, Tmp);
	renderTrack(item);


	ui.treeWidget->addTopLevelItem(item);
	qApp->processEvents();
}


static void FilterResult(const CHashes::TitleTree& in, ppl6::CGenericList& out)
{
	CHashes::TitleTree::const_iterator it;
	for (it=in.begin();it != in.end();it++) {
		DataTitle* ti=wm_main->GetTitle(*it);
		if (ti->DeviceType == 7) out.Add(ti);
	}
}

void MassImport::checkDupes(TreeItem* item)
{
	ppl7::String Key, Version;
	if (item->info.Ti.VersionId > 0) Version=wm->GetVersionText(item->info.Ti.VersionId);
	else Version=item->info.Version;
	Key.setf("%s %s ", (const char*)item->info.Ti.Artist, (const char*)item->info.Ti.Title);
	Key+=Version;
	Key.lowerCase();

	std::set<ppl7::String>::iterator it;
	it=LocalDupeCheck.find(Key);
	if (it != LocalDupeCheck.end()) {
		// Den Titel haben wir scheinbar schon
		item->dupePresumption=100;
		item->import=false;
	} else {
		LocalDupeCheck.insert(Key);
		CHashes::TitleTree Result;
		ppl6::CGenericList list;
		wm->Hashes.Find(item->info.Ti.Artist, item->info.Ti.Title, Version, "", "", "", Result);
		FilterResult(Result, list);
		if (list.Num() > 1) {
			item->dupePresumption=100;
			item->import=false;
		} else if (Result.size() > 0) {
			item->dupePresumption=90;
			item->import=false;
		} else {
			ppl6::CGenericList list2;
			wm->Hashes.Find(item->info.Ti.Artist, item->info.Ti.Title, Result);
			FilterResult(Result, list2);
			if (list2.Num() > 3) {
				item->dupePresumption=70;
				item->import=true;
			} else if (list2.Num() > 0) {
				item->dupePresumption=40;
				item->import=true;
			}
		}
	}
}




void MassImport::renderTrack(TreeItem* item)
{
	QBrush Brush(Qt::SolidPattern);
	Brush.setColor("red");
	ppl7::String Tmp;

	// Cover
	if (item->info.Ti.CoverPreview.size() > 0) {
		QPixmap pix, icon;
		pix.loadFromData((const uchar*)item->info.Ti.CoverPreview.ptr(), item->info.Ti.CoverPreview.size());
		item->setIcon(1, pix.copy(0, 0, 64, 16));
	} else {
		item->setIcon(1, QIcon());
	}

	// Interpret - Titel
	Tmp.setf("%s - %s", (const char*)item->info.Ti.Artist, (const char*)item->info.Ti.Title);
	item->setText(2, Tmp);

	// Version
	if (item->info.Ti.VersionId > 0) Tmp=wm->GetVersionText(item->info.Ti.VersionId);
	else Tmp=item->info.Version;
	item->setText(3, Tmp);
	item->setForeground(3, Brush);

	// Genre
	if (item->info.Ti.GenreId > 0) Tmp=wm->GetGenreText(item->info.Ti.GenreId);
	else Tmp=item->info.Genre;
	item->setText(4, Tmp);

	// Länge
	Tmp.setf("%4i:%02i", (int)(item->info.Ti.Length / 60), item->info.Ti.Length % 60);
	item->setText(5, Tmp);

	// Dupes
	Tmp.setf("%3i %%", item->dupePresumption);
	item->setText(6, Tmp);

	if (item->import) {
		item->setIcon(7, QIcon(":/icons/resources/button_ok.png"));
		item->setText(7, "");
	} else {
		item->setIcon(7, QIcon(":/icons/resources/edit-delete.png"));
		item->setText(7, " ");
	}


}

void MassImport::on_treeWidget_customContextMenuRequested(const QPoint& pos)
{
	QPoint p=ui.treeWidget->mapToGlobal(pos);
	currentTrackListItem=(TreeItem*)ui.treeWidget->itemAt(pos);
	if (!currentTrackListItem) return;
	QMenu* m=new QMenu(this);
	QAction* a=m->addAction(QIcon(":/icons/resources/findmore.png"), tr("search track", "trackList Context Menue"), this, SLOT(on_contextFindMoreVersions_triggered()));
	m->addAction(QIcon(":/icons/resources/play.png"), tr("Play Track", "trackList Context Menue"), this, SLOT(on_contextPlayTrack_triggered()));
	m->addAction(QIcon(":/icons/resources/edit.png"), tr("Edit Track", "trackList Context Menue"), this, SLOT(on_contextEditTrack_triggered()));
	m->addAction(QIcon(":/icons/resources/delete-track.png"), tr("Delete Track", "trackList Context Menue"), this, SLOT(on_contextDeleteTrack_triggered()));
	m->popup(p, a);
}

void MassImport::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int)
{
	TreeItem* ti=(TreeItem*)item;
	if (!ti) return;
	wm->PlayFile(ti->Filename);
}

void MassImport::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
	TreeItem* ti=(TreeItem*)item;
	if (!ti) return;
	if (column == 7) {
		ti->import=!ti->import;
		if (ti->import) item->setIcon(7, QIcon(":/icons/resources/button_ok.png"));
		else ti->setIcon(7, QIcon(":/icons/resources/edit-delete.png"));

	}
}


void MassImport::on_contextFindMoreVersions_triggered()
{
	if (!currentTrackListItem) return;
	searchWindow=wm->OpenOrReuseSearch(searchWindow, currentTrackListItem->info.Ti.Artist,
		currentTrackListItem->info.Ti.Title);
}

void MassImport::on_contextPlayTrack_triggered()
{
	if (!currentTrackListItem) return;
	wm->PlayFile(currentTrackListItem->Filename);
}

void MassImport::on_contextEditTrack_triggered()
{
	if (!currentTrackListItem) return;
	EditTrackDialog dialog(this, wm);
	dialog.setData(currentTrackListItem->info);
	dialog.setFilename(currentTrackListItem->Filename);
	int ret=dialog.exec();
	if (ret == 1) {
		currentTrackListItem->info=dialog.getData();
		renderTrack(currentTrackListItem);
	}
}

void MassImport::on_contextDeleteTrack_triggered()
{
	if (!currentTrackListItem) return;
	if (QMessageBox::question(this, tr("WinMusik: Delete this Tracks"),
		tr("Do you really want to delete this track from your harddisk?\nYou won't be able to restore it!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
		== QMessageBox::No) return;
	int	index=ui.treeWidget->indexOfTopLevelItem(currentTrackListItem);
	if (index >= 0) ui.treeWidget->takeTopLevelItem(index);
	ppl6::CFile::DeleteFile(currentTrackListItem->Filename);
	delete currentTrackListItem;
	currentTrackListItem=NULL;
}

void MassImport::on_versionApplyButton_clicked()
{
	ppl6::CString Value=ui.version->text();
	int id=wm->VersionStore.GetId(Value);
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.Version=Value;
			item->info.Ti.VersionId=id;
			checkDupes(item);
			renderTrack(item);
		}
	}
}

void MassImport::on_genreApplyButton_clicked()
{
	ppl6::CString Value=ui.genre->text();
	int id=wm->GenreStore.GetId(Value);
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.Genre=Value;
			item->info.Ti.GenreId=id;
			renderTrack(item);
		}
	}
}

void MassImport::on_labelApplyButton_clicked()
{
	ppl6::CString Value=ui.labelName->text();
	int id=wm->LabelStore.GetId(Value);
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.Label=Value;
			item->info.Ti.LabelId=id;
			renderTrack(item);
		}
	}
}

void MassImport::on_recordingSourceApplyButton_clicked()
{
	ppl6::CString Value=ui.recordSource->text();
	int id=wm->RecordSourceStore.GetId(Value);

	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.RecordingSource=Value;
			item->info.Ti.RecordSourceId=id;
			renderTrack(item);
		}
	}
}

void MassImport::on_recordDeviceApplyButton_clicked()
{
	ppl6::CString Value=ui.recordDevice->text();
	int id=wm->RecordDeviceStore.GetId(Value);

	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.RecordingDevice=Value;
			item->info.Ti.RecordDeviceId=id;
			renderTrack(item);
		}
	}
}

void MassImport::on_albumApplyButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.Ti.SetAlbum(ui.album->text());
			renderTrack(item);
		}
	}
}

void MassImport::on_tagsApplyButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.Ti.SetTags(ui.tags->text());
			renderTrack(item);
		}
	}
}

void MassImport::on_remarksApplyButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.Ti.SetRemarks(ui.remarks->text());
			renderTrack(item);
		}
	}
}

void MassImport::on_dateApplyButton_clicked()
{
	ppl6::CString Tmp;
	// Erscheinungsjahr
	QDate Date=ui.releaseDate->date();
	Tmp=Date.toString("yyyyMMdd");
	ppluint32 ReleaseDate=Tmp.ToInt();

	// Aufnahmedatum
	Date=ui.recordDate->date();
	Tmp=Date.toString("yyyyMMdd");
	//printf ("Date: %s\n",(const char*)Tmp);
	ppluint32 RecordDate=Tmp.ToInt();


	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->info.Ti.ReleaseDate=ReleaseDate;
			item->info.Ti.RecordDate=RecordDate;
			renderTrack(item);
		}
	}
}


void MassImport::on_markImportSelectedTracksButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->import=true;
			renderTrack(item);
		}
	}
}

void MassImport::on_markIgnoreSelectedTracksButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			item->import=false;
			renderTrack(item);
		}
	}
}

void MassImport::on_deleteSelectedTracksButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	if (list.size() == 0) return;
	if (QMessageBox::question(this, tr("WinMusik: Delete selected Tracks"),
		tr("Do you really want to delete the selected tracks from your harddisk?\nYou won't be able to restore them!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
		== QMessageBox::No) return;

	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item) {
			int	index=ui.treeWidget->indexOfTopLevelItem(item);
			if (index >= 0) ui.treeWidget->takeTopLevelItem(index);
			ppl6::CFile::DeleteFile(item->Filename);
			delete item;
		}
	}
}

void MassImport::on_importSelectedTracksButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	TreeItem* item;
	list=ui.treeWidget->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item != NULL && item->import == true) {
			if (importTrack(item)) {
				int	index=ui.treeWidget->indexOfTopLevelItem(item);
				if (index >= 0) ui.treeWidget->takeTopLevelItem(index);
				delete item;
			} else {
				return;
			}
			qApp->processEvents();
		}
	}
}

void MassImport::on_selectAllButton_clicked()
{
	ui.treeWidget->selectAll();
}

void MassImport::on_selectNoneButton_clicked()
{
	TreeItem* item;
	for (int i=0;i < ui.treeWidget->topLevelItemCount();i++) {
		item=(TreeItem*)ui.treeWidget->topLevelItem(i);
		if (item) item->setSelected(false);
	}
}

void MassImport::on_exitButton_clicked()
{
	this->close();
}

void MassImport::on_startImportButton_clicked()
{
	QList<QTreeWidgetItem*> list;
	for (int i=0;i < ui.treeWidget->topLevelItemCount();i++) {
		list.push_back(ui.treeWidget->topLevelItem(i));
	}
	TreeItem* item;
	for (int i = 0; i < list.size(); ++i) {
		item=(TreeItem*)list.at(i);
		if (item != NULL && item->import == true) {
			if (importTrack(item)) {
				int	index=ui.treeWidget->indexOfTopLevelItem(item);
				if (index >= 0) ui.treeWidget->takeTopLevelItem(index);
				delete item;
			} else {
				return;
			}
			qApp->processEvents();
		}
	}
}

bool MassImport::importTrack(TreeItem* item)
{
	ppl7::String Tmp;
	DataTitle Ti;
	Ti.TitleId=0;
	Ti.DeviceId=DeviceId;
	Ti.DeviceType=DeviceType;
	Ti.Page=Page;
	Ti.SetArtist(item->info.Ti.Artist);
	Ti.SetTitle(item->info.Ti.Title);
	Ti.SetRemarks(item->info.Ti.Remarks);
	Ti.SetTags(item->info.Ti.Tags);
	Ti.SetAlbum(item->info.Ti.Album);
	Ti.Length=item->info.Ti.Length;
	Ti.Bitrate=item->info.Ti.Bitrate;
	Ti.Size=item->info.Ti.Size;
	Ti.VersionId=item->info.Ti.VersionId;
	Ti.GenreId=item->info.Ti.GenreId;
	Ti.LabelId=item->info.Ti.LabelId;
	Ti.RecordSourceId=item->info.Ti.RecordSourceId;
	Ti.RecordDeviceId=item->info.Ti.RecordDeviceId;

	if (!Ti.VersionId) Ti.VersionId=wm->VersionStore.FindOrAdd(item->info.Version);
	if (!Ti.GenreId) Ti.GenreId=wm->GenreStore.FindOrAdd(item->info.Genre);
	if (!Ti.LabelId) Ti.LabelId=wm->LabelStore.FindOrAdd(item->info.Label);
	if (!Ti.RecordSourceId) Ti.RecordSourceId=wm->RecordSourceStore.FindOrAdd(item->info.RecordingSource);
	if (!Ti.RecordDeviceId) Ti.RecordDeviceId=wm->RecordDeviceStore.FindOrAdd(item->info.RecordingDevice);

	// Aufnahmedatum
	Ti.RecordDate=item->info.Ti.RecordDate;
	if (Ti.RecordDate == 0) {
		ppl7::DateTime now;
		now.setCurrentTime();
		Ti.RecordDate=now.get("%Y%m%d").toInt();
	}

	// Erscheinungsjahr
	Ti.ReleaseDate=item->info.Ti.ReleaseDate;
	if (Ti.ReleaseDate == 0) {
		QDate Date=QDate::currentDate();
		Tmp=Date.toString("yyyyMMdd");
		Ti.ReleaseDate=Tmp.toInt();
	}

	// Flags
	Ti.Flags=item->info.Ti.Flags;
	Ti.Channels=item->info.Ti.Channels;

	// Cover
	if (item->info.Ti.CoverPreview.size() > 0) {
		Ti.CoverPreview=item->info.Ti.CoverPreview;
	}

	DataTrack Track;
	Track.Track=TrackList->GetMax() + 1;	// Wo kommt die Tracknummer her?
	Ti.Track=Track.Track;

	if (!wm->TitleStore.Put(&Ti)) {
		wm->RaiseError(this, tr("Could not save Title in TitleStore"));
		return false;
	}
	DataTitle* dt=wm->TitleStore.Get(Ti.TitleId);
	if (dt) wm->Hashes.AddTitle(Ti.TitleId, dt);

	// Track speichern
	Track.TitleId=Ti.TitleId;
	Track.Device=DeviceType;
	Track.DeviceId=DeviceId;
	Track.Page=Page;
	if (!TrackList->Put(&Track)) {
		wm->RaiseError(this, tr("Could not save Track in TrackList"));
		return false;
	}
	// Datei muss umbenannt werden

	if (!wm->SaveID3Tags(Track.Device, Track.DeviceId, Page, Track.Track, Ti, item->Filename)) {
		wm->RaiseError(this, tr("Could not save ID3 Tags"));
		return false;
	}
	// Tonträger aktualisieren
	wm->DeviceStore.Update(DeviceType, DeviceId);

	return true;
}
