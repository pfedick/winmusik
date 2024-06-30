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


#ifndef PLAYLISTTRACKS_H_
#define PLAYLISTTRACKS_H_

#include <QTreeWidget>
#include <QDomElement>
#include "winmusik3.h"

#include "ppl7-audio.h"

class Playlist;

class PlaylistItem : public QTreeWidgetItem
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
	bool			cueConsistency;
	int8_t          keyModification;
	uint16_t		bitrate;

	uint32_t		DeviceId;
	uint16_t		DeviceTrack;
	uint8_t			DeviceType;
	uint8_t			DevicePage;




	PlaylistItem();
	ppl7::String exportAsXML(int indention=3) const;
	ppl7::String getExistingFilename() const;
	void importFromXML(QDomElement& e);
	void loadCoverPreview();
	void updateFromDatabase();
	void updateMixLength();
	void useTraktorCues(const ppl7::String& file);
	void useTraktorCues(const ppl7::ID3Tag& Tag);
};


class PlaylistTracks : public QTreeWidget
{
	Q_OBJECT
private:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const;
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* e);
	bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action);
	void dropEvent(QDropEvent* event);

	bool saveWMP(const ppl7::String& Filename);
	bool loadWMP(const ppl7::String& Filename);
	void loadWMPItem(QDomElement& e);

	void deleteSourceItems(QDropEvent* event);

	Playlist* playlist;
	PlaylistItem* lastmoveitem;

	ppl7::String Name;
	ppl7::String SubName;
	int IssueNumber;
	ppl7::DateTime IssueDate;

public:
	PlaylistTracks(QWidget* parent = nullptr);
	~PlaylistTracks();
	void setPlaylist(Playlist* p);

	void unselectItems();
	void deleteSelectedItems();
	void deleteItems(QList<QTreeWidgetItem*>items);
	void selectItems(QList<QTreeWidgetItem*>items);

	bool save(const ppl7::String& Filename);
	bool load(const ppl7::String& Filename);
	void setName(const ppl7::String& Name);
	ppl7::String getName() const;
	void setSubName(const ppl7::String& Name);
	ppl7::String getSubName() const;
	void setIssueNumber(int number);
	int getIssueNumber() const;
	void setIssueDate(const ppl7::DateTime& Date);
	ppl7::DateTime getIssueDate() const;


};


#endif /* PLAYLISTTRACKS_H_ */
