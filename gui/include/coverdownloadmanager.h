#ifndef COVERDOWNLOADMANAGER_H
#define COVERDOWNLOADMANAGER_H
#ifndef WITH_QT // Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#define WITH_QT
#endif
#ifndef PPL_WITH_QT6
#define PPL_WITH_QT6
#endif
#ifndef PPL_QT_STRING_UTF8
#define PPL_QT_STRING_UTF8
#endif

#include <QObject>
#include <pplib.h>
#include <queue>

class DownloadTask
{
public:
    pplib::String uuid;
    pplib::String shopUrl;
};

class CoverDownloadManager : public QObject
{
    Q_OBJECT
private:
    std::queue<DownloadTask> queue;
    bool isRunning;

public:
    CoverDownloadManager();
    ~CoverDownloadManager();

    void addDownload(const pplib::String& uuid, const pplib::String& url);

signals:
    void downloadFinished(QString uuid, QString localPath);
    void downloadFailed(QString uuid, QString error);
};

#endif