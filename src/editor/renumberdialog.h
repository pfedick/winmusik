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


#ifndef RENUMBERDIALOG_H_
#define RENUMBERDIALOG_H_

#include "winmusik3.h"
#include <QDialog>
#include "ui_renumberdialog.h"

class RenumberDialog : public QDialog
{
    Q_OBJECT

public:
    RenumberDialog(QWidget *parent = 0, CWmClient *wm = NULL);
    ~RenumberDialog();

    void setOldNumber(int number);
    void setNewNumber(int number);
    int getOldNumber() const;
    int getNewNumber() const;

    bool isTargetPathUsable(ppluint8 DeviceType, ppluint32 newDeviceId) const;
    bool isTargetDeviceFree(ppluint8 DeviceType, ppluint32 newDeviceId) const;

private:
    Ui::RenumberDialogClass ui;
    CWmClient *wm;

public slots:

	void on_okButton_clicked();
	void on_cancelButton_clicked();
};






#endif /* SHORTCUTDIALOG_H_ */
