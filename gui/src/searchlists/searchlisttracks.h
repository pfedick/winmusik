/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: Playlist.h,v 1.1 2010/11/14 13:20:11 pafe Exp $
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

#ifndef SEARCHLISTTRACKS_H_
#define SEARCHLISTTRACKS_H_

#include <QTreeWidget>
#include <QDomElement>
#include "winmusik3.h"

class SearchlistItem;

class SearchlistTracks : public QTreeWidget
{
		Q_OBJECT
	private:
		//void mouseMoveEvent ( QMouseEvent * event );
		//void mousePressEvent ( QMouseEvent * event );
		//void mouseReleaseEvent ( QMouseEvent * event );
		QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;
		void dragEnterEvent ( QDragEnterEvent * event);
		void dragMoveEvent(QDragMoveEvent *e);
		//bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
		void dropEvent ( QDropEvent * event );
		void deleteSourceItems(QDropEvent * event);

		QTreeWidgetItem *lastmoveitem;

	public:
		SearchlistTracks(QWidget * parent = 0);
		~SearchlistTracks();
		void deleteSelectedTracks();


	signals:
		void itemDropped(SearchlistItem *item);
		void changed();

		/*
		void unselectItems();
		void deleteSelectedItems();
		void deleteItems(QList<QTreeWidgetItem *>items);
		void selectItems(QList<QTreeWidgetItem *>items);
		*/
};


#endif /* SEARCHLISTTRACKS_H_ */
