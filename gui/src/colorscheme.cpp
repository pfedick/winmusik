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

#include "colorscheme.h"
#include <QApplication>
#include <QPalette>

ColorScheme::ColorScheme()
{
    init();
}

void ColorScheme::init()
{
    base=QApplication::palette().base();
    alternateBase=QApplication::palette().alternateBase();

    sameKey=base;
    alternateSameKey=alternateBase;
    relatedKey=base;
    alternateRelatedKey=alternateBase;
    boostKey=base;
    alternateBoostKey=alternateBase;
    boostKey2=base;
    alternateBoostKey2=alternateBase;
    cueConsistencyError=base;

    sameKey.setStyle(Qt::SolidPattern);
    sameKey.setStyle(Qt::SolidPattern);
    relatedKey.setStyle(Qt::SolidPattern);
    alternateRelatedKey.setStyle(Qt::SolidPattern);
    boostKey.setStyle(Qt::SolidPattern);
    alternateBoostKey.setStyle(Qt::SolidPattern);
    boostKey2.setStyle(Qt::SolidPattern);
    alternateBoostKey2.setStyle(Qt::SolidPattern);
    cueConsistencyError.setStyle(Qt::SolidPattern);

    if (base.color().lightness() < 127) {
        initDark();
    } else {
        initLight();
    }
}


void ColorScheme::initDark()
{
    QColor bgc=base.color();
    QColor bgca=alternateBase.color();

    sameKey.setColor(QColor(bgc.lightness(), 127, bgc.lightness()));
    alternateSameKey.setColor(QColor(bgca.lightness(), 127, bgca.lightness()));
    relatedKey.setColor(QColor(bgc.lightness(), 64, bgc.lightness()));
    alternateRelatedKey.setColor(QColor(bgca.lightness(), 64, bgca.lightness()));
    boostKey.setColor(QColor(128, bgc.lightness(), bgc.lightness()));
    alternateBoostKey.setColor(QColor(128, bgca.lightness(), bgca.lightness()));
    boostKey2.setColor(QColor(200, bgc.lightness(), bgc.lightness()));
    alternateBoostKey2.setColor(QColor(200, bgca.lightness(), bgca.lightness()));
    cueConsistencyError.setColor(QColor(128, 0, 0));
}

void ColorScheme::initLight()
{
    sameKey.setColor(QColor(192, 230, 192));
    alternateSameKey.setColor(QColor(192, 230, 192));
    relatedKey.setColor(QColor(192, 255, 192));
    alternateRelatedKey.setColor(QColor(192, 255, 192));
    boostKey.setColor(QColor(220, 192, 192));
    alternateBoostKey.setColor(QColor(220, 192, 192));
    boostKey2.setColor(QColor(255, 192, 192));
    alternateBoostKey2.setColor(QColor(255, 192, 192));
    cueConsistencyError.setColor(QColor(255, 128, 128));

}
