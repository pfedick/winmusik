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

namespace de {
namespace pfp {
namespace winmusik {

PlaylistItem::PlaylistItem()
{
	titleId=0;
	startPositionSec=0.0f;
	endPositionSec=0.0f;
	musicKey=0;
	energyLevel=0;
	bpm=0;
	bpmPlayed=0;
	rating=0;
	trackLength=0;
	bitrate=0;
	mixLength=0.0f;
	for (int z=0;z < 5;z++) {
		cutStartPosition[z]=0.0f;
		cutEndPosition[z]=0.0f;
	}
	keyVerified=false;
	keyModification=0;
	DeviceId=0;
	DeviceTrack=0;
	DeviceType=0;
	DevicePage=0;
}

ppl7::String PlaylistItem::exportAsXML(int indention) const
{
	ppl7::String Indent, ret;
	Indent.repeat(" ", indention);
	ret=Indent + "<item>\n";
	ret+=Indent + "   <widgetId>" + ppl7::ToString("%tu", (ptrdiff_t)this) + "</widgetId>\n";
	ret+=Indent + "   <titleId>" + ppl7::ToString("%u", titleId) + "</titleId>\n";
	ret+=Indent + "   <startPositionSec>" + ppl7::ToString("%0.3f", startPositionSec) + "</startPositionSec>\n";
	ret+=Indent + "   <endPositionSec>" + ppl7::ToString("%0.3f", endPositionSec) + "</endPositionSec>\n";
	ret+=Indent + "   <cuts>\n";
	for (int i=0;i < 5;i++) {
		ret+=Indent + "      <cut><start>" + ppl7::ToString("%0.3f", cutStartPosition[i]) + "</start>";
		ret+="<end>" + ppl7::ToString("%0.3f", cutEndPosition[i]) + "</end></cut>\n";
	}
	ret+=Indent + "   </cuts>\n";
	ret+=Indent + "   <Artist>" + ppl7::EscapeHTMLTags(Artist) + "</Artist>\n";
	ret+=Indent + "   <Title>" + ppl7::EscapeHTMLTags(Title) + "</Title>\n";
	ret+=Indent + "   <Version>" + ppl7::EscapeHTMLTags(Version) + "</Version>\n";
	ret+=Indent + "   <Genre>" + ppl7::EscapeHTMLTags(Genre) + "</Genre>\n";
	ret+=Indent + "   <Label>" + ppl7::EscapeHTMLTags(Label) + "</Label>\n";
	ret+=Indent + "   <Album>" + ppl7::EscapeHTMLTags(Album) + "</Album>\n";
	ret+=Indent + "   <Remarks>" + ppl7::EscapeHTMLTags(Remarks) + "</Remarks>\n";
	ret+=Indent + "   <File>" + ppl7::EscapeHTMLTags(File) + "</File>\n";
	ret+=Indent + "   <musicKey verified=\"";
	if (keyVerified) ret+="true"; else ret+="false";
	ret+="\">" + ppl7::EscapeHTMLTags(MusicalKeys::sharpName(musicKey)) + "</musicKey>\n";
	ret+=Indent + "   <energyLevel>" + ppl7::ToString("%u", energyLevel) + "</energyLevel>\n";
	ret+=Indent + "   <bpm>" + ppl7::ToString("%u", bpm) + "</bpm>\n";
	ret+=Indent + "   <bitrate>" + ppl7::ToString("%u", bpm) + "</bitrate>\n";
	ret+=Indent + "   <bpmPlayed>" + ppl7::ToString("%u", bpmPlayed) + "</bpmPlayed>\n";
	ret+=Indent + "   <rating>" + ppl7::ToString("%u", rating) + "</rating>\n";
	ret+=Indent + "   <trackLength>" + ppl7::ToString("%u", trackLength) + "</trackLength>\n";
	ret+=Indent + "   <mixLength>" + ppl7::ToString("%0.3f", mixLength) + "</mixLength>\n";
	ret+=Indent + "</item>\n";
	return ret;
}

/*
void PlaylistItem::exportToArray(ppl7::AssocArray& a) const
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

void PlaylistItem::importFromXML(const ppl7::String& xml)
{
	std::vector<ppl7::String> Matches;
	if (ppl7::RegEx::capture("/\\<titleId\\>(.*)\\<\\/titleId\\>/s", xml, Matches)) titleId=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<startPositionSec\\>(.*)\\<\\/startPositionSec\\>/s", xml, Matches)) startPositionSec=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toFloat();
	if (ppl7::RegEx::capture("/\\<endPositionSec\\>(.*)\\<\\/endPositionSec\\>/s", xml, Matches)) endPositionSec=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toFloat();

	if (ppl7::RegEx::capture("/<cuts>(.*?)<\\/cuts>/s", xml, Matches)) {
		ppl7::Array cuts(Matches[1], "</cut>");
		size_t rows=cuts.size();
		if (rows > 5) rows=5;
		for (size_t i=0;i < rows;i++) {
			ppl7::String cut=cuts[i];
			//printf ("cut=>>%s<<\n",(const char*)cut);
			if (ppl7::RegEx::capture("/<start>(.*?)<\\/start>/s", cut, Matches)) cutStartPosition[i]=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toFloat();
			if (ppl7::RegEx::capture("/<end>(.*?)<\\/end>/s", cut, Matches)) cutEndPosition[i]=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toFloat();
		}
	}

	if (ppl7::RegEx::capture("/\\<Artist\\>(.*)\\<\\/Artist\\>/s", xml, Matches)) Artist=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/\\<Title\\>(.*)\\<\\/Title\\>/s", xml, Matches)) Title=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/\\<Version\\>(.*)\\<\\/Version\\>/s", xml, Matches)) Version=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/\\<Genre\\>(.*)\\<\\/Genre\\>/s", xml, Matches)) Genre=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/\\<Label\\>(.*)\\<\\/Label\\>/s", xml, Matches)) Label=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/\\<Album\\>(.*)\\<\\/Album\\>/s", xml, Matches)) Album=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/\\<Remarks\\>(.*)\\<\\/Remarks\\>/s", xml, Matches)) Remarks=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/\\<File\\>(.*)\\<\\/File\\>/s", xml, Matches)) File=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	keyVerified=false;
	if (ppl7::RegEx::capture("/\\<musicKey verified=\"true\"\\>/s", xml, Matches)) keyVerified=true;
	if (ppl7::RegEx::capture("/\\<musicKey.*?\\>(.*)\\<\\/musicKey\\>/s", xml, Matches)) musicKey=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<energyLevel\\>(.*)\\<\\/energyLevel\\>/s", xml, Matches)) energyLevel=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<bpm\\>(.*)\\<\\/bpm\\>/s", xml, Matches)) bpm=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<bpmPlayed\\>(.*)\\<\\/bpmPlayed\\>/s", xml, Matches)) bpmPlayed=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<rating\\>(.*)\\<\\/rating\\>/s", xml, Matches)) rating=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<trackLength\\>(.*)\\<\\/trackLength\\>/s", xml, Matches)) trackLength=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<mixLength\\>(.*)\\<\\/mixLength\\>/s", xml, Matches)) {
		printf("found: %s\n", (const char*)Matches[1]);
		mixLength=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toFloat();
	} else printf("no match in %s\n\n", (const char*)xml);

	if (ppl7::RegEx::capture("/\\<keyModification.*?\\>(.*)\\<\\/keyModification\\>/s", xml, Matches)) keyModification=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/\\<bitrate.*?\\>(.*)\\<\\/bitrate\\>/s", xml, Matches)) bitrate=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
}

Playlist::Playlist()
{
	IssueNumber=0;
}

Playlist::~Playlist()
{

}

void Playlist::clear()
{
	playlist.clear();
	IssueNumber=0;
	Name.clear();
	SubName.clear();
	IssueDate.setCurrentTime();
}

void Playlist::load(const ppl7::String& filename)
{
	clear();
	ppl7::String xml;
	ppl7::File::load(xml, filename);
	if (!xml.has("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")) throw InvalidXMLFileException(filename);
	if (!xml.has("<WinMusikPlaylist version=\"1\">")) throw InvalidPlaylistException(filename);
	std::vector<ppl7::String> Matches;
	if (!ppl7::RegEx::capture("/<WinMusikPlaylist.*?>(.*?)<tracks>/s", xml, Matches)) throw InvalidPlaylistException(filename);
	ppl7::String header=Matches[1];
	if (ppl7::RegEx::capture("/<name>(.*?)<\\/name>/s", xml, Matches)) Name=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/<subname>(.*?)<\\/subname>/s", xml, Matches)) SubName=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (ppl7::RegEx::capture("/<issue>(.*?)<\\/issue>/s", xml, Matches)) IssueNumber=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (ppl7::RegEx::capture("/<date>(.*?)<\\/date>/s", xml, Matches)) IssueDate.set(ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])));

	if (!ppl7::RegEx::capture("/<tracks>(.*?)<\\/tracks>/s", xml, Matches)) return;
	ppl7::Array tracks(Matches[1], "</item>");
	for (size_t i=0;i < tracks.size();i++) {
		xml=tracks[i].trimmed();
		if (xml.has("<item>")) {
			PlaylistItem item;
			item.importFromXML(xml);
			playlist.push_back(item);
		}
	}
}

void Playlist::setName(const ppl7::String& Name)
{
	this->Name=Name;
}
ppl7::String Playlist::getName() const
{
	return Name;
}

void Playlist::setSubName(const ppl7::String& Name)
{
	this->SubName=Name;
}

ppl7::String Playlist::getSubName() const
{
	return SubName;
}

void Playlist::setIssueNumber(int number)
{
	this->IssueNumber=number;
}

int Playlist::getIssueNumber() const
{
	return IssueNumber;
}

void Playlist::setIssueDate(const ppl7::DateTime& Date)
{
	this->IssueDate=Date;
}

ppl7::DateTime Playlist::getIssueDate() const
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
	int length=0;
	for (it=begin();it != end();++it) {
		length+=(*it).trackLength;
	}
	return length;
}

int Playlist::getTotalMixLength() const
{
	const_iterator it;
	int length=0;
	for (it=begin();it != end();++it) {
		length+=(*it).mixLength;
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






}
}
}	// EOF Namespace de.pfp.winmusik
