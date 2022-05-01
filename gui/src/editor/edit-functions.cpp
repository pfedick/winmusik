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


#include "winmusik3.h"
#include "edit.h"
#include "editdevice.h"
#include "tablesearch.h"
#include "shortcutdialog.h"
#include "massimport.h"
#include "renumberdialog.h"
#include "cddbimport.h"
#include "cddbselect.h"
#include <QString>
#include <QScrollBar>
#include <QClipboard>
#include <QCompleter>
#include <QMenu>
#include <QUrl>
#include <QPoint>
#include <QTimer>
#include <QPixmap>
#include <QBuffer>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressDialog>
#include <QMimeData>
#include <QStringList>
#include "../include/asynchronousMessage.h"
#include "version.h"

#include "ppl7.h"
#include "ppl7-inet.h"


void Edit::UpdateDevice()
{
	ppl6::CString Tmp;
	wm->LoadDevice(DeviceType, DeviceId, &datadevice);
	Tmp.Setf("%u", datadevice.Pages);
	ui.devicePages->setText(Tmp);
	Tmp.Setf("%u", datadevice.NumTracks);
	ui.deviceTracks->setText(Tmp);
	if (datadevice.Length > 0) {
		Tmp.Setf("%0i:%02i", (int)(datadevice.Length / 60), datadevice.Length % 60);
		ui.deviceLength->setText(Tmp);
	} else {
		ui.deviceLength->setText("?");
	}
	Tmp.Setf("%0i:%02i", (int)(datadevice.Recorded / 60), datadevice.Recorded % 60);
	ui.deviceRecorded->setText(Tmp);

	Tmp.Setf("%s\n%s", (datadevice.Title ? datadevice.Title : ""), (datadevice.SubTitle ? datadevice.SubTitle : ""));
	ui.deviceTitle->setText(Tmp);
	if (ui.page->text().toInt() > datadevice.Pages) {
		Tmp="1";
		ui.page->setText(Tmp);
		OpenTrack(DeviceId, 1);
	}
	if (ui.page->text().toInt() == 0) {
		Tmp="1";
		ui.page->setText(Tmp);
	}
}

ppluint32 Edit::EditDeviceDialog(ppluint32 id)
{
	EditDevice* w=new EditDevice(this, wm, DeviceType, id);
	//w->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowCloseButtonHint);
	w->setWindowFlags(Qt::Dialog);
	ppluint32 ret=w->exec();
	delete w;
	return ret;
}



bool Edit::EditTrack()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	Cover=QPixmap();
	ppl6::CString Tmp;
	DupeCheck=false;
	DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";
	ClearEditFields();
	Oimp.Clear();
	Track.Clear();
	Track.SetValue(DeviceType, DeviceId, Page, TrackNum, 0);
	Ti.Clear();
	int t=(ppluint32)ui.track->text().toInt();
	if (t < 1 || t> 65535) {
		QApplication::restoreOverrideCursor();
		return false;
	}
	TrackNum=(ppluint16)t;
	// Track laden, falls es ihn schon gibt
	if (TrackList->GetCopy(t, &Track)) {
		DataTitle* ti=wm->GetTitle(Track.TitleId);
		if (ti) {
			Ti.CopyFrom(ti);
		}
	} else {
		Track.SetValue(DeviceType, DeviceId, Page, TrackNum, 0);
	}
	if (Ti.TitleId) FillEditFields();
	// Dateiname
	ppl6::CString Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, TrackNum);
	if (Path.IsEmpty()) {
		ui.filename->setText(tr("file not found"));
		ui.filename->setStyleSheet("color: red");
		ui.filesize->setText("");
	} else {
		ui.filename->setText(Path);
		ui.filename->setStyleSheet("");
		ppl6::CDirEntry de;
		if (ppl6::CFile::Stat(Path, de)) {
			Tmp.Setf("%0.1f", (double)de.Size / 1048576.0);
			ui.filesize->setText(Tmp);
			ppl6::CID3Tag Tag;
			if (Tag.Load(Path)) {
				// Cover?
				ppl6::CBinary cover;
				if (Tag.GetPicture(3, cover)) {
					Cover.loadFromData((const uchar*)cover.GetPtr(), cover.GetSize());
					ui.coverwidget->setPixmap(Cover);
					wm->UpdateCoverViewer(Cover);
				}
			}
		}
	}

	QApplication::restoreOverrideCursor();
	return true;
}

