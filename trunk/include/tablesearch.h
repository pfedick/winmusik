/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:29 $
 * $Id: tablesearch.h,v 1.1 2010/05/16 12:23:29 pafe Exp $
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

#ifndef SEARCH_H
#define SEARCH_H

#include "winmusik3.h"
#include <QtGui/QWidget>
#include "ui_tablesearch.h"

class TableSearch : public QDialog
{
    Q_OBJECT

protected:
	bool eventFilter(QObject *target, QEvent *event);
public:
	TableSearch(QWidget *parent = 0, CWmClient *client=NULL);
	void setSearchParams(const ppl6::CString &term, CTableStore *store, QString &Title);
	void Search();
    ~TableSearch();

private:
	CWmClient *wm;
	CTableStore *store;
    Ui::TablePopup ui;

private slots:
    void on_searchButton_clicked();
    void on_list_itemDoubleClicked ( QTreeWidgetItem * item, int column );

};

#endif // SEARCH_H
