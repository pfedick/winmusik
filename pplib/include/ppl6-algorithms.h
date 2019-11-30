/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 6 (PPL6).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author: pafe $
 * $Revision: 887 $
 * $Date: 2014-02-04 14:24:47 +0100 (Di, 04 Feb 2014) $
 * $Id: CDir.cpp 887 2014-02-04 13:24:47Z pafe $
 *
 *******************************************************************************
 * Copyright (c) 2010, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/


#ifndef PPL6ALGORITHMS_H_
#define PPL6ALGORITHMS_H_

namespace ppl6 {

template <class K> class List
{
	public:
		class Iterator;
	private:
		MemoryHeap		MyHeap;
		class ListItem {
			friend class List;
			friend class List::Iterator;
			private:
				K item;
				ListItem *previous, *next, *original;
				List *owner;
			public:
				void *operator new(size_t, void *p) { return (p);}
		};
		ListItem *first, *last;
	public:
		class Iterator
		{
			friend class List;
			private:
				ListItem* item;
				bool	init;
			public:
				Iterator() {item=NULL; init=false;}
				K& value() const
				{
					if (!item) throw NullPointerException();
					return (item->item);
				}
		};

		List() {
			first=NULL;
			last=NULL;
			MyHeap.init(sizeof(ListItem),0,100);
		}
		~List() {
			clear();
		}

 		size_t capacity() const
 		{
 			return (MyHeap.capacity());
 		}
 		size_t itemSize() const
 		{
 			return (sizeof(ListItem));
 		}
 		void reserve(size_t num)
 		{
 			MyHeap.reserve(num);
 		}

		K& add(const K &item)
		{
			ListItem *it=new (MyHeap.malloc())ListItem;
			it->item=item;
			it->original=it;
			it->previous=last;
			it->next=NULL;
			it->owner=this;
			if (last) last->next=it;
			if (!first) first=it;
			last=it;
			return (it->item);
		}

		K& push_front(const K &item)
				{
					ListItem *it=new (MyHeap.malloc())ListItem;
					it->item=item;
					it->original=it;
					it->previous=NULL;
					it->next=first;
					it->owner=this;
					if (first) first->previous=it;
					if (!last) last=it;
					first=it;
					return (it->item);
				}



		void erase(const K &item)
		{
			ListItem *it=first;
			while (it) {
				if (it->item==item) {
					if (it->previous) it->previous->next=it->next;
					if (it->next) it->next->previous=it->previous;
					if (it==first) first=it->next;
					if (it==last) last=it->previous;
					MyHeap.free(it);
					return;
				}
				it=it->next;
			}
		}

		size_t		num() const
		{
			return (MyHeap.count());
		}
		size_t		count() const
		{
			return (MyHeap.count());
		}
		size_t		size() const
		{
			return (MyHeap.count());
		}

		void		clear()
		{
			ListItem *it;
			while (first) {
				it=first;
				first=it->next;
				it->~ListItem();
			}
			last=NULL;
			MyHeap.clear();
		}

		void	reset(Iterator &it) const throw()
		{
			it.item=NULL;
			it.init=false;
		}
		bool	getFirst(Iterator &it) const throw()
		{
			it.item=first;
			it.init=true;
			if (first) return (true);
			return (false);
		}
		bool	getNext(Iterator &it) const throw()
		{
			if (!it.init) {
				it.item=first;
				it.init=true;
			} else {
				it.item=it.item->next;
			}
			ListItem *item=it.item;
			if (!item) return (false);
			return (true);
		}
		bool	getLast(Iterator &it) const throw()
		{
			it.item=last;
			it.init=true;
			if (last) return (true);
			return (false);
		}
		bool	getPrevious(Iterator &it) const throw()
		{
			if (!it.init) {
				it.item=last;
				it.init=true;
			} else {
				it.item=it.item->previous;
			}
			ListItem *item=it.item;
			if (!item) return (false);
			return (true);
		}


};


}	// EOF namespace ppl6


#endif /* PPL6ALGORITHMS_H_ */
