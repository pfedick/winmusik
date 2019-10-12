#ifndef WM_FUNCTIONS_H
#define WM_FUNCTIONS_H
#include <ppl7.h>
namespace de {
namespace pfp {
namespace winmusik {

ppluint8 Peek8(const ppl7::ByteArrayPtr &buffer, size_t offset);
ppluint16 Peek16(const ppl7::ByteArrayPtr &buffer, size_t offset);
ppluint32 Peek32(const ppl7::ByteArrayPtr &buffer, size_t offset);
ppluint64 Peek64(const ppl7::ByteArrayPtr &buffer, size_t offset);

ppl7::ByteArrayPtr SubBuffer(const ppl7::ByteArrayPtr &buffer, size_t offset, size_t size);
ppl7::String PeekString(const ppl7::ByteArrayPtr &buffer, size_t offset, size_t size);

}}}	// EOF Namespace de.pfp.winmusik

#endif // WM_FUNCTIONS_H
