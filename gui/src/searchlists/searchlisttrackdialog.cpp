/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

#include <QClipboard>
#include <QMenu>
#include <QList>
#include <QUrl>
#include <QUuid>
#include <QMimeData>
#include <QMouseEvent>
#include "searchlisttrackdialog.h"
#include <pplib/audio/id3tag.h>
#include "csearchlist.h"
#include "regexpcapture.h"
#include "wm_coverdownload.h"
#include "wm_regexpcapture.h"

SearchlistTrackDialog::SearchlistTrackDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->show();
    this->restoreGeometry(wm_main->GetGeometry("SearchlistTrackDialog"));
    QWidget* top = window();
    top->raise();
    top->activateWindow();
}

SearchlistTrackDialog::~SearchlistTrackDialog()
{
}

void SearchlistTrackDialog::closeEvent(QCloseEvent* event)
{
    wm_main->SaveGeometry("SearchlistTrackDialog", this->saveGeometry());
    QDialog::closeEvent(event);
}

void SearchlistTrackDialog::Resize()
{
}

void SearchlistTrackDialog::showEvent(QShowEvent* event)
{
    Resize();
    QWidget::showEvent(event);
}
void SearchlistTrackDialog::resizeEvent(QResizeEvent* event)
{
    Resize();
    QWidget::resizeEvent(event);
}

void SearchlistTrackDialog::set(const SearchlistItem& track)
{
    Track = track;
    ui.artistEdit->setText(track.Artist);
    ui.titleEdit->setText(track.Title);
    ui.versionEdit->setText(track.Version);
    ui.genreEdit->setText(track.Genre);
    ui.commentEdit->setText(track.Comment);
    ui.labelEdit->setText(track.Label);
    ui.shopURLEdit->setText(track.ShopURL);
    ui.tagsEdit->setText(track.Tags);
    ui.keyEdit->setText(track.Key);
    ui.bpmEdit->setText(track.Bpm);
    if (Track.CoverFilename.notEmpty()) {
        ui.coverwidget->setPixmapFromFile(Track.CoverFilename);
    }

    ui.releaseDateEdit->setText(track.ReleaseDate.get("%Y-%m-%d"));
    if (track.Length > 0) ui.lengthEdit->setText(pplib::ToString("%i:%02i", track.Length / 60, track.Length % 60));
    ui.rating->setCurrentIndex(track.Rating);
}

void SearchlistTrackDialog::setFromClipboard()
{
    RegExpMatch match;
    RegExpClipboard clip;
    copyFromClipboard(clip);
    if (wm_main->RegExpCapture.match(clip, match)) {
        ui.artistEdit->setText(match.Artist);
        ui.titleEdit->setText(match.Title);
        ui.versionEdit->setText(match.Version);
        ui.genreEdit->setText(match.Genre);
        // ui.commentEdit->setText(match.Label);
        ui.releaseDateEdit->setText(match.ReleaseDate);
        ui.labelEdit->setText(match.Label);
        ui.shopURLEdit->setText(match.ShopURL);
        // ui.tagsEdit->setText(match.Tags);
        // ui.commentEdit->setText(match.Comment);
        if (match.Length > 0) ui.lengthEdit->setText(pplib::ToString("%i:%02i", match.Length / 60, match.Length % 60));

        if (match.ShopURL.notEmpty()) {
            pplib::String uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
            pplib::String coverfile = wm_main->conf.SearchListCoverPath + "/" + uuid + ".jpg";
            MetaData meta;
            if (CoverDownload(match.ShopURL, coverfile, meta)) {
                ui.lengthEdit->setText(pplib::ToString("%i:%02i", meta.Length / 60, meta.Length % 60));
                Track.CoverFilename = meta.CoverFile;
                ui.coverwidget->setPixmapFromFile(Track.CoverFilename);
                if (meta.Bpm.notEmpty()) ui.bpmEdit->setText(meta.Bpm);
                if (meta.Key.notEmpty()) ui.keyEdit->setText(meta.Key);
                if (meta.Genre.notEmpty()) ui.genreEdit->setText(meta.Genre);
            }
        }
    }
}

