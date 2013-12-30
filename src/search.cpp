/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.15 $
 * $Date: 2010/12/23 18:04:40 $
 * $Id: search.cpp,v 1.15 2010/12/23 18:04:40 pafe Exp $
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
#include <QMimeData>
#include <QMouseEvent>
#include "../include/search.h"

#include <vector>

Search::Search(QWidget *parent, CWmClient *wm)
    : QWidget(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	resultmode=0;
	ClipBoardTimer.setParent(this);
	setAttribute(Qt::WA_DeleteOnClose,true);
	SetupTrackList();
	ui.tabWidget->setCurrentIndex(0);
	ui.artist->setFocus(Qt::ActiveWindowFocusReason);
	ui.searchButton->setDefault(true);
	ui.searchButton->setAutoDefault(true);
	currentTrackListItem=NULL;
	if (wm) {
		ui.traeger_1->setChecked(wm->conf.SearchDevice[1]);
		ui.traeger_2->setChecked(wm->conf.SearchDevice[2]);
		ui.traeger_3->setChecked(wm->conf.SearchDevice[3]);
		ui.traeger_4->setChecked(wm->conf.SearchDevice[4]);
		ui.traeger_5->setChecked(wm->conf.SearchDevice[5]);
		ui.traeger_6->setChecked(wm->conf.SearchDevice[6]);
		ui.traeger_7->setChecked(wm->conf.SearchDevice[7]);
		ui.traeger_8->setChecked(wm->conf.SearchDevice[8]);
		ui.traeger_9->setChecked(wm->conf.SearchDevice[9]);
		//ui.fastsearch->setChecked(wm->conf.SearchFast);
	}
	musicKeyDisplay=wm->conf.musicKeyDisplay;
	switch (musicKeyDisplay) {
		case musicKeyTypeMusicalSharps: ui.displayMusicKey->setCurrentIndex(0); break;
		case musicKeyTypeOpenKey: ui.displayMusicKey->setCurrentIndex(1); break;
		case musicKeyTypeCustom: ui.displayMusicKey->setCurrentIndex(2); break;
		default: ui.displayMusicKey->setCurrentIndex(1); break;
	}
	ui.displayMusicKey->setItemText(2,wm->conf.customMusicKeyName);

	update();
	connect(&ClipBoardTimer, SIGNAL(timeout()), this, SLOT(on_ClipBoardTimer_update()));
}

Search::~Search()
{
	if (wm) {
	//wm->conf.SearchFast=ui.fastsearch->isChecked();
	wm->conf.SearchDevice[1]=ui.traeger_1->isChecked();
	wm->conf.SearchDevice[2]=ui.traeger_2->isChecked();
	wm->conf.SearchDevice[3]=ui.traeger_3->isChecked();
	wm->conf.SearchDevice[4]=ui.traeger_4->isChecked();
	wm->conf.SearchDevice[5]=ui.traeger_5->isChecked();
	wm->conf.SearchDevice[6]=ui.traeger_6->isChecked();
	wm->conf.SearchDevice[7]=ui.traeger_7->isChecked();
	wm->conf.SearchDevice[8]=ui.traeger_8->isChecked();
	wm->conf.SearchDevice[9]=ui.traeger_9->isChecked();
	wm->conf.Save();
	wm->SearchClosed(this);
	}
}


void Search::Resize()
{
	int w=trackList->width();
	trackList->setColumnWidth(0,50);
	if (resultmode==0) {
		w=w-230-40-10-42-42-32;
		trackList->setColumnWidth(1,w*35/100);
		trackList->setColumnWidth(2,w*30/100);
		trackList->setColumnWidth(3,w*25/100);
		trackList->setColumnWidth(4,w*10/100);
		trackList->setColumnWidth(5,50);
		trackList->setColumnWidth(SEARCH_TRACKLIST_BPM_ROW,40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_KEY_ROW,40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_ENERGYLEVEL_ROW,30);
		trackList->setColumnWidth(SEARCH_TRACKLIST_RATING_ROW,40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_SOURCE_ROW,120);
	} else {
		printf ("resultmpode=%i\n",resultmode);
		trackList->setColumnWidth(1,w-100);
		trackList->setColumnWidth(2,50);
	}

}

void Search::showEvent(QShowEvent * event)
{
	Resize();
	QWidget::showEvent(event);
}
void Search::resizeEvent(QResizeEvent * event)
{
	Resize();
	QWidget::resizeEvent(event);
}