void Edit::ShowOimpInfo()
{
	if (oimpInfo) {
		delete oimpInfo;
		oimpInfo=NULL;
	}
	oimpInfo=new OimpInfo(this, &Oimp);
	oimpInfo->setWindowFlags(Qt::SubWindow);
	oimpInfo->setBackgroundRole(QPalette::ToolTipBase);
	oimpInfo->move((geometry().width() - oimpInfo->width()) / 2, 50 + ui.titleEdit->y() - oimpInfo->height());
	//oimpInfo->move(10,10);
	oimpInfo->show();
}

void Edit::ClearEditFields()
{
	ui.artist->clear();
	ui.title->clear();
	ui.length->clear();
	ui.bpm->clear();
	ui.musickey->clear();
	ui.energyLevel->setValue(0);
	ui.remarks->clear();
	//ui.tags->clear();
	ui.album->clear();
	ui.filename->clear();
	ui.filesize->clear();
	ui.titleId->clear();
	ui.recordVersion->clear();
	ui.bitrate->clear();
	ui.musickeyVerified->setChecked(false);

	// Flags
	ui.complete->setChecked(true);
	ui.realTitle->setChecked(true);
	ui.interrupted->setChecked(false);

	// Ton
	ui.channels->setCurrentIndex(2);

	// Qualität
	ui.quality->setCurrentIndex(0);

	// Bewertung
	ui.rating->setCurrentIndex(0);

	Cover=QPixmap();
	ui.coverwidget->clear();


	if (oimpInfo) {
		delete oimpInfo;
		oimpInfo=NULL;
	}

}

void Edit::FillEditFields()
{
	ppl6::CString Tmp;


	// TitleId und Version
	Tmp.Setf("%u", Ti.TitleId);
	ui.titleId->setText(Tmp);
	Tmp.Setf("%u", Ti.GetVersion());
	ui.recordVersion->setText(Tmp);

	// Interpret und Titel
	ui.artist->setText(Ti.Artist);
	ui.title->setText(Ti.Title);

	// Version
	if (Ti.VersionId) Tmp.Setf("%u", Ti.VersionId); else Tmp="";
	ui.versionId->setText(Tmp);
	ui.version->setText(wm->GetVersionText(Ti.VersionId));

	// Genre
	if (Ti.GenreId) Tmp.Setf("%u", Ti.GenreId); else Tmp="";
	ui.genreId->setText(Tmp);
	ui.genre->setText(wm->GetGenreText(Ti.GenreId));

	// Länge
	if (Ti.Length > 0) Tmp.Setf("%0i:%02i", (int)(Ti.Length / 60), Ti.Length % 60); else Tmp.Clear();
	ui.length->setText(Tmp);

	// BPM
	if (Ti.BPM > 0) Tmp.Setf("%i", Ti.BPM); else Tmp.Clear();
	ui.bpm->setText(Tmp);

	// Music Key
	ui.musickey->setText(ppl7::Trim(Ti.getKeyName(musicKeyDisplay)));

	// EnergyLevel
	ui.energyLevel->setValue(Ti.EnergyLevel);

	// Bitrate
	if (Ti.Bitrate > 0) Tmp.Setf("%i", Ti.Bitrate); else Tmp.Clear();
	ui.bitrate->setText(Tmp);

	// Erscheinungsjahr
	QDate Date;
	if (Ti.ReleaseDate > 0) {
		Tmp.Setf("%u", Ti.ReleaseDate);
		int year=Tmp.Mid(0, 4).ToInt();
		int month=Tmp.Mid(4, 2).ToInt();
		int day=Tmp.Mid(6, 2).ToInt();
		if (!month) month=1;
		if (!day) day=1;
		Date.setDate(year, month, day);
	} else {
		Date=QDate::currentDate();
	}
	ui.releaseDate->setDate(Date);

	// Aufnahmedatum
	Tmp.Setf("%u", Ti.RecordDate);
	Date=QDate::fromString(Tmp, "yyyyMMdd");
	ui.recordDate->setDate(Date);


	// Album
	ui.album->setText(Ti.Album);

	// Label
	if (Ti.LabelId) Tmp.Setf("%u", Ti.LabelId); else Tmp="";
	ui.labelId->setText(Tmp);
	ui.labelName->setText(wm->GetLabelText(Ti.LabelId));

	// Aufnahmequelle
	if (Ti.RecordSourceId) Tmp.Setf("%u", Ti.RecordSourceId); else Tmp="";
	ui.recordSourceId->setText(Tmp);
	ui.recordSource->setText(wm->GetRecordSourceText(Ti.RecordSourceId));

	// Aufnahmegerät
	if (Ti.RecordDeviceId) Tmp.Setf("%u", Ti.RecordDeviceId); else Tmp="";
	ui.recordDeviceId->setText(Tmp);
	ui.recordDevice->setText(wm->GetRecordDeviceText(Ti.RecordDeviceId));

	// Bemerkung
	ui.remarks->setText(Ti.Remarks);

	// Tags
	ui.tags->setText(Ti.Tags);


	// Flags
	if (Ti.Flags & 1) ui.complete->setChecked(true);
	else ui.complete->setChecked(false);

	if (Ti.Flags & 2) ui.realTitle->setChecked(true);
	else ui.realTitle->setChecked(false);

	if (Ti.Flags & 8) ui.interrupted->setChecked(true);
	else ui.interrupted->setChecked(false);


	if (Ti.Flags & 16) ui.musickeyVerified->setChecked(true);
	else ui.musickeyVerified->setChecked(false);

	// Ton
	ui.channels->setCurrentIndex(Ti.Channels);

	// Qualität
	ui.quality->setCurrentIndex(Ti.Quality);

	// Bewertung
	ui.rating->setCurrentIndex(Ti.Rating);

}

