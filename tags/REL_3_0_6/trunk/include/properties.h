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


#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "winmusik3.h"
#include <QtGui/QDialog>
#include "ui_properties.h"


class Properties : public QDialog
{
    Q_OBJECT

public:
    Properties(QWidget *parent = 0, CWmClient *wm = NULL);
    ~Properties();

private:
    Ui::PropertiesClass ui;
    CWmClient *wm;
    QString LastHDSearchDir;

    void Change();

    int CheckValues();
    int Save();

    ppl6::CString GetLocale();


    void SelectLanguage();


private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_acceptButton_clicked();

    void on_wmDataPathButton_clicked();
    void on_mp3PlayerButton_clicked();
    void on_tmpPathButton_clicked();
    void on_registerButton_clicked();
    void on_checkUpdatesNow_clicked();
    void on_addDirectory_clicked();
    void on_deleteDirectory_clicked();


    // Signals for General Options
    void on_wmDataPath_textChanged(const QString &text);
    void on_tmpPath_textChanged(const QString &text);
    void on_showSplashscreen_stateChanged(int state) {Change();}
    void on_checkForUpdates_stateChanged(int state) {Change();}
    void on_suggestArtist_stateChanged(int state) {Change();}
    void on_suggestTitle_stateChanged(int state) {Change();}
    void on_automaticEditSearch_stateChanged(int state) {Change();}
    void on_printColors_stateChanged(int state) {Change();}
    void on_font_currentIndexChanged(int index) {Change();};

    // Devices
    void on_devicePathButton_clicked(int device, QLineEdit *line);
    void on_devicePathButtonCassette_clicked() { on_devicePathButton_clicked(1, ui.devicePathCassette); };
    void on_devicePathButtonCD_clicked() { on_devicePathButton_clicked(2, ui.devicePathCD); };
    void on_devicePathButtonDataStorage_clicked() { on_devicePathButton_clicked(3, ui.devicePathDataStorage); };
    void on_devicePathButtonRecord_clicked() { on_devicePathButton_clicked(4, ui.devicePathRecord); };
    void on_devicePathButtonVHS_clicked() { on_devicePathButton_clicked(5, ui.devicePathVHS); };
    void on_devicePathButtonOther_clicked() { on_devicePathButton_clicked(6, ui.devicePathOther); };
    void on_devicePathButtonMP3_clicked() { on_devicePathButton_clicked(7, ui.devicePathMP3); };
    void on_devicePathButtonTape_clicked() { on_devicePathButton_clicked(8, ui.devicePathTape); };
    void on_devicePathButtonDVD_clicked() { on_devicePathButton_clicked(9, ui.devicePathDVD); };

    void on_devicePathCassette_textChanged(const QString &text) {Change();};
    void on_devicePathCD_textChanged(const QString &text) {Change();};
    void on_devicePathDataStorage_textChanged(const QString &text) {Change();};
    void on_devicePathRecord_textChanged(const QString &text) {Change();};
    void on_devicePathVHS_textChanged(const QString &text) {Change();};
    void on_devicePathOther_textChanged(const QString &text) {Change();};
    void on_devicePathMP3_textChanged(const QString &text) {Change();};
    void on_devicePathTape_textChanged(const QString &text) {Change();};
    void on_devicePathDVD_textChanged(const QString &text) {Change();};

    // MP3
    void on_mp3Player_textChanged(const QString &text) {Change();};
    void on_writeID3v1_stateChanged(int state) {Change();}
    void on_writeID3v2_stateChanged(int state) {Change();}
    void on_removeOriginalTags_stateChanged(int state) {Change();}
    void on_ID3v2Padding_textChanged(const QString &text) {Change();}
    void on_readID3v2_toggled(bool value) {Change();};
    void on_readID3v1_toggled(bool value) {Change();};
    void on_retryIntervall_valueChanged(const QString &text) {Change();};
    void on_maxFilenameLength_currentIndexChanged(int index) {Change();};

    // User
    void on_name_textChanged(const QString &text) {Change();};
    void on_company_textChanged(const QString &text) {Change();};
    void on_serial_textChanged(const QString &text) {Change();};
    void on_language_currentIndexChanged(int index) {Change();};

    void on_currency_textChanged(const QString &text) {Change();};


    // Debug
    void on_enableDebug_toggled(bool on) {Change();}
    void on_logfile_textChanged(const QString &text) {Change();};
    void on_logfileSize_textChanged(const QString &text) {Change();};
    void on_debuglevel_valueChanged(const QString &text) {Change();};
    void on_generations_valueChanged(const QString &text) {Change();};
    void on_logfileButton_clicked();

    // Devices
    void on_device_1_stateChanged(int state) {Change();}
    void on_device_2_stateChanged(int state) {Change();}
    void on_device_3_stateChanged(int state) {Change();}
    void on_device_4_stateChanged(int state) {Change();}
    void on_device_5_stateChanged(int state) {Change();}
    void on_device_6_stateChanged(int state) {Change();}
    void on_device_7_stateChanged(int state) {Change();}
    void on_device_8_stateChanged(int state) {Change();}
    void on_device_9_stateChanged(int state) {Change();}

    // Server
    void on_buttonSelectServerKeyfile_clicked();
    void on_serverEnable_toggled(bool on) {Change();}
    void on_serverEnableSSL_toggled(bool on) {Change();}
    void on_serverHostname_textChanged(const QString &text) {Change();};
    void on_serverPort_textChanged(const QString &text) {Change();};
    void on_serverKeyfile_textChanged(const QString &text) {Change();};
    void on_serverKeyPassword_textChanged(const QString &text) {Change();};

};

#endif // PROPERTIES_H
