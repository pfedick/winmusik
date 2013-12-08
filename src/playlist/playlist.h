/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: Playlist.h,v 1.1 2010/11/14 13:20:11 pafe Exp $
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


#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "ui_playlist.h"
#include "winmusik3.h"

class PlaylistTracks;

class PlaylistItem : public QTreeWidgetItem
{
	public:
		ppluint32	titleId;
		int			startPositionSec;
		int			endPositionSec;
		int			cutStartPosition[5];
		int			cutEndPosition[5];
		ppl6::CString	Artist;
		ppl6::CString	Title;
		ppl6::CString	Version;
		ppl6::CString	Genre;
		ppl6::CString	Label;
		ppl6::CString	Album;
		ppl6::CString	File;
		int				musicKey;
		int				bpm;
		int				rating;
		int				length;
};

class Playlist : public QMainWindow
{
    Q_OBJECT

    friend class PlaylistTracks;

public:
    Playlist(QWidget *parent = 0, CWmClient *wm=NULL);
    ~Playlist();
    void ReloadTranslation();

private:
    Ui::playlistClass ui;
    CWmClient *wm;
    PlaylistItem *currentTreeItem;

    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent * event);
    void Resize();

    void recreatePlaylist();
    void updatePlaylist();
    void renderTrack(PlaylistItem *item);

    void renderTrackViewPlaylist(PlaylistItem *item);
    void renderTrackViewDJ(PlaylistItem *item);

    bool eventFilter(QObject *target, QEvent *event);
    bool consumeEvent(QObject *target, QEvent *event);
    void handleDropEvent(QDropEvent *event);
    void unselectItems();
    void deleteSelectedItems();

    void closeEvent(QCloseEvent *event);

    bool on_tracks_MouseMove(QMouseEvent *event);
    bool on_tracks_MouseButtonPress(QMouseEvent * event);
    bool on_tracks_MouseButtonRelease(QMouseEvent * event);


    enum playlistViewType {
    	playlistViewNormal,
    	playlistViewDJ
    };
    playlistViewType playlistView;

    int columnTrack;
    int columnCover;
    int columnTitle;
    int columnGenre;
    int columnLength;
    int columnRating;
    int columnSource;
    int columnBpm;
    int columnMusicKey;
    int columnStart;
    int columnEnd;
    int columnCuts;

    QPoint startPos;	// Für Drag/Drop und multiple Markierungen
    QPoint	ratePos;	// Für Rating-Spalte


public slots:

	void on_menuNew_triggered();
	void on_menuOpen_triggered();
	void on_menuSave_triggered();
	void on_menuSaveAs_triggered();

	void on_viewPlaylist_triggered();
	void on_viewDJ_triggered();

};


#endif // PLAYLIST_H
