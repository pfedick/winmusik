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
    found = false;
    selected = false;
    Length = 0;
    DateAdded.setCurrentTime();
    Rating = 0;
}

SearchlistItem::SearchlistItem(const pplib::String& Misc)
{
    found = false;
    selected = false;
    Length = 0;
    DateAdded.setCurrentTime();
    RegExpMatch match;
    if (wm_main->RegExpCapture.match(Misc, match)) {
        Artist = match.Artist;
        Title = match.Title;
        Version = match.Version;
        Genre = match.Genre;
        Comment = match.Label;
        Length = match.Length;
        Label = match.Label;
        ShopURL = match.ShopURL;
        return;
    }

    std::vector<pplib::String> Matches;
    if (pplib::RegEx::capture("/^(.*?)\\s+\\-\\s+(.*?)\\t(.*?)\\t.*$/", Misc, Matches)) {
        // printf ("Match ASOT\n");
        Artist = pplib::Trim(Matches[1]);
        Title = pplib::Trim(Matches[2]);
        Version = pplib::Trim(Matches[3]);
    } else if (pplib::RegEx::capture("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)\\t/", Misc, Matches)) {
        Artist = pplib::Trim(Matches[3]);
        Title = pplib::Trim(Matches[1]);
        Version = pplib::Trim(Matches[2]);
    } else if (pplib::RegEx::capture("/^(.*?)\\((.*?)\\)\\s*?\\t(.*?)$/", Misc, Matches)) {
        Artist = pplib::Trim(Matches[3]);
        Title = pplib::Trim(Matches[1]);
        Version = pplib::Trim(Matches[2]);
    } else if (pplib::RegEx::capture("/^(.*?)\\-(.*?)\\((.*?)\\).*$/", Misc, Matches)) {
        Artist = pplib::Trim(Matches[1]);
        Title = pplib::Trim(Matches[2]);
        Version = pplib::Trim(Matches[3]);
    } else if (pplib::RegEx::capture("/^(.*?)\\-(.*?)$/", Misc, Matches)) {
        Artist = pplib::Trim(Matches[1]);
        Title = pplib::Trim(Matches[2]);
    }
    if (pplib::RegEx::capture("/(.*?)(\\sfeat\\..*)$/i", Title, Matches)) {
        Title = Matches[1];
        Artist += Matches[2];
    } else if (pplib::RegEx::capture("/(.*?)(\\sfeaturing.*)$/i", Title, Matches)) {
        Title = Matches[1];
        Artist += Matches[2];
    } else if (pplib::RegEx::capture("/(.*?)(\\spres\\..*)$/i", Title, Matches)) {
        Title = Matches[1];
        Artist += Matches[2];
    } else if (pplib::RegEx::capture("/(.*?)(\\spresents.*)$/i", Title, Matches)) {
        Title = Matches[1];
        Artist += Matches[2];
    }
}

SearchlistItem::SearchlistItem(const pplib::String& Artist, const pplib::String& Title, const pplib::String Version)
{
    this->Artist = Artist;
    this->Title = Title;
    this->Version = Version;
    DateAdded.setCurrentTime();
    found = false;
    selected = false;
}

void SearchlistItem::clear()
{
    found = false;
    selected = false;
    Artist.clear();
    Title.clear();
    Version.clear();
    Genre.clear();
    Comment.clear();
    ReleaseDate.clear();
    DateAdded.clear();
    Key.clear();
    Length = 0;
    Label.clear();
    ShopURL.clear();
    CoverFilename.clear();
    Tags.clear();
    Bpm.clear();
    Rating = 0;
}

