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
#include "../../../ppl6/include/ppl7.h"

namespace {

using namespace de::pfp::winmusik;


class CWMFileTest : public ::testing::Test {
protected:
    CWMFileTest() {
        if (setlocale(LC_ALL,"")==NULL) {
            printf ("setlocale fehlgeschlagen\n");
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
    ff.Open("/home/patrick/Nextcloud/WinMusik3/winmusik.dat");
    CWMFileChunk chunk;
    int count=0;
    std::map<ppl7::String,ChunkStats> stats;
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
    printf ("Number of Chunks: %u\n",count);
    std::map<ppl7::String,ChunkStats>::const_iterator it;
    for (it=stats.begin();it!=stats.end();++it) {
        printf ("    %s: Anzahl: %6u, Bytes: %9u\n",
                (const char*)it->first,
                it->second.count,
                it->second.size);

    }

}





} // EOF namespace
