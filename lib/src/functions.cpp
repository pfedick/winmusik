#include <ppl7.h>
namespace de {
namespace pfp {
namespace winmusik {

static inline void validatebuffer(const ppl7::ByteArrayPtr &buffer, size_t offset, size_t datasize)
{
    if (offset+datasize>buffer.size()) throw ppl7::OverflowException();
}


ppluint8 Peek8(const ppl7::ByteArrayPtr &buffer, size_t offset)
{
    validatebuffer(buffer, offset,1);
    return static_cast<ppluint8>(ppl7::Peek8(static_cast<const unsigned char*>(buffer.ptr())+offset));
}

ppluint16 Peek16(const ppl7::ByteArrayPtr &buffer, size_t offset)
{
    validatebuffer(buffer, offset,2);
    return static_cast<ppluint16>(ppl7::Peek16(static_cast<const unsigned char*>(buffer.ptr())+offset));
}

ppluint32 Peek32(const ppl7::ByteArrayPtr &buffer, size_t offset)
{
    validatebuffer(buffer, offset,4);
    return static_cast<ppluint32>(ppl7::Peek32(static_cast<const unsigned char*>(buffer.ptr())+offset));
}

ppluint64 Peek64(const ppl7::ByteArrayPtr &buffer, size_t offset)
{
    validatebuffer(buffer, offset,8);
    return static_cast<ppluint64>(ppl7::Peek64(static_cast<const unsigned char*>(buffer.ptr())+offset));
}

ppl7::ByteArrayPtr SubBuffer(const ppl7::ByteArrayPtr &buffer, size_t offset, size_t size)
{
    validatebuffer(buffer, offset,size);
    return ppl7::ByteArrayPtr(static_cast<const unsigned char*>(buffer.ptr())+offset,size);
}

ppl7::String PeekString(const ppl7::ByteArrayPtr &buffer, size_t offset, size_t size)
{
    validatebuffer(buffer, offset,size);
    return ppl7::String(static_cast<const char*>(buffer.ptr())+offset,size);
}

}}}	// EOF Namespace de.pfp.winmusik
