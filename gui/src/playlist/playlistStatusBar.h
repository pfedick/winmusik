/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/11/14 13:20:11 $
 * $Id: Playlist.h,v 1.1 2010/11/14 13:20:11 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
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

#ifndef PLAYLISTSTATUSBAR_H_
#define PLAYLISTSTATUSBAR_H_

#include <QLabel>
#include <QFrame>
#include <QComboBox>
#include <QLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QString>




class PlaylistStatusBar : public QFrame
{
		Q_OBJECT

	private:
		QLabel *totalTrackLength;
		QLabel *totalMixLength;
		QLabel *totalTracks;
		QLabel *selectedTrackLength;
		QLabel *selectedMixLength;
		QLabel *selectedTracks;
		QLabel *musicKeyLabel;
		QWidget *searchWidget;
		QLineEdit *searchTextWidget;
		QToolButton *searchTriggerButton;

		QComboBox *displayMusicKey;
		int	musicKeyDisplay;

		void setupUi();
		void setupSearch(QLayout *layout);

	public:
		PlaylistStatusBar(QWidget *parent=0);
		~PlaylistStatusBar();
		void ReloadTranslation();

		void setTotalLength(int sec);
		void setTotalTracks(int count);
		void setMixLength(int sec);

		void setSelectedLength(int sec);
		void setSelectedTracks(int count);
		void setSelectedMixLength(int sec);
		void setMusicKeySelectionEnabled(bool flag);

		void setFocusOnSearch();
		void setSearchEnabled(bool enable);
		void setSearchVisible(bool visible);

		const QString &searchText() const;

	signals:
		void musicKeySelectionChanged(int newValue);
		void searchTriggered();


	public slots:
		void on_displayMusicKey_currentIndexChanged(int);
		void on_search_clicked();



};


#endif /* PLAYLISTSTATUSBAR_H_ */
