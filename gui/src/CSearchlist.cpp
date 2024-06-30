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

#include <winmusik3.h>
#include "csearchlist.h"
 //

SearchlistItem::SearchlistItem()
{
	found=false;
	selected=false;
	Length=0;
	DateAdded.setCurrentTime();
	Rating=0;
}

SearchlistItem::SearchlistItem(const ppl7::String& Misc)
{
	found=false;
	selected=false;
	Length=0;
	DateAdded.setCurrentTime();
	RegExpMatch match;
	if (wm_main->RegExpCapture.match(Misc, match)) {
		Artist=match.Artist;
		Title=match.Title;
		Version=match.Version;
		Genre=match.Genre;
		Comment=match.Label;
		Length=match.Length;
		return;
	}


	ppl7::Array Matches;
	if (Misc.pregMatch("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/", Matches)) {
		//printf ("Match ASOT\n");
		Artist=ppl7::Trim(Matches[1]);
		Title=ppl7::Trim(Matches[2]);
		Version=ppl7::Trim(Matches[3]);
	} else if (Misc.pregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/", Matches)) {
		Artist=ppl7::Trim(Matches[3]);
		Title=ppl7::Trim(Matches[1]);
		Version=ppl7::Trim(Matches[2]);
	} else if (Misc.pregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/", Matches)) {
		Artist=ppl7::Trim(Matches[3]);
		Title=ppl7::Trim(Matches[1]);
		Version=ppl7::Trim(Matches[2]);
	} else if (Misc.pregMatch("/^(.*?)\\-(.*?)\\((.*?)\\).*$/", Matches)) {
		Artist=ppl7::Trim(Matches[1]);
		Title=ppl7::Trim(Matches[2]);
		Version=ppl7::Trim(Matches[3]);
	} else if (Misc.pregMatch("/^(.*?)\\-(.*?)$/", Matches)) {
		Artist=ppl7::Trim(Matches[1]);
		Title=ppl7::Trim(Matches[2]);
	}
	if (Title.pregMatch("/(.*?)(\\sfeat\\..*)$/i", Matches)) {
		Title=Matches[1];
		Artist+=Matches[2];
	} else if (Title.pregMatch("/(.*?)(\\sfeaturing.*)$/i", Matches)) {
		Title=Matches[1];
		Artist+=Matches[2];
	} else if (Title.pregMatch("/(.*?)(\\spres\\..*)$/i", Matches)) {
		Title=Matches[1];
		Artist+=Matches[2];
	} else if (Title.pregMatch("/(.*?)(\\spresents.*)$/i", Matches)) {
		Title=Matches[1];
		Artist+=Matches[2];
	}
}

SearchlistItem::SearchlistItem(const ppl7::String& Artist, const ppl7::String& Title, const ppl7::String Version)
{
	this->Artist=Artist;
	this->Title=Title;
	this->Version=Version;
	DateAdded.setCurrentTime();
	found=false;
	selected=false;
}

void SearchlistItem::clear()
{
	found=false;
	selected=false;
	Artist.clear();
	Title.clear();
	Version.clear();
	Genre.clear();
	Comment.clear();
	ReleaseDate.clear();
	DateAdded.clear();
}

void SearchlistItem::importXML(const ppl7::String& xml)
{
	ppl7::Array Matches;
	if (xml.pregMatch("/\\<artist\\>(.*)\\<\\/artist\\>/s", Matches)) Artist=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<title\\>(.*)\\<\\/title\\>/s", Matches)) Title=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<version\\>(.*)\\<\\/version\\>/s", Matches)) Version=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<genre\\>(.*)\\<\\/genre\\>/s", Matches)) Genre=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<comment\\>(.*)\\<\\/comment\\>/s", Matches)) Comment=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	try {
		if (xml.pregMatch("/\\<releasedate\\>(.*)\\<\\/releasedate\\>/s", Matches)) ReleaseDate=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	} catch (...) {
		ReleaseDate.clear();
	}
	try {
		if (xml.pregMatch("/\\<dateadded\\>(.*)\\<\\/dateadded\\>/s", Matches)) DateAdded=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	} catch (...) {
		DateAdded.clear();
	}
	if (xml.pregMatch("/\\<length\\>(.*)\\<\\/length\\>/s", Matches)) Length=ppl7::UnescapeHTMLTags(Matches[1]).toInt();
	if (xml.pregMatch("/\\<found\\>(.*)\\<\\/found\\>/s", Matches)) found=ppl7::IsTrue(ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])));
	if (xml.pregMatch("/\\<selected\\>(.*)\\<\\/selected\\>/s", Matches)) selected=ppl7::IsTrue(ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])));
	if (xml.pregMatch("/\\<rating\\>(.*)\\<\\/rating\\>/s", Matches)) Rating=ppl7::UnescapeHTMLTags(Matches[1]).toInt();
}

