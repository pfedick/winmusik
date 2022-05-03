
#define PPL_QT_STRING_UTF8
#define WITH_QT		// Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#include "ppl7.h"
#include "ppl7-inet.h"
#include "wmcoverwidget.h"
#include "ui_wmcoverwidget.h"

#include <QClipboard>
#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPixmap>



#include "winmusik3.h"


WMCoverWidget::WMCoverWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WMCoverWidget)
{
    ui->setupUi(this);
    QPixmap pix(":/cover/resources/cover_placeholder.png");
    ui->cover->setPixmap(pix.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->cover->installEventFilter(this);
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
        SLOT(fileDownloaded(QNetworkReply*)));
}

WMCoverWidget::~WMCoverWidget()
{
    delete ui;
}


bool WMCoverWidget::eventFilter(QObject* target, QEvent* event)
{
    int type=event->type();
    if (target == ui->cover && type == QEvent::DragEnter) {
        //printf("QEvent::DragEnter\n");
        //fflush(stdout);
        return handleCoverDragEnterEvent(static_cast<QDragEnterEvent*>(event));
    } else if (target == ui->cover && type == QEvent::Drop) {
        //printf("QEvent::Drop\n");
        //fflush(stdout);
        return handleCoverDropEvent(static_cast<QDropEvent*>(event));
    } else if (target == ui->cover && type == QEvent::MouseButtonPress) {
        on_cover_clicked();
        return true;
    } else if (target == ui->cover && type == QEvent::MouseButtonDblClick) {
        on_cover_doubleClicked();
        return true;

    }


    return QWidget::eventFilter(target, event);
}

