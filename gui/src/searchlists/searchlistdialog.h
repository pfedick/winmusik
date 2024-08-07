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


#ifndef SEARCHLISTDIALOG_H
#define SEARCHLISTDIALOG_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include "ui_searchlistdialog.h"
#include "winmusik3.h"
#include "csearchlist.h"

#define SL_COLUMN_ARTIST		0
#define SL_COLUMN_VERSION		1
#define SL_COLUMN_GENRE			2
#define SL_COLUMN_LABEL			3
#define SL_COLUMN_LENGTH		4
#define SL_COLUMN_RATING		5
#define SL_COLUMN_DATEADDED		6
#define SL_COLUMN_DATERELEASE	7
#define SL_COLUMN_EXISTING		8
#define SL_COLUMN_SELECTED		9
#define SL_COLUMN_DONE			10


class DupeCheckThread : public QThread
{
	Q_OBJECT
private:
	bool stop;
	QTreeWidget* trackList;

public:
	DupeCheckThread(QObject* parent = 0);
	void setTracklist(QTreeWidget* trackList);
	void run();
	void stopThread();
	void startThread();
	int dupeCheckOnTrack(QTreeWidgetItem* item);

signals:
	void updateItem(QTreeWidgetItem* item, int dupePresumption);
};

class SearchlistDialog : public QWidget
{
	Q_OBJECT

public:
	SearchlistDialog(QWidget* parent = 0, CWmClient* wm=NULL, const ppl7::String& Filename="");
	~SearchlistDialog();
	void ReloadTranslation();
	void addTrack(const SearchlistItem& track);
	void deleteSelectedTracks();
	void show();

	class SearchlistTreeItem : public QTreeWidgetItem
	{
	public:
		SearchlistItem	Track;
		int	dupePresumption;
	};

protected:
	bool eventFilter(QObject* target, QEvent* event);

private:
	SearchlistTreeItem* currentTrackListItem;
	QPoint startPos;	// Für Drag/Drop und multiple Markierungen

	Ui::SearchlistDialogClass ui;
	CWmClient* wm;
	void resizeEvent(QResizeEvent* event);
	void showEvent(QShowEvent* event);
	void closeEvent(QCloseEvent* event);
	void Resize();
	void editTrack(SearchlistTreeItem* item);
	void renderTrack(SearchlistTreeItem* item);
	void dupeCheckOnTrack(SearchlistTreeItem* item);
	void save();
	void rateCurrentTrack(int value);
	void setupStatusBar();
	void updateStatusBar();

	DupeCheckThread* dupeCheckThread;

	SearchlistItem	copyItem;
	bool			haveCopyItem;

	CSearchlist List;
	ppl7::String	Filename;
	QWidget* searchWindow;

	QTimer			ClipBoardTimer;
	ppl7::String	LastClipboardString;

	QStatusBar* statusbar;
	QLabel* statusbarTrackNumber;
	QLabel* statusbarTracksSelected;
	QLabel* statusbarTracksPreSelected;
	QLabel* statusbarTracksDone;

public slots:
	void on_trackList_customContextMenuRequested(const QPoint& pos);
	void on_trackList_itemClicked(QTreeWidgetItem* item, int column);
	void on_trackList_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_trackList_itemSelectionChanged();
	void on_trackList_itemDropped(SearchlistItem* item);
	void on_trackList_changed();

	void on_contextEditTrack_triggered();
	void on_contextInsertTrack_triggered();
	void on_contextDeleteTrack_triggered();
	void on_contextFind_triggered();
	void on_contextRate0_clicked();
	void on_contextRate1_clicked();
	void on_contextRate2_clicked();
	void on_contextRate3_clicked();
	void on_contextRate4_clicked();
	void on_contextRate5_clicked();
	void on_contextRate6_clicked();

	void on_newTrackFromClipboardButton_clicked();
	void on_newTrackButton_clicked();
	void on_searchButton_clicked();
	void on_deleteTrackButton_clicked();
	void on_saveExitButton_clicked();
	void on_saveButton_clicked();
	void on_ClipBoardTimer_update();
	void updateDupeCheckItem(QTreeWidgetItem* item, int dupePresumption);
};


#endif // SEARCHLISTDIALOG_H
