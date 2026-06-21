#ifndef COVERDOWNLOADMANAGER_H
#define COVERDOWNLOADMANAGER_H
#define WITH_QT // Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann
#define PPL_WITH_QT6
#define PPL_QT_STRING_UTF8

#include <QObject>
#include "ppl7.h"
#include <queue>

class DownloadTask
{
public:
    ppl7::String uuid;
    ppl7::String shopUrl;
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

    void addDownload(const ppl7::String& uuid, const ppl7::String& url);

signals:
    void downloadFinished(QString uuid, QString localPath);
    void downloadFailed(QString uuid, QString error);
};

#endif