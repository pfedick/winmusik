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


#include "winmusik3.h"
#include <QMenu>
#include "shortcutdialog.h"




ShortcutDialog::ShortcutDialog(QWidget *parent, CWmClient *wm)
: QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;

	wm->ShortcutStore.Reset();
	DataShortcut *sc;
	ui.treeWidget->clear();
	ui.treeWidget->setSortingEnabled(false);
	QTreeWidgetItem *item;
	while ((sc=wm->ShortcutStore.GetNext())) {
		item = new QTreeWidgetItem;
		item->setText(0,sc->GetShortcut());
		item->setText(1,sc->GetArtist());
		ui.treeWidget->addTopLevelItem(item);
	}
	ui.treeWidget->setSortingEnabled(true);
	ui.treeWidget->sortByColumn(0,Qt::AscendingOrder);
	currentTreeItem=NULL;

}

void ShortcutDialog::resizeEvent ( QResizeEvent * event )
/*!\brief Größenänderung des Fensters
 *
 * Diese Funktion wird durch Qt aufgerufen, wenn sich die Größe
 * des Fensters ändert, sowie von Edit::UpdateTrackListing, wenn sich der
 * Inhalt der Trackliste ändert.
 */
{
	int w=ui.treeWidget->width();
	ui.treeWidget->setColumnWidth(0,w*20/100);
	ui.treeWidget->setColumnWidth(1,w*80/100);
	QWidget::resizeEvent(event);
}

ShortcutDialog::~ShortcutDialog()
{

}

const char *ShortcutDialog::GetArtist()
{
	return SelectedArtist;
}

void ShortcutDialog::on_treeWidget_customContextMenuRequested ( const QPoint & pos )
/*!\brief Kontext-Menue der Liste
 *
 * Diese Funktion wird aufgerufen, wenn der Anwender mit der rechten Maustaste
 * in die Abkürzungsliste klickt. Sie generiert ein Context-abhängiges Popup-Menu
 * an der Position des Klicks.
 *
 * \param[in] pos Mausposition des Klicks
 */
{
    QPoint p=mapToGlobal(pos);
    currentTreeItem=ui.treeWidget->itemAt(pos);
    if (!currentTreeItem) return;
    //printf ("Custom Context %i\n",currentTrackListItem->Track);

    QMenu *m=new QMenu(this);
    m->setTitle("Ein Titel");
    m->addAction (QIcon(":/icons/resources/button_ok.png"),tr("Use this entry","Context Menue"),this,SLOT(on_contextUse_triggered()));
    m->addSeparator();
    m->addAction (QIcon(":/icons/resources/edit.png"),tr("Edit this entry","Context Menue"),this,SLOT(on_contextEdit_triggered()));
    m->addAction (QIcon(":/icons/resources/filenew.png"),tr("New entry","Context Menue"),this,SLOT(on_contextNew_triggered()));
    m->addAction (QIcon(":/icons/resources/trash-16.png"),tr("Delete entry","Context Menue"),this,SLOT(on_contextDelete_triggered()));
    m->popup(p);
}


void ShortcutDialog::on_contextUse_triggered()
{
	if (currentTreeItem) {
		SelectedArtist=currentTreeItem->text(1);
		done(1);
	}
}

void ShortcutDialog::on_contextEdit_triggered()
{
	if (currentTreeItem) {
		ui.shortcutEdit->setText(currentTreeItem->text(0));
		ui.artistEdit->setText(currentTreeItem->text(1));
		ui.shortcutEdit->setFocus();
	}
}

void ShortcutDialog::on_contextNew_triggered()
{
	ui.shortcutEdit->setText("");
	ui.artistEdit->setText("");
	ui.shortcutEdit->setFocus();
}

void ShortcutDialog::on_contextDelete_triggered()
{
	if (currentTreeItem) {
		ppl6::CString ShortCut=currentTreeItem->text(0);
		if (wm->ShortcutStore.Delete(ShortCut)) {
			delete currentTreeItem;
			currentTreeItem=NULL;
		} else {
			wm->RaiseError(this,tr("Could not delete shortcut"));
		}
	}
}

void ShortcutDialog::on_treeWidget_itemDoubleClicked ( QTreeWidgetItem * item, int column )
{
	(void) column;
	if (item) {
		ui.shortcutEdit->setText(item->text(0));
		ui.artistEdit->setText(item->text(1));
		ui.shortcutEdit->setFocus();
	}
}

void ShortcutDialog::on_saveButton_clicked()
{
	DataShortcut ds;
	ppl6::CString ShortCut=ui.shortcutEdit->text();
	ShortCut.Trim();
	SelectedArtist=ui.artistEdit->text();
	SelectedArtist.Trim();
	if (ShortCut.Len()>0) {
		ds.SetValue(ShortCut,SelectedArtist);
		wm->ShortcutStore.Put(&ds);
		done(1);
	}
}

void ShortcutDialog::on_cancelButton_clicked()
{
	done(0);
}

void ShortcutDialog::SetShortcut(const char *name)
{
	DataShortcut *sc;
	ui.shortcutEdit->setText(name);
	sc=wm->ShortcutStore.Get(name);
	if (sc) {
		ui.artistEdit->setText(sc->GetArtist());
	}
}
