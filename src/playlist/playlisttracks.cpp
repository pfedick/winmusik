/*
 * playlisttracks.cpp
 *
 *  Created on: 08.12.2013
 *      Author: patrick
 */

#include "playlisttracks.h"
#include "playlist.h"
#include "traktor.h"

#include <QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QDragMoveEvent>

PlaylistItem::PlaylistItem()
{
	titleId=0;
	startPositionSec=0;
	endPositionSec=0;
	musicKey=0;
	bpm=0;
	bpmPlayed=0;
	rating=0;
	trackLength=0;
	mixLength=0;
	for (int z=0;z<5;z++) {
		cutStartPosition[z]=0;
		cutEndPosition[z]=0;
	}
	keyVerified=false;
	DeviceId=0;
	DeviceTrack=0;
	DeviceType=0;
	DevicePage=0;
}

ppl6::CString PlaylistItem::exportAsXML(int indention) const
{
	ppl6::CString Indent, ret;
	Indent.Repeat(" ",indention);
	ret=Indent+"<item>\n";
	ret+=Indent+"   <widgetId>"+ppl6::ToString("%tu",(ptrdiff_t)this)+"</widgetId>\n";
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
	ret+=Indent+"   <musicKey verified=\"";
	if (keyVerified) ret+="true"; else ret+="false";
	ret+="\">"+ppl6::EscapeHTMLTags(DataTitle::keyName(musicKey,musicKeyTypeMusicalSharps))+"</musicKey>\n";
	ret+=Indent+"   <bpm>"+ppl6::ToString("%u",bpm)+"</bpm>\n";
	ret+=Indent+"   <bpmPlayed>"+ppl6::ToString("%u",bpmPlayed)+"</bpmPlayed>\n";
	ret+=Indent+"   <rating>"+ppl6::ToString("%u",rating)+"</rating>\n";
	ret+=Indent+"   <trackLength>"+ppl6::ToString("%u",trackLength)+"</trackLength>\n";
	ret+=Indent+"   <mixLength>"+ppl6::ToString("%u",mixLength)+"</mixLength>\n";
	ret+=Indent+"</item>\n";
	return ret;
}

void PlaylistItem::importFromXML(QDomElement &e)
{
	ppl6::CString Tmp;

	QDomNode node =e.namedItem("titleId");
	if (node.isNull()==false && node.isElement()==true) {
		titleId=node.toElement().text().toInt();
	}

	node =e.namedItem("deviceType");
	if (node.isNull()==false && node.isElement()==true) {
		DeviceType=node.toElement().text().toInt();
	}
	node =e.namedItem("deviceId");
	if (node.isNull()==false && node.isElement()==true) {
		DeviceId=node.toElement().text().toInt();
	}
	node =e.namedItem("devicePage");
	if (node.isNull()==false && node.isElement()==true) {
		DevicePage=node.toElement().text().toInt();
	}
	node =e.namedItem("deviceTrack");
	if (node.isNull()==false && node.isElement()==true) {
		DeviceTrack=node.toElement().text().toInt();
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
		Tmp=node.toElement().attribute("verified","false");
		keyVerified=Tmp.ToBool();
	}
	node =e.namedItem("bpm");
	if (node.isNull()==false && node.isElement()==true) {
		bpm=node.toElement().text().toInt();
	}
	node =e.namedItem("bpmPlayed");
	if (node.isNull()==false && node.isElement()==true) {
		bpmPlayed=node.toElement().text().toInt();
	}
	if (!bpmPlayed) bpmPlayed=bpm;

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
	if (bpm>0 && bpmPlayed>0 && bpmPlayed!=bpm) mixLength=mixLength*bpm/bpmPlayed;

	/*
	if (titleId>0) {
		DataTitle *ti=wm_main->GetTitle(titleId);
		if (ti) {
			DeviceId=ti->DeviceId;
			DeviceTrack=ti->Track;
			DeviceType=ti->DeviceType;
			DevicePage=ti->Page;
		}
	}
	*/
	if (endPositionSec==0) {
		this->useTraktorCues(File);
		if (endPositionSec==0) endPositionSec=trackLength;
		updateMixLength();
	}

	loadCoverPreview();
}

void PlaylistItem::updateMixLength()
{
	mixLength=endPositionSec-startPositionSec;
	for (int i=0;i<5;i++) {
		mixLength-=(cutEndPosition[i]-cutStartPosition[i]);
	}
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
	if (!bpmPlayed) bpmPlayed=bpm;
	rating=ti->Rating;
	keyVerified=(ti->Flags>>4)&1;
	Version=wm_main->GetVersionText(ti->VersionId);
	Genre=wm_main->GetGenreText(ti->GenreId);
	Label=wm_main->GetLabelText(ti->LabelId);
	DeviceId=ti->DeviceId;
	DeviceTrack=ti->Track;
	DeviceType=ti->DeviceType;
	DevicePage=ti->Page;
	File=wm_main->MP3Filename(ti->DeviceId,ti->Page,ti->Track);
}

void PlaylistItem::useTraktorCues(const ppl6::CString &file)
{
	ppl6::CID3Tag Tag;
	if (Tag.Load(file)) useTraktorCues(Tag);
}

