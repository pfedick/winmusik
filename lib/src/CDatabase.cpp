#include "libwinmusik3.h"
#include <map>
namespace de {
namespace pfp {
namespace winmusik {

CDataBase::CDataBase()
{

}

CDataBase::~CDataBase()
{
	clear();
}

void CDataBase::clear()
{

}

class ChunkStats
{
public:
	int count;
	size_t size;
	ChunkStats() {
		count=0;
		size=0;
	}
};

void CDataBase::update(const ppl7::String &oldfile, const ppl7::String &newfile)
{
	ppl7::PFPFile old;
	old.load(oldfile);
	ppl7::PFPFile::Iterator it;
	old.reset(it);
	ppl7::PFPChunk *chunk;
	std::map<ppl7::String,ChunkStats> stats;
	size_t biggest_chunk=0;
	while ((chunk=old.getNext(it))) {
		ChunkStats &cs=stats[chunk->name()];
		cs.count++;
		cs.size+=chunk->size();
		if (chunk->size()>biggest_chunk) biggest_chunk=chunk->size();
	}
	std::map<ppl7::String,ChunkStats>::const_iterator ii;
	size_t records=0;

	for (ii=stats.begin();ii!=stats.end();++ii) {
		printf ("%s: %d Elements, %d Bytes\n",
				(const char*)ii->first,ii->second.count, ii->second.size);
		records+=ii->second.count;
	}
	printf("Header-Overhead fuer %d records: %d Bytes\n",records,records*17);
	printf("Biggest Chunk: %zd Bytes\n", biggest_chunk);
}


}}}	// EOF Namespace de.pfp.winmusik
