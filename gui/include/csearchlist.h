/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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

#include "ppl6.h"


class GenericListItem
{
	public:
		virtual ~GenericListItem() {};
		virtual void clear()=0;
		virtual void importXML(const ppl6::CString &xml)=0;
		virtual ppl6::CString exportXML() const=0;
};

class SearchlistItem : public GenericListItem
{
	private:
	public:
		ppl6::CString	Artist;
		ppl6::CString	Title;
		ppl6::CString	Version;
		ppl6::CString	Genre;
		ppl6::CString	Comment;
		ppl6::CDateTime DateAdded;
		ppl6::CDateTime	ReleaseDate;
		int				Length;
		bool			found;
		bool			selected;
		ppluint8		Rating;

		SearchlistItem();
		SearchlistItem(const ppl6::CString &Misc);
		SearchlistItem(const ppl6::CString &Artist, const ppl6::CString &Title, const ppl6::CString Version="");
		virtual void clear();
		virtual void importXML(const ppl6::CString &xml);
		virtual ppl6::CString exportXML() const;
};

class CWmClient;

class CSearchlist
{
	private:
		std::vector<SearchlistItem> list;
		//std::map<ppl6::CString, int> Words;
		ppl6::CString Name;
		ppl6::CDateTime DateCreated;
		ppl6::CDateTime DateUpdated;

	public:
		CSearchlist();
		~CSearchlist();

		void setName(const ppl6::CString &name);
		const ppl6::CString &name() const;
		const ppl6::CDateTime &dateCreated() const;
		const ppl6::CDateTime &dateUpdated() const;
		size_t size() const;
		void clear();
		void add(const SearchlistItem &item);
		void insert(size_t pos, const SearchlistItem &item);
		void replace(size_t pos, const SearchlistItem &item);
		void remove(size_t pos);
		const SearchlistItem& get(size_t pos) const;
		SearchlistItem& get(size_t pos);
		const SearchlistItem& operator[] (size_t pos) const;
		SearchlistItem& operator[] (size_t pos);
		ppl6::CString toXML() const;

		int save(const ppl6::CString &filename) const;
		int load(const ppl6::CString &filename, bool headerOnly=false);

};

#endif /* CSEARCHLIST_H_ */
