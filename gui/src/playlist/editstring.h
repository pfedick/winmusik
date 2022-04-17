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

#ifndef EDITSTRING_H
#define EDITSTRING_H

#include <QDialog>
#include <QString>

namespace Ui {
class EditString;
}

class EditString : public QDialog
{
    Q_OBJECT

public:
    explicit EditString(QWidget *parent = nullptr);
    ~EditString();
    void setTitle(const QString &title);
    void setString(const QString &str);
    QString getString() const;

private:
    Ui::EditString *ui;

public slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

};

#endif // EDITSTRING_H
