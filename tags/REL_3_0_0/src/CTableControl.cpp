/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:30 $
 * $Id: CTableControl.cpp,v 1.1 2010/05/16 12:23:30 pafe Exp $
 *
 *
 * Copyright (c) 2009 Patrick Fedick
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
#include "tablesearch.h"

CTableControl::CTableControl()
{
	idWidget=NULL;
	textWidget=NULL;
	nextWidget=NULL;
	oldposition=position=0;
	skipFocusOut=false;
}

void CTableControl::SetWindow(QWidget *window)
{
	this->window=window;
}

void CTableControl::SetCWmClient(CWmClient *wm)
{
	this->wm=wm;
}


void CTableControl::SetIdWidget(QLineEdit *widget)
{
	idWidget=widget;
}

void CTableControl::SetTextWidget(QLineEdit *widget)
{
	textWidget=widget;
}

void CTableControl::SetNextWidget(QWidget *widget)
{
	nextWidget=widget;
}

void CTableControl::SetStore(CTableStore *store)
{
	this->store=store;
}

void CTableControl::Init(QWidget *window, CWmClient *wm, QLineEdit *id, QLineEdit *text, CTableStore *store)
{
	this->window=window;
	this->wm=wm;
	idWidget=id;
	textWidget=text;
	this->store=store;
	UpdateText();
}

bool CTableControl::ConsumeEvent(QObject *target, QEvent *event, int oldpos, int newpos)
{
	QKeyEvent *keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;
	QFocusEvent *focusEvent=NULL;
	int type=event->type();
	oldposition=oldpos;
	position=newpos;
	if (type==QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent *>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
	} else if (type==QEvent::FocusIn) {
		focusEvent=static_cast<QFocusEvent *>(event);
	} else if (type==QEvent::FocusOut) {
		focusEvent=static_cast<QFocusEvent *>(event);
	}
	if (target==idWidget) {
		if (type==QEvent::FocusIn) return on_Id_FocusIn();
		if (type==QEvent::KeyPress) return on_Id_KeyPress(keyEvent,key,modifier);
		if (type==QEvent::KeyRelease) return on_Id_KeyRelease(keyEvent,key,modifier);
	} else if (target==textWidget) {
		if (type==QEvent::FocusIn) return on_Text_FocusIn(focusEvent->reason());
		if (type==QEvent::FocusOut) return on_Text_FocusOut();
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
	ppl6::CString s=idWidget->text();
	s.Trim();
	if (s=="*") return;
	ppluint32 id=idWidget->text().toInt();
	CSimpleTable *item=store->Get(id);
	if (item==NULL) {
		//textWidget->setText(wm->Unknown());
		textWidget->setText("");
	} else {
		if (item->Value) textWidget->setText(item->Value);
		else textWidget->setText("");
	}
}

bool CTableControl::on_Id_KeyPress(__attribute__ ((unused)) QKeyEvent *event,int key,int modifier)
{
	if (modifier==Qt::NoModifier && key==Qt::Key_F4) {		// Suchen in Tabelle
		ppl6::CString Tmp;
		Tmp=idWidget->text();
		TableSearch *search=new TableSearch(window,wm);
		search->setSearchParams(Tmp,store,Title);
		search->Search();
		ppluint32 ret=search->exec();
		delete search;
		if (!ret) return false;

		Tmp.Setf("%u",ret);
		idWidget->setText(Tmp);
		UpdateText();
		return false;
	}
	return false;
}

bool CTableControl::on_Id_KeyRelease(__attribute__ ((unused)) QKeyEvent *event,__attribute__ ((unused)) int key,__attribute__ ((unused)) int modifier)
{
	UpdateText();
	return false;
}

bool CTableControl::on_Text_FocusIn(int reason)
{
	CSimpleTable *item=NULL;
	ppl6::CString Value=idWidget->text();
	Value.Trim();
	// Gibt's die ID?
	int id=Value.ToInt();
	if (id>0 || Value=="*") {
		if (id>0) item=store->Get(id);
		// Falls mit der Maus reingeklickt wurde, ist es egal
		if (reason==Qt::MouseFocusReason || Value=="*" || (id>0 && (item==NULL || item->Value==NULL))) {
			if (item==0 && Value!="*") textWidget->setText("");
			textWidget->deselect();
			textWidget->selectAll();
			return false;
		}
	}
	//printf ("positionold: %i, position: %i\n",oldposition, position);

	if (oldposition<position) {
		skipFocusOut=true;
		if (nextWidget) nextWidget->setFocus();
		else textWidget->nextInFocusChain()->setFocus();
	}
	if (oldposition>position) {
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
	ppl6::CString Value;
	ppl6::CString ID=idWidget->text();
	ID.Trim();
	if (ID.IsEmpty()) return false;
	ppluint32 id=ID.ToInt();
	CSimpleTable label, *found;
	Value=textWidget->text();
	Value.Trim();
	if (id>0) {
		store->GetCopy(id,&label);
	}
	if (label.Id==0) {
		// Suchen, ob es den Eintrag schon gibt
		found=store->Find((const char*)Value);
		if (found) {
			Value.Setf("%u",found->Id);
			idWidget->setText(Value);
			return false;
		}
	}
	// Wir speichern nur wenn ID>0 oder ein Text angegeben wurde
	if (label.Id>0 || Value.Len()>0) {
		label.SetValue(Value);
		if (store->Put(&label)) {
			Value.Setf("%u",label.Id);
			idWidget->setText(Value);
		}
	}
	return false;
}


void CTableControl::SetId(ppluint32 id)
{
	ppl6::CString v;
	v.Setf("%u",id);
	idWidget->setText(v);
	UpdateText();
}


int CTableControl::Finish()
{
	CSimpleTable *item=NULL;

	ppl6::CString Value;
	ppl6::CString ID=idWidget->text();
	ID.Trim();
	ppluint32 id=ID.ToInt();
	if (id>0) {
		item=store->Get(id);
		if (!item) return 0;	// Die ID gibt's nicht
		Value=item->Value;
		textWidget->setText(Value);
		return 1;
	}
	Value=textWidget->text();
	Value.Trim();
	if (Value.IsEmpty()==1 || ID!="*") {
		idWidget->setText("0");
		return 1;
	}
	item=store->Find((const char*)Value);
	if (item) {
		Value.Setf("%u",item->Id);
		idWidget->setText(Value);
		Value=item->Value;
		textWidget->setText(Value);
		return 1;
	}
	CSimpleTable label;
	label.SetValue(Value);
	if (store->Put(&label)) {
		Value.Setf("%u",label.Id);
		idWidget->setText(Value);
		return 1;
	}
	return 0;
}
