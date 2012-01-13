/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: tablesearch.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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
#include "../include/tablesearch.h"
#include <QKeyEvent>

TableSearch::TableSearch(QWidget *parent, CWmClient *client)
    : QDialog(parent)
{
	wm=client;
	ui.setupUi(this);
}

TableSearch::~TableSearch()
{

}

bool TableSearch::eventFilter(QObject *target, QEvent *event)
{
	int type=event->type();
	if (target==ui.search && type==QEvent::KeyPress) {
		QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
		int key=keyEvent->key();
		Qt::KeyboardModifiers modifier=keyEvent->modifiers();
		if (key==Qt::Key_Return && modifier==Qt::ShiftModifier) {
			done(ui.search->text().toInt());
			return true;
		}
	}
	return QWidget::eventFilter(target,event);
}


void TableSearch::setSearchParams(const ppl6::CString &term, CTableStore *store, QString &Title)
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
	ppl6::CWString Tmp;
	ui.list->clear();
	Tmp=ui.search->text();
	ppl6::CTree Result;
	ui.list->setWordWrap(false);
	ui.list->setSortingEnabled(false);

	if (store->FindAll(Tmp,Result)) {
		ppl6::CString Text;
		CSimpleTable *t;
		WMTreeItem *item;
		Result.Reset();
		while ((t=(CSimpleTable *)Result.GetNext())) {
			item=new WMTreeItem;
			item->Id=t->Id;
			Text.Setf("%6i",t->Id);
			item->setText(0,Text);
			item->setText(1,t->Value);
			ui.list->addTopLevelItem(item);
		}
	}
	Result.Clear(true);
	ui.list->setWordWrap(true);
	ui.list->setSortingEnabled(true);
	ui.list->sortByColumn(1,Qt::AscendingOrder);

}

void TableSearch::on_searchButton_clicked()
{
	Search();
	ui.search->setFocus();
	ui.search->deselect();
	ui.search->selectAll();

}

void TableSearch::on_list_itemDoubleClicked (QTreeWidgetItem * item, int column )
{
	WMTreeItem *i=(WMTreeItem*)item;
	done(i->Id);
}
