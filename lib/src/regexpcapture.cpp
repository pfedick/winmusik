/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2012 Patrick Fedick
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

void RegExpPattern::copyFrom(const RegExpPattern &other)
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

RegExpMatch::RegExpMatch()
{
	Length=0;
}



RegularExpressionCapture::RegularExpressionCapture()
{

}

RegularExpressionCapture::~RegularExpressionCapture()
{

}

void RegularExpressionCapture::load(const ppl7::String &configfile)
{
	patterns.clear();
	ppl7::ConfigParser conf;
	try {
		conf.load(configfile);
		conf.firstSection();
		while (1) {
			RegExpPattern p;
			p.Name=conf.get("Name");
			if (p.Name.isEmpty()) break;
			p.Pattern=conf.get("Pattern");
			p.artist=conf.getInt("artist",0);
			p.title=conf.getInt("title",0);
			p.version=conf.getInt("version",0);
			p.genre=conf.getInt("genre",0);
			p.label=conf.getInt("label",0);
			p.bpm=conf.getInt("bpm",0);
			p.album=conf.getInt("album",0);
			p.releasedate=conf.getInt("releasedate",0);
			p.hours=conf.getInt("hours",0);
			p.minutes=conf.getInt("minutes",0);
			p.seconds=conf.getInt("seconds",0);
			p.isHTML=conf.getBool("isHTML", false);
			patterns.push_back(p);
			conf.nextSection();
		}
	} catch (...) {

	}
	if (patterns.empty()) addDefaultPatterns();

}

void RegularExpressionCapture::addDefaultPatterns()
{
	RegExpPattern p;
	//Tonny Nesse - Memories (Fady & Mina Remix, 7:56 min, Trance) [MP3 572 A-5]
	p.Name="WinMusik Editor";
	p.Pattern="^(.*?)\\s-\\s(.*)\\s\\((.*),\\s([0-9]+):([0-9]+)\\smin,\\s(.*?)\\)\\s\\[.*?\\]$";
	p.artist=1;
	p.title=2;
	p.version=3;
	p.minutes=4;
	p.seconds=5;
	p.genre=6;
	patterns.push_back(p);
}

void RegularExpressionCapture::save(const ppl7::String &configfile)
{
	ppl7::ConfigParser conf;
	std::vector<RegExpPattern>::const_iterator it;
	int pos=0;
	ppl7::String Group;
	for (it = patterns.begin() ; it != patterns.end(); ++it) {
		Group.setf("%i",pos);
		conf.createSection(Group);
		conf.add("Name",(*it).Name);
		conf.add("Pattern",(*it).Pattern);
		conf.add("artist",(*it).artist);
		conf.add("title",(*it).title);
		conf.add("version",(*it).version);
		conf.add("genre",(*it).genre);
		conf.add("label",(*it).label);
		conf.add("bpm",(*it).bpm);
		conf.add("album",(*it).album);
		conf.add("releasedate",(*it).releasedate);
		conf.add("hours",(*it).hours);
		conf.add("minutes",(*it).minutes);
		conf.add("seconds",(*it).seconds);
		conf.add("isHTML",(*it).isHTML);
		pos++;
	}
	conf.save(configfile);
}

void RegularExpressionCapture::addPattern(const RegExpPattern &pattern)
{
	patterns.push_back(pattern);
}

void RegularExpressionCapture::insertPattern(size_t pos, const RegExpPattern &pattern)
{
	if (pos<patterns.size()) {
		std::vector<RegExpPattern>::iterator it;
		it = patterns.begin();
		try {
			it+=pos;
			patterns.insert(it,pattern);
		} catch ( const std::out_of_range & oor) {
			patterns.push_back(pattern);
		}
	} else {
		patterns.push_back(pattern);
	}
}

void RegularExpressionCapture::deletePattern(size_t pos)
{
	if (pos<patterns.size()) {
		std::vector<RegExpPattern>::iterator it;
		it = patterns.begin();
		try {
			it+=pos;
			patterns.erase(it);
		} catch ( const std::out_of_range & oor) {

		}
	}
}

