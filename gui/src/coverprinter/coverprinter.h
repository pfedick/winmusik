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

#ifndef COVERPRINTER_H
#define COVERPRINTER_H

#define WITH_QT
#include "winmusik3.h"
#include <QDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPen>
#include <QMessageBox>
#include <QCheckBox>
#include <QDragEnterEvent>
#include <QPixmap>
#include <QFileDialog>
#include <QIcon>
#include <QSettings>
#include "ui_coverprinter.h"



class CoverPrinter : public QDialog
{
    Q_OBJECT

public:
    CoverPrinter(QWidget *parent = 0, CWmClient *wm=NULL);
    ~CoverPrinter();

    void ReloadTranslation();

private:
	QPrinter *Printer;
    Ui::CoverPrinterClass ui;
    QPixmap pix;
    CWmClient *wm;

private slots:
	void on_printOptions_clicked();
	void on_loadcover_clicked();
	void on_unbekannt1_stateChanged(int state);
	void on_unbekannt2_stateChanged(int state);
	void on_type_currentIndexChanged(int i);
	//void on_front_cover_dragEnterEvent(QDragEnterEvent *event);
	//void on_front_cover_dropEvent(QDropEvent *event);
	//void on_cover_dragEnterEvent(QDragEnterEvent *event);
	//void on_cover_dropEvent(QDropEvent *event);

};

#endif // COVERPRINTER_H
