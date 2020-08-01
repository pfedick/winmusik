#ifndef WMCOVERWIDGET_H
#define WMCOVERWIDGET_H

#include <QWidget>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
class WMCoverWidget;
}

class WMCoverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WMCoverWidget(QWidget *parent = nullptr);
    ~WMCoverWidget();

    void setPixmap(const QPixmap &pix);
    void clear();
    const QPixmap &getPixmap() const;

private:
    Ui::WMCoverWidget *ui;
    QPixmap			Cover;
    QString         LastFilename;

    bool eventFilter(QObject *target, QEvent *event);
    bool handleCoverDragEnterEvent(QDragEnterEvent *event);
    bool handleCoverDropEvent(QDropEvent *event);
    bool loadImageFromUri(const QString &uri);


public slots:
    void on_cover_clicked();
    void on_cover_doubleClicked();

    void on_coverCopyButton_clicked();
	void on_coverInsertButton_clicked();
	void on_coverDeleteButton_clicked();
	void on_coverLoadButton_clicked();
	void on_coverSaveButton_clicked();

signals:
     void imageChanged(const QPixmap &Cover);
     void imageDeleted();
     void gotFocus();
};

#endif // WMCOVERWIDGET_H
