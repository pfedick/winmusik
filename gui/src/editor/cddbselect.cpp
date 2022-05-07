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
#include <QMenu>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFocusEvent>
#include "cddbselect.h"




CDDBSelect::CDDBSelect(QWidget* parent, CWmClient* wm)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	InstallFilter(ui.tracklist, 1);
	ui.tracklist->setFocus();
	selectedItem=NULL;
}

CDDBSelect::~CDDBSelect()
{

}

void CDDBSelect::InstallFilter(QObject* object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id", id);
}

void CDDBSelect::setMatches(const ppl7::CDDB::Matches& matches)
{
	ppl7::String Tmp;
	ui.tracklist->clear();
	ui.tracklist->setWordWrap(false);
	ui.tracklist->setSortingEnabled(false);
	ppl7::CDDB::Matches::const_iterator it;
	for (it=matches.begin();it != matches.end();++it) {
		CddbItem* item=new CddbItem;
		item->disc=(*it);
		ppl7::String Artist=(*it).Artist;
		ppl7::String Title=(*it).Title;
		item->setText(0, Artist + "- " + Title);

		Tmp.setf("%i", (*it).year);
		item->setText(1, Tmp);

		Tmp.setf("%i", (int)(*it).Tracks.size());
		item->setText(2, Tmp);

		Tmp.setf("%i:%02i", (*it).length / 60, (*it).length % 60);
		item->setText(3, Tmp);

		item->setText(4, (*it).genre);

		ui.tracklist->addTopLevelItem(item);
	}

}



bool CDDBSelect::eventFilter(QObject* target, QEvent* event)
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
	if (consumeEvent(target, event)) return true;
	return QWidget::eventFilter(target, event);
}

bool CDDBSelect::consumeEvent(QObject* target, QEvent* event)
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
	QKeyEvent* keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;

	int type=event->type();
	if (type == QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent*>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (on_KeyPress(target, key, modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	}
	return false;
}

bool CDDBSelect::on_KeyPress(QObject*, int key, int modifier)
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
	if (key == Qt::Key_Escape) {
		on_cancelButton_clicked();
		return true;
		// *************************************************************************** Return/Enter
	} else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
		on_tracklist_itemDoubleClicked(ui.tracklist->currentItem(), 0);
		return true;
		// *************************************************************************** F12
	} else if (key == Qt::Key_F12 && modifier == Qt::NoModifier) {
		on_tracklist_itemDoubleClicked(ui.tracklist->currentItem(), 0);
		return true;
	}
	return false;
}

void CDDBSelect::resizeEvent(QResizeEvent* event)
/*!\brief Größenänderung des Fensters
 *
 * Diese Funktion wird durch Qt aufgerufen, wenn sich die Größe
 * des Fensters ändert, sowie von Edit::UpdateTrackListing, wenn sich der
 * Inhalt der Trackliste ändert.
 */
{
	int w=ui.tracklist->width();
	ui.tracklist->setColumnWidth(1, 60);
	w-=64;
	ui.tracklist->setColumnWidth(2, 40);
	w-=44;
	ui.tracklist->setColumnWidth(3, 60);
	w-=64;
	ui.tracklist->setColumnWidth(4, 100);
	w-=104;
	ui.tracklist->setColumnWidth(0, w);

	QWidget::resizeEvent(event);
}

void CDDBSelect::on_tracklist_itemDoubleClicked(QTreeWidgetItem* item, int)
{
	if (item == NULL) return;
	selectedItem=(CddbItem*)item;
	done(1);
}

void CDDBSelect::on_cancelButton_clicked()
{
	done(0);
}

ppl7::CDDB::Disc CDDBSelect::getSelected()
{
	if (selectedItem) return selectedItem->disc;
	return ppl7::CDDB::Disc();
}
