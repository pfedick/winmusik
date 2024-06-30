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

#include "ppl7.h"


class GenericListItem
{
public:
	virtual ~GenericListItem() {};
	virtual void clear()=0;
	virtual void importXML(const ppl7::String& xml)=0;
	virtual ppl7::String exportXML() const=0;
};

class SearchlistItem : public GenericListItem
{
private:
public:
	ppl7::String	Artist;
	ppl7::String	Title;
	ppl7::String	Version;
	ppl7::String	Genre;
	ppl7::String	Comment;
	ppl7::DateTime	DateAdded;
	ppl7::DateTime	ReleaseDate;
	int				Length;
	bool			found;
	bool			selected;
	uint8_t			Rating;

	SearchlistItem();
	SearchlistItem(const ppl7::String& Misc);
	SearchlistItem(const ppl7::String& Artist, const ppl7::String& Title, const ppl7::String Version="");
	virtual void clear();
	virtual void importXML(const ppl7::String& xml);
	virtual ppl7::String exportXML() const;
};

class CWmClient;

class CSearchlist
{
private:
	std::vector<SearchlistItem> list;
	ppl7::String Name;
	ppl7::DateTime DateCreated;
	ppl7::DateTime DateUpdated;

public:
	CSearchlist();
	~CSearchlist();

	void setName(const ppl7::String& name);
	const ppl7::String& name() const;
	const ppl7::DateTime& dateCreated() const;
	const ppl7::DateTime& dateUpdated() const;
	size_t size() const;
	void clear();
	void add(const SearchlistItem& item);
	void insert(size_t pos, const SearchlistItem& item);
	void replace(size_t pos, const SearchlistItem& item);
	void remove(size_t pos);
	const SearchlistItem& get(size_t pos) const;
	SearchlistItem& get(size_t pos);
	const SearchlistItem& operator[] (size_t pos) const;
	SearchlistItem& operator[] (size_t pos);
	ppl7::String toXML() const;

	void save(const ppl7::String& filename) const;
	void load(const ppl7::String& filename, bool headerOnly=false);

};

#endif /* CSEARCHLIST_H_ */
