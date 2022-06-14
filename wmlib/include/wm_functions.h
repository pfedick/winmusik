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

#ifndef WM_FUNCTIONS_H
#define WM_FUNCTIONS_H
#include <ppl7.h>

namespace de {
namespace pfp {
namespace winmusik {

uint8_t Peek8(const ppl7::ByteArrayPtr& buffer, size_t offset);
uint16_t Peek16(const ppl7::ByteArrayPtr& buffer, size_t offset);
uint32_t Peek32(const ppl7::ByteArrayPtr& buffer, size_t offset);
uint64_t Peek64(const ppl7::ByteArrayPtr& buffer, size_t offset);

ppl7::ByteArrayPtr SubBuffer(const ppl7::ByteArrayPtr& buffer, size_t offset, size_t size);
ppl7::String PeekString(const ppl7::ByteArrayPtr& buffer, size_t offset, size_t size);

}
}
}	// EOF Namespace de.pfp.winmusik

#endif // WM_FUNCTIONS_H
