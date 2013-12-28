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
#include "playlisttracks.h"
#include <ppl6-sound.h>

class PlaylistTracks;

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
    QMenu	*menuRecentPlaylists;
    QLabel *totalTrackLength;
    QLabel *totalMixLength;
    QLabel *totalTracks;
    QWidget *searchWindow;
    QWidget *saveWidget, *saveAsWidget;

    ppl6::CString	PlaylistFileName;
    bool changed;

    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent * event);
    void Resize();
    void setChanged(bool flag);

    void recreatePlaylist();
    void updatePlaylist();
    void updateRecentPlaylistsMenu();
    void createMenue();
    void createToolbar();
    void createStatusBar();
    void createSetMusicKeyContextMenu(QMenu *m);

    QMessageBox::StandardButton saveFirst();

    void loadPlaylist(ppl6::CString &Filename);

    bool loadTrackFromDatabase(PlaylistItem *item, ppluint32 titleId);
    void loadTrackFromFile(PlaylistItem *item, const ppl6::CString &file);
    void renderTrack(PlaylistItem *item);
    void updateLengthStatus();
    void renumberTracks();
    void saveTitle(PlaylistItem *item);
    void rateCurrentTrack(int value);


    void renderTrackViewPlaylist(PlaylistItem *item);
    void renderTrackViewDJ(PlaylistItem *item);

    bool eventFilter(QObject *target, QEvent *event);
    bool consumeEvent(QObject *target, QEvent *event);
    void handleDropEvent(QDropEvent *event);
    void handleDropEvent(const QMimeData *mime, QTreeWidgetItem *insertItem);
    void handleXMLDrop(const ppl6::CString &xml, QTreeWidgetItem *insertItem);
    void handleURLDrop(const QList<QUrl> &list, QTreeWidgetItem *insertItem);
    void updateLastPlaylist();

    void closeEvent(QCloseEvent *event);

    void setItemBackgroundColor(PlaylistItem *item, const QColor &c);
    void setItemBackground(PlaylistItem *item, const QBrush &b);
    void highlightHarmonicKeys(PlaylistItem *track);


    bool on_tracks_MouseMove(QMouseEvent *event);
    bool on_tracks_MouseButtonPress(QMouseEvent * event);
    bool on_tracks_MouseButtonRelease(QMouseEvent * event);


    // Aktionen
    void editTrack(PlaylistItem *item);
    void copyTracks(const QList<QTreeWidgetItem *> items);


    enum playlistViewType {
    	playlistViewNormal,
    	playlistViewDJ
    };
    playlistViewType playlistView;

    int columnTrack;
    int columnCover;
    int columnTitle;
    int columnGenre;
    int columnComment;
    int columnLength;
    int columnRating;
    int columnSource;
    int columnBpm;
    int columnBpmPlayed;
    int columnMusicKey;
    int columnStart;
    int columnEnd;
    int columnCuts;
    int columnTotalLength;

    QPoint startPos;	// Für Drag/Drop und multiple Markierungen
    QPoint	ratePos;	// Für Rating-Spalte


public slots:

	void on_menuNew_triggered();
	void on_menuOpen_triggered();
	void on_menuSave_triggered();
	void on_menuSaveAs_triggered();

	void on_menuOpenRecent0_triggered();
	void on_menuOpenRecent1_triggered();
	void on_menuOpenRecent2_triggered();
	void on_menuOpenRecent3_triggered();
	void on_menuOpenRecent4_triggered();

	void on_viewPlaylist_triggered();
	void on_viewDJ_triggered();

	void on_tracks_itemDoubleClicked (QTreeWidgetItem * item, int column);
	void on_tracks_itemClicked (QTreeWidgetItem * item, int column);

	void on_tracks_customContextMenuRequested ( const QPoint & pos );

	void on_contextEditTrack_triggered();
	void on_contextCopyTrack_triggered();
	void on_contextPasteTrack_triggered();
	void on_contextDeleteTrack_triggered();
	void on_contextFindMoreVersions_triggered();
	void on_contextFindMoreArtist_triggered();
	void on_contextFindMoreTitle_triggered();
	void on_contextPlayTrack_triggered();


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

	void on_contextPasteCover_triggered();
};


#endif // PLAYLIST_H