void RegularExpressionCapture::setPattern(size_t pos, const RegExpPattern &pattern)
{
	if (pos<patterns.size()) {
		RegExpPattern &p=patterns.at(pos);
		p.copyFrom(pattern);
	}
}


const RegExpPattern &RegularExpressionCapture::getPattern(size_t pos) const
{
	return patterns.at(pos);
}

size_t RegularExpressionCapture::size() const
{
	return patterns.size();
}

void RegularExpressionCapture::copyToMatch(const RegExpPattern &p, const ppl7::Array &res, RegExpMatch &match) const
{
	if (p.artist) match.Artist=ppl7::Trim(res[p.artist]);
	if (p.title) match.Title=ppl7::Trim(res[p.title]);
	if (p.version) match.Version=ppl7::Trim(res[p.version]);
	if (p.genre) match.Genre=ppl7::Trim(res[p.genre]);
	if (p.label) match.Label=ppl7::Trim(res[p.label]);
	if (p.bpm) match.Bpm=ppl7::Trim(res[p.bpm]);
	if (p.album) match.Album=ppl7::Trim(res[p.album]);
	if (p.hours) match.Length+=res[p.hours].toInt()*60*60;
	if (p.minutes) match.Length+=res[p.minutes].toInt()*60;
	if (p.seconds) match.Length+=res[p.seconds].toInt();
	if (p.releasedate) match.ReleaseDate=ppl7::Trim(res[p.releasedate]);
}


static bool matchBeatPortPro100_getArtist(const ppl7::String &html, RegExpMatch &match)
{
	ppl7::Array Lines, List, matches;
	//printf ("DEBUG getArtist\n");
	if (!html.pregMatch("/<p class=\"buk-track-artists\".*?>(.*?)<\\/p>/is",matches)) return false;
	//printf ("ok 1\n");
	Lines.explode(matches.get(1),"\n");
	for (size_t i=0;i<Lines.size();i++) {
		const ppl7::String &Line=Lines.get(i);
		if (Line.pregMatch("/<a href=\".*?\" data-artist=\"[0-9]+\".*?>(.*?)<\\/a>/is",matches)) {
			List.add(matches.get(1));
		}
	}
	if (List.size()==0) return false;
	//printf ("ok 2\n");
	match.Artist=ppl7::UnescapeHTMLTags(List.implode(", "));
	//printf ("Artist: %s\n",(const char*)match.Artist);
	return true;
}

static bool matchBeatPortPro100_getLabels(const ppl7::String &html, RegExpMatch &match)
{
	ppl7::Array Lines, List, matches;
	if (!html.pregMatch("/<p class=\"buk-track-labels\".*?>(.*?)<\\/p>/is",matches)) return false;
	Lines.explode(matches.get(1),"\n");
	for (size_t i=0;i<Lines.size();i++) {
		const ppl7::String &Line=Lines.get(i);
		if (Line.pregMatch("/<a href=\".*?\" data-label=\"[0-9]+\".*?>(.*?)<\\/a>/is",matches)) {
			List.add(matches.get(1));
		}
	}
	if (List.size()==0) return false;
	match.Label=ppl7::UnescapeHTMLTags(List.implode(", "));
	//printf ("Label: %s\n",(const char*)match.Label);
	return true;
}

static bool matchBeatPortPro100_getGenres(const ppl7::String &html, RegExpMatch &match)
{
	ppl7::Array Lines, List, matches;
	if (!html.pregMatch("/<p class=\"buk-track-genre\".*?>(.*?)<\\/p>/is",matches)) return false;
	Lines.explode(matches.get(1),"\n");
	for (size_t i=0;i<Lines.size();i++) {
		const ppl7::String &Line=Lines.get(i);
		if (Line.pregMatch("/<a href=\".*?\" data-genre=\"[0-9]+\".*?>(.*?)<\\/a>/is",matches)) {
			List.add(matches.get(1));
		}
	}
	if (List.size()==0) return false;
	match.Genre=ppl7::UnescapeHTMLTags(List.implode(", "));
	//printf ("Genre: %s\n",(const char*)match.Genre);
	return true;
}


