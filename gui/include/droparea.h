/*
 * droparea.h
 *
 *  Created on: 22.12.2010
 *      Author: patrick
 */

#ifndef DROPAREA_H_
#define DROPAREA_H_

#include <QLabel>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>


class DropArea : public QLabel
{
	Q_OBJECT

	public:
		DropArea(QWidget *parent = 0);

	public slots:
		void clear();

	signals:
		void changed(const QMimeData *mimeData = 0);

	protected:
		void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		void dragLeaveEvent(QDragLeaveEvent *event);
		void dropEvent(QDropEvent *event);

};

#endif /* DROPAREA_H_ */
