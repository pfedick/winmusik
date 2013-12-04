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
#include "ui_playlist.h"
#include "winmusik3.h"

class Playlist : public QMainWindow
{
    Q_OBJECT

public:
    Playlist(QWidget *parent = 0, CWmClient *wm=NULL);
    ~Playlist();
    void ReloadTranslation();

private:
    Ui::playlistClass ui;
    CWmClient *wm;

    QMenu	*menuFile;

    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent * event);
    void Resize();


public slots:

	void on_menuNew_triggered();
	void on_menuOpen_triggered();
	void on_menuSave_triggered();
	void on_menuSaveAs_triggered();

};


#endif // PLAYLIST_H
