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
#include "wm_regexpcapture.h"

#include "ppl7.h"
#include "wmlib-tests.h"

using namespace de::pfp::winmusik;

namespace {

class RegExpCaptureTest : public ::testing::Test {
protected:
    RegExpCaptureTest() {
        if (setlocale(LC_ALL, "") == NULL) {
            printf("setlocale fehlgeschlagen\n");
            throw std::exception();
        }
    }
    ~RegExpCaptureTest()
    {

    }
};


TEST_F(RegExpCaptureTest, Beatport2023SingleArtist) {
    ppl7::String html;
    ppl7::File::load(html, "testdata/beatport2023.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(ppl7::String("Over The Moon"), match.Title);
    ASSERT_EQ(ppl7::String("Festival Mix"), match.Version);
    ASSERT_EQ(ppl7::String("XiJaro & Pitch"), match.Artist);

    ASSERT_EQ(ppl7::String("Dreamstate"), match.Label);
    ASSERT_EQ(ppl7::String("Trance"), match.Genre);
    ASSERT_EQ(ppl7::String("2023-07-14"), match.ReleaseDate);



}

TEST_F(RegExpCaptureTest, Beatport2023MultipleArtists) {
    ppl7::String html;
    ppl7::File::load(html, "testdata/beatport2023_multi_artists.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(ppl7::String("Invincible"), match.Title);
    ASSERT_EQ(ppl7::String("Extended Mix"), match.Version);
    ASSERT_EQ(ppl7::String("Johan Gielen, Airscape feat. Sarah Anglionin"), match.Artist);

    ASSERT_EQ(ppl7::String("Black Hole Recordings"), match.Label);
    ASSERT_EQ(ppl7::String("Trance"), match.Genre);
    ASSERT_EQ(ppl7::String("2023-07-07"), match.ReleaseDate);

}

TEST_F(RegExpCaptureTest, Beatport2023RemixWithArtist) {
    ppl7::String html;
    ppl7::File::load(html, "testdata/beatport2023_remix.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(ppl7::String("Saltwater"), match.Title);
    ASSERT_EQ(ppl7::String("Ilan Bluestone Extended Remix"), match.Version);
    ASSERT_EQ(ppl7::String("Chicane feat. Moya Brennan"), match.Artist);

    ASSERT_EQ(ppl7::String("Armada Music"), match.Label);
    ASSERT_EQ(ppl7::String("Trance"), match.Genre);
    ASSERT_EQ(ppl7::String("2023-09-01"), match.ReleaseDate);

}

}
