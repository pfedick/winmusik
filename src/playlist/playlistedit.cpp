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
#include <QByteArray>
#include <QBuffer>

#include "playlisttracks.h"
#include "playlistedit.h"
#include <stdio.h>
#include "traktor.h"
#include "winmusik3.h"

PlaylistEdit::PlaylistEdit(QWidget *parent, CWmClient *wm)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	ui.traktorCuesGroupBox->setVisible(false);
	restoreGeometry(wm->GetGeometry("playlistedit"));
	traktorIn=-1;
	traktorOut=-1;

	installFilter(ui.artist,0);
	installFilter(ui.title,0);
	installFilter(ui.version,0);
	installFilter(ui.genre,0);
	installFilter(ui.album,0);
	installFilter(ui.label,0);
	installFilter(ui.bpm,0);
	installFilter(ui.bpmPlayed,0);
	installFilter(ui.musicKey,0);
	installFilter(ui.keyVerified,0);
	installFilter(ui.rating,0);
	installFilter(ui.energyLevel,0);
	installFilter(ui.trackStart,0);
	installFilter(ui.trackEnd,0);
	installFilter(ui.cutStart0,0);
	installFilter(ui.cutStart1,0);
	installFilter(ui.cutStart2,0);
	installFilter(ui.cutStart3,0);
	installFilter(ui.cutStart4,0);
	installFilter(ui.cutEnd0,0);
	installFilter(ui.cutEnd1,0);
	installFilter(ui.cutEnd2,0);
	installFilter(ui.cutEnd3,0);
	installFilter(ui.cutEnd4,0);
	installFilter(ui.traktorUseInOutButton,0);

}


PlaylistEdit::~PlaylistEdit()
{

}