void Edit::UpdateTrackListing()
/*!\brief Anzeige der Titel aktualisieren
 *
 */
{
	QString QTmp;
	ppl6::CString Text, Tmp;
	WMTreeItem* item;
	DataTrack* track;
	DataTitle* title;

	//	Artists.clear();
	//	Titles.clear();


		// Tabelle leeren
	trackList->clear();
	trackList->setWordWrap(false);
	trackList->setSortingEnabled(false);
	trackList->setIconSize(QSize(64, 16));
	int count=0;
	ppluint32 length=0;
	ppluint32 size=0;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	if (TrackList) {
		// Höchste Tracknummer
		int max=TrackList->GetMax();

		for (int i=1;i <= max;i++) {
			item=new WMTreeItem;

			item->Track=i;
			item->Id=0;
			Text.Setf("%5i", i);
			item->setText(TRACKLIST_TRACK_ROW, Text);
			// Track holen
			track=TrackList->Get(i);
			if (track) {	// Kann NULL sein, wenn Tracks bei der Eingabe übersprungen wurden
				item->Id=track->TitleId;
				// Titel holen
				title=wm->GetTitle(track->TitleId);
				if (title) {
					count++;
					length+=title->Length;
					RenderTrack(item, title);
					size+=title->Size;
				}
			}
			trackList->addTopLevelItem(item);
		}
	}
	QApplication::restoreOverrideCursor();

	Tmp.Setf("%i", count);
	ui.statsTracks->setText(Tmp);
	Tmp.Setf("%4i:%02i", (int)(length / 60), length % 60);
	ui.statsLength->setText(Tmp);
	Tmp.Setf("%0.1f", (double)size / 1048576.0);
	ui.statsSize->setText(Tmp);
	resizeEvent(NULL);
	trackList->sortByColumn(0, Qt::AscendingOrder);
	trackList->setSortingEnabled(true);
}

