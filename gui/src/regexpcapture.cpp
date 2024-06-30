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

#include "winmusik3.h"
#include "regexpcapture.h"
#include <QSettings>
#include <QClipboard>
#include <exception>
#include <stdexcept>

#include "ppl7.h"
#include "ppl7-types.h"

void copyFromClipboard(de::pfp::winmusik::RegExpClipboard& clip)
{
	QString subtype="html";
	clip.Html = QApplication::clipboard()->text(subtype);
	clip.PlainText = QApplication::clipboard()->text();
}

RegularExpressionCapture::RegularExpressionCapture()
{

}

RegularExpressionCapture::~RegularExpressionCapture()
{

}

void RegularExpressionCapture::load()
{
	//loadScripts();
	patterns.clear();
	if (!wm_main) return;
	ppl7::String File=wm_main->conf.DataPath + "/regexp.conf";
	QSettings settings(File, QSettings::IniFormat);
	size_t pos=0;
	ppl7::String Group;
	while (1) {
		Group.setf("%i", (int)pos);
		settings.beginGroup(QString(Group));
		RegExpPattern p;
		p.Name=settings.value("Name").toString();
		if (p.Name.isEmpty()) break;
		p.Pattern=settings.value("Pattern").toString();
		p.artist=settings.value("artist").toInt();
		p.title=settings.value("title").toInt();
		p.version=settings.value("version").toInt();
		p.genre=settings.value("genre").toInt();
		p.label=settings.value("label").toInt();
		p.bpm=settings.value("bpm").toInt();
		p.album=settings.value("album").toInt();
		p.releasedate=settings.value("releasedate").toInt();
		p.hours=settings.value("hours").toInt();
		p.minutes=settings.value("minutes").toInt();
		p.seconds=settings.value("seconds").toInt();
		p.isHTML=settings.value("isHTML", false).toBool();
		patterns.push_back(p);

		settings.endGroup();
		pos++;
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

void RegularExpressionCapture::save()
{
	if (!wm_main) return;
	ppl7::String File=wm_main->conf.DataPath + "/regexp.conf";
	QSettings settings(File, QSettings::IniFormat);
	std::vector<RegExpPattern>::const_iterator it;

	size_t pos=0;
	ppl7::String Group;
	for (it = patterns.begin(); it != patterns.end(); ++it) {
		Group.setf("%i", (int)pos);
		settings.beginGroup(QString(Group));
		settings.setValue("Name", (*it).Name);
		settings.setValue("Pattern", (*it).Pattern);
		settings.setValue("artist", (*it).artist);
		settings.setValue("title", (*it).title);
		settings.setValue("version", (*it).version);
		settings.setValue("genre", (*it).genre);
		settings.setValue("label", (*it).label);
		settings.setValue("bpm", (*it).bpm);
		settings.setValue("album", (*it).album);
		settings.setValue("releasedate", (*it).releasedate);
		settings.setValue("hours", (*it).hours);
		settings.setValue("minutes", (*it).minutes);
		settings.setValue("seconds", (*it).seconds);
		settings.setValue("isHTML", (*it).isHTML);
		settings.endGroup();

		pos++;
	}
}

void RegularExpressionCapture::addPattern(const RegExpPattern& pattern)
{
	patterns.push_back(pattern);
}

void RegularExpressionCapture::insertPattern(size_t pos, const RegExpPattern& pattern)
{
	if (pos < patterns.size()) {
		std::vector<RegExpPattern>::iterator it;
		it = patterns.begin();
		try {
			it+=pos;
			patterns.insert(it, pattern);
		} catch (const std::out_of_range& oor) {
			patterns.push_back(pattern);
		}
	} else {
		patterns.push_back(pattern);
	}
}

void RegularExpressionCapture::deletePattern(size_t pos)
{
	if (pos < patterns.size()) {
		std::vector<RegExpPattern>::iterator it;
		it = patterns.begin();
		try {
			it+=pos;
			patterns.erase(it);
		} catch (const std::out_of_range& oor) {

		}
	}
}

void RegularExpressionCapture::setPattern(size_t pos, const RegExpPattern& pattern)
{
	if (pos < patterns.size()) {
		RegExpPattern& p=patterns.at(pos);
		p.copyFrom(pattern);
	}
}


const RegExpPattern& RegularExpressionCapture::getPattern(size_t pos) const
{
	return patterns.at(pos);
}

size_t RegularExpressionCapture::size() const
{
	return patterns.size();
}

void RegularExpressionCapture::copyToMatch(const RegExpPattern& p, const ppl7::Array& res, RegExpMatch& match) const
{
	if (p.artist) match.Artist=ppl7::Trim(res[p.artist]);
	if (p.title) match.Title=ppl7::Trim(res[p.title]);
	if (p.version) match.Version=ppl7::Trim(res[p.version]);
	if (p.genre) match.Genre=ppl7::Trim(res[p.genre]);
	if (p.label) match.Label=ppl7::Trim(res[p.label]);
	if (p.bpm) match.Bpm=ppl7::Trim(res[p.bpm]);
	if (p.album) match.Album=ppl7::Trim(res[p.album]);
	if (p.hours) match.Length+=res.get(p.hours).toInt() * 60 * 60;
	if (p.minutes) match.Length+=res.get(p.minutes).toInt() * 60;
	if (p.seconds) match.Length+=res.get(p.seconds).toInt();
	if (p.releasedate) match.ReleaseDate=ppl7::Trim(res[p.releasedate]);
}


bool RegularExpressionCapture::match(const RegExpClipboard& data, RegExpMatch& match) const
{
	//if (matchAgainstScripts(data,match)) return true;
	//if (this->match(data.Html,match)) return true;
	//if (matchBeatPortPro100(data.Html,match)) return true;
	if (de::pfp::winmusik::repexpmatch::matchBeatPort(data.Html, match)) return true;

	if (patterns.empty()) return false;
	std::vector<RegExpPattern>::const_iterator it;
	for (it = patterns.begin(); it != patterns.end(); ++it) {
		if ((*it).isHTML) {
			if (testMatch(data.Html, match, *it)) {
				repexpmatch::fixHTML(match);
				repexpmatch::fixIt(match);
				return true;
			}
		} else {
			if (testMatch(data.PlainText, match, *it)) {
				repexpmatch::fixIt(match);
				return true;
			}
		}
	}
	if (repexpmatch::buildinMatch(data.PlainText, match)) return true;
	return false;
	//return this->match(data.PlainText,match);
}

bool RegularExpressionCapture::match(const ppl7::String& data, RegExpMatch& match) const
{
	if (de::pfp::winmusik::repexpmatch::matchBeatPort(data, match)) return true;

	if (patterns.empty()) return false;
	std::vector<RegExpPattern>::const_iterator it;
	for (it = patterns.begin(); it != patterns.end(); ++it) {
		if (testMatch(data, match, *it)) {
			repexpmatch::fixIt(match);
			return true;
		}
	}
	if (repexpmatch::buildinMatch(data, match)) return true;
	return false;
}


bool RegularExpressionCapture::testMatch(const ppl7::String& data, RegExpMatch& match, const RegExpPattern& pattern) const
{
	ppl7::String Pattern;
	ppl7::Array res;
	Pattern="/" + pattern.Pattern + "/ism";
	if (data.pregMatch(Pattern, res)) {
		copyToMatch(pattern, res, match);
		return true;
	}
	return false;
}
