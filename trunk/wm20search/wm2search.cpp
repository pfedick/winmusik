#include "wm2search.h"
#include <QFileDialog>
#include <QSettings>

wm2search::wm2search(QWidget *parent)
    : QDialog(parent)
{
	wm40::InitErrors();
	wm40::SetLogfile(&Log);
	Log.SetLogfile(ppl6::LOG::DEBUG,"debug.log");
	Log.SetLogLevel(ppl6::LOG::DEBUG,10);
	ui.setupUi(this);
	ui.frame->setEnabled(false);
	
	ppl6::WINRECT r;
	LoadWindow("main",&r);
	if (r.width>0 && r.height>0) {
		move(r.left,r.top);
		resize(r.width,r.height);
	}
	
	QSettings settings(WM2S_COMPANY,WM2S_PROGNAME);
	settings.beginGroup("path");
	
	ui.wmpfad->setText(settings.value("wm2path","").toString());
	settings.endGroup();
	ppl6::CpplString s=ui.wmpfad->text();
	
	Norm.LoadDefault();
	
	if (Import(s)) ui.frame->setEnabled(true);
	
}

wm2search::~wm2search()
{
	ppl6::WINRECT r;
	r.left=x();
	r.top=y();
	r.width=width();
	r.height=height();
	SaveWindow("main",&r);

}

int wm2search::SaveWindow(char *name, ppl6::WINRECT *r)
{
	ppl6::CpplString tmp;
	ppl6::RectToString(r,&tmp);
	Mutex.Lock();
	QSettings settings(WM2S_COMPANY,WM2S_PROGNAME);
	settings.beginGroup("windows");
	settings.setValue(name,(char*)tmp);
	settings.endGroup();
	Mutex.Unlock();
	return 1;
}

int wm2search::LoadWindow(char *name, ppl6::WINRECT *r)
{
	ppl6::CpplString tmp;
	Mutex.Lock();
	QSettings settings(WM2S_COMPANY,WM2S_PROGNAME);
	settings.beginGroup("windows");
	tmp=settings.value(name,"0,0,0,0").toString();
	settings.endGroup();
	Mutex.Unlock();
	ppl6::StringToRect(&tmp,r);
	return 1;
}

void wm2search::on_WMPfadSuchen_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("WinMusik 2-Verzeichnis auswählen"),
		ui.wmpfad->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.length()) {
		ui.wmpfad->setText(dir);
	}
	QSettings settings(WM2S_COMPANY,WM2S_PROGNAME);
	settings.beginGroup("path");
	settings.setValue("wm2path",dir);
	settings.endGroup();
	ppl6::CpplString s=dir;
	Import(s);
	
	/*
	QFileDialog dialog(this);
	QString directory;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	if (dialog.exec())
	     directory = dialog.Directory;
	ui.wmpfad->setText(directory);
	*/
}

int wm2search::LoadWm20(char *path)
{
	printf ("wm2search::LoadWm20 Pfad: %s\n",path);
	if (!Ti.Load(path)) return 0;
	if (!TraegerTitel.Load(path)) return 0;
	if (!Version.Load(path,"version.bez",80)) return 0;
	if (!Aufnahme.Load(path,"aufnahme.bez",80)) return 0;
	if (!Kauf.Load(path,"kauf.bez",80)) return 0;
	if (!Label.Load(path,"label.bez",80)) return 0;
	if (!Musikart.Load(path,"musikart.bez",80)) return 0;
	if (!Quelle.Load(path,"quelle.bez",80)) return 0;
	return 1;
}

int wm2search::Import(char *path)
{
	if (LoadWm20(path)) {
		if (Convert()) {
			ui.frame->setEnabled(true);
			printf ("ok\n");
			return 1;
		}
	}
	ui.frame->setEnabled(false);
	ppl6::PrintError();
	return 0;

}

int wm2search::Convert()
{
	int nr=1;
	wm40::TITELDATEI20 *ti;
	while ((ti=Ti.Get(nr))) {
		printf ("%s: %s\n",ti->interpret, ti->titel);
		nr++;
	}
	return 1;
}


