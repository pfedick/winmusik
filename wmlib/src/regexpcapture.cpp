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

namespace de
{
namespace pfp
{
namespace winmusik
{

RegExpPattern::RegExpPattern()
{
    artist = 0;
    title = 0;
    version = 0;
    genre = 0;
    label = 0;
    bpm = 0;
    album = 0;
    hours = 0;
    minutes = 0;
    seconds = 0;
    releasedate = 0;
    isHTML = false;
}

void RegExpPattern::copyFrom(const RegExpPattern& other)
{
    Name = other.Name;
    Pattern = other.Pattern;
    artist = other.artist;
    title = other.title;
    version = other.version;
    genre = other.genre;
    label = other.label;
    bpm = other.bpm;
    album = other.album;
    hours = other.hours;
    minutes = other.minutes;
    seconds = other.seconds;
    releasedate = other.releasedate;
    isHTML = other.isHTML;
}

namespace repexpmatch
{

static void fixArtistAndTitle(const pplib::String& prefix, RegExpMatch& match, const pplib::String prefix_replace)
{
    pplib::String regex1 = "/(.*?)\\s" + prefix + "\\s+(.*)$/i";
    pplib::String regex2 = "/(.*?)\\(" + prefix + "\\s+(.*)\\)$/i";
    std::vector<pplib::String> Matches;
    if (pplib::RegEx::capture(regex1, match.Title, Matches) > 0 || pplib::RegEx::capture(regex2, match.Title, Matches) > 0) {
        pplib::String feat = Matches.at(2);
        feat.trim();
        match.Title = Matches[1];
        match.Artist.replace(feat, "");
        match.Artist.trim();
        match.Artist.trim(",");
        match.Artist.replace(",,", ",");
        match.Artist += " " + prefix_replace + " " + Matches.at(2);
    }
}

void fixIt(RegExpMatch& match)
{
    // ppl6::CArray Matches;
    fixArtistAndTitle("feat\\.", match, "feat.");
    fixArtistAndTitle("featuring", match, "feat.");
    fixArtistAndTitle("ft", match, "feat.");
    fixArtistAndTitle("ft.", match, "feat.");
    fixArtistAndTitle("pres\\.", match, "pres.");
    fixArtistAndTitle("presents", match, "pres.");
}

void fixHTML(pplib::String& text)
{
    text = pplib::UnescapeHTMLTags(text);
}

void fixHTML(RegExpMatch& match)
{
    fixHTML(match.Artist);
    fixHTML(match.Title);
    fixHTML(match.Version);
    fixHTML(match.Genre);
    fixHTML(match.Label);
    fixHTML(match.ShopURL);
    fixHTML(match.Bpm);
    fixHTML(match.Album);
    fixHTML(match.ReleaseDate);
}

bool buildinMatch(const pplib::String& data, RegExpMatch& match)
{
    std::vector<pplib::String> Matches;
    if (pplib::RegEx::capture("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/", data, Matches)) {
        // printf ("Match ASOT\n");
        match.Artist = pplib::Trim(Matches[1]);
        match.Title = pplib::Trim(Matches[2]);
        match.Version = pplib::Trim(Matches[3]);
    } else if (pplib::RegEx::capture("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/", data, Matches)) {
        match.Artist = pplib::Trim(Matches[3]);
        match.Title = pplib::Trim(Matches[1]);
        match.Version = pplib::Trim(Matches[2]);
    } else if (pplib::RegEx::capture("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/", data, Matches)) {
        match.Artist = pplib::Trim(Matches[3]);
        match.Title = pplib::Trim(Matches[1]);
        match.Version = pplib::Trim(Matches[2]);
    } else if (pplib::RegEx::capture("/^(.*?)\\-(.*?)\\((.*?)\\).*$/", data, Matches)) {
        match.Artist = pplib::Trim(Matches[1]);
        match.Title = pplib::Trim(Matches[2]);
        match.Version = pplib::Trim(Matches[3]);
    } else if (pplib::RegEx::capture("/^(.*?)\\-(.*?)$/", data, Matches)) {
        match.Artist = pplib::Trim(Matches[1]);
        match.Title = pplib::Trim(Matches[2]);
    } else {
        return false;
    }
    if (pplib::RegEx::capture("/(.*?)(\\sfeat\\..*)$/i", match.Title, Matches)) {
        match.Title = Matches[1].trimmed();
        match.Artist += Matches[2].trimmed();
    } else if (pplib::RegEx::capture("/(.*?)(\\sfeaturing.*)$/i", match.Title, Matches)) {
        match.Title = Matches[1].trimmed();
        match.Artist += Matches[2].trimmed();
    } else if (pplib::RegEx::capture("/(.*?)(\\spres\\..*)$/i", match.Title, Matches)) {
        match.Title = Matches[1].trimmed();
        match.Artist += Matches[2].trimmed();
    } else if (pplib::RegEx::capture("/(.*?)(\\spresents.*)$/i", match.Title, Matches)) {
        match.Title = Matches[1].trimmed();
        match.Artist += Matches[2].trimmed();
    }
    fixIt(match);
    return true;
}

} // namespace repexpmatch

} // namespace winmusik
} // namespace pfp
} // namespace de
