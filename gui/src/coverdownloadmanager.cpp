#include "coverdownloadmanager.h"

CoverDownloadManager::CoverDownloadManager()
    : QObject()
{
    isRunning = false;
}

CoverDownloadManager::~CoverDownloadManager()
{
}

void CoverDownloadManager::addDownload(const pplib::String& uuid, const pplib::String& url)
{
    DownloadTask task;
    task.uuid = uuid;
    task.shopUrl = url;
    queue.push(task);
}
