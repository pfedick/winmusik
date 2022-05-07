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
#include "wm_traktor.h"
#include "winmusik3.h"
#include "musickey.h"

using namespace de::pfp::winmusik;

PlaylistEdit::PlaylistEdit(QWidget* parent, CWmClient* wm)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	ui.traktorCuesGroupBox->setVisible(false);
	restoreGeometry(wm->GetGeometry("playlistedit"));
	traktorIn=-1;
	traktorOut=-1;

	installFilter(ui.artist, 0);
	installFilter(ui.title, 0);
	installFilter(ui.version, 0);
	installFilter(ui.genre, 0);
	installFilter(ui.album, 0);
	installFilter(ui.label, 0);
	installFilter(ui.bpm, 0);
	installFilter(ui.bpmPlayed, 0);
	installFilter(ui.musicKey, 0);
	installFilter(ui.musicKeyModificationSpinBox, 0);
	installFilter(ui.keyVerified, 0);
	installFilter(ui.rating, 0);
	installFilter(ui.energyLevel, 0);
	installFilter(ui.trackStart, 0);
	installFilter(ui.trackEnd, 0);
	installFilter(ui.cutStart0, 0);
	installFilter(ui.cutStart1, 0);
	installFilter(ui.cutStart2, 0);
	installFilter(ui.cutStart3, 0);
	installFilter(ui.cutStart4, 0);
	installFilter(ui.cutEnd0, 0);
	installFilter(ui.cutEnd1, 0);
	installFilter(ui.cutEnd2, 0);
	installFilter(ui.cutEnd3, 0);
	installFilter(ui.cutEnd4, 0);
	installFilter(ui.traktorUseInOutButton, 0);

}


PlaylistEdit::~PlaylistEdit()
{

}


void PlaylistEdit::installFilter(QObject* object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id", id);
}