void Search::SetupTrackList()
/*!\brief Trackliste einrichten
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
	trackList=new CSearchTrackList(ui.listWidget);
	w->addWidget(trackList);
	trackList->search=this;
	trackList->wm=wm;
	resultmode=0;
	//trackList->installEventFilter(this);
	//trackList->setMouseTracking(true);
    trackList->setDragEnabled(false);
    trackList->setDragDropMode(QAbstractItemView::DragOnly);

    DefaultTracklistHeader();

    trackList->setObjectName(QString::fromUtf8("trackList"));
    trackList->setAlternatingRowColors(true);
    trackList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    trackList->setRootIsDecorated(false);
    trackList->setItemsExpandable(false);
    trackList->setSortingEnabled(false);
    trackList->setContextMenuPolicy(Qt::CustomContextMenu);


    connect(trackList,SIGNAL(customContextMenuRequested(const QPoint &)),
    		this,SLOT(on_trackList_customContextMenuRequested(const QPoint &)));
    connect(trackList,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
    		this,SLOT(on_trackList_itemDoubleClicked(QTreeWidgetItem *, int)));
    connect(trackList,SIGNAL(itemClicked(QTreeWidgetItem *, int)),
    		this,SLOT(on_trackList_itemClicked(QTreeWidgetItem *, int)));

    Resize();
    trackList->update();
}

void Search::DefaultTracklistHeader()
{
	resultmode=0;
    trackList->headerItem()->setText(0, tr("Pos","trackList"));
    trackList->headerItem()->setText(1, tr("Artist","trackList"));
    trackList->headerItem()->setText(2, tr("Title","trackList"));
    trackList->headerItem()->setText(3, tr("Version","trackList"));
    trackList->headerItem()->setText(4, tr("Genre","trackList"));
    trackList->headerItem()->setText(5, tr("Length","trackList"));
    trackList->headerItem()->setText(SEARCH_TRACKLIST_BPM_ROW, tr("BPM","trackList"));
    trackList->headerItem()->setText(SEARCH_TRACKLIST_KEY_ROW, tr("Key","trackList"));
    trackList->headerItem()->setText(SEARCH_TRACKLIST_ENERGYLEVEL_ROW, tr("Energy","trackList"));
    trackList->headerItem()->setText(SEARCH_TRACKLIST_SOURCE_ROW, tr("Medium","trackList"));
    trackList->headerItem()->setText(SEARCH_TRACKLIST_RATING_ROW, tr("Rating","trackList"));

}


void Search::ReloadTranslation()
{
	ui.retranslateUi(this);
}

/*
void Search::StartSearch(const char *artist, const char *title)
{
	ui.artist->setText("");
	ui.title->setText("");
	if (artist) ui.artist->setText(artist);
	if (title) ui.title->setText(title);
	DoSearch();
}
*/

void Search::FastSearch(const ppl6::CString &Artist, const ppl6::CString &Title, const ppl6::CString &Version,const ppl6::CString &Genre,const ppl6::CString &Tags,const ppl6::CString &Label)
{
	ui.artist->setText(Artist);
	ui.title->setText(Title);
	ui.version->setText(Version);
	ui.genre->setText(Genre);
	ui.tags->setText(Tags);
	ui.recordLabel->setText(Label);
	ui.numTracks->setText("");

	currentTrackListItem=NULL;
	trackList->clear();
	trackList->setWordWrap(false);
	trackList->setSortingEnabled(false);
	Results.Clear();
	CheckAllowedDevices();

	ppl6::CString Tmp;
	ui.progressBar->setValue(0);
	CTitleHashTree res;
	if (wm->Hashes.Find(Artist,Title,Version,Genre,Tags,Label,res)) {
		Tmp.Setf("%u",res.Num());
		ui.numTracks->setText(Tmp);
	}
	ui.progressBar->setValue(30);

	ppl6::CGenericList list;
	FilterResult(res,list);
	LimitResult(list,Results);

	ui.progressBar->setValue(60);
	PresentResults();
	ui.progressBar->setValue(80);
	trackList->sortByColumn(0,Qt::AscendingOrder);
	trackList->setSortingEnabled(true);
	ui.progressBar->setValue(100);
	ui.artist->setFocus();

}


void Search::CheckAllowedDevices()
{
	for (int i=0;i<20;i++) AllowedDevices[i]=0;
	if (ui.traeger_1->isChecked()) AllowedDevices[1]=1;
	if (ui.traeger_2->isChecked()) AllowedDevices[2]=1;
	if (ui.traeger_3->isChecked()) AllowedDevices[3]=1;
	if (ui.traeger_4->isChecked()) AllowedDevices[4]=1;
	if (ui.traeger_5->isChecked()) AllowedDevices[5]=1;
	if (ui.traeger_6->isChecked()) AllowedDevices[6]=1;
	if (ui.traeger_7->isChecked()) AllowedDevices[7]=1;
	if (ui.traeger_8->isChecked()) AllowedDevices[8]=1;
	if (ui.traeger_9->isChecked()) AllowedDevices[9]=1;

	int c=0;
	for (int i=0;i<20;i++) if (AllowedDevices[i]) c++;
	if (c==0) {	// Nix ausgewählt
		for (int i=0;i<20;i++) AllowedDevices[i]=1;
	}
}

