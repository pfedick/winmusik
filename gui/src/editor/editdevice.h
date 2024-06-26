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

#ifndef EDITDEVICE_H
#define EDITDEVICE_H

#include "winmusik3.h"
#include "tablecontrol.h"
#include <QDialog>
#include <QKeyEvent>
#include "ui_editdevice.h"

class EditDevice : public QDialog
{
    Q_OBJECT

public:
    EditDevice(QWidget* parent = 0, CWmClient* wm=NULL, int typ=0, uint32_t DeviceId=0);
    ~EditDevice();

private:
    Ui::EditDeviceClass ui;
    CWmClient* wm;
    QWidget* parent;
    int DeviceType;
    int DeviceId;
    DataDevice	Device;

    int position, oldposition;

    CTableControl Label;
    CTableControl PurchaseSource;



    void InstallFilter(QObject* object, int id);
    bool eventFilter(QObject* target, QEvent* event);
    bool consumeEvent(QObject* target, QEvent* event);

    bool Save();

    // Events
    bool nextWidget(QObject* object);


private slots:

    void on_okButton_clicked();
    void on_cancelButton_clicked();

};

#endif // EDITDEVICE_H
