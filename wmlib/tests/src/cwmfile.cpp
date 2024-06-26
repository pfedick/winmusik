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
#include "wm_cwmfile.h"
#include "wm_exceptions.h"
#include "wmlib-tests.h"
#include <map>
#include "ppl7.h"

namespace {

using namespace de::pfp::winmusik;


class CWMFileTest : public ::testing::Test {
protected:
    CWMFileTest() {
        if (setlocale(LC_ALL, "") == NULL) {
            printf("setlocale fehlgeschlagen\n");
            throw std::exception();
        }
    }
    ~CWMFileTest()
    {

    }
};

class ChunkStats
{
public:
    unsigned int count;
    unsigned int size;
};

TEST_F(CWMFileTest, Parse) {
    CWMFile ff;
    ff.Open("testdata/winmusik.dat");
    CWMFileChunk chunk;
    int count=0;
    std::map<ppl7::String, ChunkStats> stats;
    while (ff.GetNextChunk(chunk)) {
        count++;
        ppl7::String name=chunk.GetChunkName();
        stats[name].count++;
        stats[name].size+=chunk.GetChunkDataSize();
        /*
        printf ("%s: %i Bytes, Timestamp: %u, Version: %u, Format: %u\n",
                chunk.GetChunkName(),
                chunk.GetChunkDataSize(),
                chunk.GetTimestamp(),
                chunk.GetVersion(),
                chunk.GetFormatVersion());
        */
    }
    printf("Number of Chunks: %u\n", count);
    std::map<ppl7::String, ChunkStats>::const_iterator it;
    for (it=stats.begin();it != stats.end();++it) {
        printf("    %s: Anzahl: %6u, Bytes: %9u\n",
            (const char*)it->first,
            it->second.count,
            it->second.size);

    }

}





} // EOF namespace
