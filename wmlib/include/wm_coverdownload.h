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

#ifndef WM_COVERDOWNLOAD_H
#define WM_COVERDOWNLOAD_H

#include <vector>
#include <list>

#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <pplib.h>
#include <pplib-audio.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

class MetaData
{
public:
    pplib::String Artist;
    pplib::String Title;
    pplib::String Version;
    pplib::String Genre;
    pplib::String Label;
    pplib::String Bpm;
    pplib::String Album;
    pplib::String ReleaseDate;
    pplib::String ShopURL;
    pplib::String Key;
    pplib::String CoverUrl;
    pplib::String CoverFile;

    int Length = 0; // in Sekunden

    void print() const;
};

bool CoverDownload(const pplib::String& url, const pplib::String& targetfile);
bool CoverDownload(const pplib::String& url, const pplib::String& targetfile, MetaData& data);

void SetBeatportSession(const pplib::String& clearance, const pplib::String& userAgent);

bool CoverDownload_Beatport(const pplib::String& url, const pplib::String& targetfile);
bool CoverDownload_Beatport(const pplib::String& url, const pplib::String& targetfile, MetaData& data);

void getBeatportMetaData(pplib::AssocArray& data, de::pfp::winmusik::MetaData& meta);

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif // WM_COVERDOWNLOAD_H