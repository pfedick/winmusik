/*******************************************************************************
 * This file is part of WinMusik 4 by Patrick Fedick
 * Web: https://github.com/pfedick/winmusik
 *
 * Copyright (c) 2026, Patrick Fedick
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
 *******************************************************************************/

#ifndef WM_MUSICALKEYS_H
#define WM_MUSICALKEYS_H

#include <stdlib.h>
#include <stdio.h>

#include <ppl7.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

enum MusicKeyType
{
    musicKeyTypeNone = 0,
    musicKeyTypeMusicalSharps,
    musicKeyTypeOpenKey,
    musicKeyTypeMusicalFlats,
    musicKeyTypeCustom = 99
};

class MusicalKeys
{
private:
    pplib::String customname;
    pplib::String customkey[26];

public:
    void setCustomName(const pplib::String& name);
    const pplib::String& customName() const;
    void setCustomKeyName(int key, const pplib::String& name);

    pplib::String keyName(int id, MusicKeyType type) const;
    int keyId(const pplib::String& name) const;

    static pplib::String sharpName(int id);
    static pplib::String flatName(int id);
    static pplib::String openKeyName(int id);

    static pplib::String ID3TagTkeyNameSharp(int id);
    static pplib::String ID3TagTkeyNameFlat(int id);
};

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif /* WM_MUSICALKEYS_H */
