/*******************************************************************************
 * This file is part of WinMusik 4 by Patrick Fedick
 * Web: https://github.com/pfedick/winmusik
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
 *******************************************************************************/

#ifndef WM_PLAYLIST_H_
#define WM_PLAYLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <pplib.h>
#include <pplib-audio.h>

namespace de
{
namespace pfp
{
namespace winmusik
{

class PlaylistItem
{
public:
    uint32_t titleId;
    float startPositionSec;
    float endPositionSec;
    float cutStartPosition[5];
    float cutEndPosition[5];
    pplib::String Artist;
    pplib::String Title;
    pplib::String Version;
    pplib::String Genre;
    pplib::String Label;
    pplib::String Album;
    pplib::String File;
    pplib::String Remarks;
    pplib::ByteArray CoverPreview;
    uint8_t musicKey;
    uint32_t bpm;
    uint32_t bpmPlayed;
    uint8_t rating;
    uint32_t trackLength;
    float mixLength;
    uint8_t energyLevel;
    bool keyVerified;
    int8_t keyModification;
    uint16_t bitrate;

    uint32_t DeviceId;
    uint16_t DeviceTrack;
    uint8_t DeviceType;
    uint8_t DevicePage;

    PlaylistItem();
    void clear();

    pplib::String exportAsXML(int indention = 3) const;
    pplib::String getExistingFilename() const;
    void importFromXML(const pplib::String& xml);
    void loadCoverPreview();
    float calcMixLength() const;
};

class Playlist
{
private:
    pplib::String Name, SubName;
    pplib::DateTime IssueDate;
    int IssueNumber;

    std::list<PlaylistItem> playlist;

public:
    PPLIBEXCEPTION(InvalidXMLFileException, Exception)
    PPLIBEXCEPTION(InvalidPlaylistException, Exception)

    typedef std::list<de::pfp::winmusik::PlaylistItem>::const_iterator const_iterator;

    Playlist();
    ~Playlist();
    void clear();
    void load(const pplib::String& filename);
    void save(const pplib::String& filename);
    void setName(const pplib::String& Name);
    pplib::String getName() const;
    void setSubName(const pplib::String& Name);
    pplib::String getSubName() const;
    void setIssueNumber(int number);
    int getIssueNumber() const;
    void setIssueDate(const pplib::DateTime& Date);
    pplib::DateTime getIssueDate() const;
    int getTotalTracks() const;
    int getTotalLength() const;
    int getTotalMixLength() const;

    size_t size() const;

    Playlist::const_iterator begin() const;
    Playlist::const_iterator end() const;
};

} // namespace winmusik
} // namespace pfp
} // namespace de

#endif /* WM_PLAYLIST_H_ */
