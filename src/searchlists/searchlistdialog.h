/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: SearchlistDialog.h,v 1.1 2010/11/14 13:20:11 pafe Exp $
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


#ifndef SEARCHLISTDIALOG_H
#define SEARCHLISTDIALOG_H

#include <QtGui/QWidget>
#include <QTimer>
#include "ui_searchlistdialog.h"
#include "winmusik3.h"
#include "csearchlist.h"

#define SL_COLUMN_ARTIST		0
#define SL_COLUMN_VERSION		1
#define SL_COLUMN_GENRE			2
#define SL_COLUMN_LENGTH		3
#define SL_COLUMN_RATING		4
#define SL_COLUMN_DATEADDED		5
#define SL_COLUMN_EXISTING		6
#define SL_COLUMN_SELECTED		7
#define SL_COLUMN_DONE			8



class SearchlistDialog : public QWidget
{
    Q_OBJECT

public:
    SearchlistDialog(QWidget *parent = 0, CWmClient *wm=NULL, const ppl6::CString &Filename="");
    ~SearchlistDialog();
    void ReloadTranslation();
    void addTrack(const SearchlistItem &track);

protected:
    bool eventFilter(QObject *target, QEvent *event);

private:
    class SearchlistTreeItem : public QTreeWidgetItem
    {
    	public:
    		SearchlistItem	Track;
    };
    SearchlistTreeItem *currentTrackListItem;

    Ui::SearchlistDialogClass ui;
    CWmClient *wm;
    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent * event);
    void Resize();
    void editTrack(SearchlistTreeItem *item);
    void renderTrack(SearchlistTreeItem *item);
    void dupeCheckOnTrack(SearchlistTreeItem *item);
    int save();
    void rateCurrentTrack(int value);
    void setupStatusBar();
    void updateStatusBar();

    SearchlistItem	copyItem;
    bool			haveCopyItem;

    CSearchlist List;
    ppl6::CString	Filename;
    QWidget *searchWindow;

    QTimer		ClipBoardTimer;
    ppl6::CString	LastClipboardString;

    QStatusBar	*statusbar;
    QLabel		*statusbarTrackNumber;
    QLabel		*statusbarTracksSelected;

public slots:
	void on_trackList_customContextMenuRequested ( const QPoint & pos );
	void on_trackList_itemClicked ( QTreeWidgetItem * item, int column );
	void on_trackList_itemDoubleClicked ( QTreeWidgetItem * item, int column );
	void on_trackList_itemSelectionChanged();

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
};


#endif // SEARCHLISTDIALOG_H
