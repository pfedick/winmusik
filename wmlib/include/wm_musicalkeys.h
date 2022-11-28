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

#ifndef WM_MUSICALKEYS_H
#define WM_MUSICALKEYS_H

#include <stdlib.h>
#include <stdio.h>

#include <ppl7.h>

namespace de {
namespace pfp {
namespace winmusik {


enum MusicKeyType
{
	musicKeyTypeNone=0,
	musicKeyTypeMusicalSharps,
	musicKeyTypeOpenKey,
	musicKeyTypeMusicalFlats,
	musicKeyTypeCustom=99
};


class MusicalKeys
{
private:
	ppl7::String customname;
	ppl7::String customkey[26];
public:
	void setCustomName(const ppl7::String& name);
	const ppl7::String& customName() const;
	void setCustomKeyName(int key, const ppl7::String& name);

	ppl7::String keyName(int id, MusicKeyType type) const;
	int keyId(const ppl7::String& name) const;

	static ppl7::String sharpName(int id);
	static ppl7::String flatName(int id);
	static ppl7::String openKeyName(int id);

	static ppl7::String ID3TagTkeyNameSharp(int id);
	static ppl7::String ID3TagTkeyNameFlat(int id);
};



}
}
}	// EOF Namespace de.pfp.winmusik

#endif /* WM_MUSICALKEYS_H */
