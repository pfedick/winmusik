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



#include <QClipboard>
#include <QMenu>
#include <QList>
#include <QUrl>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include "search.h"
#include "resultfilter.h"
#include <vector>
#include "ppl7-audio.h"

Search::Search(QWidget* parent, CWmClient* wm)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	wm->RegisterWindow(WindowType::Search, this);
	resultmode=0;
	ClipBoardTimer.setParent(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	SetupTrackList();
	ui.tabWidget->setCurrentIndex(0);
	ui.query->setFocus(Qt::ActiveWindowFocusReason);
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
		case musicKeyTypeMusicalFlats: ui.displayMusicKey->setCurrentIndex(1); break;
		case musicKeyTypeOpenKey: ui.displayMusicKey->setCurrentIndex(2); break;
		case musicKeyTypeCustom: ui.displayMusicKey->setCurrentIndex(3); break;
		default: ui.displayMusicKey->setCurrentIndex(3); break;
	}
	ui.displayMusicKey->setItemText(3, wm->conf.customMusicKeyName);
	for (int i=1;i <= 25;i++) {
		ui.keywheel->setKeyName(i, wm_main->MusicKeys.keyName(i, musicKeyDisplay));
	}
	on_setThisYear_clicked();
	on_setRecordingDate0_clicked();
	ui.filter->setVisible(false);
	ui.useFilter->setChecked(false);

	ui.bpmStart->installEventFilter(this);
	ui.bpmEnd->installEventFilter(this);
	ui.qs_genre->installEventFilter(this);
	ui.lengthStart->installEventFilter(this);
	ui.lengthEnd->installEventFilter(this);
	ui.energyStart->installEventFilter(this);
	ui.energyEnd->installEventFilter(this);
	ui.releaseDateStart->installEventFilter(this);
	ui.releaseDateEnd->installEventFilter(this);
	ui.recordDateStart->installEventFilter(this);
	ui.recordDateEnd->installEventFilter(this);


	ui.tabWidget->setAcceptDrops(true);
	ui.tabWidget->installEventFilter(this);
	ui.query->installEventFilter(this);
	ui.queryLayout->installEventFilter(this);
	//ui.tabWidget->setDropIndicatorShown(true);

	//update();
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
		wm->conf.trySave();
		wm->UnRegisterWindow(WindowType::Search, this);
	}
}

void Search::customEvent(QEvent* event)
{
	if (event->type() == (QEvent::Type)WinMusikEvent::retranslateUi) {
		ui.retranslateUi(this);
		DefaultTracklistHeader();
		event->accept();
	}
}

void Search::show()
{
	SetWindowGeometry(this, "search");
	QWidget::show();
}


void Search::Resize()
{
	int w=trackList->width();
	trackList->setColumnWidth(SEARCH_TRACKLIST_POS_ROW, 40);
	if (resultmode == 0) {
		trackList->setColumnWidth(SEARCH_TRACKLIST_GENRE_ROW, 50);
		trackList->setColumnWidth(SEARCH_TRACKLIST_BPM_ROW, 40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_KEY_ROW, 40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_ENERGYLEVEL_ROW, 30);
		trackList->setColumnWidth(SEARCH_TRACKLIST_YEAR_ROW, 40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_RATING_ROW, 40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_BITRATE_ROW, 40);
		trackList->setColumnWidth(SEARCH_TRACKLIST_SOURCE_ROW, 120);

		w=w - 40 - 50 - 40 - 40 - 30 - 40 - 40 - 40 - 120;

		trackList->setColumnWidth(SEARCH_TRACKLIST_ARTIST_ROW, w * 35 / 100);
		trackList->setColumnWidth(SEARCH_TRACKLIST_TITLE_ROW, w * 30 / 100);
		trackList->setColumnWidth(SEARCH_TRACKLIST_VERSION_ROW, w * 25 / 100);
		trackList->setColumnWidth(SEARCH_TRACKLIST_LENGTH_ROW, w * 10 / 100);


	} else {
		//printf ("resultmpode=%i\n",resultmode);
		trackList->setColumnWidth(1, w - 100);
		trackList->setColumnWidth(2, 50);
	}

}

void Search::showEvent(QShowEvent* event)
{
	Resize();
	ui.query->setFocus();
	QWidget::showEvent(event);
}
void Search::resizeEvent(QResizeEvent* event)
{
	Resize();
	QWidget::resizeEvent(event);
}

void Search::closeEvent(QCloseEvent* event)
{
	SaveWindowGeometry(this, "search");
	QWidget::closeEvent(event);
}