void Edit::RenderTrack(WMTreeItem* item, DataTitle* title)
{
	//QString QTmp;
	ppl6::CString Text, Tmp;
	ppl6::CString Unknown=tr("unknown");
	const char* unknown=(const char*)Unknown;
	DataVersion* version;
	DataGenre* genre;
	QBrush Brush(Qt::SolidPattern);
	Brush.setColor("red");

	Text.Setf("%s - %s",
		(title->Artist.NotEmpty() ? (const char*)title->Artist : unknown),
		(title->Title.NotEmpty() ? (const char*)title->Title : unknown));

	item->setText(TRACKLIST_NAME_ROW, Text);
	// Version holen
	version=wm->GetVersion(title->VersionId);
	if (version) Text.Set(version->Value);
	else Text.Set(unknown);
	item->setText(TRACKLIST_VERSION_ROW, Text);
	item->setForeground(TRACKLIST_VERSION_ROW, Brush);
	// Genre holen
	genre=wm->GetGenre(title->GenreId);
	if (genre) Text.Set(genre->Value);
	else Text.Set(unknown);
	item->setText(TRACKLIST_GENRE_ROW, Text);
	Text.Setf("%4i:%02i", (int)(title->Length / 60), title->Length % 60);
	item->setText(TRACKLIST_LENGTH_ROW, Text);

	// BPM, Key und EnergyLevel
	Text.Setf("%d", (int)title->BPM);
	item->setText(TRACKLIST_BPM_ROW, Text);
	item->setText(TRACKLIST_KEY_ROW, title->getKeyName(musicKeyDisplay));
	QFont f=item->font(TRACKLIST_KEY_ROW);
	if ((title->Flags & 16)) {
		f.setBold(true);
		f.setWeight(QFont::Black);
	} else {
		f.setBold(false);
	}
	item->setFont(TRACKLIST_KEY_ROW, f);
	Text.Setf("%d", (int)title->EnergyLevel);
	item->setText(TRACKLIST_ENERGYLEVEL_ROW, Text);

	Text.Setf("%d", (int)title->ReleaseDate / 10000);
	item->setText(TRACKLIST_YEAR, Text);

	Text.Setf("%d", (int)title->Bitrate);
	item->setText(TRACKLIST_BITRATE_ROW, Text);
	item->setTextAlignment(TRACKLIST_BITRATE_ROW, Qt::AlignRight);


	// Rating
	switch (title->Rating) {
	case 0: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-0.png"));
		item->setText(TRACKLIST_RATING_ROW, "0");
		break;
	case 1: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-1.png"));
		item->setText(TRACKLIST_RATING_ROW, "1");
		break;
	case 2: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-2.png"));
		item->setText(TRACKLIST_RATING_ROW, "2");
		break;
	case 3: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-3.png"));
		item->setText(TRACKLIST_RATING_ROW, "3");
		break;
	case 4: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-4.png"));
		item->setText(TRACKLIST_RATING_ROW, "4");
		break;
	case 5: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-5.png"));
		item->setText(TRACKLIST_RATING_ROW, "5");
		break;
	case 6: item->setIcon(TRACKLIST_RATING_ROW, QIcon(":/bewertung/resources/sterne64x16-6.png"));
		item->setText(TRACKLIST_RATING_ROW, "6");
		break;
	}

	if (title->Size == 0) {
		//printf ("title->Size ist Null, führe Stat durch\n");
		ppl6::CString Path=wm->GetAudioFilename(DeviceType, title->DeviceId, title->Page, title->Track);
		if (Path.NotEmpty()) {
			ppl6::CDirEntry de;
			if (ppl6::CFile::Stat(Path, de)) {
				//printf ("Stat erfolgreich\n");
				DataTitle ti;
				ti.CopyFrom(title);
				ti.Size=de.Size;
				title->Size=ti.Size;

				ppl6::CID3Tag Tag;
				if (Tag.Load(&Path)) {
					// Cover?
					ppl6::CBinary cover;
					QPixmap pix, icon;
					if (Tag.GetPicture(3, cover)) {
						pix.loadFromData((const uchar*)cover.GetPtr(), cover.GetSize());
						icon=pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);

						item->setIcon(TRACKLIST_COVER_ROW, icon.copy(0, 0, 64, 16));

						QByteArray bytes;
						QBuffer buffer(&bytes);
						buffer.open(QIODevice::WriteOnly);
						icon.save(&buffer, "JPEG", 70);
						ti.CoverPreview.Copy(bytes.data(), bytes.size());
					}
				}

				if (!wm->TitleStore.Put(&ti)) {
					printf("Speichern fehlgeschlagen!\n");
					ppl6::PrintError();

				}

			}
		}
	} else {
		//printf("Wir haben die Größe schon\n");
		if (title->CoverPreview.Size() > 0) {
			QPixmap pix, icon;
			pix.loadFromData((const uchar*)title->CoverPreview.GetPtr(), title->CoverPreview.Size());
			icon=pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			item->setIcon(TRACKLIST_COVER_ROW, icon.copy(0, 0, 64, 16));
		}
	}
}


