/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/28 15:47:58 $
 * $Id: devicelist.cpp,v 1.1 2010/11/28 15:47:58 pafe Exp $
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
#include "../include/devicelist.h"

DeviceList::DeviceList(QWidget *parent, CWmClient *wm, int typ)
    : QWidget(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	setAttribute(Qt::WA_DeleteOnClose,true);
	DeviceType=typ;
	ui.deviceIcon->setPixmap(wm->GetDevicePixmap(DeviceType));
	setWindowIcon(wm->GetDeviceIcon(DeviceType));
	ppl6::CString Name;
	Name.Setf("devicelist_type_%i",DeviceType);
	this->restoreGeometry(wm->GetGeometry(Name));
	ui.list->installEventFilter(this);
	ui.newButton->installEventFilter(this);


	switch (typ) {
		case 1:	// Cassette
			setWindowTitle(tr("List of Music Cassettes"));
			break;
		case 2:	// CD
			setWindowTitle(tr("List of Audio CDs"));
			break;
		case 3:	// Data CD
			setWindowTitle(tr("List of Data CDs"));
			break;
		case 4:	// Vinyl Record
			setWindowTitle(tr("List of Vinyl Records"));
			break;
		case 5:	// VHS
			setWindowTitle(tr("List of Video Tapes"));
			break;
		case 6:	// Other
			setWindowTitle(tr("List of Other Media Types"));
			break;
		case 7:	// MP3
			setWindowTitle(tr("List of MP3 CDs"));
			break;
		case 8:	// Tape
			setWindowTitle(tr("List of Music Tapes"));
			break;
		case 9:	// DVD
			setWindowTitle(tr("List of Audio DVDs"));
			break;
	}
	ppl6::CString Tmp;
	ui.deviceName->setText(windowTitle());
	setWindowTitle(tr("WinMusik: ")+ui.deviceName->text());
	int highest=wm->DeviceStore.GetHighestDevice(DeviceType);
	ppluint64 totalLength=0;
	ppluint32 totalTracks=0;
	ppluint32 totalDevices=0;

    QString Style="QTreeView::item {\n"
    		"border-right: 1px solid #b9b9b9;\n"
    		"border-bottom: 1px solid #b9b9b9;\n"
    		"}\n"
    		"QTreeView::item:selected {\n"
    		//"border-top: 1px solid #80c080;\n"
    		//"border-bottom: 1px solid #80c080;\n"
    		"background: #000070;\n"
    		"color: rgb(255, 255, 255);\n"
    		"}\n"
    		"";
    ui.list->setStyleSheet(Style);
    ui.list->setSortingEnabled(false);

	for (ppluint32 i=1;i<=highest;i++) {
		DataDevice d;
		if (wm->DeviceStore.GetCopy(DeviceType,i,&d)) {
			totalDevices++;
			totalTracks+=d.NumTracks;
			totalLength+=d.Recorded;

			DeviceItem *item=new(DeviceItem);
			item->Id=d.DeviceId;
			Tmp.Setf("%5i",d.DeviceId);
			item->setText(0,Tmp);
			Tmp.Clear();
			if (d.Title) Tmp=d.Title;
			if (d.SubTitle) {
				if (Tmp.NotEmpty()) Tmp+=", ";
				Tmp+=d.SubTitle;
			}
			item->setText(1,Tmp);
			Tmp.Setf("%5i",d.NumTracks);
			item->setText(2,Tmp);

			if (d.Recorded>0) {
				int h=(int)(d.Recorded/3600);
				int m=d.Recorded-(h*3600);
				int s=m%60;
				m=m/60;
				Tmp.Setf("%0i:%02i:%02i",h,m,s);
				item->setText(3,Tmp);
			} else {
				item->setText(3,"?");
			}

			// Startdatum
			QDate Date;
			Tmp.Setf("%u",d.FirstDate);
			Date=QDate::fromString(Tmp,"yyyyMMdd");
			item->setText(4,Date.toString(tr("yyyy-MM-dd")));
			// Enddatum
			Tmp.Setf("%u",d.LastDate);
			Date=QDate::fromString(Tmp,"yyyyMMdd");
			item->setText(5,Date.toString(tr("yyyy-MM-dd")));


			ui.list->addTopLevelItem(item);
		}
	}
	int h=(int)(totalLength/3600);
	int m=totalLength-(h*3600);
	int s=m%60;
	m=m/60;
	Tmp.Setf("%0i:%02i:%02i",h,m,s);
	ui.totalLength->setText(Tmp);
	Tmp.Setf("%i",totalTracks);
	ui.totalTracks->setText(Tmp);
	Tmp.Setf("%i",totalDevices);
	ui.totalDevices->setText(Tmp);


	ui.list->setSortingEnabled(true);
	ui.list->setFocus();
}

DeviceList::~DeviceList()
{
	if (wm) {
		wm->DeviceListClosed(this);
	}
}


void DeviceList::Resize()
{
	int w=ui.list->width();
	ui.list->setColumnWidth(0,60);
	ui.list->setColumnWidth(2,80);
	ui.list->setColumnWidth(3,80);
	ui.list->setColumnWidth(4,80);
	ui.list->setColumnWidth(5,100);
	w=w-60-80-80-80-100;
	if (w<100) w=100;
	ui.list->setColumnWidth(1,w);


}

void DeviceList::showEvent(QShowEvent * event)
{
	Resize();
	QWidget::showEvent(event);
}
void DeviceList::resizeEvent(QResizeEvent * event)
{
	Resize();
	QWidget::resizeEvent(event);
}

void DeviceList::closeEvent(QCloseEvent *event)
{
	ppl6::CString Name;
	Name.Setf("devicelist_type_%i",DeviceType);
	if (wm) {
		wm->SaveGeometry(Name,this->saveGeometry());
	}
    QWidget::closeEvent(event);
}


void DeviceList::ReloadTranslation()
{
	ui.retranslateUi(this);
}


void DeviceList::on_list_itemDoubleClicked ( QTreeWidgetItem * item, int column)
{
	DeviceItem *d=(DeviceItem*)item;
	if (!d) return;
	if (!d->Id) return;
	wm->OpenEditor(DeviceType,d->Id);
}

bool DeviceList::eventFilter(QObject *target, QEvent *event)
{
	if (consumeEvent(target,event)) return true;
	return QWidget::eventFilter(target,event);
}

bool DeviceList::consumeEvent(QObject *target, QEvent *event)
{
	ppl6::CString Tmp;
	QKeyEvent *keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;

	int type=event->type();
	if (type==QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent *>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (on_KeyPress(target,key,modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	}

	return false;
}

// Globale Events, Fkeys
bool DeviceList::on_KeyPress(QObject *target, int key, int modifier)
{
	// ******************************************************************************* ESC
	if (key==Qt::Key_Escape) {
		close();
		return true;
	}
	return false;
}

void DeviceList::on_newButton_clicked()
{
	wm->OpenEditor(DeviceType,0);
}

