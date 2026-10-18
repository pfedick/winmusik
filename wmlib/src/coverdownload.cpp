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

#include <pplib.h>
#include <pplib-inet.h>
#include "wm_coverdownload.h"

namespace de
{
namespace pfp
{
namespace winmusik
{

void MetaData::print() const
{
    pplib::PrintDebug("Artist:      %s\n", (const char*)Artist);
    pplib::PrintDebug("Title:       %s\n", (const char*)Title);
    pplib::PrintDebug("Version:     %s\n", (const char*)Version);
    pplib::PrintDebug("Genre:       %s\n", (const char*)Genre);
    pplib::PrintDebug("Label:       %s\n", (const char*)Label);
    pplib::PrintDebug("Bpm:         %s\n", (const char*)Bpm);
    pplib::PrintDebug("Album:       %s\n", (const char*)Album);
    pplib::PrintDebug("ReleaseDate: %s\n", (const char*)ReleaseDate);
    pplib::PrintDebug("ShopURL:     %s\n", (const char*)ShopURL);
    pplib::PrintDebug("Key:         %s\n", (const char*)Key);
    pplib::PrintDebug("CoverUrl:    %s\n", (const char*)CoverUrl);
    pplib::PrintDebug("CoverFile:   %s\n", (const char*)CoverFile);
    pplib::PrintDebug("Length:      %d sec = %0d:%02d min\n", Length, Length / 60, Length % 60);
}

static pplib::String beatport_clearance;
static pplib::String beatport_userAgent;
static bool beatport_debug = false;

void SetBeatportSession(const pplib::String& clearance, const pplib::String& userAgent)
{
    beatport_clearance = clearance;
    beatport_userAgent = userAgent;
}

bool CoverDownload(const pplib::String& url, const pplib::String& targetfile)
{
    if (url.has(".beatport.com")) {
        return CoverDownload_Beatport(url, targetfile);
    }
    return false;
}

bool CoverDownload(const pplib::String& url, const pplib::String& targetfile, MetaData& meta)
{
    meta.ShopURL = url;
    if (url.has(".beatport.com")) {
        return CoverDownload_Beatport(url, targetfile, meta);
    }

    return false;
}

void getBeatportMetaData(pplib::AssocArray& data, de::pfp::winmusik::MetaData& meta)
{
    if (data.exists("props/pageProps/track")) {
        if (beatport_debug) {
            pplib::PrintDebug("Found track data in JSON.");
        }
        const pplib::AssocArray& track = data.getAssocArray("props/pageProps/track");

        // 1. Titel & Mix
        if (track.exists("track_name")) {
            meta.Title = track.getString("track_name");
        } else if (track.exists("name")) {
            meta.Title = track.getString("name");
        }
        if (track.exists("mix_name")) {
            meta.Version = track.getString("mix_name");
        }

        // 2. Länge (ms -> s)
        if (track.exists("track_length_ms")) {
            meta.Length = track.getInt("track_length_ms") / 1000;
        } else if (track.exists("length_ms")) {
            meta.Length = track.getInt("length_ms") / 1000;
        }

        // 3. BPM
        if (track.exists("bpm")) {
            meta.Bpm = pplib::ToString("%d", track.getInt("bpm"));
        }

        // 4. Genre
        if (track.exists("genre/name")) {
            meta.Genre = track.getString("genre/name");
            meta.Genre.replace("Trance (Main Floor)", "Trance");
        }

        // 5. Key (früher Objekt key/name, jetzt direkt key)
        if (track.exists("key")) {
            meta.Key = track.getString("key");
        } else if (track.exists("key/name")) {
            meta.Key = track.getString("key/name");
        }
        // 6. Release & Datum
        if (track.exists("release/name")) {
            meta.Album = track.getString("release/name");
        }
        if (track.exists("release/release_date")) {
            meta.ReleaseDate = track.getString("release/release_date");
            // Schneidet das "T00:00:00" ab, falls du nur YYYY-MM-DD willst:
            int tPos = meta.ReleaseDate.instr("T");
            if (tPos > 0) meta.ReleaseDate = meta.ReleaseDate.left(tPos);
        } else if (track.exists("new_release_date")) {
            meta.ReleaseDate = track.getString("new_release_date");
        }

        // 7. Label (liegt jetzt direkt unter track, nicht mehr unter release!)
        if (track.exists("label/name")) {
            meta.Label = track.getString("label/name");
        } else if (track.exists("release/label/name")) {
            meta.Label = track.getString("release/label/name");
        }

        // 8. Cover-URL ({w}x{h} durch Wunschauflösung ersetzen!)
        if (track.exists("release/image_url")) {
            meta.CoverUrl = track.getString("release/image_url");
            meta.CoverUrl.replace("{w}x{h}", "1400x1400");
        } else if (track.exists("release/image/uri")) {
            meta.CoverUrl = track.getString("release/image/uri");
        }

        // 9. Artists
        if (track.exists("artists")) {
            int i = 0;
            while (track.exists(pplib::ToString("artists/%d", i))) {
                const pplib::AssocArray& artist = track.getAssocArray(pplib::ToString("artists/%d", i));
                if (meta.Artist.size() > 0) meta.Artist += ", ";
                meta.Artist.append(artist.get("name").toString());
                i++;
            }
        }
    }
}

bool CoverDownload_Beatport(const pplib::String& url, const pplib::String& targetfile, MetaData& meta)
{
    pplib::HttpRequest req;
    if (beatport_clearance.notEmpty()) {
        req.cookies["cf_clearance"] = beatport_clearance;
    }
    if (beatport_userAgent.notEmpty()) {
        req.userAgent = beatport_userAgent;
    }

    if (beatport_debug) {
        pplib::PrintDebug("Downloading Beatport cover from: %s\n", (const char*)url);
    }
    pplib::HttpResponse response = pplib::HttpClient::get(url, req);
    if (beatport_debug) {
        pplib::PrintDebug("HTTP-Response: %d\n", response.statusCode);
        pplib::PrintDebug("HTTP-Error: %s\n", (const char*)response.error);
        // pplib::PrintDebug("HTTP-Body: %s\n", (const char*)response.body);
        response.body.toString().printnl();
        pplib::File ff("tmp/beatport.html", pplib::File::FileMode::WRITE);
        ff.write(response.body);
        ff.close();
    }
    if (response.statusCode != 200) return false;

    pplib::RegEx::MatchVector match;

    if (pplib::RegEx::capture("/script id=\"__NEXT_DATA__\" type=\"application\\/json\">(.*?)<\\/script>/", pplib::String(response.body),
                              match)) {
        if (beatport_debug) {
            pplib::PrintDebug("Found __NEXT_DATA__ script tag, parsing JSON data...");
            pplib::File ff("tmp/data.json", pplib::File::FileMode::WRITE);
            ff.write(pplib::ByteArrayPtr(match[1]));
            ff.close();
        }
        try {
            pplib::AssocArray data = pplib::Json::loads(match[1]);
            if (beatport_debug) {
                pplib::PrintDebug("Parsed JSON data successfully.");
                data.list();
            }
            getBeatportMetaData(data, meta);
        }
        catch (const pplib::Exception& e) {
            e.print();
            pplib::PrintDebug("Error parsing JSON: %s\n", e.what());
            throw;
        }
    }
    if (meta.CoverUrl.notEmpty()) {
        pplib::HttpResponse coverResponse = pplib::HttpClient::get(meta.CoverUrl, req);
        if (coverResponse.statusCode == 200) {
            // pplib::PrintDebug("Cover download\n");
            pplib::File ff(targetfile, pplib::File::FileMode::WRITE);
            ff.write(coverResponse.body);
            ff.close();
            meta.CoverFile = targetfile;
            // meta.print();
            return true;
        } else {
            pplib::PrintDebug("Cover download failed with: %s\n", (const char*)coverResponse.error);
        }
    }
    return false;
}

bool CoverDownload_Beatport(const pplib::String& url, const pplib::String& targetfile)
{
    pplib::HttpRequest req;
    if (beatport_clearance.notEmpty()) {
        req.cookies["cf_clearance"] = beatport_clearance;
    }
    if (beatport_userAgent.notEmpty()) {
        req.userAgent = beatport_userAgent;
    }

    pplib::HttpResponse response = pplib::HttpClient::get(url, req);
    if (response.statusCode != 200) return false;
    pplib::RegEx::MatchVector match;
    pplib::String CoverUrl;

    if (pplib::RegEx::capture("/script id=\"__NEXT_DATA__\" type=\"application\\/json\">(.*?)<\\/script>/", pplib::String(response.body),
                              match)) {
        // pplib::PrintDebug("Found __NEXT_DATA__ script tag, parsing JSON data...");
        // pplib::File ff("tmp/data.json", pplib::File::FileMode::WRITE);
        // ff.write(match[1]);
        // ff.close();
        try {
            pplib::AssocArray data = pplib::Json::loads(match[1]);
            de::pfp::winmusik::MetaData meta;
            // getBeatportMetaData(data, meta);
            //  data.list();
            if (data.exists("props/pageProps/track")) {
                const pplib::AssocArray& track = data.getAssocArray("props/pageProps/track");
                if (track.exists("release/image_url")) {
                    meta.CoverUrl = track.getString("release/image_url");
                    meta.CoverUrl.replace("{w}x{h}", "1400x1400");
                } else if (track.exists("release/image/uri")) {
                    meta.CoverUrl = track.getString("release/image/uri");
                }
            } else if (data.exists("props/pageProps/release")) {

                const pplib::AssocArray& track = data.getAssocArray("props/pageProps/release");

                if (track.exists("release/image_url")) {
                    meta.CoverUrl = track.getString("release/image_url");
                    meta.CoverUrl.replace("{w}x{h}", "1400x1400");
                } else if (track.exists("release/image/uri")) {
                    meta.CoverUrl = track.getString("release/image/uri");
                }
            }
        }
        catch (const pplib::Exception& e) {
            e.print();
            pplib::PrintDebug("Error parsing JSON: %s\n", e.what());
            throw;
        }
    }
    if (CoverUrl.notEmpty()) {
        pplib::HttpResponse coverResponse = pplib::HttpClient::get(CoverUrl, req);
        if (coverResponse.statusCode == 200) {
            // pplib::PrintDebug("Cover download\n");
            pplib::File ff(targetfile, pplib::File::FileMode::WRITE);
            ff.write(coverResponse.body);
            ff.close();
            return true;
        } else {
            pplib::PrintDebug("Cover download failed with: %s\n", (const char*)coverResponse.error);
        }
    }
    return false;
}

} // namespace winmusik
} // namespace pfp
} // namespace de
