/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:29 $
 * $Id: shortcutdialog.h,v 1.1 2010/05/16 12:23:29 pafe Exp $
 *
 *
 * Copyright (c) 2009 Patrick Fedick
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


#ifndef SHORTCUTDIALOG_H_
#define SHORTCUTDIALOG_H_

#include "winmusik3.h"
#include <QtGui/QDialog>
#include "ui_shortcutdialog.h"

class ShortcutDialog : public QDialog
{
    Q_OBJECT

public:
	ShortcutDialog(QWidget *parent = 0, CWmClient *wm = NULL);
    ~ShortcutDialog();

    const char *GetArtist();
    void SetShortcut(const char *name);

private:
    Ui::ShortcutDialogClass ui;
    CWmClient *wm;
    void resizeEvent ( QResizeEvent * event );
    ppl6::CString	SelectedArtist;
    QTreeWidgetItem	*currentTreeItem;

public slots:

	void on_treeWidget_customContextMenuRequested ( const QPoint & pos );
	void on_contextUse_triggered();
	void on_contextEdit_triggered();
	void on_contextNew_triggered();
	void on_contextDelete_triggered();
	void on_treeWidget_itemDoubleClicked ( QTreeWidgetItem * item, int column );
	void on_saveButton_clicked();
	void on_cancelButton_clicked();
};






#endif /* SHORTCUTDIALOG_H_ */
