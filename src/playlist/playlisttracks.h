/*
 * playlisttracks.h
 *
 *  Created on: 08.12.2013
 *      Author: patrick
 */

#ifndef PLAYLISTTRACKS_H_
#define PLAYLISTTRACKS_H_

#include <QTreeWidget>
#include "winmusik3.h"

class Playlist;

class PlaylistItem : public QTreeWidgetItem
{
	public:
		ppluint32	titleId;
		int			startPositionSec;
		int			endPositionSec;
		int			cutStartPosition[5];
		int			cutEndPosition[5];
		ppl6::CString	Artist;
		ppl6::CString	Title;
		ppl6::CString	Version;
		ppl6::CString	Genre;
		ppl6::CString	Label;
		ppl6::CString	Album;
		ppl6::CString	File;
		int				musicKey;
		int				bpm;
		int				rating;
		int				length;

		PlaylistItem();
		ppl6::CString exportAsXML(int indention=3) const;
};


class PlaylistTracks : public QTreeWidget
{
	private:
		void mouseMoveEvent ( QMouseEvent * event );
		void mousePressEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent * event );

		void saveWMP(const ppl6::CString &Filename);

		Playlist *playlist;

		ppl6::CString Name;

	public:
		PlaylistTracks(QWidget * parent = 0);
		void setPlaylist(Playlist *p);

		void unselectItems();
		void deleteSelectedItems();
		void deleteItems(QList<QTreeWidgetItem *>items);

		void save(const ppl6::CString &Filename);
		void setName(const ppl6::CString &Name);

};


#endif /* PLAYLISTTRACKS_H_ */
