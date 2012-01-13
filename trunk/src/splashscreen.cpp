/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: splashscreen.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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


#include "../include/splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	ppl6::CString Tmp,DateFormat,Date;

	Tmp=tr("WinMusik Version %v% vom %d%");
	Tmp.Replace("%v%",WM_VERSION);
	DateFormat=tr("%d.%m.%Y");

	int a,year,month,day;
	a=WM_RELEASEDATE;
	year=a/10000;
	a=a-year*10000;
	month=a/100;
	day=a-month*100;
	ppluint64 t=ppl6::MkTime(year,month,day);
	ppl6::MkDate(Date,DateFormat,t);

	Tmp.Replace("%d%",(const char*)Date);

	Tmp.Concat(", ");
	Tmp+=WM_COPYRIGHT;
	ui.copyright->setText(Tmp);
}

SplashScreen::~SplashScreen()
{

}

void SplashScreen::setMessage(QString msg)
{
	ui.loading->setText(msg);
	qApp->processEvents();
}

void SplashScreen::Update()
{
	ui.progressBar->setValue(progress);
	qApp->processEvents();
}
