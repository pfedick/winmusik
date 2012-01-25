#ifndef EDITTRACK_H
#define EDITTRACK_H

#include <QtGui/QWidget>
#include "ui_edittrack.h"
#include "winmusik3.h"
#include <ppl6-sound.h>
#include <QRegExpValidator>
#include <QKeyEvent>


class EditTrack : public QWidget
{
    Q_OBJECT

public:
    EditTrack(QWidget *parent = 0);
    ~EditTrack();

    void setWinMusikClient(CWmClient *wm);
    void setData(const TrackInfo &data);
    void setFilename(const ppl6::CString &Filename);
    const TrackInfo &getData() const;

private:
    Ui::EditTrackClass ui;
    CWmClient *wm;
    TrackInfo	data;
    QPixmap			Cover;
};

#endif // EDITTRACK_H
