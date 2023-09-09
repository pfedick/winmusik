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


#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QMainWindow>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>
#include "ui_playlist.h"
#include "winmusik3.h"
#include "playlisttracks.h"
#include "playlistStatusBar.h"
#include "playlistexport.h"
#include "colorscheme.h"

#include "ppl7-audio.h"

class PlaylistTracks;

class Playlist : public QMainWindow
{
    Q_OBJECT

        friend class PlaylistTracks;

public:
    Playlist(QWidget* parent = nullptr, CWmClient* wm=nullptr);
    ~Playlist();
    void ReloadTranslation();

private:
    Ui::playlistClass ui;
    CWmClient* wm;
    PlaylistItem* currentTreeItem;
    QMenu* menuRecentPlaylists;

    // StatusBar
    PlaylistStatusBar* statusbar;


    QWidget* searchWindow;
    QWidget* saveWidget, * saveAsWidget;

    ppl7::String	PlaylistFileName;
    ppl7::String   LastPrintFileName;
    bool changed;
    MusicKeyType	musicKeyDisplay;
    ColorScheme     colorscheme;

    void resizeEvent(QResizeEvent* event);
    void showEvent(QShowEvent* event);
    void Resize();
    void setChanged(bool flag);

    void recreatePlaylist();
    void updatePlaylist();
    void updateRecentPlaylistsMenu();
    void createMenue();
    void createToolbar();
    void createStatusBar();
    void createSetMusicKeyContextMenu(QMenu* m);
    void createSetEnergyLevelContextMenu(QMenu* m);

    QMessageBox::StandardButton saveFirst();

    void loadPlaylist(ppl7::String& Filename);

    bool loadTrackFromDatabase(PlaylistItem* item, uint32_t titleId);
    void loadTrackFromFile(PlaylistItem* item, const ppl7::String& file);
    void renderTrack(PlaylistItem* item);
    void calcMixLength(PlaylistItem* item);
    void updateLengthStatus();
    void renumberTracks();
    void saveTitle(PlaylistItem* item);
    void rateCurrentTrack(int value);


    void renderTrackViewPlaylist(PlaylistItem* item);
    void renderTrackViewDJ(PlaylistItem* item);

    bool eventFilter(QObject* target, QEvent* event);
    bool consumeEvent(QObject* target, QEvent* event);
    void handleDropEvent(QDropEvent* event);
    void handleDropEvent(const QMimeData* mime, QTreeWidgetItem* insertItem);
    void handleXMLDrop(const ppl7::String& xml, QTreeWidgetItem* insertItem);
    void handleURLDrop(const QList<QUrl>& list, QTreeWidgetItem* insertItem);
    void updateLastPlaylist();
    void updateMixBpmUpDownIndicator();

    void closeEvent(QCloseEvent* event);

    void highlightHarmonicKeys(PlaylistItem* track);
    void unHighlightHarmonicKeys();

    bool on_tracks_MouseButtonPress(QMouseEvent* event);
    bool on_tracks_MouseButtonRelease(QMouseEvent* event);

    // Export Playlist
    void exportFile(PlaylistExport& dialog, int track, const ppl7::String& SourceFile);
    void exportCoverIcon(const ppl7::String& TargetFile, const ppl7::String& SourceFile);
    void exportM3U();
    void exportPLS();
    void exportXSPF();
    void exportTXT(bool withStartTimes);
    void exportJSON();
    ppl7::String getExportFilename(int track, const ppl7::String& SourceFile);

    // Aktionen
    void editTrack(PlaylistItem* item);
    void copyTracks(const QList<QTreeWidgetItem*> items);

    // Filter
    bool isFilterEnabled() const;
    void handleFilterDropEvent(QDropEvent* event);
    void filterChanged();


    enum playlistViewType {
        playlistViewNormal=0,
        playlistViewDJ=1
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
    int columnEnergyLevel;
    int columnStart;
    int columnEnd;
    int columnCuts;
    int columnTotalLength;
    int columnBitrate;
    int columnTimeCode;

    QPoint startPos;	// Für Drag/Drop und multiple Markierungen
    QPoint	ratePos;	// Für Rating-Spalte
    bool	harmonicsHighlighted;


public slots:

    void on_menuNew_triggered();
    void on_menuOpen_triggered();
    void on_menuSave_triggered();
    void on_menuSaveAs_triggered();

    void on_menuExport_triggered();
    void on_printPlaylist_triggered();

    void on_menuOpenRecent0_triggered();
    void on_menuOpenRecent1_triggered();
    void on_menuOpenRecent2_triggered();
    void on_menuOpenRecent3_triggered();
    void on_menuOpenRecent4_triggered();

    void on_viewPlaylist_triggered();
    void on_viewDJ_triggered();
    void on_viewFilter_triggered();

    void on_tracks_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_tracks_itemClicked(QTreeWidgetItem* item, int column);
    void on_tracks_customContextMenuRequested(const QPoint& pos);
    void on_tracks_itemSelectionChanged();
    void on_shufflePlaylist_triggered();


    void on_playlistName_textChanged(const QString& text);
    void on_playlistSubName_textChanged(const QString& text);
    void on_issueNumber_valueChanged(int value);
    void on_issueDate_dateChanged(const QDate& date);