static void fixArtistAndTitle(const ppl7::String &prefix, RegExpMatch &match, const ppl7::String prefix_replace)
{
	ppl7::String regex1="/(.*?)\\s"+prefix+"\\s+(.*)$/i";
	ppl7::String regex2="/(.*?)\\("+prefix+"\\s+(.*)\\)$/i";
	ppl7::Array Matches;
	if (match.Title.pregMatch(regex1, Matches)==true || match.Title.pregMatch(regex2, Matches)==true) {
		ppl7::String feat=Matches.get(2);
		feat.trim();
		match.Title=Matches[1];
		match.Artist.replace(feat,"");
		match.Artist.trim();
		match.Artist.trim(",");
		match.Artist.replace(",,",",");
		match.Artist+=" "+prefix_replace+" "+Matches.get(2);
	}
}

static void fixIt(RegExpMatch &match)
{
	//ppl6::CArray Matches;
	fixArtistAndTitle("feat\\.",match, "feat.");
	fixArtistAndTitle("featuring",match, "feat.");
	fixArtistAndTitle("ft",match, "feat.");
	fixArtistAndTitle("ft.",match, "feat.");
	fixArtistAndTitle("pres\\.",match, "pres.");
	fixArtistAndTitle("presents",match, "pres.");
}

static void fixHTML(ppl7::String &text)
{
	text=ppl7::UnescapeHTMLTags(text);
}

static void fixHTML(RegExpMatch &match)
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

static bool matchBeatPortPro100(const ppl7::String &html, RegExpMatch &match)
{
	ppl7::Array matches;
	//printf ("Try match: %s\n\n",(const char*)html);
	//<span class="buk-track-primary-title">Access</span>
	if (!html.pregMatch("/<span class=\"buk-track-primary-title\".*?>(.*?)<\\/span>/is",matches)) return false;
	match.Title=ppl7::UnescapeHTMLTags(matches.get(1));
	//printf ("Title ok => %s\n", (const char*)match.Title);
	if (!html.pregMatch("/<span class=\"buk-track-remixed\".*?>(.*?)<\\/span>/is",matches)) return false;
	match.Version=ppl7::UnescapeHTMLTags(matches.get(1));
	//printf ("Version ok => %s\n", (const char*)match.Version);
	if (html.pregMatch("/<p class=\"buk-track-released\".*?>([0-9\\-]+)<\\/p>/is",matches)) {
		match.ReleaseDate=ppl7::UnescapeHTMLTags(matches.get(1));
		//printf ("Released ok => %s\n", (const char*)match.ReleaseDate);
	}
	if (!matchBeatPortPro100_getArtist(html,match)) return false;
	matchBeatPortPro100_getLabels(html,match);
	matchBeatPortPro100_getGenres(html,match);
	fixIt(match);
	return true;
}

