#include "../include/oimpinfo.h"

OimpInfo::OimpInfo(QWidget *parent, DataOimp *oimp)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.tabWidget->setCurrentIndex(1);
	if (oimp) CopyData(oimp);

}

OimpInfo::~OimpInfo()
{

}

void OimpInfo::CopyData(DataOimp *oimp)
{
	ppl6::CString Tmp;

	ui.filename->setText(oimp->Filename);

	Tmp=oimp->ID3v2.Get("artist");
	ui.v2Artist->setText(Tmp);
	Tmp=oimp->ID3v2.Get("title");
	ui.v2Title->setText(Tmp);
	Tmp=oimp->ID3v2.Get("album");
	ui.v2Album->setText(Tmp);
	Tmp=oimp->ID3v2.Get("year");
	ui.v2Year->setText(Tmp);
	Tmp=oimp->ID3v2.Get("comment");
	ui.v2Comment->setText(Tmp);
	Tmp=oimp->ID3v2.Get("genre");
	ui.v2Genre->setText(Tmp);
	Tmp=oimp->ID3v2.Get("remixer");
	ui.v2Remixer->setText(Tmp);

	Tmp=oimp->ID3v1.Get("artist");
	ui.v1Artist->setText(Tmp);
	Tmp=oimp->ID3v1.Get("title");
	ui.v1Title->setText(Tmp);
	Tmp=oimp->ID3v1.Get("album");
	ui.v1Album->setText(Tmp);
	Tmp=oimp->ID3v1.Get("year");
	ui.v1Year->setText(Tmp);
	Tmp=oimp->ID3v1.Get("comment");
	ui.v1Comment->setText(Tmp);
	Tmp=oimp->ID3v1.Get("genre");
	ui.v1Genre->setText(Tmp);

}

void OimpInfo::on_deleteButton_clicked()
{
	close();
}

void OimpInfo::on_closeButton_clicked()
{
	close();
}