void Edit::SaveEditorTrack()
{
	ppl7::String Tmp, Artist, Title;
	// Wir müssen sicherstellen, dass die Tabellen-Index-Felder gültige Werte enthalten
	if (!TCVersion.Finish()) {
		ui.versionId->setFocus();
		return;
	}
	if (!TCGenre.Finish()) {
		ui.genreId->setFocus();
		return;
	}
	if (!TCLabel.Finish()) {
		ui.labelId->setFocus();
		return;
	}
	if (!TCRecordSource.Finish()) {
		ui.recordSourceId->setFocus();
		return;
	}
	if (!TCRecordDevice.Finish()) {
		ui.recordDeviceId->setFocus();
		return;
	}


	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.trim();
	Ti.SetArtist(Artist);
	Title=ui.title->text();
	Title.trim();
	Ti.SetTitle(Title);

	if ((Artist.isEmpty()) && (Title.isEmpty())) {
		// Warning
		int ret=QMessageBox::warning(this, tr("WinMusik: Attention"),
			tr("Artist and title are empty!\nDo you realy want to save this track?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (ret == QMessageBox::No) {
			ui.artist->setFocus();
			return;
		}
	}

	// Version
	Ti.VersionId=ui.versionId->text().toInt();
	// Genre
	Ti.GenreId=ui.genreId->text().toInt();

	// Länge
	Ti.Length=Time2Int(ui.length->text());

	// BPM
	Ti.BPM=ui.bpm->text().toInt();

	// Music Key
	Ti.SetKey(ui.musickey->text());

	// EnergyLevel
	Ti.EnergyLevel=ui.energyLevel->value();

	// Bitrate
	Ti.Bitrate=ui.bitrate->text().toInt();

	// Erscheinungsjahr
	QDate Date=ui.releaseDate->date();
	Tmp=Date.toString("yyyyMMdd");
	Ti.ReleaseDate=Tmp.toInt();

	// Aufnahmedatum
	Date=ui.recordDate->date();
	Tmp=Date.toString("yyyyMMdd");
	//printf ("Date: %s\n",(const char*)Tmp);
	Ti.RecordDate=Tmp.toInt();

	// Album
	Tmp=ui.album->text();
	Ti.SetAlbum(Tmp);

	// Label
	Ti.LabelId=ui.labelId->text().toInt();

	// Aufnahmequelle
	Ti.RecordSourceId=ui.recordSourceId->text().toInt();

	// Aufnahmegerät
	Ti.RecordDeviceId=ui.recordDeviceId->text().toInt();

	// Bemerkung
	Tmp=ui.remarks->text();
	Ti.SetRemarks(Tmp);

	// Tags
	Tmp=ui.tags->text();
	Tmp=ppl7::Trim(ppl7::LowerCase(Tmp));
	Ti.SetTags(Tmp);

	// Flags
	if (ui.complete->isChecked()) Ti.Flags|=1;
	else Ti.Flags&=(0xff - 1);
	if (ui.realTitle->isChecked()) Ti.Flags|=2;
	else Ti.Flags&=(0xff - 2);
	if (ui.interrupted->isChecked()) Ti.Flags|=8;
	else Ti.Flags&=(0xff - 8);
	if (ui.musickeyVerified->isChecked()) Ti.Flags|=16;
	else Ti.Flags&=(0xff - 16);


	// Ton
	Ti.Channels=ui.channels->currentIndex();

	// Qualität
	Ti.Quality=ui.quality->currentIndex();

	// Bewertung
	Ti.Rating=ui.rating->currentIndex();

	Ti.DeviceType=DeviceType;
	Ti.DeviceId=DeviceId;
	Ti.Page=Page;
	Ti.Track=TrackNum;

	// Cover
	ppl6::CString Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, TrackNum);
	if (Path.NotEmpty()) {
		ppl6::CDirEntry de;
		if (ppl6::CFile::Stat(Path, de)) {
			Ti.Size=de.Size;
			if (Cover.isNull()) {
				Ti.CoverPreview.Clear();
			} else {
				QPixmap icon=Cover.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				QByteArray bytes;
				QBuffer buffer(&bytes);
				buffer.open(QIODevice::WriteOnly);
				icon.save(&buffer, "JPEG", wm->conf.JpegQualityPreview);
				Ti.CoverPreview.Copy(bytes.data(), bytes.size());
			}
		}
	}

	if (!SaveTrack(Ti)) return;

	FillEditFields();
	UpdateTrackListing();
	UpdateCompleters();
	Tmp.setf("%u", Track.Track + 1);
	ui.track->setText(Tmp);
	ui.track->setFocus();

}