void Search::DoSearch()
{
	ppl6::CString Artist=ui.artist->text();
	ppl6::CString Title=ui.title->text();
	ppl6::CString Version=ui.version->text();
	ppl6::CString Genre=ui.genre->text();
	ppl6::CString Tags=ui.tags->text();
	ppl6::CString Label=ui.recordLabel->text();
	Artist=ppl6::Trim(ppl6::LCase(Artist));
	Title=ppl6::Trim(ppl6::LCase(Title));
	Version=ppl6::Trim(ppl6::LCase(Version));
	Genre=ppl6::Trim(ppl6::LCase(Genre));
	Tags=ppl6::Trim(ppl6::LCase(Tags));
	Label=ppl6::Trim(ppl6::LCase(Label));

	//if (ui.fastsearch->isChecked()) {
		FastSearch(Artist,Title,Version,Genre,Tags,Label);
		ui.artist->setFocus();
		return;
	//}
	/*
	ui.numTracks->setText("");
	currentTrackListItem=NULL;
	trackList->clear();
	trackList->setWordWrap(false);
	trackList->setSortingEnabled(false);
	Results.Clear();
	CheckAllowedDevices();


	ppl6::CGenericList Res;
	CTitleStore *store=&wm->TitleStore;
	ppluint32 max=store->MaxId();
	int oldprozent=0,prozent=0;
	DataTitle *ti;
	ppl6::CString SArtist, STitle;
	for (ppluint32 i=0;i<=max;i++) {
		prozent=(i*100)/max;
		if (prozent!=oldprozent) {
			oldprozent=prozent;
			ui.progressBar->setValue(prozent);
			qApp->processEvents();
		}
		ti=store->Get(i);
		if (ti) {
			if (ti->DeviceType<20 && AllowedDevices[ti->DeviceType]==1) {
				SArtist=ti->Artist;
				STitle=ti->Title;
				SArtist.LCase();
				STitle.LCase();
				if (Artist.NotEmpty()) {
					if (SArtist.Instr(Artist)>=0) {
						if (Title.IsEmpty()) Res.Add(ti);
						else if (STitle.Instr(Title)>=0) Res.Add(ti);
					}
				} else if (Title.NotEmpty()) {
					if (STitle.Instr(Title)>=0) Res.Add(ti);
				}
			}
		}
	}
	LimitResult(Res,Results);

	PresentResults();
	prozent=100;
	ui.progressBar->setValue(prozent);
	trackList->sortByColumn(0,Qt::AscendingOrder);
	trackList->setSortingEnabled(true);
	ui.artist->setFocus();
	*/
}


void Search::FilterResult(const CTitleHashTree &in, ppl6::CGenericList &out)
{
	ppl6::CAVLTree::Walker walk;
	DataTitle *ti;
	while ((ti=(DataTitle*)in.GetPrevious(walk))) {
		if (ti->DeviceType<20 && AllowedDevices[ti->DeviceType]==1) {
			out.Add(ti);
		}
	}
}

void Search::RandomResult(const ppl6::CGenericList &in, ppl6::CGenericList &out, size_t num)
{
	// Zuerst kopieren wir die Eingangsliste zum besseren Zugriff in einen vector
	std::vector<DataTitle*> list, list2;
	size_t r;
	DataTitle *ti;
	ppl6::CGenericList::Walker walk;
	while((ti=(DataTitle*)in.GetNext(walk))) list.push_back(ti);
	if (num>list.size()) num=list.size();
	while (num) {
		num--;
		ti=NULL;
		while (!ti) {
			r=ppl6::rand(0,list.size()-1);
			ti=list[r];
			if (!ti) {
				// vector muss aufgeräumt werden
				list2.clear();
				for (size_t i=0;i<list.size();i++) {
					ti=list[i];
					if (ti) list2.push_back(ti);
				}
				if (!list2.size()) return;
				list=list2;
				ti=NULL;
			}
		}
		list[r]=NULL;
		out.Add(ti);
	}
}

void Search::LimitResult(const ppl6::CGenericList &in, ppl6::CGenericList &out)
{


	ppl6::CGenericList::Walker walk;
	DataTitle *ti;
	int limit=ui.limitResultSpinBox->value();
	if (ui.randomResultCheckBox->isChecked()) {
		// Wir wollen Zufall
		if (limit==0 || limit>=in.Num()) limit=in.Num();
		RandomResult(in,out,limit);
		return;
	}
	if (limit==0 || limit>=in.Num()) {
		for (int i=0;i<in.Num();i++) {
			ti=(DataTitle*)in.GetPrevious(walk);
			out.Add(ti);
		}

	} else {
		for (int i=0;i<limit;i++) {
			ti=(DataTitle*)in.GetNext(walk);
			out.Add(ti);
		}
	}
}

void Search::updateTrackListing()
{
	for (int i=0;i<trackList->topLevelItemCount();i++) {
		WMTreeItem *item=(WMTreeItem*)trackList->topLevelItem(i);
		if (item!=NULL && item->Id>0) {
			DataTitle *ti=wm->GetTitle(item->Id);
			if (ti) renderTrack(item,ti);
		}
	}
}


