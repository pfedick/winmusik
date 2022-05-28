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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <gtest/gtest.h>
#include "libwinmusik3.h"
#include "wm_playlist.h"

#include "ppl7.h"
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class PlaylistTest : public ::testing::Test {
protected:
	PlaylistTest() {
		if (setlocale(LC_ALL, "") == NULL) {
			printf("setlocale fehlgeschlagen\n");
			throw std::exception();
		}
	}
	~PlaylistTest()
	{

	}
};

TEST_F(PlaylistTest, loadPlaylist) {
	Playlist pl;
	pl.load("testdata/pp287.wmp");
	ASSERT_EQ(ppl7::String("Powerplay 287 - 10/2018"), pl.getName());
	ASSERT_EQ(ppl7::String("Subname"), pl.getSubName());
	ASSERT_EQ((int)287, pl.getIssueNumber());
	ASSERT_EQ(ppl7::DateTime("2018-11-11"), pl.getIssueDate());
	ASSERT_EQ((size_t)33, pl.size());
	ASSERT_EQ((int)33, pl.getTotalTracks());
	ASSERT_EQ((int)12298, pl.getTotalLength());
	ASSERT_EQ((int)7183, pl.getTotalMixLength());

	Playlist::const_iterator it;
	for (it=pl.begin();it != pl.end();++it) {
		if ((*it).titleId == 90471) {
			ASSERT_EQ((int)54, (*it).startPositionSec);
			ASSERT_EQ((int)322, (*it).endPositionSec);
			ASSERT_EQ((int)219, (*it).cutStartPosition[0]);
			ASSERT_EQ((int)262, (*it).cutEndPosition[0]);
			ASSERT_EQ(ppl7::String("Skylex"), (*it).Artist);
			ASSERT_EQ(ppl7::String("Chrysippus"), (*it).Title);
			ASSERT_EQ(ppl7::String("Extended Mix"), (*it).Version);
			ASSERT_EQ(ppl7::String("Trance"), (*it).Genre);
			ASSERT_EQ((int)322, (*it).trackLength);
			ASSERT_EQ((int)225, (*it).mixLength);
			ASSERT_EQ(ppl7::String("D:/musik/mp3/07/739/037-Skylex - Chrysippus (Extended Mix).mp3"), (*it).File);
		}
	}
}





} // EOF namespace
