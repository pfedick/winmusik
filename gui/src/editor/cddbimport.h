/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: shortcutdialog.h,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2014 Patrick Fedick
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


#ifndef CDDBIMPORT_H_
#define CDDBIMPORT_H_

#include "winmusik3.h"
#include "tablecontrol.h"
#include <QDialog>

#include "ppl6-sound.h"
#include "ui_cddbimport.h"

class CDDBImport : public QDialog
{
    Q_OBJECT

public:
    CDDBImport(QWidget *parent = 0, CWmClient *wm = NULL);
    ~CDDBImport();

    void setDisc(ppl6::CDDB::Disc &disc);

    bool checkAndConfirmOverwrite(ppluint8 devicetype, ppluint32 deviceid, ppluint8 page);
    void startImport(ppl6::CDDB::Disc &disc, ppluint8 devicetype, ppluint32 deviceid, ppluint8 page);

private:
    Ui::CDDBImport ui;
    CWmClient *wm;
    int position, oldposition;
    CTableControl TCGenre;
    CTableControl TCLabel;
    CTableControl TCRecordSource;
    CTableControl TCRecordDevice;

    void InstallFilter(QObject *object, int id);
    bool eventFilter(QObject *target, QEvent *event);
    bool consumeEvent(QObject *target, QEvent *event);
    void resizeEvent ( QResizeEvent * event );
    ppl6::CString getVersionFromTitle(ppl6::CString &Title, int length);

    // Globale Events
    bool on_KeyPress(QObject *target, int key,int modifier);
    void MoveToNextWidget();
    QWidget *GetWidgetFromPosition(int position);
    void FixFocus();

    void updateTracklist(ppl6::CDDB::Disc &disc);
    void getTitle(DataTitle &Ti, const ppl6::CDDB::Track &track);
    void addDataFromFile(DataTitle &Ti);
    int saveTitle(CTrackList *tracklist, DataTitle &Ti);

public slots:

	void on_okButton_clicked();
	void on_cancelButton_clicked();
};






#endif /* CDDBIMPORT_H_ */
