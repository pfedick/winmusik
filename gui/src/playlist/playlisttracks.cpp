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

#include "playlisttracks.h"
#include "playlist.h"
#include "wm_traktor.h"

#include <QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QDragMoveEvent>

using namespace de::pfp::winmusik;

PlaylistItem::PlaylistItem()
{
	titleId=0;
	startPositionSec=0.0f;
	endPositionSec=0.0f;
	musicKey=0;
	energyLevel=0;
	bpm=0;
	bpmPlayed=0;
	rating=0;
	trackLength=0;
	bitrate=0;
	mixLength=0.0f;
	for (int z=0;z < 5;z++) {
		cutStartPosition[z]=0.0f;
		cutEndPosition[z]=0.0f;
	}
	keyVerified=false;
	cueConsistency=true;
	keyModification=0;
	DeviceId=0;
	DeviceTrack=0;
	DeviceType=0;
	DevicePage=0;
}

PlaylistTracks::~PlaylistTracks()
{

}

ppl7::String PlaylistItem::exportAsXML(int indention) const
{
	ppl7::String Indent, ret;
	Indent.repeat(" ", indention);
	ret=Indent + "<item>\n";
	ret+=Indent + "   <widgetId>" + ppl7::ToString("%tu", (ptrdiff_t)this) + "</widgetId>\n";
	ret+=Indent + "   <titleId>" + ppl7::ToString("%u", titleId) + "</titleId>\n";
	ret+=Indent + "   <startPositionSec>" + ppl7::ToString("%0.3f", startPositionSec) + "</startPositionSec>\n";
	ret+=Indent + "   <endPositionSec>" + ppl7::ToString("%0.3f", endPositionSec) + "</endPositionSec>\n";
	ret+=Indent + "   <cuts>\n";
	for (int i=0;i < 5;i++) {
		ret+=Indent + "      <cut><start>" + ppl7::ToString("%0.3f", cutStartPosition[i]) + "</start>";
		ret+="<end>" + ppl7::ToString("%0.3f", cutEndPosition[i]) + "</end></cut>\n";
	}
	ret+=Indent + "   </cuts>\n";
	ret+=Indent + "   <Artist>" + ppl7::EscapeHTMLTags(Artist) + "</Artist>\n";
	ret+=Indent + "   <Title>" + ppl7::EscapeHTMLTags(Title) + "</Title>\n";
	ret+=Indent + "   <Version>" + ppl7::EscapeHTMLTags(Version) + "</Version>\n";
	ret+=Indent + "   <Genre>" + ppl7::EscapeHTMLTags(Genre) + "</Genre>\n";
	ret+=Indent + "   <Label>" + ppl7::EscapeHTMLTags(Label) + "</Label>\n";
	ret+=Indent + "   <Album>" + ppl7::EscapeHTMLTags(Album) + "</Album>\n";
	ret+=Indent + "   <Remarks>" + ppl7::EscapeHTMLTags(Remarks) + "</Remarks>\n";
	ret+=Indent + "   <File>" + ppl7::EscapeHTMLTags(File) + "</File>\n";
	ret+=Indent + "   <musicKey verified=\"";
	if (keyVerified) ret+="true"; else ret+="false";
	ret+="\">" + ppl7::EscapeHTMLTags(wm_main->MusicKeys.keyName(musicKey, musicKeyTypeMusicalSharps)) + "</musicKey>\n";
	ret+=Indent + "   <keyModification>" + ppl7::ToString("%d", keyModification) + "</keyModification>\n";
	ret+=Indent + "   <energyLevel>" + ppl7::ToString("%u", energyLevel) + "</energyLevel>\n";
	ret+=Indent + "   <bpm>" + ppl7::ToString("%u", bpm) + "</bpm>\n";
	ret+=Indent + "   <bitrate>" + ppl7::ToString("%u", bpm) + "</bitrate>\n";
	ret+=Indent + "   <bpmPlayed>" + ppl7::ToString("%u", bpmPlayed) + "</bpmPlayed>\n";
	ret+=Indent + "   <rating>" + ppl7::ToString("%u", rating) + "</rating>\n";
	ret+=Indent + "   <trackLength>" + ppl7::ToString("%u", trackLength) + "</trackLength>\n";
	ret+=Indent + "   <mixLength>" + ppl7::ToString("%0.3f", mixLength) + "</mixLength>\n";
	ret+=Indent + "   <cueConsistency>";
	if (cueConsistency) ret+="true"; else ret+="false";
	ret+= "</cueConsistency>\n";
	ret+=Indent + "</item>\n";
	return ret;
}

