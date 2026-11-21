/*
 * This file is part of WinMusik 4 by Patrick Fedick
 *
 * Copyright (c) 2026, Patrick Fedick
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

#include <gtest/gtest.h>
#include "wm_musicalkeys.h"
#include "wmlib-tests.h"

namespace
{

using namespace de::pfp::winmusik;

class MusicalKeysTest : public ::testing::Test
{
protected:
    MusicalKeys keys;
};

TEST_F(MusicalKeysTest, FlatNamesIncludeMinorSuffixForMinorKeys)
{
    EXPECT_EQ(pplib::String("A♭m"), MusicalKeys::flatName(1));
    EXPECT_EQ(pplib::String("B"), MusicalKeys::flatName(2));
    EXPECT_EQ(pplib::String("E♭m"), MusicalKeys::flatName(3));
    EXPECT_EQ(pplib::String("G♭"), MusicalKeys::flatName(4));
    EXPECT_EQ(pplib::String("B♭m"), MusicalKeys::flatName(5));
    EXPECT_EQ(pplib::String("D♭"), MusicalKeys::flatName(6));
    EXPECT_EQ(pplib::String("A♭"), MusicalKeys::flatName(8));
    EXPECT_EQ(pplib::String("G♭m"), MusicalKeys::flatName(21));
    EXPECT_EQ(pplib::String("D♭m"), MusicalKeys::flatName(23));
    EXPECT_EQ(pplib::String("none"), MusicalKeys::flatName(25));
    EXPECT_EQ(pplib::String(""), MusicalKeys::flatName(0));
}

TEST_F(MusicalKeysTest, KeyIdResolvesCamelotAndOpenKeyWithoutCollisions)
{
    EXPECT_EQ(1, keys.keyId("6m"));
    EXPECT_EQ(1, keys.keyId("1A"));
    EXPECT_EQ(4, keys.keyId("2B"));
    EXPECT_EQ(4, keys.keyId("7d"));
    EXPECT_EQ(7, keys.keyId("4A"));
    EXPECT_EQ(7, keys.keyId("9m"));
    EXPECT_EQ(18, keys.keyId("9B"));
    EXPECT_EQ(18, keys.keyId("2d"));
}

TEST_F(MusicalKeysTest, KeyIdResolvesSharpFlatBeatportAndCustomNames)
{
    EXPECT_EQ(0, keys.keyId(""));
    EXPECT_EQ(0, keys.keyId("invalid"));
    EXPECT_EQ(1, keys.keyId("G♯m"));
    EXPECT_EQ(1, keys.keyId("Abm"));
    EXPECT_EQ(1, keys.keyId("Ab minor"));
    EXPECT_EQ(16, keys.keyId("C"));
    EXPECT_EQ(16, keys.keyId("C major"));

    keys.setCustomName("MyKeys");
    EXPECT_EQ(pplib::String("MyKeys"), keys.customName());
    keys.setCustomKeyName(15, "CustomAm");
    EXPECT_EQ(15, keys.keyId("CustomAm"));
    EXPECT_EQ(pplib::String("CustomAm"), keys.keyName(15, musicKeyTypeCustom));
}

TEST_F(MusicalKeysTest, ID3TagTkeyNamesUseAsciiAccidentals)
{
    EXPECT_EQ(pplib::String("G#m"), MusicalKeys::ID3TagTkeyNameSharp(1));
    EXPECT_EQ(pplib::String("Abm"), MusicalKeys::ID3TagTkeyNameFlat(1));
    EXPECT_EQ(pplib::String("o"), MusicalKeys::ID3TagTkeyNameSharp(25));
    EXPECT_EQ(pplib::String("o"), MusicalKeys::ID3TagTkeyNameFlat(25));
}

} // namespace
