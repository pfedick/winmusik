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

#include "wm_playlist.h"
#include "wm_musicalkeys.h"

namespace de
{
namespace pfp
{
namespace winmusik
{

PlaylistItem::PlaylistItem()
{
    titleId = 0;
    startPositionSec = 0.0f;
    endPositionSec = 0.0f;
    musicKey = 0;
    energyLevel = 0;
    bpm = 0;
    bpmPlayed = 0;
    rating = 0;
    trackLength = 0;
    bitrate = 0;
    mixLength = 0.0f;
    for (int z = 0; z < 5; z++) {
        cutStartPosition[z] = 0.0f;
        cutEndPosition[z] = 0.0f;
    }
    keyVerified = false;
    keyModification = 0;
    DeviceId = 0;
    DeviceTrack = 0;
    DeviceType = 0;
    DevicePage = 0;
}

pplib::String PlaylistItem::exportAsXML(int indention) const
{
    pplib::String Indent, ret;
    Indent.repeat(" ", indention);
    ret = Indent + "<item>\n";
    ret += Indent + "   <widgetId>" + pplib::ToString("%tu", (ptrdiff_t)this) + "</widgetId>\n";
    ret += Indent + "   <titleId>" + pplib::ToString("%u", titleId) + "</titleId>\n";
    ret += Indent + "   <startPositionSec>" + pplib::ToString("%0.3f", startPositionSec) + "</startPositionSec>\n";
    ret += Indent + "   <endPositionSec>" + pplib::ToString("%0.3f", endPositionSec) + "</endPositionSec>\n";
    ret += Indent + "   <cuts>\n";
    for (int i = 0; i < 5; i++) {
        ret += Indent + "      <cut><start>" + pplib::ToString("%0.3f", cutStartPosition[i]) + "</start>";
        ret += "<end>" + pplib::ToString("%0.3f", cutEndPosition[i]) + "</end></cut>\n";
    }
    ret += Indent + "   </cuts>\n";
    ret += Indent + "   <Artist>" + pplib::EscapeHTMLTags(Artist) + "</Artist>\n";
    ret += Indent + "   <Title>" + pplib::EscapeHTMLTags(Title) + "</Title>\n";
    ret += Indent + "   <Version>" + pplib::EscapeHTMLTags(Version) + "</Version>\n";
    ret += Indent + "   <Genre>" + pplib::EscapeHTMLTags(Genre) + "</Genre>\n";
    ret += Indent + "   <Label>" + pplib::EscapeHTMLTags(Label) + "</Label>\n";
    ret += Indent + "   <Album>" + pplib::EscapeHTMLTags(Album) + "</Album>\n";
    ret += Indent + "   <Remarks>" + pplib::EscapeHTMLTags(Remarks) + "</Remarks>\n";
    ret += Indent + "   <File>" + pplib::EscapeHTMLTags(File) + "</File>\n";
    ret += Indent + "   <musicKey verified=\"";
    if (keyVerified)
        ret += "true";
    else
        ret += "false";
    ret += "\">" + pplib::EscapeHTMLTags(MusicalKeys::sharpName(musicKey)) + "</musicKey>\n";
    ret += Indent + "   <energyLevel>" + pplib::ToString("%u", energyLevel) + "</energyLevel>\n";
    ret += Indent + "   <bpm>" + pplib::ToString("%u", bpm) + "</bpm>\n";
    ret += Indent + "   <bitrate>" + pplib::ToString("%u", bpm) + "</bitrate>\n";
    ret += Indent + "   <bpmPlayed>" + pplib::ToString("%u", bpmPlayed) + "</bpmPlayed>\n";
    ret += Indent + "   <rating>" + pplib::ToString("%u", rating) + "</rating>\n";
    ret += Indent + "   <trackLength>" + pplib::ToString("%u", trackLength) + "</trackLength>\n";
    ret += Indent + "   <mixLength>" + pplib::ToString("%0.3f", mixLength) + "</mixLength>\n";
    ret += Indent + "</item>\n";
    return ret;
}

/*
void PlaylistItem::exportToArray(pplib::AssocArray& a) const
{
    a.setf("titleId", "%u", titleId);
    a.set("Artist", Artist);
    a.set("Title", Title);
    a.set("Version", Version);
    a.set("Genre", Genre);
    a.set("Label", Label);
    a.set("Album", Album);
    a.set("File", File);
    a.set("Remarks", Remarks);
    a.set("musicKey", MusicalKeys::sharpName(musicKey));
    a.setf("bpm", "%d", bpm);
    a.setf("bpmPlayed", "%d", bpmPlayed);
    a.setf("trackLength", "%d", trackLength);
    a.setf("mixLength", "%0.3f", mixLength);
    a.setf("startPositionSec", "%0.3f", startPositionSec);
    a.setf("endPositionSec", "%0.3f", endPositionSec);
}
*/

void PlaylistItem::importFromXML(const pplib::String& xml)
{
    std::vector<pplib::String> Matches;
    if (pplib::RegEx::capture("/\\<titleId\\>(.*)\\<\\/titleId\\>/s", xml, Matches))
        titleId = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<startPositionSec\\>(.*)\\<\\/startPositionSec\\>/s", xml, Matches))
        startPositionSec = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toFloat();
    if (pplib::RegEx::capture("/\\<endPositionSec\\>(.*)\\<\\/endPositionSec\\>/s", xml, Matches))
        endPositionSec = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toFloat();

    if (pplib::RegEx::capture("/<cuts>(.*?)<\\/cuts>/s", xml, Matches)) {
        pplib::Array cuts(Matches[1], "</cut>");
        size_t rows = cuts.size();
        if (rows > 5) rows = 5;
        for (size_t i = 0; i < rows; i++) {
            pplib::String cut = cuts[i];
            // printf ("cut=>>%s<<\n",(const char*)cut);
            if (pplib::RegEx::capture("/<start>(.*?)<\\/start>/s", cut, Matches))
                cutStartPosition[i] = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toFloat();
            if (pplib::RegEx::capture("/<end>(.*?)<\\/end>/s", cut, Matches))
                cutEndPosition[i] = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toFloat();
        }
    }

    if (pplib::RegEx::capture("/\\<Artist\\>(.*)\\<\\/Artist\\>/s", xml, Matches))
        Artist = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<Title\\>(.*)\\<\\/Title\\>/s", xml, Matches)) Title = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<Version\\>(.*)\\<\\/Version\\>/s", xml, Matches))
        Version = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<Genre\\>(.*)\\<\\/Genre\\>/s", xml, Matches)) Genre = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<Label\\>(.*)\\<\\/Label\\>/s", xml, Matches)) Label = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<Album\\>(.*)\\<\\/Album\\>/s", xml, Matches)) Album = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<Remarks\\>(.*)\\<\\/Remarks\\>/s", xml, Matches))
        Remarks = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/\\<File\\>(.*)\\<\\/File\\>/s", xml, Matches)) File = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    keyVerified = false;
    if (pplib::RegEx::capture("/\\<musicKey verified=\"true\"\\>/s", xml, Matches)) keyVerified = true;
    if (pplib::RegEx::capture("/\\<musicKey.*?\\>(.*)\\<\\/musicKey\\>/s", xml, Matches))
        musicKey = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<energyLevel\\>(.*)\\<\\/energyLevel\\>/s", xml, Matches))
        energyLevel = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<bpm\\>(.*)\\<\\/bpm\\>/s", xml, Matches)) bpm = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<bpmPlayed\\>(.*)\\<\\/bpmPlayed\\>/s", xml, Matches))
        bpmPlayed = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<rating\\>(.*)\\<\\/rating\\>/s", xml, Matches))
        rating = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<trackLength\\>(.*)\\<\\/trackLength\\>/s", xml, Matches))
        trackLength = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<mixLength\\>(.*)\\<\\/mixLength\\>/s", xml, Matches)) {
        printf("found: %s\n", (const char*)Matches[1]);
        mixLength = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toFloat();
    } else
        printf("no match in %s\n\n", (const char*)xml);

    if (pplib::RegEx::capture("/\\<keyModification.*?\\>(.*)\\<\\/keyModification\\>/s", xml, Matches))
        keyModification = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/\\<bitrate.*?\\>(.*)\\<\\/bitrate\\>/s", xml, Matches))
        bitrate = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
}