ppl7::String PlaylistItem::getExistingFilename() const
{
	if (File.notEmpty() && ppl7::File::exists(File)) return File;
	return wm_main->GetAudioFilename(DeviceType, DeviceId, DevicePage, DeviceTrack);
}

void PlaylistItem::importFromXML(QDomElement& e)
{
	ppl7::String Tmp;

	QDomNode node =e.namedItem("titleId");
	if (node.isNull() == false && node.isElement() == true) {
		titleId=node.toElement().text().toUInt();
	}

	node =e.namedItem("deviceType");
	if (node.isNull() == false && node.isElement() == true) {
		DeviceType=static_cast<uint8_t>(node.toElement().text().toUInt());
	}
	node =e.namedItem("deviceId");
	if (node.isNull() == false && node.isElement() == true) {
		DeviceId=node.toElement().text().toUInt();
	}
	node =e.namedItem("devicePage");
	if (node.isNull() == false && node.isElement() == true) {
		DevicePage=static_cast<uint8_t>(node.toElement().text().toUInt());
	}
	node =e.namedItem("deviceTrack");
	if (node.isNull() == false && node.isElement() == true) {
		DeviceTrack=node.toElement().text().toUShort();
	}


	node =e.namedItem("startPositionSec");
	if (node.isNull() == false && node.isElement() == true) {
		startPositionSec=node.toElement().text().toFloat();
	}
	node =e.namedItem("endPositionSec");
	if (node.isNull() == false && node.isElement() == true) {
		endPositionSec=node.toElement().text().toFloat();
	}
	node =e.namedItem("trackLength");
	if (node.isNull() == false && node.isElement() == true) {
		trackLength=node.toElement().text().toUInt();
	}
	node =e.namedItem("Artist");
	if (node.isNull() == false && node.isElement() == true) {
		Artist=node.toElement().text();
	}
	node =e.namedItem("Title");
	if (node.isNull() == false && node.isElement() == true) {
		Title=node.toElement().text();
	}
	node =e.namedItem("Version");
	if (node.isNull() == false && node.isElement() == true) {
		Version=node.toElement().text();
	}
	node =e.namedItem("Genre");
	if (node.isNull() == false && node.isElement() == true) {
		Genre=node.toElement().text();
	}
	node =e.namedItem("Label");
	if (node.isNull() == false && node.isElement() == true) {
		Label=node.toElement().text();
	}
	node =e.namedItem("Album");
	if (node.isNull() == false && node.isElement() == true) {
		Album=node.toElement().text();
	}
	node =e.namedItem("Remarks");
	if (node.isNull() == false && node.isElement() == true) {
		Remarks=node.toElement().text();
	}

	node =e.namedItem("File");
	if (node.isNull() == false && node.isElement() == true) {
		File=node.toElement().text();
	}
	node =e.namedItem("musicKey");
	if (node.isNull() == false && node.isElement() == true) {
		musicKey=wm_main->MusicKeys.keyId(node.toElement().text());
		Tmp=node.toElement().attribute("verified", "false");
		keyVerified=Tmp.toBool();
	}
	node =e.namedItem("keyModification");
	if (node.isNull() == false && node.isElement() == true) {
		keyModification=static_cast<int8_t>(node.toElement().text().toInt());
	}

	node =e.namedItem("energyLevel");
	if (node.isNull() == false && node.isElement() == true) {
		energyLevel=static_cast<uint8_t>(node.toElement().text().toUInt());
	}

	node =e.namedItem("bpm");
	if (node.isNull() == false && node.isElement() == true) {
		bpm=node.toElement().text().toUInt();
	}
	node =e.namedItem("bpmPlayed");
	if (node.isNull() == false && node.isElement() == true) {
		bpmPlayed=node.toElement().text().toUInt();
	}
	if (!bpmPlayed) bpmPlayed=bpm;

	node =e.namedItem("bitrate");
	if (node.isNull() == false && node.isElement() == true) {
		bitrate=node.toElement().text().toUInt();
	}

	node =e.namedItem("cueConsistency");
	if (node.isNull() == false && node.isElement() == true) {
		cueConsistency=ppl7::IsTrue(node.toElement().text());
	}
	node =e.namedItem("rating");
	if (node.isNull() == false && node.isElement() == true) {
		rating=static_cast<uint8_t>(node.toElement().text().toUInt());
	}

	mixLength=endPositionSec - startPositionSec;
	QDomNode cuts=e.namedItem("cuts");
	int c=0;
	if (cuts.isNull() == false) {
		//printf ("   Parsing cuts...\n");
		QDomElement e=cuts.firstChildElement("cut");
		while (!e.isNull()) {
			//printf ("      Parsing cut...\n");
			node =e.namedItem("start");
			if (node.isNull() == false && node.isElement() == true) {
				cutStartPosition[c]=node.toElement().text().toFloat();
				//printf ("       start found\n");
			}
			node =e.namedItem("end");
			if (node.isNull() == false && node.isElement() == true) {
				cutEndPosition[c]=node.toElement().text().toFloat();
				//printf ("       end found\n");
			}
			mixLength-=(cutEndPosition[c] - cutStartPosition[c]);
			c++;
			if (c > 4) break;
			e=e.nextSiblingElement("cut");
		}
	}
	if (bpm > 0 && bpmPlayed > 0 && bpmPlayed != bpm) mixLength=mixLength * bpm / bpmPlayed;

	if (endPositionSec == 0.0f) {
		//printf ("endPositionSec==0\n");
		this->useTraktorCues(File);
		if (endPositionSec == 0.0f) endPositionSec=trackLength;
		updateMixLength();
	}
	if (titleId == 0) titleId=findTitleIdByFilename(File);
	loadCoverPreview();
}

