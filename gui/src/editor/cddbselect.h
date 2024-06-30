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


#ifndef CDDBSELECT_H_
#define CDDBSELECT_H_

#include "winmusik3.h"
#include <QDialog>
#include <QTreeWidgetItem>

#include "ppl7-audio.h"
#include "ui_cddbselect.h"

class CDDBSelect : public QDialog
{
    Q_OBJECT

public:
    CDDBSelect(QWidget* parent = 0, CWmClient* wm = NULL);
    ~CDDBSelect();

    void setMatches(const ppl7::CDDB::Matches& matches);
    ppl7::CDDB::Disc getSelected();


private:
    class CddbItem : public QTreeWidgetItem
    {
    public:
        ppl7::CDDB::Disc disc;
    };
    Ui::CDDBSelect ui;
    CWmClient* wm;
    CddbItem* selectedItem;

    void InstallFilter(QObject* object, int id);
    bool eventFilter(QObject* target, QEvent* event);
    bool consumeEvent(QObject* target, QEvent* event);
    void resizeEvent(QResizeEvent* event);

    // Globale Events
    bool on_KeyPress(QObject* target, int key, int modifier);

public slots:
    void on_tracklist_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_cancelButton_clicked();
};






#endif /* CDDBSELECT_H_ */
