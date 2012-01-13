/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:30 $
 * $Id: properties.cpp,v 1.1 2010/05/16 12:23:30 pafe Exp $
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

#include "winmusik3.h"
#include "updater.h"

#include "../include/properties.h"
#include "../include/registration.h"

Properties::Properties(QWidget *parent, CWmClient *wm)
    : QDialog(parent)
{
	ui.setupUi(this);
	ppl6::CString Tmp;

	this->wm=wm;

	Config *c=&wm->conf;

	// General Options
	ui.wmDataPath->setText(c->DataPath);
	ui.tmpPath->setText(c->TmpPath);
	ui.showSplashscreen->setChecked(c->bShowSplashScreen);
	ui.checkForUpdates->setChecked(c->bCheckForUpdatesOnStartup);
	ui.suggestArtist->setChecked(c->bAutomaticArtistSuggestion);
	ui.suggestTitle->setChecked(c->bAutomaticTitleSuggestion);
	ui.printColors->setChecked(c->bPrintColors);
	//ui.font->setText(c->PrinterFont);

	// MP3
	ui.mp3Path->setText(c->MP3Path);
	ui.mp3Player->setText(c->MP3Player);
	if (c->ReadId3Tag==1) ui.readID3v1->setChecked(true);
	if (c->ReadId3Tag==2) ui.readID3v2->setChecked(true);
	ui.writeID3v1->setChecked(c->bWriteId3v1);
	ui.writeID3v2->setChecked(c->bWriteId3v2);
	ui.removeOriginalTags->setChecked(c->bRemoveOriginalId3Tags);
	Tmp.Setf("%i",c->ID3v2Padding);
	ui.ID3v2Padding->setText(Tmp);
	ui.saveOriginalTags->setChecked(c->bSaveOriginalMp3Tags);
	ui.saveOriginalTagsOnAutoImport->setChecked(c->bSaveOriginalMp3TagsOnAutoImport);
	ui.normalizeFilenames->setChecked(c->bNormalizeFilename);
	ui.retryIntervall->setValue(c->TagSaverRetryIntervall);
	if (c->MaxFilenameLength<=64) ui.maxFilenameLength->setCurrentIndex(0);
	else if (c->MaxFilenameLength<=100) ui.maxFilenameLength->setCurrentIndex(1);
	else if (c->MaxFilenameLength<=207) ui.maxFilenameLength->setCurrentIndex(2);
	else ui.maxFilenameLength->setCurrentIndex(3);


	// User
	ui.name->setText(c->UserName);
	ui.company->setText(c->UserCompany);
	ui.serial->setText(c->Serial);
	ui.currency->setText(c->Currency);

	int index=ui.font->findText(c->PrinterFont);
	if (index>=0) ui.font->setCurrentIndex(index);
	SelectLanguage();
	ui.tabWidget->setCurrentIndex(0);

	// Debug
	ui.enableDebug->setChecked(wm->conf.bEnableDebug);
	ui.logfile->setText(c->Logfile);
	ui.debuglevel->setValue(c->Debuglevel);
	ui.generations->setValue(c->LogfileGenerations);
	Tmp.Setf("%i",c->LogfileSize);
	ui.logfileSize->setText(Tmp);

	// Devices
	ui.device_1->setChecked(wm->conf.VisibleDevice[1]);
	ui.device_2->setChecked(wm->conf.VisibleDevice[2]);
	ui.device_3->setChecked(wm->conf.VisibleDevice[3]);
	ui.device_4->setChecked(wm->conf.VisibleDevice[4]);
	ui.device_5->setChecked(wm->conf.VisibleDevice[5]);
	ui.device_6->setChecked(wm->conf.VisibleDevice[6]);
	ui.device_7->setChecked(wm->conf.VisibleDevice[7]);
	ui.device_8->setChecked(wm->conf.VisibleDevice[8]);
	ui.device_9->setChecked(wm->conf.VisibleDevice[9]);

	// Directory search
	ui.cacheDirectories->setChecked(c->bCacheDirectorySearch);
	c->DirectorySearch.Reset();
	const char* row;
	while ((row=c->DirectorySearch.GetNext())) {
		ui.directoryList->addItem(row);
	}

	ui.acceptButton->setEnabled(false);
}

Properties::~Properties()
{

}

void Properties::Change()
{
	ui.acceptButton->setEnabled(true);
}

