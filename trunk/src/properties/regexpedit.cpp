/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/10/10 10:31:01 $
 * $Id: properties.cpp,v 1.5 2010/10/10 10:31:01 pafe Exp $
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


#include "winmusik3.h"

#include "properties.h"
#include "regexpedit.h"

RegExpEdit::RegExpEdit(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

RegExpEdit::~RegExpEdit()
{

}

int RegExpEdit::checkValues()
{
	return 1;
}

void RegExpEdit::on_cancelButton_clicked()
{
	done(0);
}

void RegExpEdit::on_okButton_clicked()
{
	if (!checkValues()) return;
	done(1);
}
