/*
 * playlisttracks.cpp
 *
 *  Created on: 08.12.2013
 *      Author: patrick
 */

#include "playlisttracks.h"
#include "playlist.h"

#include <QDomDocument>
#include <QFile>
#include <QMessageBox>


PlaylistItem::PlaylistItem()
{
	titleId=0;
	startPositionSec=0;
	endPositionSec=0;
	musicKey=0;
	bpm=0;
	rating=0;
	trackLength=0;
	mixLength=0;
	for (int z=0;z<5;z++) {
		cutStartPosition[z]=0;
		cutEndPosition[z]=0;
	}
}

ppl6::CString PlaylistItem::exportAsXML(int indention) const
{
	ppl6::CString Indent, ret;
	Indent.Repeat(" ",indention);
	ret=Indent+"<item>\n";
	ret+=Indent+"   <titleId>"+ppl6::ToString("%u",titleId)+"</titleId>\n";
	ret+=Indent+"   <startPositionSec>"+ppl6::ToString("%u",startPositionSec)+"</startPositionSec>\n";
	ret+=Indent+"   <endPositionSec>"+ppl6::ToString("%u",endPositionSec)+"</endPositionSec>\n";
	ret+=Indent+"   <cuts>\n";
	for (int i=0;i<5;i++) {
		ret+=Indent+"      <cut><start>"+ppl6::ToString("%u",cutStartPosition[i])+"</start>";
		ret+="<end>"+ppl6::ToString("%u",cutEndPosition[i])+"</end></cut>\n";
	}
	ret+=Indent+"   </cuts>\n";
	ret+=Indent+"   <Artist>"+ppl6::EscapeHTMLTags(Artist)+"</Artist>\n";
	ret+=Indent+"   <Title>"+ppl6::EscapeHTMLTags(Title)+"</Title>\n";
	ret+=Indent+"   <Version>"+ppl6::EscapeHTMLTags(Version)+"</Version>\n";
	ret+=Indent+"   <Genre>"+ppl6::EscapeHTMLTags(Genre)+"</Genre>\n";
	ret+=Indent+"   <Label>"+ppl6::EscapeHTMLTags(Label)+"</Label>\n";
	ret+=Indent+"   <Album>"+ppl6::EscapeHTMLTags(Album)+"</Album>\n";
	ret+=Indent+"   <Remarks>"+ppl6::EscapeHTMLTags(Remarks)+"</Remarks>\n";
	ret+=Indent+"   <File>"+ppl6::EscapeHTMLTags(File)+"</File>\n";
	ret+=Indent+"   <musicKey>"+ppl6::EscapeHTMLTags(DataTitle::keyName(musicKey))+"</musicKey>\n";
	ret+=Indent+"   <bpm>"+ppl6::ToString("%u",bpm)+"</bpm>\n";
	ret+=Indent+"   <rating>"+ppl6::ToString("%u",rating)+"</rating>\n";
	ret+=Indent+"   <trackLength>"+ppl6::ToString("%u",trackLength)+"</trackLength>\n";
	ret+=Indent+"   <mixLength>"+ppl6::ToString("%u",mixLength)+"</mixLength>\n";
	ret+=Indent+"</item>\n";
	return ret;
}

