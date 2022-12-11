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

#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QColor>
#include <QBrush>


class ColorScheme
{
private:
    void initDark();
    void initLight();
public:
    ColorScheme();
    void init();
    QBrush base;
    QBrush alternateBase;
    QBrush sameKey;
    QBrush alternateSameKey;
    QBrush relatedKey;
    QBrush alternateRelatedKey;
    QBrush boostKey;
    QBrush alternateBoostKey;
    QBrush boostKey2;
    QBrush alternateBoostKey2;
    QBrush cueConsistencyError;
};



#endif // COLORSCHEME_H