void SearchlistItem::importXML(const pplib::String& xml)
{
    std::vector<pplib::String> Matches;
    if (pplib::RegEx::capture("/\\<artist\\>(.*)\\<\\/artist\\>/s", xml, Matches))
        Artist = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<title\\>(.*)\\<\\/title\\>/s", xml, Matches)) Title = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<version\\>(.*)\\<\\/version\\>/s", xml, Matches))
        Version = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<genre\\>(.*)\\<\\/genre\\>/s", xml, Matches)) Genre = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<label\\>(.*)\\<\\/label\\>/s", xml, Matches)) Label = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<shopurl\\>(.*)\\<\\/shopurl\\>/s", xml, Matches))
        ShopURL = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<key\\>(.*)\\<\\/key\\>/s", xml, Matches)) Key = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<bpm\\>(.*)\\<\\/bpm\\>/s", xml, Matches)) Bpm = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));

    if (pplib::RegEx::capture("/\\<tags\\>(.*)\\<\\/tags\\>/s", xml, Matches)) Tags = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<comment\\>(.*)\\<\\/comment\\>/s", xml, Matches))
        Comment = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<coverfilename\\>(.*)\\<\\/coverfilename\\>/s", xml, Matches))
        CoverFilename = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));

    try {
        if (pplib::RegEx::capture("/\\<releasedate\\>(.*)\\<\\/releasedate\\>/s", xml, Matches))
            ReleaseDate = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    }
    catch (...) {
        ReleaseDate.clear();
    }
    try {
        if (pplib::RegEx::capture("/\\<dateadded\\>(.*)\\<\\/dateadded\\>/s", xml, Matches))
            DateAdded = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    }
    catch (...) {
        DateAdded.clear();
    }
    if (pplib::RegEx::capture("/\\<length\\>(.*)\\<\\/length\\>/s", xml, Matches)) Length = pplib::UnescapeHTMLTags(Matches[1]).toInt();
    if (pplib::RegEx::capture("/\\<found\\>(.*)\\<\\/found\\>/s", xml, Matches))
        found = pplib::IsTrue(pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])));
    if (pplib::RegEx::capture("/\\<selected\\>(.*)\\<\\/selected\\>/s", xml, Matches))
        selected = pplib::IsTrue(pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])));
    if (pplib::RegEx::capture("/\\<rating\\>(.*)\\<\\/rating\\>/s", xml, Matches)) Rating = pplib::UnescapeHTMLTags(Matches[1]).toInt();
}

pplib::String SearchlistItem::exportXML() const
{
    pplib::String s;
    s = "<searchlistitem>\n"
        "	<artist>" +
        pplib::EscapeHTMLTags(Artist) +
        "</artist>\n"
        "	<title>" +
        pplib::EscapeHTMLTags(Title) +
        "</title>\n"
        "	<version>" +
        pplib::EscapeHTMLTags(Version) +
        "</version>\n"
        "	<genre>" +
        pplib::EscapeHTMLTags(Genre) +
        "</genre>\n"
        "	<comment>" +
        pplib::EscapeHTMLTags(Comment) + "</comment>\n";
    s += "	<label>" + pplib::EscapeHTMLTags(Label) + "</label>\n";
    s += "	<key>" + pplib::EscapeHTMLTags(Key) + "</key>\n";
    s += "	<bpm>" + pplib::EscapeHTMLTags(Bpm) + "</bpm>\n";
    s += "	<shopurl>" + pplib::EscapeHTMLTags(ShopURL) + "</shopurl>\n";
    s += "	<tags>" + pplib::EscapeHTMLTags(Tags) + "</tags>\n";
    s += "	<coverfilename>" + pplib::EscapeHTMLTags(CoverFilename) + "</coverfilename>\n";
    try {
        s += "	<releasedate>" + ReleaseDate.getISO8601() + "</releasedate>\n";
    }
    catch (...) {
    }
    try {
        s += "	<dateadded>" + DateAdded.getISO8601() + "</dateadded>\n";
    }
    catch (...) {
    }
    s += "	<length>" + pplib::ToString("%i", Length) +
         "</length>\n"
         "	<found>" +
         (found ? "true" : "false") +
         "</found>\n"
         "	<selected>" +
         (selected ? "true" : "false") +
         "</selected>\n"
         "	<rating>" +
         pplib::ToString("%i", Rating) +
         "</rating>\n"
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

void CSearchlist::setName(const pplib::String& name)
{
    this->Name = name;
}

const pplib::String& CSearchlist::name() const
{
    return Name;
}

const pplib::DateTime& CSearchlist::dateCreated() const
{
    return DateCreated;
}

const pplib::DateTime& CSearchlist::dateUpdated() const
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
    // DateCreated.setCurrentTime();	// Das bleibt, wie es war
}