void PlaylistEdit::installFilter(QObject *object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id",id);
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


void PlaylistEdit::on_traktorUseInOutButton_clicked()
{
	if (traktorIn>=0) ui.trackStart->setText(ppl6::ToString("%0d:%02d",(int)(traktorIn/60),traktorIn%60));
	if (traktorOut>=0) ui.trackEnd->setText(ppl6::ToString("%0d:%02d",(int)(traktorOut/60),traktorOut%60));
	updateTotalTime();
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
	ui.bpmPlayed->setText(ppl6::ToString("%i",item->bpmPlayed));
	ui.musicKey->setText(DataTitle::keyName(item->musicKey,wm->conf.musicKeyDisplay));
	ui.keyVerified->setChecked(item->keyVerified);
	ui.energyLevel->setValue(item->energyLevel);

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
	ui.trackLength->setText(ppl6::ToString("%0d:%02d",(int)(item->trackLength/60),item->trackLength%60));

	CoverPreview=item->CoverPreview;
	Filename=item->File;

	ppl6::CID3Tag Tag;
	Tag.SetLogfile(wmlog);

	if(wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"WinMusik","PlaylistEdit::filloutFields",__FILE__,__LINE__,"Try to load ID3-Tags: %s",(const char*)item->File);
	if (!Tag.Load(item->File)) {
		if(wmlog) wmlog->LogError("WinMusik","PlaylistEdit::filloutFields",__FILE__,__LINE__);
		ppl6::CString Tmp=wm->MP3Filename(item->DeviceId,item->DevicePage,item->DeviceTrack);

		if (Tmp.NotEmpty()==true && Tmp!=item->File) {
			item->File=Tmp;
			if(wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"WinMusik","PlaylistEdit::filloutFields",__FILE__,__LINE__,"File seems to be renamed, try to load again: %s",(const char*)item->File);
			if (!Tag.Load(item->File)) {
				if(wmlog) wmlog->LogError("WinMusik","PlaylistEdit::filloutFields",__FILE__,__LINE__);
			}
		}
	}
	if (Tag.FrameCount()>0) {
		if(wmlog) wmlog->Printf(ppl6::LOG::DEBUG,9,"WinMusik","PlaylistEdit::filloutFields",__FILE__,__LINE__,"ID3-Tag with %zd frames loaded",Tag.FrameCount());
		loadCover(Tag);
		loadTraktorCues(Tag);
		ppl6::CString Tmp;
		if (item->bpm==0) {
			// BPM
			Tmp=Tag.GetBPM();
			NormalizeImportString(Tmp);
			ui.bpm->setText(Tmp);
		}
		if (item->musicKey==0) {
			// Music Key
			ui.musicKey->setText(DataTitle::keyName(DataTitle::keyId(Tag.GetKey()),wm->conf.musicKeyDisplay));
		}
	}
	updateTotalTime();
}

void PlaylistEdit::loadCover(const ppl6::CID3Tag &Tag)
{
	ppl6::CBinary cover;
	Tag.GetPicture(3,cover);
	if (cover.Size()>0) {
		Cover.loadFromData((const uchar*)cover.GetPtr(),cover.GetSize());
		updateCover();
	}
}

void PlaylistEdit::updateCover()
{
	ui.cover->setPixmap(Cover.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
	QPixmap icon;
	icon=Cover.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation);
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	icon.save(&buffer, "JPEG",70);
	CoverPreview.Copy(bytes.data(),bytes.size());
}

void PlaylistEdit::loadTraktorCues(const ppl6::CID3Tag &Tag)
{
	ppl6::CString Tmp;
	std::list <TraktorTagCue> cuelist;
	std::list <TraktorTagCue>::const_iterator it;
	getTraktorCues(cuelist, Tag);
	if (cuelist.size()==0) return;
	ui.traktorCuesGroupBox->setVisible(true);

	for (it=cuelist.begin();it!=cuelist.end();it++) {
		QTreeWidgetItem *item=new QTreeWidgetItem;
		if (it->hotcue>=0) item->setText(0,ppl6::ToString("%d",it->hotcue+1));
		item->setText(1,it->typeName());
		if (it->type==TraktorTagCue::GRID) item->setIcon(1,QIcon(":/icons/resources/cuegrid.png"));
		else if (it->type==TraktorTagCue::IN) item->setIcon(1,QIcon(":/icons/resources/cuein.png"));
		else if (it->type==TraktorTagCue::OUT) item->setIcon(1,QIcon(":/icons/resources/cueout.png"));
		else if (it->type==TraktorTagCue::LOAD) item->setIcon(1,QIcon(":/icons/resources/cueload.png"));
		else if (it->type==TraktorTagCue::LOOP) item->setIcon(1,QIcon(":/icons/resources/cueloop.png"));
		else item->setIcon(1,QIcon(":/icons/resources/cuepoint.png"));
		item->setText(3,it->name);
		int sec=(int)(it->start/1000.0);
		item->setText(2,ppl6::ToString("%0d:%02d",(int)(sec/60),sec%60));
		ui.traktorCues->addTopLevelItem(item);

		if (it->type==TraktorTagCue::IN) traktorIn=sec;
		if (it->type==TraktorTagCue::OUT) traktorOut=sec;

	}
	ui.traktorCues->resizeColumnToContents(0);
	ui.traktorCues->resizeColumnToContents(1);
	ui.traktorCues->resizeColumnToContents(2);
	ui.traktorCues->resizeColumnToContents(3);

	if(traktorIn>=0 || traktorOut>=0) ui.traktorUseInOutButton->setEnabled(true);
	else ui.traktorUseInOutButton->setEnabled(false);

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
	item->musicKey=DataTitle::keyId(ui.musicKey->text().trimmed());
	item->keyVerified=ui.keyVerified->isChecked();
	item->bpm=ui.bpm->text().trimmed().toInt();
	item->bpmPlayed=ui.bpmPlayed->text().trimmed().toInt();
	item->rating=ui.rating->currentIndex();
	item->energyLevel=ui.energyLevel->value();
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
	item->mixLength=item->endPositionSec-item->startPositionSec;
	item->CoverPreview=CoverPreview;
	for (int i=0;i<5;i++) item->mixLength-=(item->cutEndPosition[i]-item->cutStartPosition[i]);
	if (item->bpm>0 && item->bpmPlayed>0 && item->bpmPlayed!=item->bpm) item->mixLength=item->mixLength*item->bpm/item->bpmPlayed;
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
	int bpm=ui.bpm->text().trimmed().toInt();
	int bpmPlayed=ui.bpmPlayed->text().trimmed().toInt();
	if (bpm>0 && bpmPlayed>0 && bpmPlayed!=bpm) length=length*bpm/bpmPlayed;
	ui.mixLength->setText(ppl6::ToString("%0d:%02d",(int)(length/60),length%60));
}

void PlaylistEdit::on_coverCopyButton_clicked()
{
	QClipboard *clipboard = QApplication::clipboard();
	if (!clipboard) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	clipboard->setPixmap(Cover);
	QApplication::restoreOverrideCursor();
}

void PlaylistEdit::on_coverInsertButton_clicked()
{
	QClipboard *clipboard = QApplication::clipboard();
	if (!clipboard) return;
	if (clipboard->pixmap().isNull()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	Cover=clipboard->pixmap();
	saveCover(Filename,Cover);
	updateCover();
	QApplication::restoreOverrideCursor();
}

bool PlaylistEdit::eventFilter(QObject *target, QEvent *event)
{
	if (consumeEvent(target,event)) return true;
	return QDialog::eventFilter(target,event);
}

bool PlaylistEdit::consumeEvent(QObject *target, QEvent *event)
{
	int type=event->type();
	if (type==QEvent::KeyPress) {
		QKeyEvent *e=static_cast<QKeyEvent *>(event);
		if (e->key()==Qt::Key_F12) {
			done(1);
			return true;
		} else if (e->key()==Qt::Key_Escape) {
			done(0);
			return true;
		}
	}
	return false;
}


void PlaylistEdit::on_coverLoadButton_clicked()
{
	ppl6::CString Dir=wm->conf.LastCoverPath+"/";
	if (Dir.IsEmpty()) {
		Dir=QDir::homePath();
	}
	QString newfile = QFileDialog::getOpenFileName(this, tr("Select cover image"),
			Dir,
			tr("Images (*.png *.bmp *.jpg)"));
	if (newfile.isNull()) return;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	wm->conf.LastCoverPath=ppl6::GetPath(newfile);
	wm->conf.Save();
	if (!Cover.load(newfile)) {
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this,tr("Error: could not load Cover"),
				tr("The specified file could not be loaded.\nPlease check if the file exists, is readable and contains an image format, which is supported by WinMusik (.png, .jpg or .bmp)")
				);
		return;
	} else {
		saveCover(Filename,Cover);
		QApplication::restoreOverrideCursor();
		updateCover();
	}
}

void PlaylistEdit::on_coverSaveButton_clicked()
{
	if (Cover.isNull()) return;
	ppl6::CString Dir=wm->conf.LastCoverPath+"/";

	QString newfile = QFileDialog::getSaveFileName(this, tr("Save cover to file"),
				Dir,
				tr("Images (*.png *.bmp *.jpg)"));
	if (newfile.isNull()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	wm->conf.LastCoverPath=ppl6::GetPath(newfile);
	wm->conf.Save();
	if (!Cover.save (newfile)) {
		/*
		 * StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton ) [static]
		 *
		 */
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this,tr("Error: could not save Cover"),
				tr("The cover of this track could not be saved.\nPlease check if the target directory exists and is writable.\nPlease also check the file extension. WinMusik only supports .png, .jpg and .bmp")
				);
		return;
	}
	QApplication::restoreOverrideCursor();
}

void PlaylistEdit::cue2CutStart(int cut)
{
	QTreeWidgetItem *item=ui.traktorCues->currentItem();
	if (!item) return;
	QString Tmp=item->text(2);
	switch (cut) {
		case 0: ui.cutStart0->setText(Tmp); break;
		case 1: ui.cutStart1->setText(Tmp); break;
		case 2: ui.cutStart2->setText(Tmp); break;
		case 3: ui.cutStart3->setText(Tmp); break;
		case 4: ui.cutStart4->setText(Tmp); break;
	}
	updateTotalTime();
}

void PlaylistEdit::cue2CutEnd(int cut)
{
	QTreeWidgetItem *item=ui.traktorCues->currentItem();
	if (!item) return;
	QString Tmp=item->text(2);
	switch (cut) {
		case 0: ui.cutEnd0->setText(Tmp); break;
		case 1: ui.cutEnd1->setText(Tmp); break;
		case 2: ui.cutEnd2->setText(Tmp); break;
		case 3: ui.cutEnd3->setText(Tmp); break;
		case 4: ui.cutEnd4->setText(Tmp); break;
	}
	updateTotalTime();
}

void PlaylistEdit::cutDelete(int cut)
{
	switch (cut) {
		case 0: ui.cutStart0->setText("0:00");
				ui.cutEnd0->setText("0:00");
				break;
		case 1: ui.cutStart1->setText("0:00");
				ui.cutEnd1->setText("0:00");
				break;
		case 2: ui.cutStart2->setText("0:00");
				ui.cutEnd2->setText("0:00");
				break;
		case 3: ui.cutStart3->setText("0:00");
				ui.cutEnd3->setText("0:00");
				break;
		case 4: ui.cutStart4->setText("0:00");
				ui.cutEnd4->setText("0:00");
				break;

	}
	updateTotalTime();
}


