#ifndef COVERVIEWER_H
#define COVERVIEWER_H

#include <QWidget>
#include "ui_coverviewer.h"
#include "winmusik3.h"

class CoverViewer : public QWidget
{
    Q_OBJECT

public:
    CoverViewer(QWidget *parent = 0, CWmClient *wm=NULL);
    ~CoverViewer();

    void setCover(const QPixmap &pix);


private:
    CWmClient *wm;
    Ui::CoverViewerClass ui;

    void closeEvent(QCloseEvent *event);
};

#endif // COVERVIEWER_H