bool Edit::SaveTrack(DataTitle& Ti)
{
	bool weHaveChanges=true;
	if (Ti.TitleId > 0) {
		DataTitle* OldTi=wm->TitleStore.Get(Ti.TitleId);
		if (OldTi != NULL && *OldTi == Ti) weHaveChanges=false;
	}
	if (weHaveChanges) {
		//printf ("changes detected\n");
		// Titel speichern
		if (Ti.TitleId > 0) wm->Hashes.RemoveTitle(Ti.TitleId);
		if (!wm->TitleStore.Put(&Ti)) {
			wm->RaiseError(this, tr("Could not save Title in TitleStore"));
			ui.artist->setFocus();
			if (Ti.TitleId > 0) wm->Hashes.AddTitle(Ti.TitleId);
			return false;
		}
		// An die Hashes dürfen wir natürlich nicht den Pointer auf den lokalen Titel "Ti" übergeben,
		// sondern den Pointer innerhalb der Datenbank
		DataTitle* dt=wm->TitleStore.Get(Ti.TitleId);
		if (dt) wm->Hashes.AddTitle(Ti.TitleId, dt);
		// Track speichern
		Track.TitleId=Ti.TitleId;
		if (!TrackList->Put(&Track)) {
			wm->RaiseError(this, tr("Could not save Track in TrackList"));
			ui.artist->setFocus();
			return false;
		}
		// Tonträger aktualisieren
		wm->DeviceStore.Update(DeviceType, DeviceId);
		UpdateDevice();
	}

	// ID3-Tags speichern, sofern gewünscht und eine Datei vorhanden ist
	if (wm_main->conf.bWriteID3Tags == true) {
		ppl6::CString Path=wm->GetAudioFilename(DeviceType, Track.DeviceId, Page, Track.Track);
		if (Path.NotEmpty()) {
			if (!wm->SaveID3Tags(DeviceType, Track.DeviceId, Page, Track.Track, Ti)) {
				wm->RaiseError(this, tr("Could not save ID3 Tags"));
			}
		}
	}
	return true;
}

void Edit::UpdateCompleters()
{
	DataTrack* track;
	DataTitle* title;

	QString QTmp;
	Artists.clear();
	Titles.clear();
	Albums.clear();
	ppl6::CTree* t=&wm->TitleStore.Artists;
	t->Reset();
	CStringCounterItem* item, * found;
	while ((item=(CStringCounterItem*)t->GetNext())) {
		QTmp=(const char*)item->Name;
		Artists.append(QTmp);
	}

	// Wir bauen neue Trees auf
	ppl6::CTree TmpTitles;
	ppl6::CTree TmpAlbums;

	if (TrackList) {
		// Höchste Tracknummer
		int max=TrackList->GetMax();
		for (int i=1;i <= max;i++) {
			// Track holen
			track=TrackList->Get(i);
			if (track) {	// Kann NULL sein, wenn Tracks bei der Eingabe übersprungen wurden
				// Titel holen
				title=wm->GetTitle(track->TitleId);
				if (title) {
					/*
					if (title->Artist) {
						item=new CStringCounterItem;
						item->Name=title->Artist;
						item->Count=1;
						found=(CStringCounterItem*)TmpArtists.Find(item);
						if (found) {
							found->Count++;
							delete item;
						} else TmpArtists.Add(item);
					}
					*/
					if (title->Title.NotEmpty()) {
						item=new CStringCounterItem;
						item->Name=title->Title;
						item->Count=1;
						found=(CStringCounterItem*)TmpTitles.Find(item);
						if (found) {
							found->Count++;
							delete item;
						} else TmpTitles.Add(item);
					}
					if (title->Album.NotEmpty()) {
						item=new CStringCounterItem;
						item->Name=title->Album;
						item->Count=1;
						found=(CStringCounterItem*)TmpAlbums.Find(item);
						if (found) {
							found->Count++;
							delete item;
						} else TmpAlbums.Add(item);
					}
				}
			}
		}
	}
	/*
	TmpArtists.Reset();
	while ((item=(CStringCounterItem*)TmpArtists.GetNext())) {
		QTmp=(const char*)item->Name;
		Artists.append(QTmp);
	}
	*/

	TmpTitles.Reset();
	while ((item=(CStringCounterItem*)TmpTitles.GetNext())) {
		QTmp=(const char*)item->Name;
		//printf ("Append Titles: %s\n",(const char*)item->Name);
		Titles.append(QTmp);
	}
	//TmpArtists.Clear(true);
	TmpTitles.Clear(true);

	TmpAlbums.Reset();
	while ((item=(CStringCounterItem*)TmpAlbums.GetNext())) {
		QTmp=(const char*)item->Name;
		//printf ("Append Titles: %s\n",(const char*)item->Name);
		Albums.append(QTmp);
	}
	//TmpArtists.Clear(true);
	TmpAlbums.Clear(true);

}