Playlist::Playlist()
{
    IssueNumber = 0;
}

Playlist::~Playlist()
{
}

void Playlist::clear()
{
    playlist.clear();
    IssueNumber = 0;
    Name.clear();
    SubName.clear();
    IssueDate.setCurrentTime();
}

void Playlist::load(const pplib::String& filename)
{
    clear();
    pplib::String xml;
    pplib::File::load(xml, filename);
    if (!xml.has("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")) throw InvalidXMLFileException(filename);
    if (!xml.has("<WinMusikPlaylist version=\"1\">")) throw InvalidPlaylistException(filename);
    std::vector<pplib::String> Matches;
    if (!pplib::RegEx::capture("/<WinMusikPlaylist.*?>(.*?)<tracks>/s", xml, Matches)) throw InvalidPlaylistException(filename);
    pplib::String header = Matches[1];
    if (pplib::RegEx::capture("/<name>(.*?)<\\/name>/s", xml, Matches)) Name = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/<subname>(.*?)<\\/subname>/s", xml, Matches)) SubName = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1]));
    if (pplib::RegEx::capture("/<issue>(.*?)<\\/issue>/s", xml, Matches))
        IssueNumber = pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])).toInt();
    if (pplib::RegEx::capture("/<date>(.*?)<\\/date>/s", xml, Matches)) IssueDate.set(pplib::Trim(pplib::UnescapeHTMLTags(Matches[1])));

    if (!pplib::RegEx::capture("/<tracks>(.*?)<\\/tracks>/s", xml, Matches)) return;
    pplib::Array tracks(Matches[1], "</item>");
    for (size_t i = 0; i < tracks.size(); i++) {
        xml = tracks[i].trimmed();
        if (xml.has("<item>")) {
            PlaylistItem item;
            item.importFromXML(xml);
            playlist.push_back(item);
        }
    }
}

