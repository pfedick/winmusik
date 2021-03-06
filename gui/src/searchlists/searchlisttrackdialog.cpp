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
#include "searchlisttrackdialog.h"
#include "csearchlist.h"
#include "regexpcapture.h"

SearchlistTrackDialog::SearchlistTrackDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->show();
	this->restoreGeometry(wm_main->GetGeometry("SearchlistTrackDialog"));
}

SearchlistTrackDialog::~SearchlistTrackDialog()
{
}

void SearchlistTrackDialog::closeEvent(QCloseEvent *event)
{
	wm_main->SaveGeometry("SearchlistTrackDialog",this->saveGeometry());
	QDialog::closeEvent(event);
}



void SearchlistTrackDialog::Resize()
{

}

void SearchlistTrackDialog::showEvent(QShowEvent * event)
{
	Resize();
	QWidget::showEvent(event);
}
void SearchlistTrackDialog::resizeEvent(QResizeEvent * event)
{
	Resize();
	QWidget::resizeEvent(event);
}


void SearchlistTrackDialog::set(const SearchlistItem &track)
{
	Track=track;
	ui.artistEdit->setText(track.Artist);
	ui.titleEdit->setText(track.Title);
	ui.versionEdit->setText(track.Version);
	ui.genreEdit->setText(track.Genre);
	ui.commentEdit->setText(track.Comment);
	ui.releaseDateEdit->setText(track.ReleaseDate.get("%Y-%m-%d"));
	if (track.Length>0) ui.lengthEdit->setText(ppl6::ToString("%i:%02i",track.Length/60,track.Length%60));
	ui.rating->setCurrentIndex(track.Rating);
}

void SearchlistTrackDialog::setFromClipboard()
{
	RegExpMatch match;
	RegExpClipboard clip;
	clip.copyFromClipboard();
	if (wm_main->RegExpCapture.match(clip,match)) {
		ui.artistEdit->setText(match.Artist);
		ui.titleEdit->setText(match.Title);
		ui.versionEdit->setText(match.Version);
		ui.genreEdit->setText(match.Genre);
		ui.commentEdit->setText(match.Label);
		ui.releaseDateEdit->setText(match.ReleaseDate);
		if (match.Length>0) ui.lengthEdit->setText(ppl6::ToString("%i:%02i",match.Length/60,match.Length%60));
	}
}

SearchlistItem SearchlistTrackDialog::get() const
{
	SearchlistItem track;
	track=Track;	// Bisherige Daten übernehmen, insbesondere Datum der Anlage
	track.Artist=ppl6::Trim(ppl6::CString(ui.artistEdit->text()));
	track.Title=ppl6::Trim(ppl6::CString(ui.titleEdit->text()));
	track.Version=ppl6::Trim(ppl6::CString(ui.versionEdit->text()));
	track.Genre=ppl6::Trim(ppl6::CString(ui.genreEdit->text()));
	track.Comment=ppl6::Trim(ppl6::CString(ui.commentEdit->text()));
	try {
		track.ReleaseDate.set(ppl6::Trim(ppl6::CString(ui.releaseDateEdit->text())));
	} catch (...) {
		track.ReleaseDate.clear();
	}
	track.Rating=ui.rating->currentIndex();
	ppl6::CString Tmp=ppl6::Trim(ppl6::CString(ui.lengthEdit->text()));
	if (Tmp.PregMatch("/^([0-9]+):([0-9]{2})$/")) {
		track.Length=ppl6::atoi(Tmp.GetMatch(1))*60+ppl6::atoi(Tmp.GetMatch(2));
	} else {
		track.Length=0;
	}
	return track;
}

void SearchlistTrackDialog::on_saveButton_clicked()
{
	done(1);
}

void SearchlistTrackDialog::on_cancelButton_clicked()
{
	done(0);
}