void Edit::CopyFromTrackInfo(TrackInfo& info)
{
	ppl6::CString Tmp;
	ui.artist->setText(info.Ti.Artist);		// Artist
	ui.title->setText(info.Ti.Title);		// Title
	ui.album->setText(info.Ti.Album);		// Album
	QDate Date;								// ReleaseDate
	Tmp.Setf("%u", info.Ti.ReleaseDate);
	int year=Tmp.Mid(0, 4).ToInt();
	int month=Tmp.Mid(4, 2).ToInt();
	int day=Tmp.Mid(6, 2).ToInt();
	if (!month) month=1;
	if (!day) day=1;
	Date.setDate(year, month, day);
	ui.releaseDate->setDate(Date);

	ui.remarks->setText(info.Ti.Remarks);	// Comment
	ui.genre->setText(info.Genre);			// Genre
	if (info.Genre.NotEmpty()) {
		DataGenre* dg=(DataGenre*)wm->GenreStore.Find(info.Genre);
		if (dg) {
			Tmp.Setf("%u", dg->Id);
			ui.genreId->setText(Tmp);
		} else {
			ui.genreId->setText("*");
		}
	}
	if (info.Ti.Length > 0) {					// Laenge
		Tmp.Setf("%0i:%02i", (int)(info.Ti.Length / 60), info.Ti.Length % 60);
		ui.length->setText(Tmp);
	}
	if (info.Ti.Bitrate > 0) {				// Bitrate
		Tmp.Setf("%i", info.Ti.Bitrate);
		ui.bitrate->setText(Tmp);
	}
	if (info.Ti.BPM > 0) {					// BPM
		Tmp.Setf("%i", info.Ti.BPM);
		ui.bpm->setText(Tmp);
	}
	if (info.Ti.EnergyLevel > 0) {			// EnergyLevel
		ui.energyLevel->setValue(info.Ti.EnergyLevel);
	}

	// Music Key
	ui.musickey->setText(ppl7::Trim(info.Ti.getKeyName(musicKeyDisplay)));

	if (info.Version.IsEmpty()) {			// Version
		if (info.Ti.Length < 5 * 60) {
			info.Version="Single";
		} else {
			info.Version="Maxi";
		}
	}
	ui.version->setText(info.Version);
	if (info.Version.NotEmpty()) {
		DataVersion* dv=(DataVersion*)wm->VersionStore.Find(info.Version);
		if (dv) {
			Tmp.Setf("%u", dv->Id);
			ui.versionId->setText(Tmp);
		} else {
			ui.versionId->setText("*");
		}
	}
	ui.labelName->setText(info.Label);		// Label
	if (info.Label.NotEmpty()) {
		DataLabel* dl=(DataLabel*)wm->LabelStore.Find(info.Label);
		if (dl) {
			Tmp.Setf("%u", dl->Id);
			ui.labelId->setText(Tmp);
		} else {
			ui.labelId->setText("*");
		}
	} else {
		ui.labelId->setText("*");
	}
	if (info.Cover.Size()) {				// Cover
		Cover.loadFromData((const uchar*)info.Cover.GetPtr(), info.Cover.GetSize());
		ui.coverwidget->setPixmap(Cover);
		wm->UpdateCoverViewer(Cover);
	}
}

void Edit::UpdateCover()
{
	wm->UpdateCoverViewer(Cover);
	if (wm_main->conf.bWriteID3Tags == true) {
		ppl6::CString Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, TrackNum);
		if (Path.NotEmpty()) {
			saveCover(Path, Cover);
		}
	}
}

void Edit::hideEditor()
{
	ui.track->setFocus();
	ui.titleEdit->setVisible(false);
	ui.titleEdit->setEnabled(false);
	ui.hideEditor->setIcon(QIcon(":/icons/resources/1uparrow.png"));
}

void Edit::showEditor()
{
	showEditorWithoutFocusChange();
	ui.track->setFocus();
}

void Edit::showEditorWithoutFocusChange()
{
	ui.titleEdit->setVisible(true);
	ui.titleEdit->setEnabled(true);
	ui.hideEditor->setIcon(QIcon(":/icons/resources/1downarrow.png"));
}