void PlaylistItem::updateMixLength()
{
	mixLength=endPositionSec - startPositionSec;
	for (int i=0;i < 5;i++) mixLength-=(cutEndPosition[i] - cutStartPosition[i]);
	if (bpm > 0 && bpmPlayed > 0 && bpmPlayed != bpm) {
		float perc=(float)bpmPlayed * 100.0f / (float)bpm;
		mixLength=mixLength * 100.0f / perc;
	}
}

void PlaylistItem::loadCoverPreview()
{
	// Cover laden
	if (titleId > 0) {
		const DataTitle* ti=wm_main->GetTitle(titleId);
		if (ti) {
			//printf ("Cover-Preview from Database\n");
			CoverPreview=ti->CoverPreview;
			return;
		}
	}
	if (CoverPreview.size() == 0 && File.size() > 0) {
		TrackInfo info;
		if (getTrackInfoFromFile(info, File)) {
			//printf ("Cover-Preview from File\n");
			CoverPreview=info.Ti.CoverPreview;
		}
	}
}

void PlaylistItem::updateFromDatabase()
{
	if (titleId == 0) return;
	const DataTitle* ti=wm_main->GetTitle(titleId);
	if (!ti) return;
	CoverPreview=ti->CoverPreview;
	trackLength=ti->Length;
	Artist=ti->Artist;
	Title=ti->Title;
	Album=ti->Album;
	musicKey=ti->Key;
	energyLevel=ti->EnergyLevel;
	bitrate=ti->Bitrate;
	bpm=ti->BPM;
	if (!bpmPlayed) bpmPlayed=0;
	rating=ti->Rating;
	keyVerified=(ti->Flags >> 4) & 1;
	Version=wm_main->GetVersionText(ti->VersionId);
	Genre=wm_main->GetGenreText(ti->GenreId);
	Label=wm_main->GetLabelText(ti->LabelId);
	DeviceId=ti->DeviceId;
	DeviceTrack=ti->Track;
	DeviceType=ti->DeviceType;
	DevicePage=ti->Page;
	if (File.notEmpty()) {
		if (ppl7::File::exists(File)) return;
	}
	File=wm_main->GetAudioFilename(ti->DeviceType, ti->DeviceId, ti->Page, ti->Track);
}

void PlaylistItem::useTraktorCues(const ppl7::String& file)
{
	ppl7::ID3Tag Tag;
	try {
		Tag.load(file);
		useTraktorCues(Tag);
	} catch (...) {}
}

void PlaylistItem::useTraktorCues(const ppl7::ID3Tag& Tag)
{
	ppl7::String Tmp;
	std::list <TraktorTagCue> cuelist;
	std::list <TraktorTagCue>::const_iterator it;
	getTraktorCues(cuelist, Tag);
	if (cuelist.size() == 0) return;
	for (it=cuelist.begin();it != cuelist.end();it++) {
		float sec=static_cast<float>(it->start / 1000.0);
		if (it->type == TraktorTagCue::IN) startPositionSec=sec;
		if (it->type == TraktorTagCue::OUT) endPositionSec=sec;
	}
	updateMixLength();
}

