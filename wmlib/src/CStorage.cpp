#include "libwinmusik3.h"

namespace de {
namespace pfp {
namespace winmusik {

CStorage::CStorage()
{

}

CStorage::~CStorage()
{

}


void CStorage::loadDatabase(const ppl7::String &filename, CDataBase &db)
{
	ppl7::File ff(filename);
	db.clear();
}


}}}	// EOF Namespace de.pfp.winmusik
