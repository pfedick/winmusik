/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: editdevice.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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

#include "winmusik3.h"
#include "../include/editdevice.h"
#include "tablesearch.h"

EditDevice::EditDevice(QWidget *parent, CWmClient *wm, int typ, ppluint32 DeviceId)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	this->parent=parent;
	DeviceType=typ;
	this->DeviceId=DeviceId;
	QDate Date;
	ppl6::CString Tmp, Subject;

	if (Device.DeviceId) {
		Tmp=tr("Edit Device:");
	} else {
		Tmp=tr("New Device:");
	}
	Tmp+=" ";
	Tmp+=wm->GetDeviceName(DeviceType);
	Subject=wm->GetDeviceName(DeviceType);
	if (Device.DeviceId) {
		Tmp.Concatf(" %u",DeviceId);
		Subject.Concatf(" %u",DeviceId);
	}
	setWindowTitle(Tmp);
	ui.Subject->setText(Subject);
	ui.deviceIcon->setPixmap(wm->GetDevicePixmap(DeviceType));
	setWindowIcon(wm->GetDeviceIcon(DeviceType));
	ui.currency->setText(wm->conf.Currency);
	ui.purchaseDate->setDate(wm->GetLatestPurchaseDate());

	// Bereits vorhandene Daten laden


	// Eventfilter installieren
	InstallFilter(ui.title,1);
	InstallFilter(ui.subTitle,2);
	InstallFilter(ui.pages,3);
	InstallFilter(ui.labelId,4);
	InstallFilter(ui.labelText,5);
	InstallFilter(ui.purchaseId,6);
	InstallFilter(ui.purchaseText,7);
	InstallFilter(ui.purchasePrice,8);
	InstallFilter(ui.purchaseDate,9);
	position=oldposition=0;

	Label.Title=tr("Producer / Label");
	Label.Init(this,wm,ui.labelId,ui.labelText,&wm->LabelStore);
	Label.SetNextWidget(ui.purchaseId);

	PurchaseSource.Title=tr("Purchase store");
	PurchaseSource.Init(this,wm,ui.purchaseId,ui.purchaseText,&wm->PurchaseSourceStore);
	PurchaseSource.SetNextWidget(ui.purchasePrice);


	// Device laden
	if (wm->DeviceStore.GetCopy(DeviceType,DeviceId,&Device)) {
		if (Device.Title) ui.title->setText(Device.Title);
		if (Device.SubTitle) ui.subTitle->setText(Device.SubTitle);
		ui.pages->setValue(Device.Pages);
		Label.SetId(Device.LabelId);
		PurchaseSource.SetId(Device.PurchaseId);
		Tmp.Setf("%0.2f",Device.PurchasePrice);
		ui.purchasePrice->setText(Tmp);
		// Aufnahmedatum
		Tmp.Setf("%u",Device.PurchaseDate);
		Date=QDate::fromString(Tmp,"yyyyMMdd");
		ui.purchaseDate->setDate(Date);
		// Länge
		Tmp=Int2Time(Device.Length);
		ui.length->setText(Tmp);


	} else {
		Device.DeviceId=DeviceId;
		Device.DeviceType=DeviceType;
		Date=QDate::currentDate();
		ui.purchaseDate->setDate(Date);
	}
}

EditDevice::~EditDevice()
{
	if (parent) parent->setFocus();
}

void EditDevice::InstallFilter(QObject *object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id",id);
}


bool EditDevice::eventFilter(QObject *target, QEvent *event)
/*!\brief Event Handler
 *
 * Dies ist eine überladene Funktion, die von Qt immer dann aufgerufen wird, wenn bei einem Widget, bei dem ein
 * Filter installiert wurde (Edit::InstallFilter) ein beliebiger Event auftritt. Die Funktion ruft lediglich
 * die Funktion Edit::consumeEvent auf. Liefert diese \i true zurück, wird davon ausgegangen, dass der
 * Event behandelt wurde und der Default-Eventhandler (QWidget::eventFilter) nicht aufgerufen werden muss.
 *
 * \param[in] Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Liefert \b true zurück, wenn der Event von Edit::consumeEvent verarbeitet wurde. Andernfalls
 * wird die übergeordnete Basisfunktion QWidget::eventFilter aufgerufen und deren Returncode zurückgeliefert.
 */
{
	if (consumeEvent(target,event)) return true;
	return QWidget::eventFilter(target,event);
}