void Edit::importFromCddb()
{
	if (ppl7::AudioCD::isSupported() != true || ppl7::CDDB::isSupported() != true) return;
	ppl7::CDDB::Matches matches;
	asynchronousMessage msg;
	msg.setMessageText(tr("Reading and querying cd in internet database, please wait."));
	msg.show();
	QCoreApplication::processEvents();
	QCoreApplication::sendPostedEvents();
	QCoreApplication::processEvents();

	ppl7::CDDB cddb;
	cddb.setHttpServer(wm->conf.cddb.server, wm->conf.cddb.port);
	cddb.setUser(wm->conf.cddb.username, wm->conf.cddb.hostname);
	cddb.setClient(WM_APPNAME, WM_VERSION);
	if (wm->conf.cddb.useProxy) {
		cddb.setProxy(wm->conf.cddb.proxy_server, wm->conf.cddb.proxy_port);
	}

	try {
		ppl7::AudioCD cd;
		QCoreApplication::processEvents();
		cd.openDevice();
		QCoreApplication::processEvents();
		cddb.query(cd, matches);
		QCoreApplication::processEvents();
	} catch (const ppl6::Exception& e) {
		msg.setVisible(false);
		QString intro=tr("An error occured, when trying to access the audio cd or querying the internet database");
		QMessageBox::critical(NULL, tr("Error"),
			intro,
			QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	msg.setVisible(false);
	// If we don't have any results, we show a message and exit
	if (matches.size() < 1) {
		QMessageBox::information(NULL, tr("audio cd not found"),
			tr("The audio cd was not found in the internet cd database"),
			QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	ppl7::CDDB::Disc disc;
	// If we have more than one result, we show a selection dialog
	if (matches.size() > 1) {
		CDDBSelect cddbselect(this, wm);
		cddbselect.setMatches(matches);
		if (!cddbselect.exec()) return;
		disc=cddbselect.getSelected();
	} else {
		disc=matches.front();
	}

	// Now we can show the import dialog
	CDDBImport Dialog(this, wm);
	Dialog.setModal(false);
	Dialog.setDisc(disc);
	if (!Dialog.exec()) return;
	if (Dialog.checkAndConfirmOverwrite(DeviceType, DeviceId, Page) != true) return;
	// Now lets to the import
	Dialog.startImport(disc, DeviceType, DeviceId, Page);
	// reopen this device
	OpenTrack(DeviceId, Page, 0);
}


void Edit::renumber()
{
	ppl6::CString Tmp;
	RenumberDialog Dialog(this, wm);
	Dialog.setModal(true);
	Dialog.setOldNumber(DeviceId);
	if (!Dialog.exec()) return;
	ppluint32 newDeviceId=Dialog.getNewNumber();
	if (newDeviceId < 1) {
		QMessageBox::critical(NULL, tr("Error"),
			tr("Invalid number"),
			QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	if (!Dialog.isTargetPathUsable(DeviceType, newDeviceId)) return;
	if (!Dialog.isTargetDeviceFree(DeviceType, newDeviceId)) return;

	// Zielverzeichnisse anlegen
	for (int i=1;i <= datadevice.Pages;i++) {
		ppl6::CString Path=wm->GetAudioPath(DeviceType, newDeviceId, i);
		if (Path.NotEmpty()) {
			ppl6::MkDir(Path, 1);
		}
	}

	// Titel updaten
	for (int p=1;p <= datadevice.Pages;p++) {
		CTrackList* tl=wm->GetTracklist(DeviceType, DeviceId, p);
		if (tl) {
			//printf ("Tracks auf Seite %i: %i\n",p,tl->Num());
			tl->Reset();
			DataTrack* track;
			while ((track=tl->GetNext())) {
				track->DeviceId=newDeviceId;
				wm->TrackStore.Put(track);
				DataTitle* ti=wm->GetTitle(track->TitleId);
				if (ti) {
					ppl6::CString OldFile=wm->GetAudioFilename(ti->DeviceType, ti->DeviceId, ti->Page, ti->Track);
					if (OldFile.NotEmpty()) {
						ppl6::CString NewPath=wm->GetAudioPath(DeviceType, newDeviceId, ti->Page);
						if (NewPath.NotEmpty()) {
							ppl6::CFile::MoveFile(OldFile, NewPath + "/" + GetFilename(OldFile));
						}
					}
					ti->DeviceId=newDeviceId;
					wm->TitleStore.Put(ti);

				}
			}
		}
	}
	// Vorhandene Dateien verschieben

	if (wm->DeviceStore.Renumber(DeviceType, DeviceId, newDeviceId)) {
		//printf ("Renumber ok\n");
		wm->DeviceStore.Update(DeviceType, DeviceId);
		OpenTrack(newDeviceId, Page, 0);
		return;
	}
	//printf ("Renumber FAILED!\n");
}





void Edit::on_coverwidget_imageChanged(const QPixmap& NewCover)
{
	Cover=NewCover;
	UpdateCover();
}

void Edit::on_coverwidget_imageDeleted()
{
	Cover=QPixmap();
	wm->UpdateCoverViewer(Cover);
	if (wm_main->conf.bWriteID3Tags == true) {
		ppl6::CString Path=wm->GetAudioFilename(DeviceType, DeviceId, Page, TrackNum);
		if (Path.NotEmpty()) {
			ppl6::CID3Tag Tag;
			Tag.Load(&Path);
			Tag.RemovePicture(3);
			Tag.Save();
		}
	}
}

void Edit::on_coverwidget_gotFocus()
{
	FixFocus();
}