PlaylistTracks::PlaylistTracks(QWidget* parent)
	:QTreeWidget(parent)
{
	playlist=nullptr;
	lastmoveitem=nullptr;
	IssueNumber=0;
	IssueDate=ppl7::DateTime::currentTime();
}

void PlaylistTracks::mouseMoveEvent(QMouseEvent* event)
{
	//printf ("PlaylistTracks::mouseMoveEvent\n");
	//playlist->on_tracks_MouseMove(event);
	QTreeWidget::mouseMoveEvent(event);
}

void PlaylistTracks::mousePressEvent(QMouseEvent* event)
{
	playlist->on_tracks_MouseButtonPress(event);
	//printf ("PlaylistTracks::mousePressEvent\n");
	QTreeWidget::mousePressEvent(event);
}

void PlaylistTracks::mouseReleaseEvent(QMouseEvent* event)
{
	playlist->on_tracks_MouseButtonRelease(event);
	//printf ("PlaylistTracks::mouseReleaseEvent\n");
	QTreeWidget::mouseReleaseEvent(event);
}


QMimeData* PlaylistTracks::mimeData(const QList<QTreeWidgetItem*>) const
{
	QList<QUrl> list;
	QPixmap Icon;
	ppl7::String xml;
	xml="<winmusikTracklist>\n";
	xml+="<tracks>\n";
	/* Order of items in the list to this function is not always identical to the order in
	 * the tree widget. Therefore we use selectedItems()
	 */
	QList<QTreeWidgetItem*> items=this->selectedItems();
	for (int i=0;i < items.size();i++) {
		PlaylistItem* item=static_cast<PlaylistItem*>(items[i]);
		//PlaylistItem *item=static_cast<PlaylistItem *>(topLevelItem[i]);
		if (Icon.isNull()) {
			Icon.loadFromData(static_cast<const uchar*>(item->CoverPreview.ptr()),
				static_cast<unsigned int>(item->CoverPreview.size()));
		}
		xml+=item->exportAsXML(0);

#ifdef _WIN32
		list.append(QUrl::fromLocalFile(item->File));
#else
		list.append(QUrl::fromLocalFile(item->File));
#endif
		//printf("File: %s\n",(const char*)item->File);
	}
	xml+="</tracks>\n";
	xml+="</winmusikTracklist>\n";

	//xml.Print(true);

	QMimeData* mimeData = new QMimeData;
	QByteArray ba(static_cast<const char*>(xml), static_cast<int>(xml.size()));
	mimeData->setData("application/winmusik+xml", ba);
	//mimeData->setText(xml);
	mimeData->setUrls(list);
	mimeData->setImageData(Icon);
	return mimeData;
}

void PlaylistTracks::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept();
}

void PlaylistTracks::dragMoveEvent(QDragMoveEvent* e)
{
	e->accept();
	//QTreeWidget::dragMoveEvent(e);
	//return;
	PlaylistItem* item = static_cast<PlaylistItem*>(itemAt(e->position().toPoint()));
	if (item) {
		if (item != lastmoveitem) {
			unselectItems();
			//if (lastmoveitem) lastmoveitem->setSelected(false);
			this->scrollToItem(item);
			lastmoveitem=item;
		}
		item->setSelected(true);
	} else if (lastmoveitem) {
		//lastmoveitem->setSelected(false);
		unselectItems();
		lastmoveitem=nullptr;
	}

	//e->ignore();
	//QTreeWidget::dragMoveEvent(e);
}

bool PlaylistTracks::dropMimeData(QTreeWidgetItem* parent, int, const QMimeData* data, Qt::DropAction)
{
	//printf ("PlaylistTracks::dropMimeData\n");
	ppl7::String Tmp;
	playlist->handleDropEvent(data, parent);
	lastmoveitem=nullptr;
	return true;
}

void PlaylistTracks::dropEvent(QDropEvent* event)
{
	//printf ("PlaylistTracks::dropEvent, action: %i\n",event->dropAction());
	if (lastmoveitem) {
		lastmoveitem->setSelected(false);
		lastmoveitem=nullptr;
	}
	event->acceptProposedAction();
	playlist->handleDropEvent(event);
	if (event->source() == this) {
		//printf ("Quelle ist gleich\n");
		if (event->dropAction() == 1) {
			deleteSourceItems(event);
			playlist->renumberTracks();
			playlist->updateLengthStatus();
		}
	}
}