void PlaylistItem::useTraktorCues(const ppl6::CID3Tag &Tag)
{
	ppl6::CString Tmp;
	std::list <TraktorTagCue> cuelist;
	std::list <TraktorTagCue>::const_iterator it;
	getTraktorCues(cuelist, Tag);
	if (cuelist.size()==0) return;
	for (it=cuelist.begin();it!=cuelist.end();it++) {
		int sec=(int)(it->start/1000.0);
		if (it->type==TraktorTagCue::IN) startPositionSec=sec;
		if (it->type==TraktorTagCue::OUT) endPositionSec=sec;
	}
	updateMixLength();
}

PlaylistTracks::PlaylistTracks(QWidget * parent)
	:QTreeWidget(parent)
{
	playlist=NULL;
	lastmoveitem=NULL;
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


QMimeData *PlaylistTracks::mimeData(const QList<QTreeWidgetItem *> items) const
{
	QList<QUrl> list;
	QPixmap Icon;
	ppl6::CString xml;
	xml="<winmusikTracklist>\n";
	xml+="<tracks>\n";
	for (int i=0;i<items.size();i++) {
		PlaylistItem *item=(PlaylistItem *)items[i];
		if (Icon.isNull()) {
			Icon.loadFromData((const uchar*)item->CoverPreview.GetPtr(),item->CoverPreview.GetSize());
		}
		xml+=item->exportAsXML(0);

#ifdef _WIN32
		list.append(QUrl::fromLocalFile(item->File));
#else
		list.append(QUrl::fromLocalFile(item->File));
#endif
	}
	xml+="</tracks>\n";
	xml+="</winmusikTracklist>\n";


	QMimeData *mimeData = new QMimeData;
	mimeData->setText(xml);
	mimeData->setUrls(list);
	mimeData->setImageData(Icon);
	return mimeData;
}

void PlaylistTracks::dragEnterEvent ( QDragEnterEvent * event)
{
	event->accept();
}

void PlaylistTracks::dragMoveEvent(QDragMoveEvent *e)
{
	e->accept();
	//QTreeWidget::dragMoveEvent(e);
	//return;
	PlaylistItem *item = (PlaylistItem*)itemAt(e->pos());
	if (item) {
		if (item!=lastmoveitem) {
			if (lastmoveitem) lastmoveitem->setSelected(false);
			this->scrollToItem(item);
			lastmoveitem=item;
		}
		item->setSelected(true);
	} else if (lastmoveitem) {
		lastmoveitem->setSelected(false);
		lastmoveitem=NULL;
	}

	//e->ignore();
	//QTreeWidget::dragMoveEvent(e);
}

bool PlaylistTracks::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{
	printf ("PlaylistTracks::dropMimeData, parent=%tu, index=%i\n",(ptrdiff_t)parent,index);
	ppl6::CString Tmp;
	playlist->handleDropEvent(data,parent);
	lastmoveitem=NULL;
	return true;
}

void PlaylistTracks::dropEvent ( QDropEvent * event )
{
	printf ("PlaylistTracks::dropEvent, action: %i\n",event->dropAction());
	if (lastmoveitem) {
		lastmoveitem->setSelected(false);
		lastmoveitem=NULL;
	}
	event->acceptProposedAction();
	playlist->handleDropEvent(event);
	if (event->source()==this) {
		printf ("Quelle ist gleich\n");
		if (event->dropAction()==1) {
			deleteSourceItems(event);
			playlist->renumberTracks();
			playlist->updateLengthStatus();
		}
	}
}

void PlaylistTracks::deleteSourceItems(QDropEvent * event)
{
	const QMimeData *mime=event->mimeData();
	if (!mime) return;
	ppl6::CString xml=mime->text();
	if (xml.Left(18)!="<winmusikTracklist") return;
	QDomDocument doc("winmusikTracklist");
	if (doc.setContent(xml)) {
		QDomElement root=doc.documentElement();
		if (root.tagName()=="winmusikTracklist") {
			QDomNode tracks=root.namedItem("tracks");
			if (tracks.isNull()==false) {
				QDomElement e=tracks.firstChildElement("item");
				while (!e.isNull()) {
					QDomNode node =e.namedItem("widgetId");
					if (node.isNull()==false && node.isElement()==true) {
						QTreeWidgetItem *item=(QTreeWidgetItem*)node.toElement().text().toULongLong();
						int id=indexOfTopLevelItem(item);
//						printf ("delete Widget with id=%i\n",id);
						if (item!=NULL && id>=0) this->takeTopLevelItem(id);
					}
					e=e.nextSiblingElement("item");
				}
			}
		}
	}
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

void PlaylistTracks::selectItems(QList<QTreeWidgetItem *>items)
{
	for (int i=0;i<items.size();i++) {
		items.at(i)->setSelected(true);
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

bool PlaylistTracks::save(const ppl6::CString &Filename)
{
	ppl6::CString ext=ppl6::UCase(ppl6::FileSuffix(Filename));
	if (ext=="WMP") return saveWMP(Filename);
	return false;

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

bool PlaylistTracks::saveWMP(const ppl6::CString &Filename)
{
	ppl6::CFile ff;
	if (!ff.Open(Filename,"wb")) {
		CWmClient::RaiseError(NULL,tr("Could not open File"));
		return false;
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
	return true;
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
		m+=tr("Filename:"); m+=" "+Filename+"\n";
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
		m+=tr("Filename:"); m+=" "+Filename+"\n";
		QMessageBox::critical(NULL, tr("WinMusik Error"),
				m, QMessageBox::Ok);
		return false;
	}
	if (root.attribute("version")!="1") {
		ppl6::CString m=tr("Unknown or unsupported version of WinMusik playlist");
		m+="\n\n";
		m+=tr("Filename:"); m+=" "+Filename+"\n";
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