void Playlist::setName(const pplib::String& Name)
{
    this->Name = Name;
}
pplib::String Playlist::getName() const
{
    return Name;
}

void Playlist::setSubName(const pplib::String& Name)
{
    this->SubName = Name;
}

pplib::String Playlist::getSubName() const
{
    return SubName;
}

void Playlist::setIssueNumber(int number)
{
    this->IssueNumber = number;
}

int Playlist::getIssueNumber() const
{
    return IssueNumber;
}

void Playlist::setIssueDate(const pplib::DateTime& Date)
{
    this->IssueDate = Date;
}

pplib::DateTime Playlist::getIssueDate() const
{
    return IssueDate;
}

size_t Playlist::size() const
{
    return playlist.size();
}

int Playlist::getTotalTracks() const
{
    return (int)playlist.size();
}

int Playlist::getTotalLength() const
{
    std::list<PlaylistItem>::const_iterator it;
    int length = 0;
    for (it = begin(); it != end(); ++it) {
        length += (*it).trackLength;
    }
    return length;
}

int Playlist::getTotalMixLength() const
{
    const_iterator it;
    int length = 0;
    for (it = begin(); it != end(); ++it) {
        length += (*it).mixLength;
    }
    return length;
}

Playlist::const_iterator Playlist::begin() const
{
    return playlist.begin();
}

Playlist::const_iterator Playlist::end() const
{
    return playlist.end();
}

} // namespace winmusik
} // namespace pfp
} // namespace de
