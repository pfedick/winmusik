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

#include <pplib.h>
#include <pplib-inet.h>
#include "wm_regexpcapture.h"

namespace de
{
namespace pfp
{
namespace winmusik
{
namespace repexpmatch
{

static pplib::String BeatportGetGenreFromId(int id)
{
    switch (id) {
    case 2:
        return pplib::String("Hard Techno");
    case 5:
        return pplib::String("House");
    case 6:
        return pplib::String("Techno (Peak Time / Driving)");
    case 7:
        return pplib::String("Trance");
    case 11:
        return pplib::String("Tech House");
    case 29:
        return pplib::String("Tech Trance");
    case 39:
        return pplib::String("Dance / Electro Pop");
    case 90:
        return pplib::String("Melodic House & Techno");
    case 91:
        return pplib::String("Bass House");
    case 96:
        return pplib::String("Main Stage");
        // case 99: return pplib::String("Deep Trance");

    case 128:
        return pplib::String("Uplifting Trance");
    case 129:
        return pplib::String("Vocal Trance");
    case 245:
        return pplib::String("Big Room");
    case 246:
        return pplib::String("Electro House");
    case 247:
        return pplib::String("Future House");
    case 252:
        return pplib::String("Future Rave");
    case 257:
        return pplib::String("Latin Tech");
    case 264:
        return pplib::String("Raw Trance");
    case 265:
        return pplib::String("Deep Trance");
    case 266:
        return pplib::String("Hypnotic Trance");
    }
    return pplib::String();
}

bool matchBeatPort2023(const pplib::String& html, RegExpMatch& match)
{
    std::vector<pplib::String> matches;
    pplib::String Artist, Title, Version, Genre, Released, Label, ShopURL;
    int found = 0;
    if (pplib::RegEx::capture("/<a title=.*?href=\"(.*?www.beatport.com.*?\\/track\\/.*?)\" .*?>(.*?)<\\/a>/is", html, matches)) {
        pplib::String todo = matches[2];
        ShopURL = matches[1].trimmed();
        // printf("We found track: %s\n", (const char*)todo); fflush(stdout);
        // pplib::PrintDebug("We found track: %s\n", (const char*)matches[1]);
        if (pplib::RegEx::capture("/<span .*?>(.*?)<span>.*?<\\/span>.*?<span .*?>(.*?)<\\/span>.*?<\\/span>/is", todo, matches)) {
            // Beatport 2023 3
            Title = matches[1].trimmed();
            Version = matches[2].trimmed();
            // printf("Match Title: >>%s<<, >>%s<<!\n", (const char*)Title, (const char*)Version);
            found++;
        } else if (pplib::RegEx::capture("/<span .*?>(.*?)<span .*?>(.*?)<\\/span>.*?<\\/span>/is", todo, matches)) {
            Title = matches[1].trimmed();
            Version = matches[2].trimmed();
            // printf("Match Title: >>%s<<, >>%s<<!\n", (const char*)Title, (const char*)Version);
            found++;
        } else if (pplib::RegEx::capture("/^(.*?)<span .*?>(.*?)<\\/span>/is", todo, matches)) {
            // Beatport 2026_2
            Title = matches[1].trimmed();
            Version = matches[2].trimmed();
            found++;
        } else if (!todo.isEmpty()) {
            Title = todo.trimmed();
            found++;
        }
    }
    pplib::Array artists;
    // bool have_artist=false;

    if (pplib::RegEx::capture("/(<div class=.*?cell title.*?<div class=.*?cell label)/is", html, matches)) {
        pplib::String Block = matches[1];
        pplib::Array links(Block, "</a>");
        for (size_t i = 0; i < links.size(); i++) {
            pplib::String row = links[i] + "</a>";
            if (pplib::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?artist\\/.*?>(.*?)</a>/is", row, matches)) {
                // printf("MATCH! >>%s<<\n", (const char*)matches[1]);
                artists.add(matches[1].trimmed());
                // have_artist=true;
            }
        }
    } else if (pplib::RegEx::capture("/(<div class=.*?InfoColumn.*?<div class=.*?CatalogColumn)/is", html, matches)) {
        pplib::String Block = matches[1];
        pplib::Array links(Block, "</a>");
        for (size_t i = 0; i < links.size(); i++) {
            pplib::String row = links[i] + "</a>";
            if (pplib::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?artist\\/.*?>(.*?)</a>/is", row, matches)) {
                artists.add(matches[1].trimmed());
            }
        }
    }

    pplib::Array links(html, "</a>");
    for (size_t i = 0; i < links.size(); i++) {
        pplib::String row = links[i] + "</a>";
        if (pplib::RegEx::capture("/<a title=\"(.*?)\" href=.*?www.beatport.com\\/.*?\\/label\\/.*?<div class.*?>.*?</div>.*?</a>/is", row,
                                  matches)) {
            // Beatport 2025_2
            Label = matches[1].trimmed();

        } else if (pplib::RegEx::capture("/<a title=.*? href=.*?www.beatport.com\\/.*?label\\/.*?<div class.*?>(.*?)</div>.*?</a>/is", row,
                                         matches)) {
            // Beatport 2024_2
            Label = matches[1].trimmed();
        } else if (pplib::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?label\\/.*?<p class.*?>(.*?)</p>.*?</a>/is", row,
                                         matches)) {
            // Beatport 2023 3
            Label = matches[1].trimmed();
        } else if (pplib::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?label\\/.*?>(.*?)</a>/is", row, matches)) {
            Label = matches[1].trimmed();
        }
        if (pplib::RegEx::capture(
                "/<a title=\"(.*?)\" href=.*?www.beatport.com\\/.*?\\/genre\\/.*?\\/([0-9]+)\".*?>.*?<div class.*?>(.*?)</div>.*?</a>/is",
                row, matches)) {
            // Beatport 2025_2
            Genre = BeatportGetGenreFromId(matches[2].toInt());
            if (Genre.isEmpty() || Genre == "Trance") Genre = matches[1].trimmed();
        } else if (pplib::RegEx::capture(
                       "/<a title=.*?href=.*?www.beatport.com\\/.*?genre\\/.*?\\/([0-9]+)\".*?>.*?<div class.*?>(.*?)</div>.*?</a>/is", row,
                       matches)) {
            // Beatport 2024_2
            Genre = BeatportGetGenreFromId(matches[1].toInt());
            if (Genre.isEmpty() || Genre == "Trance") Genre = matches[2].trimmed();
        } else if (pplib::RegEx::capture(
                       "/<a title=.*?href=.*?www.beatport.com\\/.*?genre\\/.*?\\/([0-9]+)\".*?>.*?<p class.*?>(.*?)</p>.*?</a>/is", row,
                       matches)) {
            // Beatport 2023 3
            Genre = BeatportGetGenreFromId(matches[1].toInt());
            if (Genre.isEmpty() || Genre == "Trance") Genre = matches[2].trimmed();
        } else if (pplib::RegEx::capture("/<a title=.*?href=.*?www.beatport.com\\/.*?genre\\/.*?\\/([0-9]+)\".*?>(.*?)</a>/is", row,
                                         matches)) {
            Genre = BeatportGetGenreFromId(matches[1].toInt());
            if (Genre.isEmpty() || Genre == "Trance") Genre = matches[2].trimmed();
        }
    }
    if (pplib::RegEx::capture("/.*\\|.*<span>.*</span>(.*)$/", Genre, matches)) {
        Genre = matches[1].trimmed();
    } else {
        Genre.replace("Trance (Main Floor)", "Trance");
    }
    if (artists.size()) {
        Artist = artists.implode(", ");
        found++;
    }
    if (pplib::RegEx::capture("/<div class=[^>]*DateColumn[^>]*>.*?<div class=[^>]*Text[^>]*>(.*?)<\\/div>/is", html, matches)) {
        try {
            pplib::Date d;
            d.set(matches[1].trimmed());
            Released = d.toString();
        } catch (...) {
            Released = matches[1].trimmed();
        }
    } else if (pplib::RegEx::capture("/<div class=[^>]*date[^>]*>([0-9\\-]+)<\\/div>/is", html, matches)) {
        Released = matches[1].trimmed();
    }

    match.Artist = Artist;
    match.Title = Title;
    match.Genre = Genre;
    match.Label = Label;
    match.ReleaseDate = Released;
    match.Version = Version;
    match.ShopURL = ShopURL;
    fixHTML(match);
    fixIt(match);
    // printf ("matches: %d\n",found);
    if (found > 1) return true;
    return false;
}

bool matchBeatPort(const pplib::String& html, RegExpMatch& match)
{
    if (matchBeatPort2023(html, match)) return true;
    return false;
}

} // namespace repexpmatch

} // namespace winmusik
} // namespace pfp
} // namespace de