void Properties::SelectLanguage()
{
	Config *c=&wm->conf;
	if (c->Locale=="de") ui.language->setCurrentIndex(0);
	else ui.language->setCurrentIndex(1);

}

ppl6::CString Properties::GetLocale()
{
	ppl6::CString Tmp;

	if (ui.language->currentIndex()==0) Tmp="de";
	else Tmp="en";
	return Tmp;
}

int Properties::CheckValues()
{
	ppl6::CString Tmp;
	/*
	Tmp=GetLocale();
	if (Tmp!=wm->conf.Locale) {
		QMessageBox::information(NULL, tr("WinMusik"),tr("You have changed the language.\nYou need to restart the Application to enable the new language."));
	}
	*/

	Tmp=ui.wmDataPath->text();
	if (Tmp!=wm->conf.DataPath) {
		int b;
		b=QMessageBox::question(NULL, tr("WinMusik - Change database directory"),
				tr("Are you sure you want to change the directory of the database? "
				"This could result in loss of data!"),
				QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
				QMessageBox::Cancel);
		if (b!=QMessageBox::Yes) ui.wmDataPath->setText(wm->conf.DataPath);
	}
	return 1;
}

int Properties::Save()
{
	ppl6::CString Tmp;

	if (!wm) {
		// This should never happen
		QMessageBox::critical(this, tr("WinMusik - Internal error!"),
				tr("An internal data pointer is NULL, this should never happen and is a Bug. Please write a bugreport."));
		return 0;
	}
	Config *c=&wm->conf;

	Tmp=ui.wmDataPath->text();

	// DataPath has changed, we need to close the database and all currently opened
	// Windows before
	if (Tmp!=wm->conf.DataPath) wm->CloseDatabase();

	// General Options
	c->DataPath=ui.wmDataPath->text();
	c->TmpPath=ui.tmpPath->text();
	c->bShowSplashScreen=ui.showSplashscreen->isChecked();
	c->bCheckForUpdatesOnStartup=ui.checkForUpdates->isChecked();
	c->bAutomaticArtistSuggestion=ui.suggestArtist->isChecked();
	c->bAutomaticTitleSuggestion=ui.suggestTitle->isChecked();
	c->bPrintColors=ui.printColors->isChecked();
	c->PrinterFont=ui.font->currentText();
	c->bCacheDirectorySearch=ui.cacheDirectories->isChecked();

	// MP3
	c->MP3Path=ui.mp3Path->text();
	c->MP3Player=ui.mp3Player->text();
	if (ui.readID3v1->isChecked()) c->ReadId3Tag=1;
	else c->ReadId3Tag=2;
	c->bWriteId3v1=ui.writeID3v1->isChecked();
	c->bWriteId3v2=ui.writeID3v2->isChecked();
	c->bRemoveOriginalId3Tags=ui.removeOriginalTags->isChecked();
	c->ID3v2Padding=ui.ID3v2Padding->text().toInt();
	c->bSaveOriginalMp3Tags=ui.saveOriginalTags->isChecked();
	c->bSaveOriginalMp3TagsOnAutoImport=ui.saveOriginalTagsOnAutoImport->isChecked();
	c->bNormalizeFilename=ui.normalizeFilenames->isChecked();
	c->TagSaverRetryIntervall=ui.retryIntervall->value();
	if (ui.maxFilenameLength->currentIndex()==0) c->MaxFilenameLength=64;
	else if (ui.maxFilenameLength->currentIndex()==1) c->MaxFilenameLength=100;
	else if (ui.maxFilenameLength->currentIndex()==1) c->MaxFilenameLength=207;
	else c->MaxFilenameLength=1024;

	// User
	Tmp=c->Locale;
	c->Locale=GetLocale();
	if (c->Locale!=Tmp) {
		wm->ReloadTranslation();
		ui.retranslateUi(this);
		SelectLanguage();
	}
	c->UserName=ui.name->text();
	c->UserCompany=ui.company->text();
	c->Serial=ui.serial->text();
	c->Currency=ui.currency->text();

	// Debug
	c->bEnableDebug=ui.enableDebug->isChecked();
	c->Logfile=ui.logfile->text();
	c->LogfileSize=ui.logfileSize->text().toInt();
	c->LogfileGenerations=ui.generations->value();
	c->Debuglevel=ui.debuglevel->value();

	// Devices
	wm->conf.VisibleDevice[1]=ui.device_1->isChecked();
	wm->conf.VisibleDevice[2]=ui.device_2->isChecked();
	wm->conf.VisibleDevice[3]=ui.device_3->isChecked();
	wm->conf.VisibleDevice[4]=ui.device_4->isChecked();
	wm->conf.VisibleDevice[5]=ui.device_5->isChecked();
	wm->conf.VisibleDevice[6]=ui.device_6->isChecked();
	wm->conf.VisibleDevice[7]=ui.device_7->isChecked();
	wm->conf.VisibleDevice[8]=ui.device_8->isChecked();
	wm->conf.VisibleDevice[9]=ui.device_9->isChecked();

	c->DirectorySearch.Clear();
	for (int i=0;i<ui.directoryList->count();i++) {
		QListWidgetItem * item=ui.directoryList->item(i);
		Tmp=item->text();
		c->DirectorySearch.Add(Tmp);
	}


	if (!wm->conf.Save()) {
		return 0;
	}
	wm->InitLogging();
	wm->UpdateMenue();
	return 1;
}

