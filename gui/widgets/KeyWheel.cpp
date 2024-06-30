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

#include "winmusik3.h"
#include "keywheel.h"
#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QFont>
#include <math.h>

KeyWheel::KeyWheel(QWidget* parent)
	: QLabel(parent)
{
	key=0;
	setCurrentKey(0);
}

KeyWheel::~KeyWheel()
{

}

void KeyWheel::setKeyName(int key, const QString& name)
{
	if (key < 0 || key>25) return;
	keyname[key]=name;
}

void KeyWheel::drawKey(QPainter& painter, QPen& PenFront, QPen& PenBorder, int x, int y, int key)
{
	QString& text=keyname[key];
	if (key == this->key) {
		x++;
		y++;
	}
	QRect r=painter.boundingRect(QRect(), Qt::AlignLeft | Qt::AlignTop, text);
	//printf ("width: %i, height: %i\n",r.width(),r.height());
	x=x - (r.width() / 2);
	y=y + (r.height() / 2);
	painter.setPen(PenBorder);
	painter.drawText(x + 1, y + 1, text.trimmed());
	painter.setPen(PenFront);
	painter.drawText(x, y, text.trimmed());

}

void KeyWheel::paintEvent(QPaintEvent* event)
{
	QLabel::paintEvent(event);
	QPainter painter(this);
	QFont Font;
	Font.setFamily(wm_main->conf.PrinterFont);
	Font.setPointSize(7);
	Font.setBold(false);
	QColor labelColor(255, 255, 255);

	QPen PenWhite;
	PenWhite.setStyle(Qt::SolidLine);
	PenWhite.setWidth(1);
	PenWhite.setBrush(Qt::white);

	QPen PenBlack;
	PenBlack.setStyle(Qt::SolidLine);
	PenBlack.setWidth(1);
	PenBlack.setBrush(Qt::black);

	painter.setPen(PenWhite);

	painter.setFont(Font);

	drawKey(painter, PenWhite, PenBlack, 63, 63, 25);

	drawKey(painter, PenWhite, PenBlack, 63, 29, 15);
	drawKey(painter, PenWhite, PenBlack, 80, 34, 17);
	drawKey(painter, PenWhite, PenBlack, 93, 46, 19);
	drawKey(painter, PenWhite, PenBlack, 97, 63, 21);
	drawKey(painter, PenWhite, PenBlack, 93, 80, 23);
	drawKey(painter, PenWhite, PenBlack, 80, 93, 1);
	drawKey(painter, PenWhite, PenBlack, 63, 98, 3);
	drawKey(painter, PenWhite, PenBlack, 46, 93, 5);
	drawKey(painter, PenWhite, PenBlack, 34, 80, 7);
	drawKey(painter, PenWhite, PenBlack, 29, 63, 9);
	drawKey(painter, PenWhite, PenBlack, 34, 46, 11);
	drawKey(painter, PenWhite, PenBlack, 46, 33, 13);

	drawKey(painter, PenWhite, PenBlack, 63, 10, 16);
	drawKey(painter, PenWhite, PenBlack, 90, 17, 18);
	drawKey(painter, PenWhite, PenBlack, 109, 37, 20);
	drawKey(painter, PenWhite, PenBlack, 117, 63, 22);
	drawKey(painter, PenWhite, PenBlack, 109, 90, 24);
	drawKey(painter, PenWhite, PenBlack, 90, 110, 2);
	drawKey(painter, PenWhite, PenBlack, 63, 117, 4);
	drawKey(painter, PenWhite, PenBlack, 36, 110, 6);
	drawKey(painter, PenWhite, PenBlack, 17, 90, 8);
	drawKey(painter, PenWhite, PenBlack, 10, 63, 10);
	drawKey(painter, PenWhite, PenBlack, 17, 37, 12);
	drawKey(painter, PenWhite, PenBlack, 36, 17, 14);




}

void KeyWheel::mousePressEvent(QMouseEvent* event)
{
	QPoint p=event->pos();
		//int angle=shiftAngle(getAngle(p.x(),p.y()));
		//int radius=getRadius(p.x(),p.y());
	int newkey=getKeyFromClickPosition(p);
	if (newkey != key) {
		setCurrentKey(newkey);
		emit clicked(newkey);
	}
	//printf ("x=%i, y=%i, Winkel=%i, newkey=%i, radius=%i\n",p.x(),p.y(),angle,newkey, radius);
	QLabel::mousePressEvent(event);
}

