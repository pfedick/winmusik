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

#ifndef EDITTRACK_H
#define EDITTRACK_H

#include <QWidget>
#include "ui_edittrack.h"
#include "winmusik3.h"
#include <QKeyEvent>
#include "tablecontrol.h"

#include "fkeys.h"



class EditTrack : public QWidget
{
    Q_OBJECT

public:
    EditTrack(QWidget* parent = 0);
    ~EditTrack();

    void setWinMusikClient(CWmClient* wm);
    void setData(const TrackInfo& data);
    void setFilename(const ppl7::String& Filename);
    void setFkeys(Fkeys* fkeys);
    const TrackInfo& getData();
    void setFocus();
    void setFocus(Qt::FocusReason reason);
    void ReloadTranslation();

    PPL7EXCEPTION(DataIncompleteException, Exception);


private:
    Ui::EditTrackClass ui;
    CWmClient* wm;
    Fkeys* fkeys;
    TrackInfo	data;
    QPixmap			Cover;
    int position, oldposition;

    CTableControl TCVersion;
    CTableControl TCGenre;
    CTableControl TCLabel;
    CTableControl TCRecordSource;
    CTableControl TCRecordDevice;
    CTrackList* TrackList;
    QStringList Artists;
    QStringList Titles;
    QStringList Albums;
    QCompleter* artistCompleter;
    QCompleter* titleCompleter;
    QCompleter* albumCompleter;
    bool		DupeCheck;
    QTimer* DupeTimer;
    const char* DupeCheckIcon;
    WMTreeItem* currentTrackListItem;
    QWidget* searchWindow;



    bool eventFilter(QObject* target, QEvent* event);
    bool consumeEvent(QObject* target, QEvent* event);
    void InstallFilter(QObject* object, int id);
    void CheckDupes();

    bool checkKeyPress(QObject* target, int key, int modifier);
    bool on_length_FocusOut();
    void UpdateFkeys();
    void MoveToNextWidget();

public slots:
    void on_DupeTimer_update();

signals:
    void escPressed();
    void pageDownPressed();
    void pageUpPressed();
    void save();
    void updateFkeys(int position);

};

#endif // EDITTRACK_H
