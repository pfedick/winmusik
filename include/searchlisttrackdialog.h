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


#ifndef SEARCHLISTTRACKDIALOG_H
#define SEARCHLISTTRACKDIALOG_H

#include <QtGui/QWidget>
#include <QTimer>
#include "ui_searchlisttrackdialog.h"
#include "winmusik3.h"
#include "csearchlist.h"


class SearchlistTrackDialog : public QDialog
{
    Q_OBJECT

public:
    SearchlistTrackDialog(QWidget *parent = 0);
    ~SearchlistTrackDialog();

    void set(const SearchlistItem &track);
    SearchlistItem get() const;


private:
    SearchlistItem Track;
    Ui::SearchlistTrackDialogClass ui;
    CWmClient *wm;
    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent * event);
    void Resize();



public slots:
	void on_saveButton_clicked();
	void on_cancelButton_clicked();

};


#endif // SEARCHLISTTRACKDIALOG_H
