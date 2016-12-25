/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: shortcutdialog.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2014 Patrick Fedick
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
#include <QMenu>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFocusEvent>
#include "cddbimport.h"




CDDBImport::CDDBImport(QWidget *parent, CWmClient *wm)
: QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	position=1;
	oldposition=1;

	InstallFilter(ui.artist,1);
	InstallFilter(ui.album,2);
	InstallFilter(ui.releaseDate,3);
	InstallFilter(ui.recordDate,4);

	InstallFilter(ui.genreId,5);
	InstallFilter(ui.genre,6);

	InstallFilter(ui.labelId,7);
	InstallFilter(ui.labelName,8);
	InstallFilter(ui.recordSourceId,9);
	InstallFilter(ui.recordSource,10);
	InstallFilter(ui.recordDeviceId,11);
	InstallFilter(ui.recordDevice,12);
	InstallFilter(ui.tags,13);

	TCGenre.Title=tr("Genre");
	TCGenre.Init(this,wm,ui.genreId,ui.genre,&wm->GenreStore);
	TCGenre.SetNextWidget(ui.labelId);

	TCLabel.Title=tr("Label");
	TCLabel.Init(this,wm,ui.labelId,ui.labelName,&wm->LabelStore);
	TCLabel.SetNextWidget(ui.recordSourceId);

	TCRecordSource.Title=tr("Record Source");
	TCRecordSource.Init(this,wm,ui.recordSourceId,ui.recordSource,&wm->RecordSourceStore);
	TCRecordSource.SetNextWidget(ui.recordDeviceId);

	TCRecordDevice.Title=tr("Record Device");
	TCRecordDevice.Init(this,wm,ui.recordDeviceId,ui.recordDevice,&wm->RecordDeviceStore);
	TCRecordDevice.SetNextWidget(ui.tags);

	ui.artist->setFocus();
}

CDDBImport::~CDDBImport()
{

}

void CDDBImport::InstallFilter(QObject *object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id",id);
}

void CDDBImport::setDisc(ppl6::CDDB::Disc &disc)
{
	ui.artist->setText(disc.Artist);

	ppl6::CString Tmp;
	Tmp=disc.Artist+ " - "+disc.Title;
	ui.album->setText(Tmp);

	if (disc.genre.NotEmpty()) {
		ui.genreId->setText("*");
		ui.genre->setText(disc.genre);
		int id=wm->GenreStore.GetId(disc.genre);
		if (id>0) {
			Tmp.Setf("%i",id);
			ui.genreId->setText(Tmp);
			ui.genre->setText(wm->GetGenreText(id));
		}
	}


	// Releasedate
	if (disc.year>0) {
		Tmp.Setf("%u",disc.year*10000);
	} else {
		Tmp=QDate::currentDate().toString("yyyyMMdd");
	}
	int year=Tmp.Mid(0,4).ToInt();
	int month=Tmp.Mid(4,2).ToInt();
	int day=Tmp.Mid(6,2).ToInt();
	if (!month) month=1;
	if (!day) day=1;
	QDate Date;
	Date.setDate(year,month,day);
	ui.releaseDate->setDate(Date);

	// Aufnahmedatum
	ui.recordDate->setDate(QDate::currentDate());

	updateTracklist(disc);

}

void CDDBImport::updateTracklist(ppl6::CDDB::Disc &disc)
{
	ppl6::CString Tmp;
	ui.tracklist->clear();
	ui.tracklist->setWordWrap(false);
	ui.tracklist->setSortingEnabled(false);
	ppl6::CDDB::Disc::TrackList::const_iterator it;
	for (it=disc.Tracks.begin();it!=disc.Tracks.end();++it) {
		QTreeWidgetItem *item=new QTreeWidgetItem;
		Tmp.Setf("%i",(*it).number);
		item->setText(0,Tmp);
		ppl6::CString Artist=ppl6::Trim((*it).Artist);
		ppl6::CString Title=ppl6::Trim((*it).Title);
		ppl6::CString Version=getVersionFromTitle(Title,(*it).length);

		item->setText(1,Artist + "- "+Title);
		item->setText(2,Version);
		Tmp.Setf("%i:%02i",(*it).length/60, (*it).length%60);
		item->setText(3,Tmp);

		ui.tracklist->addTopLevelItem(item);
	}

}

