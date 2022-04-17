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



#ifndef TABLECONTROL_H_
#define TABLECONTROL_H_
#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QFocusEvent>

class CTableControl
{
	Q_DECLARE_TR_FUNCTIONS(CTableControl)

	private:
		QLineEdit	*idWidget;
		QLineEdit	*textWidget;
		QWidget		*nextWidget;
		CTableStore *store;
		QWidget		*window;
		CWmClient	*wm;
		int			oldposition, position;
		bool		skipFocusOut;

	    bool on_Id_FocusIn();
	    bool on_Id_KeyPress(QKeyEvent *event,int key,int modifier);
	    bool on_Id_KeyRelease(QKeyEvent *event,int key,int modifier);
	    bool on_Text_FocusOut();
	    bool on_Text_FocusIn(int reason);
	    void UpdateText();

	public:
		QString Title;
		CTableControl();
		void SetWindow(QWidget *window);
		void SetCWmClient(CWmClient *wm);
		void SetIdWidget(QLineEdit *widget);
		void SetTextWidget(QLineEdit *widget);
		void SetNextWidget(QWidget *widget);
		void SetStore(CTableStore *store);
		void Init(QWidget *window, CWmClient *wm, QLineEdit *id, QLineEdit *text, CTableStore *store);
		bool ConsumeEvent(QObject *target, QEvent *event, int oldpos, int newpos);

		void SetId(ppluint32 id);
		int Finish();

};


#endif /* TABLECONTROL_H_ */
