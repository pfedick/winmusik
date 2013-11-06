/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: about.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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
#include "version.h"
#include "../include/about.h"

About::About(QWidget *parent)
    : QDialog(parent)
{
	ppl6::CString Tmp,DateFormat,Date;
	ui.setupUi(this);
	ppl6::Cppl6Core *core=ppl6::PPLInit();
	this->setWindowTitle(tr("About WinMusik"));

	//ui.note->setText(tr("This program makes use of the following Open Source Libraries:"));

	Tmp="<div align=\"center\" style=\"font-size: 12pt;\"><b>"+tr("License:")+"</b></div>\n";
	Tmp.Concatf("%s<p>\n",WM_COPYRIGHT);
	Tmp+=tr("GPL3 short");

	Tmp+="<div align=\"center\" style=\"font-size: 12pt;\"><b>"+tr("Credits:")+"</b></div>\n";
	Tmp+=tr("This program makes use of the following Open Source Libraries:");
	Tmp.Concat("<ul>");
	Tmp.Concatf("<li>PPL %s, %s</li>\n",(const char*)core->GetVersion(), (const char*)core->GetCopyright());
	Tmp.Concatf("<li>Qt %s, Copyright (C) 2012 Digia Plc",qVersion());
	AddFeature(Tmp,"zlib");
	AddFeature(Tmp,"bzip2");
	AddFeature(Tmp,"pcre");
	AddFeature(Tmp,"iconv");
	//AddFeature(Tmp,"lame");
	AddFeature(Tmp,"openssl");
	AddFeature(Tmp,"curl");
	//AddFeature(Tmp,"libmad");


	Tmp.Concat("</li>\n");
	Tmp.Concat("</ul>\n");
	Tmp+=tr("Some graphics and icons are made with Lightwave 3D 9.6 from NewTek and Paint Shop Pro 7.0 from Jasc Software. ");
	Tmp+=" ";
	Tmp+=tr("Some icons are taken from the Crystal Project from Everaldo.com.");

	ui.libs->setTextFormat(Qt::RichText);
	ui.libs->setText(Tmp);

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

	ui.wmVersion->setText(Tmp);


	Tmp=WM_COPYRIGHT;
	ui.wmCopyright->setText(Tmp);

}

About::~About()
{

}

void About::AddFeature(ppl6::CString &out, const char *feature)
{
	ppl6::CAssocArray ppl, *row;
	ppl6::Cppl6Core::GetFeatures(ppl);
	row=ppl.GetArray(feature);
	if (row) out.Concatf("<li>%s, Version %s, %s</li>\n",row->Get("name"),row->Get("version"),row->Get("copyright"));
}
