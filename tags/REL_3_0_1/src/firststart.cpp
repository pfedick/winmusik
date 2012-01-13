/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: firststart.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
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

#include "../include/firststart.h"


FirstStart::FirstStart(QWidget *parent, CWmClient *wm)
    : QDialog(parent)
{
	ui.setupUi(this);
	page=0;
	ui.stackedWidget->setCurrentIndex(0);
	ui.buttonAbbrechen->setEnabled(true);
	ui.buttonWeiter->setEnabled(true);
	ui.buttonZurueck->setEnabled(false);
	for (int i=0;i<10;i++) pageplan[i]=-1;
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
	ppl6::CArray charsets;
	ppl6::CIconv::Enumerate(charsets);

	QStringList qlist;
	charsets.Reset();
	const char *name;
	while ((name=charsets.GetNext())) {
		qlist.append(name);

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
	if (pageplan[page]==1) {
		// Es muss geprüft werden, ob der User einen gültigen Pfad einer früheren Installation angegeben hat
		ppl6::CString Path=ui.recoverpath->text();
		if (Path.Len()) {
			if (UseExistingInstallation(&Path)) {
				pageplan[2]=7;
				maxpageplan=2;
				wm->conf.DataPath=Path;
				if (!wm->conf.Save()) {
					ppl6::CString ee;
					ee=ppl6::Error();
					QString e=tr("Konfiguration konnte nicht gespeichert werden!\n");
					QString e2=ee;
					e+=e2;
					QMessageBox::critical(this, tr("WinMusik - Fehler!"),e);
					return;
				}
			} else {
				return;
			}
		} else {
			pageplan[2]=2;
			maxpageplan=2;
		}

	} else if (pageplan[page]==2) {
		ppl6::CString Path=ui.localdatapath->text();
		Path.RTrim("/");
		if (!Path.Len()) {			// Pfad darf nicht leer sein
			ui.buttonWeiter->setEnabled(false);
			return;
		} else {
			ui.buttonWeiter->setEnabled(true);
		}
		// Den Pageplan schonmal erweitern
		pageplan[3]=3;
		maxpageplan=3;

		on_localdatapath_textChanged();
		if (!ppl6::IsDir((const char*)Path)) {		// Pfad existiert nicht, sollen wir ihn anlegen?
			int ret=QMessageBox::question(this, tr("WinMusik"),
				tr("Das angegebene Verzeichnis existiert nicht. Soll es angelegt werden?"),
				QMessageBox::Yes | QMessageBox::No| QMessageBox::Abort);
			if (ret==QMessageBox::Yes) {
				if (!ppl6::MkDir(Path,1)) {
					QMessageBox::critical(this, tr("WinMusik - Fehler!"),
							tr("Das gewünschte Verzeichnis konnte nicht angelegt werden. Entweder ist der Pfad ungültig oder Sie haben keine Zugriffsrechte"));
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
				int ret=QMessageBox::critical(this, tr("WinMusik - Fehler!"),
						tr("Der angegebene Pfad enthält bereits Daten von WinMusik 3. Möchten Sie diese verwenden?"),
						QMessageBox::Yes | QMessageBox::No| QMessageBox::Abort);
				if (ret==QMessageBox::Yes) {
					// TODO: Der Datenpfad muss gespeichert werden
					pageplan[3]=7;
					maxpageplan=3;
				} else {
					return;
				}
			}
			//QMessageBox::information(NULL, "Debug","keine frühere Installation");
		}
	} else 	if (pageplan[page]==3) {
		pageplan[4]=4;
		maxpageplan=4;
		// Daten von WinMusik 2.x importieren?
		ppl6::CString Path=ui.importpath->text();
		if (Path.Len()) {
			// TODO: Prüfen, ob es sich um ein gültiges WM2-Datenverzeichnis handelt
			pageplan[4]=5;
			pageplan[5]=6;
			maxpageplan=5;
		}
	} else 	if (pageplan[page]==4) {
		wm->conf.DataPath=ui.localdatapath->text();
		//QMessageBox::information(this, tr("WinMusik Locale"),conf->Locale);
		if (!wm->conf.Save()) {
			ppl6::CString ee;
			ee=ppl6::Error();
			QString e=tr("Konfiguration konnte nicht gespeichert werden!\n");
			QString e2=ee;
			e+=e2;
			QMessageBox::critical(this, tr("WinMusik - Fehler!"),e);
			return;
		} else {
			wm->InitDataPath();
			wm->CreateInitialDatabase();
			done(1);
			return;
		}
	} else 	if (pageplan[page]==5) {
		wm->conf.DataPath=ui.localdatapath->text();
		if (!wm->conf.Save()) {
			ppl6::CString ee;
			ee=ppl6::Error();
			QString e=tr("Konfiguration konnte nicht gespeichert werden!\n");
			QString e2=ee;
			e+=e2;
			QMessageBox::critical(this, tr("WinMusik - Fehler!"),e);
			return;
		} else {
			wm->InitDataPath();
		}
	}

	page++;
	if (page>maxpageplan) {
		done(1);
		return;
	}
	ui.stackedWidget->setCurrentIndex(pageplan[page]);
	ui.buttonZurueck->setEnabled(true);
	if (pageplan[page]==2) {
		ppl6::CString Path=ui.localdatapath->text();
		Path.RTrim("/");
		if (!Path.Len()) {			// Pfad darf nicht leer sein
			ui.buttonWeiter->setEnabled(false);
			return;
		} else {
			ui.buttonWeiter->setEnabled(true);
		}

	} else if (pageplan[page]==4) {
		ui.buttonWeiter->setText(tr("Fertig"));
	} else if (pageplan[page]==6) {
		DoImport();
	} else if (pageplan[page]==7) {
		ui.buttonZurueck->setVisible(false);
		ui.buttonAbbrechen->setVisible(false);
		ui.buttonWeiter->setEnabled(true);
		ui.buttonWeiter->setText(tr("fertig"));
	} else if (pageplan[page]==8) {
		ui.buttonWeiter->setText(tr("fertig"));
		ui.buttonWeiter->setEnabled(true);
	} else if (pageplan[page]==9) {
		done(1);
	}
}

void FirstStart::on_buttonZurueck_clicked()
{
	if (pageplan[page]==3) {
		ui.buttonWeiter->setText(tr("weiter"));
	}

	if (pageplan[page]==2) ui.buttonWeiter->setEnabled(true);
	if (page>0)	page--;
	if (page<=0) {
		page=0;
		ui.buttonZurueck->setEnabled(false);
	}
	ui.stackedWidget->setCurrentIndex(pageplan[page]);

}

void FirstStart::on_buttonAbbrechen_clicked()
{
	int ret = QMessageBox::question(this, tr("WinMusik 3"),
	                                tr("Möchten Sie die Installation wirklich abbrechen?"),
	                                QMessageBox::Yes | QMessageBox::No,
	                                QMessageBox::No);
	ppl6::SetError(0);
	if (ret==QMessageBox::Yes) done(0);
}

void FirstStart::on_selectrecoverpath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Bitte das Verzeichnis mit einer früheren WinMusik-Installation auswählen"),
			ui.recoverpath->text(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		ui.recoverpath->setText(dir);
	}
}

void FirstStart::on_selectimportpath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Bitte das Datenverzeichnis von WinMusik 2.x auswählen"),
			ui.importpath->text(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		ui.importpath->setText(dir);
	}
}



