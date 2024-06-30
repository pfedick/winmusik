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
#include "tablesearch.h"
#include <QKeyEvent>

TableSearch::TableSearch(QWidget* parent, CWmClient* client)
	: QDialog(parent)
{
	wm=client;
	ui.setupUi(this);
}

TableSearch::~TableSearch()
{

}

bool TableSearch::eventFilter(QObject* target, QEvent* event)
{
	int type=event->type();
	if (target == ui.search && type == QEvent::KeyPress) {
		QKeyEvent* keyEvent=static_cast<QKeyEvent*>(event);
		int key=keyEvent->key();
		Qt::KeyboardModifiers modifier=keyEvent->modifiers();
		if (key == Qt::Key_Return && modifier == Qt::ShiftModifier) {
			done(ui.search->text().toInt());
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}


void TableSearch::setSearchParams(const ppl7::String& term, CTableStore* store, QString& Title)
{
	ui.search->setText(term);
	setWindowTitle(Title);
	this->store=store;
	this->setWindowModality(Qt::WindowModal);
	ui.search->installEventFilter(this);
	ui.search->setFocus();
	ui.search->deselect();
	ui.search->selectAll();
}

void TableSearch::Search()
{
	ppl7::String Tmp;
	ui.list->clear();
	Tmp=ppl7::Trim(ui.search->text());
	CTableStore::IndexTree Result;
	ui.list->setWordWrap(false);
	ui.list->setSortingEnabled(false);
	if (store->FindAll(Tmp, Result)) {
		ppl7::String Text;

		WMTreeItem* item;
		CTableStore::IndexTree::const_iterator it;
		for (it=Result.begin();it != Result.end();++it) {
			const CSimpleTable* t=store->GetPtr(*it);
			if (t) {
				item=new WMTreeItem;
				item->Id=*it;
				Text.setf("%6i", item->Id);
				item->setText(0, Text);
				item->setText(1, t->Value);
				ui.list->addTopLevelItem(item);
			}
		}
	}
	ui.list->setWordWrap(true);
	ui.list->setSortingEnabled(true);
	ui.list->sortByColumn(1, Qt::AscendingOrder);

}

void TableSearch::on_searchButton_clicked()
{
	Search();
	ui.search->setFocus();
	ui.search->deselect();
	ui.search->selectAll();

}

void TableSearch::on_list_itemDoubleClicked(QTreeWidgetItem* item, int)
{
	WMTreeItem* i=(WMTreeItem*)item;
	done(i->Id);
}
