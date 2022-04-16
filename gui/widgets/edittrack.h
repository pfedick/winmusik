#ifndef EDITTRACK_H
#define EDITTRACK_H

#include <QWidget>
#include "ui_edittrack.h"
#include "winmusik3.h"
#include <QRegExpValidator>
#include <QKeyEvent>
#include "tablecontrol.h"

#include "ppl6-sound.h"
#include "fkeys.h"
#include "src/editor/oimpinfo.h"



class EditTrack : public QWidget
{
    Q_OBJECT

public:
    EditTrack(QWidget *parent = 0);
    ~EditTrack();

    void setWinMusikClient(CWmClient *wm);
    void setData(const TrackInfo &data);
    void setFilename(const ppl6::CString &Filename);
    void setFkeys(Fkeys *fkeys);
    const TrackInfo &getData();
    void setFocus();
    void setFocus (Qt::FocusReason reason);
    void ReloadTranslation();

    PPLNORMALEXCEPTION(DataIncompleteException);


private:
    Ui::EditTrackClass ui;
    CWmClient *wm;
    Fkeys		*fkeys;
    TrackInfo	data;
    QPixmap			Cover;
    int position, oldposition;

    CTableControl TCVersion;
    CTableControl TCGenre;
    CTableControl TCLabel;
    CTableControl TCRecordSource;
    CTableControl TCRecordDevice;
    CTrackList	*TrackList;
    QStringList Artists;
    QStringList Titles;
    QStringList Albums;
    QCompleter *artistCompleter;
    QCompleter *titleCompleter;
    QCompleter *albumCompleter;
    bool		DupeCheck;
    QTimer		*DupeTimer;
    const char	*DupeCheckIcon;
    WMTreeItem	*currentTrackListItem;
    QWidget *searchWindow;



    bool eventFilter(QObject *target, QEvent *event);
    bool consumeEvent(QObject *target, QEvent *event);
    void InstallFilter(QObject *object, int id);
    void CheckDupes();

    bool checkKeyPress(QObject *target, int key,int modifier);
    bool on_length_FocusOut();
    void UpdateFkeys();
    void MoveToNextWidget();

public slots:
	void on_DupeTimer_update();

signals:
	void escPressed();
	void pageDownPressed();
	void pageUpPressed();
	void save();
	void showOimpData(const DataOimp &data);
	void updateFkeys(int position);

};

#endif // EDITTRACK_H
