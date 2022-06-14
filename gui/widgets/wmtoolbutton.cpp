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

#include "wmtoolbutton.h"
#include <QMouseEvent>
#include <QInputEvent>


WMToolButton::WMToolButton(QWidget* parent)
	: QToolButton(parent)
{
	//printf ("WMToolButton\n");
}

WMToolButton::~WMToolButton()
{

}


void WMToolButton::mousePressEvent(QMouseEvent* event)
{
	//printf ("Klick\n");
	if (event->type() == QEvent::MouseButtonPress && event->modifiers() == Qt::ControlModifier) {
		controlClicked();
		return;
	}


	QToolButton::mousePressEvent(event);
}
