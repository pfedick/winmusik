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


namespace de {
namespace pfp {
namespace winmusik {


class PlaylistItem
{
	public:
		ppluint32		titleId;
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
		ppluint32		DeviceId;
		ppluint16		DeviceTrack;
		ppluint8		DeviceType;
		ppluint8		DevicePage;

		PlaylistItem();
		void clear();

		ppl7::String exportAsXML(int indention=3) const;
		ppl7::String getExistingFilename() const;
		void importFromXML(const ppl7::String &xml);
		void loadCoverPreview();
		void updateFromDatabase();
		void updateMixLength();
		void useTraktorCues(const ppl7::String &file);
		void useTraktorCues(const ppl7::ID3Tag &Tag);
};

class Playlist
{
private:
public:
	Playlist();
	~Playlist();
	void load(const ppl7::String &filename);
	void save(const ppl7::String &filename);


};

}}}	// EOF Namespace de.pfp.winmusik

#endif /* LIB_INCLUDE_WM_PLAYLIST_H_ */
