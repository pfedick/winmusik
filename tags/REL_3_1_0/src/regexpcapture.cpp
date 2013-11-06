/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/10/10 10:31:01 $
 * $Id: properties.cpp,v 1.5 2010/10/10 10:31:01 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
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

void RegExpClipboard::copyFromClipboard()
{
	QString subtype="html";
	Html = QApplication::clipboard()->text(subtype);
	PlainText = QApplication::clipboard()->text();
}

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
}

RegularExpressionCapture::RegularExpressionCapture()
{

}

RegularExpressionCapture::~RegularExpressionCapture()
{

}

void RegularExpressionCapture::load()
{
	patterns.clear();
	if (!wm_main) return;
	ppl6::CString File=wm_main->conf.DataPath+"/regexp.conf";
	QSettings settings(File,QSettings::IniFormat);
	size_t pos=0;
	ppl6::CString Group;
	while (1) {
		Group.Setf("%zi",pos);
		settings.beginGroup(Group);
		RegExpPattern p;
		p.Name=settings.value("Name").toString();
		if (p.Name.IsEmpty()) break;
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
	ppl6::CString File=wm_main->conf.DataPath+"/regexp.conf";
	QSettings settings(File,QSettings::IniFormat);
	std::vector<RegExpPattern>::const_iterator it;

	size_t pos=0;
	ppl6::CString Group;
	for (it = patterns.begin() ; it != patterns.end(); ++it) {
		Group.Setf("%zi",pos);
		settings.beginGroup(Group);
		settings.setValue("Name",(*it).Name);
		settings.setValue("Pattern",(*it).Pattern);
		settings.setValue("artist",(*it).artist);
		settings.setValue("title",(*it).title);
		settings.setValue("version",(*it).version);
		settings.setValue("genre",(*it).genre);
		settings.setValue("label",(*it).label);
		settings.setValue("bpm",(*it).bpm);
		settings.setValue("album",(*it).album);
		settings.setValue("releasedate",(*it).releasedate);
		settings.setValue("hours",(*it).hours);
		settings.setValue("minutes",(*it).minutes);
		settings.setValue("seconds",(*it).seconds);

		settings.endGroup();

		pos++;
	}
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

void RegularExpressionCapture::copyToMatch(const RegExpPattern &p, const ppl6::CArray &res, RegExpMatch &match) const
{
	if (p.artist) match.Artist=ppl6::Trim(res[p.artist]);
	if (p.title) match.Title=ppl6::Trim(res[p.title]);
	if (p.version) match.Version=ppl6::Trim(res[p.version]);
	if (p.genre) match.Genre=ppl6::Trim(res[p.genre]);
	if (p.label) match.Label=ppl6::Trim(res[p.label]);
	if (p.bpm) match.Bpm=ppl6::Trim(res[p.bpm]);
	if (p.album) match.Album=ppl6::Trim(res[p.album]);
	if (p.hours) match.Length+=res.GetString(p.hours).ToInt()*60*60;
	if (p.minutes) match.Length+=res.GetString(p.minutes).ToInt()*60;
	if (p.seconds) match.Length+=res.GetString(p.seconds).ToInt();
	if (p.releasedate) match.ReleaseDate=ppl6::Trim(res[p.releasedate]);
}


bool RegularExpressionCapture::match(const RegExpClipboard &data, RegExpMatch &match) const
{
	//if (this->match(data.Html,match)) return true;
	return this->match(data.PlainText,match);
}

bool RegularExpressionCapture::match(const ppl6::CString &data, RegExpMatch &match) const
{
	if (patterns.empty()) return false;
	std::vector<RegExpPattern>::const_iterator it;
	for (it = patterns.begin() ; it != patterns.end(); ++it) {
		if(testMatch(data,match,*it)) return true;
	}
	if (buildinMatch(data,match)) return true;
	return false;
}

bool RegularExpressionCapture::buildinMatch(const ppl6::CString &data, RegExpMatch &match) const
{
	ppl6::CArray Matches;
	if (data.PregMatch("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/",Matches)) {
		//printf ("Match ASOT\n");
		match.Artist=ppl6::Trim(Matches[1]);
		match.Title=ppl6::Trim(Matches[2]);
		match.Version=ppl6::Trim(Matches[3]);
	} else if (data.PregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/",Matches)) {
		match.Artist=ppl6::Trim(Matches[3]);
		match.Title=ppl6::Trim(Matches[1]);
		match.Version=ppl6::Trim(Matches[2]);
	} else if (data.PregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/",Matches)) {
		match.Artist=ppl6::Trim(Matches[3]);
		match.Title=ppl6::Trim(Matches[1]);
		match.Version=ppl6::Trim(Matches[2]);
	} else if (data.PregMatch("/^(.*?)\\-(.*?)\\((.*?)\\).*$/",Matches)) {
		match.Artist=ppl6::Trim(Matches[1]);
		match.Title=ppl6::Trim(Matches[2]);
		match.Version=ppl6::Trim(Matches[3]);
	} else if (data.PregMatch("/^(.*?)\\-(.*?)$/",Matches)) {
		match.Artist=ppl6::Trim(Matches[1]);
		match.Title=ppl6::Trim(Matches[2]);
	} else {
		return false;
	}
	if (match.Title.PregMatch("/(.*?)(\\sfeat\\..*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.PregMatch("/(.*?)(\\sfeaturing.*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.PregMatch("/(.*?)(\\spres\\..*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	} else if (match.Title.PregMatch("/(.*?)(\\spresents.*)$/i", Matches)) {
		match.Title=Matches[1];
		match.Artist+=Matches[2];
	}
	return true;
}

bool RegularExpressionCapture::testMatch(const ppl6::CString &data, RegExpMatch &match, const RegExpPattern &pattern) const
{
	ppl6::CString Pattern;
	ppl6::CArray res;
	Pattern="/"+(pattern).Pattern+"/ism";
	if (data.PregMatch(Pattern,res)) {
		copyToMatch(pattern,res,match);
		return true;
	}
	return false;
}
