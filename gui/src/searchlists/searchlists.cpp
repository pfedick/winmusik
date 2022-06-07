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

#include <unistd.h>
#include <QClipboard>
#include <QMenu>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QMouseEvent>
#include "searchlists.h"
#include "csearchlist.h"

Searchlists::Searchlists(QWidget* parent, CWmClient* wm)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	if (wm) wm->RegisterWindow(WindowType::SearchlistOverview, this);
	setAttribute(Qt::WA_DeleteOnClose, true);

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
	ui.treeWidget->setStyleSheet(Style);
	Update();
}

Searchlists::~Searchlists()
{
	if (wm) wm->UnRegisterWindow(WindowType::SearchlistOverview, this);
}

void Searchlists::show()
{
	SetWindowGeometry(this, "Searchlists");
	QWidget::show();
}

void Searchlists::Update()
{
	ui.treeWidget->clear();

	// vorhandene Suchlisten anzeigen
	CSearchlist sl;
	ppl7::Dir Dir;
	ppl7::Dir::Iterator dirit;
	try {
		Dir.open(wm->conf.DataPath);
		Dir.reset(dirit);
		ppl7::DirEntry entry;
		while (Dir.getNextRegExp(entry, dirit, "/^searchlist.*\\.xml$/")) {
			try {
				sl.load(entry.File);
				SearchlistTreeItem* item=new SearchlistTreeItem;
				item->Filename=entry.File;
				item->setText(0, sl.name());
				item->setText(1, ppl7::ToString("%i", sl.size()));
				item->setText(2, sl.dateCreated().get("%Y-%m-%d"));
				item->setText(3, sl.dateUpdated().get("%Y-%m-%d"));

				ui.treeWidget->addTopLevelItem(item);
			} catch (const ppl7::Exception& exp) {
				exp.print();
			}
		}
	} catch (...) {

	}
}

void Searchlists::Resize()
{
	int s=ui.treeWidget->width();
	ui.treeWidget->setColumnWidth(3, 100);
	ui.treeWidget->setColumnWidth(2, 100);
	ui.treeWidget->setColumnWidth(1, 50);
	ui.treeWidget->setColumnWidth(0, s - 102 - 102 - 52);
}

void Searchlists::showEvent(QShowEvent* event)
{
	Resize();
	QWidget::showEvent(event);
}
void Searchlists::resizeEvent(QResizeEvent* event)
{
	Resize();
	QWidget::resizeEvent(event);
}

void Searchlists::closeEvent(QCloseEvent* event)
{
	wm_main->SaveGeometry("Searchlists", this->saveGeometry());
	QWidget::closeEvent(event);
}


void Searchlists::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void Searchlists::on_newSearchlistButton_clicked()
{
	int highest=1;
	int id;
	ppl7::Dir Dir;
	ppl7::Dir::Iterator dirit;
	try {
		Dir.open(wm->conf.DataPath);
		Dir.reset(dirit);
		ppl7::DirEntry entry;
		while (Dir.getNextRegExp(entry, dirit, "/^searchlist[0-9]+\\.xml$/")) {
			ppl7::Array Matches;
			if (entry.Filename.pregMatch("/searchlist([0-9]+)\\.xml$/", Matches)) {
				id=Matches[1].toInt();
				if (id > highest) highest=id;
			}
		}
	} catch (...) {}

	ppl7::String Filename=wm->conf.DataPath;
	Filename.trimRight("/");
	Filename.trimRight("\\");
	Filename.appendf("/searchlist%05i.xml", highest + 1);
	wm->OpenSearchlistDialog(Filename);
}

void Searchlists::on_deleteSearchlistButton_clicked()
{
	SearchlistTreeItem* it=(SearchlistTreeItem*)ui.treeWidget->currentItem();
	if (!it) return;
	if (QMessageBox::question(this, tr("WinMusik: Delete searchlist"),
		tr("Are you sure you want to delete this searchlist?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
		== QMessageBox::No) return;
	ppl7::File::unlink(it->Filename);
	int index=ui.treeWidget->indexOfTopLevelItem(it);
	if (index < 0) return;
	ui.treeWidget->topLevelItem(index);
	delete it;
}

void Searchlists::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int)
{
	SearchlistTreeItem* it=(SearchlistTreeItem*)item;
	if (!it) return;
	wm->OpenSearchlistDialog(it->Filename);

}
