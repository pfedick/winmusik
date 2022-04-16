#include "libwinmusik3.h"
#include "wm_cwmfile.h"
#include "wm_dataobjects.h"
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

void CDataBase::load(const ppl7::String &filename)
{
    clear();
    CWMFile ff;
    ff.Open(filename);
    CWMFileChunk chunk;
    while (ff.GetNextChunk(chunk)) {
        ppl7::String name=chunk.GetChunkName();
        if (name=="TITL") {
            DataTitle ti(chunk);
        }
    }
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




}}}	// EOF Namespace de.pfp.winmusik
