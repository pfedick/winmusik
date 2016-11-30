/*
 * csearchlist.h
 *
 *  Created on: 14.01.2012
 *      Author: patrick
 */

#ifndef CSEARCHLIST_H_
#define CSEARCHLIST_H_

#include <vector>
#include <map>
#include <ppl6.h>


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
		ppl6::CString	ReleaseDate;
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
		std::map<ppl6::CString, int> Words;
		ppl6::CString Name;
		ppl6::CDateTime DateCreated;
		ppl6::CDateTime DateUpdated;

		CWmClient *wm;

	public:
		CSearchlist();
		~CSearchlist();

		void setWmClient(CWmClient *wm);

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
