/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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


SearchlistTrackDialog::SearchlistTrackDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->show();
	this->restoreGeometry(wm_main->GetGeometry("SearchlistTrackDialog"));
}

SearchlistTrackDialog::~SearchlistTrackDialog()
{
}

void SearchlistTrackDialog::closeEvent(QCloseEvent* event)
{
	wm_main->SaveGeometry("SearchlistTrackDialog", this->saveGeometry());
	QDialog::closeEvent(event);
}



void SearchlistTrackDialog::Resize()
{

}

void SearchlistTrackDialog::showEvent(QShowEvent* event)
{
	Resize();
	QWidget::showEvent(event);
}
void SearchlistTrackDialog::resizeEvent(QResizeEvent* event)
{
	Resize();
	QWidget::resizeEvent(event);
}


void SearchlistTrackDialog::set(const SearchlistItem& track)
{
	Track=track;
	ui.artistEdit->setText(track.Artist);
	ui.titleEdit->setText(track.Title);
	ui.versionEdit->setText(track.Version);
	ui.genreEdit->setText(track.Genre);
	ui.commentEdit->setText(track.Comment);
	ui.releaseDateEdit->setText(track.ReleaseDate.get("%Y-%m-%d"));
	if (track.Length > 0) ui.lengthEdit->setText(ppl7::ToString("%i:%02i", track.Length / 60, track.Length % 60));
	ui.rating->setCurrentIndex(track.Rating);
}

void SearchlistTrackDialog::setFromClipboard()
{
	RegExpMatch match;
	RegExpClipboard clip;
	copyFromClipboard(clip);
	if (wm_main->RegExpCapture.match(clip, match)) {
		ui.artistEdit->setText(match.Artist);
		ui.titleEdit->setText(match.Title);
		ui.versionEdit->setText(match.Version);
		ui.genreEdit->setText(match.Genre);
		ui.commentEdit->setText(match.Label);
		ui.releaseDateEdit->setText(match.ReleaseDate);
		if (match.Length > 0) ui.lengthEdit->setText(ppl7::ToString("%i:%02i", match.Length / 60, match.Length % 60));
	}
}

SearchlistItem SearchlistTrackDialog::get() const
{
	SearchlistItem track;
	track=Track;	// Bisherige Daten übernehmen, insbesondere Datum der Anlage
	track.Artist=ppl7::Trim(ppl7::String(ui.artistEdit->text()));
	track.Title=ppl7::Trim(ppl7::String(ui.titleEdit->text()));
	track.Version=ppl7::Trim(ppl7::String(ui.versionEdit->text()));
	track.Genre=ppl7::Trim(ppl7::String(ui.genreEdit->text()));
	track.Comment=ppl7::Trim(ppl7::String(ui.commentEdit->text()));
	try {
		track.ReleaseDate.set(ppl7::Trim(ppl7::String(ui.releaseDateEdit->text())));
	} catch (...) {
		track.ReleaseDate.clear();
	}
	track.Rating=ui.rating->currentIndex();
	ppl7::String Tmp=ppl7::Trim(ppl7::String(ui.lengthEdit->text()));
	ppl7::Array matches;
	if (Tmp.pregMatch("/^([0-9]+):([0-9]{2})$/", matches)) {
		track.Length=matches.get(1).toInt() * 60 + matches.get(2).toInt();
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