int KeyWheel::getKeyFromClickPosition(const QPoint& p)
{
	// Inner Radius=24
	// Middle Radius=43
	// Outer Radius=62
	int radius=getRadius(p.x(), p.y());
	if (radius < 24) return 0;

	int angle=shiftAngle(getAngle(p.x(), p.y()));
	int slice=angle / 30;

	if (radius < 43) {
		switch (slice) {
			case 0: return 15;
			case 1: return 17;
			case 2: return 19;
			case 3: return 21;
			case 4: return 23;
			case 5: return 1;
			case 6: return 3;
			case 7: return 5;
			case 8: return 7;
			case 9: return 9;
			case 10: return 11;
			case 11: return 13;

		}
	} else if (radius < 62) {
		switch (slice) {
			case 0: return 16;
			case 1: return 18;
			case 2: return 20;
			case 3: return 22;
			case 4: return 24;
			case 5: return 2;
			case 6: return 4;
			case 7: return 6;
			case 8: return 8;
			case 9: return 10;
			case 10: return 12;
			case 11: return 14;

		}
	}
	return key;
}

void KeyWheel::setCurrentKey(int key)
{
	this->key=key;
	switch (key) {
		case 0: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0000.png")); break;
		case 1: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0012.png")); break;
		case 2: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0011.png")); break;
		case 3: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0014.png")); break;
		case 4: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0013.png")); break;
		case 5: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0016.png")); break;
		case 6: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0015.png")); break;
		case 7: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0018.png")); break;
		case 8: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0017.png")); break;
		case 9: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0020.png")); break;
		case 10: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0019.png")); break;
		case 11: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0022.png")); break;
		case 12: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0021.png")); break;
		case 13: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0024.png")); break;
		case 14: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0023.png")); break;
		case 15: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0002.png")); break;
		case 16: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0001.png")); break;
		case 17: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0004.png")); break;
		case 18: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0003.png")); break;
		case 19: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0006.png")); break;
		case 20: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0005.png")); break;
		case 21: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0008.png")); break;
		case 22: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0007.png")); break;
		case 23: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0010.png")); break;
		case 24: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0009.png")); break;
		default: this->setPixmap(QPixmap(":/openkey/resources/keywheel/key0000.png")); break;
	}
}

int KeyWheel::currentKey() const
{
	return key;
}

int KeyWheel::shiftAngle(int angle)
{
	angle+=15;
	if (angle > 359) angle-=360;
	return angle;
}

int KeyWheel::getAngle(int x, int y)
{
	if (x >= 64 && y <= 64) {
		double b=x - 64;
		double a=64 - y;
		double c=sqrt(a * a + b * b);
		return (int)(90.0 - ((asin(a / c) * 180.0) / 3.1415926535));
	} else if (x >= 64 && y > 64) {
		double b=x - 64;
		double a=y - 64;
		double c=sqrt(a * a + b * b);
		return (int)(90.0 + ((asin(a / c) * 180.0) / 3.1415926535));
	} else if (x < 64 && y>64) {
		double b=64 - x;
		double a=y - 64;
		double c=sqrt(a * a + b * b);
		return (int)(270 - ((asin(a / c) * 180.0) / 3.1415926535));
	} else if (x < 64 && y < 64) {
		double b=64 - x;
		double a=64 - y;
		double c=sqrt(a * a + b * b);
		return (int)(270 + ((asin(a / c) * 180.0) / 3.1415926535));
	}
	return 0;
}

int KeyWheel::getRadius(int x, int y)
{
	int b=0, a=0;
	if (x >= 64 && y <= 64) {
		b=x - 64;
		a=64 - y;
	} else if (x >= 64 && y > 64) {
		b=x - 64;
		a=y - 64;
	} else if (x < 64 && y>64) {
		b=64 - x;
		a=y - 64;
	} else if (x < 64 && y < 64) {
		b=64 - x;
		a=64 - y;
	}
	return (int)sqrt(a * a + b * b);
}
