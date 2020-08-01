#define PPL_QT_STRING_UTF8
#define WITH_QT		// Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#include <ppl7.h>
#include <ppl7-inet.h>
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


WMCoverWidget::WMCoverWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WMCoverWidget)
{
    ui->setupUi(this);
    QPixmap pix(":/cover/resources/cover_placeholder.png");
    ui->cover->setPixmap(pix.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->cover->installEventFilter(this);

}

WMCoverWidget::~WMCoverWidget()
{
    delete ui;
}


bool WMCoverWidget::eventFilter(QObject *target, QEvent *event)
{
    int type=event->type();
    if (target==ui->cover && type==QEvent::DragEnter) {
        //printf("QEvent::DragEnter\n");
        //fflush(stdout);
        return handleCoverDragEnterEvent(static_cast<QDragEnterEvent *>(event));
    } else if (target==ui->cover && type==QEvent::Drop) {
        //printf("QEvent::Drop\n");
        //fflush(stdout);
        return handleCoverDropEvent(static_cast<QDropEvent *>(event));
    } else if (target==ui->cover && type==QEvent::MouseButtonPress) {
        on_cover_clicked();
        return true;
    } else if (target==ui->cover && type==QEvent::MouseButtonDblClick) {
        on_cover_doubleClicked();
        return true;

    }


    return QWidget::eventFilter(target,event);
}

void WMCoverWidget::clear()
{
    Cover=QPixmap();
    QPixmap pix(":/cover/resources/cover_placeholder.png");
    ui->cover->setPixmap(pix.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void WMCoverWidget::setPixmap(const QPixmap &pix)
{
    Cover=pix;
    LastFilename=tr("cover.jpg");
    ui->cover->setPixmap(pix.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

const QPixmap &WMCoverWidget::getPixmap() const
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
    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard) return;
    clipboard->setPixmap(Cover);
}

void WMCoverWidget::on_coverInsertButton_clicked()
{
    emit gotFocus();
    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard) return;
    if (clipboard->pixmap().isNull()) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    Cover=clipboard->pixmap();
    ui->cover->setPixmap(Cover.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    QApplication::restoreOverrideCursor();
    emit imageChanged(Cover);
}

void WMCoverWidget::on_coverDeleteButton_clicked()
{
    emit gotFocus();
    if (Cover.isNull()) return;
    if (QMessageBox::question(this, tr("WinMusik: delete MP3-Cover"),
        tr("Do you want to remove the cover from the audio file?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)
        ==QMessageBox::No) return;
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
    ppl6::CString Dir=wm_main->conf.LastCoverPath+"/";
    if (Dir.IsEmpty()) {
        Dir=QDir::homePath();
    }
    QString newfile = QFileDialog::getOpenFileName(this, tr("Select cover image"),
            Dir,
            tr("Images (*.png *.bmp *.jpg)"));
    if (newfile.isNull()) return;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    wm_main->conf.LastCoverPath=ppl6::GetPath(newfile);
    wm_main->conf.Save();
    QPixmap NewCover;
    if (!NewCover.load(newfile)) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this,tr("Error: could not load Cover"),
                tr("The specified file could not be loaded.\nPlease check if the file exists, is readable and contains an image format, which is supported by WinMusik (.png, .jpg or .bmp)")
                );
        emit gotFocus();
        return;
    }
    Cover=NewCover;
    ui->cover->setPixmap(Cover.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    QApplication::restoreOverrideCursor();
    emit imageChanged(Cover);
}

void WMCoverWidget::on_coverSaveButton_clicked()
{
    emit gotFocus();
    if (Cover.isNull()) return;
    ppl6::CString Dir=wm_main->conf.LastCoverPath+"/";
    Dir+=LastFilename;

    QString newfile = QFileDialog::getSaveFileName(this, tr("Save cover to file"),
                Dir,
                tr("Images (*.png *.bmp *.jpg)"));
    if (newfile.isNull()) return;
    QString blah=ppl7::File::getFilename(newfile);
    LastFilename=blah;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    wm_main->conf.LastCoverPath=ppl6::GetPath(newfile);
    wm_main->conf.Save();
    if (!Cover.save (newfile)) {
        /*
         * StandardButton QMessageBox::critical ( QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton ) [static]
         *
         */
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this,tr("Error: could not save Cover"),
                tr("The cover of this track could not be saved.\nPlease check if the target directory exists and is writable.\nPlease also check the file extension. WinMusik only supports .png, .jpg and .bmp")
                );
        emit gotFocus();
        return;
    }
    QApplication::restoreOverrideCursor();
}

bool WMCoverWidget::handleCoverDragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimedata=event->mimeData();
    if (mimedata->hasText()) {
        ppl7::String text=mimedata->text();
        if(text.left(7)=="http://" or text.left(8)=="https://") {
            if (text.has(".png") or text.has(".jpg") or text.has(".jpeg")) {
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

bool WMCoverWidget::loadImageFromUri(const QString &uri)
{
    ppl7::String tmp=uri;
    QPixmap previous= ui->cover->pixmap(Qt::ReturnByValue);
    QPixmap pix(":/cover/resources/cover_loading.png");
    ui->cover->setPixmap(pix.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    QApplication::processEvents();
    QApplication::processEvents();
    //tmp.printnl();
    ppl7::Curl curl;
    try {
        curl.setURL(uri);
        curl.get();
    } catch (const ppl7::Exception &exp) {
        QString error=exp.toString();
        QMessageBox::critical(this,tr("Error: could not load Cover"),
                tr("An error occured, when loading the file.\n\n")+error
                );
        ui->cover->setPixmap(previous);
        return false;
    }
    ppl7::ByteArrayPtr baptr=curl.getResultBuffer();
    QPixmap img;
    if (img.loadFromData((const uchar*)baptr.ptr(), (int)baptr.size())) {
        Cover=img;
        ui->cover->setPixmap(Cover.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        emit imageChanged(Cover);
        activateWindow();
        return true;
    }
    ui->cover->setPixmap(previous);
    return false;
}

bool WMCoverWidget::handleCoverDropEvent(QDropEvent *event)
{
    const QMimeData *mimedata=event->mimeData();
    if (mimedata->hasText()) {
        ppl7::String text=mimedata->text();
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        try {
            ppl7::AssocArray data;
            ppl7::Json::loads(data, text);
            if (data.exists("containerInfo/image")) {
                if (loadImageFromUri(data.getString("containerInfo/image"))) {
                    QApplication::restoreOverrideCursor();
                    event->accept();
                    return true;
                }
            }
        } catch (...) {}
        try {
            if (text.has("http://") or text.has("https://")) {
                if (loadImageFromUri(text)) {
                    QApplication::restoreOverrideCursor();
                    event->accept();
                    return true;
                }
            }
        } catch (...) {}
        QApplication::restoreOverrideCursor();
    }
    return false;
}


