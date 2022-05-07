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


#include "splashscreen.h"
#include "version.h"

SplashScreen::SplashScreen(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ppl7::String Tmp, DateFormat, Date;

	Tmp=tr("WinMusik Version %v% vom %d%");
	Tmp.replace("%v%", WM_VERSION);
	DateFormat=tr("%d.%m.%Y");

	int a, year, month, day;
	a=WM_RELEASEDATE;
	year=a / 10000;
	a=a - year * 10000;
	month=a / 100;
	day=a - month * 100;
	ppl7::ppl_time_t t=ppl7::MkTime(year, month, day);
	Date=ppl7::MkDate(DateFormat, t);

	Tmp.replace("%d%", (const char*)Date);

	Tmp.append(", ");
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
