#include "../include/splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	ppl6::CString Tmp,DateFormat,Date;

	Tmp=tr("WinMusik Version %v% vom %d%");
	Tmp.Replace("%v%",WM_VERSION);
	DateFormat=tr("%d.%m.%Y");

	int a,year,month,day;
	a=WM_RELEASEDATE;
	year=a/10000;
	a=a-year*10000;
	month=a/100;
	day=a-month*100;
	ppluint64 t=ppl6::MkTime(year,month,day);
	ppl6::MkDate(Date,DateFormat,t);

	Tmp.Replace("%d%",(const char*)Date);

	Tmp.Concat(", ");
	Tmp+=WM_COPYRIGHT;
	ui.copyright->setText(Tmp);
}

SplashScreen::~SplashScreen()
{

}

void SplashScreen::setMessage(QString msg)
{
	ui.loading->setText(msg);
	qApp->processEvents();
}

void SplashScreen::Update()
{
	ui.progressBar->setValue(progress);
	qApp->processEvents();
}
