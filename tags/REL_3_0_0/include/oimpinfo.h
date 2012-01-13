#ifndef OIMPINFO_H
#define OIMPINFO_H

#include <QtGui/QWidget>
#include "ui_oimpinfo.h"
#include "winmusik3.h"

class OimpInfo : public QWidget
{
    Q_OBJECT

public:
    OimpInfo(QWidget *parent = 0, DataOimp *oimp=NULL);
    ~OimpInfo();

    void CopyData(DataOimp *oimp);

private:
    Ui::OimpInfoClass ui;

private slots:
	void on_deleteButton_clicked();
	void on_closeButton_clicked();
};

#endif // OIMPINFO_H
