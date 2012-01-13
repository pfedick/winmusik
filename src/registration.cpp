/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: registration.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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


#include "../include/registration.h"

Registration::Registration(QWidget *parent, CWmClient *wm)
    : QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	ppl6::CString s, Country, Code;;
	ppl6::CTok Tok;
	QString Line;
	QFile file(":/other/resources/countries.csv");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	while (!file.atEnd()) {
		Line=file.readLine(1024);
		s=Line;
		s.Trim();
		if (s.NotEmpty()) {
			Tok.Split(s,";");
			Country=Tok[0];
			Code=Tok[1];
			Country.LCase();
			Country.UCWords();
			ui.country->addItem(Country,Code);
		}
	}
	int index=-1;
	if (wm->conf.Locale=="de") index=ui.country->findText("Germany");
	if (index>=0) ui.country->setCurrentIndex(index);
	ui.name->setText(wm->conf.UserName);
	ui.name2->setText(wm->conf.UserName);
	ui.stackedWidget->setCurrentIndex(0);

	ui.continueButton4->setEnabled(false);

    connect(ui.block1,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_block1_gotFocus()));
    connect(ui.block2,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_block2_gotFocus()));
    connect(ui.block3,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_block3_gotFocus()));
    connect(ui.block4,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_block4_gotFocus()));
    connect(ui.block5,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_block5_gotFocus()));
    connect(ui.block6,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_block6_gotFocus()));

    connect(ui.ser1,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_ser1_gotFocus()));
    connect(ui.ser2,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_ser2_gotFocus()));
    connect(ui.ser3,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_ser3_gotFocus()));
    connect(ui.ser4,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_ser4_gotFocus()));
    connect(ui.ser5,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_ser5_gotFocus()));
    connect(ui.ser6,SIGNAL(focusInEvent(QFocusEvent *)),
    		this,SLOT(on_ser6_gotFocus()));

}

Registration::~Registration()
{

}

void Registration::on_continueButton1_clicked()
{
	ppl6::CString Name, Email;
	Name=ui.name->text();
	Name.Trim();
	if (Name.IsEmpty()) {
		return;
	}
	Email=ui.email->text();
	Email.Trim();
	if (Email.IsEmpty()==true || Email.Instr("@")<0) {
		return;
	}

	ui.checkName->setText(ui.name->text());
	ui.checkEmail->setText(ui.email->text());
	ui.checkCountry->setText(ui.country->currentText());
	ui.checkLanguage->setText(wm->conf.Locale);
	ui.checkWMVersion->setText(WM_VERSION);
	ui.checkOS->setText(wm->GetOperatingSystem());
	if (ui.informAboutUpdates->isChecked())	ui.checkWantUpdates->setText(tr("yes"));
	else ui.checkWantUpdates->setText(tr("no"));
	ui.stackedWidget->setCurrentIndex(2);
}

void Registration::on_continueButton2_clicked()
{
	ui.stackedWidget->setCurrentIndex(3);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	//ppl6::CString AppPath=QCoreApplication::applicationDirPath();
	ui.regmsg1->setText(tr("Connecting to server, please wait..."));
	qApp->processEvents();
	if (wmlog) Curl.SetLogfile(wmlog);
	Curl.SetURL(WM_REGISTRATION_URL);
	Curl.VerifyPeer(false);
	Curl.SetBrowser("WinMusik 3.0");
	ppl6::CAssocArray cmd, answer;
	cmd.Set("action","startSession");
	if (!wm->CurlTalk(Curl,cmd,answer)) {
		QApplication::restoreOverrideCursor();
		wm->RaiseError(this,tr("Connect with registration server failed.\nPlease check if your internet connection is active and try again."));
		ui.stackedWidget->setCurrentIndex(2);
		return;
	}
	ui.regmsg1->setText(tr("Sending registration data, please wait..."));
	qApp->processEvents();
	ppl6::CString Name=ui.name->text();
	ppl6::CString Email=ui.email->text();
	int c=ui.country->currentIndex();
	ppl6::CString Country=ui.country->itemData(c).toString();


	cmd.Clear();
	cmd.Set("action","newReg");
	cmd.Set("name",Name);
	cmd.Set("email",Email);
	cmd.Set("country",Country);
	cmd.Set("lang",wm->conf.Locale);
	cmd.Set("version",WM_VERSION);
	cmd.Set("os",(const char*)wm->GetOperatingSystem());
	cmd.Setf("wantupdates","%i",ui.informAboutUpdates->isChecked());
	//cmd.Set("os",)
	if (!wm->CurlTalk(Curl,cmd,answer)) {
		QApplication::restoreOverrideCursor();
		wm->RaiseError(this,tr("Registration failed"));
		ui.stackedWidget->setCurrentIndex(2);
		return;
	}
	ui.continueButton3->setEnabled(false);
	ui.stackedWidget->setCurrentIndex(4);
	qApp->processEvents();
	QApplication::restoreOverrideCursor();
	SerialMD5=answer["SerialMD5"];
}

