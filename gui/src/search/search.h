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


#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QTimer>
#include "ui_search.h"
#include "winmusik3.h"


class Search;
class CSearchTrackList;
class ResultFilter;

#define SEARCH_TRACKLIST_POS_ROW        0
#define SEARCH_TRACKLIST_ARTIST_ROW     1
#define SEARCH_TRACKLIST_TITLE_ROW      2
#define SEARCH_TRACKLIST_VERSION_ROW	3
#define SEARCH_TRACKLIST_GENRE_ROW      4
#define SEARCH_TRACKLIST_LENGTH_ROW     5
#define SEARCH_TRACKLIST_BPM_ROW        6
#define SEARCH_TRACKLIST_KEY_ROW    	7
#define SEARCH_TRACKLIST_ENERGYLEVEL_ROW	8
#define SEARCH_TRACKLIST_YEAR_ROW       9
#define SEARCH_TRACKLIST_RATING_ROW     10
#define SEARCH_TRACKLIST_BITRATE_ROW    11
#define SEARCH_TRACKLIST_SOURCE_ROW     12

class Search : public QWidget
{
	Q_OBJECT

		friend class CSearchTrackList;
public:
	Search(QWidget* parent = 0, CWmClient* wm=NULL);
	~Search();

	//void StartSearch(const char *artist, const char *title);
	void FastSearch(const ppl7::String& Artist, const ppl7::String& Title, const ppl7::String& Version=ppl7::String(), const ppl7::String& Genre=ppl7::String(), const ppl7::String& Tags=ppl7::String(), const ppl7::String& Label=ppl7::String());
	void FastSearch(const ppl7::String& Words);
	void ReloadTranslation();

	void DoSearch();
	void Resize();
	void show();

private:
	Ui::searchClass ui;
	CWmClient* wm;
	int resultmode;
	std::list<DataTitle*> Results;
	CSearchTrackList* trackList;
	WMTreeItem* currentTrackListItem;
	QPoint startPos;	// Für Drag/Drop und multiple Markierungen
	char AllowedDevices[20];

	QTimer		ClipBoardTimer;
	ppl7::String	LastClipboardString;
	MusicKeyType	musicKeyDisplay;

	void PresentResults();
	void SetupTrackList();
	void resizeEvent(QResizeEvent* event);
	void showEvent(QShowEvent* event);
	bool eventFilter(QObject* target, QEvent* event);
	void closeEvent(QCloseEvent* event);

	void handleDropEvent(QDropEvent* event);

	bool on_trackList_MousePress(QMouseEvent* event);
	bool on_trackList_MouseRelease(QMouseEvent* event);
	bool on_trackList_MouseMove(QMouseEvent* event);
	void CheckAllowedDevices();

	void RecursiveDirSearch(ppl7::Array& search, const ppl7::String& dir);
	void NormalizeTerm(ppl7::String& term);
	void DefaultTracklistHeader();

	void FilterResult(const CHashes::TitleTree& in, std::list<DataTitle*>& out);
	void LimitResult(const std::list<DataTitle*>& in, std::list<DataTitle*>& out);
	void RandomResult(const std::list<DataTitle*>& in, std::list<DataTitle*>& out, size_t num);
	void SortResult(const std::list<DataTitle*>& in, std::list<DataTitle*>& out);
	void rateCurrentTrack(int value);

	void updateTrackListing();
	void renderTrack(WMTreeItem* item, DataTitle* ti);
	void createSetMusicKeyContextMenu(QMenu* m);

	void configureFilter(ResultFilter& filter);



public slots:
	void on_searchButton_clicked();
	void on_quicksearchButton_clicked();
	void on_watchClipboard_toggled(bool checked);
	void on_ClipBoardTimer_update();
	void on_hardDiskSearchButton_clicked();
	void on_watchClipboardHarddisk_toggled(bool checked);



	void on_trackList_customContextMenuRequested(const QPoint& pos);
	void on_trackList_itemClicked(QTreeWidgetItem* item, int column);
	void on_trackList_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_markAllButton_clicked();
	void on_markNoneButton_clicked();


	void on_contextFindMoreArtist_triggered();
	void on_contextFindMoreTitle_triggered();
	void on_contextFindMoreVersions_triggered();
	void on_contextFindMoreMedium_triggered();
	void on_contextPlayTrack_triggered();
	void on_contextEditTrack_triggered();
	void on_contextCopyTrack_triggered();
	void on_contextCopyFile_triggered();
	void on_contextCopyCover_triggered();

