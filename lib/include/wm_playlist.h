/*
 * cwmfile.h
 *
 *  Created on: 20.06.2016
 *      Author: patrick
 */

#ifndef LIB_INCLUDE_WM_PLAYLIST_H_
#define LIB_INCLUDE_WM_PLAYLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <ppl7.h>
#include <ppl7-audio.h>
#include <list>


namespace de {
namespace pfp {
namespace winmusik {


class PlaylistItem
{
	public:
		uint32_t		titleId;
		int				startPositionSec;
		int				endPositionSec;
		int				cutStartPosition[5];
		int				cutEndPosition[5];
		ppl7::String	Artist;
		ppl7::String	Title;
		ppl7::String	Version;
		ppl7::String	Genre;
		ppl7::String	Label;
		ppl7::String	Album;
		ppl7::String	File;
		ppl7::String	Remarks;
		ppl7::ByteArray	CoverPreview;
		int				musicKey;
		int				bpm;
		int				bpmPlayed;
		int				rating;
		int				trackLength;
		int				mixLength;
		int				energyLevel;
		bool			keyVerified;
		uint32_t		DeviceId;
		uint16_t		DeviceTrack;
		uint8_t		DeviceType;
		uint8_t		DevicePage;

		PlaylistItem();
		void clear();

		ppl7::String exportAsXML(int indention=3) const;
		ppl7::String getExistingFilename() const;
		void importFromXML(const ppl7::String &xml);
		void loadCoverPreview();

		float calcMixLength() const;
};

class Playlist
{
	private:
		ppl7::String Name,SubName;
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
		void load(const ppl7::String &filename);
		void save(const ppl7::String &filename);
		void setName(const ppl7::String &Name);
		ppl7::String getName() const;
		void setSubName(const ppl7::String &Name);
		ppl7::String getSubName() const;
		void setIssueNumber(int number);
		int getIssueNumber() const;
		void setIssueDate(const ppl7::DateTime &Date);
		ppl7::DateTime getIssueDate() const;
		int getTotalTracks() const;
		int getTotalLength() const;
		int getTotalMixLength() const;

		size_t size() const;

		Playlist::const_iterator begin() const;
		Playlist::const_iterator end() const;



};

}}}	// EOF Namespace de.pfp.winmusik

#endif /* LIB_INCLUDE_WM_PLAYLIST_H_ */