void Search::renderTrack(WMTreeItem *item, DataTitle *ti)
{
	const char Seite[]="?ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	ppl6::CString Tmp;
	QPixmap icon;
	QBrush Brush(Qt::SolidPattern);
	Brush.setColor("red");


	Tmp.Setf("%5u",item->Track);
	item->setText(0,Tmp);

	if (ti->CoverPreview.Size()) {
		icon.loadFromData((const uchar*)ti->CoverPreview.GetPtr(),ti->CoverPreview.GetSize());
		item->setIcon(1,icon.scaled(16,16,Qt::KeepAspectRatio,Qt::SmoothTransformation));

	} else {
		item->setIcon(1,QIcon(":/icons/resources/cover16.png"));

	}
	Tmp=ti->Artist;
	item->setText(1,Tmp);
	Tmp=ti->Title;
	item->setText(2,Tmp);
	item->setForeground(3,Brush);
	Tmp=wm->GetVersionText(ti->VersionId);
	item->setText(3,Tmp);
	Tmp=wm->GetGenreText(ti->GenreId);
	item->setText(4,Tmp);
	Tmp.Setf("%4i:%02i",(int)(ti->Length/60),ti->Length%60);
	item->setText(5,Tmp);

	// BPM, Key und EnergyLevel
	Tmp.Setf("%d",(int)ti->BPM);
	item->setText(SEARCH_TRACKLIST_BPM_ROW,Tmp);
	item->setText(SEARCH_TRACKLIST_KEY_ROW,ti->getKeyName(musicKeyDisplay));
	if ((ti->Flags&16)) item->setTextColor(SEARCH_TRACKLIST_KEY_ROW,QColor(0,0,0));
	else item->setTextColor(SEARCH_TRACKLIST_KEY_ROW,QColor(192,192,192));
	Tmp.Setf("%d",(int)ti->EnergyLevel);
	item->setText(SEARCH_TRACKLIST_ENERGYLEVEL_ROW,Tmp);

	// Tonträger
	Tmp.Setf("%u %c-%03u", ti->DeviceId,Seite[(ti->Page<10?ti->Page:0)],ti->Track);
	item->setText(SEARCH_TRACKLIST_SOURCE_ROW,Tmp);
	QIcon Icon;
	Tmp.Setf(":/devices16/resources/tr16x16-%04i.png",ti->DeviceType);
	Icon.addFile(Tmp);
	item->setIcon(SEARCH_TRACKLIST_SOURCE_ROW,Icon);

	// Bewertung
	switch (ti->Rating) {
		case 0: item->setIcon(SEARCH_TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/rating-0.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW,"0");
			break;
		case 1: item->setIcon(SEARCH_TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/rating-1.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW,"1");
			break;
		case 2: item->setIcon(SEARCH_TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/rating-2.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW,"2");
			break;
		case 3: item->setIcon(SEARCH_TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/rating-3.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW,"3");
			break;
		case 4: item->setIcon(SEARCH_TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/rating-4.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW,"4");
			break;
		case 5: item->setIcon(SEARCH_TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/rating-5.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW,"5");
			break;
		case 6: item->setIcon(SEARCH_TRACKLIST_RATING_ROW,QIcon(":/bewertung/resources/rating-6.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW,"6");
			break;
	}
}

void Search::PresentResults()
{

	DataTitle *ti;

	ppl6::CString Tmp;
	Tmp.Setf("%u",Results.Num());
	ui.numTracks->setText(Tmp);
	ui.totalLength->setText("");
	DefaultTracklistHeader();
    Resize();
    trackList->update();

    ppluint64 totalLength=0;

	Results.Reset();
	ppluint32 i=0;
	while ((ti=(DataTitle*)Results.GetPrevious())) {
		i++;
		WMTreeItem *item=new WMTreeItem;
		item->Track=i;
		item->Id=ti->TitleId;

		renderTrack(item,ti);
		totalLength+=ti->Length;
		trackList->addTopLevelItem(item);
	}

	int h=(int)(totalLength/3600);
	int m=totalLength-(h*3600);
	int s=m%60;
	m=m/60;
	Tmp.Setf("%0i:%02i:%02i",h,m,s);
	ui.totalLength->setText(Tmp);


}

void Search::on_searchButton_clicked()
{
	DoSearch();
	ui.artist->setFocus();
}

void Search::on_quicksearchButton_clicked()
{
	ui.numTracks->setText("");
	currentTrackListItem=NULL;
	trackList->clear();
	trackList->setWordWrap(false);
	trackList->setSortingEnabled(false);
	Results.Clear();
	CheckAllowedDevices();
	ppl6::CString Tmp;
	ppl6::CString Query=ui.query->text();
	ui.progressBar->setValue(0);
	CTitleHashTree res;
	int flags=0;
	if (ui.searchArtist->isChecked()) flags|=CHashes::SearchArtist;
	if (ui.searchTitle->isChecked()) flags|=CHashes::SearchTitle;
	if (ui.searchVersion->isChecked()) flags|=CHashes::SearchVersion;
	if (ui.searchLabel->isChecked()) flags|=CHashes::SearchLabel;
	if (ui.searchGenre->isChecked()) flags|=CHashes::SearchGenre;
	if (ui.searchTags->isChecked()) flags|=CHashes::SearchTags;
	if (ui.searchRemarks->isChecked()) flags|=CHashes::SearchRemarks;
	if (ui.searchAlbum->isChecked()) flags|=CHashes::SearchAlbum;

	if (wm->Hashes.FindGlobal(Query,res,flags)) {
		Tmp.Setf("%u",res.Num());
		ui.numTracks->setText(Tmp);
	}
	ui.progressBar->setValue(30);
	ppl6::CGenericList list1;
	FilterResult(res,list1);
	LimitResult(list1,Results);

	/*
	DataTitle *ti;
	res.Reset();
	while ((ti=(DataTitle*)res.GetNext())) {
		if (ti->DeviceType<20 && AllowedDevices[ti->DeviceType]==1) {
			Results.Add(ti);
		}
	}
	*/
	ui.progressBar->setValue(60);
	PresentResults();
	ui.progressBar->setValue(80);
	trackList->sortByColumn(0,Qt::AscendingOrder);
	trackList->setSortingEnabled(true);
	ui.progressBar->setValue(100);
	ui.query->setFocus();
}

void Search::on_artist_returnPressed()
{
	DoSearch();
}

void Search::on_title_returnPressed()
{
	DoSearch();
}

void Search::on_version_returnPressed()
{
	DoSearch();
}

void Search::on_genre_returnPressed()
{
	DoSearch();
}

void Search::on_tags_returnPressed()
{
	DoSearch();
}

void Search::on_recordLabel_returnPressed()
{
	DoSearch();
}


void Search::on_query_returnPressed()
{
	on_quicksearchButton_clicked();
}

void Search::on_markAllButton_clicked()
{
	WMTreeItem * item;
	for (int i=0;i<trackList->topLevelItemCount ();i++) {
		item=(WMTreeItem *)trackList->topLevelItem (i);
		if (item) item->setSelected(true);
	}
	ui.artist->setFocus();
	ui.artist->selectAll();
}

void Search::on_markNoneButton_clicked()
{
	WMTreeItem * item;
	for (int i=0;i<trackList->topLevelItemCount ();i++) {
		item=(WMTreeItem *)trackList->topLevelItem (i);
		if (item) item->setSelected(false);
	}
	ui.artist->setFocus();
	ui.artist->selectAll();
}

void Search::on_trackList_customContextMenuRequested ( const QPoint & pos )
{
    QPoint p=ui.listWidget->mapToGlobal(pos);
    currentTrackListItem=(WMTreeItem*)trackList->itemAt(pos);
    //printf ("Custom Context %i\n",currentTrackListItem->Track);
    if (!currentTrackListItem) return;
    int DeviceType=0;
    DataTitle *t=NULL;
    t=wm->GetTitle(currentTrackListItem->Id);
    if (t) {
    	DeviceType=t->DeviceType;
    }


    QMenu *m=new QMenu(this);
    QAction *a=NULL;

    int column=trackList->currentColumn();
    if (column==SEARCH_TRACKLIST_RATING_ROW) {
    	a=m->addAction (QIcon(":/bewertung/resources/rating-0.png"),"0",this,SLOT(on_contextRate0_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-1.png"),"1",this,SLOT(on_contextRate1_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-2.png"),"2",this,SLOT(on_contextRate2_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-3.png"),"3",this,SLOT(on_contextRate3_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-4.png"),"4",this,SLOT(on_contextRate4_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-5.png"),"5",this,SLOT(on_contextRate5_clicked()));
    	m->addAction (QIcon(":/bewertung/resources/rating-6.png"),"6",this,SLOT(on_contextRate6_clicked()));
    } else if (column==SEARCH_TRACKLIST_KEY_ROW) {
       	if (t!=NULL && (t->Flags&16)==0) a=m->addAction (QIcon(":/icons/resources/musicKeyOk.png"),tr("Music Key is verified","trackList Context Menue"),this,SLOT(on_contextMusicKeyVerified_triggered()));
       	else if (t!=NULL && (t->Flags&16)==16) a=m->addAction (QIcon(":/icons/resources/musicKeyNotOk.png"),tr("Music Key is not verified","trackList Context Menue"),this,SLOT(on_contextMusicKeyVerified_triggered()));
    	QMenu *mk=m->addMenu ( QIcon(":/icons/resources/musicKey.png"),tr("Set Music-Key","trackList Context Menue") );
    	createSetMusicKeyContextMenu(mk);
    } else {
    	a=m->addAction (QIcon(":/icons/resources/frame_text.png"),tr("select all","trackList Context Menue"),this,SLOT(on_markAllButton_clicked()));
    	m->addAction (QIcon(":/icons/resources/eraser.png"),tr("select none","trackList Context Menue"),this,SLOT(on_markNoneButton_clicked()));

    	m->addSeparator();
    	m->addAction (QIcon(":/icons/resources/findmore.png"),tr("Find other versions","trackList Context Menue"),this,SLOT(on_contextFindMoreVersions_triggered()));
    	m->addAction (QIcon(":/icons/resources/findmore-artist.png"),tr("Find more of artist","trackList Context Menue"),this,SLOT(on_contextFindMoreArtist_triggered()));
    	m->addAction (QIcon(":/icons/resources/findmore-title.png"),tr("Find other artists of this title","trackList Context Menue"),this,SLOT(on_contextFindMoreTitle_triggered()));
    	if (t) m->addAction (wm->GetDeviceIcon(t->DeviceType),tr("All tracks on this medium","trackList Context Menue"),this,SLOT(on_contextFindMoreMedium_triggered()));
    	m->addSeparator();
    	if (DeviceType==7) m->addAction (QIcon(":/icons/resources/play.png"),tr("Play Track","trackList Context Menue"),this,SLOT(on_contextPlayTrack_triggered()));
    	m->addAction (QIcon(":/icons/resources/edit.png"),tr("Edit Track","trackList Context Menue"),this,SLOT(on_contextEditTrack_triggered()));
    	m->addAction (QIcon(":/icons/resources/copytrack.png"),tr("Copy Artist and Title","trackList Context Menue"),this,SLOT(on_contextCopyTrack_triggered()));
    	if (DeviceType==7) m->addAction (QIcon(":/icons/resources/copyfile.png"),tr("Copy MP3-File","trackList Context Menue"),this,SLOT(on_contextCopyFile_triggered()));
    }
    m->popup(p,a);
    ui.artist->setFocus();

}

void Search::createSetMusicKeyContextMenu(QMenu *m)
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


void Search::on_contextMusicKeyVerified_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;
	DataTitle tUpdate=*t;
	if (tUpdate.Flags&16) tUpdate.Flags-=16;
	else tUpdate.Flags|=16;
	if (!wm->TitleStore.Put(&tUpdate)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		return;
	}
	renderTrack(currentTrackListItem,&tUpdate);

}

void Search::on_contextSetMusicKey(int k)
{
	if (!currentTrackListItem) return;
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;

	DataTitle tUpdate=*t;
	tUpdate.Key=k;
	if (!wm->TitleStore.Put(&tUpdate)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		return;
	}
	renderTrack(currentTrackListItem,&tUpdate);
}



void Search::on_trackList_itemClicked ( QTreeWidgetItem * item,int  )
{
	Qt::KeyboardModifiers key=QApplication::keyboardModifiers ();
	DataTitle *t=wm->GetTitle(((WMTreeItem*)item)->Id);
	if (t) {
		QClipboard *clipboard = QApplication::clipboard();
		ppl6::CString Text;
		if (key&Qt::MetaModifier) {
			Text.Setf("%s %s",t->Artist,t->Title);
		} else {
			Text.Setf("%s - %s (%s, %0i:%02i min, %s)",t->Artist,t->Title,
					wm->GetVersionText(t->VersionId), t->Length/60,t->Length%60, wm->GetGenreText(t->GenreId));
			Text.Concatf(" [%s %u %c-%i]",(const char*)wm->GetDeviceNameShort(t->DeviceType),
					t->DeviceId,(t->Page+'A'-1),t->Track);
		}
		LastClipboardString=Text;
		clipboard->setText(Text,QClipboard::Clipboard);
		clipboard->setText(Text,QClipboard::Selection);
	}
	ui.artist->setFocus();

}

void Search::on_trackList_itemDoubleClicked ( QTreeWidgetItem * item,int )
{
	DataTitle *t=wm->GetTitle(((WMTreeItem*)item)->Id);
	if (t!=NULL && t->DeviceType==7) {
		ppl6::CString Path=wm->MP3Filename(t->DeviceId,t->Page,t->Track);
		if (Path.IsEmpty()) return;
		//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (char*)Path);
		wm->PlayFile(Path);
	}
	ui.artist->setFocus();
}


void Search::on_contextFindMoreMedium_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		CTrackList *l=wm->GetTracklist(t->DeviceType,t->DeviceId,t->Page);
		if (l) {
			ui.numTracks->setText("");
			currentTrackListItem=NULL;
			trackList->clear();
			trackList->setWordWrap(false);
			trackList->setSortingEnabled(false);
			Results.Clear();
			int min=l->GetMin();
			int max=l->GetMax();
			for (int i=min;i<=max;i++) {
				DataTrack *track=l->Get(i);
				t=wm->GetTitle(track->TitleId);
				if (t) Results.Add(t);
			}
			PresentResults();
			ui.progressBar->setValue(100);
			trackList->sortByColumn(0,Qt::AscendingOrder);
			trackList->setSortingEnabled(true);
			delete l;
		}
	}
	ui.artist->setFocus();
}


void Search::on_contextFindMoreVersions_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		FastSearch(t->Artist,t->Title);
	}
	ui.artist->setFocus();
}

