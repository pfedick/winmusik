#include "../include/coverviewer.h"

CoverViewer::CoverViewer(QWidget *parent, CWmClient *wm)
    : QWidget(parent)
{
	this->wm=wm;
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose,true);
	this->restoreGeometry(wm->GetGeometry("CoverViewer"));
}

CoverViewer::~CoverViewer()
{
	if (wm) wm->CoverViewerClosed(this);
}


void CoverViewer::setCover(const QPixmap &pix)
{
	ui.cover->setPixmap(pix);
}


void Edit::closeEvent(QCloseEvent *event)
{
	if (wm) {
		wm->SaveGeometry("CoverViewer",this->saveGeometry());
	}
	if (oimpInfo) {
		delete oimpInfo;
		oimpInfo=NULL;
	}
    QWidget::closeEvent(event);
}
