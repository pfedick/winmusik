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

#ifndef WMCOVERWIDGET_H
#define WMCOVERWIDGET_H

#include <QWidget>
#define PPL_QT_STRING_UTF8
#define WITH_QT // Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#define PPL_WITH_QT6

#include <ppl7-audio.h>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui
{
class WMCoverWidget;
}

class WMCoverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WMCoverWidget(QWidget* parent = nullptr);
    ~WMCoverWidget();

    // TODO: wir brauchen Methoden, über die wir von "außen" das Cover
    // setzen und löschen können, ohne dass die Signals ausgelöst werden.
    // Die Signals sollen nur dann ausgelöst werden, wenn der User das Cover
    // inerhalb des Widgets ändert. Also z.B. über Drag&Drop oder über die Buttons.

    // setter: QPixmap, File, ID3Frame
    void setPixmap(const QPixmap& pix);
    bool setPixmapFromFile(const QString& filename);
    bool setPixmapFromID3Tag(const ppl7::ID3Tag& tag);
    bool setPixmapFromAudioFile(const ppl7::String& filename);
    void clear();
    const QPixmap& getPixmap() const;
    bool hasCover() const;

private:
    Ui::WMCoverWidget* ui;
    QPixmap Cover;
    QPixmap previousCover;
    QString LastFilename;
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
    bool bHaveCover;

    bool eventFilter(QObject* target, QEvent* event);
    bool handleCoverDragEnterEvent(QDragEnterEvent* event);
    bool handleCoverDropEvent(QDropEvent* event);
    void loadImageFromUri(const QString& uri);
    void downloadFromShopUrl(const QString& url);
    void loadFromFile(const QString& filename);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

    // public slots:
    void on_cover_clicked();
    void on_cover_doubleClicked();

    void on_coverCopyButton_clicked();
    void on_coverInsertButton_clicked();
    void on_coverDeleteButton_clicked();
    void on_coverLoadButton_clicked();
    void on_coverSaveButton_clicked();

signals:
    void imageChanged(const QPixmap& Cover);
    void imageDeleted();
    void gotFocus();
};

#endif // WMCOVERWIDGET_H
