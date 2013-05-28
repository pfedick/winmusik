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
	if (wm) wm->CoverViewerClosed();
}


void CoverViewer::setCover(const QPixmap &pix)
{
	ui.cover->setPixmap(pix);
}


void CoverViewer::closeEvent(QCloseEvent *event)
{
	if (wm) {
		wm->SaveGeometry("CoverViewer",this->saveGeometry());
		wm->CoverViewerClosed();
		wm=NULL;
	}
    QWidget::closeEvent(event);
}
