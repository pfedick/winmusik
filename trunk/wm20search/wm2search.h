#ifndef WM2SEARCH_H
#define WM2SEARCH_H
#define WITH_QT
#include <ppl6.h>
#include <winmusik/winmusik.h>
#include <winmusik/import.h>

#include <QtGui/QDialog>
#include <QCloseEvent>
#include "ui_wm2search.h"

#define WM2S_COMPANY "Patrick F.-Productions"
#define WM2S_PROGNAME "wm2search"

class wm2search : public QDialog
{
    Q_OBJECT

private:
    Ui::wm2searchClass ui;
    ppl6::CpplMutex	Mutex;
    ppl6::CpplLog	Log;
    wm40::CNormalizer Norm;
    
	wm40::Cwm20Titel Ti;
	wm40::Cwm20TraegerTitel TraegerTitel;
	wm40::Cwm20Table Version;
	wm40::Cwm20Table Aufnahme;
	wm40::Cwm20Table Kauf;
	wm40::Cwm20Table Label;
	wm40::Cwm20Table Musikart;
	wm40::Cwm20Table Quelle;

	int LoadWm20(char *path);
	int Convert();

public:
    wm2search(QWidget *parent = 0);
    ~wm2search();
    int Import(char *path);
    
    int SaveWindow(char *name, ppl6::WINRECT *r);
    int LoadWindow(char *name, ppl6::WINRECT *r);

private slots:
	void on_WMPfadSuchen_clicked();

};

#endif // WM2SEARCH_H
