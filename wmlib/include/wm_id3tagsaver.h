#ifndef WM_DATAOBJECTS_H
#define WM_DATAOBJECTS_H

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
	int PaddingSize;
	int RetryIntervall;
	void UpdateNow(CID3TagSaver::WorkItem& item);
	void IterateQueue();

public:
	CID3TagSaver();
	~CID3TagSaver();

	void SetPaddingSize(int bytes);
	void SetRetryIntervall(int seconds);
	virtual void run();
	void Add(const ppl7::String& filename, const ppl7::AssocArray& Tags, bool cleartag=false);

};

}	// EOF namespace winmusik
}	// EOF namespace pfp
}	// EOF namespace de

#endif // WM_DATAOBJECTS_H
