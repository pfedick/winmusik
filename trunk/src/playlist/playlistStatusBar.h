/*
 * playlistStatusBar.h
 *
 *  Created on: 01.01.2014
 *      Author: patrick
 */

#ifndef PLAYLISTSTATUSBAR_H_
#define PLAYLISTSTATUSBAR_H_

#include <QLabel>
#include <QFrame>
#include <QComboBox>
#include <QLayout>
#include <QLineEdit>
#include <QToolButton>




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
		QLineEdit *searchText;
		QToolButton *searchForward;
		QToolButton *searchBackward;

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

	signals:
		void musicKeySelectionChanged(int newValue);

	public slots:
		void on_displayMusicKey_currentIndexChanged(int);




};


#endif /* PLAYLISTSTATUSBAR_H_ */
