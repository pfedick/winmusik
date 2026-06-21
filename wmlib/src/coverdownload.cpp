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

#include <ppl7.h>
#include <ppl7-inet.h>
#include "wm_coverdownload.h"

namespace de
{
namespace pfp
{
namespace winmusik
{

void MetaData::print() const
{
    ppl7::PrintDebug("Artist:      %s\n", (const char*)Artist);
    ppl7::PrintDebug("Title:       %s\n", (const char*)Title);
    ppl7::PrintDebug("Version:     %s\n", (const char*)Version);
    ppl7::PrintDebug("Genre:       %s\n", (const char*)Genre);
    ppl7::PrintDebug("Label:       %s\n", (const char*)Label);
    ppl7::PrintDebug("Bpm:         %s\n", (const char*)Bpm);
    ppl7::PrintDebug("Album:       %s\n", (const char*)Album);
    ppl7::PrintDebug("ReleaseDate: %s\n", (const char*)ReleaseDate);
    ppl7::PrintDebug("ShopURL:     %s\n", (const char*)ShopURL);
    ppl7::PrintDebug("Key:         %s\n", (const char*)Key);
    ppl7::PrintDebug("CoverUrl:    %s\n", (const char*)CoverUrl);
    ppl7::PrintDebug("CoverFile:   %s\n", (const char*)CoverFile);
    ppl7::PrintDebug("Length:      %d sec = %0d:%02d min\n", Length, Length / 60, Length % 60);
}

bool CoverDownload(const ppl7::String& url, const ppl7::String& targetfile, MetaData& meta)
{
    meta.ShopURL = url;
    if (url.has(".beatport.com")) {
        return CoverDownload_Beatport(url, targetfile, meta);
    }

    return false;
}

bool CoverDownload_Beatport(const ppl7::String& url, const ppl7::String& targetfile, MetaData& meta)
{
    ppl7::HttpResponse response = ppl7::HttpClient::get(url);
    // ppl7::PrintDebug("HTTP-Response: %d\n", response.statusCode);
    if (response.statusCode != 200) return false;

    // response.body.toString().printnl();
    // ppl7::File ff("tmp/beatport.html", ppl7::File::WRITE);
    // ff.write(response.body);
    // ff.close();

    ppl7::RegEx::MatchVector match;
    /*
    if (ppl7::RegEx::capture(
            "/<meta\\s+property=\"og:image\"\\s+content=\"(https:\\/\\/geo-media\\.beatport\\.com\\/image_size\\/1400x1400\\/.*?)\"/",
            response.body, match)) {
        ppl7::PrintDebug("Cover gefunden: %s\n", (const char*)match[1]);
        ppl7::HttpResponse coverResponse = ppl7::HttpClient::get(match[1]);
        if (coverResponse.statusCode == 200) {
            ppl7::File ff(targetfile, ppl7::File::WRITE);
            ff.write(coverResponse.body);
            ff.close();
            meta.CoverFile = targetfile;

        } else {
            ppl7::PrintDebug("Fehler beim Download des Covers: HTTP %d\n", coverResponse.statusCode);
        }
    } else {
        ppl7::PrintDebug("Kein Cover gefunden im HTML.\n");
    }
    */

    if (ppl7::RegEx::capture("/script id=\"__NEXT_DATA__\" type=\"application\\/json\">(.*?)<\\/script>/", response.body, match)) {
        // ppl7::PrintDebug("Found __NEXT_DATA__ script tag, parsing JSON data...");
        // ppl7::File ff("tmp/data.json", ppl7::File::WRITE);
        // ff.write(match[1]);
        // ff.close();
        try {
            ppl7::AssocArray data = ppl7::Json::loads(match[1]);
            // data.list();
            if (data.exists("props/pageProps/track")) {
                const ppl7::AssocArray& track = data.getAssocArray("props/pageProps/track");
                if (track.exists("length_ms")) {
                    meta.Length = track.getString("length_ms").toInt() / 1000;
                }
                if (track.exists("mix_name")) {
                    meta.Version = track.getString("mix_name");
                }
                if (track.exists("name")) {
                    meta.Title = track.getString("name");
                }
                if (track.exists("new_release_date")) {
                    meta.ReleaseDate = track.getString("new_release_date");
                }
                if (track.exists("new_release_date")) {
                    meta.ReleaseDate = track.getString("new_release_date");
                }
                if (track.exists("bpm")) {
                    meta.Bpm = track.getString("bpm");
                }
                if (track.exists("genre/name")) {
                    meta.Genre = track.getString("genre/name");
                }
                if (track.exists("key/name")) {
                    meta.Key = track.getString("key/name");
                }
                if (track.exists("release/image/uri")) {
                    meta.CoverUrl = track.getString("release/image/uri");
                }
                if (track.exists("release/label/name")) {
                    meta.Label = track.getString("release/label/name");
                }
                if (track.exists("release/name")) {
                    meta.Album = track.getString("release/name");
                }
                if (track.exists("artists")) {
                    int i = 0;
                    while (track.exists(ppl7::ToString("artists/%d", i))) {
                        const ppl7::AssocArray& artist = track.getAssocArray(ppl7::ToString("artists/%d", i));
                        if (meta.Artist.size() > 0) meta.Artist += ", ";
                        meta.Artist.append(artist.get("name"));
                        i++;
                    }
                }
            }
        }
        catch (const ppl7::Exception& e) {
            e.print();
            ppl7::PrintDebug("Error parsing JSON: %s\n", e.what());
            throw;
        }
    }
    if (meta.CoverUrl.notEmpty()) {
        ppl7::HttpResponse coverResponse = ppl7::HttpClient::get(meta.CoverUrl);
        if (coverResponse.statusCode == 200) {
            // ppl7::PrintDebug("Cover download\n");
            ppl7::File ff(targetfile, ppl7::File::WRITE);
            ff.write(coverResponse.body);
            ff.close();
            meta.CoverFile = targetfile;
            meta.print();
            return true;
        } else {
            ppl7::PrintDebug("Cover download failed with: %s\n", (const char*)coverResponse.error);
        }
    }
    return false;
}

} // namespace winmusik
} // namespace pfp
} // namespace de
