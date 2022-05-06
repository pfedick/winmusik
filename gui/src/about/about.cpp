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



#include "winmusik3.h"
#include "version.h"
#include "about.h"

About::About(QWidget* parent)
	: QDialog(parent)
{
	ppl7::String Tmp, DateFormat, Date;
	ui.setupUi(this);
	this->setWindowTitle(tr("About WinMusik"));

	//ui.note->setText(tr("This program makes use of the following Open Source Libraries:"));

	Tmp="<div align=\"center\" style=\"font-size: 12pt;\"><b>" + tr("License:") + "</b></div>\n";
	Tmp.appendf("%s<p>\n", WM_COPYRIGHT);
	Tmp+=ppl7::String(tr("GPL3 short"));

	Tmp+="<div align=\"center\" style=\"font-size: 12pt;\"><b>" + ppl7::String(tr("Credits:")) + "</b></div>\n";
	Tmp+=ppl7::String(tr("This program makes use of the following Open Source Libraries:"));
	Tmp.append("<ul>");
	Tmp.appendf("<li>PPL %d.%d.%d, %s</li>\n",
		PPL7_VERSION_MAJOR,
		PPL7_VERSION_MINOR,
		PPL7_VERSION_BUILD,
		PPL7_COPYRIGHT);
	Tmp.appendf("<li>Qt %s", qVersion());


	Tmp.append("</li>\n");
	Tmp.append("</ul>\n");
	Tmp+=ppl7::String(tr("Some graphics and icons are made with Lightwave 3D from NewTek and Paint Shop Pro from Jasc Software. "));
	Tmp+=" ";
	Tmp+=tr("Some icons are taken from the Crystal Project from Everaldo.com.");

	ui.libs->setTextFormat(Qt::RichText);
	ui.libs->setText(Tmp);

	Tmp=tr("WinMusik Version %v% build %b% vom %d%");
	Tmp.replace("%v%", WM_VERSION);
	Tmp.replace("%b%", WM_REVISION);
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

	ui.wmVersion->setText(Tmp);


	Tmp=WM_COPYRIGHT;
	ui.wmCopyright->setText(Tmp);

}

About::~About()
{

}
