/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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
#include "version.h"
#include "menue.h"
#include "src/search/search.h"
#include "src/properties/properties.h"
#include "src/about/about.h"
#include "wmtoolbutton.h"


Menue::Menue(QWidget* parent, CWmClient* client)
	: QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	ui.setupUi(this);
	wm=client;
	searchWindow=NULL;
	QString Title=tr("WinMusik");
	Title+=" ";
	Title+=WM_VERSION;
	Title+=" ";
	Title+=tr("build");
	Title+=" ";
	Title+=WM_REVISION;

	this->setWindowTitle(Title);
	QByteArray r=wm->GetGeometry("menue");
	this->restoreGeometry(r);
	UpdateMenue();

	UpdateInformed=false;
	ui.searchEdit->setFocus();
}

Menue::~Menue()
{
	if (wm) {
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

void Menue::closeEvent(QCloseEvent* event)
{
	if (wm) {
		wm->SaveGeometry("menue", this->saveGeometry());
		wm->MainMenueClosed();
	}
	QWidget::closeEvent(event);
}

void Menue::OpenEditDialog(int traeger)
{
	if (wm) wm->OpenEditor(traeger);
	ui.searchEdit->setFocus();
}

void Menue::OpenDeviceList(int traeger)
{
	if (wm) wm->OpenDeviceList(traeger);
	ui.searchEdit->setFocus();
}

void Menue::OpenSearchDialog()
{
	if (wm) wm->OpenSearch();
	ui.searchEdit->setFocus();
	/*
	Search *w=new Search(NULL,wm);
	//w->Init();

	//w->setWindowFlags(Qt::Dialog|Qt::WindowMinMaxButtonsHint);
	w->show();
	*/
}

void Menue::OpenPropertiesDialog()
{
	Properties* w=new Properties(NULL, wm);
	//w->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint);
	w->exec();

	delete w;
	ui.searchEdit->setFocus();

}


void Menue::OpenAboutDialog()
{
	About w;
	//w.setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowSystemMenuHint);
	w.exec();
	ui.searchEdit->setFocus();
}

void Menue::OpenCoverPrinterDialog()
{
	if (wm) wm->OpenCoverPrinter();
	ui.searchEdit->setFocus();
}

void Menue::OpenPlaylistDialog()
{
	if (wm) wm->OpenPlaylistDialog();
	ui.searchEdit->setFocus();
}

void Menue::OpenSearchlistDialog()
{
	if (wm) wm->OpenSearchlistOverview();
	ui.searchEdit->setFocus();
}


void Menue::on_searchEdit_returnPressed()
{
	ppl7::String words=ui.searchEdit->text();
	words.trim();
	if (words.isEmpty()) return;
	searchWindow=wm->OpenOrReuseSearch(searchWindow);
	Search* win=(Search*)searchWindow;
	win->FastSearch(words);
	ui.searchEdit->setFocus();
}