bool Search::eventFilter(QObject* target, QEvent* event)
/*!\brief Event Handler
 */
{
	int type=event->type();
	if (type == QEvent::FocusIn) {
		if (target == ui.lengthStart || target == ui.lengthEnd
			|| target == ui.qs_genre) {
			((QLineEdit*)target)->selectAll();
		} else if (target == ui.energyStart || target == ui.energyEnd
			|| target == ui.bpmStart || target == ui.bpmEnd) {
			((QSpinBox*)target)->selectAll();
		} else if (target == ui.releaseDateStart || target == ui.releaseDateEnd
			|| target == ui.recordDateStart || target == ui.recordDateEnd) {
			((QDateEdit*)target)->selectAll();
		}
	} else if (type == QEvent::DragEnter) {
		if (target == ui.tabWidget
			|| target == ui.query
			|| target == ui.queryLayout) {
			QDragEnterEvent* e=static_cast<QDragEnterEvent*>(event);
			//printf("QEvent::DragEnter\n");
			//fflush(stdout);
			e->accept();
			return true;
		}
	} else if (type == QEvent::Drop) {
		if (target == ui.tabWidget
			|| target == ui.query
			|| target == ui.queryLayout) {
			QDropEvent* e=static_cast<QDragEnterEvent*>(event);
			//printf("QEvent::Drop\n");
			//fflush(stdout);
			handleDropEvent(e);
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
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
	QLayout* w=ui.listWidget->layout();
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


	connect(trackList, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(on_trackList_customContextMenuRequested(const QPoint&)));
	connect(trackList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(on_trackList_itemDoubleClicked(QTreeWidgetItem*, int)));
	connect(trackList, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
		this, SLOT(on_trackList_itemClicked(QTreeWidgetItem*, int)));

	Resize();
	trackList->update();
}

void Search::DefaultTracklistHeader()
{
	trackList->headerItem()->setText(SEARCH_TRACKLIST_POS_ROW, tr("Pos", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_ARTIST_ROW, tr("Artist", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_TITLE_ROW, tr("Title", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_VERSION_ROW, tr("Version", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_GENRE_ROW, tr("Genre", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_LENGTH_ROW, tr("Length", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_BPM_ROW, tr("BPM", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_KEY_ROW, tr("Key", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_ENERGYLEVEL_ROW, tr("Energy", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_YEAR_ROW, tr("Year", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_SOURCE_ROW, tr("Medium", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_RATING_ROW, tr("Rating", "trackList"));
	trackList->headerItem()->setText(SEARCH_TRACKLIST_BITRATE_ROW, tr("KBit", "trackList"));

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

void Search::FastSearch(const ppl7::String& Artist, const ppl7::String& Title, const ppl7::String& Version, const ppl7::String& Genre, const ppl7::String& Tags, const ppl7::String& Label)
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
	Results.clear();
	CheckAllowedDevices();

	ppl7::String Tmp;
	ui.progressBar->setValue(0);
	CHashes::TitleTree res;
	if (wm->Hashes.Find(Artist, Title, Version, Genre, Tags, Label, res)) {
		Tmp.setf("%zu", res.size());
		ui.numTracks->setText(Tmp);
	}
	ui.progressBar->setValue(30);

	std::list<const DataTitle*> list, sorted_list;
	FilterResult(res, list);
	LimitResult(list, sorted_list);
	SortResult(sorted_list, Results);

	ui.progressBar->setValue(60);
	PresentResults();
	ui.progressBar->setValue(80);
	trackList->sortByColumn(0, Qt::AscendingOrder);
	trackList->setSortingEnabled(true);
	ui.progressBar->setValue(100);
	ui.artist->setFocus();

}

void Search::FastSearch(const ppl7::String& Words)
{
	ui.tabWidget->setCurrentIndex(0);
	ui.query->setText(Words);
	on_quicksearchButton_clicked();
}


void Search::CheckAllowedDevices()
{
	for (int i=0;i < 20;i++) AllowedDevices[i]=0;
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
	for (int i=0;i < 20;i++) if (AllowedDevices[i]) c++;
	if (c == 0) {	// Nix ausgewählt
		for (int i=0;i < 20;i++) AllowedDevices[i]=1;
	}
}

void Search::DoSearch()
{
	ppl7::String Artist=ui.artist->text();
	ppl7::String Title=ui.title->text();
	ppl7::String Version=ui.version->text();
	ppl7::String Genre=ui.genre->text();
	ppl7::String Tags=ui.tags->text();
	ppl7::String Label=ui.recordLabel->text();
	Artist=ppl7::Trim(ppl7::LowerCase(Artist));
	Title=ppl7::Trim(ppl7::LowerCase(Title));
	Version=ppl7::Trim(ppl7::LowerCase(Version));
	Genre=ppl7::Trim(ppl7::LowerCase(Genre));
	Tags=ppl7::Trim(ppl7::LowerCase(Tags));
	Label=ppl7::Trim(ppl7::LowerCase(Label));

	FastSearch(Artist, Title, Version, Genre, Tags, Label);
	ui.artist->setFocus();
	return;
}


void Search::FilterResult(const CHashes::TitleTree& in, std::list<const DataTitle*>& out)
{
	CHashes::TitleTree::const_iterator it;
	const DataTitle* ti;
	for (it=in.begin();it != in.end();it++) {
		ti=wm->GetTitle(*it);
		if (ti->DeviceType < 20 && AllowedDevices[ti->DeviceType] == 1) {
			out.push_back(ti);
		}
	}
}

void Search::SortResult(const std::list<const DataTitle*>& in, std::list<const DataTitle*>& out)
{
	out.clear();
	std::map<ppl7::String, const DataTitle*> sorter;
	std::list<const DataTitle*>::const_iterator it;
	for (it=in.begin();it != in.end();++it) {
		const DataTitle* ti=(*it);
		sorter.insert(std::pair<ppl7::String, const DataTitle*>(
			ppl7::ToString("%08u:%08u", ti->ReleaseDate, ti->TitleId),
			ti));
	}
	std::map<ppl7::String, const DataTitle*>::const_reverse_iterator it_sorted;
	for (it_sorted=sorter.rbegin();it_sorted != sorter.rend();++it_sorted) {
		out.push_back(it_sorted->second);
	}
}

void Search::RandomResult(const std::list<const DataTitle*>& in, std::list<const DataTitle*>& out, size_t num)
{
	// Zuerst kopieren wir die Eingangsliste zum besseren Zugriff in einen vector
	std::vector<const DataTitle*> list, list2;
	size_t r;
	std::list<const DataTitle*>::const_iterator it;
	for (it=in.begin();it != in.end();++it) {
		list.push_back((*it));
	}
	if (num > list.size()) num=list.size();
	while (num) {
		num--;
		const DataTitle* ti=NULL;
		while (!ti) {
			r=ppl7::rand(0, list.size() - 1);
			ti=list[r];
			if (!ti) {
				// vector muss aufgeräumt werden
				list2.clear();
				for (size_t i=0;i < list.size();i++) {
					ti=list[i];
					if (ti) list2.push_back(ti);
				}
				if (!list2.size()) return;
				list=list2;
				ti=NULL;
			}
		}
		list[r]=NULL;
		out.push_back(ti);
	}
}

void Search::LimitResult(const std::list<const DataTitle*>& in, std::list<const DataTitle*>& out)
{
	size_t limit=ui.limitResultSpinBox->value();
	if (ui.randomResultCheckBox->isChecked()) {
		// Wir wollen Zufall
		if (limit == 0 || limit >= in.size()) limit=in.size();
		RandomResult(in, out, limit);
		return;
	}
	if (limit == 0 || limit >= in.size()) {
		std::list<const DataTitle*>::const_reverse_iterator it;
		for (it=in.rbegin(); it != in.rend();++it) {
			out.push_back((*it));
		}
	} else {
		std::list<const DataTitle*>::const_iterator it;
		size_t i;
		for (i=0, it=in.begin();(i < limit) && (it != in.end());i++, ++it) {
			out.push_back((*it));
		}
	}
}

void Search::updateTrackListing()
{
	for (int i=0;i < trackList->topLevelItemCount();i++) {
		WMTreeItem* item=(WMTreeItem*)trackList->topLevelItem(i);
		if (item != NULL && item->Id > 0) {
			const DataTitle* ti=wm->GetTitle(item->Id);
			if (ti) renderTrack(item, ti);
		}
	}
}


void Search::renderTrack(WMTreeItem* item, const DataTitle* ti)
{
	const char Seite[]="?ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	ppl7::String Tmp;
	QPixmap icon;
	QBrush Brush(Qt::SolidPattern);
	Brush.setColor("red");


	Tmp.setf("%5u", item->Track);
	item->setText(SEARCH_TRACKLIST_POS_ROW, Tmp);

	if (ti->CoverPreview.size()) {
		icon.loadFromData((const uchar*)ti->CoverPreview.ptr(), ti->CoverPreview.size());
		item->setIcon(SEARCH_TRACKLIST_ARTIST_ROW, icon.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	} else {
		item->setIcon(SEARCH_TRACKLIST_ARTIST_ROW, QIcon(":/icons/resources/cover16.png"));
	}
	Tmp=ti->Artist;
	item->setText(SEARCH_TRACKLIST_ARTIST_ROW, Tmp);
	Tmp=ti->Title;
	item->setText(SEARCH_TRACKLIST_TITLE_ROW, Tmp);
	item->setForeground(SEARCH_TRACKLIST_VERSION_ROW, Brush);
	Tmp=wm->GetVersionText(ti->VersionId);
	item->setText(SEARCH_TRACKLIST_VERSION_ROW, Tmp);
	Tmp=wm->GetGenreText(ti->GenreId);
	item->setText(SEARCH_TRACKLIST_GENRE_ROW, Tmp);
	Tmp.setf("%4i:%02i", (int)(ti->Length / 60), ti->Length % 60);
	item->setText(SEARCH_TRACKLIST_LENGTH_ROW, Tmp);

	// BPM, Key und EnergyLevel
	Tmp.setf("%d", (int)ti->BPM);
	item->setText(SEARCH_TRACKLIST_BPM_ROW, Tmp);
	item->setText(SEARCH_TRACKLIST_KEY_ROW, wm_main->MusicKeys.keyName(ti->Key, musicKeyDisplay));

	if ((ti->Flags & 16)) item->setForeground(SEARCH_TRACKLIST_KEY_ROW, QColor(0, 0, 0));
	else item->setForeground(SEARCH_TRACKLIST_KEY_ROW, QColor(192, 192, 192));
	Tmp.setf("%d", (int)ti->EnergyLevel);
	item->setText(SEARCH_TRACKLIST_ENERGYLEVEL_ROW, Tmp);

	// Year
	Tmp.setf("%d", ti->ReleaseDate / 10000);
	item->setText(SEARCH_TRACKLIST_YEAR_ROW, Tmp);

	// Tonträger
	Tmp.setf("%u %c-%03u", ti->DeviceId, Seite[(ti->Page < 10 ? ti->Page : 0)], ti->Track);
	item->setText(SEARCH_TRACKLIST_SOURCE_ROW, Tmp);
	QIcon Icon;
	Tmp.setf(":/devices16/resources/tr16x16-%04i.png", ti->DeviceType);
	Icon.addFile(Tmp);
	item->setIcon(SEARCH_TRACKLIST_SOURCE_ROW, Icon);

	// Bitrate
	Tmp.setf("%d", ti->Bitrate);
	item->setText(SEARCH_TRACKLIST_BITRATE_ROW, Tmp);

	// Bewertung
	switch (ti->Rating) {
		case 0: item->setIcon(SEARCH_TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/rating-0.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW, "0");
			break;
		case 1: item->setIcon(SEARCH_TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/rating-1.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW, "1");
			break;
		case 2: item->setIcon(SEARCH_TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/rating-2.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW, "2");
			break;
		case 3: item->setIcon(SEARCH_TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/rating-3.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW, "3");
			break;
		case 4: item->setIcon(SEARCH_TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/rating-4.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW, "4");
			break;
		case 5: item->setIcon(SEARCH_TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/rating-5.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW, "5");
			break;
		case 6: item->setIcon(SEARCH_TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/rating-6.png"));
			item->setText(SEARCH_TRACKLIST_RATING_ROW, "6");
			break;
	}
}

void Search::PresentResults()
{


	ppl7::String Tmp;
	Tmp.setf("%zu", Results.size());
	ui.numTracks->setText(Tmp);
	ui.totalLength->setText("");
	resultmode=0;
	DefaultTracklistHeader();
	Resize();
	trackList->update();

	uint64_t totalLength=0;

	std::list<const DataTitle*>::const_iterator it;
	uint32_t i=0;
	for (it=Results.begin();it != Results.end();++it) {
		const DataTitle* ti=(*it);
		i++;
		WMTreeItem* item=new WMTreeItem;
		item->Track=i;
		item->Id=ti->TitleId;

		renderTrack(item, ti);
		totalLength+=ti->Length;
		trackList->addTopLevelItem(item);
	}
	int h=(int)(totalLength / 3600);
	int m=totalLength - (h * 3600);
	int s=m % 60;
	m=m / 60;
	Tmp.setf("%0i:%02i:%02i", h, m, s);
	ui.totalLength->setText(Tmp);


}

void Search::configureFilter(ResultFilter& filter)
{
	if (!ui.useFilter->isChecked()) return;

	if (ui.enableGenreSearch->isChecked())
		filter.setGenres(true, ui.qs_genre->text());
	if (ui.enableBpmSearch->isChecked())
		filter.setBpmRange(true, ui.bpmStart->value(), ui.bpmEnd->value());
	if (ui.enableYearSearch->isChecked()) {
		filter.setYearRange(true, ui.releaseDateStart->date().year(), ui.releaseDateEnd->date().year());
	}
	if (ui.enableRecordingDateSearch->isChecked()) {
		QDate Date=ui.recordDateStart->date();
		ppl7::String Tmp=Date.toString("yyyyMMdd");
		uint32_t start=Tmp.toInt();
		Date=ui.recordDateEnd->date();
		Tmp=Date.toString("yyyyMMdd");
		uint32_t end=Tmp.toInt();
		filter.setRecordingRange(true, start, end);
	}
	if (ui.enableRatingSearch->isChecked()) {
		filter.setRatingRange(true, ui.ratingStart->currentIndex(), ui.ratingEnd->currentIndex());
	}
	if (ui.enableLengthSearch->isChecked()) {
		filter.setLengthRange(true, Time2Int(ui.lengthStart->text()), Time2Int(ui.lengthEnd->text()));
	}
	if (ui.enableEnergySearch->isChecked()) {
		filter.setEnergyRange(true, ui.energyStart->value(), ui.energyEnd->value());
	}
	if (ui.keywheel->currentKey() > 0) filter.setMusicKey(true, ui.keywheel->currentKey());
	filter.setTracksWithFilesOnly(ui.tracksWithFilesOnly->isChecked());
	filter.setTracksWithCoverOnly(ui.tracksWithCoverOnly->isChecked());
}

void Search::on_searchButton_clicked()
{
	DoSearch();
	ui.artist->setFocus();
}

void Search::on_quicksearchButton_clicked()
{
	ppl7::String Query=ui.query->text();
	Query.trim();
	ui.query->setFocus();


	ResultFilter filter;
	configureFilter(filter);

	if (Query.isEmpty()) {
		if (!ui.useFilter->isChecked()) return;
		if (!filter.allowSearchWithoutQuery()) return;
	}

	ui.numTracks->setText("");
	currentTrackListItem=NULL;
	trackList->clear();
	trackList->setWordWrap(false);
	trackList->setSortingEnabled(false);
	Results.clear();
	CheckAllowedDevices();
	ppl7::String Tmp;
	ui.progressBar->setValue(0);




	CHashes::TitleTree res;
	int flags=0;
	if (ui.searchArtist->isChecked()) flags|=CHashes::SearchArtist;
	if (ui.searchTitle->isChecked()) flags|=CHashes::SearchTitle;
	if (ui.searchVersion->isChecked()) flags|=CHashes::SearchVersion;
	if (ui.searchLabel->isChecked()) flags|=CHashes::SearchLabel;
	//if (ui.searchGenre->isChecked()) flags|=CHashes::SearchGenre;
	if (ui.searchTags->isChecked()) flags|=CHashes::SearchTags;
	if (ui.searchRemarks->isChecked()) flags|=CHashes::SearchRemarks;
	if (ui.searchAlbum->isChecked()) flags|=CHashes::SearchAlbum;

	if (wm->Hashes.FindGlobal(Query, res, flags, filter)) {
		Tmp.setf("%zu", res.size());
		ui.numTracks->setText(Tmp);
	}
	ui.progressBar->setValue(30);
	std::list<const DataTitle*> list1, sorted_list1;
	FilterResult(res, list1);
	LimitResult(list1, sorted_list1);
	SortResult(sorted_list1, Results);

	ui.progressBar->setValue(60);
	PresentResults();
	ui.progressBar->setValue(80);
	trackList->sortByColumn(0, Qt::AscendingOrder);
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

void Search::on_qs_genre_returnPressed()
{
	on_quicksearchButton_clicked();
}

void Search::on_markAllButton_clicked()
{
	WMTreeItem* item;
	for (int i=0;i < trackList->topLevelItemCount();i++) {
		item=(WMTreeItem*)trackList->topLevelItem(i);
		if (item) item->setSelected(true);
	}
	ui.artist->setFocus();
	ui.artist->selectAll();
}

void Search::on_markNoneButton_clicked()
{
	WMTreeItem* item;
	for (int i=0;i < trackList->topLevelItemCount();i++) {
		item=(WMTreeItem*)trackList->topLevelItem(i);
		if (item) item->setSelected(false);
	}
	ui.artist->setFocus();
	ui.artist->selectAll();
}

void Search::on_trackList_customContextMenuRequested(const QPoint& pos)
{
	QPoint p=ui.listWidget->mapToGlobal(pos);
	currentTrackListItem=(WMTreeItem*)trackList->itemAt(pos);
	//printf ("Custom Context %i\n",currentTrackListItem->Track);
	if (!currentTrackListItem) return;
	int DeviceType=0;
	const DataTitle* t=NULL;
	t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		DeviceType=t->DeviceType;
	}


	QMenu* m=new QMenu(this);
	QAction* a=NULL;

	int column=trackList->currentColumn();
	if (column == SEARCH_TRACKLIST_RATING_ROW) {
		a=m->addAction(QIcon(":/bewertung/resources/rating-0.png"), "0", this, SLOT(on_contextRate0_clicked()));
		m->addAction(QIcon(":/bewertung/resources/rating-1.png"), "1", this, SLOT(on_contextRate1_clicked()));
		m->addAction(QIcon(":/bewertung/resources/rating-2.png"), "2", this, SLOT(on_contextRate2_clicked()));
		m->addAction(QIcon(":/bewertung/resources/rating-3.png"), "3", this, SLOT(on_contextRate3_clicked()));
		m->addAction(QIcon(":/bewertung/resources/rating-4.png"), "4", this, SLOT(on_contextRate4_clicked()));
		m->addAction(QIcon(":/bewertung/resources/rating-5.png"), "5", this, SLOT(on_contextRate5_clicked()));
		m->addAction(QIcon(":/bewertung/resources/rating-6.png"), "6", this, SLOT(on_contextRate6_clicked()));
	} else if (column == SEARCH_TRACKLIST_KEY_ROW) {
		if (t != NULL && (t->Flags & 16) == 0) a=m->addAction(QIcon(":/icons/resources/musicKeyOk.png"), tr("Music Key is verified", "trackList Context Menue"), this, SLOT(on_contextMusicKeyVerified_triggered()));
		else if (t != NULL && (t->Flags & 16) == 16) a=m->addAction(QIcon(":/icons/resources/musicKeyNotOk.png"), tr("Music Key is not verified", "trackList Context Menue"), this, SLOT(on_contextMusicKeyVerified_triggered()));
		QMenu* mk=m->addMenu(QIcon(":/icons/resources/musicKey.png"), tr("Set Music-Key", "trackList Context Menue"));
		createSetMusicKeyContextMenu(mk);
	} else {
		a=m->addAction(QIcon(":/icons/resources/frame_text.png"), tr("select all", "trackList Context Menue"), this, SLOT(on_markAllButton_clicked()));
		m->addAction(QIcon(":/icons/resources/eraser.png"), tr("select none", "trackList Context Menue"), this, SLOT(on_markNoneButton_clicked()));

		m->addSeparator();
		m->addAction(QIcon(":/icons/resources/findmore.png"), tr("Find other versions", "trackList Context Menue"), this, SLOT(on_contextFindMoreVersions_triggered()));
		m->addAction(QIcon(":/icons/resources/findmore-artist.png"), tr("Find more of artist", "trackList Context Menue"), this, SLOT(on_contextFindMoreArtist_triggered()));
		m->addAction(QIcon(":/icons/resources/findmore-title.png"), tr("Find other artists of this title", "trackList Context Menue"), this, SLOT(on_contextFindMoreTitle_triggered()));
		if (t) m->addAction(wm->GetDeviceIcon(t->DeviceType), tr("All tracks on this medium", "trackList Context Menue"), this, SLOT(on_contextFindMoreMedium_triggered()));
		m->addSeparator();
		if (DeviceType == 7) m->addAction(QIcon(":/icons/resources/play.png"), tr("Play Track", "trackList Context Menue"), this, SLOT(on_contextPlayTrack_triggered()));
		m->addAction(QIcon(":/icons/resources/edit.png"), tr("Edit Track", "trackList Context Menue"), this, SLOT(on_contextEditTrack_triggered()));
		m->addAction(QIcon(":/icons/resources/copytrack.png"), tr("Copy Artist and Title", "trackList Context Menue"), this, SLOT(on_contextCopyTrack_triggered()));
		m->addAction(QIcon(":/icons/resources/copycover.png"), tr("Copy Cover", "trackList Context Menue"), this, SLOT(on_contextCopyCover_triggered()));
		if (DeviceType == 7) m->addAction(QIcon(":/icons/resources/copyfile.png"), tr("Copy MP3-File", "trackList Context Menue"), this, SLOT(on_contextCopyFile_triggered()));
	}
	m->popup(p, a);
	ui.artist->setFocus();

}

void Search::createSetMusicKeyContextMenu(QMenu* m)
{
	de::pfp::winmusik::MusicalKeys& keys=wm_main->MusicKeys;
	m->addAction(tr("unknown", "trackList Context Menue"), this, SLOT(on_contextMusicKey0_triggered()));
	m->addAction(keys.keyName(22, musicKeyDisplay), this, SLOT(on_contextMusicKey22_triggered()));
	m->addAction(keys.keyName(12, musicKeyDisplay), this, SLOT(on_contextMusicKey12_triggered()));
	m->addAction(keys.keyName(5, musicKeyDisplay), this, SLOT(on_contextMusicKey5_triggered()));
	m->addAction(keys.keyName(15, musicKeyDisplay), this, SLOT(on_contextMusicKey15_triggered()));
	m->addAction(keys.keyName(2, musicKeyDisplay), this, SLOT(on_contextMusicKey2_triggered()));
	m->addAction(keys.keyName(19, musicKeyDisplay), this, SLOT(on_contextMusicKey19_triggered()));
	m->addAction(keys.keyName(16, musicKeyDisplay), this, SLOT(on_contextMusicKey16_triggered()));
	m->addAction(keys.keyName(6, musicKeyDisplay), this, SLOT(on_contextMusicKey6_triggered()));
	m->addAction(keys.keyName(23, musicKeyDisplay), this, SLOT(on_contextMusicKey23_triggered()));
	m->addAction(keys.keyName(9, musicKeyDisplay), this, SLOT(on_contextMusicKey9_triggered()));
	m->addAction(keys.keyName(20, musicKeyDisplay), this, SLOT(on_contextMusicKey20_triggered()));
	m->addAction(keys.keyName(10, musicKeyDisplay), this, SLOT(on_contextMusicKey10_triggered()));
	m->addAction(keys.keyName(3, musicKeyDisplay), this, SLOT(on_contextMusicKey3_triggered()));
	m->addAction(keys.keyName(13, musicKeyDisplay), this, SLOT(on_contextMusicKey13_triggered()));
	m->addAction(keys.keyName(24, musicKeyDisplay), this, SLOT(on_contextMusicKey24_triggered()));
	m->addAction(keys.keyName(17, musicKeyDisplay), this, SLOT(on_contextMusicKey17_triggered()));
	m->addAction(keys.keyName(14, musicKeyDisplay), this, SLOT(on_contextMusicKey14_triggered()));
	m->addAction(keys.keyName(4, musicKeyDisplay), this, SLOT(on_contextMusicKey4_triggered()));
	m->addAction(keys.keyName(21, musicKeyDisplay), this, SLOT(on_contextMusicKey21_triggered()));
	m->addAction(keys.keyName(7, musicKeyDisplay), this, SLOT(on_contextMusicKey7_triggered()));
	m->addAction(keys.keyName(18, musicKeyDisplay), this, SLOT(on_contextMusicKey18_triggered()));
	m->addAction(keys.keyName(8, musicKeyDisplay), this, SLOT(on_contextMusicKey8_triggered()));
	m->addAction(keys.keyName(1, musicKeyDisplay), this, SLOT(on_contextMusicKey1_triggered()));
	m->addAction(keys.keyName(11, musicKeyDisplay), this, SLOT(on_contextMusicKey11_triggered()));
	m->addAction(keys.keyName(25, musicKeyDisplay), this, SLOT(on_contextMusicKey25_triggered()));
}


void Search::on_contextMusicKeyVerified_triggered()
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
	renderTrack(currentTrackListItem, &tUpdate);

}

void Search::on_contextSetMusicKey(int k)
{
	if (!currentTrackListItem) return;
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
	renderTrack(currentTrackListItem, &tUpdate);
}



void Search::on_trackList_itemClicked(QTreeWidgetItem* item, int)
{
	Qt::KeyboardModifiers key=QApplication::keyboardModifiers();
	const DataTitle* t=wm->GetTitle(((WMTreeItem*)item)->Id);
	if (t) {
		QClipboard* clipboard = QApplication::clipboard();
		ppl7::String Text;
		if (key & (Qt::AltModifier)) {
			Text.setf("%s %s", (const char*)t->Artist, (const char*)t->Title);
		} else {

			Text.setf("%s - %s (%s, %0i:%02i min, %s)", (const char*)t->Artist, (const char*)t->Title,
				(const char*)wm->GetVersionText(t->VersionId), t->Length / 60, t->Length % 60, (const char*)wm->GetGenreText(t->GenreId));
			Text.appendf(" [%s %u %c-%i]", (const char*)wm->GetDeviceNameShort(t->DeviceType),
				t->DeviceId, (t->Page + 'A' - 1), t->Track);
			//printf("Text=%s\n", (const char*)Text);
		}
		LastClipboardString=Text;
		clipboard->setText(Text, QClipboard::Clipboard);
		clipboard->setText(Text, QClipboard::Selection);
	}
	ui.artist->setFocus();

}

void Search::on_trackList_itemDoubleClicked(QTreeWidgetItem* item, int)
{
	const DataTitle* t=wm->GetTitle(((WMTreeItem*)item)->Id);
	if (t != NULL) {
		ppl7::String Path=wm->GetAudioFilename(t->DeviceType, t->DeviceId, t->Page, t->Track);
		if (Path.isEmpty()) return;
		//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (char*)Path);
		wm->PlayFile(Path);
	}
	ui.artist->setFocus();
}


void Search::on_contextFindMoreMedium_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		CTrackList l=wm->GetTracklist(t->DeviceType, t->DeviceId, t->Page);
		if (l.Num() > 0) {
			ui.numTracks->setText("");
			currentTrackListItem=NULL;
			trackList->clear();
			trackList->setWordWrap(false);
			trackList->setSortingEnabled(false);
			Results.clear();
			int min=l.GetMin();
			int max=l.GetMax();
			for (int i=min;i <= max;i++) {
				const DataTrack* track=l.GetPtr(i);
				if (track) {
					t=wm->GetTitle(track->TitleId);
					if (t) Results.push_back(t);
				}
			}
			PresentResults();
			ui.progressBar->setValue(100);
			trackList->sortByColumn(0, Qt::AscendingOrder);
			trackList->setSortingEnabled(true);
		}
	}
	ui.artist->setFocus();
}


void Search::on_contextFindMoreVersions_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		FastSearch(t->Artist, t->Title);
	}
	ui.artist->setFocus();
}

void Search::on_contextFindMoreArtist_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		FastSearch(t->Artist, "");
	}
	ui.artist->setFocus();
}

void Search::on_contextFindMoreTitle_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t) {
		FastSearch("", t->Title);
	}
	ui.artist->setFocus();
}

void Search::on_contextPlayTrack_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t != NULL) {
		ppl7::String Path=wm->GetAudioFilename(t->DeviceType, t->DeviceId, t->Page, t->Track);
		if (Path.isEmpty()) return;
		//printf ("Play Device %i, Track: %i: %s\n",DeviceId, currentTrackListItem->Track, (char*)Path);
		wm->PlayFile(Path);
	}
	ui.artist->setFocus();
}

void Search::on_contextEditTrack_triggered()
{
	ui.artist->setFocus();
	if (!currentTrackListItem) return;
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;
	wm->OpenEditor(t->DeviceType, t->DeviceId, t->Page, t->Track);
}

void Search::on_contextCopyTrack_triggered()
{
	on_trackList_itemClicked(currentTrackListItem, 0);
	ui.artist->setFocus();
}

void Search::on_contextCopyFile_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t != NULL) {
		ppl7::String Path=wm->GetAudioFilename(t->DeviceType, t->DeviceId, t->Page, t->Track);
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
	ui.artist->setFocus();
}

void Search::on_contextCopyCover_triggered()
{
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (t != NULL) {
		ppl7::String Path=wm->GetAudioFilename(t->DeviceType, t->DeviceId, t->Page, t->Track);
		if (Path.isEmpty()) return;
		loadCoverToClipboard(Path);
	}
}

void Search::rateCurrentTrack(int value)
{
	if (!currentTrackListItem) return;
	const DataTitle* t=wm->GetTitle(currentTrackListItem->Id);
	if (!t) return;
	if (value == t->Rating) return;
	DataTitle tUpdate=*t;
	tUpdate.Rating=value;
	try {
		wm->TitleStore.Put(tUpdate);
	} catch (const ppl7::Exception& exp) {
		ShowException(exp, tr("Could not save Title in TitleStore"));
		return;
	}
	renderTrack(currentTrackListItem, &tUpdate);
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


bool Search::on_trackList_MousePress(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) startPos=event->pos();
	return false;
}

bool Search::on_trackList_MouseRelease(QMouseEvent*)
{
	startPos.setX(0);
	startPos.setY(0);
	return false;
}

bool Search::on_trackList_MouseMove(QMouseEvent* event)
{
	if (!(event->buttons() == Qt::LeftButton)) {
		//QTreeWidget::mouseMoveEvent(event);
		return false;
	}
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
	ppl7::String xml;
	xml="<winmusikTracklist>\n";
	xml+="<tracks>\n";
	QPixmap Icon;
	int count=0;
	for (int i=0;i < Items.size();i++) {
		item=(WMTreeItem*)Items[i];
		const DataTitle* t=wm->GetTitle(item->Id);
		xml+="<item>\n";
		xml+=wm->getXmlTitle(item->Id);
		if (t != NULL) {
			if (Icon.isNull()) {
				if (t->CoverPreview.size() > 0) {
					Icon.loadFromData((const uchar*)t->CoverPreview.ptr(), t->CoverPreview.size());
				}
			}

			File=wm->GetAudioFilename(t->DeviceType, t->DeviceId, t->Page, t->Track);
			if (File.notEmpty()) {
				xml+="<File>" + ppl7::EscapeHTMLTags(File) + "</File>";
#ifdef _WIN32
				//if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 10, "Search", "on_trackList_MouseMove", __FILE__, __LINE__, "Add File to Drag: %s", (const char*)File);
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
	QDrag* drag = new QDrag(trackList);
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

	return false;
}


void Search::on_watchClipboard_toggled(bool checked)
{
	//printf ("Toggled: %i\n",checked);
	if (!checked) {
		ClipBoardTimer.stop();

		return;
	}
	ClipBoardTimer.start(200);
}

void Search::on_watchClipboardHarddisk_toggled(bool checked)
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
	copyFromClipboard(clip);
	if (clip.PlainText == LastClipboardString) return;

	LastClipboardString=clip.PlainText;
	if (LastClipboardString.isEmpty()) return;
	RegExpMatch match;
	ppl7::String s;
	if (wm->RegExpCapture.match(clip, match)) {
		s=match.Artist + " " + match.Title;
		//printf ("RegExpMatch: %s\n",(const char*)s);
	} else {
		s=clip.PlainText;
		if (s.pregMatch("/^.*? - .*? \\(.*?,.*?,.*?\\).*$/")) return;
		if (s.instr("\n") >= 0) return;
		if (s.instrCase("https://") >= 0) return;
		s.replace("\t", " ");
		s.pregReplace("/\\(.*?\\)/", "");
		s.trim();
		if (s.isEmpty() || s.size() > 1024) return;
		//printf ("NO RegExpMatch: %s\n",(const char*)s);
	}
	wm->normalizer.NormalizeTerm(s);
	ClipBoardTimer.stop();
	//printf ("Update\n");

	if (ui.watchClipboardHarddisk->isChecked()) {
		ui.tabWidget->setCurrentIndex(2);
		ui.query_harddisk->setText(s);
		on_hardDiskSearchButton_clicked();

	}
	if (ui.watchClipboard->isChecked()) {
		ui.tabWidget->setCurrentIndex(0);
		ui.query->setText(s);
		on_quicksearchButton_clicked();
	}
	ClipBoardTimer.start(200);

}

void Search::on_hardDiskSearchButton_clicked()
{
	if (wm->conf.DirectorySearch.size() == 0) return;
	ppl7::String Query=ui.query_harddisk->text();
	wm->normalizer.NormalizeTerm(Query);

	QTreeWidgetItem* item=new QTreeWidgetItem;

	trackList->clear();
	item->setText(0, tr("Pos"));
	item->setText(1, tr("Filename"));
	item->setText(2, tr("Size"));

	trackList->setHeaderItem(item);
	resultmode=1;
	Resize();
	trackList->update();

	if (Query.isEmpty()) return;
	ppl7::Array Words;
	Words.explode(Query, " ");

	ui.numTracks->setText("");
	currentTrackListItem=NULL;

	trackList->setWordWrap(false);
	trackList->setSortingEnabled(false);
	Results.clear();
	std::list<ppl7::String>::const_iterator it;
	for (it=wm->conf.DirectorySearch.begin();it != wm->conf.DirectorySearch.end();++it) {
		RecursiveDirSearch(Words, (*it));
	}
	ui.progressBar->setValue(80);
	trackList->sortByColumn(0, Qt::AscendingOrder);
	trackList->setSortingEnabled(true);
	ui.progressBar->setValue(100);
	ui.query_harddisk->setFocus();

}

void Search::RecursiveDirSearch(ppl7::Array& search, const ppl7::String& dir)
{
	ppl7::Dir Dir;
	ppl7::String File, Tmp;
	//const char* tmp;
	try {
		Dir.open(dir, ppl7::Dir::SORT_NONE);
	} catch (...) {
		return;
	}
	ppl7::Dir::Iterator it;
	ppl7::DirEntry entry;
	Dir.reset(it);
	while (Dir.getNext(entry, it)) {
		if (entry.isDir()) {
			if (entry.Filename != "." && entry.Filename != "..")
				RecursiveDirSearch(search, entry.File);
		} else if (entry.isFile()) {
			File=entry.Filename;
			wm->normalizer.NormalizeTerm(File);
			// Wir prüfen, ob jedes Suchwort im Filenamen auftaucht
			bool match=true;
			for (size_t i=0;i < search.size();i++) {
				if (File.instr(search[i]) < 0) {
					match=false;
					break;
				}
			}
			if (match) {
				//printf ("Match: %s\n",entry->File.GetPtr());
				WMTreeItem* item=new WMTreeItem;
				item->Id=0;
				Tmp.setf("%5u", 0);
				item->setText(0, Tmp);
				item->setText(1, entry.File);
				Tmp.setf("%u MB", (uint32_t)(entry.Size / 1024 / 1024));
				item->setText(2, Tmp);
				trackList->addTopLevelItem(item);

			}
		}
	}
}

void Search::on_displayMusicKey_currentIndexChanged(int)
{
	switch (ui.displayMusicKey->currentIndex()) {
		case 0: musicKeyDisplay=musicKeyTypeMusicalSharps; break;
		case 1: musicKeyDisplay=musicKeyTypeMusicalFlats; break;
		case 2: musicKeyDisplay=musicKeyTypeOpenKey; break;
		case 3: musicKeyDisplay=musicKeyTypeCustom; break;
		default: musicKeyDisplay=musicKeyTypeOpenKey; break;
	}
	updateTrackListing();
	for (int i=1;i <= 24;i++) {
		ui.keywheel->setKeyName(i, wm_main->MusicKeys.keyName(i, musicKeyDisplay));
	}
	ui.keywheel->repaint();
}

void Search::on_keywheel_clicked(int)
{
	on_quicksearchButton_clicked();
}

void Search::on_enableGenreSearch_toggled(bool enabled)
{
	ui.qs_genre->setEnabled(enabled);
	ui.qs_genre->setFocus();
}

void Search::on_enableLengthSearch_toggled(bool enabled)
{
	ui.lengthStart->setEnabled(enabled);
	ui.lengthEnd->setEnabled(enabled);
	ui.lengthStart->setFocus();
}

void Search::on_enableEnergySearch_toggled(bool enabled)
{
	ui.energyStart->setEnabled(enabled);
	ui.energyEnd->setEnabled(enabled);
	ui.energyStart->setFocus();
}


void Search::on_enableBpmSearch_toggled(bool enabled)
{
	ui.bpmStart->setEnabled(enabled);
	ui.bpmEnd->setEnabled(enabled);
	ui.bpmStart->setFocus();
}

void Search::on_enableYearSearch_toggled(bool enabled)
{
	ui.releaseDateStart->setEnabled(enabled);
	ui.releaseDateEnd->setEnabled(enabled);
	ui.setThisYear->setEnabled(enabled);
	ui.setLastYear->setEnabled(enabled);
	ui.releaseDateStart->setFocus();
}

void Search::on_enableRecordingDateSearch_toggled(bool enabled)
{
	ui.recordDateStart->setEnabled(enabled);
	ui.recordDateEnd->setEnabled(enabled);
	ui.setRecordingDate0->setEnabled(enabled);
	ui.setRecordingDate1->setEnabled(enabled);
	ui.setRecordingDate2->setEnabled(enabled);
	ui.recordDateStart->setFocus();
}

void Search::on_enableRatingSearch_toggled(bool enabled)
{
	ui.ratingStart->setEnabled(enabled);
	ui.ratingEnd->setEnabled(enabled);

	ui.ratingStart->setFocus();
}

void Search::on_setThisYear_clicked()
{
	QDate now=QDate::currentDate();
	now.setDate(now.year(), 01, 01);
	ui.releaseDateStart->setDate(now);
	now.setDate(now.year(), 12, 31);
	ui.releaseDateEnd->setDate(now);
}

void Search::on_setLastYear_clicked()
{
	QDate now=QDate::currentDate();
	now.setDate(now.year() - 1, 01, 01);
	ui.releaseDateStart->setDate(now);
	now.setDate(now.year(), 12, 31);
	ui.releaseDateEnd->setDate(now);
}

void Search::on_setRecordingDate0_clicked()
{
	QDate now=QDate::currentDate();
	now.setDate(now.year(), now.month(), 01);
	ui.recordDateStart->setDate(now);
	now.setDate(now.year(), now.month(), now.daysInMonth());
	ui.recordDateEnd->setDate(now);
}

void Search::on_setRecordingDate1_clicked()
{
	QDate now=QDate::currentDate().addMonths(-1);
	now.setDate(now.year(), now.month(), 01);
	ui.recordDateStart->setDate(now);
	now.setDate(now.year(), now.month(), now.daysInMonth());
	ui.recordDateEnd->setDate(now);
}

void Search::on_setRecordingDate2_clicked()
{
	QDate now=QDate::currentDate().addMonths(-2);
	now.setDate(now.year(), now.month(), 01);
	ui.recordDateStart->setDate(now);
	now.setDate(now.year(), now.month(), now.daysInMonth());
	ui.recordDateEnd->setDate(now);
}

void Search::on_useFilter_toggled(bool enabled)
{
	ui.filter->setVisible(enabled);
}


void Search::handleDropEvent(QDropEvent* event)
{
	event->accept();
	const QMimeData* mime=event->mimeData();
	if (!mime) return;
	if (mime->hasFormat("application/winmusik+xml")) {
		QByteArray ba=mime->data("application/winmusik+xml");
		ppl7::String xml;
		xml.set(ba.constData(), ba.size());
		if (xml.left(18) == "<winmusikTracklist") {
			//handleXMLDrop(xml,insertItem);
			//return;
		}
	}
	QList<QUrl>	list=mime->urls();
	if (list.size() == 0) return;
	QUrl url=list[0];
	QString file=url.toLocalFile();
	//printf ("File is (toString):  %s\n", (const char*)ppl7::String(file));
	//printf ("File is (toLocalFile):  %s\n", (const char*)ppl7::String(url.toLocalFile()));
	TrackInfo info;
	if (!getTrackInfoFromFile(info, file)) {
		return;
	}

	ui.query->setText(info.Ti.Artist + " " + info.Ti.Title);

	on_quicksearchButton_clicked();


}
