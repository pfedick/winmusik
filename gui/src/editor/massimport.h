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


#ifndef MASSIMPORT_H
#define MASSIMPORT_H

#include <QWidget>
#include <QTimer>
#include "ui_massimport.h"
#include "winmusik3.h"
#include "tablecontrol.h"
#include <set>

class MassImport : public QDialog
{
	Q_OBJECT

public:
	MassImport(QWidget* parent = 0, CWmClient* wm=NULL);
	~MassImport();
	void ReloadTranslation();
	int load(uint8_t DeviceType, uint32_t DeviceId, uint8_t Page, uint16_t StartTrack);

	class TreeItem : public QTreeWidgetItem
	{
	public:
		TrackInfo		info;
		ppl7::String	Filename;
		int				dupePresumption;
		bool			import;
	};


	void setSearchWindow(QWidget* widget);
	QWidget* getSearchWindow() const;


private:
	Ui::massimportClass ui;
	CWmClient* wm;
	uint8_t DeviceType;
	uint32_t DeviceId;
	uint8_t Page;
	uint16_t StartTrack;
	std::set<ppl7::String> LocalDupeCheck;
	QWidget* searchWindow;
	TreeItem* currentTrackListItem;
	int			position, oldposition;

	CTrackList    TrackList;

	CTableControl TCVersion;
	CTableControl TCGenre;
	CTableControl TCLabel;
	CTableControl TCRecordSource;
	CTableControl TCRecordDevice;


	void resizeEvent(QResizeEvent* event);
	void showEvent(QShowEvent* event);
	void Resize();

	void addTrack(const ppl7::String Filename);

	void renderTrack(TreeItem* item);

	bool importTrack(TreeItem* item);
	void checkDupes(TreeItem* item);

protected:
	void InstallFilter(QObject* object, int id);
	bool eventFilter(QObject* target, QEvent* event);
	bool consumeEvent(QObject* target, QEvent* event);


public slots:
	void on_treeWidget_customContextMenuRequested(const QPoint& pos);
	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);

	void on_contextFindMoreVersions_triggered();
	void on_contextPlayTrack_triggered();
	void on_contextEditTrack_triggered();
	void on_contextDeleteTrack_triggered();

	void on_versionApplyButton_clicked();
	void on_genreApplyButton_clicked();
	void on_labelApplyButton_clicked();
	void on_recordingSourceApplyButton_clicked();
	void on_recordDeviceApplyButton_clicked();
	void on_albumApplyButton_clicked();
	void on_tagsApplyButton_clicked();
	void on_remarksApplyButton_clicked();
	void on_dateApplyButton_clicked();

	void on_markImportSelectedTracksButton_clicked();
	void on_markIgnoreSelectedTracksButton_clicked();
	void on_deleteSelectedTracksButton_clicked();
	void on_importSelectedTracksButton_clicked();

	void on_selectAllButton_clicked();
	void on_selectNoneButton_clicked();
	void on_exitButton_clicked();
	void on_startImportButton_clicked();


};


#endif // MASSIMPORT_H
