/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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

#include "ppl7.h"
namespace de {
namespace pfp {
namespace winmusik {

static inline void validatebuffer(const ppl7::ByteArrayPtr& buffer, size_t offset, size_t datasize)
{
    if (offset + datasize > buffer.size()) throw ppl7::OverflowException();
}


uint8_t Peek8(const ppl7::ByteArrayPtr& buffer, size_t offset)
{
    validatebuffer(buffer, offset, 1);
    return static_cast<uint8_t>(ppl7::Peek8(static_cast<const unsigned char*>(buffer.ptr()) + offset));
}

uint16_t Peek16(const ppl7::ByteArrayPtr& buffer, size_t offset)
{
    validatebuffer(buffer, offset, 2);
    return static_cast<uint16_t>(ppl7::Peek16(static_cast<const unsigned char*>(buffer.ptr()) + offset));
}

uint32_t Peek32(const ppl7::ByteArrayPtr& buffer, size_t offset)
{
    validatebuffer(buffer, offset, 4);
    return static_cast<uint32_t>(ppl7::Peek32(static_cast<const unsigned char*>(buffer.ptr()) + offset));
}

uint64_t Peek64(const ppl7::ByteArrayPtr& buffer, size_t offset)
{
    validatebuffer(buffer, offset, 8);
    return static_cast<uint64_t>(ppl7::Peek64(static_cast<const unsigned char*>(buffer.ptr()) + offset));
}

ppl7::ByteArrayPtr SubBuffer(const ppl7::ByteArrayPtr& buffer, size_t offset, size_t size)
{
    validatebuffer(buffer, offset, size);
    return ppl7::ByteArrayPtr(static_cast<const unsigned char*>(buffer.ptr()) + offset, size);
}

ppl7::String PeekString(const ppl7::ByteArrayPtr& buffer, size_t offset, size_t size)
{
    validatebuffer(buffer, offset, size);
    return ppl7::String(static_cast<const char*>(buffer.ptr()) + offset, size);
}

}
}
}	// EOF Namespace de.pfp.winmusik
