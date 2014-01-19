/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: StorageTrack.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
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

#include <stdio.h>
#include "traktor.h"

TraktorTagCue::TraktorTagCue()
{
	version=0;
	displ_order=0;
	type=CUE;
	start=0.0f;
	len=0.0f;
	repeats=0;
	hotcue=0;
}

void TraktorTagCue::print() const
{
	printf ("CUE: version=%i, name=%s, displ_order=%i, type=%s, start=%f, len=%f, repeats=%i, hotcue=%i\n",
			version,(const char*)name,displ_order,(const char*)typeName(),
			start,len,repeats,hotcue);
}

ppl6::CString TraktorTagCue::typeName() const
{
	switch (type) {
		case CUE: return "CUE";
		case IN: return "IN";
		case OUT: return "OUT";
		case LOAD: return "LOAD";
		case GRID: return "GRID";
		case LOOP: return "LOOP";
	}
	return "";
}

void getTraktorFrames(std::map<ppl6::CString,TraktorTagFrame> &frames, const char *adr, size_t size)
{
	size_t pos=0;
	char FrameName[5];
	FrameName[4]=0;
	while (pos<size) {
		/*
		0 (0x00)	4 (0x04)	frame identifier, e.g. 'D', 'M', 'R', 'T' for "DMRT" (Traktor Media?)
		4 (0x04)	4 (0x04)	frame length in bytes (FRAMELEN)
		8 (0x08)	4 (0x04)	children count in bytes
		12 (0x0c)	FRAMELEN	data content of frame
		*/
		TraktorTagFrame frame;
		ppl6::Poke32(FrameName, ppl6::PeekN32(adr));
		frame.name=FrameName;
		frame.size=ppl6::Peek32(adr+4);
		frame.numChilds=ppl6::Peek32(adr+8);
		frame.data=adr+12;
		//printf("FrameName: %s, length: %zd, children: %zd\n",FrameName,frame.size,frame.numChilds);
		if (frame.numChilds>0) {
			//printf ("Iterate start: %s...\n",FrameName);
			getTraktorFrames(frame.childs,adr+12,frame.size);
			//printf ("Iterate end: %s...\n",FrameName);
		}
		frames.insert ( std::pair<ppl6::CString,TraktorTagFrame>(frame.name,frame));
		//printf("FrameName: %s, length: %zd, children: %zd\n",FrameName,length,children);
		adr+=12+frame.size;
		pos+=12+frame.size;
		if (frame.name=="TRMD") break;

	}
}

void getTraktorCues(std::list <TraktorTagCue> &cuelist, const TraktorTagFrame &cuep)
{
	ppl6::CIconv iconv;
	iconv.Init("UTF-16LE","UTF-8");
	if (cuep.size<10) return;
	const char *adr=cuep.data;
	int cuepoints=ppl6::Peek32(adr);
	adr+=4;
	for (int i=0;i<cuepoints;i++) {
		TraktorTagCue cue;
		cue.version=ppl6::Peek32(adr);
		if (cue.version!=1) {
			return;
		}
		adr+=4;
		int namelen=ppl6::Peek32(adr);
		adr+=4;
		if (namelen) {
			ppl6::HexDump((void*)adr,namelen*2);
			char *t=iconv.Transcode(adr,namelen*2);
			cue.name.Set(t);
			free(t);
			adr+=namelen*2;
		}
		cue.displ_order=ppl6::Peek32(adr);
		cue.type=(TraktorTagCue::TraktorTagType)ppl6::Peek32(adr+4);
		cue.start=((double*)(adr+8))[0];
		cue.len=((double*)(adr+16))[0];
		cue.repeats=ppl6::Peek32(adr+24);
		cue.hotcue=ppl6::Peek32(adr+28);

		adr+=32;
		//cue.print();
		cuelist.push_back(cue);
	}
}

void getTraktorCues(std::list <TraktorTagCue> &cuelist, const ppl6::CID3Tag &Tag)
{
	ppl6::CMemoryReference traktor=Tag.GetPrivateData("TRAKTOR4");
	if (traktor.isNull()) return;

	std::map<ppl6::CString,TraktorTagFrame> frames;
	getTraktorFrames(frames,(const char*)traktor.adr(),traktor.size());

	try {
		const TraktorTagFrame &trmd=frames.at("TRMD");
		const TraktorTagFrame &data=trmd.childs.at("DATA");
		const TraktorTagFrame &cuep=data.childs.at("CUEP");
		getTraktorCues(cuelist,cuep);
	} catch (const std::exception &e) {

	}
}



