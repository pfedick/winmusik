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


#ifndef SEARCH_H
#define SEARCH_H

#include "winmusik3.h"
#include <QWidget>
#include "ui_tablesearch.h"

class TableSearch : public QDialog
{
	Q_OBJECT

protected:
	bool eventFilter(QObject* target, QEvent* event);
public:
	TableSearch(QWidget* parent = 0, CWmClient* client=NULL);
	void setSearchParams(const ppl7::String& term, CTableStore* store, QString& Title);
	void Search();
	~TableSearch();

private:
	CWmClient* wm;
	CTableStore* store;
	Ui::TablePopup ui;

private slots:
	void on_searchButton_clicked();
	void on_list_itemDoubleClicked(QTreeWidgetItem* item, int column);

};

#endif // SEARCH_H
