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

#include <ppl7.h>
#include "wm_regexpcapture.h"

namespace de {
namespace pfp {
namespace winmusik {
namespace repexpmatch {


static ppl7::String BeatportGetGenreFromId(int id)
{
    switch (id) {
        case 2: return ppl7::String("Hard Techno");
        case 5: return ppl7::String("House");
        case 6: return ppl7::String("Techno (Peak Time / Driving)");
        case 7: return ppl7::String("Trance");
        case 11: return ppl7::String("Tech House");
        case 29: return ppl7::String("Tech Trance");
        case 39: return ppl7::String("Dance / Electro Pop");
        case 90: return ppl7::String("Melodic House & Techno");
        case 91: return ppl7::String("Bass House");
        case 96: return ppl7::String("Main Stage");
        //case 99: return ppl7::String("Deep Trance");

        case 128: return ppl7::String("Uplifting Trance");
        case 129: return ppl7::String("Vocal Trance");
        case 245: return ppl7::String("Big Room");
        case 246: return ppl7::String("Electro House");
        case 247: return ppl7::String("Future House");
        case 252: return ppl7::String("Future Rave");
        case 257: return ppl7::String("Latin Tech");
        case 264: return ppl7::String("Raw Trance");
        case 265: return ppl7::String("Deep Trance");
        case 266: return ppl7::String("Hypnotic Trance");


    }
    return ppl7::String();
}

bool matchBeatPort2023(const ppl7::String& html, RegExpMatch& match)
{
    std::vector<ppl7::String> matches;
    ppl7::String Artist, Title, Version, Genre, Released, Label;
    int found=0;
    if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com.*?\\/track\\/.*?>(.*?)<\\/a>/is", html, matches)) {
        ppl7::String todo=matches[1];
        //printf("We found track: %s\n", (const char*)todo); fflush(stdout);
        if (ppl7::RegEx::capture("/<span .*?>(.*?)<span>.*?<\\/span>.*?<span .*?>(.*?)<\\/span>.*?<\\/span>/is", todo, matches)) {
            // Beatport 2023 3
            Title=matches[1].trimmed();
            Version=matches[2].trimmed();
            //printf("Match Title: >>%s<<, >>%s<<!\n", (const char*)Title, (const char*)Version);
            found++;
        } else if (ppl7::RegEx::capture("/<span .*?>(.*?)<span .*?>(.*?)<\\/span>.*?<\\/span>/is", todo, matches)) {
            Title=matches[1].trimmed();
            Version=matches[2].trimmed();
            //printf("Match Title: >>%s<<, >>%s<<!\n", (const char*)Title, (const char*)Version);
            found++;
        }

    }
    ppl7::Array artists;
    //bool have_artist=false;

    if (ppl7::RegEx::capture("/(<div class=.*?cell title.*?<div class=.*?cell label)/is", html, matches)) {
        ppl7::String Block=matches[1];
        ppl7::Array links(Block, "</a>");
        for (size_t i=0;i < links.size();i++) {
            ppl7::String row=links[i] + "</a>";
            if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?artist\\/.*?>(.*?)</a>/is", row, matches)) {
                //printf("MATCH! >>%s<<\n", (const char*)matches[1]);
                artists.add(matches[1].trimmed());
                //have_artist=true;
            }
        }

    }


    ppl7::Array links(html, "</a>");
    for (size_t i=0;i < links.size();i++) {
        ppl7::String row=links[i] + "</a>";
        if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?label\\/.*?<div class.*?>(.*?)</div>.*?</a>/is", row, matches)) {
            // Beatport 2024_2
            Label=matches[1].trimmed();
        } else if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?label\\/.*?<p class.*?>(.*?)</p>.*?</a>/is", row, matches)) {
            // Beatport 2023 3
            Label=matches[1].trimmed();
        } else if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?label\\/.*?>(.*?)</a>/is", row, matches)) {
            Label=matches[1].trimmed();
        }
        if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?genre\\/.*?\\/([0-9]+)\".*?>.*?<div class.*?>(.*?)</div>.*?</a>/is", row, matches)) {
            // Beatport 2024_2
            Genre=BeatportGetGenreFromId(matches[1].toInt());
            if (Genre.isEmpty() || Genre == "Trance") Genre=matches[2].trimmed();
        } else if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?genre\\/.*?\\/([0-9]+)\".*?>.*?<p class.*?>(.*?)</p>.*?</a>/is", row, matches)) {
            // Beatport 2023 3
            Genre=BeatportGetGenreFromId(matches[1].toInt());
            if (Genre.isEmpty() || Genre == "Trance") Genre=matches[2].trimmed();
        } else if (ppl7::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?genre\\/.*?\\/([0-9]+)\".*?>(.*?)</a>/is", row, matches)) {
            Genre=BeatportGetGenreFromId(matches[1].toInt());
            if (Genre.isEmpty()) Genre=matches[2].trimmed();
        }
    }
    if (ppl7::RegEx::capture("/.*\\|.*<span>.*</span>(.*)$/", Genre, matches)) {
        Genre=matches[1].trimmed();
    } else {
        Genre.replace("Trance (Main Floor)", "Trance");
    }
    if (artists.size()) {
        Artist=artists.implode(", ");
        found++;
    }
    if (ppl7::RegEx::capture("/<div class.*?date.*?>([0-9\\-]+)</div>/is", html, matches)) {
        Released=matches[1].trimmed();
    }



    match.Artist=Artist;
    match.Title=Title;
    match.Genre=Genre;
    match.Label=Label;
    match.ReleaseDate=Released;
    match.Version=Version;
    fixHTML(match);
    fixIt(match);
    //printf ("matches: %d\n",found);
    if (found > 1) return true;
    return false;

}


bool matchBeatPort(const ppl7::String& html, RegExpMatch& match)
{
    if (matchBeatPort2023(html, match)) return true;
    return false;

}

} // EOF namespace repexpmatch


} // EOF namespace winkusik
} // EOF namespace pfp
} // EOF namespace de
