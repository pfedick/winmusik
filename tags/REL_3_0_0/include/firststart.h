/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:29 $
 * $Id: firststart.h,v 1.1 2010/05/16 12:23:29 pafe Exp $
 *
 *
 * Copyright (c) 2009 Patrick Fedick
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

#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include "winmusik3.h"
#include "wm20import.h"

#include <QtGui/QDialog>
#include "ui_firststart.h"

class FirstStart;

class FirstStartImportProgress: public ImportProgress
{
	public:
		FirstStart *widget;
		virtual void Update();
};

class FirstStart : public QDialog
{
    Q_OBJECT

public:
    FirstStart(QWidget *parent = 0, CWmClient *wm=NULL);
    ~FirstStart();

    int UseExistingInstallation(ppl6::CString *Path);
    //int ValidInstallation(ppl6::CString *Path);

    void UpdateProgress(FirstStartImportProgress *p);

private:
    Ui::FirstStartClass ui;
    int pageplan[10];
    int page;
    int maxpageplan;

    CWmClient *wm;

    void DoImport();

private slots:
    void on_buttonWeiter_clicked();
    void on_buttonZurueck_clicked();
    void on_buttonAbbrechen_clicked();
    void on_selectrecoverpath_clicked();
    void on_selectdatapath_clicked();
    void on_selectimportpath_clicked();
    void on_localdatapath_textChanged();

};



#endif // FIRSTSTART_H
