#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ppl7.h>
#include <gtest/gtest.h>
#include "wm_regexpcapture.h"
#include "wmlib-tests.h"
#include <map>

namespace {

using namespace de::pfp::winmusik;


class RegExpCaptureTest : public ::testing::Test {
protected:
    RegExpCaptureTest() {
        if (setlocale(LC_ALL,"")==NULL) {
            printf ("setlocale fehlgeschlagen\n");
            throw std::exception();
        }
    }
    ~RegExpCaptureTest()
    {

    }
};

TEST_F(RegExpCaptureTest, load_save) {
    RegularExpressionCapture rec;
    ppl7::Dir::mkDir("tmp");
    if (ppl7::File::exists("tmp/regexp.conf"))
        ppl7::File::unlink("tmp/regexp.conf");
    rec.load("testdata/regexp.conf");
    ASSERT_EQ((size_t)15,rec.size());
    rec.save("tmp/regexp.conf");
}

TEST_F(RegExpCaptureTest, match_beatport_title_list) {
    ppl7::String html;
    ppl7::File::load(html, "testdata/beatport_titlelist_clipboard.html");
    
    RegularExpressionCapture rec;
    RegExpMatch match;
    ASSERT_TRUE(rec.match(html,match));
    ASSERT_EQ(ppl7::String("Philippe El Sisi"), match.Artist);
    ASSERT_EQ(ppl7::String("Pendulum"), match.Title);
    ASSERT_EQ(ppl7::String("Extended Mix"), match.Version);
    ASSERT_EQ(ppl7::String("Trance"), match.Genre);
    ASSERT_EQ(ppl7::String("FSOE"), match.Label);
    ASSERT_EQ(ppl7::String(""), match.Bpm);
    ASSERT_EQ(ppl7::String(""), match.Album);
    ASSERT_EQ(ppl7::String("2022-04-11"), match.ReleaseDate);
    ASSERT_EQ(0, match.Length);
}








} // EOF namespace
