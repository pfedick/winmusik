/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: langselect.h,v 1.2 2010/05/16 12:40:40 pafe Exp $
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

#ifndef LANGSELECT_H
#define LANGSELECT_H

#include <QtGui/QDialog>
#include "ui_langselect.h"

class LangSelect : public QDialog
{
    Q_OBJECT

public:
    LangSelect(QWidget *parent = 0);
    ~LangSelect();
    void Select(QString lang);
    QString GetSelected();

private:
    Ui::LangSelectClass ui;
    QString Lang;

private slots:
    void on_okButton_clicked();

};

#endif // LANGSELECT_H