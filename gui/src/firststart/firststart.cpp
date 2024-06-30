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
#include "firststart.h"


FirstStart::FirstStart(QWidget* parent, CWmClient* wm)
	: QDialog(parent)
{
	ui.setupUi(this);
	page=0;
	ui.stackedWidget->setCurrentIndex(0);
	ui.buttonAbbrechen->setEnabled(true);
	ui.buttonWeiter->setEnabled(true);
	ui.buttonZurueck->setEnabled(false);
	for (int i=0;i < 10;i++) pageplan[i]=-1;
	pageplan[0]=0;
	pageplan[1]=1;
	pageplan[2]=2;
	maxpageplan=2;
	this->wm=wm;
	if (wm) {
		ui.localdatapath->setText(wm->conf.DataPath);
		if (wm->isValidDataPath(wm->conf.DataPath)) {
			ui.recoverpath->setText(wm->conf.DataPath);
		}
	}

}

FirstStart::~FirstStart()
{

}

bool FirstStart::UseExistingInstallation(const ppl7::String& Path)
{
	// Der Pfad muss existieren
	if (!ppl7::File::isDir(Path)) {
		//StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton )
		QMessageBox::critical(this, tr("WinMusik"),
			tr("The selected path is invalid or does not exist"));
		return false;
	}
	if (!wm->isValidDataPath(Path)) {
		QMessageBox::critical(this, tr("WinMusik"),
			tr("The selected path does not contain any valid WinMusik database"));
		return false;
	}
	return true;
}

void FirstStart::on_buttonWeiter_clicked()
{
	if (pageplan[page] == 1) {
		// Es muss geprüft werden, ob der User einen gültigen Pfad einer früheren Installation angegeben hat
		ppl7::String Path=ui.recoverpath->text();
		if (Path.size()) {
			if (UseExistingInstallation(Path)) {
				pageplan[2]=4;
				maxpageplan=2;
				wm->conf.DataPath=Path;
			} else {
				return;
			}
		} else {
			pageplan[2]=2;
			maxpageplan=2;
		}

	} else if (pageplan[page] == 2) {
		ppl7::String Path=ui.localdatapath->text();
		Path.trimRight("/");
		if (!Path.size()) {			// Pfad darf nicht leer sein
			ui.buttonWeiter->setEnabled(false);
			return;
		} else {
			ui.buttonWeiter->setEnabled(true);
		}
		// Den Pageplan schonmal erweitern
		pageplan[3]=4;
		maxpageplan=3;

		on_localdatapath_textChanged();
		if (!ppl7::File::isDir((const char*)Path)) {		// Pfad existiert nicht, sollen wir ihn anlegen?
			int ret=QMessageBox::question(this, tr("WinMusik"),
				tr("The specified directory does not exist. Should WinMusik create it?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
			if (ret == QMessageBox::Yes) {
				try {
					ppl7::Dir::mkDir(Path, true);
				} catch (const ppl7::Exception& exp) {
					ShowException(exp,
						tr("Could not create the requested directory. "
							"Please check, if the name of the directory is "
							"correct and that you have the rights to create it"));
					return;
				}
			}

		} else {
			// Pfad darf keine frühere Installation enthalten
			//QMessageBox::information(NULL, "Debug","Prüfe, ob Pfad frühere Installation enthält");
			if (wm->isValidDataPath(Path)) {
				//QMessageBox::information(NULL, "Debug","frühere Installation gefunden");
				int ret=QMessageBox::critical(this, tr("WinMusik - Error!"),
					tr("The selected path already contains a WinMusik 3 database. Do you want to use it?"),
					QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
				if (ret == QMessageBox::Yes) {
					// TODO: Der Datenpfad muss gespeichert werden
					wm->conf.DataPath=Path;
					pageplan[3]=4;
					maxpageplan=3;
				} else {
					return;
				}
			}
			//QMessageBox::information(NULL, "Debug","keine frühere Installation");
		}
	} else if (pageplan[page] == 3) {
		wm->conf.DataPath=ui.localdatapath->text();
		//QMessageBox::information(this, tr("WinMusik Locale"),conf->Locale);
		try {
			wm->conf.save();
		} catch (const ppl7::Exception& exp) {
			ShowException(exp, tr("Configuration could not be saved!"));
			return;
		}
		wm->InitDataPath();
		wm->CreateInitialDatabase();
		done(1);
		return;
	} else 	if (pageplan[page] == 4) {
		try {
			wm->conf.save();
		} catch (const ppl7::Exception& exp) {
			ShowException(exp, tr("Configuration could not be saved!"));
			return;
		}
		wm->InitDataPath();
		done(1);
		return;

	}

	page++;
	if (page > maxpageplan) {
		printf("Sollten wir hier landen?\n");
		wm->InitDataPath();
		done(1);
		return;
	}
	ui.stackedWidget->setCurrentIndex(pageplan[page]);
	ui.buttonZurueck->setEnabled(true);
	if (pageplan[page] == 2) {
		ppl7::String Path=ui.localdatapath->text();
		Path.trimRight("/");
		if (!Path.len()) {			// Pfad darf nicht leer sein
			ui.buttonWeiter->setEnabled(false);
			return;
		} else {
			ui.buttonWeiter->setEnabled(true);
		}

	} else if (pageplan[page] == 3 || pageplan[page] == 4) {
		ui.buttonWeiter->setText(tr("finish"));
	} else if (pageplan[page] >= 5) {
		done(1);
	} else {
		ui.buttonWeiter->setText(tr("next"));
	}
}

void FirstStart::on_buttonZurueck_clicked()
{
	ui.buttonWeiter->setText(tr("next"));
	if (pageplan[page] == 2) ui.buttonWeiter->setEnabled(true);
	if (page > 0)	page--;
	if (page <= 0) {
		page=0;
		ui.buttonZurueck->setEnabled(false);
	}
	ui.stackedWidget->setCurrentIndex(pageplan[page]);

}

void FirstStart::on_buttonAbbrechen_clicked()
{
	int ret = QMessageBox::question(this, tr("WinMusik 3"),
		tr("Do you really want to cancel the configuration?"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No);
	if (ret == QMessageBox::Yes) done(0);
}

void FirstStart::on_selectrecoverpath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Please select the directory of a former WinMusik installation"),
		ui.recoverpath->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.length()) {
		ui.recoverpath->setText(dir);
	}
}

void FirstStart::on_selectdatapath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Please select the directory in which WinMusik should save it's database"),
		ui.localdatapath->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.length()) {
		ui.localdatapath->setText(dir);
	}
}

void FirstStart::on_localdatapath_textChanged()
{
	ppl7::String Path=ui.localdatapath->text();
	Path.trimRight("/");
	//ui.localdatapath->setText(Path);
	if (Path.size()) {
		ui.buttonWeiter->setEnabled(true);
		pageplan[3]=3;
		maxpageplan=3;
	} else {
		ui.buttonWeiter->setEnabled(false);
	}
}