void PlaylistTracks::deleteSourceItems(QDropEvent* event)
{
	//printf("PlaylistTracks::deleteSourceItems\n");
	const QMimeData* mime=event->mimeData();
	if (!mime) return;
	//ppl7::String xml=mime->text();
	QByteArray ba=mime->data("application/winmusik+xml");
	ppl7::String xml;
	xml.set(ba.constData(), ba.length());

	//printf("debug 1\n");
	//xml.Print(true);
	if (xml.left(18) != "<winmusikTracklist") return;
	//printf ("winmusikTracklist\n");
	QDomDocument doc("winmusikTracklist");
	if (doc.setContent(QString(xml))) {
		QDomElement root=doc.documentElement();
		if (root.tagName() == "winmusikTracklist") {
			QDomNode tracks=root.namedItem("tracks");
			if (tracks.isNull() == false) {
				QDomElement e=tracks.firstChildElement("item");
				while (!e.isNull()) {
					QDomNode node =e.namedItem("widgetId");
					if (node.isNull() == false && node.isElement() == true) {
						QTreeWidgetItem* item=(QTreeWidgetItem*)node.toElement().text().toULongLong();
						int id=indexOfTopLevelItem(item);
						//						printf ("delete Widget with id=%i\n",id);
						if (item != nullptr && id >= 0) this->takeTopLevelItem(id);
					}
					e=e.nextSiblingElement("item");
				}
			}
		}
	}
}


void PlaylistTracks::setPlaylist(Playlist* p)
{
	playlist=p;
}

void PlaylistTracks::unselectItems()
{
	QList<QTreeWidgetItem*>selected=selectedItems();
	for (int i=0;i < selected.size();i++) selected.at(i)->setSelected(false);
}

void PlaylistTracks::deleteSelectedItems()
{
	QList<QTreeWidgetItem*>selected=selectedItems();
	for (int i=0;i < selected.size();i++) {
		PlaylistItem* item=static_cast<PlaylistItem*>(takeTopLevelItem(indexOfTopLevelItem(selected.at(i))));
		delete item;
	}
}

void PlaylistTracks::deleteItems(QList<QTreeWidgetItem*>items)
{
	for (int i=0;i < items.size();i++) {
		PlaylistItem* item=static_cast<PlaylistItem*>(takeTopLevelItem(indexOfTopLevelItem(items.at(i))));
		delete item;
	}
}

void PlaylistTracks::selectItems(QList<QTreeWidgetItem*>items)
{
	for (int i=0;i < items.size();i++) {
		items.at(i)->setSelected(true);
	}
}

void PlaylistTracks::setName(const ppl7::String& Name)
{
	this->Name=Name;
}

ppl7::String PlaylistTracks::getName() const
{
	return Name;
}

void PlaylistTracks::setSubName(const ppl7::String& Name)
{
	this->SubName=Name;
}

ppl7::String PlaylistTracks::getSubName() const
{
	return SubName;
}

void PlaylistTracks::setIssueNumber(int number)
{
	this->IssueNumber=number;
}

int PlaylistTracks::getIssueNumber() const
{
	return IssueNumber;
}

void PlaylistTracks::setIssueDate(const ppl7::DateTime& Date)
{
	this->IssueDate=Date;
}

ppl7::DateTime PlaylistTracks::getIssueDate() const
{
	return IssueDate;
}

bool PlaylistTracks::save(const ppl7::String& Filename)
{
	ppl7::String ext=ppl7::UpperCase(ppl7::File::getSuffix(Filename));
	if (ext == "WMP") return saveWMP(Filename);
	return false;

}

bool PlaylistTracks::load(const ppl7::String& Filename)
{
	ppl7::String ext=ppl7::UpperCase(ppl7::File::getSuffix(Filename));
	if (ext == "WMP") return loadWMP(Filename);
	ppl7::String m=tr("Unknown playlist format:");
	m+="\n\n" + Filename;
	QMessageBox::critical(nullptr, tr("WinMusik Error"),
		m, QMessageBox::Ok);
	return false;
}

