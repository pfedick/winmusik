#include "droparea.h"
#include <QString>
#include <QStringList>
#include <QUrl>
#define WITH_QT		// Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#include "ppl6.h"

DropArea::DropArea(QWidget *parent)
     : QLabel(parent)
{
     setAcceptDrops(true);
     //setAutoFillBackground(true);
     clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
 {
     //setBackgroundRole(QPalette::Highlight);

     event->acceptProposedAction();
     emit changed(event->mimeData());
 }

 void DropArea::dragMoveEvent(QDragMoveEvent *event)
 {
     event->acceptProposedAction();
 }

 void DropArea::dropEvent(QDropEvent *event)
 {
     const QMimeData *mimeData = event->mimeData();
     QStringList list=mimeData->formats();
     /*
     for (int i = 0; i < list.size(); ++i) {
    	 ppl6::CString s=list[i];
    	 printf ("drop Event: %s\n",(const char*)s);
     }
     */


     if (mimeData->hasImage()) {

         setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
     }
     if (mimeData->hasHtml()) {
         ppl6::CString t = mimeData->html();
         //printf ("Html: %s\n",(const char*)t);

     }
     if (mimeData->hasText()) {
         ppl6::CString t = mimeData->text();
         //printf ("Text: %s\n",(const char*)t);
     }
     if (mimeData->hasUrls()) {
         QList<QUrl> urlList = mimeData->urls();
         for (int i = 0; i < urlList.size() && i < 32; ++i) {
             ppl6::CString url = urlList.at(i).path();
             //printf ("Url: %s\n",(const char*)url);

         }
     }
     //setBackgroundRole(QPalette::Dark);
     event->acceptProposedAction();
 }

 void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
 {
	 clear();
     event->accept();
 }

 void DropArea::clear()
  {
      //setBackgroundRole(QPalette::Dark);
      emit changed();
  }

