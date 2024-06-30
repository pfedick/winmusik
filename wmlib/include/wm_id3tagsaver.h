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

#ifndef WM_ID3TAGSAVER_H
#define WM_ID3TAGSAVER_H

#include <list>
#include <ppl7.h>

namespace de {
namespace pfp {
namespace winmusik {

class CID3TagSaver : public ppl7::Thread
{
private:
	class WorkItem
	{
	public:
		ppl7::String Filename;
		ppl7::AssocArray Tags;
		ppl7::ppl_time_t retry;
		int retry_counter;
		bool cleartag;
		WorkItem();
		WorkItem(const ppl7::String& filename, const ppl7::AssocArray& Tags, bool cleartag=false);
	};
	ppl7::Mutex Mutex;
	std::list<WorkItem> Queue;
	ppl7::Logger* logger;
	int PaddingSize;
	int RetryIntervall;
	void UpdateNow(CID3TagSaver::WorkItem& item);
	void IterateQueue();

public:
	CID3TagSaver();
	~CID3TagSaver();
	void SetLogger(ppl7::Logger* logger);

	void SetPaddingSize(int bytes);
	void SetRetryIntervall(int seconds);
	virtual void run();
	void Add(const ppl7::String& filename, const ppl7::AssocArray& Tags, bool cleartag=false);

};

}	// EOF namespace winmusik
}	// EOF namespace pfp
}	// EOF namespace de

#endif // WM_ID3TAGSAVER_H