void Search::on_contextFindMoreArtist_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		FastSearch(t->Artist,"");
	}
	ui.artist->setFocus();
}

void Search::on_contextFindMoreTitle_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		FastSearch("",t->Title);
	}
	ui.artist->setFocus();
}

void Search::on_contextPlayTrack_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t!=NULL && t->DeviceType==7) {
		ppl6::CString Path=wm->MP3Filename(t->DeviceId,t->Page,t->Track);
		if (Path.IsEmpty()) return;
		//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (char*)Path);
		wm->PlayFile(Path);
	}
	ui.artist->setFocus();
}

void Search::on_contextEditTrack_triggered()
{
	ui.artist->setFocus();
	if (!currentTrackListItem) return;
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;
	wm->OpenEditor(t->DeviceType, t->DeviceId, t->Page, t->Track);
}

void Search::on_contextCopyTrack_triggered()
{
	on_trackList_itemClicked(currentTrackListItem,0);
	ui.artist->setFocus();
}

void Search::on_contextCopyFile_triggered()
{
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (t!=NULL && t->DeviceType==7) {
		ppl6::CString Path=wm->MP3Filename(t->DeviceId,t->Page,t->Track);
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
	ui.artist->setFocus();
}

void Search::rateCurrentTrack(int value)
{
	if (!currentTrackListItem) return;
	DataTitle *t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;
	if (value==t->Rating) return;
	DataTitle tUpdate=*t;
	tUpdate.Rating=value;
	if (!wm->TitleStore.Put(&tUpdate)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		return;
	}
	renderTrack(currentTrackListItem,&tUpdate);
}

void Search::on_contextRate0_clicked()
{
	rateCurrentTrack(0);
}

void Search::on_contextRate1_clicked()
{
	rateCurrentTrack(1);
}

void Search::on_contextRate2_clicked()
{
	rateCurrentTrack(2);
}

void Search::on_contextRate3_clicked()
{
	rateCurrentTrack(3);
}

void Search::on_contextRate4_clicked()
{
	rateCurrentTrack(4);
}

void Search::on_contextRate5_clicked()
{
	rateCurrentTrack(5);
}

void Search::on_contextRate6_clicked()
{
	rateCurrentTrack(6);
}


bool Search::on_trackList_MousePress(QMouseEvent * event)
{
	if (event->buttons() == Qt::LeftButton) startPos=event->pos();
	return false;
}

bool Search::on_trackList_MouseRelease(QMouseEvent * )
{
	startPos.setX(0);
	startPos.setY(0);
	return false;
}

bool Search::on_trackList_MouseMove(QMouseEvent *event)
{
	if (!(event->buttons() == Qt::LeftButton)) {
		//QTreeWidget::mouseMoveEvent(event);
		return false;
	}
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
	ppl6::CString xml;
	xml="<winmusikTracklist>\n";
	xml+="<tracks>\n";
	QPixmap Icon;
	int count=0;
	for (int i=0;i<Items.size();i++) {
		item=(WMTreeItem *)Items[i];
		DataTitle *t=wm->GetTitle(item->Id);
		xml+="<item>\n";
		xml+=wm->getXmlTitle(item->Id);
		if (t!=NULL && t->DeviceType==7) {
			if (Icon.isNull()) {
				if (t->CoverPreview.Size()>0) {
					Icon.loadFromData((const uchar*)t->CoverPreview.GetPtr(),t->CoverPreview.GetSize());
				}
			}

			File=wm->MP3Filename(t->DeviceId,t->Page,t->Track);
			if (File.NotEmpty()) {
				xml+="<File>"+ppl6::EscapeHTMLTags(File)+"</File>";
#ifdef _WIN32
				if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,10,"Search","on_trackList_MouseMove",__FILE__,__LINE__,"Add File to Drag: %s",(const char*)File);
				list.append(QUrl::fromLocalFile(File));
#else
				list.append(QUrl::fromLocalFile(File));
#endif
				count++;
			}
		}
		xml+="</item>";
	}
	xml+="</tracks>\n";
	xml+="</winmusikTracklist>\n";

	if (!count) return false;
    QDrag *drag = new QDrag(trackList);
    QMimeData *mimeData = new QMimeData;
    if (Icon.isNull()) Icon.load(":/devices48/resources/tr48x48-0007.png");
    drag->setPixmap(Icon);
    mimeData->setText(xml);
    mimeData->setUrls(list);
    drag->setMimeData(mimeData);
    // start drag
    drag->start(Qt::CopyAction | Qt::MoveAction);
	startPos.setX(0);
	startPos.setY(0);

	return false;
}