ppl7::String SearchlistItem::exportXML() const
{
	ppl7::String s;
	s="<searchlistitem>\n"
		"	<artist>" + ppl7::EscapeHTMLTags(Artist) + "</artist>\n"
		"	<title>" + ppl7::EscapeHTMLTags(Title) + "</title>\n"
		"	<version>" + ppl7::EscapeHTMLTags(Version) + "</version>\n"
		"	<genre>" + ppl7::EscapeHTMLTags(Genre) + "</genre>\n"
		"	<comment>" + ppl7::EscapeHTMLTags(Comment) + "</comment>\n";
	try {
		s+="	<releasedate>" + ReleaseDate.getISO8601() + "</releasedate>\n";
	} catch (...) {

	}
	try {
		s+="	<dateadded>" + DateAdded.getISO8601() + "</dateadded>\n";
	} catch (...) {

	}
	s+=		"	<length>" + ppl7::ToString("%i", Length) + "</length>\n"
		"	<found>" + (found ? "true" : "false") + "</found>\n"
		"	<selected>" + (selected ? "true" : "false") + "</selected>\n"
		"	<rating>" + ppl7::ToString("%i", Rating) + "</rating>\n"
		"</searchlistitem>\n";
	return s;
}


CSearchlist::CSearchlist()
{
	DateUpdated.setCurrentTime();
	DateCreated.setCurrentTime();
}

CSearchlist::~CSearchlist()
{

}

void CSearchlist::setName(const ppl7::String& name)
{
	this->Name=name;
}

const ppl7::String& CSearchlist::name() const
{
	return Name;
}

const ppl7::DateTime& CSearchlist::dateCreated() const
{
	return DateCreated;
}

const ppl7::DateTime& CSearchlist::dateUpdated() const
{
	return DateUpdated;
}


size_t CSearchlist::size() const
{
	return list.size();
}

void CSearchlist::clear()
{
	Name.clear();
	list.clear();
	DateUpdated.setCurrentTime();
	//DateCreated.setCurrentTime();	// Das bleibt, wie es war
}

void CSearchlist::add(const SearchlistItem& item)
{
	list.push_back(item);
	DateUpdated.setCurrentTime();
}

void CSearchlist::insert(size_t pos, const SearchlistItem& item)
{
	if (pos >= list.size()) {		// Am Ende einfügen
		list.push_back(item);
		return;
	}
	std::vector<SearchlistItem>::iterator it;
	it=list.begin();
	list.insert(it + pos, item);
	DateUpdated.setCurrentTime();
}

void CSearchlist::replace(size_t pos, const SearchlistItem& item)
{
	SearchlistItem& old=list.at(pos);
	old=item;
	DateUpdated.setCurrentTime();
}

void CSearchlist::remove(size_t pos)
{
	if (pos > list.size()) return;
	std::vector<SearchlistItem>::iterator it;
	it=list.begin();
	list.erase(it + pos);
	DateUpdated.setCurrentTime();
}

const SearchlistItem& CSearchlist::get(size_t pos) const
{
	return list.at(pos);
}

SearchlistItem& CSearchlist::get(size_t pos)
{
	return list.at(pos);
}

const SearchlistItem& CSearchlist::operator[] (size_t pos) const
{
	return list.at(pos);
}

SearchlistItem& CSearchlist::operator[] (size_t pos)
{
	return list.at(pos);
}

ppl7::String CSearchlist::toXML() const
{
	ppl7::String s;
	s="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
	s+="<searchlist>\n";
	s+="<header>\n";
	s+="   <name>" + ppl7::EscapeHTMLTags(Name) + "</name>\n";
	s+="   <tracks>" + ppl7::ToString("%zu", list.size()) + "</tracks>\n";
	s+="   <dateCreated>" + DateCreated.getISO8601() + "</dateCreated>\n";
	s+="   <dateUpdated>" + DateUpdated.getISO8601() + "</dateUpdated>\n";
	s+="</header>\n";
	for (size_t i=0;i < list.size();i++) {
		s+=list.at(i).exportXML();
	}
	s+="</searchlist>\n";
	return s;
}

void CSearchlist::save(const ppl7::String& filename) const
{
	ppl7::File ff;
	ff.open(filename, ppl7::File::WRITE);
	ff.puts(toXML());
}

void CSearchlist::load(const ppl7::String& filename, bool headerOnly)
{
	clear();
	ppl7::String content;
	ppl7::File::load(content, filename);
	ppl7::Array matches;

	if (content.pregMatch("/\\<header\\>(.*?)\\<\\/header\\>/s", matches)) {
		ppl7::String header=matches.get(1);
		if (header.pregMatch("/\\<name\\>(.*)\\<\\/name\\>/s", matches)) Name=ppl7::Trim(ppl7::UnescapeHTMLTags(matches.get(1)));
		if (header.pregMatch("/\\<dateCreated\\>(.*)\\<\\/dateCreated\\>/s", matches)) DateCreated=ppl7::Trim(ppl7::UnescapeHTMLTags(matches.get(1)));
		if (header.pregMatch("/\\<dateUpdated\\>(.*)\\<\\/dateUpdated\\>/s", matches)) DateUpdated=ppl7::Trim(ppl7::UnescapeHTMLTags(matches.get(1)));

	}
	if (headerOnly) return;
	ppl7::Array rows;
	rows.explode(content, "<searchlistitem>");
	for (size_t i=1;i < rows.size();i++) {
		SearchlistItem item;
		item.importXML(rows.get(i));
		list.push_back(item);
	}
	return;
}
