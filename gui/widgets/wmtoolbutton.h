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

#ifndef WMTOOLBUTTON_H_
#define WMTOOLBUTTON_H_

#include <QToolButton>

class WMToolButton : public QToolButton
{
	Q_OBJECT

public:
	WMToolButton(QWidget* parent = 0);
	~WMToolButton();

protected:
	void mousePressEvent(QMouseEvent* event);

signals:
	void controlClicked();

};

#endif /* WMTOOLBUTTON_H_ */
