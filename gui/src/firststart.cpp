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

	// Charset-Auswahl
	std::list<ppl7::String> charsets;
	ppl7::Iconv::enumerateCharsets(charsets);

	QStringList qlist;
	std::list<ppl7::String>::const_iterator it;
	for (it=charsets.begin();it != charsets.end();++it) {
		qlist.append((*it));
	}
	qlist.sort();
	ui.charset->addItems(qlist);
	ui.charset->setCurrentIndex(ui.charset->findText("ISO-8859-15"));
}

FirstStart::~FirstStart()
{

}



void FirstStart::on_buttonWeiter_clicked()
{
	if (pageplan[page] == 1) {
		// Es muss geprüft werden, ob der User einen gültigen Pfad einer früheren Installation angegeben hat
		ppl6::CString Path=ui.recoverpath->text();
		if (Path.Len()) {
			if (UseExistingInstallation(&Path)) {
				pageplan[2]=7;
				maxpageplan=2;
				wm->conf.DataPath=Path;
				try {
					wm->conf.save();
				} catch (const ppl7::Exception& exp) {
					ShowException(exp, tr("Configuration could not be saved!"));
					return;
				}
			} else {
				return;
			}
		} else {
			pageplan[2]=2;
			maxpageplan=2;
		}

	} else if (pageplan[page] == 2) {
		ppl6::CString Path=ui.localdatapath->text();
		Path.RTrim("/");
		if (!Path.Len()) {			// Pfad darf nicht leer sein
			ui.buttonWeiter->setEnabled(false);
			return;
		} else {
			ui.buttonWeiter->setEnabled(true);
		}
		// Den Pageplan schonmal erweitern
		pageplan[3]=5;
		maxpageplan=3;

		on_localdatapath_textChanged();
		if (!ppl6::IsDir((const char*)Path)) {		// Pfad existiert nicht, sollen wir ihn anlegen?
			int ret=QMessageBox::question(this, tr("WinMusik"),
				tr("The specified directory does not exist. Should WinMusik create it?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
			if (ret == QMessageBox::Yes) {
				if (!ppl6::MkDir(Path, 1)) {
					QMessageBox::critical(this, tr("WinMusik - Error!"),
						tr("Could not create the requested directory. Please check, if the name of the directory is correct and that you have the rights to create it"));
					return;
				}

			} else {
				return;
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
					pageplan[3]=7;
					maxpageplan=3;
				} else {
					return;
				}
			}
			//QMessageBox::information(NULL, "Debug","keine frühere Installation");
		}
	} else 	if (pageplan[page] == 4) {
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

	}

	page++;
	if (page > maxpageplan) {
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

	} else if (pageplan[page] == 4) {
		ui.buttonWeiter->setText(tr("finish"));
	} else if (pageplan[page] == 7) {
		ui.buttonZurueck->setVisible(false);
		ui.buttonAbbrechen->setVisible(false);
		ui.buttonWeiter->setEnabled(true);
		ui.buttonWeiter->setText(tr("finish"));
	} else if (pageplan[page] == 9) {
		done(1);
	}
}

void FirstStart::on_buttonZurueck_clicked()
{
	if (pageplan[page] == 3) {
		ui.buttonWeiter->setText(tr("next"));
	}

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
		tr("Do you really want to cancel the installation?"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No);
	ppl6::SetError(0);
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

void FirstStart::on_selectimportpath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Please select the directory of the WinMusik 2 database"),
		ui.importpath->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.length()) {
		ui.importpath->setText(dir);
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
	ppl6::CString Path=ui.localdatapath->text();
	Path.RTrim("/");
	//ui.localdatapath->setText(Path);
	if (Path.Len()) {
		ui.buttonWeiter->setEnabled(true);
		pageplan[3]=3;
		pageplan[4]=4;
		maxpageplan=4;
	} else {
		ui.buttonWeiter->setEnabled(false);
	}
}



int FirstStart::UseExistingInstallation(ppl6::CString* Path)
{
	char* p=(char*)Path->GetPtr();
	// Der Pfad muss existieren
	if (!ppl6::IsDir(p)) {
		//StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton )
		QMessageBox::critical(this, tr("WinMusik"),
			tr("The selected path is invalid or does not exist"));
		return 0;
	}
	if (!wm->isValidDataPath(*Path)) {
		QMessageBox::critical(this, tr("WinMusik"),
			tr("The selected path does not contain any valid WinMusik database"));
		return 0;
	}
	return 1;
}