    void on_contextEditTrack_triggered();
    void on_contextOpenTrackInEditor_triggered();
    void on_contextCopyTrack_triggered();
    void on_contextPasteTrack_triggered();
    void on_contextDeleteTrack_triggered();
    void on_contextFindMoreVersions_triggered();
    void on_contextFindMoreArtist_triggered();
    void on_contextFindMoreTitle_triggered();
    void on_contextPlayTrack_triggered();
    void on_contextSetBPMPlayed_triggered();
    void on_contextEditComment_triggered();
    void on_contextReReadInAndOuts_triggered();


    void on_contextRate0_clicked();
    void on_contextRate1_clicked();
    void on_contextRate2_clicked();
    void on_contextRate3_clicked();
    void on_contextRate4_clicked();
    void on_contextRate5_clicked();
    void on_contextRate6_clicked();

    void on_contextMusicKeyVerified_triggered();
    void on_contextSetMusicKey(int k);
    void on_contextMusicKey0_triggered() { on_contextSetMusicKey(0); }
    void on_contextMusicKey1_triggered() { on_contextSetMusicKey(1); }
    void on_contextMusicKey2_triggered() { on_contextSetMusicKey(2); }
    void on_contextMusicKey3_triggered() { on_contextSetMusicKey(3); }
    void on_contextMusicKey4_triggered() { on_contextSetMusicKey(4); }
    void on_contextMusicKey5_triggered() { on_contextSetMusicKey(5); }
    void on_contextMusicKey6_triggered() { on_contextSetMusicKey(6); }
    void on_contextMusicKey7_triggered() { on_contextSetMusicKey(7); }
    void on_contextMusicKey8_triggered() { on_contextSetMusicKey(8); }
    void on_contextMusicKey9_triggered() { on_contextSetMusicKey(9); }
    void on_contextMusicKey10_triggered() { on_contextSetMusicKey(10); }
    void on_contextMusicKey11_triggered() { on_contextSetMusicKey(11); }
    void on_contextMusicKey12_triggered() { on_contextSetMusicKey(12); }
    void on_contextMusicKey13_triggered() { on_contextSetMusicKey(13); }
    void on_contextMusicKey14_triggered() { on_contextSetMusicKey(14); }
    void on_contextMusicKey15_triggered() { on_contextSetMusicKey(15); }
    void on_contextMusicKey16_triggered() { on_contextSetMusicKey(16); }
    void on_contextMusicKey17_triggered() { on_contextSetMusicKey(17); }
    void on_contextMusicKey18_triggered() { on_contextSetMusicKey(18); }
    void on_contextMusicKey19_triggered() { on_contextSetMusicKey(19); }
    void on_contextMusicKey20_triggered() { on_contextSetMusicKey(20); }
    void on_contextMusicKey21_triggered() { on_contextSetMusicKey(21); }
    void on_contextMusicKey22_triggered() { on_contextSetMusicKey(22); }
    void on_contextMusicKey23_triggered() { on_contextSetMusicKey(23); }
    void on_contextMusicKey24_triggered() { on_contextSetMusicKey(24); }
    void on_contextMusicKey25_triggered() { on_contextSetMusicKey(25); }
    void on_statusbar_musicKeySelectionChanged(int newValue);

    void on_contextPasteCover_triggered();

    void on_contextSetEnergyLevel(int v);
    void on_contextEnergyLevel0_triggered() { on_contextSetEnergyLevel(0); }
    void on_contextEnergyLevel1_triggered() { on_contextSetEnergyLevel(1); }
    void on_contextEnergyLevel2_triggered() { on_contextSetEnergyLevel(2); }
    void on_contextEnergyLevel3_triggered() { on_contextSetEnergyLevel(3); }
    void on_contextEnergyLevel4_triggered() { on_contextSetEnergyLevel(4); }
    void on_contextEnergyLevel5_triggered() { on_contextSetEnergyLevel(5); }
    void on_contextEnergyLevel6_triggered() { on_contextSetEnergyLevel(6); }
    void on_contextEnergyLevel7_triggered() { on_contextSetEnergyLevel(7); }
    void on_contextEnergyLevel8_triggered() { on_contextSetEnergyLevel(8); }
    void on_contextEnergyLevel9_triggered() { on_contextSetEnergyLevel(9); }
    void on_contextEnergyLevel10_triggered() { on_contextSetEnergyLevel(10); }

    void on_searchTriggered();

    // Filter
    void on_ct_modificationSpinBox_valueChanged(int i);
    void on_targetTrackGroupBox_toggled(bool) { filterChanged(); }
    void on_tt_keyModificationSpinBox_valueChanged(int) { filterChanged(); }
    void on_tt_sameKeyCheckBox_toggled(bool) { filterChanged(); }
    void on_tt_nextKeyCheckBox_toggled(bool) { filterChanged(); }
    void on_tt_previousKeyCheckBox_toggled(bool) { filterChanged(); }
    void on_tt_minorMajorSwitchCheckBox_toggled(bool) { filterChanged(); }
    void on_tt_minorMajorJump3CheckBox_toggled(bool) { filterChanged(); }
    void on_tt_minorMajorJump1CheckBox_toggled(bool) { filterChanged(); }
    void on_tt_boostSemitone1CheckBox_toggled(bool) { filterChanged(); }
    void on_tt_boostSemitone2CheckBox_toggled(bool) { filterChanged(); }
    void on_tt_relatedKeyCheckBox_toggled(bool) { filterChanged(); }
    void on_tt_bpmStartSpinBox_valueChanged(int) { filterChanged(); }
    void on_tt_bpmEndSpinBox_valueChanged(int) { filterChanged(); }

    void on_tt_bpmCheckBox_toggled(bool) { filterChanged(); }

};


#endif // PLAYLIST_H
