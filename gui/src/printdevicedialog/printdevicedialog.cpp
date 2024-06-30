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


#include "printdevicedialog.h"

PrintDeviceDialog::PrintDeviceDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

PrintDeviceDialog::~PrintDeviceDialog()
{

}

void PrintDeviceDialog::setDeviceType(int Type)
{
	if (Type == 2 || Type == 7) {
		ui.printCoverButton->setEnabled(true);
		ui.printCoverButton->setChecked(true);
	} else {
		ui.printCoverButton->setEnabled(false);
		ui.printTracklistButton->setChecked(true);

	}
}