/*
 *	Buttons
 */

void Properties::on_cancelButton_clicked()
{
	done(0);
}

void Properties::on_okButton_clicked()
{
	if (!CheckValues()) return;
	if (!Save()) return;
	done(0);
}

void Properties::on_acceptButton_clicked()
{
	if (!CheckValues()) return;
	if (!Save()) return;
	ui.acceptButton->setEnabled(false);
}

void Properties::on_wmDataPathButton_clicked()
{
	QString p=ui.wmDataPath->text();
	if (p.length()==0) {

	}
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory for WinMusik database.\nAttention: Changing this directory can result in loss of data!"),
		p,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		ui.wmDataPath->setText(dir);
		Change();
	}
}


void Properties::on_mp3PathButton_clicked()
{
	QString p=ui.mp3Path->text();
	if (p.length()==0) {
		p=QDir::homePath();
	}
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory with your MP3 files"),
		p,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		ui.mp3Path->setText(dir);
		Change();
	}
}

void Properties::on_mp3PlayerButton_clicked()
{
	QString p=ui.mp3Player->text();
	if (p.length()==0) {
		p=QDir::homePath();
	}
	QString file = QFileDialog::getOpenFileName(this, tr("Select the program with which you want to use for playing MP3-Files"),
	                                                 p);
	if(file.length()) {
		ui.mp3Player->setText(file);
		Change();
	}
}


void Properties::on_tmpPathButton_clicked()
{
	QString p=ui.tmpPath->text();
	if (p.length()==0) {
		p=QDir::tempPath();
	}
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory for temporary files"),
		p,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		ui.tmpPath->setText(dir);
		Change();
	}
}

void Properties::on_logfileButton_clicked()
{
	QString p=ui.logfile->text();
	if (p.length()==0) {
		p=QDir::homePath();
	}
	QString dir = QFileDialog::getSaveFileName(this, tr("Select location and filename for debug logfile"),
		p,"*.log");
	if(dir.length()) {
		ui.logfile->setText(dir);
		Change();
	}
}

void Properties::on_registerButton_clicked()
{
	Registration reg(this,wm);
	reg.exec();
	ui.serial->setText(wm->conf.Serial);
}

void Properties::on_checkUpdatesNow_clicked()
{
	CUpdateChecker uc(this,wm);
	uc.CheckNow(this);
}

/*
 * Changes
 */

void Properties::on_wmDataPath_textChanged(const QString &text)
{
	Change();
}

void Properties::on_tmpPath_textChanged(const QString &text)
{
	Change();
}

void Properties::on_addDirectory_clicked()
{
	QString p;
	if (LastHDSearchDir.isEmpty()) LastHDSearchDir=QDir::currentPath();
	p=LastHDSearchDir;
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select the directory you want to add to the harddisk search"),
		p,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		LastHDSearchDir=dir;
		// Sofern das Verzeichnis noch nicht in der Liste ist. fügen wir es hinzu
		QList<QListWidgetItem *> list;
		list=ui.directoryList->findItems (dir, Qt::MatchExactly);
		if (list.count()==0) {
			ui.directoryList->addItem(dir);
			Change();
		}
	}
}

void Properties::on_deleteDirectory_clicked()
{
	ui.directoryList->takeItem(ui.directoryList->currentRow());
	Change();
}
