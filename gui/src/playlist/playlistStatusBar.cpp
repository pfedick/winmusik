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


#include "playlistStatusBar.h"
#include <QHBoxLayout>
#include "winmusik3.h"


PlaylistStatusBar::PlaylistStatusBar(QWidget* parent)
	: QFrame(parent)
{
	setupUi();
}

PlaylistStatusBar::~PlaylistStatusBar()
{

}

static void addSpacer(QLayout* layout)
{
	QFrame* line = new QFrame();
	line->setGeometry(QRect(320, 150, 118, 3));
	line->setFrameShape(QFrame::VLine);
	line->setFrameShadow(QFrame::Sunken);
	layout->addWidget(line);
}

static QHBoxLayout* createPanel(QLayout* layout)
{
	QHBoxLayout* l=new QHBoxLayout;
	l->setContentsMargins(1, 1, 1, 1);

	QFrame* frame=new QFrame;
	frame->setFrameShape(QFrame::Panel);
	frame->setFrameShadow(QFrame::Sunken);
	frame->setLayout(l);
	if (layout) layout->addWidget(frame);
	return l;
}


void PlaylistStatusBar::setupUi()
{
	QHBoxLayout* panel;
	QHBoxLayout* layout=new QHBoxLayout;
	layout->setContentsMargins(1, 1, 1, 1);
	QLabel* label;

	// Search
	setupSearch(layout);
	layout->addStretch(0);

	// MusicKey selection
	//panel=createPanel(layout);

	musicKeyLabel=new QLabel(tr("Musical Key:"));
	layout->addWidget(musicKeyLabel);
	displayMusicKey=new QComboBox();
	displayMusicKey->addItem(tr("musical sharps"));
	displayMusicKey->addItem(tr("musical flats"));
	displayMusicKey->addItem(tr("open key"));
	displayMusicKey->addItem(wm_main->conf.customMusicKeyName);
	layout->addWidget(displayMusicKey);
	connect(displayMusicKey, SIGNAL(currentIndexChanged(int)),
		this, SLOT(on_displayMusicKey_currentIndexChanged(int)));
	displayMusicKey->setEnabled(false);

	musicKeyDisplay=wm_main->conf.musicKeyDisplay;
	switch (musicKeyDisplay) {
	case musicKeyTypeMusicalSharps: displayMusicKey->setCurrentIndex(0); break;
	case musicKeyTypeMusicalFlats: displayMusicKey->setCurrentIndex(1); break;
	case musicKeyTypeOpenKey: displayMusicKey->setCurrentIndex(2); break;
	case musicKeyTypeCustom: displayMusicKey->setCurrentIndex(3); break;
	default: displayMusicKey->setCurrentIndex(2); break;
	}

	// Selection
	panel=createPanel(layout);

	// Selected Tracks
	label=new QLabel(tr("selected tracks:"));
	panel->addWidget(label);
	selectedTracks=new QLabel();
	selectedTracks->setFrameShadow(QFrame::Sunken);
	selectedTracks->setFrameShape(QFrame::StyledPanel);
	selectedTracks->setMinimumWidth(60);
	panel->addWidget(selectedTracks);
	addSpacer(panel);

	// Selected Track length
	label=new QLabel(tr("length:"));
	panel->addWidget(label);
	selectedTrackLength=new QLabel();
	selectedTrackLength->setFrameShadow(QFrame::Sunken);
	selectedTrackLength->setFrameShape(QFrame::StyledPanel);
	selectedTrackLength->setMinimumWidth(80);
	panel->addWidget(selectedTrackLength);
	addSpacer(panel);

	// Selected Mix length
	label=new QLabel(tr("mixLength:"));
	panel->addWidget(label);
	selectedMixLength=new QLabel();
	selectedMixLength->setFrameShadow(QFrame::Sunken);
	selectedMixLength->setFrameShape(QFrame::StyledPanel);
	selectedMixLength->setMinimumWidth(80);
	panel->addWidget(selectedMixLength);


	// Total
	panel=createPanel(layout);

	// Total Tracks
	label=new QLabel(tr("total tracks:"));
	panel->addWidget(label);
	totalTracks=new QLabel();
	totalTracks->setFrameShadow(QFrame::Sunken);
	totalTracks->setFrameShape(QFrame::StyledPanel);
	totalTracks->setMinimumWidth(60);
	panel->addWidget(totalTracks);
	addSpacer(panel);

	// Total Track length
	label=new QLabel(tr("length:"));
	panel->addWidget(label);
	totalTrackLength=new QLabel();
	totalTrackLength->setFrameShadow(QFrame::Sunken);
	totalTrackLength->setFrameShape(QFrame::StyledPanel);
	totalTrackLength->setMinimumWidth(80);
	panel->addWidget(totalTrackLength);
	addSpacer(panel);

	// Total Mix length
	label=new QLabel(tr("mixLength:"));
	panel->addWidget(label);
	totalMixLength=new QLabel();
	totalMixLength->setFrameShadow(QFrame::Sunken);
	totalMixLength->setFrameShape(QFrame::StyledPanel);
	totalMixLength->setMinimumWidth(80);
	panel->addWidget(totalMixLength);


	this->setContentsMargins(0, 0, 0, 0);
	this->setLayout(layout);
}

