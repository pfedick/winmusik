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

#include "searchlisttracks.h"
#include "searchlistdialog.h"

#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QDragMoveEvent>
#include <QDomDocument>
#include "csearchlist.h"

SearchlistTracks::~SearchlistTracks()
{

}

SearchlistTracks::SearchlistTracks(QWidget * parent)
	:QTreeWidget(parent)
{
	lastmoveitem=NULL;
}

/*
void SearchlistTracks::mouseMoveEvent ( QMouseEvent * event )
{
	//printf ("SearchlistTracks::mouseMoveEvent\n");
	//playlist->on_tracks_MouseMove(event);
	QTreeWidget::mouseMoveEvent(event);
}

void SearchlistTracks::mousePressEvent ( QMouseEvent * event )
{
	//playlist->on_tracks_MouseButtonPress(event);
	//printf ("SearchlistTracks::mousePressEvent\n");
	QTreeWidget::mousePressEvent(event);
}

void SearchlistTracks::mouseReleaseEvent ( QMouseEvent * event )
{
	//playlist->on_tracks_MouseButtonRelease(event);
	//printf ("SearchlistTracks::mouseReleaseEvent\n");
	QTreeWidget::mouseReleaseEvent(event);
}
*/


QMimeData *SearchlistTracks::mimeData(const QList<QTreeWidgetItem *> items) const
{
	//printf("SearchlistTracks::mimeData\n");
	QList<QUrl> list;
	ppl6::CString xml;
	xml="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	xml+="<winmusikSearchlist version=\"1\">\n";
	xml+="<tracks>\n";
	for (int i=0;i<items.size();i++) {
		xml+=((SearchlistDialog::SearchlistTreeItem*)items[i])->Track.exportXML();
	}
	xml+="</tracks>\n";
	xml+="</winmusikSearchlist>\n";
	QMimeData *mimeData = new QMimeData;
	mimeData->setText(xml);
	//xml.Print(true);
	//mimeData->setImageData(Icon);
	return mimeData;
}

void SearchlistTracks::dragEnterEvent (QDragEnterEvent * event)
{
	if (event->source()==this) {
		event->ignore();
		return;
	}
	//printf("SearchlistTracks::dragEnterEvent\n");
	event->accept();
}

void SearchlistTracks::dragMoveEvent(QDragMoveEvent *e)
{
	//printf("SearchlistTracks::dragMoveEvent\n");
	e->accept();
}

/*
bool SearchlistTracks::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction)
{
	printf ("SearchlistTracks::dropMimeData, parent=%tu, index=%i\n",(ptrdiff_t)parent,index);
	lastmoveitem=NULL;
	return true;
}
*/

void SearchlistTracks::dropEvent ( QDropEvent * event )
{
	//printf ("SearchlistTracks::dropEvent, action: %i\n",event->dropAction());
	const QMimeData *data=event->mimeData();
	if (!data->hasText()) {
		event->ignore();
		return;
	}
	QDomDocument doc("winmusikSearchlist");
	doc.setContent(data->text());
	QDomElement root=doc.documentElement();
	if (root.tagName()!="winmusikSearchlist" || root.attribute("version")!="1") {
		event->ignore();
		return;

	}
	event->accept();
	ppl6::CArray rows;
	rows.Explode(data->text(),"<searchlistitem>");
	for (int i=1;i<rows.Num();i++) {
		SearchlistItem *item=new SearchlistItem();
		item->importXML(rows.GetString(i));
		emit itemDropped(item);
	}
	emit changed();
	if (event->dropAction()==Qt::MoveAction && event->source()!=NULL) {
		((SearchlistTracks*)event->source())->deleteSelectedTracks();
	}
}

void SearchlistTracks::deleteSelectedTracks()
{
	QList<QTreeWidgetItem *> list=selectedItems();
	for (int i=0;i<list.size();i++) {
		SearchlistDialog::SearchlistTreeItem *item=(SearchlistDialog::SearchlistTreeItem*)list[i];
		int index=indexOfTopLevelItem (item);
		if (index>=0) {
			item=(SearchlistDialog::SearchlistTreeItem*) takeTopLevelItem(index);
			if (item) delete item;
		}
	}
	emit changed();
}

