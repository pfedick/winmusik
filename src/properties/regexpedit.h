/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/10/10 10:31:02 $
 * $Id: properties.h,v 1.5 2010/10/10 10:31:02 pafe Exp $
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


#ifndef REGEXPEDIT_H
#define REGEXPEDIT_H

#include "winmusik3.h"
#include <QtGui/QDialog>
#include "ui_regexpedit.h"


class RegExpEdit : public QDialog
{
    Q_OBJECT

public:
    RegExpEdit(QWidget *parent = 0);
    ~RegExpEdit();
    void setPattern(const RegExpPattern &pat);
    RegExpPattern getPattern() const;

private:
    Ui::RegExpEditClass ui;

    int checkValues();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_pattern_textChanged();
    void on_teststring_textChanged();

    void on_capture_artist_valueChanged() {on_teststring_textChanged();};
    void on_capture_title_valueChanged() {on_teststring_textChanged();};
    void on_capture_version_valueChanged() {on_teststring_textChanged();};
    void on_capture_genre_valueChanged() {on_teststring_textChanged();};
    void on_capture_label_valueChanged() {on_teststring_textChanged();};
    void on_capture_bpm_valueChanged() {on_teststring_textChanged();};
    void on_capture_album_valueChanged() {on_teststring_textChanged();};
    void on_capture_hours_valueChanged() {on_teststring_textChanged();};
    void on_capture_minutes_valueChanged() {on_teststring_textChanged();};
    void on_capture_seconds_valueChanged() {on_teststring_textChanged();};


};

#endif // REGEXPEDIT_H
