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


#include "searchlistdialog.h"
#include "searchlisttrackdialog.h"
#include "csearchlist.h"

DupeCheckThread::DupeCheckThread(QObject* parent)
	:QThread(parent)
{
	stop=false;
	trackList=0;
}

void DupeCheckThread::setTracklist(QTreeWidget* trackList)
{
	this->trackList=trackList;
}

void DupeCheckThread::run()
{
	for (int i=0;i < trackList->topLevelItemCount();i++) {
		if (stop) return;
		QTreeWidgetItem* item=trackList->topLevelItem(i);
		emit updateItem(item, dupeCheckOnTrack(item));
	}
}

void DupeCheckThread::stopThread()
{
	stop=true;
	while (isRunning()) QThread::msleep(20);
}

void DupeCheckThread::startThread()
{
	if (isRunning()) return;
	stop=false;
	start();
}

int DupeCheckThread::dupeCheckOnTrack(QTreeWidgetItem* tw_item)
{
	SearchlistDialog::SearchlistTreeItem* item=(SearchlistDialog::SearchlistTreeItem*)tw_item;
	// Titel in Datenbank suchen
	CHashes::TitleTree Result;
	int dupePresumption=0;
	wm_main->Hashes.Find(item->Track.Artist, item->Track.Title, item->Track.Version, "", "", "", Result);
	if (Result.size() > 1) {
		dupePresumption=100;
	} else if (Result.size() > 0) {
		dupePresumption=90;
	} else {
		wm_main->Hashes.Find(item->Track.Artist, item->Track.Title, Result);
		if (Result.size() > 3) {
			dupePresumption=70;
		} else if (Result.size() > 0) {
			dupePresumption=40;
		}
	}
	return dupePresumption;
}
