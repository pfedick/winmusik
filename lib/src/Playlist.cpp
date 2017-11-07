#include "wm_playlist.h"
#include "wm_musicalkeys.h"

namespace de {
namespace pfp {
namespace winmusik {

PlaylistItem::PlaylistItem()
{
	titleId=0;
	startPositionSec=0;
	endPositionSec=0;
	musicKey=0;
	energyLevel=0;
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

ppl7::String PlaylistItem::exportAsXML(int indention) const
{
	ppl7::String Indent, ret;
	Indent.repeat(" ",indention);
	ret=Indent+"<item>\n";
	ret+=Indent+"   <widgetId>"+ppl7::ToString("%tu",(ptrdiff_t)this)+"</widgetId>\n";
	ret+=Indent+"   <titleId>"+ppl7::ToString("%u",titleId)+"</titleId>\n";
	ret+=Indent+"   <startPositionSec>"+ppl7::ToString("%u",startPositionSec)+"</startPositionSec>\n";
	ret+=Indent+"   <endPositionSec>"+ppl7::ToString("%u",endPositionSec)+"</endPositionSec>\n";
	ret+=Indent+"   <cuts>\n";
	for (int i=0;i<5;i++) {
		ret+=Indent+"      <cut><start>"+ppl7::ToString("%u",cutStartPosition[i])+"</start>";
		ret+="<end>"+ppl7::ToString("%u",cutEndPosition[i])+"</end></cut>\n";
	}
	ret+=Indent+"   </cuts>\n";
	ret+=Indent+"   <Artist>"+ppl7::EscapeHTMLTags(Artist)+"</Artist>\n";
	ret+=Indent+"   <Title>"+ppl7::EscapeHTMLTags(Title)+"</Title>\n";
	ret+=Indent+"   <Version>"+ppl7::EscapeHTMLTags(Version)+"</Version>\n";
	ret+=Indent+"   <Genre>"+ppl7::EscapeHTMLTags(Genre)+"</Genre>\n";
	ret+=Indent+"   <Label>"+ppl7::EscapeHTMLTags(Label)+"</Label>\n";
	ret+=Indent+"   <Album>"+ppl7::EscapeHTMLTags(Album)+"</Album>\n";
	ret+=Indent+"   <Remarks>"+ppl7::EscapeHTMLTags(Remarks)+"</Remarks>\n";
	ret+=Indent+"   <File>"+ppl7::EscapeHTMLTags(File)+"</File>\n";
	ret+=Indent+"   <musicKey verified=\"";
	if (keyVerified) ret+="true"; else ret+="false";
	ret+="\">"+ppl7::EscapeHTMLTags(MusicalKeys::sharpName(musicKey))+"</musicKey>\n";
	ret+=Indent+"   <energyLevel>"+ppl7::ToString("%u",energyLevel)+"</energyLevel>\n";
	ret+=Indent+"   <bpm>"+ppl7::ToString("%u",bpm)+"</bpm>\n";
	ret+=Indent+"   <bpmPlayed>"+ppl7::ToString("%u",bpmPlayed)+"</bpmPlayed>\n";
	ret+=Indent+"   <rating>"+ppl7::ToString("%u",rating)+"</rating>\n";
	ret+=Indent+"   <trackLength>"+ppl7::ToString("%u",trackLength)+"</trackLength>\n";
	ret+=Indent+"   <mixLength>"+ppl7::ToString("%u",mixLength)+"</mixLength>\n";
	ret+=Indent+"</item>\n";
	return ret;
}

void PlaylistItem::importFromXML(const ppl7::String &xml)
{
	ppl7::Array Matches;
	if (xml.pregMatch("/\\<titleId\\>(.*)\\<\\/titleId\\>/s",Matches)) titleId=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<startPositionSec\\>(.*)\\<\\/startPositionSec\\>/s",Matches)) startPositionSec=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<endPositionSec\\>(.*)\\<\\/endPositionSec\\>/s",Matches)) endPositionSec=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();

	if (xml.pregMatch("/\\<cuts\\>(.*)\\<\\/cuts\\>/s",Matches)) {
		ppl7::Array cuts(Matches[1],"<cut>");
		size_t rows=cuts.size();
		if (rows>6) rows=6;
		for (size_t i=0;i<rows;i++) {
			ppl7::String cut=cuts[i+1];
			if (cut.pregMatch("/\\<start\\>(.*)\\<\\/start\\>/s",Matches)) cutStartPosition[i]=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
			if (cut.pregMatch("/\\<end\\>(.*)\\<\\/end\\>/s",Matches)) cutEndPosition[i]=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
		}
	}
	if (xml.pregMatch("/\\<Artist\\>(.*)\\<\\/Artist\\>/s",Matches)) Artist=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<Title\\>(.*)\\<\\/Title\\>/s",Matches)) Title=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<Version\\>(.*)\\<\\/Version\\>/s",Matches)) Version=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<Genre\\>(.*)\\<\\/Genre\\>/s",Matches)) Genre=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<Label\\>(.*)\\<\\/Label\\>/s",Matches)) Label=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<Album\\>(.*)\\<\\/Album\\>/s",Matches)) Album=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<Remarks\\>(.*)\\<\\/Remarks\\>/s",Matches)) Remarks=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	if (xml.pregMatch("/\\<File\\>(.*)\\<\\/File\\>/s",Matches)) File=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1]));
	keyVerified=false;
	if (xml.pregMatch("/\\<musicKey verified=\"true\"\\>/s",Matches)) keyVerified=true;

	if (xml.pregMatch("/\\<musicKey.*?\\>(.*)\\<\\/musicKey\\>/s",Matches)) musicKey=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<energyLevel\\>(.*)\\<\\/energyLevel\\>/s",Matches)) energyLevel=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<bpm\\>(.*)\\<\\/bpm\\>/s",Matches)) bpm=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<bpmPlayed\\>(.*)\\<\\/bpmPlayed\\>/s",Matches)) bpmPlayed=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<rating\\>(.*)\\<\\/rating\\>/s",Matches)) rating=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<trackLength\\>(.*)\\<\\/trackLength\\>/s",Matches)) trackLength=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();
	if (xml.pregMatch("/\\<mixLength\\>(.*)\\<\\/mixLength\\>/s",Matches)) mixLength=ppl7::Trim(ppl7::UnescapeHTMLTags(Matches[1])).toInt();


}



}}}	// EOF Namespace de.pfp.winmusik