void CSearchlist::add(const SearchlistItem& item)
{
    list.push_back(item);
    DateUpdated.setCurrentTime();
}

void CSearchlist::insert(size_t pos, const SearchlistItem& item)
{
    if (pos >= list.size()) { // Am Ende einfügen
        list.push_back(item);
        return;
    }
    std::vector<SearchlistItem>::iterator it;
    it = list.begin();
    list.insert(it + pos, item);
    DateUpdated.setCurrentTime();
}

void CSearchlist::replace(size_t pos, const SearchlistItem& item)
{
    SearchlistItem& old = list.at(pos);
    old = item;
    DateUpdated.setCurrentTime();
}

void CSearchlist::remove(size_t pos)
{
    if (pos > list.size()) return;
    std::vector<SearchlistItem>::iterator it;
    it = list.begin();
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

const SearchlistItem& CSearchlist::operator[](size_t pos) const
{
    return list.at(pos);
}

SearchlistItem& CSearchlist::operator[](size_t pos)
{
    return list.at(pos);
}

pplib::String CSearchlist::toXML() const
{
    pplib::String s;
    s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    s += "<searchlist>\n";
    s += "<header>\n";
    s += "   <name>" + pplib::EscapeHTMLTags(Name) + "</name>\n";
    s += "   <tracks>" + pplib::ToString("%zu", list.size()) + "</tracks>\n";
    s += "   <dateCreated>" + DateCreated.getISO8601() + "</dateCreated>\n";
    s += "   <dateUpdated>" + DateUpdated.getISO8601() + "</dateUpdated>\n";
    s += "</header>\n";
    for (size_t i = 0; i < list.size(); i++) {
        s += list.at(i).exportXML();
    }
    s += "</searchlist>\n";
    return s;
}

void CSearchlist::save(const pplib::String& filename) const
{
    pplib::File ff;
    ff.open(filename, pplib::File::FileMode::WRITE);
    ff.puts(toXML());
}

void CSearchlist::load(const pplib::String& filename, bool headerOnly)
{
    clear();
    pplib::String content;
    pplib::File::load(content, filename);
    std::vector<pplib::String> matches;

    if (pplib::RegEx::capture("/\\<header\\>(.*?)\\<\\/header\\>/s", content, matches)) {
        pplib::String header = matches.at(1);
        if (pplib::RegEx::capture("/\\<name\\>(.*)\\<\\/name\\>/s", header, matches))
            Name = pplib::Trim(pplib::UnescapeHTMLTags(matches.at(1)));
        if (pplib::RegEx::capture("/\\<dateCreated\\>(.*)\\<\\/dateCreated\\>/s", header, matches))
            DateCreated = pplib::Trim(pplib::UnescapeHTMLTags(matches.at(1)));
        if (pplib::RegEx::capture("/\\<dateUpdated\\>(.*)\\<\\/dateUpdated\\>/s", header, matches))
            DateUpdated = pplib::Trim(pplib::UnescapeHTMLTags(matches.at(1)));
    }
    if (headerOnly) return;
    pplib::Array rows;
    rows.explode(content, "<searchlistitem>");
    for (size_t i = 1; i < rows.size(); i++) {
        SearchlistItem item;
        item.importXML(rows.get(i));
        list.push_back(item);
    }
    return;
}
