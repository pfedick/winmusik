/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.4 $
 * $Date: 2010/11/28 15:47:58 $
 * $Id: menue.h,v 1.4 2010/11/28 15:47:58 pafe Exp $
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


#ifndef MENUE_H
#define MENUE_H
#include "winmusik3.h"
#include <QtGui/QWidget>
#include "ui_menue.h"
#include <QTimer>

class Menue : public QWidget
{
    Q_OBJECT

public:
    Menue(QWidget *parent = 0, CWmClient *client=NULL);
    ~Menue();


    void closeEvent(QCloseEvent *event);
    void ReloadTranslation();
    void UpdateMenue();


private:
    Ui::MenueClass ui;
    CWmClient *wm;
    QTimer *timer;

    bool UpdateInformed;

    void OpenEditDialog(int traeger);
    void OpenDeviceList(int traeger);
    void OpenSearchDialog();
    void OpenPropertiesDialog();
    void OpenAboutDialog();
    void OpenCoverPrinterDialog();
    void OpenPlaylistDialog();

private slots:
	void on_toolButton_1_clicked() {OpenEditDialog(1);};
	void on_toolButton_2_clicked() {OpenEditDialog(2);};
	void on_toolButton_3_clicked() {OpenEditDialog(3);};
	void on_toolButton_4_clicked() {OpenEditDialog(4);};
	void on_toolButton_5_clicked() {OpenEditDialog(5);};
	void on_toolButton_6_clicked() {OpenEditDialog(6);};
	void on_toolButton_7_clicked() {OpenEditDialog(7);};
	void on_toolButton_8_clicked() {OpenEditDialog(8);};
	void on_toolButton_9_clicked() {OpenEditDialog(9);};

	void on_toolButton_1_controlClicked() {OpenDeviceList(1);};
	void on_toolButton_2_controlClicked() {OpenDeviceList(2);};
	void on_toolButton_3_controlClicked() {OpenDeviceList(3);};
	void on_toolButton_4_controlClicked() {OpenDeviceList(4);};
	void on_toolButton_5_controlClicked() {OpenDeviceList(5);};
	void on_toolButton_6_controlClicked() {OpenDeviceList(6);};
	void on_toolButton_7_controlClicked() {OpenDeviceList(7);};
	void on_toolButton_8_controlClicked() {OpenDeviceList(8);};
	void on_toolButton_9_controlClicked() {OpenDeviceList(9);};


	void on_searchButton_clicked() {OpenSearchDialog();};
	void on_propertiesButton_clicked() {OpenPropertiesDialog();};
	void on_aboutButton_clicked() {OpenAboutDialog();};
	void on_coverButton_clicked() {OpenCoverPrinterDialog();};
	void on_playlistButton_clicked() {OpenPlaylistDialog();};

	void on_TimerUpdate();


};

#endif // MENUE_H
