/*
 * wmtoolbutton.h
 *
 *  Created on: 28.11.2010
 *      Author: patrick
 */

#ifndef WMTOOLBUTTON_H_
#define WMTOOLBUTTON_H_

#include <QToolButton>

class WMToolButton : public QToolButton
{
	Q_OBJECT

	public:
		WMToolButton(QWidget *parent = 0);
		~WMToolButton();

	protected:
		void mousePressEvent ( QMouseEvent * event );

	signals:
		void controlClicked();

};

#endif /* WMTOOLBUTTON_H_ */
