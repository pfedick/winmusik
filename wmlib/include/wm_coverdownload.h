/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

#ifndef WM_COVERDOWNLOAD_H
#define WM_COVERDOWNLOAD_H

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

class MetaData
{
public:
    ppl7::String Artist;
    ppl7::String Title;
    ppl7::String Version;
    ppl7::String Genre;
    ppl7::String Label;
    ppl7::String Bpm;
    ppl7::String Album;
    ppl7::String ReleaseDate;
    ppl7::String ShopURL;
    ppl7::String Key;
    ppl7::String CoverUrl;
    ppl7::String CoverFile;

    int Length = 0; // in Sekunden

    void print() const;
};

bool CoverDownload(const ppl7::String& url, const ppl7::String& targetfile);
bool CoverDownload(const ppl7::String& url, const ppl7::String& targetfile, MetaData& data);

bool CoverDownload_Beatport(const ppl7::String& url, const ppl7::String& targetfile);
bool CoverDownload_Beatport(const ppl7::String& url, const ppl7::String& targetfile, MetaData& data);

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif // WM_COVERDOWNLOAD_H