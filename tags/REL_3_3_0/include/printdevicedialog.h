/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: printdevicedialog.h,v 1.2 2010/05/16 12:40:40 pafe Exp $
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


#ifndef PRINTDEVICEDIALOG_H
#define PRINTDEVICEDIALOG_H

#include "winmusik3.h"
#include <QDialog>
#include "ui_printdevicedialog.h"

class PrintDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    PrintDeviceDialog(QWidget *parent = 0);
    ~PrintDeviceDialog();

    void setDeviceRange(int min, int max) {
    	ui.fromDevice->setMinimum(min);
    	ui.toDevice->setMinimum(min);
    	ui.fromDevice->setMaximum(max);
    	ui.toDevice->setMaximum(max);
    }

    void setUseColor(bool flag) {
    	ui.printColors->setChecked(flag);
    }

    void setFont(ppl6::CString Font) {
    	int index=ui.font->findText(Font);
    	if (index>=0) ui.font->setCurrentIndex(index);
    }

    void setDeviceType(int Type);

    bool printCover() {
    	return ui.printCoverButton->isChecked();
    }

    void setFromDevice(int id) {
    	ui.fromDevice->setValue(id);
    }
    void setToDevice(int id) {
    	ui.toDevice->setValue(id);
    }

    int min() {
    	return ui.fromDevice->value();
    }

    int max() {
    	return ui.toDevice->value();
    }

    bool useColor() {
    	return ui.printColors->isChecked();
    }

    ppl6::CString font() {
    	return ui.font->currentText();
    }


private:
    Ui::PrintDeviceDialogClass ui;

public slots:
	void on_okButton_clicked() {
		done(1);
	}
	void on_cancelButton_clicked() {
		done(0);
	}

};

#endif // PRINTDEVICEDIALOG_H
