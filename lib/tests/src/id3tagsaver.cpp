#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ppl7.h>
#include <ppl7-audio.h>
#include <gtest/gtest.h>
#include "libwinmusik3.h"
#include "wmlib-tests.h"
#include <map>

#include "wm_id3tagsaver.h"

namespace {

using namespace de::pfp::winmusik;


class ID3TagSaverTest : public ::testing::Test {
protected:
	ID3TagSaverTest() {
		if (setlocale(LC_ALL,"")==NULL) {
			printf ("setlocale fehlgeschlagen\n");
			throw std::exception();
		}
	}
	~ID3TagSaverTest()
	{

	}
};

TEST_F(ID3TagSaverTest, updateNow) {
}



} // EOF namespace
