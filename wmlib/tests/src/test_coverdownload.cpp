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
#include "wm_coverdownload.h"

#include "ppl7.h"
#include "wmlib-tests.h"

using namespace de::pfp::winmusik;

namespace
{

class CoverDownloadTest : public ::testing::Test
{
protected:
    CoverDownloadTest()
    {
        if (setlocale(LC_ALL, "") == NULL) {
            printf("setlocale fehlgeschlagen\n");
            throw std::exception();
        }
        ppl7::Dir::mkDir("tmp");
    }
    ~CoverDownloadTest()
    {
    }
};

TEST_F(CoverDownloadTest, Beatport2026SmallCover)
{
    //
    // https://www.beatport.com/de/track/waxed-future/26843848
    ppl7::String targetFile = "tmp/beatport2026_cover.jpg";
    ppl7::File::unlink(targetFile);
    MetaData meta;
    ASSERT_TRUE(CoverDownload("https://www.beatport.com/de/track/waxed-future/26843848", targetFile, meta));
    ASSERT_TRUE(ppl7::File::exists(targetFile));
}

} // namespace