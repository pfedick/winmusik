/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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

#ifndef WM_PLAYLIST_H_
#define WM_PLAYLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <ppl7.h>
#include <ppl7-audio.h>


namespace de {
namespace pfp {
namespace winmusik {


class PlaylistItem
{
public:
	uint32_t		titleId;
	float			startPositionSec;
	float			endPositionSec;
	float			cutStartPosition[5];
	float			cutEndPosition[5];
	ppl7::String	Artist;
	ppl7::String	Title;
	ppl7::String	Version;
	ppl7::String	Genre;
	ppl7::String	Label;
	ppl7::String	Album;
	ppl7::String	File;
	ppl7::String	Remarks;
	ppl7::ByteArray	CoverPreview;
	uint8_t			musicKey;
	uint32_t		bpm;
	uint32_t		bpmPlayed;
	uint8_t			rating;
	uint32_t		trackLength;
	float			mixLength;
	uint8_t			energyLevel;
	bool			keyVerified;
	int8_t          keyModification;
	uint16_t		bitrate;

	uint32_t		DeviceId;
	uint16_t		DeviceTrack;
	uint8_t			DeviceType;
	uint8_t			DevicePage;

	PlaylistItem();
	void clear();

	ppl7::String exportAsXML(int indention=3) const;
	ppl7::String getExistingFilename() const;
	void importFromXML(const ppl7::String& xml);
	void loadCoverPreview();
	float calcMixLength() const;
};

class Playlist
{
private:
	ppl7::String Name, SubName;
	ppl7::DateTime IssueDate;
	int IssueNumber;

	std::list<PlaylistItem> playlist;

public:

	PPL7EXCEPTION(InvalidXMLFileException, Exception)
		PPL7EXCEPTION(InvalidPlaylistException, Exception)

		typedef std::list<de::pfp::winmusik::PlaylistItem>::const_iterator const_iterator;

	Playlist();
	~Playlist();
	void clear();
	void load(const ppl7::String& filename);
	void save(const ppl7::String& filename);
	void setName(const ppl7::String& Name);
	ppl7::String getName() const;
	void setSubName(const ppl7::String& Name);
	ppl7::String getSubName() const;
	void setIssueNumber(int number);
	int getIssueNumber() const;
	void setIssueDate(const ppl7::DateTime& Date);
	ppl7::DateTime getIssueDate() const;
	int getTotalTracks() const;
	int getTotalLength() const;
	int getTotalMixLength() const;

	size_t size() const;

	Playlist::const_iterator begin() const;
	Playlist::const_iterator end() const;



};

}
}
}	// EOF Namespace de.pfp.winmusik

#endif /* WM_PLAYLIST_H_ */