void Search::on_watchClipboard_toggled (bool checked)
{
	//printf ("Toggled: %i\n",checked);
	if (!checked) {
		ClipBoardTimer.stop();

		return;
	}
	ClipBoardTimer.start(200);
}

void Search::on_watchClipboardHarddisk_toggled (bool checked)
{
	//printf ("Toggled: %i\n",checked);
	if (!checked) {
		ClipBoardTimer.stop();
		return;
	}
	ClipBoardTimer.start(200);
}

void Search::on_ClipBoardTimer_update()
{
	RegExpClipboard clip;
	clip.copyFromClipboard();
	if (clip.PlainText==LastClipboardString) return;
	LastClipboardString=clip.PlainText;
	RegExpMatch match;
	ppl6::CString s;
	if (wm->RegExpCapture.match(clip,match)) {
		s=match.Artist+" "+match.Title;
		//printf ("RegExpMatch: %s\n",(const char*)s);
	} else {
		s=clip.PlainText;
		if (s.PregMatch("/^.*? - .*? \\(.*?,.*?,.*?\\).*$/")) return;
		if (s.Instr("\n")>=0) return;
		s.Replace("\t"," ");
		s.PregReplace("/\\(.*?\\)/","");
		//printf ("NO RegExpMatch: %s\n",(const char*)s);
	}
	wm->NormalizeTerm(s);
	ClipBoardTimer.stop();
	//printf ("Update\n");

	if (ui.watchClipboardHarddisk->isChecked()) {
		ui.tabWidget->setCurrentIndex(2);
		ui.query_harddisk->setText(s);
		on_hardDiskSearchButton_clicked();

	}
	if (ui.watchClipboard->isChecked()) {
		ui.tabWidget->setCurrentIndex(1);
		ui.query->setText(s);
		on_quicksearchButton_clicked();
	}
	ClipBoardTimer.start(200);

}

