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

#ifndef CSEARCHLIST_H_
#define CSEARCHLIST_H_

#include <vector>
#include <map>

#include <pplib.h>

class GenericListItem
{
public:
    virtual ~GenericListItem() {};
    virtual void clear() = 0;
    virtual void importXML(const pplib::String& xml) = 0;
    virtual pplib::String exportXML() const = 0;
};

class SearchlistItem : public GenericListItem
{
private:
public:
    pplib::String Artist;
    pplib::String Title;
    pplib::String Version;
    pplib::String Genre;
    pplib::String Comment;
    pplib::String Label;
    pplib::String Key;
    pplib::String Bpm;
    pplib::String ShopURL;
    pplib::String Tags;
    pplib::String CoverFilename;
    pplib::DateTime DateAdded;
    pplib::DateTime ReleaseDate;
    int Length;
    bool found;
    bool selected;
    uint8_t Rating;

    SearchlistItem();
    SearchlistItem(const pplib::String& Misc);
    SearchlistItem(const pplib::String& Artist, const pplib::String& Title, const pplib::String Version = "");
    virtual void clear();
    virtual void importXML(const pplib::String& xml);
    virtual pplib::String exportXML() const;
};

class CWmClient;

class CSearchlist
{
private:
    std::vector<SearchlistItem> list;
    pplib::String Name;
    pplib::DateTime DateCreated;
    pplib::DateTime DateUpdated;

public:
    CSearchlist();
    ~CSearchlist();

    void setName(const pplib::String& name);
    const pplib::String& name() const;
    const pplib::DateTime& dateCreated() const;
    const pplib::DateTime& dateUpdated() const;
    size_t size() const;
    void clear();
    void add(const SearchlistItem& item);
    void insert(size_t pos, const SearchlistItem& item);
    void replace(size_t pos, const SearchlistItem& item);
    void remove(size_t pos);
    const SearchlistItem& get(size_t pos) const;
    SearchlistItem& get(size_t pos);
    const SearchlistItem& operator[](size_t pos) const;
    SearchlistItem& operator[](size_t pos);
    pplib::String toXML() const;

    void save(const pplib::String& filename) const;
    void load(const pplib::String& filename, bool headerOnly = false);
};

#endif /* CSEARCHLIST_H_ */
