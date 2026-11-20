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

#include <pplib.h>
#include <pplib/inet/httpclient.h>
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
        pplib::Dir::mkDir("tmp");
    }
    ~CoverDownloadTest()
    {
    }
};

TEST_F(CoverDownloadTest, Beatport2026SmallCover)
{
    //
    // https://www.beatport.com/de/track/waxed-future/26843848
    pplib::String targetFile = "tmp/beatport2026_cover.jpg";
    pplib::File::unlink(targetFile);
    MetaData meta;
    ASSERT_TRUE(CoverDownload("https://www.beatport.com/de/track/waxed-future/26843848", targetFile, meta));
    ASSERT_TRUE(pplib::File::exists(targetFile));
}

TEST_F(CoverDownloadTest, Beatport2026TranceSubgenre)
{
    //
    // https://www.beatport.com/de/track/waxed-future/26843848
    pplib::String targetFile = "tmp/beatport2026_cover.jpg";
    pplib::File::unlink(targetFile);
    MetaData meta;
    ASSERT_TRUE(CoverDownload("https://www.beatport.com/de/track/chase-the-sun/30197900", targetFile, meta));
    ASSERT_TRUE(pplib::File::exists(targetFile));
    // meta.print();
    EXPECT_EQ(pplib::String("U-Mount"), meta.Artist);
    EXPECT_EQ(pplib::String("Chase The Sun"), meta.Title);
    EXPECT_EQ(pplib::String("Extended Mix"), meta.Version);
    EXPECT_EQ(pplib::String("Uplifting Trance"), meta.Genre);
    EXPECT_EQ(pplib::String("Extrema Global Music"), meta.Label);
    EXPECT_EQ(pplib::String("140"), meta.Bpm);
    EXPECT_EQ(pplib::String("Chase The Sun"), meta.Album);
    EXPECT_EQ(pplib::String("2026-09-18"), meta.ReleaseDate);
    EXPECT_EQ(pplib::String("https://www.beatport.com/de/track/chase-the-sun/30197900"), meta.ShopURL);
    EXPECT_EQ(pplib::String("G Minor"), meta.Key);
    EXPECT_EQ(pplib::String("https://geo-media.beatport.com/image_size/1400x1400/28d275ff-d812-4769-ab6d-26511ec5a99d.jpg"), meta.CoverUrl);
    EXPECT_EQ(pplib::String("tmp/beatport2026_cover.jpg"), meta.CoverFile);
    EXPECT_EQ(421, meta.Length);
}

TEST_F(CoverDownloadTest, Cloudflare2026Debug)
{
    pplib::HttpRequest req;
    const char* beatport_clearance = getenv("BEATPORT_CLEARANCE");
    if (beatport_clearance != nullptr && *beatport_clearance != '\0') {
        req.cookies["cf_clearance"] = beatport_clearance;
        // pplib::PrintDebug("Using Beatport clearance cookie: %s\n", beatport_clearance);
    }
    req.userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/157.0.0.0 Safari/537.36";

    req.headers["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/jxl,image/avif,image/webp,image/apng,*/"
                            "*;q=0.8,application/signed-exchange;v=b3;q=0.7";
    req.headers["Accept-Language"] = "de-DE,de;q=0.9,en-US;q=0.8,en;q=0.7";
    req.headers["Priority"] = "u=0, i";
    req.headers["Sec-CH-UA"] = "\"Not-A.Brand\";v=\"99\", \"Google Chrome\";v=\"157\", \"Chromium\";v=\"157\"";
    req.headers["Sec-CH-UA-Arch"] = "\"x86\"";
    req.headers["Sec-CH-UA-Bitness"] = "\"64\"";
    req.headers["Sec-CH-UA-Full-Version"] = "\"157.0.8103.80\"";
    req.headers["Sec-CH-UA-Full-Version-List"] =
        "\"Not-A.Brand\";v=\"99.0.0.0\", \"Google Chrome\";v=\"157.0.8103.80\", \"Chromium\";v=\"157.0.8103.80\"";
    req.headers["Sec-CH-UA-Mobile"] = "?0";
    req.headers["Sec-CH-UA-Model"] = "\"\"";
    req.headers["Sec-CH-UA-Platform"] = "\"Windows\"";
    req.headers["Sec-CH-UA-Platform-Version"] = "\"19.0.0\"";
    req.headers["Sec-Fetch-Dest"] = "document";
    req.headers["Sec-Fetch-Mode"] = "navigate";
    req.headers["Sec-Fetch-Site"] = "none";
    req.headers["Sec-Fetch-User"] = "?1";
    req.headers["Upgrade-Insecure-Requests"] = "1";

    pplib::String url = "https://www.beatport.com/de/track/luster-glow/31022495";

    pplib::PrintDebug("Downloading Beatport cover from: %s\n", (const char*)url);

    pplib::HttpResponse response = pplib::HttpClient::get(url, req);
    pplib::PrintDebug("HTTP-Response: %d\n", response.statusCode);
    pplib::PrintDebug("HTTP-Error: %s\n", (const char*)response.error);
    pplib::PrintDebug("HTTP-Headers:\n");
    for (auto& header : response.headers) {
        pplib::PrintDebug("    %s: %s\n", (const char*)header.first, (const char*)header.second);
    }
    // pplib::PrintDebug("HTTP-Body: %s\n", (const char*)response.body);
    // response.body.toString().printnl();
    pplib::File ff("tmp/beatport.html", pplib::File::FileMode::WRITE);
    ff.write(response.body);
    ff.close();
}

TEST_F(CoverDownloadTest, BeatportMetaDataFiltersRemixerIgnoreCase)
{
    pplib::AssocArray data = pplib::Json::loads(
        R"({"props":{"pageProps":{"track":{"track_name":"Silver Bath","mix_name":"Fish56Octagon Extended Remix","artists":[{"name":"Push"},{"name":"Plastic Boy"},{"name":"Fish56octagon"}]}}}})");
    MetaData meta;
    getBeatportMetaData(data, meta);
    EXPECT_EQ(pplib::String("Silver Bath"), meta.Title);
    EXPECT_EQ(pplib::String("Fish56Octagon Extended Remix"), meta.Version);
    EXPECT_EQ(pplib::String("Push, Plastic Boy"), meta.Artist);
}

TEST_F(CoverDownloadTest, BeatportMetaDataKeepsSelfRemixer)
{
    pplib::AssocArray data = pplib::Json::loads(
        R"({"props":{"pageProps":{"track":{"track_name":"Universal Nation","mix_name":"Push Extended Remix","artists":[{"name":"Push"}]}}}})");
    MetaData meta;
    getBeatportMetaData(data, meta);
    EXPECT_EQ(pplib::String("Push"), meta.Artist);
}

} // namespace