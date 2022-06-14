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

#include "playlistexport.h"
#include "ui_playlistexport.h"
#include <QFileDialog>

PlaylistExport::PlaylistExport(QWidget* parent, CWmClient* wm) :
    QDialog(parent),
    ui(new Ui::PlaylistExport)
{
    this->wm=wm;
    ui->setupUi(this);
    abort_state=false;

    ui->target_path->setText(wm->conf.playlist_export.TargetPath);
    ui->copy_audio_files->setChecked(wm->conf.playlist_export.option_copy_files);
    ui->prepend_tracknumber->setChecked(wm->conf.playlist_export.option_prepend_tracknumber);
    ui->export_m3u->setChecked(wm->conf.playlist_export.export_m3u);
    ui->export_pls->setChecked(wm->conf.playlist_export.export_pls);
    ui->export_xspf->setChecked(wm->conf.playlist_export.export_xspf);
    ui->export_txt->setChecked(wm->conf.playlist_export.export_txt);

}

PlaylistExport::~PlaylistExport()
{
    delete ui;
}

void PlaylistExport::on_okButton_clicked()
{
    wm->conf.playlist_export.TargetPath=ui->target_path->text();
    wm->conf.playlist_export.option_copy_files=ui->copy_audio_files->isChecked();
    wm->conf.playlist_export.option_prepend_tracknumber=ui->prepend_tracknumber->isChecked();
    wm->conf.playlist_export.export_m3u=ui->export_m3u->isChecked();
    wm->conf.playlist_export.export_pls=ui->export_pls->isChecked();
    wm->conf.playlist_export.export_xspf=ui->export_xspf->isChecked();
    wm->conf.playlist_export.export_txt=ui->export_txt->isChecked();
    wm->conf.trySave();
    done(1);
}

void PlaylistExport::on_cancelButton_clicked()
{
    done(0);
    abort_state=true;
}

void PlaylistExport::on_selectTargetPath_clicked()
{
    QString pathname=QFileDialog::getExistingDirectory(NULL, tr("select target directory"), ui->target_path->text());
    if (pathname.isEmpty()) return;
    ui->target_path->setText(pathname);
}

void PlaylistExport::start(int number_of_tracks)
{
    ui->optionsGroupBox->setEnabled(false);
    ui->progressGroupBox->setEnabled(true);
    ui->okButton->setEnabled(false);
    ui->cancelButton->setEnabled(true);
    ui->progress_file->setValue(0);
    ui->progress_total->setMaximum(number_of_tracks);
    ui->progress_total->setValue(0);
    abort_state=false;
}

bool PlaylistExport::abortClicked() const
{
    return abort_state;
}

void PlaylistExport::setCurrentFile(const ppl7::String& filename, size_t size)
{
    ui->progress_file->setMaximum((int)size);
    ui->current_file->setText(filename);
}

void PlaylistExport::setTotalProgress(int track)
{
    ui->progress_total->setValue(track);
}

void PlaylistExport::setFileProgress(size_t bytes)
{
    ui->progress_file->setValue((int)bytes);
}
