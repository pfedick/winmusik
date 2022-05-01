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


#ifndef TRAKTOR_H_
#define TRAKTOR_H_
#include <map>
#include <list>
#include "ppl7.h"
#include "ppl7-audio.h"

class TraktorTagFrame
{
public:
	ppl7::String	name;
	size_t			size;
	size_t			numChilds;
	const char* data;
	std::map<ppl7::String, TraktorTagFrame> childs;
};

class TraktorTagCue
{
public:
	enum TraktorTagType {
		CUE,
		IN,
		OUT,
		LOAD,
		GRID,
		LOOP
	};
	int				version;
	ppl7::String	name;
	int				displ_order;
	TraktorTagType	type;
	double			start;
	double			len;
	int				repeats;
	int				hotcue;

	TraktorTagCue();
	void print() const;
	ppl7::String typeName() const;
};


void getTraktorFrames(std::map<ppl7::String, TraktorTagFrame>& frames, const char* adr, size_t size);
void getTraktorCues(std::list <TraktorTagCue>& cuelist, const TraktorTagFrame& cuep);
void getTraktorCues(std::list <TraktorTagCue>& cuelist, const ppl7::ID3Tag& Tag);
int getTraktorCuesFromFile(std::list <TraktorTagCue>& cuelist, const ppl7::String& Filename);

#endif /* TRAKTOR_H_ */
