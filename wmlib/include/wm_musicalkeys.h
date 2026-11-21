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

#include <cstdint>
#include <stdlib.h>
#include <stdio.h>

#include <pplib.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

enum class MusicalKey : uint8_t
{
    Unknown = 0,
    GSharpMinor = 1,
    AFlatMinor = GSharpMinor,
    B = 2,
    DSharpMinor = 3,
    EFlatMinor = DSharpMinor,
    FSharp = 4,
    GFlat = FSharp,
    ASharpMinor = 5,
    BFlatMinor = ASharpMinor,
    CSharp = 6,
    DFlat = CSharp,
    FMinor = 7,
    GSharp = 8,
    AFlat = GSharp,
    CMinor = 9,
    DSharp = 10,
    EFlat = DSharp,
    GMinor = 11,
    ASharp = 12,
    BFlat = ASharp,
    DMinor = 13,
    F = 14,
    AMinor = 15,
    C = 16,
    EMinor = 17,
    G = 18,
    BMinor = 19,
    D = 20,
    FSharpMinor = 21,
    GFlatMinor = FSharpMinor,
    A = 22,
    CSharpMinor = 23,
    DFlatMinor = CSharpMinor,
    E = 24,
    None = 25
};

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
    pplib::String keyName(MusicalKey key, MusicKeyType type) const;
    int keyId(const pplib::String& name) const;
    MusicalKey key(const pplib::String& name) const;

    static constexpr MusicalKey toMusicalKey(int id) noexcept
    {
        if (id < 0 || id > 25) return MusicalKey::Unknown;
        return static_cast<MusicalKey>(id);
    }

    static constexpr int toInt(MusicalKey key) noexcept
    {
        return static_cast<int>(key);
    }

    static pplib::String sharpName(int id);
    static pplib::String sharpName(MusicalKey key);
    static pplib::String flatName(int id);
    static pplib::String flatName(MusicalKey key);
    static pplib::String openKeyName(int id);
    static pplib::String openKeyName(MusicalKey key);

    static pplib::String ID3TagTkeyNameSharp(int id);
    static pplib::String ID3TagTkeyNameSharp(MusicalKey key);
    static pplib::String ID3TagTkeyNameFlat(int id);
    static pplib::String ID3TagTkeyNameFlat(MusicalKey key);
};

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif /* WM_MUSICALKEYS_H */