void Search::on_hardDiskSearchButton_clicked()
{
	if (wm->conf.DirectorySearch.Num()==0) return;
	ppl6::CString Query=ui.query_harddisk->text();
	wm->NormalizeTerm(Query);

	QTreeWidgetItem *item=new QTreeWidgetItem;

	trackList->clear();
	item->setText(0,tr("Pos"));
	item->setText(1,tr("Filename"));
	item->setText(2,tr("Size"));

	trackList->setHeaderItem(item);
	resultmode=1;
    Resize();
    trackList->update();

	if (Query.IsEmpty()) return;
	ppl6::CArray Words;
	Words.Explode(Query," ");

	ui.numTracks->setText("");
	currentTrackListItem=NULL;

	trackList->setWordWrap(false);
	trackList->setSortingEnabled(false);
	Results.Clear();
	ppl6::CArray DirList;
	DirList.Add(wm->conf.DirectorySearch);
	DirList.Reset();
	const char *tmp;
	while ((tmp=DirList.GetNext())) {
		RecursiveDirSearch(Words,tmp);
	}
	ui.progressBar->setValue(80);
	trackList->sortByColumn(0,Qt::AscendingOrder);
	trackList->setSortingEnabled(true);
	ui.progressBar->setValue(100);
	ui.query_harddisk->setFocus();

}