	void on_contextRate0_clicked();
	void on_contextRate1_clicked();
	void on_contextRate2_clicked();
	void on_contextRate3_clicked();
	void on_contextRate4_clicked();
	void on_contextRate5_clicked();
	void on_contextRate6_clicked();

	void on_contextMusicKeyVerified_triggered();
	void on_contextSetMusicKey(int k);
	void on_contextMusicKey0_triggered() { on_contextSetMusicKey(0); };
	void on_contextMusicKey1_triggered() { on_contextSetMusicKey(1); };
	void on_contextMusicKey2_triggered() { on_contextSetMusicKey(2); };
	void on_contextMusicKey3_triggered() { on_contextSetMusicKey(3); };
	void on_contextMusicKey4_triggered() { on_contextSetMusicKey(4); };
	void on_contextMusicKey5_triggered() { on_contextSetMusicKey(5); };
	void on_contextMusicKey6_triggered() { on_contextSetMusicKey(6); };
	void on_contextMusicKey7_triggered() { on_contextSetMusicKey(7); };
	void on_contextMusicKey8_triggered() { on_contextSetMusicKey(8); };
	void on_contextMusicKey9_triggered() { on_contextSetMusicKey(9); };
	void on_contextMusicKey10_triggered() { on_contextSetMusicKey(10); };
	void on_contextMusicKey11_triggered() { on_contextSetMusicKey(11); };
	void on_contextMusicKey12_triggered() { on_contextSetMusicKey(12); };
	void on_contextMusicKey13_triggered() { on_contextSetMusicKey(13); };
	void on_contextMusicKey14_triggered() { on_contextSetMusicKey(14); };
	void on_contextMusicKey15_triggered() { on_contextSetMusicKey(15); };
	void on_contextMusicKey16_triggered() { on_contextSetMusicKey(16); };
	void on_contextMusicKey17_triggered() { on_contextSetMusicKey(17); };
	void on_contextMusicKey18_triggered() { on_contextSetMusicKey(18); };
	void on_contextMusicKey19_triggered() { on_contextSetMusicKey(19); };
	void on_contextMusicKey20_triggered() { on_contextSetMusicKey(20); };
	void on_contextMusicKey21_triggered() { on_contextSetMusicKey(21); };
	void on_contextMusicKey22_triggered() { on_contextSetMusicKey(22); };
	void on_contextMusicKey23_triggered() { on_contextSetMusicKey(23); };
	void on_contextMusicKey24_triggered() { on_contextSetMusicKey(24); };
	void on_contextMusicKey25_triggered() { on_contextSetMusicKey(25); };
	void on_displayMusicKey_currentIndexChanged(int);


	void on_artist_returnPressed();
	void on_title_returnPressed();
	void on_version_returnPressed();
	void on_genre_returnPressed();
	void on_tags_returnPressed();
	void on_query_returnPressed();
	void on_qs_genre_returnPressed();
	void on_recordLabel_returnPressed();

	void on_keywheel_clicked(int key);

	void on_enableGenreSearch_toggled(bool);
	void on_enableBpmSearch_toggled(bool);
	void on_enableYearSearch_toggled(bool);
	void on_enableRecordingDateSearch_toggled(bool);
	void on_enableRatingSearch_toggled(bool);
	void on_enableLengthSearch_toggled(bool);
	void on_enableEnergySearch_toggled(bool);
	void on_setThisYear_clicked();
	void on_setLastYear_clicked();
	void on_setRecordingDate0_clicked();
	void on_setRecordingDate1_clicked();
	void on_setRecordingDate2_clicked();

	void on_useFilter_toggled(bool);

};

class CSearchTrackList : public QTreeWidget
{
	friend class Search;
private:
	Search* search;
	CWmClient* wm;
	QPoint startPos;

public:
	CSearchTrackList(QWidget* parent = 0)
		: QTreeWidget(parent)
	{
		search=NULL;
		wm=NULL;
	}


private:
	//void mouseDoubleClickEvent ( QMouseEvent * event);
	//void itemClicked ( QTreeWidgetItem * item, int column);
	//void itemDoubleClicked ( QTreeWidgetItem * item, int column);
	void mousePressEvent(QMouseEvent* event) {
		search->on_trackList_MousePress(event);
		QTreeWidget::mousePressEvent(event);
	}
	void mouseReleaseEvent(QMouseEvent* event) {
		search->on_trackList_MouseRelease(event);
		QTreeWidget::mouseReleaseEvent(event);
	}
	void mouseMoveEvent(QMouseEvent* event) {
		search->on_trackList_MouseMove(event);
	}

	//void contextMenuEvent(QContextMenuEvent *event);
	//void customContextMenuRequested ( const QPoint & pos );
};

#endif // SEARCH_H