bool CDDBImport::eventFilter(QObject *target, QEvent *event)
/*!\brief Event Handler
 *
 * Dies ist eine überladene Funktion, die von Qt immer dann aufgerufen wird, wenn bei einem Widget, bei dem ein
 * Filter installiert wurde (Edit::InstallFilter) ein beliebiger Event auftritt. Die Funktion ruft lediglich
 * die Funktion Edit::consumeEvent auf. Liefert diese \c true zurück, wird davon ausgegangen, dass der
 * Event behandelt wurde und der Default-Eventhandler (QWidget::eventFilter) nicht aufgerufen werden muss.
 *
 * \param[in] target Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] event Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Liefert \c true zurück, wenn der Event von Edit::consumeEvent verarbeitet wurde. Andernfalls
 * wird die übergeordnete Basisfunktion QWidget::eventFilter aufgerufen und deren Returncode zurückgeliefert.
 */
{
	if (consumeEvent(target,event)) return true;
	return QWidget::eventFilter(target,event);
}

bool CDDBImport::consumeEvent(QObject *target, QEvent *event)
/*!\brief Event verarbeiten
 *
 * Diese Funktion wird von Edit::eventFilter aufgerufen, wenn Qt einen Event für ein Widget registriert hat,
 * für das ein Filter installiert wurde (Edit::InstallFilter).
 *
 * Die Funktion prüft, ob für das angegebene \p target ein Handler existiert und ruft diesen auf. Der
 * spezielle Handler muss \c true zurückgeben, wenn er den Event verarbeitet hat und der Standardhandler
 * von Qt nicht mehr aufgerufen werden muss. Die Rückgabe von \c false bewirkt, dass der
 * Standardhandler QWidget::eventFilter aufgerufen wird.
 *
 * \param[in] target Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] event Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Gibt \c true zurück, wenn der Event verarbeit wurde, sonst \c false
 */
{
	ppl6::CString Tmp;
	QKeyEvent *keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;
    //QFocusEvent *focusEvent=NULL;

	// Id auslesen
	int id=target->property("id").toInt();
	int type=event->type();
	if (type==QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent *>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (on_KeyPress(target,key,modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	} else if (type==QEvent::FocusIn || type==QEvent::FocusOut) {
        //focusEvent=static_cast<QFocusEvent *>(event);
		if (type==QEvent::FocusIn) {
			position=id;
		}
		else if (type==QEvent::FocusOut) {
			oldposition=id;
		}
	}

	if (target==ui.genre || target==ui.genreId) {
		return TCGenre.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.labelName || target==ui.labelId) {
		return TCLabel.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.recordSource || target==ui.recordSourceId) {
		return TCRecordSource.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.recordDevice || target==ui.recordDeviceId) {
		return TCRecordDevice.ConsumeEvent(target,event,oldposition,position);
	}
	return false;
}

bool CDDBImport::on_KeyPress(QObject *, int key, int modifier)
/*!\brief Globale KeyPress Events behandeln
 *
 * Diese Funktion behandelt globale KeyPress Events, als Beispielsweise das Drücken der ESC-Taste,
 * F1-F12 aber auch die Return/Enter-Taste und weitere Shortcuts.
 *
 * \param[in] target Das QObject, auf das sich der Event bezieht
 * \param[in] key Code der Taste (Qt::Key)
 * \param[in] modifier Modifizierung dazu (Qt::KeyboardModifiers)
 *
 * \returns Die Funktion gibt \b true zurück, wenn der Event behandelt wurde und das System sonst
 * nichts mehr machen muss, andernfalls \b false.
 */
{
	// ******************************************************************************* ESC
	if (key==Qt::Key_Escape) {
		on_cancelButton_clicked();
		return true;
		// *************************************************************************** Return/Enter
	} else if (key==Qt::Key_Return || key==Qt::Key_Enter) {
		MoveToNextWidget();
		return true;
		// *************************************************************************** F12
	} else if (key==Qt::Key_F12 && modifier==Qt::NoModifier) {
		on_okButton_clicked();
		return true;
	}
	return false;
}

void CDDBImport::resizeEvent ( QResizeEvent * event )
/*!\brief Größenänderung des Fensters
 *
 * Diese Funktion wird durch Qt aufgerufen, wenn sich die Größe
 * des Fensters ändert, sowie von Edit::UpdateTrackListing, wenn sich der
 * Inhalt der Trackliste ändert.
 */
{
	int w=ui.tracklist->width();
	ui.tracklist->setColumnWidth(0,60);
	w-=64;
	ui.tracklist->setColumnWidth(3,60);
	w-=64;
	ui.tracklist->setColumnWidth(1,w*70/100);
	ui.tracklist->setColumnWidth(2,w*30/100);
	QWidget::resizeEvent(event);
}

void CDDBImport::MoveToNextWidget()
/*!\brief zum nächsten Eingabefeld wechseln
 *
 * Diese Funktion prüft anhand der Variablen \c position in welchem Eingabefeld sich der Focus
 * gerade befindet und setzt den Focus auf das nachfolgende Eingabefeld.
 */
{
	switch (position) {
		case 1: ui.album->setFocus(); break;
		case 2: ui.releaseDate->setFocus(); break;
		case 3: ui.recordDate->setFocus(); break;
		case 4: ui.genreId->setFocus(); break;
		case 5: ui.genre->setFocus(); break;
		case 6: ui.labelId->setFocus(); break;
		case 7: ui.labelName->setFocus(); break;
		case 8: ui.recordSourceId->setFocus(); break;
		case 9: ui.recordSource->setFocus(); break;
		case 10: ui.recordDeviceId->setFocus(); break;
		case 11: ui.recordDevice->setFocus(); break;
		case 12: ui.tags->setFocus(); break;
		case 13: ui.okButton->setFocus(); break;
	};
}

QWidget *CDDBImport::GetWidgetFromPosition(int position)
{
	switch (position) {
		case 1: return ui.artist;
		case 2: return ui.album;
		case 3: return ui.releaseDate;
		case 4: return ui.recordDate;
		case 5: return ui.genreId;
		case 6: return ui.genre;
		case 7: return ui.labelId;
		case 8: return ui.labelName;
		case 9: return ui.recordSourceId;
		case 10: return ui.recordSource;
		case 11: return ui.recordDeviceId;
		case 12: return ui.recordDevice;
		case 13: return ui.tags;
	}
	return NULL;
}


void CDDBImport::FixFocus()
{
	QWidget *widget=this->GetWidgetFromPosition(position);
	if (widget) widget->setFocus();
}


void CDDBImport::on_okButton_clicked()
{
	// Wir müssen sicherstellen, dass die Tabellen-Index-Felder gültige Werte enthalten
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
	done(1);
}

void CDDBImport::on_cancelButton_clicked()
{
	done(0);
}

bool CDDBImport::checkAndConfirmOverwrite(ppluint8 devicetype, ppluint32 deviceid, ppluint8 page)
{
	CTrackList *tracklist=wm->GetTracklist(devicetype,deviceid,page);
	if (tracklist->Num()==0) return true;

	int ret=QMessageBox::warning(this, tr("WinMusik: Attention"),
			tr("This device already has tracks in the database. When you start "
					"the import from the CDDB now, the new tracks will be appended, which is "
					"probably not what you want.\n\n"
					"Do you really want to append the tracks?"),
			QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
	if (ret==QMessageBox::No) return false;
	return true;
}

void CDDBImport::startImport(ppl6::CDDB::Disc &disc, ppluint8 devicetype, ppluint32 deviceid, ppluint8 page)
{
	ppl6::CString Tmp;
	CTrackList *tracklist=wm->GetTracklist(devicetype,deviceid,page);
	DataDevice datadevice;
	if (!wm->DeviceStore.GetCopy(devicetype, deviceid, &datadevice)) {
		wm->RaiseError(NULL,tr("Unexpected Error, Device does not exist"));
		return;
	}
	if (datadevice.GetTitle().IsEmpty()) {
		Tmp=ui.album->text();
		datadevice.SetTitle(Tmp);
	}
	if (datadevice.LabelId==0) {
		datadevice.LabelId=ui.labelId->text().toInt();
	}
	if (datadevice.PurchaseId==0) {
		datadevice.PurchaseId=ui.recordSourceId->text().toInt();
	}
	int trackNum=tracklist->GetMax()+1;
	ppl6::CDDB::Disc::TrackList::const_iterator it;
	for (it=disc.Tracks.begin();it!=disc.Tracks.end();++it) {
		DataTitle Ti;
		getTitle(Ti,*it);
		Ti.DeviceType=devicetype;
		Ti.DeviceId=deviceid;
		Ti.Page=page;
		Ti.Track=trackNum;
		addDataFromFile(Ti);
		if (!saveTitle(tracklist,Ti)) {
			it=disc.Tracks.end();
			break;
		}
		trackNum++;
	}

	// Tonträger aktualisieren
	wm->DeviceStore.Put(&datadevice);
	wm->DeviceStore.Update(devicetype,deviceid);
}


ppl6::CString CDDBImport::getVersionFromTitle(ppl6::CString &Title, int length)
{
	ppl6::CString Version="";
	if (Title.PregMatch("/^(.*)\\((.*?)\\)$")) {
		Title=ppl6::Trim(Title.GetMatch(1));
		Version=ppl6::Trim(Title.GetMatch(2));
	} else {
		if (length<300) Version=tr("Single");
		else Version=tr("Original Version");
	}
	return Version;
}

void CDDBImport::getTitle(DataTitle &Ti, const ppl6::CDDB::Track &track)
{
	ppl6::CString Tmp;

	Ti.SetTitle(track.Title);
	Tmp=ppl6::Trim(track.Artist);
	if (Tmp.IsEmpty()) {
		Tmp=ui.artist->text();
		Tmp.Trim();
	}
	Ti.SetArtist(Tmp);

	// Tags
	Tmp=ui.tags->text();
	Tmp=ppl6::Trim(ppl6::LCase(Tmp));
	Ti.SetTags(Tmp);


	ppl6::CString Title=ppl6::Trim(track.Title);
	ppl6::CString Version=getVersionFromTitle(Title,track.length);

	Ti.SetTitle(Title);

	// Version
	Ti.VersionId=wm->VersionStore.FindOrAdd(Version);
	// Genre
	Ti.GenreId=ui.genreId->text().toInt();
	// Label
	Ti.LabelId=ui.labelId->text().toInt();
	// Aufnahmequelle
	Ti.RecordSourceId=ui.recordSourceId->text().toInt();
	// Aufnahmegerät
	Ti.RecordDeviceId=ui.recordDeviceId->text().toInt();

	// Länge
	Ti.Length=track.length;

	Tmp=ui.album->text();
	Ti.SetAlbum(Tmp);

	// Erscheinungsjahr
	QDate Date=ui.releaseDate->date();
	Tmp=Date.toString("yyyyMMdd");
	Ti.ReleaseDate=Tmp.ToInt();

	// Aufnahmedatum
	Date=ui.recordDate->date();
	Tmp=Date.toString("yyyyMMdd");
	//printf ("Date: %s\n",(const char*)Tmp);
	Ti.RecordDate=Tmp.ToInt();


	// Ton
	Ti.Channels=2;

	// Qualität
	Ti.Quality=0;

	// Bewertung
	Ti.Rating=0;

	// Flags
	Ti.Flags=3;
}

void CDDBImport::addDataFromFile(DataTitle &Ti)
{
	ppl6::CString Path=wm->GetAudioFilename(Ti.DeviceType,Ti.DeviceId,Ti.Page,Ti.Track);
	if (Path.NotEmpty()==true) {
		TrackInfo tinfo;
		if (getTrackInfoFromFile(tinfo,Path)) {
			if (tinfo.Ti.Key != Ti.Key) {
				Ti.Key=tinfo.Ti.Key;
			}
			if (Ti.EnergyLevel==0 && tinfo.Ti.EnergyLevel>0) {
				Ti.EnergyLevel=tinfo.Ti.EnergyLevel;
			}
			if (Ti.BPM==0 && tinfo.Ti.BPM>0) {
				Ti.BPM=tinfo.Ti.BPM;
			}
			if (tinfo.Ti.Bitrate>0 && tinfo.Ti.Bitrate!=Ti.Bitrate) {
				Ti.Bitrate=tinfo.Ti.Bitrate;
			}
			if (tinfo.Ti.Length>0 && tinfo.Ti.Length!=Ti.Length) {
				Ti.Length=tinfo.Ti.Length;
			}
			if (tinfo.Ti.CoverPreview.Size()>0) {
				Ti.CoverPreview=tinfo.Ti.CoverPreview;
			}
		}
	}

}

int CDDBImport::saveTitle(CTrackList *tracklist, DataTitle &Ti)
{
	// Titel speichern
	if (!wm->TitleStore.Put(&Ti)) {
		wm->RaiseError(this,tr("Could not save Title in TitleStore"));
		return 0;
	}
	// An die Hashes dürfen wir natürlich nicht den Pointer auf den lokalen Titel "Ti" übergeben,
	// sondern den Pointer innerhalb der Datenbank
	DataTitle *dt=wm->TitleStore.Get(Ti.TitleId);
	if (dt) wm->Hashes.AddTitle(Ti.TitleId,dt);
	// Track speichern
	DataTrack Track;
	Track.SetValue(Ti.DeviceType,Ti.DeviceId,Ti.Page,Ti.Track,Ti.TitleId);
	if (!tracklist->Put(&Track)) {
		wm->RaiseError(this,tr("Could not save Track in TrackList"));
		ui.artist->setFocus();
		return 0;
	}

	if (wm_main->conf.bWriteID3Tags==true) {
		wm->SaveID3Tags(Track.Device, Track.DeviceId, Track.Page, Track.Track,Ti);
	}

	return 1;
}

