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

#ifndef FKEYS_H
#define FKEYS_H

#include <QWidget>
#include "ui_fkeys.h"

class Fkeys : public QWidget
{
    Q_OBJECT

public:
    Fkeys(QWidget* parent = 0);
    ~Fkeys();

    void setFkey(int num, const QString& Icon, const QString& Text, bool enabled=true);
    void setFkey(int num, const QIcon& Icon, const QString& Text, bool enabled=true);
    void setEnabled(int num, bool flag);
    bool isEnabled(int num) const;

private:
    Ui::FkeysClass ui;
    QToolButton* button[13];


public slots:
    void on_esc_clicked();
    void on_f1_clicked();
    void on_f2_clicked();
    void on_f3_clicked();
    void on_f4_clicked();
    void on_f5_clicked();
    void on_f6_clicked();
    void on_f7_clicked();
    void on_f8_clicked();
    void on_f9_clicked();
    void on_f10_clicked();
    void on_f11_clicked();
    void on_f12_clicked();

signals:
    void clicked(int keynum);

};

#endif // FKEYS_H
