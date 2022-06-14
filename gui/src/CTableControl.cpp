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
#include "tablecontrol.h"
#include "src/editor/tablesearch.h"

CTableControl::CTableControl()
{
	idWidget=NULL;
	textWidget=NULL;
	nextWidget=NULL;
	oldposition=position=0;
	skipFocusOut=false;
}

void CTableControl::SetWindow(QWidget* window)
{
	this->window=window;
}

void CTableControl::SetCWmClient(CWmClient* wm)
{
	this->wm=wm;
}


void CTableControl::SetIdWidget(QLineEdit* widget)
{
	idWidget=widget;
}

void CTableControl::SetTextWidget(QLineEdit* widget)
{
	textWidget=widget;
}

void CTableControl::SetNextWidget(QWidget* widget)
{
	nextWidget=widget;
}

void CTableControl::SetStore(CTableStore* store)
{
	this->store=store;
}

void CTableControl::Init(QWidget* window, CWmClient* wm, QLineEdit* id, QLineEdit* text, CTableStore* store)
{
	this->window=window;
	this->wm=wm;
	idWidget=id;
	textWidget=text;
	this->store=store;
	UpdateText();
}

bool CTableControl::ConsumeEvent(QObject* target, QEvent* event, int oldpos, int newpos)
{
	QKeyEvent* keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;
	QFocusEvent* focusEvent=NULL;
	int type=event->type();
	oldposition=oldpos;
	position=newpos;
	if (type == QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent*>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
	} else if (type == QEvent::FocusIn) {
		focusEvent=static_cast<QFocusEvent*>(event);
	} else if (type == QEvent::FocusOut) {
		focusEvent=static_cast<QFocusEvent*>(event);
	}
	if (target == idWidget) {
		if (type == QEvent::FocusIn) return on_Id_FocusIn();
		if (type == QEvent::KeyPress) return on_Id_KeyPress(keyEvent, key, modifier);
		if (type == QEvent::KeyRelease) return on_Id_KeyRelease(keyEvent, key, modifier);
	} else if (target == textWidget) {
		if (type == QEvent::FocusIn) return on_Text_FocusIn(focusEvent->reason());
		if (type == QEvent::FocusOut) return on_Text_FocusOut();
	}
	return false;
}


bool CTableControl::on_Id_FocusIn()
{
	idWidget->deselect();
	idWidget->selectAll();
	return false;
}

void CTableControl::UpdateText()
{
	ppl7::String s=idWidget->text();
	s.trim();
	if (s == "*") return;
	uint32_t id=idWidget->text().toInt();
	const CSimpleTable* item=store->GetPtr(id);
	if (item == NULL) {
		//textWidget->setText(wm->Unknown());
		textWidget->setText("");
	} else {
		if (item->Value.notEmpty()) textWidget->setText(item->Value);
		else textWidget->setText("");
	}
}

bool CTableControl::on_Id_KeyPress(__attribute__((unused)) QKeyEvent* event, int key, int modifier)
{
	if (modifier == Qt::NoModifier && key == Qt::Key_F4) {		// Suchen in Tabelle
		ppl7::String Tmp;
		Tmp=idWidget->text();
		TableSearch* search=new TableSearch(window, wm);
		search->setSearchParams(Tmp, store, Title);
		search->Search();
		uint32_t ret=search->exec();
		delete search;
		if (!ret) return false;

		Tmp.setf("%u", ret);
		idWidget->setText(Tmp);
		UpdateText();
		return false;
	}
	return false;
}

bool CTableControl::on_Id_KeyRelease(__attribute__((unused)) QKeyEvent* event, __attribute__((unused)) int key, __attribute__((unused)) int modifier)
{
	UpdateText();
	return false;
}

bool CTableControl::on_Text_FocusIn(int reason)
{
	const CSimpleTable* item=NULL;
	ppl7::String Value=idWidget->text();
	Value.trim();
	// Gibt's die ID?
	int id=Value.toInt();
	if (id > 0 || Value == "*") {
		if (id > 0) item=store->GetPtr(id);
		// Falls mit der Maus reingeklickt wurde, ist es egal
		if (reason == Qt::MouseFocusReason || Value == "*" || (id > 0 && (item == NULL || item->Value.isEmpty()))) {
			if (item == 0 && Value != "*") textWidget->setText("");
			textWidget->deselect();
			textWidget->selectAll();
			return false;
		}
	}
	//printf ("positionold: %i, position: %i\n",oldposition, position);

	if (oldposition < position) {
		skipFocusOut=true;
		if (nextWidget) nextWidget->setFocus();
		else textWidget->nextInFocusChain()->setFocus();
	}
	if (oldposition > position) {
		skipFocusOut=true;
		idWidget->setFocus();
	}
	return true;
}

bool CTableControl::on_Text_FocusOut()
{
	if (skipFocusOut) {
		skipFocusOut=false;
		return false;
	}
	ppl7::String Value;
	ppl7::String ID=idWidget->text();
	ID.trim();
	if (ID.isEmpty()) return false;
	uint32_t id=ID.toInt();
	CSimpleTable label;
	Value=textWidget->text();
	Value.trim();
	if (id > 0) {
		if (store->Exists(id)) label=store->Get(id);
	}
	if (label.Id == 0) {
		// Suchen, ob es den Eintrag schon gibt
		const CSimpleTable* found=store->Find(Value);
		if (found) {
			Value.setf("%u", found->Id);
			idWidget->setText(Value);
			return false;
		}
	}
	// Wir speichern nur wenn ID>0 oder ein Text angegeben wurde
	if (label.Id > 0 || Value.len() > 0) {
		label.SetValue(Value);
		try {
			Value.setf("%u", store->Put(label).Id);
			idWidget->setText(Value);
		} catch (const ppl7::Exception& exp) {
			ShowException(exp, tr("Could not save data"));
		}
	}
	return false;
}


void CTableControl::SetId(uint32_t id)
{
	ppl7::String v;
	v.setf("%u", id);
	idWidget->setText(v);
	UpdateText();
}


int CTableControl::Finish()
{
	const CSimpleTable* item=NULL;

	ppl7::String Value;
	ppl7::String ID=idWidget->text();
	ID.trim();
	uint32_t id=ID.toInt();
	if (id > 0) {
		item=store->GetPtr(id);
		if (!item) return 0;	// Die ID gibt's nicht
		Value=item->Value;
		textWidget->setText(Value);
		return 1;
	}
	Value=textWidget->text();
	Value.trim();
	if (Value.isEmpty() == true || ID != "*") {
		idWidget->setText("0");
		return 1;
	}
	item=store->Find(Value);
	if (item) {
		Value.setf("%u", item->Id);
		idWidget->setText(Value);
		Value=item->Value;
		textWidget->setText(Value);
		return 1;
	}
	CSimpleTable label;
	label.SetValue(Value);
	try {
		Value.setf("%u", store->Put(label).Id);
		idWidget->setText(Value);
		return 1;
	} catch (const ppl7::Exception& exp) {
		ShowException(exp, tr("could not save data"));
	}
	return 0;
}
