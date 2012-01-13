#ifndef EDITDEVICE_H
#define EDITDEVICE_H

#include "winmusik3.h"
#include "tablecontrol.h"
#include <QtGui/QDialog>
#include <QKeyEvent>
#include "ui_editdevice.h"

class EditDevice : public QDialog
{
    Q_OBJECT

public:
    EditDevice(QWidget *parent = 0, CWmClient *wm=NULL, int typ=0, ppluint32 DeviceId=0);
    ~EditDevice();

private:
    Ui::EditDeviceClass ui;
    CWmClient *wm;
    QWidget *parent;
    int DeviceType;
    int DeviceId;
    DataDevice	Device;

    int position, oldposition;

    CTableControl Label;
    CTableControl PurchaseSource;



    void InstallFilter(QObject *object, int id);
    bool eventFilter(QObject *target, QEvent *event);
    bool consumeEvent(QObject *target, QEvent *event);

    bool Save();

    // Events
    bool nextWidget(QObject *object);


private slots:

	void on_okButton_clicked();
	void on_cancelButton_clicked();

};

#endif // EDITDEVICE_H
