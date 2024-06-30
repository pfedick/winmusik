/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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


#ifndef SETBPMPLAYED_H
#define SETBPMPLAYED_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "ui_setbpmplayed.h"
#include "winmusik3.h"




class SetBPMPlayed : public QDialog
{
    Q_OBJECT
public:
    SetBPMPlayed(QWidget* parent = 0);
    ~SetBPMPlayed();
    void setValue(int bpm);
    int getValue() const;
private:
    Ui::setBPMPlayedClass ui;

public slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};


#endif // SETBPMPLAYED_H
