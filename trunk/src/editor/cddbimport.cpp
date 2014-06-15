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
		ppl6::CString Artist=(*it).Artist;
		ppl6::CString Title=(*it).Title;
		ppl6::CString Version="";
		if (Title.PregMatch("/^(.*)\\((.*?)\\)$")) {
			Title=Title.GetMatch(1);
			Version=Title.GetMatch(2);
		}

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
	QFocusEvent *focusEvent=NULL;

	// Id auslesen
	int id=target->property("id").toInt();
	int type=event->type();
	if (type==QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent *>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (on_KeyPress(target,key,modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	} else if (type==QEvent::FocusIn || type==QEvent::FocusOut) {
		focusEvent=static_cast<QFocusEvent *>(event);
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

bool CDDBImport::on_KeyPress(QObject *target, int key, int modifier)
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
	} else if (key==Qt::Key_F12 && modifier==Qt::NoModifier && position>3) {
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
	done(1);
}

void CDDBImport::on_cancelButton_clicked()
{
	done(0);
}
