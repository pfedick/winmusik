/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: playlists.cpp,v 1.1 2010/11/14 13:20:11 pafe Exp $
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


#include <QClipboard>
#include <QMenu>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QMouseEvent>
#include "../include/massimport.h"

MassImport::MassImport(QWidget *parent, CWmClient *wm)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
}

MassImport::~MassImport()
{
}

void MassImport::Resize()
{

}

void MassImport::showEvent(QShowEvent * event)
{
	Resize();
	QDialog::showEvent(event);
}

void MassImport::resizeEvent(QResizeEvent * event)
{
	Resize();
	QDialog::resizeEvent(event);
}



void MassImport::ReloadTranslation()
{
	ui.retranslateUi(this);
}

int MassImport::load(ppluint8 DeviceType, ppluint32 DeviceId, ppluint8 Page, ppluint16 StartTrack)
{
	this->DeviceType=DeviceType;
	this->DeviceId=DeviceId;
	this->Page=Page;
	this->StartTrack=StartTrack;

	ppl6::CString Path, Filename, Pattern;
	ppl6::CString MP3Path=wm->conf.DevicePath[DeviceType];

	if (MP3Path.IsEmpty()) return 0;
	Path=MP3Path;
	Path.RTrim("/");
	Path.RTrim("\\");
	Path.Concatf("/%02u/%03u/",(ppluint32)(DeviceId/100),DeviceId);
	Pattern.Setf("*.mp3");
	ppl6::CDir Dir;
	ppl6::CDirEntry *entry;
	int count=0;
	if (Dir.Open(Path,ppl6::CDir::Sort_Filename_IgnoreCase)) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"CWMClient","NextMP3File",__FILE__,__LINE__,"%i Dateien vorhanden, suche nach Pattern...",Dir.Num());
		while ((entry=Dir.GetNextPattern(Pattern,true))) {
			Filename=entry->Filename;
			// Der Dateiname darf nicht mit drei Ziffern und Bindestrich beginnen
			if (!Filename.PregMatch("/^[0-9]{3}\\-.*/")) {
				// Muss aber mit .mp3 enden
				if (Filename.PregMatch("/^.*\\.mp3$/i")) {
					//printf ("%s\n",(const char*)Filename);
					addTrack(entry->File);
					count++;
				}
			}
		}
	}
	if (count) return 1;
	QMessageBox::information(this, tr("WinMusik: Notice"),
			tr("There are no tracks to import"));
	return 0;

}

void MassImport::addTrack(const ppl6::CString Filename)
{
	printf ("%s\n",(const char*)Filename);
}
