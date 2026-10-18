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

#ifndef WM_REGEXPCAPTURE_H_
#define WM_REGEXPCAPTURE_H_

#include <vector>
#include <list>

#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <ppl7.h>
#include <ppl7-audio.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

class RegExpPattern
{
public:
    RegExpPattern();

    void copyFrom(const RegExpPattern& other);

    pplib::String Name;
    pplib::String Pattern;
    int artist;
    int title;
    int version;
    int genre;
    int label;
    int bpm;
    int album;
    int hours;
    int minutes;
    int seconds;
    int releasedate;
    bool isHTML;
};

class RegExpMatch
{
public:
    RegExpMatch()
    {
        Length = 0;
    }
    pplib::String Artist;
    pplib::String Title;
    pplib::String Version;
    pplib::String Genre;
    pplib::String Label;
    pplib::String Bpm;
    pplib::String Album;
    pplib::String ReleaseDate;
    pplib::String ShopURL;
    int Length; // in Sekunden
};

class RegExpClipboard
{
public:
    // void copyFromClipboard();
    pplib::String PlainText;
    pplib::String Html;
};

namespace repexpmatch
{
bool buildinMatch(const pplib::String& data, RegExpMatch& match);
bool matchBeatPort(const pplib::String& html, RegExpMatch& match);

void fixIt(RegExpMatch& match);
void fixHTML(pplib::String& text);
void fixHTML(RegExpMatch& match);

// for internal use only
bool matchBeatPort2023(const pplib::String& html, RegExpMatch& match);
} // namespace repexpmatch

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif /* WM_REGEXPCAPTURE_H_ */
