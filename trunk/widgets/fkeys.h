#ifndef FKEYS_H
#define FKEYS_H

#include <QtGui/QWidget>
#include "ui_fkeys.h"

class Fkeys : public QWidget
{
    Q_OBJECT

public:
    Fkeys(QWidget *parent = 0);
    ~Fkeys();

    void setFkey(int num, const QString &Icon, const QString &Text, bool enabled=true);
    void setFkey(int num, const QIcon &Icon, const QString &Text, bool enabled=true);
    void setEnabled(int num, bool flag);
    bool isEnabled(int num) const;

private:
    Ui::FkeysClass ui;
    QToolButton *button[13];


public slots:
void on_esc_clicked();
void on_f1_clicked();
void on_f2_clicked();
void on_f3_clicked();
void on_f4_clicked();
void on_f5_clicked();
void on_f6_clicked();
void on_f7_clicked();
void on_f8_clicked();
void on_f9_clicked();
void on_f10_clicked();
void on_f11_clicked();
void on_f12_clicked();

signals:
     void clicked(int keynum);

};

#endif // FKEYS_H
