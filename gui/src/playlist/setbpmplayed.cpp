/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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


#include "setbpmplayed.h"
#include "winmusik3.h"


SetBPMPlayed::SetBPMPlayed(QWidget *parent)
:QDialog(parent)
{
	ui.setupUi(this);
}

SetBPMPlayed::~SetBPMPlayed()
{

}

void SetBPMPlayed::setValue(int bpm)
{
	ui.bpmValue->setValue(bpm);
	ui.bpmValue->selectAll();
}

int SetBPMPlayed::getValue() const
{
	return ui.bpmValue->value();
}


void SetBPMPlayed::on_okButton_clicked()
{
	done(1);
}

void SetBPMPlayed::on_cancelButton_clicked()
{
	done(0);
}
