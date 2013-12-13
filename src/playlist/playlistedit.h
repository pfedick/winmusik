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


#ifndef PLAYLISTEDIT_H
#define PLAYLISTEDIT_H

#include <QtGui/QDialog>
#include <QTreeWidgetItem>
#include "ui_playlistedit.h"
#include "winmusik3.h"
#include "playlisttracks.h"


class PlaylistEdit : public QDialog
{
    Q_OBJECT
public:
    PlaylistEdit(QWidget *parent = 0, CWmClient *wm=NULL);
    ~PlaylistEdit();
    void ReloadTranslation();
    void filloutFields(PlaylistItem *item);
    void storeFileds(PlaylistItem *item);

private:
    Ui::playlistEditClass ui;
    CWmClient *wm;
    void closeEvent(QCloseEvent *event);
    void updateTotalTime();
    int getSecondsFromLine(QLineEdit *line);

public slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();

	void on_trackStart_editingFinished() { updateTotalTime(); }
	void on_trackEnd_editingFinished() { updateTotalTime(); }
	void on_cutStart0_editingFinished() { updateTotalTime(); }
	void on_cutStart1_editingFinished() { updateTotalTime(); }
	void on_cutStart2_editingFinished() { updateTotalTime(); }
	void on_cutStart3_editingFinished() { updateTotalTime(); }
	void on_cutStart4_editingFinished() { updateTotalTime(); }
	void on_cutEnd0_editingFinished() { updateTotalTime(); }
	void on_cutEnd1_editingFinished() { updateTotalTime(); }
	void on_cutEnd2_editingFinished() { updateTotalTime(); }
	void on_cutEnd3_editingFinished() { updateTotalTime(); }
	void on_cutEnd4_editingFinished() { updateTotalTime(); }


};


#endif // PLAYLISTEDIT_H
