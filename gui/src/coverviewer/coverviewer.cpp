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

#include "coverviewer.h"

CoverViewer::CoverViewer(QWidget* parent, CWmClient* wm)
	: QWidget(parent)
{
	this->wm=wm;
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	this->restoreGeometry(wm->GetGeometry("CoverViewer"));
}

CoverViewer::~CoverViewer()
{
	if (wm) wm->CoverViewerClosed();
}


void CoverViewer::setCover(const QPixmap& pix)
{
	//ui.cover->setPixmap(pix.scaled(ui.cover->width(),ui.cover->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
	ui.cover->setPixmap(pix);
}


void CoverViewer::closeEvent(QCloseEvent* event)
{
	if (wm) {
		wm->SaveGeometry("CoverViewer", this->saveGeometry());
		wm->CoverViewerClosed();
		wm=NULL;
	}
	QWidget::closeEvent(event);
}
