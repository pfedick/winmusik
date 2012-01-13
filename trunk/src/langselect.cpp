/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:30 $
 * $Id: langselect.cpp,v 1.1 2010/05/16 12:23:30 pafe Exp $
 *
 *
 * Copyright (c) 2009 Patrick Fedick
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

#include "../include/langselect.h"

LangSelect::LangSelect(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

LangSelect::~LangSelect()
{

}

void LangSelect::Select(QString lang)
{
	if (lang=="de") {
		ui.lang_de->setChecked(true);
		ui.label->setText("Bitte wählen Sie Ihre gewünschte Sprache aus:");
	} else {
		ui.lang_en->setChecked(true);
		ui.label->setText("Please select your preferred language:");
	}
}

void LangSelect::on_okButton_clicked()
{
	if (ui.lang_de->isChecked()) Lang="de";
	else Lang="en";
	done(1);
}

QString LangSelect::GetSelected()
{
	return Lang;
}
