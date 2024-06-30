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
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class WMCoverWidget;
}

class WMCoverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WMCoverWidget(QWidget* parent = nullptr);
    ~WMCoverWidget();

    void setPixmap(const QPixmap& pix);
    void clear();
    const QPixmap& getPixmap() const;

private:
    Ui::WMCoverWidget* ui;
    QPixmap			Cover;
    QPixmap         previousCover;
    QString         LastFilename;
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;

    bool eventFilter(QObject* target, QEvent* event);
    bool handleCoverDragEnterEvent(QDragEnterEvent* event);
    bool handleCoverDropEvent(QDropEvent* event);
    void loadImageFromUri(const QString& uri);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

public slots:
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
