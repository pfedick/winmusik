#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ppl7.h>
#include <ppl7-audio.h>
#include <gtest/gtest.h>
#include "../../include/libwinmusik3.h"
#include "../wmlib-tests.h"
#include <map>

#include "../../include/wm_traktor.h"

namespace {

using namespace de::pfp::winmusik;


class TraktorTest : public ::testing::Test {
	protected:
		TraktorTest() {
			if (setlocale(LC_ALL,"")==NULL) {
				printf ("setlocale fehlgeschlagen\n");
				throw std::exception();
			}
		}
		~TraktorTest()
		{

		}
};

void iterate(const std::map<ppl7::String,TraktorTagFrame> frames, int indent=0)
{
	std::map<ppl7::String,TraktorTagFrame>::const_iterator it;
	ppl7::String space;
	space.repeat(' ',indent);
	for (it=frames.begin();it!=frames.end();++it) {
		const TraktorTagFrame &traktor=(*it).second;
		printf ("%sName: >>%s<< -> >>%s<<, size: %zd, childs: %zd\n",(const char*)space,
				(const char*)(*it).first,
				(const char*)traktor.name, traktor.size, traktor.numChilds);
		if (traktor.numChilds==0) {
			/*
			if (traktor.name!="ARTW" && traktor.name!="TRN3")
				ppl7::HexDump(traktor.data, traktor.size);
				*/
			if (traktor.name=="TRN3")  ppl7::HexDump(traktor.data, 256);

		}
		iterate(traktor.childs,indent+3);
	}
}

#ifdef TODO
TEST_F(TraktorTest, load) {
	ppl7::ID3Tag Tag;
	Tag.load("/musik/mp3/07/746/061-Mauro Picotto - Komodo (Binary Finary Extended Remix).mp3");
	Tag.listFrames(false);
	ppl7::ID3Frame *popm=Tag.findFrame("POPM");
	if (popm) {
		popm->hexDump();
	}

	std::map<ppl7::String,TraktorTagFrame> frames;
	getTraktorFrames(frames,Tag);
	iterate(frames);

	/*
	ppl7::ByteArray ba;
	ppl7::File::load(ba, "testdata/IJWHX0DPWJ10ZCKS5SAFBAQJKVBA");
	ba.hexDump(1,256);

	ppl7::File::load(ba, "testdata/stripes/IJWHX0DPWJ10ZCKS5SAFBAQJKVBA");
	ba.hexDump();
	*/
}

#endif



} // EOF namespace
