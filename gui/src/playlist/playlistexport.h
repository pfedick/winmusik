/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    explicit PlaylistExport(QWidget* parent = 0, CWmClient* wm=NULL);
    ~PlaylistExport();

    void start(int number_of_tracks);
    bool abortClicked() const;
    void setCurrentFile(const ppl7::String& filename, size_t size);
    void setTotalProgress(int track);
    void setFileProgress(size_t bytes);



private:
    Ui::PlaylistExport* ui;
    CWmClient* wm;
    bool abort_state;

public slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

    void on_selectTargetPath_clicked();

};

#endif // PLAYLISTEXPORT_H
