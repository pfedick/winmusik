/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/10/10 10:31:01 $
 * $Id: properties.cpp,v 1.5 2010/10/10 10:31:01 pafe Exp $
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

#include "properties.h"
#include "regexpedit.h"
#include <QTableWidgetItem>
#include "ppl6-sound.h"

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
	ui.suggestArtist->setChecked(c->bAutomaticArtistSuggestion);
	ui.suggestTitle->setChecked(c->bAutomaticTitleSuggestion);
	ui.automaticEditSearch->setChecked(c->bAutomaticEditSearch);
	ui.printColors->setChecked(c->bPrintColors);
	//ui.font->setText(c->PrinterFont);

	//Devices
	ui.devicePathCassette->setText(c->DevicePath[1]);
	ui.devicePathCD->setText(c->DevicePath[2]);
	ui.devicePathDataStorage->setText(c->DevicePath[3]);
	ui.devicePathRecord->setText(c->DevicePath[4]);
	ui.devicePathVHS->setText(c->DevicePath[5]);
	ui.devicePathOther->setText(c->DevicePath[6]);
	ui.devicePathMP3->setText(c->DevicePath[7]);
	ui.devicePathTape->setText(c->DevicePath[8]);
	ui.devicePathDVD->setText(c->DevicePath[9]);

	// MP3
	ui.mp3Player->setText(c->MP3Player);
	ui.aiffPlayer->setText(c->AIFFPlayer);
	if (c->ReadId3Tag==1) ui.readID3v1->setChecked(true);
	if (c->ReadId3Tag==2) ui.readID3v2->setChecked(true);
	ui.writeID3->setChecked(c->bWriteID3Tags);
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
	else if (c->MaxFilenameLength<=255) ui.maxFilenameLength->setCurrentIndex(3);
	else ui.maxFilenameLength->setCurrentIndex(4);

	ui.JpegQualityCover->setValue(c->JpegQualityCover);
	ui.JpegQualityPreview->setValue(c->JpegQualityPreview);


	// User
	ui.name->setText(c->UserName);
	ui.company->setText(c->UserCompany);
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

	// MusicKey
	switch (c->musicKeyDisplay) {
		case musicKeyTypeMusicalSharps: ui.displayMusicKey->setCurrentIndex(0); break;
		case musicKeyTypeOpenKey: ui.displayMusicKey->setCurrentIndex(1); break;
		case musicKeyTypeCustom: ui.displayMusicKey->setCurrentIndex(2); break;
		default: ui.displayMusicKey->setCurrentIndex(1); break;

	}

	switch (c->musicKeyTag) {
		case musicKeyTypeNone: ui.saveMusicKey->setCurrentIndex(0); break;
		case musicKeyTypeMusicalSharps: ui.saveMusicKey->setCurrentIndex(1); break;
		case musicKeyTypeOpenKey: ui.saveMusicKey->setCurrentIndex(2); break;
		case musicKeyTypeCustom: ui.saveMusicKey->setCurrentIndex(3); break;
		default: ui.saveMusicKey->setCurrentIndex(2); break;
	}
	ui.customMusicKeyName->setText(c->customMusicKeyName);
	ui.customMusicalKey_1->setText(c->customMusicKey[1]);
	ui.customMusicalKey_2->setText(c->customMusicKey[2]);
	ui.customMusicalKey_3->setText(c->customMusicKey[3]);
	ui.customMusicalKey_4->setText(c->customMusicKey[4]);
	ui.customMusicalKey_5->setText(c->customMusicKey[5]);
	ui.customMusicalKey_6->setText(c->customMusicKey[6]);
	ui.customMusicalKey_7->setText(c->customMusicKey[7]);
	ui.customMusicalKey_8->setText(c->customMusicKey[8]);
	ui.customMusicalKey_9->setText(c->customMusicKey[9]);
	ui.customMusicalKey_10->setText(c->customMusicKey[10]);
	ui.customMusicalKey_11->setText(c->customMusicKey[11]);
	ui.customMusicalKey_12->setText(c->customMusicKey[12]);
	ui.customMusicalKey_13->setText(c->customMusicKey[13]);
	ui.customMusicalKey_14->setText(c->customMusicKey[14]);
	ui.customMusicalKey_15->setText(c->customMusicKey[15]);
	ui.customMusicalKey_16->setText(c->customMusicKey[16]);
	ui.customMusicalKey_17->setText(c->customMusicKey[17]);
	ui.customMusicalKey_18->setText(c->customMusicKey[18]);
	ui.customMusicalKey_19->setText(c->customMusicKey[19]);
	ui.customMusicalKey_20->setText(c->customMusicKey[20]);
	ui.customMusicalKey_21->setText(c->customMusicKey[21]);
	ui.customMusicalKey_22->setText(c->customMusicKey[22]);
	ui.customMusicalKey_23->setText(c->customMusicKey[23]);
	ui.customMusicalKey_24->setText(c->customMusicKey[24]);
	ui.customMusicalKey_25->setText(c->customMusicKey[25]);

	// CDDB
	if (ppl6::AudioCD::isSupported()==true && ppl6::CDDB::isSupported()==true) {
		ui.cddbServer->setText(c->cddb.server);
		Tmp.Setf("%i",c->cddb.port);
		ui.cddbPort->setText(Tmp);
		ui.cdioDevice->clear();
		std::list<ppl6::CString> devices=ppl6::AudioCD::getDevices();
		ui.cdioDevice->addItem(tr("Default"),"default");
		std::list<ppl6::CString>::const_iterator it;
		for (it=devices.begin();it!=devices.end();++it) {
			ui.cdioDevice->addItem((*it),(*it));

		}
		index=ui.cdioDevice->findData(c->cddb.cddevice);
		if (index>=0) ui.cdioDevice->setCurrentIndex(index);

		ui.cddbQueryPath->setText(c->cddb.querypath);
		ui.cddbUserName->setText(c->cddb.username);
		ui.cddbHostName->setText(c->cddb.hostname);
		ui.cddbProxyServer->setText(c->cddb.proxy_server);
		Tmp.Setf("%i",c->cddb.proxy_port);
		ui.cddbProxyPort->setText(Tmp);
		ui.cddbUseProxy->setChecked(c->cddb.useProxy);
		ui.cddbProxyLayout_2->setEnabled(c->cddb.useProxy);

	} else {
		ui.tab_cddb->setEnabled(false);
	}


	// Finish
	ui.acceptButton->setEnabled(false);
	QString Style="QTreeView::item {\n"
	    		"border-right: 1px solid #b9b9b9;\n"
	    		"border-bottom: 1px solid #b9b9b9;\n"
	    		"}\n"
	    		"QTreeView::item:selected {\n"
	    		"background: #d0d0ff;\n"
	    		"color: #000000;\n"
	    		"}\n"
	    		"";
	ui.regexpTable->setStyleSheet(Style);
	UpdateRegExpPatternTable();
}

