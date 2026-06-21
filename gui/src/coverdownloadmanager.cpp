#include "coverdownloadmanager.h"

CoverDownloadManager::CoverDownloadManager()
    : QObject()
{
    isRunning = false;
}

CoverDownloadManager::~CoverDownloadManager()
{
}

void CoverDownloadManager::addDownload(const ppl7::String& uuid, const ppl7::String& url)
{
    DownloadTask task;
    task.uuid = uuid;
    task.shopUrl = url;
    queue.push(task);
}
