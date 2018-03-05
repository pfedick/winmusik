#ifndef PLAYLISTEXPORT_H
#define PLAYLISTEXPORT_H

#include <QDialog>
#include "winmusik3.h"

namespace Ui {
class PlaylistExport;
}

class PlaylistExport : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistExport(QWidget *parent = 0, CWmClient *wm=NULL);
    ~PlaylistExport();

    void start(int number_of_tracks);
    bool abortClicked() const;
    void setCurrentFile(const ppl6::CString &filename, size_t size);
    void setTotalProgress(int track);
    void setFileProgress(size_t bytes);



private:
    Ui::PlaylistExport *ui;
    CWmClient *wm;
    bool abort_state;

public slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

    void on_selectTargetPath_clicked();

};

#endif // PLAYLISTEXPORT_H
