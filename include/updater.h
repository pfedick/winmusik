/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: updater.h,v 1.2 2010/05/16 12:40:40 pafe Exp $
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



#ifndef UPDATER_H_
#define UPDATER_H_

#include "winmusik3.h"
#include <QtGui/QDialog>
#include <QProgressDialog>
#include <QTimer>
#include "ui_updater.h"

class Updater : public QDialog
{
    Q_OBJECT

public:
    Updater(QWidget *parent = 0, CWmClient *wm = NULL);
    ~Updater();

private:
    Ui::UpdaterClass ui;
    CWmClient *wm;
    ppl6::CString DownloadUrl;

public:
    void Init(const ppl6::CAssocArray &UpdateInfo, ppl6::CString &CurrentVersion, ppluint64 CurrentReleasedate, ppl6::CString &DownloadLink);

public slots:
	void on_okButton_clicked();
	void on_webButton_clicked();

};


class CUpdateChecker : public QObject, public ppl6::CThread
{
	Q_OBJECT

	friend class CWmClient;
	friend class Menue;
	private:
		CWmClient *wm;

		QProgressDialog *pd;
		QTimer			*timer;
		int				timeoutsteps;
		bool			manualcheck;
		bool			updateavailable;

		ppl6::PPL_ERROR	result;

	public:
		ppl6::CAssocArray	UpdateInfo;
		ppl6::CString		CurrentVersion;
		ppluint64			CurrentReleasedate;
		int					CurrentId;
		ppl6::CString		DownloadLink;

		CUpdateChecker(QObject *parent, CWmClient *wm=NULL);
		virtual ~CUpdateChecker();

		virtual void ThreadMain(void *data);

		void CheckNow(QWidget *parent);
		int	UpdateAvailable();

	public slots:
	void on_timer_perform();
	void on_progressDialog_cancel();

};




#endif /* UPDATER_H_ */
