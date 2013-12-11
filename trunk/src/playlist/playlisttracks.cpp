/*
 * playlisttracks.cpp
 *
 *  Created on: 08.12.2013
 *      Author: patrick
 */

#include "playlisttracks.h"
#include "playlist.h"

PlaylistItem::PlaylistItem()
{
	titleId=0;
	startPositionSec=0;
	endPositionSec=0;
	musicKey=0;
	bpm=0;
	rating=0;
	length=0;
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
	ret+=Indent+"   <File>"+ppl6::EscapeHTMLTags(File)+"</File>\n";
	ret+=Indent+"   <musicKey>"+ppl6::EscapeHTMLTags(DataTitle::keyName(musicKey))+"</musicKey>\n";
	ret+=Indent+"   <bpm>"+ppl6::ToString("%u",bpm)+"</bpm>\n";
	ret+=Indent+"   <rating>"+ppl6::ToString("%u",musicKey)+"</rating>\n";
	ret+=Indent+"   <length>"+ppl6::ToString("%u",musicKey)+"</length>\n";
	ret+=Indent+"</item>\n";
	return ret;
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

void PlaylistTracks::save(const ppl6::CString &Filename)
{
	ppl6::CString ext=ppl6::UCase(ppl6::FileSuffix(Filename));
	if (ext=="WMP") return saveWMP(Filename);

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
	xml+="<name>"+ppl6::EscapeHTMLTags(Name)+"</name>\n";
	int count=topLevelItemCount();
	xml+="<totalTracks>"+ppl6::ToString("%u",count)+"</totalTracks>\n";
	ppluint64 length=0;
	for (int i=0;i<count;i++) {
		PlaylistItem *item=(PlaylistItem*)this->topLevelItem(i);
		length+=item->length;
	}
	xml+="<totalLength>"+ppl6::ToString("%llu",length)+"</totalLength>\n";
	xml+="<tracks>\n";
	for (int i=0;i<count;i++) {
		PlaylistItem *item=(PlaylistItem*)this->topLevelItem(i);
		xml+=item->exportAsXML(3);
	}
	xml+="</tracks>\n";
	ff.Write(xml);
	ff.Close();
}
