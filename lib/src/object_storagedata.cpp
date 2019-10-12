#include "wm_dataobjects.h"
#include "wm_cwmfile.h"

namespace de {
namespace pfp {
namespace winmusik {

CStorageData::CStorageData()
{
    filepos=lastchange=version=0;
}

void CStorageData::copyFrom(const CStorageData &other)
{
    filepos=other.filepos;
    lastchange=other.lastchange;
    version=other.version;
}

void CStorageData::copyFrom(const CWMFileChunk &chunk)
{
    filepos=chunk.GetFilepos();
    lastchange=chunk.GetTimestamp();
    version=chunk.GetVersion();
}

void CStorageData::clear()
{
    filepos=lastchange=version=0;
}

ppluint32 CStorageData::getFilePos() const
{
    return filepos;
}

ppluint32 CStorageData::getLastchange() const
{
    return lastchange;
}

ppluint32 CStorageData::getVersion() const
{
    return version;
}

}}}	// EOF Namespace de.pfp.winmusik
