/*******************************************************************************
 * This file is part of WinMusik 4 by Patrick Fedick
 * Web: https://github.com/pfedick/winmusik
 *
 * Copyright (c) 2026, Patrick Fedick
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
 *******************************************************************************/

#ifndef WM_ID3TAGSAVER_H
#define WM_ID3TAGSAVER_H

#include <list>
#include <pplib/core/thread.h>
#include <pplib/core/time.h>
#include <pplib/core/logging.h>
#include <pplib/types/string.h>
#include <pplib/types/assocarray.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

class CID3TagSaver : public pplib::Thread
{
private:
    class WorkItem
    {
    public:
        pplib::String Filename;
        pplib::AssocArray Tags;
        pplib::ppl_time_t retry;
        int retry_counter;
        bool cleartag;
        WorkItem();
        WorkItem(const pplib::String& filename, const pplib::AssocArray& Tags, bool cleartag = false);
    };
    pplib::Mutex Mutex;
    std::list<WorkItem> Queue;
    pplib::Logger* logger;
    int PaddingSize;
    int RetryIntervall;
    void UpdateNow(CID3TagSaver::WorkItem& item);
    void IterateQueue();

public:
    CID3TagSaver();
    ~CID3TagSaver();
    void SetLogger(pplib::Logger* logger);

    void SetPaddingSize(int bytes);
    void SetRetryIntervall(int seconds);
    virtual void run();
    void Add(const pplib::String& filename, const pplib::AssocArray& Tags, bool cleartag = false);
};

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif // WM_ID3TAGSAVER_H
