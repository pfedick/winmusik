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

#ifndef WM_TRAKTOR_H_
#define WM_TRAKTOR_H_
#include <map>
#include <list>
#include <pplib.h>
#include <pplib-audio.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

class TraktorTagFrame
{
public:
    pplib::String name;
    size_t size;
    size_t numChilds;
    const char* data;
    std::map<pplib::String, TraktorTagFrame> childs;
};

/*
 * COM2 = Comment 2
 * TMIX = Mix (taucht im ID3-Tag nicht auf)
 * CUEP = Cuepoints
 *
 * ID3 POPM = TRAKTOR4 "RANK" Rating 1-255, 1=worst, 255=best
 * 0xff=5
 * 0xcc=4
 * 0x99=3
 * 0x66=2
 * 0x33=1
 * 0x00=0
 *
 * LOCK: 00 00 00 00 oder 01 00 00 00
 *

 */

class TraktorTagCue
{
public:
    enum TraktorTagType
    {
        CUE,
        IN,
        OUT,
        LOAD,
        GRID,
        LOOP
    };
    int version;
    pplib::String name;
    int displ_order;
    TraktorTagType type;
    double start;
    double len;
    int repeats;
    int hotcue;

    TraktorTagCue();
    void print() const;
    pplib::String typeName() const;
};

void getTraktorFrames(std::map<pplib::String, TraktorTagFrame>& frames, const char* adr, size_t size);
void getTraktorFrames(std::map<pplib::String, TraktorTagFrame>& frames, const pplib::ID3Tag& Tag);

void getTraktorCues(std::list<TraktorTagCue>& cuelist, const TraktorTagFrame& cuep);
void getTraktorCues(std::list<TraktorTagCue>& cuelist, const pplib::ID3Tag& Tag);
void getTraktorCuesFromFile(std::list<TraktorTagCue>& cuelist, const pplib::String& Filename);

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif /* TRAKTOR_H_ */