void Search::RecursiveDirSearch(ppl6::CArray &search, const ppl6::CString &dir)
{
	ppl6::CDir Dir;
	ppl6::CString File,Tmp;
	const char *tmp;
	if (Dir.Open(dir,ppl6::CDir::Sort_None)) {
		ppl6::CDirEntry *entry;
		Dir.Reset();
		while ((entry=Dir.GetNext())) {
			if (entry->IsDir()==true) {
				if (entry->Filename!="." && entry->Filename!="..")
					RecursiveDirSearch(search,entry->File);
			} else if (entry->IsFile()) {
				File=entry->Filename;
				wm->NormalizeTerm(File);
				// Wir prüfen, ob jedes Suchwort im Filenamen auftaucht
				search.Reset();
				bool match=true;
				while ((tmp=search.GetNext())) {
					if (File.Instr(tmp)<0) {
						match=false;
						break;
					}
				}
				if (match) {
					//printf ("Match: %s\n",entry->File.GetPtr());
					WMTreeItem *item=new WMTreeItem;
					item->Id=0;
					Tmp.Setf("%5u",0);
					item->setText(0,Tmp);
					item->setText(1,entry->File);
					Tmp.Setf("%u MB",(ppluint32)(entry->Size/1024/1024));
					item->setText(2,Tmp);
					trackList->addTopLevelItem(item);

				}
			}
		}
	}

}

void Search::on_displayMusicKey_currentIndexChanged(int)
{
	switch (ui.displayMusicKey->currentIndex()) {
		case 0: musicKeyDisplay=musicKeyTypeMusicalSharps; break;
		case 1: musicKeyDisplay=musicKeyTypeOpenKey; break;
		case 2: musicKeyDisplay=musicKeyTypeCustom; break;
		default: musicKeyDisplay=musicKeyTypeOpenKey; break;
	}
	updateTrackListing();
}
