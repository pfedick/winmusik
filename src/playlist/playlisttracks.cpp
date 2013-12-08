/*
 * playlisttracks.cpp
 *
 *  Created on: 08.12.2013
 *      Author: patrick
 */

#include "playlisttracks.h"
#include "playlist.h"

PlaylistTracks::PlaylistTracks(QWidget * parent)
	:QTreeWidget(parent)
{
	playlist=NULL;
}

void PlaylistTracks::mouseMoveEvent ( QMouseEvent * event )
{
	//printf ("PlaylistTracks::mouseMoveEvent\n");
	playlist->on_tracks_MouseMove(event);
	QTreeWidget::mouseMoveEvent(event);
}

void PlaylistTracks::mousePressEvent ( QMouseEvent * event )
{
	playlist->on_tracks_MouseButtonPress(event);
	//printf ("PlaylistTracks::mousePressEvent\n");
	QTreeWidget::mousePressEvent(event);
}

void PlaylistTracks::mouseReleaseEvent ( QMouseEvent * event )
{
	playlist->on_tracks_MouseButtonRelease(event);
	//printf ("PlaylistTracks::mouseReleaseEvent\n");
	QTreeWidget::mouseReleaseEvent(event);
}

void PlaylistTracks::setPlaylist(Playlist *p)
{
	playlist=p;
}
