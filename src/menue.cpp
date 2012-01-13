/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/11/28 16:06:16 $
 * $Id: menue.cpp,v 1.5 2010/11/28 16:06:16 pafe Exp $
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
#include "menue.h"
#include "search.h"
#include "properties.h"
#include "about.h"
#include "updater.h"
#include "wmtoolbutton.h"


Menue::Menue(QWidget *parent, CWmClient *client)
    : QWidget(parent)
{
	ui.setupUi(this);
	wm=client;
	QString Title=tr("WinMusik");
	Title+=" ";
	Title+=WM_VERSION;
	this->setWindowTitle(Title);
	QByteArray r=wm->GetGeometry("menue");
	this->restoreGeometry(r);
	UpdateMenue();

	UpdateInformed=false;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(on_TimerUpdate()));
	timer->start(3000);

	ui.playlistButton->setVisible(false);
}

Menue::~Menue()
{
	if (wm) {
		wm->SaveGeometry("menue",this->saveGeometry());
		wm->MainMenueClosed();
	}
}

void Menue::UpdateMenue()
{
	if (!wm) return;
	ui.toolButton_1->setVisible(wm->conf.VisibleDevice[1]);
	ui.toolButton_2->setVisible(wm->conf.VisibleDevice[2]);
	ui.toolButton_3->setVisible(wm->conf.VisibleDevice[3]);
	ui.toolButton_4->setVisible(wm->conf.VisibleDevice[4]);
	ui.toolButton_5->setVisible(wm->conf.VisibleDevice[5]);
	ui.toolButton_6->setVisible(wm->conf.VisibleDevice[6]);
	ui.toolButton_7->setVisible(wm->conf.VisibleDevice[7]);
	ui.toolButton_8->setVisible(wm->conf.VisibleDevice[8]);
	ui.toolButton_9->setVisible(wm->conf.VisibleDevice[9]);
}

void Menue::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void Menue::closeEvent(QCloseEvent *event)
{
	if (wm) {
		wm->SaveGeometry("menue",this->saveGeometry());
	}
    QWidget::closeEvent(event);
}

void Menue::OpenEditDialog(int traeger)
{
	if (wm) wm->OpenEditor(traeger);
}

void Menue::OpenDeviceList(int traeger)
{
	if (wm) wm->OpenDeviceList(traeger);
}

void Menue::OpenSearchDialog()
{
	if (wm) wm->OpenSearch();
	/*
	Search *w=new Search(NULL,wm);
	//w->Init();

	//w->setWindowFlags(Qt::Dialog|Qt::WindowMinMaxButtonsHint);
	w->show();
	*/
}

void Menue::OpenPropertiesDialog()
{
	Properties *w=new Properties(NULL, wm);
	//w->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint);
	w->exec();

	delete w;

}


void Menue::OpenAboutDialog()
{
	About w;
	//w.setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint);
	w.exec();
}

void Menue::OpenCoverPrinterDialog()
{
	if (wm) wm->OpenCoverPrinter();
}

void Menue::OpenPlaylistDialog()
{
	if (wm) wm->OpenPlaylistDialog();
}


void Menue::on_TimerUpdate()
{
	if (!wm->UpdateChecker) return;
	if (UpdateInformed) return;
	CUpdateChecker *cuc=(CUpdateChecker*) wm->UpdateChecker;
	if (cuc->manualcheck==true) return;
	//printf ("Timer\n");
	if (cuc->updateavailable) {
		UpdateInformed=true;
		Updater *w=new Updater;
		w->Init(cuc->UpdateInfo,cuc->CurrentVersion,cuc->CurrentReleasedate,cuc->DownloadLink);
		w->exec();
		delete w;

	}
}
