/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: SearchlistDialog.cpp,v 1.1 2010/11/14 13:20:11 pafe Exp $
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
#include "../include/searchlistdialog.h"

SearchlistDialog::SearchlistDialog(QWidget *parent, CWmClient *wm)
    : QWidget(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	setAttribute(Qt::WA_DeleteOnClose,true);
}

SearchlistDialog::~SearchlistDialog()
{
	if (wm) {
		wm->PlaylistOverviewClosed(this);
	}
}


void SearchlistDialog::Resize()
{

}

void SearchlistDialog::showEvent(QShowEvent * event)
{
	Resize();
	QWidget::showEvent(event);
}
void SearchlistDialog::resizeEvent(QResizeEvent * event)
{
	Resize();
	QWidget::resizeEvent(event);
}



void SearchlistDialog::ReloadTranslation()
{
	ui.retranslateUi(this);
}
