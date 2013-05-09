/*
 * CSearchlist.cpp
 *
 *  Created on: 14.01.2012
 *      Author: patrick
 */

#include <ppl6.h>
#include "csearchlist.h"


SearchlistItem::SearchlistItem()
{
	found=false;
	Length=0;
	DateAdded.setCurrentTime();
	Rating=0;
}

SearchlistItem::SearchlistItem(const ppl6::CString &Misc)
{
	found=false;
	Length=0;
	DateAdded.setCurrentTime();
	ppl6::CArray Matches;
	// Mike Koglin - Dyno (Original Mix, 8:05 min, Trance) [MP3 542 A-8]

	/* 1 Exemplar(e) von: So Out Of Reach (Original) [MP3-Download]
	Von: Space RockerZ feat. Ellie Lawson
	Zustand: Neu
	Verkauf durch: Amazon Media EU S.\xc3\xa0 r.l.
	*/
	if (Misc.PregMatch("/Exemplar.*?von: (.*?)\\((.*?)\\).*?MP3-Download.*?Von: (.*?)\\n/s",Matches)) {
		Artist=ppl6::Trim(Matches[3]);
		Title=ppl6::Trim(Matches[1]);
		Version=ppl6::Trim(Matches[2]);
	} else if (Misc.PregMatch("/Exemplar.*?von: (.*?).*?MP3-Download.*?Von: (.*?)\\n/s",Matches)) {
			Artist=ppl6::Trim(Matches[2]);
			Title=ppl6::Trim(Matches[1]);

			/*
			 *     Helion (Original Mix)
    Genix, Mike Koglin
    2011-12-067:08AnjunabeatsTrance
    */
			// Beatport Player
	} else if (Misc.PregMatch("/^\\s*(.*?)\\s\\((.*?)\\)\\n\\s*(.*?)\\n/s",Matches)) {
		Title=ppl6::Trim(Matches[1]);
		Version=ppl6::Trim(Matches[2]);
		Artist=ppl6::Trim(Matches[3]);
	} else if (Misc.PregMatch("/^(.*?)\\-(.*?)\\((.*?),\\s[0-9:\\smin]+,\\s(.*)\\).*$/",Matches)) {
		Artist=ppl6::Trim(Matches[1]);
		Title=ppl6::Trim(Matches[2]);
		Version=ppl6::Trim(Matches[3]);
		Genre=ppl6::Trim(Matches[4]);
		// Beatport Liste
		// Eclipse (Broning Remix) 	Sequentia 	Broning 	Perfecto Records 	Trance 	2012-01-27 	2,17 €
	} else if (Misc.PregMatch("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t(.*?)\\t(.*?)\\t(.*?)\\t*/",Matches)) {
		Artist=ppl6::Trim(Matches[3]);
		Title=ppl6::Trim(Matches[1]);
		Version=ppl6::Trim(Matches[2]);
		Genre=ppl6::Trim(Matches[6]);
		Comment=ppl6::Trim(Matches[5]);
	// PFP-Webpage ASOT Playlist
	} else if (Misc.PregMatch("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/",Matches)) {
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
}

void SearchlistItem::clear()
{
	found=false;
	Artist.Clear();
	Title.Clear();
	Version.Clear();
	Genre.Clear();
	Comment.Clear();
	ReleaseDate.Clear();
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
	if (xml.PregMatch("/\\<releasedate\\>(.*)\\<\\/releasedate\\>/s",Matches)) ReleaseDate=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<dateadded\\>(.*)\\<\\/dateadded\\>/s",Matches)) DateAdded=ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<length\\>(.*)\\<\\/length\\>/s",Matches)) Length=ppl6::atoi(ppl6::UnescapeHTMLTags(Matches[1]));
	if (xml.PregMatch("/\\<found\\>(.*)\\<\\/found\\>/s",Matches)) found=ppl6::IsTrue(ppl6::Trim(ppl6::UnescapeHTMLTags(Matches[1])));
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
			"	<comment>"+ppl6::EscapeHTMLTags(Comment)+"</comment>\n"
			"	<releasedate>"+ppl6::EscapeHTMLTags(ReleaseDate)+"</releasedate>\n"
			"	<dateadded>"+DateAdded.getISO8601()+"</dateadded>\n"
			"	<length>"+ppl6::ToString("%i",Length)+"</length>\n"
			"	<found>"+(found?"true":"false")+"</found>\n"
			"	<rating>"+ppl6::ToString("%i",Rating)+"</rating>\n"
			"</searchlistitem>\n";
	return s;
}


CSearchlist::CSearchlist()
{
	DateUpdated.setCurrentTime();
	DateCreated.setCurrentTime();
	wm=NULL;
}

CSearchlist::~CSearchlist()
{

}

void CSearchlist::setWmClient(CWmClient *wm)
{
	this->wm=wm;
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

