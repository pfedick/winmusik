/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: updater.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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
#include "version.h"
#include "updater.h"
#include <QDesktopServices>


CUpdateChecker::CUpdateChecker(QObject *parent, CWmClient *wm)
: QObject(parent)
{
	this->wm=wm;
	pd=NULL;
	timer=NULL;
	manualcheck=false;
	updateavailable=false;
	CurrentId=0;
}

CUpdateChecker::~CUpdateChecker()
{
	if (timer) delete timer;
	if (pd) delete pd;
}

void CUpdateChecker::ThreadMain(void *data)
/*!\brief Update suchen im eigenen Thread
 *
 * \desc
 * Diese Funktion führt eine Abfrage beim WinMusik Registrierungsserver durch, ob eine neue Version
 * verfügbar ist. Sie wird in der Regel als eigener Thread ausgeführt.
 * \par
 * Folgende Daten werden an den Registrierungsserver geschickt:
 * - aktuelle Version
 * - eingesetzte Qt-Version
 * - eingestellte Sprache
 * - Betriebssystem-String (siehe CWmClient::GetOperatingSystem)
 * - Registrierungsschlüssel (sofern WinMusik registriert wurde)
 *
 * \param[in] data Wird nicht verwendet
 */
{
	(void) data;
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CUpdateChecker","ThreadMain",__FILE__,__LINE__,"Prüfe auf Updates");
	updateavailable=false;
	ppl6::CCurl Curl;
	if (wmlog) Curl.SetLogfile(wmlog);
	Curl.SetURL(WM_REGISTRATION_URL);
	Curl.VerifyPeer(false);
	Curl.SetTimeout(5);
	Curl.SetBrowser("WinMusik 3.0");
	ppl6::CAssocArray cmd, answer;
	cmd.Set("action","checkUpdates");
	cmd.Set("version",WM_VERSION);
	cmd.Set("lang",(const char*)wm->conf.Locale);
	cmd.Set("serial",(const char*)wm->conf.Serial);
	cmd.Set("os",(const char*)wm->GetOperatingSystem());
	if (manualcheck) cmd.Set("trigger","manual");
	else cmd.Set("trigger","auto");

	if (!wm->CurlTalk(Curl,cmd,answer)) {
		ppl6::StoreError(&result);
		if (wmlog) {
			wmlog->LogError("CUpdateChecker","ThreadMain",__FILE__,__LINE__);
			wmlog->Printf(ppl6::LOG::DEBUG,1,"CUpdateChecker","ThreadMain",__FILE__,__LINE__,"Keine Updates gefunden");
		}
		return;
	}
	ppl6::SetError(0);
	ppl6::StoreError(&result);
	if (wmlog) {
		wmlog->Printf(ppl6::LOG::DEBUG,1,"CUpdateChecker","ThreadMain",__FILE__,__LINE__,"Updates gefunden");
		wmlog->PrintArray(ppl6::LOG::DEBUG,5,"CUpdateChecker","ThreadMain",__FILE__,__LINE__,&answer,"answer");
	}
	updateavailable=true;
	UpdateInfo=answer.GetArray("releases");
	CurrentVersion=answer["CurrentVersion"];
	DownloadLink=answer["DownloadLink"];
	CurrentId=answer.ToInt("ID");
	CurrentReleasedate=answer.ToLongLong("CurrentReleasedate");
}

void CUpdateChecker::CheckNow(QWidget *parent)
{
	timeoutsteps=0;
	manualcheck=true;
	pd = new QProgressDialog(tr("Checking for WinMusik updates..."),tr("Cancel"), 0, 100,parent);
	pd->setWindowTitle(tr("WinMusik: Updates"));
	pd->setModal(true);
	QObject::connect(pd, SIGNAL(canceled()), this, SLOT(on_progressDialog_cancel()));
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_perform()));
	timer->start(100);
	ThreadStart();
	while (ThreadIsRunning()) {
		qApp->processEvents();
	}
	timer->stop();
	delete timer;
	delete pd;
	timer=NULL;
	pd=NULL;
	if (result.err==20037) {
		QMessageBox::information(parent, tr("WinMusik: Updates"),
				tr("There are no updates available,\nyou are running the latest version."));
		return;
	} else if (result.err>0) {
		ppl6::RestoreError(&result);
		wm->RaiseError(parent,tr("Check for Updates failed."));
		return;
	}

	Updater *w=new Updater;
	w->Init(UpdateInfo,CurrentVersion,CurrentReleasedate,DownloadLink);
	w->exec();
	delete w;
}

 void CUpdateChecker::on_timer_perform()
 {
     pd->setValue(timeoutsteps);
     //... perform one percent of the operation
     timeoutsteps++;
     if (timeoutsteps > pd->maximum())
         timer->stop();
 }

 void CUpdateChecker::on_progressDialog_cancel()
 {
     timer->stop();
     ThreadShouldStop();
 }


Updater::Updater(QWidget *parent, CWmClient *wm)
: QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	ppl6::CString Tmp=tr("%d.%m.%Y");
	ui.oldVersion->setText(WM_VERSION);
	ui.oldReleasedate->setText(ppl6::Long2Date(Tmp,WM_RELEASEDATE));
}

Updater::~Updater()
{

}

void Updater::Init(const ppl6::CAssocArray &UpdateInfo, ppl6::CString &CurrentVersion, ppluint64 CurrentReleasedate, ppl6::CString &DownloadLink)
{
	DownloadUrl=DownloadLink;
	ppl6::CString Tmp=tr("%d.%m.%Y");
	ui.newVersion->setText(CurrentVersion);
	ui.newReleasedate->setText(ppl6::MkDate(Tmp,CurrentReleasedate));
	ppl6::CString Notes;
	Notes="<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
		"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
		"p, li { white-space: pre-wrap; }\n"
		"</style></head><body style=\" font-family:'Bitstream Vera Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
		"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\n";

	ppl6::CAssocArray release;
	ppl6::CAssocArray info=UpdateInfo;
	info.Reset();
	while ((info.GetNextArray(release))) {
		Notes+="<b>";
		Notes+=tr("Version:");
		Notes+=" ";
		Notes+=release["version"];
		Notes+=", ";
		Tmp=tr("Releasedate: %d.%m.%Y");
		Notes+=ppl6::MkDate(Tmp,release.ToLongLong("releasedate"));
		Notes+="<p style=\"margin-left: 30px;\">";
		Notes+=release["releasenotes"];
		Notes+="</p>\n";
	}


	Notes+="</p></body></html>\n";

	ui.changes->setHtml(Notes);
}

void Updater::on_okButton_clicked()
{
	done(1);
}

void Updater::on_webButton_clicked()
{
	QDesktopServices::openUrl(QUrl(DownloadUrl));
	done(1);
}