void PlaylistEdit::closeEvent(QCloseEvent* event)
{
	if (wm) {
		wm->SaveGeometry("playlistedit", saveGeometry());
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

static ppl7::String floatTimeToString(float sec)
{
	ppl7::String Tmp;
	int msec=(int)(sec * 1000.0f) - (int)sec * 1000;
	Tmp.setf("%0d:%02d.%03d", (int)(sec / 60.0f), (int)sec % 60, msec);
	return Tmp;
}

void PlaylistEdit::on_traktorUseInOutButton_clicked()
{
	if (traktorIn >= 0) ui.trackStart->setText(floatTimeToString(traktorIn));
	if (traktorOut >= 0) ui.trackEnd->setText(floatTimeToString(traktorOut));
	updateTotalTime();
}


void PlaylistEdit::filloutFields(PlaylistItem* item)
{
	ui.artist->setText(item->Artist);
	ui.title->setText(item->Title);
	ui.version->setText(item->Version);
	ui.genre->setText(item->Genre);
	ui.labelName->setText(item->Label);
	ui.album->setText(item->Album);
	ui.remarks->setText(item->Remarks);
	ui.bpm->setText(ppl7::ToString("%i", item->bpm));
	ui.bpmPlayed->setText(ppl7::ToString("%i", item->bpmPlayed));
	ui.musicKey->setText(DataTitle::keyName(item->musicKey, wm->conf.musicKeyDisplay));
	ui.keyVerified->setChecked(item->keyVerified);
	ui.energyLevel->setValue(item->energyLevel);
	ui.musicKeyModificationSpinBox->setValue(item->keyModification);
	on_musicKeyModificationSpinBox_valueChanged(item->keyModification);


	ui.rating->setCurrentIndex(item->rating);

	ui.trackStart->setText(floatTimeToString(item->startPositionSec));
	ui.trackEnd->setText(floatTimeToString(item->endPositionSec));

	ui.cutStart0->setText(floatTimeToString(item->cutStartPosition[0]));
	ui.cutStart1->setText(floatTimeToString(item->cutStartPosition[1]));
	ui.cutStart2->setText(floatTimeToString(item->cutStartPosition[2]));
	ui.cutStart3->setText(floatTimeToString(item->cutStartPosition[3]));
	ui.cutStart4->setText(floatTimeToString(item->cutStartPosition[4]));

	ui.cutEnd0->setText(floatTimeToString(item->cutEndPosition[0]));
	ui.cutEnd1->setText(floatTimeToString(item->cutEndPosition[1]));
	ui.cutEnd2->setText(floatTimeToString(item->cutEndPosition[2]));
	ui.cutEnd3->setText(floatTimeToString(item->cutEndPosition[3]));
	ui.cutEnd4->setText(floatTimeToString(item->cutEndPosition[4]));

	ui.trackLength->setText(ppl7::ToString("%0d:%02d.000", (int)(item->trackLength / 60), item->trackLength % 60));

	CoverPreview=item->CoverPreview;
	Filename=item->File;

	ppl7::ID3Tag Tag;
	if (Tag.loaded(item->File)) {
		ppl7::String Tmp=wm->GetAudioFilename(item->DeviceType, item->DeviceId, item->DevicePage, item->DeviceTrack);
		if (Tmp.notEmpty() == true && Tmp != item->File) {
			item->File=Tmp;
			Tag.loaded(item->File);
		}
	}
	if (Tag.frameCount() > 0) {
		loadCover(Tag);
		loadTraktorCues(Tag);
		ppl7::String Tmp;
		if (item->bpm == 0) {
			// BPM
			Tmp=Tag.getBPM();
			NormalizeImportString(Tmp);
			ui.bpm->setText(Tmp);
		}
		if (item->musicKey == 0) {
			// Music Key
			ui.musicKey->setText(DataTitle::keyName(DataTitle::keyId(Tag.getKey()), wm->conf.musicKeyDisplay));
		}
	} else {
		//if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 9, "WinMusik", "PlaylistEdit::filloutFields", __FILE__, __LINE__, "ID3-Tags not loaded");
	}
	updateTotalTime();
}

void PlaylistEdit::loadCover(const ppl7::ID3Tag& Tag)
{
	ppl7::ByteArray cover;
	Tag.getPicture(3, cover);
	if (cover.size() > 0) {
		Cover.loadFromData((const uchar*)cover.ptr(), cover.size());
		updateCover();
	}
}

void PlaylistEdit::updateCover()
{
	ui.cover->setPixmap(Cover.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	if (CoverPreview.size() == 0) updateCoverPreview();
}

void PlaylistEdit::updateCoverPreview()
{
	QPixmap icon;
	icon=Cover.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	icon.save(&buffer, "JPEG", 70);
	CoverPreview.copy(bytes.data(), bytes.size());
}

void PlaylistEdit::loadTraktorCues(const ppl7::ID3Tag& Tag)
{
	std::list <TraktorTagCue> cuelist;
	std::list <TraktorTagCue>::const_iterator it;
	getTraktorCues(cuelist, Tag);
	if (cuelist.size() == 0) return;
	ui.traktorCuesGroupBox->setVisible(true);
	//ui.traktorCues->setIconSize(QSize(16,16));

	for (it=cuelist.begin();it != cuelist.end();it++) {
		QTreeWidgetItem* item=new QTreeWidgetItem;
		if (it->hotcue >= 0) item->setText(0, ppl7::ToString("%d", it->hotcue + 1));
		item->setText(1, it->typeName());
		if (it->type == TraktorTagCue::GRID) item->setIcon(1, QIcon(":/icons/resources/cuegrid.png"));
		else if (it->type == TraktorTagCue::IN) item->setIcon(1, QIcon(":/icons/resources/cuein.png"));
		else if (it->type == TraktorTagCue::OUT) item->setIcon(1, QIcon(":/icons/resources/cueout.png"));
		else if (it->type == TraktorTagCue::LOAD) item->setIcon(1, QIcon(":/icons/resources/cueload.png"));
		else if (it->type == TraktorTagCue::LOOP) item->setIcon(1, QIcon(":/icons/resources/cueloop.png"));
		else item->setIcon(1, QIcon(":/icons/resources/cuepoint.png"));
		item->setText(3, it->name);
		float sec=(float)(it->start / 1000.0f);
		item->setText(2, floatTimeToString(sec));
		ui.traktorCues->addTopLevelItem(item);

		if (it->type == TraktorTagCue::IN) traktorIn=sec;
		if (it->type == TraktorTagCue::OUT) traktorOut=sec;

	}
	ui.traktorCues->resizeColumnToContents(0);
	ui.traktorCues->resizeColumnToContents(1);
	ui.traktorCues->resizeColumnToContents(2);
	ui.traktorCues->resizeColumnToContents(3);

	if (traktorIn >= 0 || traktorOut >= 0) ui.traktorUseInOutButton->setEnabled(true);
	else ui.traktorUseInOutButton->setEnabled(false);

}

void PlaylistEdit::storeFileds(PlaylistItem* item)
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
	item->CoverPreview=CoverPreview;
	item->keyModification=ui.musicKeyModificationSpinBox->value();
	item->updateMixLength();
}

float PlaylistEdit::getSecondsFromLine(QLineEdit* line)
{
	ppl7::Array Token(line->text(), ":");
	if (Token.size() > 2) {
		float seconds=(float)Token.get(0).toInt() * 60.0f + (float)Token.get(1).toInt();
		ppl7::Array Token2(Token.get(1), ".");
		if (Token2.size() > 1) seconds+=(float)Token2.get(1).toInt() / 1000.0f;
		return seconds;
	}
	return 0.0f;
}

void PlaylistEdit::updateTotalTime()
{
	float length=getSecondsFromLine(ui.trackEnd) - getSecondsFromLine(ui.trackStart);
	length-=(getSecondsFromLine(ui.cutEnd0) - getSecondsFromLine(ui.cutStart0));
	length-=(getSecondsFromLine(ui.cutEnd1) - getSecondsFromLine(ui.cutStart1));
	length-=(getSecondsFromLine(ui.cutEnd2) - getSecondsFromLine(ui.cutStart2));
	length-=(getSecondsFromLine(ui.cutEnd3) - getSecondsFromLine(ui.cutStart3));
	length-=(getSecondsFromLine(ui.cutEnd4) - getSecondsFromLine(ui.cutStart4));
	int bpm=ui.bpm->text().trimmed().toInt();
	int bpmPlayed=ui.bpmPlayed->text().trimmed().toInt();
	if (bpm > 0 && bpmPlayed > 0 && bpmPlayed != bpm) {
		float perc=(float)bpmPlayed * 100.0f / (float)bpm;
		length=length * 100.0f / perc;
	}
	ui.mixLength->setText(floatTimeToString(length));
}

void PlaylistEdit::on_coverCopyButton_clicked()
{
	QClipboard* clipboard = QApplication::clipboard();
	if (!clipboard) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	clipboard->setPixmap(Cover);
	QApplication::restoreOverrideCursor();
}

void PlaylistEdit::on_coverInsertButton_clicked()
{
	QClipboard* clipboard = QApplication::clipboard();
	if (!clipboard) return;
	if (clipboard->pixmap().isNull()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	Cover=clipboard->pixmap();
	saveCover(Filename, Cover);
	updateCover();
	updateCoverPreview();
	QApplication::restoreOverrideCursor();
}

bool PlaylistEdit::eventFilter(QObject* target, QEvent* event)
{
	if (consumeEvent(target, event)) return true;
	return QDialog::eventFilter(target, event);
}

bool PlaylistEdit::consumeEvent(QObject*, QEvent* event)
{
	int type=event->type();
	if (type == QEvent::KeyPress) {
		QKeyEvent* e=static_cast<QKeyEvent*>(event);
		if (e->key() == Qt::Key_F12) {
			done(1);
			return true;
		} else if (e->key() == Qt::Key_Escape) {
			done(0);
			return true;
		}
	}
	return false;
}


void PlaylistEdit::on_coverLoadButton_clicked()
{
	ppl7::String Dir=wm->conf.LastCoverPath + "/";
	if (Dir.isEmpty()) {
		Dir=QDir::homePath();
	}
	QString newfile = QFileDialog::getOpenFileName(this, tr("Select cover image"),
		Dir,
		tr("Images (*.png *.bmp *.jpg)"));
	if (newfile.isNull()) return;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	wm->conf.LastCoverPath=ppl7::File::getPath(newfile);
	wm->conf.trySave();
	if (!Cover.load(newfile)) {
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this, tr("Error: could not load Cover"),
			tr("The specified file could not be loaded.\nPlease check if the file exists, is readable and contains an image format, which is supported by WinMusik (.png, .jpg or .bmp)")
		);
		return;
	} else {
		saveCover(Filename, Cover);
		QApplication::restoreOverrideCursor();
		updateCover();
		updateCoverPreview();
	}
}

void PlaylistEdit::on_coverSaveButton_clicked()
{
	if (Cover.isNull()) return;
	ppl7::String Dir=wm->conf.LastCoverPath + "/";

	QString newfile = QFileDialog::getSaveFileName(this, tr("Save cover to file"),
		Dir,
		tr("Images (*.png *.bmp *.jpg)"));
	if (newfile.isNull()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	wm->conf.LastCoverPath=ppl7::File::getPath(newfile);
	wm->conf.trySave();
	if (!Cover.save(newfile)) {
		/*
		 * StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton ) [static]
		 *
		 */
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this, tr("Error: could not save Cover"),
			tr("The cover of this track could not be saved.\nPlease check if the target directory exists and is writable.\nPlease also check the file extension. WinMusik only supports .png, .jpg and .bmp")
		);
		return;
	}
	QApplication::restoreOverrideCursor();
}

void PlaylistEdit::cue2CutStart(int cut)
{
	QTreeWidgetItem* item=ui.traktorCues->currentItem();
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
	QTreeWidgetItem* item=ui.traktorCues->currentItem();
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
	case 0: ui.cutStart0->setText("0:00.000");
		ui.cutEnd0->setText("0:00.000");
		break;
	case 1: ui.cutStart1->setText("0:00.000");
		ui.cutEnd1->setText("0:00.000");
		break;
	case 2: ui.cutStart2->setText("0:00.000");
		ui.cutEnd2->setText("0:00.000");
		break;
	case 3: ui.cutStart3->setText("0:00.000");
		ui.cutEnd3->setText("0:00.000");
		break;
	case 4: ui.cutStart4->setText("0:00.000");
		ui.cutEnd4->setText("0:00.000");
		break;

	}
	updateTotalTime();
}

void PlaylistEdit::on_cueTrackStart_clicked()
{
	QTreeWidgetItem* item=ui.traktorCues->currentItem();
	if (!item) return;
	QString Tmp=item->text(2);
	ui.trackStart->setText(Tmp);
	updateTotalTime();
}

void PlaylistEdit::on_cueTrackEnd_clicked()
{
	QTreeWidgetItem* item=ui.traktorCues->currentItem();
	if (!item) return;
	QString Tmp=item->text(2);
	ui.trackEnd->setText(Tmp);
	updateTotalTime();
}

void PlaylistEdit::on_trackStartFromBeginning_clicked()
{
	ui.trackStart->setText("0:00.000");
}

void PlaylistEdit::on_trackEndUntilEnd_clicked()
{
	ui.trackEnd->setText(ui.trackLength->text());
}

void PlaylistEdit::on_musicKeyModificationSpinBox_valueChanged(int value)
{
	MusicKey key(ui.musicKey->text().trimmed());
	ui.musicKeyPlayed->setText(key.addSemitone(value).name(wm->conf.musicKeyDisplay));
}
