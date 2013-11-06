/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/28 15:47:58 $
 * $Id: devicelist.h,v 1.1 2010/11/28 15:47:58 pafe Exp $
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


#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QtGui/QWidget>
#include <QTimer>
#include "ui_devicelist.h"
#include "winmusik3.h"

class DeviceItem : public QTreeWidgetItem
{
	public:
		ppluint32	Id;
};


class DeviceList : public QWidget
{
    Q_OBJECT

public:
    DeviceList(QWidget *parent = 0, CWmClient *wm=NULL, int typ=0);
    ~DeviceList();
    void ReloadTranslation();

private:
    Ui::deviceListClass ui;
    CWmClient *wm;
    int DeviceType;
    void resizeEvent(QResizeEvent * event);
    void showEvent(QShowEvent * event);
    void closeEvent(QCloseEvent *event);
    void Resize();
    bool on_KeyPress(QObject *target, int key,int modifier);

    bool consumeEvent(QObject *target, QEvent *event);

protected:
    bool eventFilter(QObject *target, QEvent *event);


public slots:
	void on_list_itemDoubleClicked ( QTreeWidgetItem * item, int column);
	void on_newButton_clicked();

};


#endif // DEVICELIST_H