void Registration::on_continueButton3_clicked()
{
	ui.stackedWidget->setCurrentIndex(3);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	ui.regmsg1->setText(tr("Verifying registration key, please wait..."));
	qApp->processEvents();
	ppl6::CAssocArray cmd, answer;
	cmd.Set("action","verifyReg");
	ppl6::CString Serial;
	Serial=ui.block1->text();
	Serial+="-";
	Serial+=ui.block2->text();
	Serial+="-";
	Serial+=ui.block3->text();
	Serial+="-";
	Serial+=ui.block4->text();
	Serial+="-";
	Serial+=ui.block5->text();
	Serial+="-";
	Serial+=ui.block6->text();
	Serial.UCase();
	ppl6::CString Email=ui.email->text();
	cmd.Set("serial",Serial);
	cmd.Set("email",Email);
	if (!wm->CurlTalk(Curl,cmd,answer)) {
		QApplication::restoreOverrideCursor();
		wm->RaiseError(this,tr("Verification failed"));
		ui.stackedWidget->setCurrentIndex(4);
		return;
	}
	QApplication::restoreOverrideCursor();
	wm->conf.Serial=Serial;
	wm->conf.Save();
	QMessageBox::information(this, tr("WinMusik: Registration"),
		tr("WinMusik was successfully registered.\nThank you!"));
	done(1);
}

void Registration::CheckSerialInput()
{
	ui.continueButton3->setEnabled(false);
	if (ui.block1->text().size()<5) return;
	if (ui.block2->text().size()<5) return;
	if (ui.block3->text().size()<5) return;
	if (ui.block4->text().size()<5) return;
	if (ui.block5->text().size()<5) return;
	if (ui.block6->text().size()<5) return;
	ui.continueButton3->setEnabled(true);
}

void Registration::CheckSerialInput2()
{
	ui.continueButton4->setEnabled(false);
	if (ui.ser1->text().size()<5) return;
	if (ui.ser2->text().size()<5) return;
	if (ui.ser3->text().size()<5) return;
	if (ui.ser4->text().size()<5) return;
	if (ui.ser5->text().size()<5) return;
	if (ui.ser6->text().size()<5) return;
	ui.continueButton4->setEnabled(true);
}

void Registration::on_continueButton4_clicked()
{
	ppl6::CString Name, Email;
	Name=ui.name->text();
	Name.Trim();
	Email=ui.email->text();
	Email.Trim();
	if (Name.IsEmpty()) {
		if (Email.IsEmpty()==true || Email.Instr("@")<0) {
			return;
		}
	}

	ui.stackedWidget->setCurrentIndex(3);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	//ppl6::CString AppPath=QCoreApplication::applicationDirPath();
	ui.regmsg1->setText(tr("Connecting to server, please wait..."));
	qApp->processEvents();
	if (wmlog) Curl.SetLogfile(wmlog);
	Curl.SetURL(WM_REGISTRATION_URL);
	Curl.VerifyPeer(false);
	Curl.SetBrowser("WinMusik 3.0");
	ppl6::CAssocArray cmd, answer;
	cmd.Set("action","startSession");
	if (!wm->CurlTalk(Curl,cmd,answer)) {
		QApplication::restoreOverrideCursor();
		wm->RaiseError(this,tr("Connect with registration server failed.\nPlease check if your internet connection is active and try again."));
		ui.stackedWidget->setCurrentIndex(5);
		return;
	}
	ui.regmsg1->setText(tr("Verifying registration key, please wait..."));
	qApp->processEvents();

	cmd.Clear();
	cmd.Set("action","recoverReg");
	ppl6::CString Serial;
	Serial=ui.ser1->text();
	Serial+="-";
	Serial+=ui.ser2->text();
	Serial+="-";
	Serial+=ui.ser3->text();
	Serial+="-";
	Serial+=ui.ser4->text();
	Serial+="-";
	Serial+=ui.ser5->text();
	Serial+="-";
	Serial+=ui.ser6->text();
	Serial.UCase();
	cmd.Set("serial",Serial);
	cmd.Set("email",Email);
	cmd.Set("name",Name);
	if (!wm->CurlTalk(Curl,cmd,answer)) {
		QApplication::restoreOverrideCursor();
		wm->RaiseError(this,tr("Verification failed"));
		ui.stackedWidget->setCurrentIndex(5);
		return;
	}
	QApplication::restoreOverrideCursor();
	wm->conf.Serial=Serial;
	wm->conf.Save();
	QMessageBox::information(this, tr("WinMusik: Registration"),
		tr("Your registration key was verified.\nThank you!"));
	done(1);
}