bool EditDevice::consumeEvent(QObject *target, QEvent *event)
/*!\brief Event verarbeiten
 *
 * Diese Funktion wird von Edit::eventFilter aufgerufen, wenn Qt einen Event für ein Widget registriert hat,
 * für das ein Filter installiert wurde (Edit::InstallFilter).
 *
 * Die Funktion prüft, ob für das angegebene \p target ein Handler existiert und ruft diesen auf. Der
 * spezielle Handler muss \b true zurückgeben, wenn er den Event verarbeitet hat und der Standardhandler
 * von Qt nicht mehr aufgerufen werden muss. Die Rückgabe von \b false bewirkt, dass der
 * Standardhandler QWidget::eventFilter aufgerufen wird.
 *
 * \param[in] Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Gibt \b true zurück, wenn der Event verarbeit wurde, sonst \b false
 */
{
	QKeyEvent *keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;
	QFocusEvent *focusEvent=NULL;

	// Id auslesen
	__attribute__ ((unused)) int id=target->property("id").toInt();
	int type=event->type();
	if (type==QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent *>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (modifier==Qt::NoModifier && (key==Qt::Key_Return || key==Qt::Key_Enter)) {
			if (target!=ui.okButton && target!=ui.cancelButton) {
				nextWidget(target);
				return false;
			}
		} else if (modifier==Qt::NoModifier && key==Qt::Key_F12) {
			if (Save()) done(DeviceId);
			return true;
		}
	} else if (type==QEvent::FocusIn) {
		focusEvent=static_cast<QFocusEvent *>(event);
		position=id;
	} else if (type==QEvent::FocusOut) {
		focusEvent=static_cast<QFocusEvent *>(event);
		oldposition=id;
	}
	if (target==ui.labelId || target==ui.labelText) {
		return Label.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.purchaseId || target==ui.purchaseText) {
		return PurchaseSource.ConsumeEvent(target,event,oldposition,position);
	}

	return false;
}

bool EditDevice::nextWidget(QObject *object)
{
	if (object==ui.title) ui.subTitle->setFocus();
	else if (object==ui.subTitle) ui.pages->setFocus();
	else if (object==ui.pages) ui.length->setFocus();
	else if (object==ui.length) ui.labelId->setFocus();
	else if (object==ui.labelId) ui.labelText->setFocus();
	else if (object==ui.labelText) ui.purchaseId->setFocus();
	else if (object==ui.purchaseId) ui.purchaseText->setFocus();
	else if (object==ui.purchaseText) ui.purchasePrice->setFocus();
	else if (object==ui.purchasePrice) ui.purchaseDate->setFocus();
	else if (object==ui.purchaseDate) ui.okButton->setFocus();
	return false;
}

bool EditDevice::Save()
{
	ppl6::CString Tmp;
	Tmp=ui.title->text();
	Tmp.Trim();
	Device.SetTitle(Tmp);
	Tmp=ui.subTitle->text();
	Tmp.Trim();
	Device.SetSubTitle(Tmp);
	Device.LabelId=ui.labelId->text().toInt();
	Device.PurchaseId=ui.purchaseId->text().toInt();
	Tmp=ui.length->text();
	Device.Length=Time2Int(Tmp);

	QDate Date=ui.purchaseDate->date();
	Tmp=Date.toString("yyyyMMdd");
	Device.PurchaseDate=Tmp.ToInt();

	if (!Device.DateCreated) {
		Date=QDate::currentDate();
		Tmp=Date.toString("yyyyMMdd");
		Device.DateCreated=Tmp.ToInt();
	}
	Device.DeviceId=DeviceId;
	Device.DeviceType=DeviceType;
	Device.Pages=ui.pages->value();
	Device.PurchasePrice=ui.purchasePrice->text().toFloat();

	if (wm->DeviceStore.Put(&Device)) {
		DeviceId=Device.DeviceId;
		return true;
	}
	return false;
}

void EditDevice::on_okButton_clicked()
{
	if (Save()) done(DeviceId);
}

void EditDevice::on_cancelButton_clicked()
{
	done(0);
}

