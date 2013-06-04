/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.4 $
 * $Date: 2010/11/26 16:08:40 $
 * $Id: search.h,v 1.4 2010/11/26 16:08:40 pafe Exp $
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


#ifndef SEARCH_H
#define SEARCH_H

#include <QtGui/QWidget>
#include <QTimer>
#include "ui_search.h"
#include "winmusik3.h"


class Search;
class CSearchTrackList;

#define SEARCH_TRACKLIST_RATING_ROW	6
#define SEARCH_TRACKLIST_SOURCE_ROW 7

class Search : public QWidget
{
    Q_OBJECT

    friend class CSearchTrackList;
public:
    Search(QWidget *parent = 0, CWmClient *wm=NULL);
    ~Search();

    //void StartSearch(const char *artist, const char *title);
    void FastSearch(const ppl6::CString &Artist, const ppl6::CString &Title, const ppl6::CString &Version=ppl6::CString(),const ppl6::CString &Genre=ppl6::CString(),const ppl6::CString &Tags=ppl6::CString(),const ppl6::CString &Label=ppl6::CString());
    void ReloadTranslation();

    void DoSearch();
    void Resize();

private:
    Ui::searchClass ui;
    CWmClient *wm;
    int resultmode;
    ppl6::CGenericList Results;
    CSearchTrackList *trackList;
    WMTreeItem *currentTrackListItem;
    QPoint startPos;	// Für Drag/Drop und multiple Markierungen
    char AllowedDevices[20];

    QTimer		ClipBoardTimer;
    ppl6::CString	LastClipboardString;

    void PresentResults();
    void SetupTrackList();
    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent * event);

    bool on_trackList_MousePress(QMouseEvent *event);
    bool on_trackList_MouseRelease(QMouseEvent *event);
    bool on_trackList_MouseMove(QMouseEvent *event);
    void CheckAllowedDevices();

    void RecursiveDirSearch(ppl6::CArray &search, const ppl6::CString &dir);
    void NormalizeTerm(ppl6::CString &term);
    void DefaultTracklistHeader();

    void FilterResult(const CTitleHashTree &in, ppl6::CGenericList &out);
    void LimitResult(const ppl6::CGenericList &in, ppl6::CGenericList &out);
    void RandomResult(const ppl6::CGenericList &in, ppl6::CGenericList &out, size_t num);
    void rateCurrentTrack(int value);

    void renderTrack(WMTreeItem *item, DataTitle *ti);



public slots:
    void on_searchButton_clicked();
    void on_quicksearchButton_clicked();
    void on_watchClipboard_toggled (bool checked);
    void on_ClipBoardTimer_update();
    void on_hardDiskSearchButton_clicked();
    void on_watchClipboardHarddisk_toggled (bool checked);



	void on_trackList_customContextMenuRequested ( const QPoint & pos );
	void on_trackList_itemClicked ( QTreeWidgetItem * item, int column);
	void on_trackList_itemDoubleClicked ( QTreeWidgetItem * item, int column);
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

	void on_contextRate0_clicked();
	void on_contextRate1_clicked();
	void on_contextRate2_clicked();
	void on_contextRate3_clicked();
	void on_contextRate4_clicked();
	void on_contextRate5_clicked();
	void on_contextRate6_clicked();

	void on_artist_returnPressed();
	void on_title_returnPressed();
	void on_version_returnPressed();
	void on_genre_returnPressed();
	void on_tags_returnPressed();
	void on_query_returnPressed();
	void on_recordLabel_returnPressed();
};

class CSearchTrackList : public QTreeWidget
{
	friend class Search;
	private:
		Search *search;
		CWmClient *wm;
		QPoint startPos;

	public:
		CSearchTrackList(QWidget * parent = 0)
		: QTreeWidget(parent)
		{
			search=NULL;
			wm=NULL;
		}


	private:
		//void mouseDoubleClickEvent ( QMouseEvent * event);
		//void itemClicked ( QTreeWidgetItem * item, int column);
		//void itemDoubleClicked ( QTreeWidgetItem * item, int column);
		void mousePressEvent ( QMouseEvent * event ) {
			search->on_trackList_MousePress(event);
			QTreeWidget::mousePressEvent(event);
		}
		void mouseReleaseEvent ( QMouseEvent * event ) {
			search->on_trackList_MouseRelease(event);
			QTreeWidget::mouseReleaseEvent(event);
		}
		void mouseMoveEvent ( QMouseEvent * event ) {
			search->on_trackList_MouseMove(event);
		}

		//void contextMenuEvent(QContextMenuEvent *event);
		//void customContextMenuRequested ( const QPoint & pos );
};

#endif // SEARCH_H