Properties::~Properties()
{

}


void Properties::UpdateRegExpPatternTable()
{
	ppl6::CString Tmp;
	ui.regexpTable->clear();
	for (size_t i=0;i<wm->RegExpCapture.size();i++) {
		const RegExpPattern &p=wm->RegExpCapture.getPattern(i);
		QTreeWidgetItem *item=new QTreeWidgetItem;
		Tmp.Setf("%i",(int)i+1);
		item->setText(0,Tmp);
		item->setText(1,p.Name);
		item->setText(2,p.Pattern);
		ui.regexpTable->addTopLevelItem (item);
	}
	current_regexp_item=NULL;
}

void Properties::resize ()
{
	int w=ui.regexpTable->width();
	ui.regexpTable->setColumnWidth(0,60);
	w=w-64;
	ui.regexpTable->setColumnWidth(1,w*30/100);
	ui.regexpTable->setColumnWidth(2,w*70/100);
}

void Properties::resizeEvent ( QResizeEvent * event )
{
	resize();
	QDialog::resizeEvent(event);
}

void Properties::showEvent ( QShowEvent * event )
{
	resize();
	QDialog::showEvent(event);
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
	int index;

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
	c->bAutomaticArtistSuggestion=ui.suggestArtist->isChecked();
	c->bAutomaticTitleSuggestion=ui.suggestTitle->isChecked();
	c->bAutomaticEditSearch=ui.automaticEditSearch->isChecked();
	c->bPrintColors=ui.printColors->isChecked();
	c->PrinterFont=ui.font->currentText();
	c->bCacheDirectorySearch=ui.cacheDirectories->isChecked();

	// Devices
	c->DevicePath[1]=ui.devicePathCassette->text();
	c->DevicePath[2]=ui.devicePathCD->text();
	c->DevicePath[3]=ui.devicePathDataStorage->text();
	c->DevicePath[4]=ui.devicePathRecord->text();
	c->DevicePath[5]=ui.devicePathVHS->text();
	c->DevicePath[6]=ui.devicePathOther->text();
	c->DevicePath[7]=ui.devicePathMP3->text();
	c->DevicePath[8]=ui.devicePathTape->text();
	c->DevicePath[9]=ui.devicePathDVD->text();

	// MP3
	c->MP3Player=ui.mp3Player->text();
	c->AIFFPlayer=ui.aiffPlayer->text();
	if (ui.readID3v1->isChecked()) c->ReadId3Tag=1;
	else c->ReadId3Tag=2;
	c->bWriteID3Tags=ui.writeID3->isChecked();
	c->bRemoveOriginalId3Tags=ui.removeOriginalTags->isChecked();
	c->ID3v2Padding=ui.ID3v2Padding->text().toInt();
	c->bSaveOriginalMp3Tags=ui.saveOriginalTags->isChecked();
	c->bSaveOriginalMp3TagsOnAutoImport=ui.saveOriginalTagsOnAutoImport->isChecked();
	c->bNormalizeFilename=ui.normalizeFilenames->isChecked();
	c->TagSaverRetryIntervall=ui.retryIntervall->value();
	if (ui.maxFilenameLength->currentIndex()==0) c->MaxFilenameLength=64;
	else if (ui.maxFilenameLength->currentIndex()==1) c->MaxFilenameLength=100;
	else if (ui.maxFilenameLength->currentIndex()==2) c->MaxFilenameLength=207;
	else if (ui.maxFilenameLength->currentIndex()==3) c->MaxFilenameLength=255;
	else c->MaxFilenameLength=1024;
	c->JpegQualityCover=ui.JpegQualityCover->value();
	c->JpegQualityPreview=ui.JpegQualityPreview->value();

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

	// MusicKey
	switch (ui.displayMusicKey->currentIndex()) {
		case 0: c->musicKeyDisplay=musicKeyTypeMusicalSharps; break;
		case 1: c->musicKeyDisplay=musicKeyTypeOpenKey; break;
		case 2: c->musicKeyDisplay=musicKeyTypeCustom; break;
		default: c->musicKeyDisplay=musicKeyTypeOpenKey; break;
	}
	switch (ui.saveMusicKey->currentIndex()) {
		case 0: c->musicKeyTag=musicKeyTypeNone; break;
		case 1: c->musicKeyTag=musicKeyTypeMusicalSharps; break;
		case 2: c->musicKeyTag=musicKeyTypeOpenKey; break;
		case 3: c->musicKeyTag=musicKeyTypeCustom; break;
		default: c->musicKeyTag=musicKeyTypeOpenKey; break;
	}
	c->customMusicKeyName=ui.customMusicKeyName->text();
	c->customMusicKey[1]=ui.customMusicalKey_1->text();
	c->customMusicKey[2]=ui.customMusicalKey_2->text();
	c->customMusicKey[3]=ui.customMusicalKey_3->text();
	c->customMusicKey[4]=ui.customMusicalKey_4->text();
	c->customMusicKey[5]=ui.customMusicalKey_5->text();
	c->customMusicKey[6]=ui.customMusicalKey_6->text();
	c->customMusicKey[7]=ui.customMusicalKey_7->text();
	c->customMusicKey[8]=ui.customMusicalKey_8->text();
	c->customMusicKey[9]=ui.customMusicalKey_9->text();
	c->customMusicKey[10]=ui.customMusicalKey_10->text();
	c->customMusicKey[11]=ui.customMusicalKey_11->text();
	c->customMusicKey[12]=ui.customMusicalKey_12->text();
	c->customMusicKey[13]=ui.customMusicalKey_13->text();
	c->customMusicKey[14]=ui.customMusicalKey_14->text();
	c->customMusicKey[15]=ui.customMusicalKey_15->text();
	c->customMusicKey[16]=ui.customMusicalKey_16->text();
	c->customMusicKey[17]=ui.customMusicalKey_17->text();
	c->customMusicKey[18]=ui.customMusicalKey_18->text();
	c->customMusicKey[19]=ui.customMusicalKey_19->text();
	c->customMusicKey[20]=ui.customMusicalKey_20->text();
	c->customMusicKey[21]=ui.customMusicalKey_21->text();
	c->customMusicKey[22]=ui.customMusicalKey_22->text();
	c->customMusicKey[23]=ui.customMusicalKey_23->text();
	c->customMusicKey[24]=ui.customMusicalKey_24->text();
	c->customMusicKey[25]=ui.customMusicalKey_25->text();

	// CDDB
	if (ppl6::AudioCD::isSupported()==true && ppl6::CDDB::isSupported()==true) {
		c->cddb.server=ui.cddbServer->text();
		c->cddb.port=ui.cddbPort->text().toInt();
		index=ui.cdioDevice->currentIndex();
		if (index>=0) c->cddb.cddevice=ui.cdioDevice->itemData(index).toString();

		c->cddb.proxy_server=ui.cddbProxyServer->text();
		c->cddb.proxy_port=ui.cddbProxyPort->text().toInt();
		c->cddb.username=ui.cddbUserName->text();
		c->cddb.hostname=ui.cddbHostName->text();
		c->cddb.querypath=ui.cddbQueryPath->text();
		c->cddb.useProxy=ui.cddbUseProxy->isChecked();

	}

	// Save

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

void Properties::on_tabWidget_currentChanged ( int  )
{
	resize();
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


void Properties::on_devicePathButton_clicked(int device, QLineEdit *line)
{
	QString p=line->text();
	if (p.length()==0) {
		p=QDir::homePath();
	}
	ppl6::CString q=tr("Select directory with your files for device type: %s");
	ppl6::CString Tmp;
	Tmp.Setf((const char*)q,(const char*)wm->GetDeviceName(device));


	QString dir = QFileDialog::getExistingDirectory(this, Tmp,
		p,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		line->setText(dir);
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

void Properties::on_aiffPlayerButton_clicked()
{
	QString p=ui.aiffPlayer->text();
	if (p.length()==0) {
		p=QDir::homePath();
	}
	QString file = QFileDialog::getOpenFileName(this, tr("Select the program with which you want to use for playing AIFF-Files"),
	                                                 p);
	if(file.length()) {
		ui.aiffPlayer->setText(file);
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


/*
 * Changes
 */

void Properties::on_wmDataPath_textChanged(const QString &)
{
	Change();
}

void Properties::on_tmpPath_textChanged(const QString &)
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


/*
 * Regular Expressions
 */

void Properties::on_regexpAdd_clicked()
{
	ppl6::CString Tmp;
	RegExpEdit reg(this);
	int ret=reg.exec();
	if (ret==1) {
		RegExpPattern p=reg.getPattern();
		wm->RegExpCapture.addPattern(p);
		wm->RegExpCapture.save();
		size_t pos=wm->RegExpCapture.size();
		QTreeWidgetItem *item=new QTreeWidgetItem;
		Tmp.Setf("%i",(int)pos);
		item->setText(0,Tmp);
		item->setText(1,p.Name);
		item->setText(2,p.Pattern);
		ui.regexpTable->addTopLevelItem (item);
		current_regexp_item=item;
		ui.regexpTable->setCurrentItem(item);
	}
}

void Properties::on_regexpEdit_clicked()
{
	if (!current_regexp_item) return;
	ppl6::CString Tmp;
	Tmp=current_regexp_item->text(0);
	size_t pos=Tmp.ToInt();
	if (pos<1) return;
	const RegExpPattern &pat=wm->RegExpCapture.getPattern(pos-1);

	RegExpEdit reg(this);
	reg.setPattern(pat);
	int ret=reg.exec();
	if (ret==1) {
		RegExpPattern p=reg.getPattern();
		wm->RegExpCapture.setPattern(pos-1,p);
		wm->RegExpCapture.save();
		current_regexp_item->setText(1,p.Name);
		current_regexp_item->setText(2,p.Pattern);
	}
}

void Properties::on_regexpDelete_clicked()
{
	if (!current_regexp_item) return;
}

void Properties::on_regexpUp_clicked()
{
	if (!current_regexp_item) return;
	ppl6::CString Tmp;
	Tmp=current_regexp_item->text(0);
	size_t pos=Tmp.ToInt();
	if (pos<2) return;
	RegExpPattern p=wm->RegExpCapture.getPattern(pos-1);
	wm->RegExpCapture.deletePattern(pos-1);
	wm->RegExpCapture.insertPattern(pos-2,p);
	wm->RegExpCapture.save();
	UpdateRegExpPatternTable();
}

void Properties::on_regexpDown_clicked()
{
	if (!current_regexp_item) return;
	ppl6::CString Tmp;
	Tmp=current_regexp_item->text(0);
	size_t pos=Tmp.ToInt();
	if (pos>=wm->RegExpCapture.size()) return;
	RegExpPattern p=wm->RegExpCapture.getPattern(pos-1);
	wm->RegExpCapture.deletePattern(pos-1);
	wm->RegExpCapture.insertPattern(pos,p);
	wm->RegExpCapture.save();
	UpdateRegExpPatternTable();
}

void Properties::on_regexpTable_itemActivated ( QTreeWidgetItem * item, int )
{
	current_regexp_item=item;
}

void Properties::on_regexpTable_itemClicked ( QTreeWidgetItem * item, int )
{
	current_regexp_item=item;
}

void Properties::on_regexpTable_itemDoubleClicked ( QTreeWidgetItem * item, int )
{
	current_regexp_item=item;
	on_regexpEdit_clicked();
}


void Properties::on_JpegQualityPreview_valueChanged (int value)
{
	Change();
	ppl6::CString Tmp;
	Tmp.Setf("%d",value);
	ui.JpegQualityPreviewValue->setText(Tmp);
}

void Properties::on_JpegQualityCover_valueChanged (int value)
{
	Change();
	ppl6::CString Tmp;
	Tmp.Setf("%d",value);
	ui.JpegQualityCoverValue->setText(Tmp);

}


void Properties::on_cddbUseProxy_toggled(bool)
{
	Change();
	ui.cddbProxyLayout_2->setEnabled(ui.cddbUseProxy->isChecked());
}