/*
static bool matchBeatPortProReleases(const ppl7::String &html, RegExpMatch &match)
{
	ppl7::Array matches;
	ppl7::String Artists,Title,Version,Genre,Released,Label;
	int found=0;
	if (html.pregMatch("/<p class=\"buk-track-artists\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		while (todo.pregMatch("/^(.*?)<a href=.*?>(.*?)<\\/a>(.*?)$/i",matches)) {
			todo=matches[1]+matches[3];
			Artists=Artists+", "+matches[2];
		}
		Artists.trim(" ,");
		found++;
	}
	if (html.pregMatch("/<p class=\"buk-track-title\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		if (todo.pregMatch("/<span class=\"buk-track-primary-title\".*?>(.*?)<\\/span>/i",matches)) {
			Title=matches[1];
		}
		if (todo.pregMatch("/<span class=\"buk-track-remixed\".*?>(.*?)<\\/span>/i",matches)) {
			Version=matches[1];
		}
		found++;

	}
	if (html.pregMatch("/<p class=\"buk-track-genre\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		//printf ("Match1\n");
		//todo.printnl();
		if (todo.pregMatch("/<a href=.*?>(.*?)<\\/a>/i",matches)) {
			Genre=matches[1];
		}
		found++;

	}
	if (html.pregMatch("/<p class=\"buk-track-released\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		Released=matches[1];
		found++;
	}
	if (html.pregMatch("/<p class=\"buk-track-labels\".*?>(.*?)<\\/p>/i",matches)) {
		ppl7::String todo=matches[1];
		if (todo.pregMatch("/<a href=.*?>(.*?)<\\/a>/i",matches)) {
			Label=matches[1];
		}
		found++;

	}
	match.Artist=Artists;
	match.Title=Title;
	match.Genre=Genre;
	match.Label=Label;
	match.ReleaseDate=Released;
	match.Version=Version;
	fixHTML(match);
	fixIt(match);
	//printf ("matches: %d\n",found);
	if (found>1) return true;
	return false;
}
*/


bool RegularExpressionCapture::match(const ppl7::String &data, RegExpMatch &match) const
{
	if (matchBeatPortPro100(data,match)) return true;

	if (patterns.empty()) return false;
	std::vector<RegExpPattern>::const_iterator it;
	for (it = patterns.begin() ; it != patterns.end(); ++it) {
		if(testMatch(data,match,*it)) {
			fixIt(match);
			return true;
		}
	}
	if (buildinMatch(data,match)) return true;
	return false;
}

bool RegularExpressionCapture::buildinMatch(const ppl7::String &data, RegExpMatch &match) const
{
	ppl7::Array Matches;
	if (data.pregMatch("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/",Matches)) {
		//printf ("Match ASOT\n");
		match.Artist=ppl7::Trim(Matches[1]);
		match.Title=ppl7::Trim(Matches[2]);
		match.Version=ppl7::Trim(Matches[3]);
	} else if (data.pregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/",Matches)) {
		match.Artist=ppl7::Trim(Matches[3]);
		match.Title=ppl7::Trim(Matches[1]);
		match.Version=ppl7::Trim(Matches[2]);
	} else if (data.pregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/",Matches)) {
		match.Artist=ppl7::Trim(Matches[3]);
		match.Title=ppl7::Trim(Matches[1]);
		match.Version=ppl7::Trim(Matches[2]);
	} else if (data.pregMatch("/^(.*?)\\-(.*?)\\((.*?)\\).*$/",Matches)) {
		match.Artist=ppl7::Trim(Matches[1]);
		match.Title=ppl7::Trim(Matches[2]);
		match.Version=ppl7::Trim(Matches[3]);
	} else if (data.pregMatch("/^(.*?)\\-(.*?)$/",Matches)) {
		match.Artist=ppl7::Trim(Matches[1]);
		match.Title=ppl7::Trim(Matches[2]);
	} else {
		return false;
	}
	if (match.Title.pregMatch("/(.*?)(\\sfeat\\..*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.pregMatch("/(.*?)(\\sfeaturing.*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.pregMatch("/(.*?)(\\spres\\..*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.pregMatch("/(.*?)(\\spresents.*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	}
	fixIt(match);
	return true;
}

bool RegularExpressionCapture::testMatch(const ppl7::String &data, RegExpMatch &match, const RegExpPattern &pattern) const
{
	ppl7::String Pattern;
	ppl7::Array res;
	Pattern="/"+(pattern).Pattern+"/ism";
	if (data.pregMatch(Pattern,res)) {
		copyToMatch(pattern,res,match);
		return true;
	}
	return false;
}




}}}	// EOF Namespace de.pfp.winmusik