bool PlaylistTracks::saveWMP(const ppl7::String& Filename)
{
	ppl7::File ff;
	try {
		ff.open(Filename, ppl7::File::WRITE);

		ppl7::String xml;
		xml="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		xml+="<WinMusikPlaylist version=\"1\">\n";
		xml+="   <name>" + ppl7::EscapeHTMLTags(Name) + "</name>\n";
		xml+="   <subname>" + ppl7::EscapeHTMLTags(SubName) + "</subname>\n";
		xml+="   <issue>" + ppl7::ToString("%d", IssueNumber) + "</issue>\n";
		xml+="   <date>" + IssueDate.getDate() + "</date>\n";
		int count=topLevelItemCount();
		xml+="   <totalTracks>" + ppl7::ToString("%u", count) + "</totalTracks>\n";
		uint64_t totalTrackLength=0;
		double totalMixLength=0;
		for (int i=0;i < count;i++) {
			PlaylistItem* item=static_cast<PlaylistItem*>(this->topLevelItem(i));
			totalTrackLength+=item->trackLength;
			totalMixLength+=item->mixLength;
		}
		xml+="   <totalTrackLength>" + ppl7::ToString("%llu", totalTrackLength) + "</totalTrackLength>\n";
		xml+="   <totalMixLength>" + ppl7::ToString("%0.3f", totalMixLength) + "</totalMixLength>\n";
		xml+="   <tracks>\n";
		for (int i=0;i < count;i++) {
			PlaylistItem* item=static_cast<PlaylistItem*>(this->topLevelItem(i));
			xml+=item->exportAsXML(6);
		}
		xml+="   </tracks>\n";
		xml+="</WinMusikPlaylist>\n";
		ff.write(xml);
		ff.close();
		return true;
	} catch (const ppl7::Exception& exp) {
		ShowException(exp, QObject::tr("could not save playlist"));
	}
	return false;
}

bool PlaylistTracks::loadWMP(const ppl7::String& Filename)
{
	QDomDocument doc("WinMusikPlaylist");
	QFile file(Filename);
	if (!file.open(QIODevice::ReadOnly)) {
		ppl7::String m=tr("Could not open file:");
		m+="\n\n" + Filename;
		QMessageBox::critical(nullptr, tr("WinMusik Error"),
			m, QMessageBox::Ok);
		return false;
	}
	QString errorMsg;
	int errorLine=0;
	int errorColumn=0;
	if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
		file.close();
		ppl7::String m=tr("Could not read playlist, invalid XML-format:");
		m+=ppl7::String(tr("Filename:")) + " " + Filename + "\n";
		m+=ppl7::String(tr("Error:")) + " " + ppl7::String(errorMsg) + "\n";
		m+=ppl7::String(tr("Line:")) + " " + ppl7::ToString("%i\n", errorLine);
		m+=ppl7::String(tr("Column:")) + " " + ppl7::ToString("%i\n", errorColumn);
		QMessageBox::critical(nullptr, tr("WinMusik Error"),
			m, QMessageBox::Ok);
		return false;
	}
	file.close();
	QDomElement root=doc.documentElement();
	if (root.tagName() != "WinMusikPlaylist") {
		ppl7::String m=tr("File is not a WinMusik playlist");
		m+="\n\n";
		m+=ppl7::String(tr("Filename:")) + " " + Filename + "\n";
		QMessageBox::critical(nullptr, tr("WinMusik Error"),
			m, QMessageBox::Ok);
		return false;
	}
	if (root.attribute("version") != "1") {
		ppl7::String m=tr("Unknown or unsupported version of WinMusik playlist");
		m+="\n\n";
		m+=ppl7::String(tr("Filename:")) + " " + Filename + "\n";
		m+=ppl7::String(tr("Version:")) + " " + ppl7::String(root.attribute("version")) + "\n";
		QMessageBox::critical(nullptr, tr("WinMusik Error"),
			m, QMessageBox::Ok);
		return false;
	}

	clear();
	QDomNode node=root.namedItem("name");
	if (node.isNull() == false && node.isElement() == true) {
		Name=node.toElement().text();
	}
	node=root.namedItem("subname");
	if (node.isNull() == false && node.isElement() == true) {
		SubName=node.toElement().text();
	}
	node=root.namedItem("issue");
	if (node.isNull() == false && node.isElement() == true) {
		IssueNumber=node.toElement().text().toInt();
	}
	node=root.namedItem("date");
	if (node.isNull() == false && node.isElement() == true) {
		IssueDate=node.toElement().text();
	}
	QDomNode tracks=root.namedItem("tracks");
	if (tracks.isNull() == false) {
		//printf ("Parsing tracks...\n");
		QDomElement e=tracks.firstChildElement("item");
		while (!e.isNull()) {
			//printf ("Parsing item...\n");
			PlaylistItem* item=new PlaylistItem;
			item->importFromXML(e);
			item->updateFromDatabase();
			addTopLevelItem(item);
			e=e.nextSiblingElement("item");
		}

	}
	return true;

}