void SearchlistTrackDialog::setFromUrl(const pplib::String& url)
{
    pplib::String uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    pplib::String coverfile = wm_main->conf.SearchListCoverPath + "/" + uuid + ".jpg";
    MetaData meta;
    if (CoverDownload(url, coverfile, meta)) {
        RegExpMatch match;
        match.Album = meta.Album;
        match.Artist = meta.Artist;
        match.Title = meta.Title;
        match.Genre = meta.Genre;
        match.Version = meta.Version;
        match.Bpm = meta.Bpm;
        match.Label = meta.Label;
        match.ReleaseDate = meta.ReleaseDate;
        match.ShopURL = meta.ShopURL;
        match.Length = meta.Length;
        repexpmatch::fixHTML(match);
        repexpmatch::fixIt(match);
        ui.artistEdit->setText(match.Artist);
        ui.titleEdit->setText(match.Title);
        ui.versionEdit->setText(match.Version);
        ui.genreEdit->setText(match.Genre);
        // ui.commentEdit->setText(match.Label);
        ui.releaseDateEdit->setText(match.ReleaseDate);
        ui.labelEdit->setText(match.Label);
        ui.shopURLEdit->setText(match.ShopURL);
        // ui.tagsEdit->setText(match.Tags);
        // ui.commentEdit->setText(match.Comment);
        if (match.Length > 0) ui.lengthEdit->setText(pplib::ToString("%i:%02i", match.Length / 60, match.Length % 60));

        Track.CoverFilename = meta.CoverFile;
        ui.coverwidget->setPixmapFromFile(Track.CoverFilename);
        if (meta.Bpm.notEmpty()) ui.bpmEdit->setText(meta.Bpm);
        if (meta.Key.notEmpty()) ui.keyEdit->setText(meta.Key);
        if (meta.Genre.notEmpty()) ui.genreEdit->setText(meta.Genre);
    }
}

SearchlistItem SearchlistTrackDialog::get() const
{
    SearchlistItem track;
    track.CoverFilename = Track.CoverFilename;
    track.found = Track.found;
    track.selected = Track.selected;
    track = Track; // Bisherige Daten übernehmen, insbesondere Datum der Anlage
    track.Artist = pplib::Trim(pplib::String(ui.artistEdit->text()));
    track.Title = pplib::Trim(pplib::String(ui.titleEdit->text()));
    track.Version = pplib::Trim(pplib::String(ui.versionEdit->text()));
    track.Genre = pplib::Trim(pplib::String(ui.genreEdit->text()));
    track.Comment = pplib::Trim(pplib::String(ui.commentEdit->text()));
    track.Label = pplib::Trim(pplib::String(ui.labelEdit->text()));
    track.ShopURL = pplib::Trim(pplib::String(ui.shopURLEdit->text()));
    track.Tags = pplib::Trim(pplib::String(ui.tagsEdit->text()));
    track.Key = pplib::Trim(pplib::String(ui.keyEdit->text()));
    track.Bpm = pplib::Trim(pplib::String(ui.bpmEdit->text()));
    try {
        track.ReleaseDate.set(pplib::Trim(pplib::String(ui.releaseDateEdit->text())));
    }
    catch (...) {
        track.ReleaseDate.clear();
    }
    track.Rating = ui.rating->currentIndex();
    pplib::String Tmp = pplib::Trim(pplib::String(ui.lengthEdit->text()));
    std::vector<pplib::String> matches;
    if (pplib::RegEx::capture("/^([0-9]+):([0-9]{2})$/", Tmp, matches)) {
        track.Length = matches.at(1).toInt() * 60 + matches.at(2).toInt();
    } else {
        track.Length = 0;
    }
    return track;
}

void SearchlistTrackDialog::on_saveButton_clicked()
{
    done(1);
}

void SearchlistTrackDialog::on_cancelButton_clicked()
{
    done(0);
}

void SearchlistTrackDialog::on_coverwidget_imageChanged(const QPixmap& NewCover)
{
    // pplib::PrintDebug("SearchlistTrackDialog::on_coverwidget_imageChanged\n");
    /*
    Cover = NewCover;
    UpdateCover();
    */
}

void SearchlistTrackDialog::on_coverwidget_imageDeleted()
{
    // pplib::PrintDebug("SearchlistTrackDialog::on_coverwidget_imageDeleted\n");
    /*
    Cover = QPixmap();
    wm->UpdateCoverViewer(Cover);
    if (wm_main->conf.bWriteID3Tags == true) {
        pplib::String Path = wm->GetAudioFilename(DeviceType, DeviceId, Page, TrackNum);
        if (Path.notEmpty()) {
            try {
                pplib::ID3Tag Tag;
                Tag.load(Path);
                Tag.removePicture(3);
                Tag.save();
            }
            catch (const pplib::Exception& exp) {
                ShowException(exp, tr("could not remove cover picture from audio file"));
            }
        }
    }
        */
}