
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

#ifndef KEYWHEEL_H_
#define KEYWHEEL_H_

#define WITH_QT		// Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann

#include <QLabel>
#include <QPainter>
#include <QFont>


class KeyWheel : public QLabel
{
	Q_OBJECT

private:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);

	int shiftAngle(int angle);
	int getAngle(int x, int y);
	int getRadius(int x, int y);
	int getKeyFromClickPosition(const QPoint& p);
	void drawKey(QPainter& painter, QPen& PenFront, QPen& PenBorder, int x, int y, int key);


	int key;

	QString keyname[26];

public:
	KeyWheel(QWidget* parent = 0);
	~KeyWheel();
	void setKeyName(int key, const QString& name);
	void setCurrentKey(int key);
	int currentKey() const;

signals:
	void clicked(int key);
};



#endif /* KEYWHEEL_H_ */
