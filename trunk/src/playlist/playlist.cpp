/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: Playlist.cpp,v 1.1 2010/11/14 13:20:11 pafe Exp $
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
#include "playlist.h"

Playlist::Playlist(QWidget *parent, CWmClient *wm)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	setAttribute(Qt::WA_DeleteOnClose,true);

	menuFile=menuBar()->addMenu(QIcon(":/icons/resources/edit.png"),tr("&File"));

	menuFile->addAction(QIcon(":/icons/resources/filenew.png"),tr("&new Playlist"),this,SLOT(on_menuNew_triggered()));
	menuFile->addAction(QIcon(":/icons/resources/fileopen.png"),tr("&load Playlist"),this,SLOT(on_menuOpen_triggered()));
	menuFile->addAction(QIcon(":/icons/resources/filesave.png"),tr("&save Playlist"),this,SLOT(on_menuSave_triggered()));
	menuFile->addAction(QIcon(":/icons/resources/filesaveas.png"),tr("save Playlist &as"),this,SLOT(on_menuSaveAs_triggered()));

}

Playlist::~Playlist()
{
	if (wm) {
		wm->PlaylistClosed(this);
	}
}


void Playlist::Resize()
{

}

void Playlist::showEvent(QShowEvent * event)
{
	Resize();
	QWidget::showEvent(event);
}
void Playlist::resizeEvent(QResizeEvent * event)
{
	Resize();
	QWidget::resizeEvent(event);
}



void Playlist::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void Playlist::on_menuNew_triggered()
{

}

void Playlist::on_menuOpen_triggered()
{

}

void Playlist::on_menuSave_triggered()
{

}

void Playlist::on_menuSaveAs_triggered()
{

}
