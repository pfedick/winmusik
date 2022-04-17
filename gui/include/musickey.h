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

#ifndef MUSICKEY_H
#define MUSICKEY_H

#include <set>
#include <map>
#include "winmusik3.h"
#include "ppl6.h"

int getMusicKeyId(const ppl6::CString &name);
ppl6::CString keyNameSharps(int id);
ppl6::CString keyNameOpenKey(int id);
ppl6::CString keyNameCustom(int id);

enum HarmonicType {
    harmonicSameKey=0,
    harmonicNextKey,			// +1 on wheel
    harmonicPreviousKey,		// -1 on wheel
    harmonicMinorMajorSwitch,
    harmonicMinorMajorJump3,    // from major -3 on wheel to minor
                                // from minor +3 on wheel to major
    harmonicMinorMajorJump1,    // from major +1 on wheel to minor
                                // from minor -1 on wheel to major
    harmonicSemitoneUp,			// +7 on wheel
    harmonicSemitoneDown,		// -7 on wheel
    harmonicTwoSemitoneUp,		// +2 on wheel
    harmonicTwoSemitoneDown,    // -2 on wheel
    harmonicAvbBoost            // +4 on wheel
};

void getHarmonicKeys(std::map<int,HarmonicType> &harmonics, int key);

class MusicKey
{
public:
    enum KeyType {
        unknown=0,
        minor=1,
        major=2
    };

private:
    int ok;
    int wmid;
    KeyType type;

public:
    MusicKey();
    MusicKey(int wm_keyid);
    MusicKey(int openkeyId, KeyType type);
    MusicKey(const ppl6::CString &name);

    void setKey(int wm_keyid);
    void setKey(int openkeyId, KeyType type);
    ppl6::CString name(MusicKeyType type=musicKeyTypeOpenKey) const;
    int wmKeyId() const;

    MusicKey add(int modification) const;
    MusicKey addSemitone(int modification) const;

    MusicKey nextKey() const;
    MusicKey previousKey() const;
    MusicKey minorMajorSwitch() const;
    MusicKey minorMajorJump3() const;
    MusicKey minorMajorJump1() const;
    MusicKey plus1Semitone() const;
    MusicKey minus1Semitone() const;
    MusicKey plus2Semitone() const;
    MusicKey minus2Semitone() const;
    MusicKey avbBoost() const;

    bool operator==(const MusicKey &other) const;
};

class RelatedKeys
{
public:
};


#endif // MUSICKEY_H
