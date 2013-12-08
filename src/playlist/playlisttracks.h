/*
 * playlisttracks.h
 *
 *  Created on: 08.12.2013
 *      Author: patrick
 */

#ifndef PLAYLISTTRACKS_H_
#define PLAYLISTTRACKS_H_

#include <QTreeWidget>
class Playlist;

class PlaylistTracks : public QTreeWidget
{
	private:
		void mouseMoveEvent ( QMouseEvent * event );
		void mousePressEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent * event );

		Playlist *playlist;

	public:
		PlaylistTracks(QWidget * parent = 0);
		void setPlaylist(Playlist *p);

};


#endif /* PLAYLISTTRACKS_H_ */