void PlaylistItem::importFromXML(QDomElement &e)
{
	QDomNode node =e.namedItem("titleId");
	if (node.isNull()==false && node.isElement()==true) {
		titleId=node.toElement().text().toInt();
	}

	node =e.namedItem("startPositionSec");
	if (node.isNull()==false && node.isElement()==true) {
		startPositionSec=node.toElement().text().toInt();
	}
	node =e.namedItem("endPositionSec");
	if (node.isNull()==false && node.isElement()==true) {
		endPositionSec=node.toElement().text().toInt();
	}
	node =e.namedItem("trackLength");
	if (node.isNull()==false && node.isElement()==true) {
		trackLength=node.toElement().text().toInt();
	}
	node =e.namedItem("Artist");
	if (node.isNull()==false && node.isElement()==true) {
		Artist=node.toElement().text();
	}
	node =e.namedItem("Title");
	if (node.isNull()==false && node.isElement()==true) {
		Title=node.toElement().text();
	}
	node =e.namedItem("Version");
	if (node.isNull()==false && node.isElement()==true) {
		Version=node.toElement().text();
	}
	node =e.namedItem("Genre");
	if (node.isNull()==false && node.isElement()==true) {
		Genre=node.toElement().text();
	}
	node =e.namedItem("Label");
	if (node.isNull()==false && node.isElement()==true) {
		Label=node.toElement().text();
	}
	node =e.namedItem("Album");
	if (node.isNull()==false && node.isElement()==true) {
		Album=node.toElement().text();
	}
	node =e.namedItem("Remarks");
	if (node.isNull()==false && node.isElement()==true) {
		Remarks=node.toElement().text();
	}

	node =e.namedItem("File");
	if (node.isNull()==false && node.isElement()==true) {
		File=node.toElement().text();
	}
	node =e.namedItem("musicKey");
	if (node.isNull()==false && node.isElement()==true) {
		musicKey=DataTitle::keyId(node.toElement().text());
	}
	node =e.namedItem("bpm");
	if (node.isNull()==false && node.isElement()==true) {
		bpm=node.toElement().text().toInt();
	}
	node =e.namedItem("rating");
	if (node.isNull()==false && node.isElement()==true) {
		rating=node.toElement().text().toInt();
	}

	mixLength=endPositionSec-startPositionSec;
	QDomNode cuts=e.namedItem("cuts");
	int c=0;
	if (cuts.isNull()==false) {
		//printf ("   Parsing cuts...\n");
		QDomElement e=cuts.firstChildElement("cut");
		while (!e.isNull()) {
			//printf ("      Parsing cut...\n");
			node =e.namedItem("start");
			if (node.isNull()==false && node.isElement()==true) {
				cutStartPosition[c]=node.toElement().text().toInt();
				//printf ("       start found\n");
			}
			node =e.namedItem("end");
			if (node.isNull()==false && node.isElement()==true) {
				cutEndPosition[c]=node.toElement().text().toInt();
				//printf ("       end found\n");
			}
			mixLength-=(cutEndPosition[c]-cutStartPosition[c]);
			c++;
			if (c>4) break;
			e=e.nextSiblingElement("cut");
		}
	}
	loadCoverPreview();
}

void PlaylistItem::loadCoverPreview()
{
	// Cover laden
	if (titleId>0) {
		DataTitle *ti=wm_main->GetTitle(titleId);
		if (ti) CoverPreview=ti->CoverPreview;
	}
	if (CoverPreview.Size()==0 && File.Size()>0) {
		TrackInfo info;
		if (getTrackInfoFromFile(info,File)) {
			CoverPreview=info.Ti.CoverPreview;
		}
	}
}

void PlaylistItem::updateFromDatabase()
{
	if (titleId==0) return;
	DataTitle *ti=wm_main->GetTitle(titleId);
	if (!ti) return;
	trackLength=ti->Length;
	Artist=ti->Artist;
	Title=ti->Title;
	Album=ti->Album;
	musicKey=ti->Key;
	bpm=ti->BPM;
	rating=ti->Rating;
	Version=wm_main->GetVersionText(ti->VersionId);
	Genre=wm_main->GetGenreText(ti->GenreId);
	Label=wm_main->GetLabelText(ti->LabelId);
	DeviceId=ti->DeviceId;
	DeviceTrack=ti->Track;
	DeviceType=ti->DeviceType;
	DevicePage=ti->Page;
	File=wm_main->MP3Filename(ti->DeviceId,ti->Page,ti->Track);
}


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

void PlaylistTracks::unselectItems()
{
	QList<QTreeWidgetItem *>selected=selectedItems();
	for (int i=0;i<selected.size();i++) selected.at(i)->setSelected(false);
}

void PlaylistTracks::deleteSelectedItems()
{
	QList<QTreeWidgetItem *>selected=selectedItems();
	for (int i=0;i<selected.size();i++) {
		PlaylistItem *item=(PlaylistItem*)takeTopLevelItem(indexOfTopLevelItem(selected.at(i)));
		delete item;
	}
}

void PlaylistTracks::deleteItems(QList<QTreeWidgetItem *>items)
{
	for (int i=0;i<items.size();i++) {
		PlaylistItem *item=(PlaylistItem*)takeTopLevelItem(indexOfTopLevelItem(items.at(i)));
		delete item;
	}
}

void PlaylistTracks::setName(const ppl6::CString &Name)
{
	this->Name=Name;
}

ppl6::CString PlaylistTracks::getName() const
{
	return Name;
}

void PlaylistTracks::save(const ppl6::CString &Filename)
{
	ppl6::CString ext=ppl6::UCase(ppl6::FileSuffix(Filename));
	if (ext=="WMP") return saveWMP(Filename);

}

