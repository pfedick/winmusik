#ifndef EDITTRACK_H
#define EDITTRACK_H

#include <QtGui/QWidget>
#include "ui_edittrack.h"

class EditTrack : public QWidget
{
    Q_OBJECT

public:
    EditTrack(QWidget *parent = 0);
    ~EditTrack();

private:
    Ui::EditTrackClass ui;
};

#endif // EDITTRACK_H