void FirstStart::on_selectdatapath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Bitte das Verzeichnis auswählen, in dem WinMusik seine Daten speichern soll"),
			ui.localdatapath->text(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
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



int FirstStart::UseExistingInstallation(ppl6::CString *Path)
{
	char *p=(char*)Path->GetPtr();
	// Der Pfad muss existieren
	if (!ppl6::IsDir(p)) {
		//StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton )
		QMessageBox::critical(this, tr("WinMusik"),
			tr("Der angegebene Pfad ist ungültig oder existiert nicht"));
		return 0;
	}
	if (!wm->isValidDataPath(*Path)) {
		QMessageBox::critical(this, tr("WinMusik"),
			tr("Der angegebene Pfad enthält keine gültigen WinMusik-Daten"));
		return 0;
	}
	return 1;
}

void FirstStart::DoImport()
{
	ui.buttonWeiter->setEnabled(false);
	ui.buttonZurueck->setEnabled(false);
	ui.buttonAbbrechen->setEnabled(false);
	ui.progressBar->setMaximum(100);
	ui.progressBar->setValue(0);

	FirstStartImportProgress *p=new FirstStartImportProgress();
	p->widget=this;
	UpdateProgress(p);

	wm20::Cwm20Import Import;
	ppl6::CString wm2path, charset;
	charset=ui.charset->currentText();
	Import.SetCharset(charset);
	wm2path=ui.importpath->text();
	wm2path.Trim();

	wm->Storage.DeleteDatabase();
	if (!Import.Load(wm2path,&wm->Storage,p)) {
		wm->RaiseError(this,tr("Import failed"));
		ui.buttonWeiter->setEnabled(false);
		ui.buttonZurueck->setEnabled(true);
		ui.buttonAbbrechen->setEnabled(true);
		delete p;
		return;
	}
	ui.buttonWeiter->setEnabled(true);
	ui.buttonWeiter->setText(tr("fertig"));
	pageplan[0]=8;
	pageplan[1]=9;
	maxpageplan=1;
	page=0;
	ui.stackedWidget->setCurrentIndex(pageplan[page]);
	delete p;
}

void FirstStart::UpdateProgress(FirstStartImportProgress *p)
{
	ui.import_titeldatei->setVisible(p->import_titeldatei);
	ui.import_traegertitel->setVisible(p->import_traegertitel);
	ui.import_versionen->setVisible(p->import_versionen);
	ui.import_musikarten->setVisible(p->import_musikarten);
	ui.import_quellen->setVisible(p->import_quellen);
	ui.import_kaufquellen->setVisible(p->import_kaufquellen);
	ui.import_label->setVisible(p->import_label);
	ui.import_geraete->setVisible(p->import_geraete);
	ui.import_kassetten->setVisible(p->import_kassetten);
	ui.import_cds->setVisible(p->import_cds);
	ui.import_platten->setVisible(p->import_platten);
	ui.import_mp3->setVisible(p->import_mp3);
	ui.import_daten->setVisible(p->import_daten);
	ui.import_video->setVisible(p->import_video);
	ui.import_band->setVisible(p->import_band);
	ui.import_sonstige->setVisible(p->import_sonstige);
	ui.progressBar->setValue(p->progress);
	ui.currentProgress->setValue(p->current);
	//update();
	qApp->processEvents();

}


void FirstStartImportProgress::Update()
{
	widget->UpdateProgress(this);
}



