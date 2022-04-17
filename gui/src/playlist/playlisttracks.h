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


#ifndef PLAYLISTTRACKS_H_
#define PLAYLISTTRACKS_H_

#include <QTreeWidget>
#include <QDomElement>
#include "winmusik3.h"

#include "ppl6-sound.h"

class Playlist;

class PlaylistItem : public QTreeWidgetItem
{
	public:
		ppluint32	titleId;
        float		startPositionSec;
        float		endPositionSec;
        float		cutStartPosition[5];
        float		cutEndPosition[5];
		ppl6::CString	Artist;
		ppl6::CString	Title;
		ppl6::CString	Version;
		ppl6::CString	Genre;
		ppl6::CString	Label;
		ppl6::CString	Album;
		ppl6::CString	File;
		ppl6::CString	Remarks;
		ppl6::CBinary	CoverPreview;
        ppluint8		musicKey;
        ppluint32		bpm;
        ppluint32		bpmPlayed;
        ppluint8		rating;
        ppluint32		trackLength;
        float			mixLength;
        ppluint8		energyLevel;
		bool			keyVerified;
        pplint8         keyModification;
        ppluint16       bitrate;

		ppluint32		DeviceId;
		ppluint16		DeviceTrack;
		ppluint8		DeviceType;
		ppluint8		DevicePage;




		PlaylistItem();
		ppl6::CString exportAsXML(int indention=3) const;
		ppl6::CString getExistingFilename() const;
		void importFromXML(QDomElement &e);
		void loadCoverPreview();
		void updateFromDatabase();
		void updateMixLength();
		void useTraktorCues(const ppl6::CString &file);
		void useTraktorCues(const ppl6::CID3Tag &Tag);
};


class PlaylistTracks : public QTreeWidget
{
		Q_OBJECT
	private:
		void mouseMoveEvent ( QMouseEvent * event );
		void mousePressEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent * event );
		QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;
		void dragEnterEvent ( QDragEnterEvent * event);
		void dragMoveEvent(QDragMoveEvent *e);
		bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
		void dropEvent ( QDropEvent * event );

		bool saveWMP(const ppl6::CString &Filename);
		bool loadWMP(const ppl6::CString &Filename);
		void loadWMPItem(QDomElement &e);

		void deleteSourceItems(QDropEvent * event);

		Playlist *playlist;
		PlaylistItem *lastmoveitem;

		ppl6::CString Name;
        ppl6::CString SubName;
        int IssueNumber;
        ppl6::CDateTime IssueDate;

	public:
        PlaylistTracks(QWidget * parent = nullptr);
		~PlaylistTracks();
		void setPlaylist(Playlist *p);

		void unselectItems();
		void deleteSelectedItems();
		void deleteItems(QList<QTreeWidgetItem *>items);
		void selectItems(QList<QTreeWidgetItem *>items);

		bool save(const ppl6::CString &Filename);
		bool load(const ppl6::CString &Filename);
		void setName(const ppl6::CString &Name);
		ppl6::CString getName() const;
        void setSubName(const ppl6::CString &Name);
        ppl6::CString getSubName() const;
        void setIssueNumber(int number);
        int getIssueNumber() const;
        void setIssueDate(const ppl6::CDateTime &Date);
        ppl6::CDateTime getIssueDate() const;


};


#endif /* PLAYLISTTRACKS_H_ */

