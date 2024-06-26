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



#ifndef ASYNCHRONOUSMESSAGE_H
#define ASYNCHRONOUSMESSAGE_H

#include <QWidget>
#include <QTimer>
#include "ui_asynchronousMessage.h"
#include "winmusik3.h"

class asynchronousMessage : public QWidget
{
    Q_OBJECT

public:
    asynchronousMessage(QWidget* parent = 0);
    ~asynchronousMessage();
    void ReloadTranslation();

    void setMessagePixmap(const QPixmap& pixmap);
    void setMessageText(const QString& text);

private:
    Ui::asynchronousMessageClass ui;


public slots:


};


#endif // ASYNCHRONOUSMESSAGE_H
