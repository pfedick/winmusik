#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "winmusik3.h"
#include <QtGui/QDialog>
#include "ui_splashscreen.h"

class SplashScreen : public QDialog, public CCallback
{
    Q_OBJECT

public:
    SplashScreen(QWidget *parent = 0);
    ~SplashScreen();

    virtual void Update();
    void setMessage(QString msg);


private:
    Ui::SplashScreenClass ui;
};

#endif // SPLASHSCREEN_H
