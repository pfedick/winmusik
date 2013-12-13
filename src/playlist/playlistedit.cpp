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
#include <QFileDialog>

#include "playlisttracks.h"
#include "playlistedit.h"
#include <stdio.h>

PlaylistEdit::PlaylistEdit(QWidget *parent, CWmClient *wm)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	restoreGeometry(wm->GetGeometry("playlistedit"));
}


PlaylistEdit::~PlaylistEdit()
{

}

void PlaylistEdit::closeEvent(QCloseEvent *event)
{
	if (wm) {
		wm->SaveGeometry("playlistedit",saveGeometry());
	}
	QDialog::closeEvent(event);
}

void PlaylistEdit::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void PlaylistEdit::on_okButton_clicked()
{
	done(1);
}

void PlaylistEdit::on_cancelButton_clicked()
{
	done(0);
}

void PlaylistEdit::filloutFields(PlaylistItem *item)
{
	ui.artist->setText(item->Artist);
	ui.title->setText(item->Title);
	ui.version->setText(item->Version);
	ui.genre->setText(item->Genre);
	ui.labelName->setText(item->Label);
	ui.album->setText(item->Album);
	ui.remarks->setText(item->Remarks);
	ui.bpm->setText(ppl6::ToString("%i",item->bpm));
	ui.musicKey->setText(DataTitle::keyName(item->musicKey));

	ui.rating->setCurrentIndex(item->rating);

	ui.trackStart->setText(ppl6::ToString("%0d:%02d",(int)(item->startPositionSec/60),item->startPositionSec%60));
	ui.trackEnd->setText(ppl6::ToString("%0d:%02d",(int)(item->endPositionSec/60),item->endPositionSec%60));

	ui.cutStart0->setText(ppl6::ToString("%0d:%02d",(int)(item->cutStartPosition[0]/60),item->cutStartPosition[0]%60));
	ui.cutStart1->setText(ppl6::ToString("%0d:%02d",(int)(item->cutStartPosition[1]/60),item->cutStartPosition[1]%60));
	ui.cutStart2->setText(ppl6::ToString("%0d:%02d",(int)(item->cutStartPosition[2]/60),item->cutStartPosition[2]%60));
	ui.cutStart3->setText(ppl6::ToString("%0d:%02d",(int)(item->cutStartPosition[3]/60),item->cutStartPosition[3]%60));
	ui.cutStart4->setText(ppl6::ToString("%0d:%02d",(int)(item->cutStartPosition[4]/60),item->cutStartPosition[4]%60));

	ui.cutEnd0->setText(ppl6::ToString("%0d:%02d",(int)(item->cutEndPosition[0]/60),item->cutEndPosition[0]%60));
	ui.cutEnd1->setText(ppl6::ToString("%0d:%02d",(int)(item->cutEndPosition[1]/60),item->cutEndPosition[1]%60));
	ui.cutEnd2->setText(ppl6::ToString("%0d:%02d",(int)(item->cutEndPosition[2]/60),item->cutEndPosition[2]%60));
	ui.cutEnd3->setText(ppl6::ToString("%0d:%02d",(int)(item->cutEndPosition[3]/60),item->cutEndPosition[3]%60));
	ui.cutEnd4->setText(ppl6::ToString("%0d:%02d",(int)(item->cutEndPosition[4]/60),item->cutEndPosition[4]%60));
	updateTotalTime();
}

void PlaylistEdit::storeFileds(PlaylistItem *item)
{
	item->Artist=ui.artist->text().trimmed();
	item->Title=ui.title->text().trimmed();
	item->Version=ui.version->text().trimmed();
	item->Genre=ui.genre->text().trimmed();
	item->Label=ui.labelName->text().trimmed();
	item->Album=ui.album->text().trimmed();
	item->Remarks=ui.remarks->text().trimmed();
	item->musicKey=DataTitle::keyId(ui.artist->text().trimmed());
	item->bpm=ui.artist->text().trimmed().toInt();
	item->rating=ui.rating->currentIndex();
	item->startPositionSec=getSecondsFromLine(ui.trackStart);
	item->endPositionSec=getSecondsFromLine(ui.trackEnd);
	item->cutStartPosition[0]=getSecondsFromLine(ui.cutStart0);
	item->cutStartPosition[1]=getSecondsFromLine(ui.cutStart1);
	item->cutStartPosition[2]=getSecondsFromLine(ui.cutStart2);
	item->cutStartPosition[3]=getSecondsFromLine(ui.cutStart3);
	item->cutStartPosition[4]=getSecondsFromLine(ui.cutStart4);
	item->cutEndPosition[0]=getSecondsFromLine(ui.cutEnd0);
	item->cutEndPosition[1]=getSecondsFromLine(ui.cutEnd1);
	item->cutEndPosition[2]=getSecondsFromLine(ui.cutEnd2);
	item->cutEndPosition[3]=getSecondsFromLine(ui.cutEnd3);
	item->cutEndPosition[4]=getSecondsFromLine(ui.cutEnd4);
}

int PlaylistEdit::getSecondsFromLine(QLineEdit *line)
{
	ppl6::CTok Token(line->text(),":");
	return ppl6::atoi(Token.Get(0))*60+ppl6::atoi(Token.Get(1));
}

void PlaylistEdit::updateTotalTime()
{
	int length=getSecondsFromLine(ui.trackEnd)-getSecondsFromLine(ui.trackStart);
	length-=(getSecondsFromLine(ui.cutEnd0)-getSecondsFromLine(ui.cutStart0));
	length-=(getSecondsFromLine(ui.cutEnd1)-getSecondsFromLine(ui.cutStart1));
	length-=(getSecondsFromLine(ui.cutEnd2)-getSecondsFromLine(ui.cutStart2));
	length-=(getSecondsFromLine(ui.cutEnd3)-getSecondsFromLine(ui.cutStart3));
	length-=(getSecondsFromLine(ui.cutEnd4)-getSecondsFromLine(ui.cutStart4));
	ui.totalTime->setText(ppl6::ToString("%0d:%02d",(int)(length/60),length%60));
}
