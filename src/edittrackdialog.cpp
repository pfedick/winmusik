/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: playlists.cpp,v 1.1 2010/11/14 13:20:11 pafe Exp $
 *
 *
 * Copyright (c) 2012 Patrick Fedick
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
#include "../include/edittrackdialog.h"

EditTrackDialog::EditTrackDialog(QWidget *parent, CWmClient *wm)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	searchWindow=NULL;
	position=oldposition=0;
	ui.editor->setWinMusikClient(wm);
	ui.editor->setFkeys(ui.fkeys);
	ui.editor->setFocus();

}

EditTrackDialog::~EditTrackDialog()
{
	//printf ("EditTrackDialog::~EditTrackDialog\n");
}

void EditTrackDialog::setSearchWindow(QWidget *widget)
{
	searchWindow=widget;
}

QWidget *EditTrackDialog::getSearchWindow() const
{
	return searchWindow;
}


void EditTrackDialog::Resize()
{
}

void EditTrackDialog::showEvent(QShowEvent * event)
{
	Resize();
	QDialog::showEvent(event);
}

void EditTrackDialog::resizeEvent(QResizeEvent * event)
{
	Resize();
	QDialog::resizeEvent(event);
}



void EditTrackDialog::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void EditTrackDialog::setFilename(const ppl6::CString &Filename)
{
	ui.editor->setFilename(Filename);
}

void EditTrackDialog::setData(const TrackInfo &data)
{
	ui.editor->setData(data);
}

const TrackInfo & EditTrackDialog::getData()
{
	return ui.editor->getData();
}

void EditTrackDialog::on_editor_escPressed()
{
	done(0);
}

void EditTrackDialog::on_editor_save()
{
	done(1);
}