bool PlaylistTracks::load(const ppl6::CString &Filename)
{
	ppl6::CString ext=ppl6::UCase(ppl6::FileSuffix(Filename));
	if (ext=="WMP") return loadWMP(Filename);
	ppl6::CString m=tr("Unknown playlist format:");
	m+="\n\n"+Filename;
	QMessageBox::critical(NULL, tr("WinMusik Error"),
			m, QMessageBox::Ok);
	return false;
}

void PlaylistTracks::saveWMP(const ppl6::CString &Filename)
{
	ppl6::CFile ff;
	if (!ff.Open(Filename,"wb")) {
		CWmClient::RaiseError(NULL,tr("Could not open File"));
		return;
	}
	ppl6::CString xml;
	xml="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	xml+="<WinMusikPlaylist version=\"1\">\n";
	xml+="   <name>"+ppl6::EscapeHTMLTags(Name)+"</name>\n";
	int count=topLevelItemCount();
	xml+="   <totalTracks>"+ppl6::ToString("%u",count)+"</totalTracks>\n";
	ppluint64 totalTrackLength=0;
	ppluint64 totalMixLength=0;
	for (int i=0;i<count;i++) {
		PlaylistItem *item=(PlaylistItem*)this->topLevelItem(i);
		totalTrackLength+=item->trackLength;
		totalMixLength+=item->mixLength;
	}
	xml+="   <totalTrackLength>"+ppl6::ToString("%llu",totalTrackLength)+"</totalTrackLength>\n";
	xml+="   <totalMixLength>"+ppl6::ToString("%llu",totalMixLength)+"</totalMixLength>\n";
	xml+="   <tracks>\n";
	for (int i=0;i<count;i++) {
		PlaylistItem *item=(PlaylistItem*)this->topLevelItem(i);
		xml+=item->exportAsXML(6);
	}
	xml+="   </tracks>\n";
	xml+="</WinMusikPlaylist>\n";
	ff.Write(xml);
	ff.Close();
}

bool PlaylistTracks::loadWMP(const ppl6::CString &Filename)
{
	QDomDocument doc("WinMusikPlaylist");
	QFile file(Filename);
	if (!file.open(QIODevice::ReadOnly)) {
		ppl6::CString m=tr("Could not open file:");
		m+="\n\n"+Filename;
		QMessageBox::critical(NULL, tr("WinMusik Error"),
				m, QMessageBox::Ok);
		return false;
	}
	QString errorMsg;
	int errorLine=0;
	int errorColumn=0;
	if (!doc.setContent(&file, &errorMsg,&errorLine,&errorColumn)) {
		file.close();
		ppl6::CString m=tr("Could not read playlist, invalid XML-format:");
		m+=tr("Fielname:"); m+=" "+Filename+"\n";
		m+=tr("Error:"); m+=" "+errorMsg+"\n";
		m+=tr("Line:"); m+=" "+ppl6::ToString("%i\n",errorLine);
		m+=tr("Column:"); m+=" "+ppl6::ToString("%i\n",errorColumn);
		QMessageBox::critical(NULL, tr("WinMusik Error"),
				m, QMessageBox::Ok);
		return false;
	}
	file.close();
	QDomElement root=doc.documentElement();
	if (root.tagName()!="WinMusikPlaylist") {
		ppl6::CString m=tr("File is not a WinMusik playlist");
		m+="\n\n";
		m+=tr("Fielname:"); m+=" "+Filename+"\n";
		QMessageBox::critical(NULL, tr("WinMusik Error"),
				m, QMessageBox::Ok);
		return false;
	}
	if (root.attribute("version")!="1") {
		ppl6::CString m=tr("Unknown or unsupported version of WinMusik playlist");
		m+="\n\n";
		m+=tr("Fielname:"); m+=" "+Filename+"\n";
		m+=tr("Version:"); m+=" "+root.attribute("version")+"\n";
		QMessageBox::critical(NULL, tr("WinMusik Error"),
				m, QMessageBox::Ok);
		return false;
	}

	clear();

	QDomNode node=root.namedItem("name");
	if (node.isNull()==false && node.isElement()==true) {
		Name=node.toElement().text();
	}
	QDomNode tracks=root.namedItem("tracks");
	if (tracks.isNull()==false) {
		//printf ("Parsing tracks...\n");
		QDomElement e=tracks.firstChildElement("item");
		while (!e.isNull()) {
			//printf ("Parsing item...\n");
			PlaylistItem *item=new PlaylistItem;
			item->importFromXML(e);
			item->updateFromDatabase();
			addTopLevelItem(item);
			e=e.nextSiblingElement("item");
		}

	}


	return true;

}


