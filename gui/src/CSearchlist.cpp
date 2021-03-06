/*
 * CSearchlist.cpp
 *
 *  Created on: 14.01.2012
 *      Author: patrick
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

SearchlistItem::SearchlistItem(const ppl6::CString &Misc)
{
	found=false;
	selected=false;
	Length=0;
	DateAdded.setCurrentTime();
	RegExpMatch match;
	if (wm_main->RegExpCapture.match(Misc,match)) {
		Artist=match.Artist;
		Title=match.Title;
		Version=match.Version;
		Genre=match.Genre;
		Comment=match.Label;
		Length=match.Length;
		return;
	}


	ppl6::CArray Matches;
	if (Misc.PregMatch("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/",Matches)) {
		//printf ("Match ASOT\n");
		Artist=ppl6::Trim(Matches[1]);
		Title=ppl6::Trim(Matches[2]);
		Version=ppl6::Trim(Matches[3]);
	} else if (Misc.PregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/",Matches)) {
		Artist=ppl6::Trim(Matches[3]);
		Title=ppl6::Trim(Matches[1]);
		Version=ppl6::Trim(Matches[2]);
	} else if (Misc.PregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/",Matches)) {
		Artist=ppl6::Trim(Matches[3]);
		Title=ppl6::Trim(Matches[1]);
		Version=ppl6::Trim(Matches[2]);
	} else if (Misc.PregMatch("/^(.*?)\\-(.*?)\\((.*?)\\).*$/",Matches)) {
		Artist=ppl6::Trim(Matches[1]);
		Title=ppl6::Trim(Matches[2]);
		Version=ppl6::Trim(Matches[3]);
	} else if (Misc.PregMatch("/^(.*?)\\-(.*?)$/",Matches)) {
		Artist=ppl6::Trim(Matches[1]);
		Title=ppl6::Trim(Matches[2]);
	}
	if (Title.PregMatch("/(.*?)(\\sfeat\\..*)$/i", Matches)) {
		Title=Matches[1];
		Artist+=Matches[2];
	} else if (Title.PregMatch("/(.*?)(\\sfeaturing.*)$/i", Matches)) {
			Title=Matches[1];
			Artist+=Matches[2];
	} else if (Title.PregMatch("/(.*?)(\\spres\\..*)$/i", Matches)) {
			Title=Matches[1];
			Artist+=Matches[2];
	} else if (Title.PregMatch("/(.*?)(\\spresents.*)$/i", Matches)) {
			Title=Matches[1];
			Artist+=Matches[2];
	}
}

SearchlistItem::SearchlistItem(const ppl6::CString &Artist, const ppl6::CString &Title, const ppl6::CString Version)
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
	Artist.Clear();
	Title.Clear();
	Version.Clear();
	Genre.Clear();
	Comment.Clear();
	ReleaseDate.clear();
	DateAdded.clear();
}

void SearchlistItem::importXML(const ppl6::CString &xml)
{
	ppl6::CArray Matches;
	if (xml.PregMatch("/\\<artist\\>(.*)\\<\\/artist\\>/s",Matches)) Artist=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<title\\>(.*)\\<\\/title\\>/s",Matches)) Title=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<version\\>(.*)\\<\\/version\\>/s",Matches)) Version=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<genre\\>(.*)\\<\\/genre\\>/s",Matches)) Genre=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<comment\\>(.*)\\<\\/comment\\>/s",Matches)) Comment=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	try {
		if (xml.PregMatch("/\\<releasedate\\>(.*)\\<\\/releasedate\\>/s",Matches)) ReleaseDate=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	} catch (...) {
		ReleaseDate.clear();
	}
	try {
		if (xml.PregMatch("/\\<dateadded\\>(.*)\\<\\/dateadded\\>/s",Matches)) DateAdded=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	} catch (...) {
		DateAdded.clear();
	}
	if (xml.PregMatch("/\\<length\\>(.*)\\<\\/length\\>/s",Matches)) Length=ppl6::atoi(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<found\\>(.*)\\<\\/found\\>/s",Matches)) found=ppl6::IsTrue(ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1])));
	if (xml.PregMatch("/\\<selected\\>(.*)\\<\\/selected\\>/s",Matches)) selected=ppl6::IsTrue(ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1])));
	if (xml.PregMatch("/\\<rating\\>(.*)\\<\\/rating\\>/s",Matches)) Rating=ppl6::atoi(ppl6::UnescapeHTMLTags(Matches[1]));
}

ppl6::CString SearchlistItem::exportXML() const
{
	ppl6::CString s;
	s="<searchlistitem>\n"
			"	<artist>"+ppl6::EscapeHTMLTags(Artist)+"</artist>\n"
			"	<title>"+ppl6::EscapeHTMLTags(Title)+"</title>\n"
			"	<version>"+ppl6::EscapeHTMLTags(Version)+"</version>\n"
			"	<genre>"+ppl6::EscapeHTMLTags(Genre)+"</genre>\n"
			"	<comment>"+ppl6::EscapeHTMLTags(Comment)+"</comment>\n";
	try {
		s+="	<releasedate>"+ReleaseDate.getISO8601()+"</releasedate>\n";
	} catch (...) {

	}
	try {
		s+="	<dateadded>"+DateAdded.getISO8601()+"</dateadded>\n";
	} catch (...) {

	}
	s+=		"	<length>"+ppl6::ToString("%i",Length)+"</length>\n"
			"	<found>"+(found?"true":"false")+"</found>\n"
			"	<selected>"+(selected?"true":"false")+"</selected>\n"
			"	<rating>"+ppl6::ToString("%i",Rating)+"</rating>\n"
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

void CSearchlist::setName(const ppl6::CString &name)
{
	this->Name=name;
}

const ppl6::CString &CSearchlist::name() const
{
	return Name;
}

const ppl6::CDateTime &CSearchlist::dateCreated() const
{
	return DateCreated;
}

const ppl6::CDateTime &CSearchlist::dateUpdated() const
{
	return DateUpdated;
}


size_t CSearchlist::size() const
{
	return list.size();
}

void CSearchlist::clear()
{
	Name.Clear();
	list.clear();
	DateUpdated.setCurrentTime();
	//DateCreated.setCurrentTime();	// Das bleibt, wie es war
}

void CSearchlist::add(const SearchlistItem &item)
{
	list.push_back(item);
	DateUpdated.setCurrentTime();
}

void CSearchlist::insert(size_t pos, const SearchlistItem &item)
{
	if (pos>=list.size()) {		// Am Ende einfügen
		list.push_back(item);
		return;
	}
	std::vector<SearchlistItem>::iterator it;
	it=list.begin();
	list.insert(it+pos,item);
	DateUpdated.setCurrentTime();
}

void CSearchlist::replace(size_t pos, const SearchlistItem &item)
{
	SearchlistItem &old=list.at(pos);
	old=item;
	DateUpdated.setCurrentTime();
}

void CSearchlist::remove(size_t pos)
{
	if (pos>list.size()) return;
	std::vector<SearchlistItem>::iterator it;
	it=list.begin();
	list.erase(it+pos);
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

ppl6::CString CSearchlist::toXML() const
{
	ppl6::CString s;
	s="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
	s+="<searchlist>\n";
	s+="<header>\n";
	s+="   <name>"+ppl6::EscapeHTMLTags(Name)+"</name>\n";
	s+="   <tracks>"+ppl6::ToString("%zu",list.size())+"</tracks>\n";
	s+="   <dateCreated>"+DateCreated.getISO8601()+"</dateCreated>\n";
	s+="   <dateUpdated>"+DateUpdated.getISO8601()+"</dateUpdated>\n";
	s+="</header>\n";
	for (size_t i=0;i<list.size();i++) {
		s+=list.at(i).exportXML();
	}
	s+="</searchlist>\n";
	return s;
}

int CSearchlist::save(const ppl6::CString &filename) const
{
	ppl6::CFile ff;
	if (!ff.Open(filename,"wb")) return 0;
	ff.Puts(toXML());
	return 1;
}

int CSearchlist::load(const ppl6::CString &filename, bool headerOnly)
{
	clear();
	ppl6::CFile ff;
	if (!ff.Open(filename,"rb")) return 0;
	ppl6::CString content;
	if (!ff.Load(content)) return 0;
	if (content.PregMatch("/\\<header\\>(.*?)\\<\\/header\\>/s")) {
		ppl6::CString header=content.GetMatch(1);
		if (header.PregMatch("/\\<name\\>(.*)\\<\\/name\\>/s")) Name=ppl6::Trim(ppl6::UnescapeHTMLTags(header.GetMatch(1)));
		if (header.PregMatch("/\\<dateCreated\\>(.*)\\<\\/dateCreated\\>/s")) DateCreated=ppl6::Trim(ppl6::UnescapeHTMLTags(header.GetMatch(1)));
		if (header.PregMatch("/\\<dateUpdated\\>(.*)\\<\\/dateUpdated\\>/s")) DateUpdated=ppl6::Trim(ppl6::UnescapeHTMLTags(header.GetMatch(1)));

	}
	if (headerOnly) return 1;
	ppl6::CArray rows;
	rows.Explode(content,"<searchlistitem>");
	for (int i=1;i<rows.Num();i++) {
		SearchlistItem item;
		item.importXML(rows.GetString(i));
		list.push_back(item);
	}
	return 1;
}

