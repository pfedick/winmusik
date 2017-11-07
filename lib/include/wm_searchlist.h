/*
 * cwmfile.h
 *
 *  Created on: 20.06.2016
 *      Author: patrick
 */

#ifndef LIB_INCLUDE_WM_SEARCHLIST_H_
#define LIB_INCLUDE_WM_SEARCHLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <ppl7.h>
#include <vector>


namespace de {
namespace pfp {
namespace winmusik {


class SearchlistItem
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
		ppluint8		Rating;

		SearchlistItem();
		SearchlistItem(const ppl7::String &Misc);
		SearchlistItem(const ppl7::String &Artist, const ppl7::String &Title, const ppl7::String Version="");
		void clear();
		void importXML(const ppl7::String &xml);
		ppl7::String exportXML() const;
};


class Searchlist
{
	private:
		std::vector<SearchlistItem> list;
		ppl7::String Name;
		ppl7::DateTime DateCreated;
		ppl7::DateTime DateUpdated;

	public:
		Searchlist();
		~Searchlist();

		void setName(const ppl7::String &name);
		const ppl7::String &name() const;
		const ppl7::DateTime &dateCreated() const;
		const ppl7::DateTime &dateUpdated() const;
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
		ppl7::String toXML() const;

		int save(const ppl7::String &filename) const;
		int load(const ppl7::String &filename, bool headerOnly=false);

};


}}}	// EOF Namespace de.pfp.winmusik

#endif /* LIB_INCLUDE_WM_SEARCHLIST_H_ */