void PlaylistStatusBar::setupSearch(QLayout* layout)
{
	//QHBoxLayout *panel=createPanel(layout);
	searchWidget=new QWidget;
	QHBoxLayout* panel=new QHBoxLayout();
	panel->setContentsMargins(0, 0, 0, 0);
	searchWidget->setLayout(panel);

	layout->addWidget(searchWidget);

	QLabel* label;
	//panel->setEnabled(false);			// TODO: Enablen, wenn fertig implementiert
	label=new QLabel(tr("search:"));
	panel->addWidget(label);

	searchTextWidget=new QLineEdit;
	panel->addWidget(searchTextWidget);
	connect(searchTextWidget, SIGNAL(returnPressed()),
		this, SLOT(on_search_clicked()));

	searchTriggerButton=new QToolButton();
	searchTriggerButton->setIcon(QIcon(":/icons/resources/next.png"));
	panel->addWidget(searchTriggerButton);
	connect(searchTriggerButton, SIGNAL(clicked()),
		this, SLOT(on_search_clicked()));
}

void PlaylistStatusBar::ReloadTranslation()
{

}

void PlaylistStatusBar::setMusicKeySelectionEnabled(bool flag)
{
	musicKeyLabel->setEnabled(flag);
	displayMusicKey->setEnabled(flag);
}

void PlaylistStatusBar::setTotalLength(int sec)
{
	setReadableLength(totalTrackLength, sec);
}

void PlaylistStatusBar::setTotalTracks(int count)
{
	ppl7::String Tmp;
	Tmp.setf("%d", count);
	totalTracks->setText(Tmp);
}

void PlaylistStatusBar::setMixLength(int sec)
{
	setReadableLength(totalMixLength, sec);
}

void PlaylistStatusBar::setSelectedLength(int sec)
{
	setReadableLength(selectedTrackLength, sec);
}

void PlaylistStatusBar::setSelectedTracks(int count)
{
	ppl7::String Tmp;
	Tmp.setf("%d", count);
	selectedTracks->setText(Tmp);
}

void PlaylistStatusBar::setSelectedMixLength(int sec)
{
	setReadableLength(selectedMixLength, sec);
}


void PlaylistStatusBar::on_displayMusicKey_currentIndexChanged(int)
{
	switch (displayMusicKey->currentIndex()) {
	case 0: musicKeyDisplay=musicKeyTypeMusicalSharps; break;
	case 1: musicKeyDisplay=musicKeyTypeMusicalFlats; break;
	case 2: musicKeyDisplay=musicKeyTypeOpenKey; break;
	case 3: musicKeyDisplay=musicKeyTypeCustom; break;
	default: musicKeyDisplay=musicKeyTypeOpenKey; break;
	}
	emit musicKeySelectionChanged(musicKeyDisplay);
}

void PlaylistStatusBar::setFocusOnSearch()
{
	searchTextWidget->setFocus();
	searchTextWidget->deselect();
	searchTextWidget->selectAll();
}

void PlaylistStatusBar::setSearchEnabled(bool enable)
{
	searchWidget->setEnabled(enable);
}

void PlaylistStatusBar::setSearchVisible(bool visible)
{
	searchWidget->setVisible(visible);
}

QString PlaylistStatusBar::searchText() const
{
	return searchTextWidget->text();
}

void PlaylistStatusBar::on_search_clicked()
{
	emit searchTriggered();
}
