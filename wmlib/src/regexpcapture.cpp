/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2023 Patrick Fedick
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



RegExpPattern::RegExpPattern()
{
	artist=0;
	title=0;
	version=0;
	genre=0;
	label=0;
	bpm=0;
	album=0;
	hours=0;
	minutes=0;
	seconds=0;
	releasedate=0;
	isHTML=false;
}

void RegExpPattern::copyFrom(const RegExpPattern& other)
{
	Name=other.Name;
	Pattern=other.Pattern;
	artist=other.artist;
	title=other.title;
	version=other.version;
	genre=other.genre;
	label=other.label;
	bpm=other.bpm;
	album=other.album;
	hours=other.hours;
	minutes=other.minutes;
	seconds=other.seconds;
	releasedate=other.releasedate;
	isHTML=other.isHTML;
}

namespace repexpmatch {

static void fixArtistAndTitle(const ppl7::String& prefix, RegExpMatch& match, const ppl7::String prefix_replace)
{
	ppl7::String regex1="/(.*?)\\s" + prefix + "\\s+(.*)$/i";
	ppl7::String regex2="/(.*?)\\(" + prefix + "\\s+(.*)\\)$/i";
	ppl7::Array Matches;
	if (match.Title.pregMatch(regex1, Matches) > 0 || match.Title.pregMatch(regex2, Matches) > 0) {
		ppl7::String feat=Matches.get(2);
		feat.trim();
		match.Title=Matches[1];
		match.Artist.replace(feat, "");
		match.Artist.trim();
		match.Artist.trim(",");
		match.Artist.replace(",,", ",");
		match.Artist+=" " + prefix_replace + " " + Matches.get(2);
	}
}

void fixIt(RegExpMatch& match)
{
	//ppl6::CArray Matches;
	fixArtistAndTitle("feat\\.", match, "feat.");
	fixArtistAndTitle("featuring", match, "feat.");
	fixArtistAndTitle("ft", match, "feat.");
	fixArtistAndTitle("ft.", match, "feat.");
	fixArtistAndTitle("pres\\.", match, "pres.");
	fixArtistAndTitle("presents", match, "pres.");
}

void fixHTML(ppl7::String& text)
{
	text=ppl7::UnescapeHTMLTags(text);
}

void fixHTML(RegExpMatch& match)
{
	fixHTML(match.Artist);
	fixHTML(match.Title);
	fixHTML(match.Version);
	fixHTML(match.Genre);
	fixHTML(match.Label);
	fixHTML(match.Bpm);
	fixHTML(match.Album);
	fixHTML(match.ReleaseDate);
}

bool buildinMatch(const ppl7::String& data, RegExpMatch& match)
{
	ppl7::Array Matches;
	if (data.pregMatch("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/", Matches)) {
		//printf ("Match ASOT\n");
		match.Artist=ppl7::Trim(Matches[1]);
		match.Title=ppl7::Trim(Matches[2]);
		match.Version=ppl7::Trim(Matches[3]);
	} else if (data.pregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/", Matches)) {
		match.Artist=ppl7::Trim(Matches[3]);
		match.Title=ppl7::Trim(Matches[1]);
		match.Version=ppl7::Trim(Matches[2]);
	} else if (data.pregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/", Matches)) {
		match.Artist=ppl7::Trim(Matches[3]);
		match.Title=ppl7::Trim(Matches[1]);
		match.Version=ppl7::Trim(Matches[2]);
	} else if (data.pregMatch("/^(.*?)\\-(.*?)\\((.*?)\\).*$/", Matches)) {
		match.Artist=ppl7::Trim(Matches[1]);
		match.Title=ppl7::Trim(Matches[2]);
		match.Version=ppl7::Trim(Matches[3]);
	} else if (data.pregMatch("/^(.*?)\\-(.*?)$/", Matches)) {
		match.Artist=ppl7::Trim(Matches[1]);
		match.Title=ppl7::Trim(Matches[2]);
	} else {
		return false;
	}
	if (match.Title.pregMatch("/(.*?)(\\sfeat\\..*)$/i", Matches)) {
		match.Title=Matches[1].trimmed();
		match.Artist+=Matches[2].trimmed();
	} else if (match.Title.pregMatch("/(.*?)(\\sfeaturing.*)$/i", Matches)) {
		match.Title=Matches[1].trimmed();
		match.Artist+=Matches[2].trimmed();
	} else if (match.Title.pregMatch("/(.*?)(\\spres\\..*)$/i", Matches)) {
		match.Title=Matches[1].trimmed();
		match.Artist+=Matches[2].trimmed();
	} else if (match.Title.pregMatch("/(.*?)(\\spresents.*)$/i", Matches)) {
		match.Title=Matches[1].trimmed();
		match.Artist+=Matches[2].trimmed();
	}
	fixIt(match);
	return true;
}

} // EOF namespace repexpmatch

} // EOF namespace winkusik
} // EOF namespace pfp
} // EOF namespace de