void WMCoverWidget::clear()
{
    Cover=QPixmap();
    QPixmap pix(":/cover/resources/cover_placeholder.png");
    ui->cover->setPixmap(pix.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void WMCoverWidget::setPixmap(const QPixmap& pix)
{
    Cover=pix;
    LastFilename=tr("cover.jpg");
    ui->cover->setPixmap(pix.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

const QPixmap& WMCoverWidget::getPixmap() const
{
    return Cover;
}



void WMCoverWidget::on_cover_clicked()
{
    wm_main->OpenCoverViewer(Cover);
    emit gotFocus();
}

void WMCoverWidget::on_cover_doubleClicked()
{
    on_cover_clicked();
}





void WMCoverWidget::on_coverCopyButton_clicked()
{
    emit gotFocus();
    QClipboard* clipboard = QApplication::clipboard();
    if (!clipboard) return;
    clipboard->setPixmap(Cover);
}

void WMCoverWidget::on_coverInsertButton_clicked()
{
    emit gotFocus();
    QClipboard* clipboard = QApplication::clipboard();
    if (!clipboard) return;
    if (clipboard->pixmap().isNull()) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    Cover=clipboard->pixmap();
    ui->cover->setPixmap(Cover.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QApplication::restoreOverrideCursor();
    emit imageChanged(Cover);
}

void WMCoverWidget::on_coverDeleteButton_clicked()
{
    emit gotFocus();
    if (Cover.isNull()) return;
    if (QMessageBox::question(this, tr("WinMusik: delete MP3-Cover"),
        tr("Do you want to remove the cover from the audio file?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::No) return;
    clear();
    wm_main->UpdateCoverViewer(Cover);
    /*
    if (wm_main->conf.bWriteID3Tags==true) {
        ppl6::CString Path=wm_main->GetAudioFilename(DeviceType,DeviceId,Page,TrackNum);
        if (Path.NotEmpty()) {
            ppl6::CID3Tag Tag;
            Tag.Load(&Path);
            Tag.RemovePicture(3);
            Tag.Save();
        }
    }
    */
    emit imageDeleted();
}

void WMCoverWidget::on_coverLoadButton_clicked()
{
    emit gotFocus();
    ppl7::String Dir=wm_main->conf.LastCoverPath + "/";
    if (Dir.isEmpty()) {
        Dir=QDir::homePath();
    }
    QString newfile = QFileDialog::getOpenFileName(this, tr("Select cover image"),
        Dir,
        tr("Images (*.png *.bmp *.jpg)"));
    if (newfile.isNull()) return;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    wm_main->conf.LastCoverPath=ppl7::File::getPath(newfile);
    wm_main->conf.trySave();
    QPixmap NewCover;
    if (!NewCover.load(newfile)) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error: could not load Cover"),
            tr("The specified file could not be loaded.\nPlease check if the file exists, is readable and contains an image format, which is supported by WinMusik (.png, .jpg or .bmp)")
        );
        emit gotFocus();
        return;
    }
    Cover=NewCover;
    ui->cover->setPixmap(Cover.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QApplication::restoreOverrideCursor();
    emit imageChanged(Cover);
}

void WMCoverWidget::on_coverSaveButton_clicked()
{
    emit gotFocus();
    if (Cover.isNull()) return;
    ppl7::String Dir=wm_main->conf.LastCoverPath + "/";
    Dir+=LastFilename;

    QString newfile = QFileDialog::getSaveFileName(this, tr("Save cover to file"),
        Dir,
        tr("Images (*.png *.bmp *.jpg)"));
    if (newfile.isNull()) return;
    QString blah=ppl7::File::getFilename(newfile);
    LastFilename=blah;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    wm_main->conf.LastCoverPath=ppl7::File::getPath(newfile);
    wm_main->conf.trySave();
    if (!Cover.save(newfile)) {
        /*
         * StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton ) [static]
         *
         */
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error: could not save Cover"),
            tr("The cover of this track could not be saved.\nPlease check if the target directory exists and is writable.\nPlease also check the file extension. WinMusik only supports .png, .jpg and .bmp")
        );
        emit gotFocus();
        return;
    }
    QApplication::restoreOverrideCursor();
}

static void analyzeDragEnter(QDragEnterEvent* event)
{
    const QMimeData* mimedata=event->mimeData();
    printf("Wir haben was bekommen, nur was?\n");
    if (mimedata->hasText()) {
        printf("\n================================\nwir haben text\n");
        ppl7::String text=mimedata->text();
        text.printnl();
    }
    if (mimedata->hasColor()) printf("wir haben color\n");
    if (mimedata->hasHtml()) {
        printf("\n================================\nwir haben html\n");
        ppl7::String text=mimedata->html();
        text.printnl();
    }
    if (mimedata->hasImage()) printf("wir haben image\n");
    if (mimedata->hasUrls()) {
        printf("\n================================\nwir haben urls\n");
        const QList<QUrl>& urllist=mimedata->urls();
        for (int i = 0; i < urllist.size(); ++i) {
            ppl7::String text=urllist.at(i).url();
            text.printnl();

        }
    }
    /*
    QStringList formats=mimedata->formats();
    printf("\n================================\nformatss\n");
    for (int i = 0; i < formats.size(); ++i) {
        ppl7::String text=formats.at(i);
        text.printnl();
        QByteArray ba=mimedata->data(formats.at(i));
        ppl7::HexDump(ba.constData(), ba.size());

    }
wir haben text
https://www.amazon.de/morpho/webapp/#/album/detail/2609137383?context=prime&showHawkfireUpsell=false&id=2609137383&libraryId=2609137383&asin=B08L99DB8D

================================
wir haben html
<html>
<body>
<!--StartFragment--><a data-v-3c011e4b="" href="https://www.amazon.de/morpho/webapp/#/album/detail/2609137383?context=prime&amp;showHawkfireUpsell=false&amp;id=2609137383&amp;libraryId=2609137383&amp;asin=B08L99DB8D" class="entityLink link">Crash (Extended Mix) </a><!--EndFragment-->
</body>
</html>

================================
wir haben urls
https://www.amazon.de/morpho/webapp/#/album/detail/2609137383?context=prime&showHawkfireUpsell=false&id=2609137383&libraryId=2609137383&asin=B08L99DB8D




    */

    fflush(stdout);
}

bool WMCoverWidget::handleCoverDragEnterEvent(QDragEnterEvent* event)
{
    //analyzeDragEnter(event);
    const QMimeData* mimedata=event->mimeData();
    if (mimedata->hasText()) {
        ppl7::String text=mimedata->text();
        if (text.left(7) == "http://" or text.left(8) == "https://") {
            if (text.has(".png") or text.has(".jpg") or text.has(".jpeg") or text.has("amazon")) {
                event->accept();
                return true;
            }
        }
        try {
            ppl7::AssocArray data;
            ppl7::Json::loads(data, text);
            if (data.exists("containerInfo/image")) {
                event->accept();
                return true;
            }
            //data.list();
        } catch (...) {}
        //text.printnl();

    }

    //fflush(stdout);
    //event->accept();
    return false;
}

void WMCoverWidget::loadImageFromUri(const QString& uri)
{
    ppl7::String tmp=uri;
    const QPixmap* tmppix=ui->cover->pixmap();
    previousCover=*tmppix;
    QPixmap pix(":/cover/resources/cover_loading.png");
    ui->cover->setPixmap(pix.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QApplication::processEvents();
    QApplication::processEvents();

    QNetworkRequest request(uri);
    //request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    m_WebCtrl.get(request);

}

/*
 * Amazon Music:
 * Song: https://www.amazon.de/morpho/webapp/#/album/detail/2910154465?context=prime&showHawkfireUpsell=false&id=2910154465&libraryId=2910154465&asin=B08L8677MH
 *       https://www.amazon.de/morpho/webapp/#/album/detail/2910154465?context=prime&showHawkfireUpsell=false&id=2910154465&libraryId=2910154465&asin=B08L8677MH
 * Web: URL=https://m.media-amazon.com/images/I/51r2s5SOUUL._AA256_.jpg
 *
 * ASIN: https://www.amazon.de/dp/B08L853GL3
 *  => Bild: https://m.media-amazon.com/images/I/91emrYbBsML._SS500_.jpg
 *
 * <img alt="If I Spoke Your Language (Extended Mix)" src="https://m.media-amazon.com/images/I/91emrYbBsML._SS500_.jpg">
 * kommt in der Konstellation scheinbar nur einmal vor, pattern: <img alt=".*" src="(https://m.media-amazon.com/images.*\.jpg)">
 */


bool WMCoverWidget::handleCoverDropEvent(QDropEvent* event)
{
    const QMimeData* mimedata=event->mimeData();
    if (mimedata->hasText()) {
        ppl7::String text=mimedata->text();
        //printf("URL=%s\n",(const char*)text);

        try {
            ppl7::AssocArray data;
            ppl7::Json::loads(data, text);
            if (data.exists("containerInfo/image")) {
                loadImageFromUri(data.getString("containerInfo/image"));
                event->accept();
                return true;
            }
        } catch (...) {}
        try {
            ppl7::Array matches;
            if (ppl7::PregMatch("/^(https://www\\.amazon\\..*?)/.*&asin=(.*)$/", text, matches)) {
                ppl7::String amazon=matches[1] + "/dp/" + matches[2];
                loadImageFromUri(amazon);
                /*
                printf ("match: %s\n", (const char*)matches[2]);
                printf("URL: %s\n",(const char*)amazon);
                fflush(stdout);
                */
                event->accept();
                return true;
            } else if (text.has("http://") or text.has("https://")) {
                loadImageFromUri(text);
                event->accept();
                return true;
            }
        } catch (...) {}
    }
    return false;
}

void WMCoverWidget::fileDownloaded(QNetworkReply* pReply)
{
    //printf("download done\n");
    //fflush(stdout);
    if (pReply->error() != QNetworkReply::NoError) {
        ui->cover->setPixmap(previousCover);
        QMessageBox::critical(this, tr("Error: could not load Cover"),
            tr("An error occured, when loading the file.\n\n")
        );
        return;
    }
    ppl7::String url=pReply->request().url().url();
    m_DownloadedData = pReply->readAll();
    pReply->deleteLater();
    if (ppl7::PregMatch("/^https://www.amazon.*?/dp/.*$/", url)) {
        ppl7::String page(m_DownloadedData.constData(), m_DownloadedData.size());
        ppl7::Array matches;
        if (ppl7::PregMatch("/<img alt=\".*\" src=\"(https://m\\.media-amazon\\.com/images.*\\.jpg)\">/", page, matches)) {
            matches[1].printnl();
            QString qurl=matches[1];
            QNetworkRequest request2(qurl);
            m_WebCtrl.get(request2);
            return;
        }
        page.printnl();
        ui->cover->setPixmap(previousCover);
        //fflush(stdout);
        return;
    }

    QPixmap img;
    if (img.loadFromData(m_DownloadedData)) {
        Cover=img;
        ui->cover->setPixmap(Cover.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        emit imageChanged(Cover);
        activateWindow();
        return;
    } else {

    }
    ui->cover->setPixmap(previousCover);
}
