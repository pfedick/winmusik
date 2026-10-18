/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

namespace
{

class RegExpCaptureTest : public ::testing::Test
{
protected:
    RegExpCaptureTest()
    {
        if (setlocale(LC_ALL, "") == NULL) {
            printf("setlocale fehlgeschlagen\n");
            throw std::exception();
        }
    }
    ~RegExpCaptureTest()
    {
    }
};

TEST_F(RegExpCaptureTest, Beatport2023SingleArtist)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2023.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Over The Moon"), match.Title);
    ASSERT_EQ(pplib::String("Festival Mix"), match.Version);
    ASSERT_EQ(pplib::String("XiJaro & Pitch"), match.Artist);

    ASSERT_EQ(pplib::String("Dreamstate"), match.Label);
    ASSERT_EQ(pplib::String("Trance"), match.Genre);
    ASSERT_EQ(pplib::String("2023-07-14"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2023MultipleArtists)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2023_multi_artists.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Invincible"), match.Title);
    ASSERT_EQ(pplib::String("Extended Mix"), match.Version);
    ASSERT_EQ(pplib::String("Johan Gielen, Airscape feat. Sarah Anglionin"), match.Artist);

    ASSERT_EQ(pplib::String("Black Hole Recordings"), match.Label);
    ASSERT_EQ(pplib::String("Trance"), match.Genre);
    ASSERT_EQ(pplib::String("2023-07-07"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2023RemixWithArtist)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2023_remix.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Saltwater"), match.Title);
    ASSERT_EQ(pplib::String("Ilan Bluestone Extended Remix"), match.Version);
    ASSERT_EQ(pplib::String("Chicane feat. Moya Brennan"), match.Artist);

    ASSERT_EQ(pplib::String("Armada Music"), match.Label);
    ASSERT_EQ(pplib::String("Trance"), match.Genre);
    ASSERT_EQ(pplib::String("2023-09-01"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2023_2)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2023_2.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Somebody That I Used To Know"), match.Title);
    ASSERT_EQ(pplib::String("Extended Mix"), match.Version);
    ASSERT_EQ(pplib::String("Kevin McKay, James Cole"), match.Artist);

    ASSERT_EQ(pplib::String("Glasgow Underground"), match.Label);
    ASSERT_EQ(pplib::String("Deep House"), match.Genre);
    ASSERT_EQ(pplib::String("2023-08-04"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2023_3)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2023_3.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Delta"), match.Title);
    ASSERT_EQ(pplib::String("Original Mix"), match.Version);
    ASSERT_EQ(pplib::String("Deepcry"), match.Artist);

    ASSERT_EQ(pplib::String("Pure Progressive"), match.Label);
    ASSERT_EQ(pplib::String("Trance (Raw / Deep / Hypnotic)"), match.Genre);
    ASSERT_EQ(pplib::String("2023-12-08"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2024_1_pipe_in_genre)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2024_1_pipe_in_genre.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Attack"), match.Title);
    ASSERT_EQ(pplib::String("Extended Mix"), match.Version);
    ASSERT_EQ(pplib::String("Paul Sawyer, Fra.Gile, Deestopia"), match.Artist);

    ASSERT_EQ(pplib::String("Krafted Underground"), match.Label);
    ASSERT_EQ(pplib::String("Deep Trance"), match.Genre);
    ASSERT_EQ(pplib::String("2024-02-09"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2024_2)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2024_2.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Mechanismus"), match.Title);
    ASSERT_EQ(pplib::String("Original Mix"), match.Version);
    ASSERT_EQ(pplib::String("Tobi Tobermann, Dennis Nocken"), match.Artist);

    ASSERT_EQ(pplib::String("Volume Berlin Records"), match.Label);
    ASSERT_EQ(pplib::String("Raw Trance"), match.Genre);
    ASSERT_EQ(pplib::String("2024-04-05"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2024_NestedBrackets)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2024_brackets.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Lost In Dreams (UnKonscious 2024 Anthem)"), match.Title);
    ASSERT_EQ(pplib::String("Classix Extended Mix"), match.Version);
    ASSERT_EQ(pplib::String("Ferry Tayle, Tonks, Mirage (Fr)"), match.Artist);

    ASSERT_EQ(pplib::String("FSOE"), match.Label);
    ASSERT_EQ(pplib::String("Trance"), match.Genre);
    ASSERT_EQ(pplib::String("2024-01-26"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2025_1)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2025_1.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("What's a Girl to Do in '25 (Extended)"), match.Title);
    ASSERT_EQ(pplib::String("Extended"), match.Version);
    ASSERT_EQ(pplib::String("KI/KI"), match.Artist);

    ASSERT_EQ(pplib::String("Disorder"), match.Label);
    ASSERT_EQ(pplib::String(""), match.Genre);
    ASSERT_EQ(pplib::String("2025-09-12"), match.ReleaseDate);
}

TEST_F(RegExpCaptureTest, Beatport2025_2)
{
    pplib::String html;
    pplib::File::load(html, "testdata/beatport2025_2.xml");
    RegExpMatch match;
    ASSERT_TRUE(repexpmatch::matchBeatPort(html, match));

    ASSERT_EQ(pplib::String("Mind Body Soul"), match.Title);
    ASSERT_EQ(pplib::String("Eric Sneo Remix"), match.Version);
    ASSERT_EQ(pplib::String("Tom Wax"), match.Artist);

    ASSERT_EQ(pplib::String("Phuture Wax Records"), match.Label);
    ASSERT_EQ(pplib::String("Techno (Raw / Deep / Hypnotic)"), match.Genre);
    ASSERT_EQ(pplib::String("2025-07-25"), match.ReleaseDate);
}

} // namespace
