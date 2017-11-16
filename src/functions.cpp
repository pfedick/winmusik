/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: functions.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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


#include "winmusik3.h"
#include <ppl6-sound.h>

#include <QWidget>
#include <QPixmap>
#include <QBuffer>
#include <QLabel>
#include <QClipboard>


static bool CopyFromFilename(TrackInfo &info, const ppl6::CString &Filename);
static bool CopyFromID3v1Tag(TrackInfo &info, const ppl6::CString &Filename, ppl6::CFile &File);
static bool CopyFromID3v2Tag(TrackInfo &info, const ppl6::CString &Filename, ppl6::CFile &File);

ppluint32 Time2Int(ppl6::CString &Time)
{
	Time.Replace(",",":");
	Time.Replace(".",":");
	Time.Trim();
	if (Time.IsEmpty()) return 0;
	ppl6::CArray a;
	a.Explode(Time,":");
	return ppl6::atoi(a.Get(0))*60+ppl6::atoi(a.Get(1));
}

ppl6::CString Int2Time(ppluint32 Time)
{
	ppl6::CString a;
	a.Setf("%0i:%02i",(int)(Time/60),Time%60);
	return a;
}

ppl6::CString Long2Date(ppl6::CString &Format, ppluint32 Date)
{
	ppl6::CString ret, Tmp;
	if (!Date) return ret;
	Tmp.Setf("%u",Date);
	int day=Tmp.Right(2).ToInt();
	Tmp.TrimR(2);
	int month=Tmp.Right(2).ToInt();
	Tmp.TrimR(2);
	int year=Tmp.ToInt();
	ret=Format;
	Tmp.Setf("%02i",day);
	ret.Replace("%d",Tmp);
	Tmp.Setf("%02i",month);
	ret.Replace("%m",Tmp);
	Tmp.Setf("%04i",year);
	ret.Replace("%Y",Tmp);
	Tmp.Setf("%02i",year%100);
	ret.Replace("%y",Tmp);

	return ret;
}


void NormalizeImportString(ppl6::CString &Buffer)
{
	Buffer.Replace("_"," ");
	Buffer.Replace("'","'");
	Buffer.Replace("`","'");
	Buffer.Trim();
}

static void fixArtistAndTitle(const ppl6::CString &prefix, TrackInfo &info, const ppl6::CString prefix_replace)
{
	ppl6::CString regex="/(.*?)\\s"+prefix+"\\s+(.*)$/i";
	ppl6::CArray Matches;
	ppl6::CString Title=info.Ti.Title;
	ppl6::CString Artist=info.Ti.Artist;
	if (Title.PregMatch(regex, Matches)) {
		ppl6::CString feat=Matches.GetString(2);
		feat.Trim();
		Title=Matches[1];
		Artist.Replace(feat,"");
		Artist.Trim();
		Artist.Trim(",");
		Artist.Replace(",,",",");
		Artist+=" "+prefix_replace+" "+Matches.GetString(2);
		info.Ti.SetArtist(Artist);
		info.Ti.SetTitle(Title);
	}
}

static void fixIt(TrackInfo &info)
{
	ppl6::CArray Matches;
	fixArtistAndTitle("feat\\.",info, "feat.");
	fixArtistAndTitle("featuring",info, "feat.");
	fixArtistAndTitle("ft",info, "feat.");
	fixArtistAndTitle("ft.",info, "feat.");
	fixArtistAndTitle("pres\\.",info, "pres.");
	fixArtistAndTitle("presents",info, "pres.");
}

static bool CopyFromID3v1Tag(TrackInfo &info, const ppl6::CString &Filename, ppl6::CFile &File)
{
	ppl6::CString Tmp,Title;
	// ID3v1-Tag einlesen falls vorhanden
	const char *buffer=File.Map(File.Lof()-128,128);
	if (buffer[0]=='T' && buffer[1]=='A' && buffer[2]=='G') {
		ID3TAG id3;
		memcpy (&id3,buffer,128);
		Tmp.Set(id3.Artist,30);
		NormalizeImportString(Tmp);

		info.Ti.SetArtist(Tmp);
		Tmp.Set(id3.SongName,30);
		NormalizeImportString(Tmp);
		// Sind Klammern enthalten?
		if (Tmp.PregMatch("/^(.*)\\((.*)\\).*$/")) {
			Title=Tmp.GetMatch(1);
			Title.Trim();
			info.Ti.SetTitle(Title);
			info.Version=Tmp.GetMatch(2);
			info.Version.Trim();
		} else if (Tmp.PregMatch("/^(.*)\\[(.*)\\].*$/")) {
			Title=Tmp.GetMatch(1);
			Title.Trim();
			info.Ti.SetTitle(Title);
			info.Version=Tmp.GetMatch(2);
			info.Version.Trim();
		} else {
			info.Ti.SetTitle(Tmp);
		}
		Tmp=id3.Album;
		NormalizeImportString(Tmp);
		info.Ti.SetAlbum(Tmp);
		// ReleaseDate
		info.Ti.ReleaseDate=ppl6::atoi(id3.Year)*10000;
		Tmp.Set(id3.Comment,29);
		NormalizeImportString(Tmp);
		info.Ti.SetRemarks(Tmp);
		info.Genre=ppl6::GetID3GenreName(id3.Genre);
		NormalizeImportString(info.Genre);
		fixIt(info);
		return true;
	}
	return CopyFromFilename(info,Filename);
}

static bool CopyFromID3v2Tag(TrackInfo &info, const ppl6::CString &Filename, ppl6::CFile &File)
{
	ppl6::CString Tmp;
	ppl6::CID3Tag Tag;
	ppl6::CString Title, Version, Genre, Comment, Artist;
	if (Tag.Load(File)) {
		Title=Tag.GetTitle();
		NormalizeImportString(Title);

		Artist=Tag.GetArtist();
		NormalizeImportString(Artist);
		info.Ti.SetArtist(Artist);

		// Album
		Tmp=Tag.GetAlbum();
		NormalizeImportString(Tmp);
		info.Ti.SetAlbum(Tmp);

		// BPM
		Tmp=Tag.GetBPM();
		NormalizeImportString(Tmp);
		info.Ti.BPM=Tmp.ToInt();

		// Music Key
		info.Ti.SetKey(Tag.GetKey());

		// EnergyLevel
		info.Ti.EnergyLevel=(char)(Tag.GetEnergyLevel().ToInt()&255);

		// ReleaseDate
		Tmp=Tag.GetYear();
		NormalizeImportString(Tmp);
		info.Ti.ReleaseDate=Tmp.ToInt()*10000;

		// Comment
		Comment=Tag.GetComment();
		NormalizeImportString(Comment);

		// Genre
		Genre=Tag.GetGenre();
		NormalizeImportString(Genre);
		if (Genre.NotEmpty()) {
			info.Genre=Genre;
		}

		// Remixer
		if (Title.PregMatch("/^(.*?)\\s*\\((.*?)\\)\\s*\\((.*?)\\).*$/")) {
			//printf ("Match 0\n");
			ppl6::CString Tmp1=Title.GetMatch(2);
			ppl6::CString Tmp2=Title.GetMatch(3);
			Title=Title.GetMatch(1);
			Tmp1.Trim();
			Tmp2.Trim();
			Title.Trim();
			//printf ("Tmp1=%s, Tmp2=%s\n",(const char*)Tmp1,(const char*)Tmp2);
			if (Tmp1.PregMatch("/feat\\.\\s+/i")) {
				//printf ("xxx 1\n");
				Tmp=Tmp2;
				Artist+=" "+Tmp1;
				info.Ti.SetArtist(Artist);
			} else if (Tmp2.PregMatch("/feat\\.\\s+/i")) {
				//printf ("xxx 2\n");
				Tmp=Tmp1;
				Artist+=" "+Tmp2;
				info.Ti.SetArtist(Artist);
			} else {
				//printf ("xxx 3\n");
				Title+=" ("+Tmp1+")";
				Tmp=Tmp2;
			}
		} else if (Title.PregMatch("/^(.*)\\((.*?)\\).*$/")) {
			//printf ("Match 1\n");
			Tmp=Title.GetMatch(2);
			Title=Title.GetMatch(1);
			Tmp.Trim();
			Title.Trim();
		} else if (Title.PregMatch("/^(.*)\\[(.*?)\\].*$/")) {
			//printf ("Match 2\n");
			Tmp=Title.GetMatch(2);
			Title=Title.GetMatch(1);
			Tmp.Trim();
			Title.Trim();
		} else {
			//printf ("Match 3\n");
			Tmp=Tag.GetRemixer();
			NormalizeImportString(Tmp);
		}
		Tmp.PregReplace("/\\brmx\\b/i"," Remix");
		Tmp.Trim();
		Version=Tmp;
		info.Version=Version;

		// Label
		Tmp=Tag.GetLabel();
		NormalizeImportString(Tmp);
		info.Label=Tmp;

		// Cover?
		Tag.GetPicture(3,info.Cover);
		if (info.Cover.Size()>0) {
			QPixmap pix, icon;
			pix.loadFromData((const uchar*)info.Cover.GetPtr(),info.Cover.GetSize());
			icon=pix.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation);
			QByteArray bytes;
			QBuffer buffer(&bytes);
			buffer.open(QIODevice::WriteOnly);
			icon.save(&buffer, "JPEG",wm_main->conf.JpegQualityPreview);
			info.Ti.CoverPreview.Copy(bytes.data(),bytes.size());
		}

		// Falls die Version auch im Titel vorhanden ist, entfernen wir sie
		Title.Replace(Version,"");
		Title.Replace("()","");
		Title.Trim();
		// Falls die Version auch im Comment vorhanden ist, entfernen wir sie
		Comment.Replace(Version,"");
		Comment.Replace("()","");
		Comment.Trim();
		info.Ti.SetTitle(Title);
		info.Ti.SetRemarks(Comment);
		fixIt(info);
		if (Title.Len()>0 && Artist.Len()>0) return true;
	}
	return CopyFromFilename(info,Filename);
}


static bool CopyFromFilename(TrackInfo &info, const ppl6::CString &Filename)
{
	// Vielleicht können wir mit dem Dateinamen was anfangen
	ppl6::CString Name=ppl6::GetFilename(Filename);
	ppl6::CString Tmp;
	ppl6::CString Title, Version;
	NormalizeImportString(Name);
	Name.PregReplace("/.mp3$/i","");

	if (Name.PregMatch("/^[0-9]{3}-/")) {
		Name.Shl(0,4);
		if (Name.PregMatch("/^(.*)-(.*)$/u")) {
			Tmp=Name.GetMatch(1);
			Tmp.Trim();
			info.Ti.SetArtist(Tmp);
			Title=Name.GetMatch(2);
			// Hier könnte die Version drinstehen
			if (Title.PregMatch("/^(.*)\\((.*)\\)")) {
				Tmp=Title.GetMatch(1);
				Tmp.Trim();
				info.Ti.SetTitle(Tmp);
				info.Version=Title.GetMatch(2);
				info.Version.Trim();
			} else if (Title.PregMatch("/^(.*)\\[(.*)\\]")) {
				Tmp=Title.GetMatch(1);
				Tmp.Trim();
				info.Ti.SetTitle(Tmp);
				info.Version=Title.GetMatch(2);
				info.Version.Trim();
			} else {
				Tmp=Title.GetMatch(2);
				Tmp.Trim();
				info.Ti.SetTitle(Tmp);
			}
		}
	}
	fixIt(info);
	return true;
}

bool getTrackInfoFromFile(TrackInfo &info, const ppl6::CString &Filename, int preferedId3Version)
{
	ppl6::CDirEntry de;
	//printf ("getTrackInfoFromFile: %s\n",(const char*)Filename);
	if (ppl6::CFile::Stat(Filename,de)) {
		info.Ti.Size=de.Size;
	} else return false;
	//printf ("debug 1\n");
	info.Ti.Channels=2;
	info.Ti.Flags=1+2;
	info.Ti.ReleaseDate=0;
	info.Ti.LabelId=0;

	ppl6::CFile File;
	if (!File.Open(Filename,"rb")) return false;
	ppl6::AudioInfo ai;
	if (!ppl6::IdentAudioFile(File,ai)) return false;
	//printf ("Ok\n");
	info.Ti.Length=ai.Length/1000;
	info.Ti.Bitrate=ai.Bitrate;
	//info.Ti.Channels=pmp3.
	int ret=0;
	if (ai.Format==ppl6::AudioInfo::MP3 && preferedId3Version==1) ret=CopyFromID3v1Tag(info,Filename,File);
	else if (preferedId3Version==2) ret=CopyFromID3v2Tag(info,Filename,File);
	else ret=CopyFromFilename(info,Filename);
	if (ret) {
		if (info.Version.IsEmpty()) {
			if (info.Ti.Length<5*60) info.Version="Single";
			else info.Version="Maxi";
		}
	}
	return true;
}

ppluint32 findTitleIdByFilename(const ppl6::CString &Filename)
{
	ppl6::CString LowerFilename=Filename;
	LowerFilename.LCase();
	for (int DeviceType=0;DeviceType<MAX_DEVICE_TYPES;DeviceType++) {
		ppl6::CString path=wm_main->conf.DevicePath[DeviceType];
		ppl6::CString LowerPath=path;
		LowerPath.LCase();
		if (path.NotEmpty()) {
			int p=LowerFilename.Instr(LowerPath);
			if (p>=0) {
				ppl6::CString f=Filename.Mid(p);
				f.Replace(path,"");
				//printf ("versuche match: %s\n",(const char*)f);
				if (f.PregMatch("/\\/([0-9]{3})\\/([0-9]+)\\/([0-9]{3})[^0-9]+.*$/")) {
					int myDeviceId=ppl6::atoi(f.GetMatch(1));
					int myPage=ppl6::atoi(f.GetMatch(2));
					int myTrack=ppl6::atoi(f.GetMatch(3));
					//printf ("myDeviceId=%i, myPage=%i, myTrack=%i\n",myDeviceId,myPage,myTrack);
					const DataTrack *tr=wm_main->GetTrack(DeviceType,myDeviceId,myPage,myTrack);
					if (tr) {
						return tr->TitleId;
					}
				} else if (f.PregMatch("/\\/([0-9]{3})\\/([0-9]{3})[^0-9]+.*$/")) {
					int myDeviceId=ppl6::atoi(f.GetMatch(1));
					int myTrack=ppl6::atoi(f.GetMatch(2));
					//printf ("myDeviceId=%i, myTrack=%i\n",myDeviceId,myTrack);
					const DataTrack *tr=wm_main->GetTrack(DeviceType,myDeviceId,1,myTrack);
					if (tr) {
						return tr->TitleId;
					}
				}
			}
		}
	}
	return 0;
}

static int harms[24][6] = {
		{1,3,23,2,0,0},			// 1:  G#m
		{2,1,24,4,0,0},			// 2:  B
		{3,1,5,4,0,0},			// 3:  D#m
		{4,2,6,3,0,0},			// 4:  F#
		{5,3,7,6,0,0},			// 5:  A#m
		{6,4,8,5,0,0},			// 6:  C#
		{7,5,9,8,0,0},			// 7:  Fm
		{8,6,10,7,0,0},			// 8:  G#
		{9,7,11,10,0,0},		// 9:  Cm
		{10,8,12,9,0,0},		// 10: D#
		{11,9,13,12,0,0},		// 11: Gm
		{12,10,14,11,0,0},		// 12: A#
		{13,11,15,14,0,0},		// 13: Dm
		{14,12,16,13,0,0},		// 14: F
		{15,13,17,16,0,0},		// 15: Am
		{16,14,18,15,0,0},		// 16: C
		{17,15,19,18,0,0},		// 17: Em
		{18,16,20,17,0,0},		// 18: G
		{19,17,21,20,0,0},		// 19: Bm
		{20,18,22,19,0,0},		// 20: D
		{21,19,23,22,0,0},		// 21: F#m
		{22,20,24,21,0,0},		// 22: A
		{23,21,1,24,0,0},		// 23: C#m
		{24,22,2,23,0,0}		// 24: E

};

void getHarmonicKeys(std::set<int> &harmonics, int key)
{
	harmonics.clear();
	if (key<1 || key>24) return;
	key--;
	for (int i=0;i<6;i++) {
		if (harms[key][i]>0) harmonics.insert(harms[key][i]);
		else break;
	}
}


#define KEY_MINOR	128
#define KEY_MAJOR	0

static int wm2openkey[25] = {
		0,
		6|KEY_MINOR,
		6|KEY_MAJOR,
		7|KEY_MINOR,
		7|KEY_MAJOR,
		8|KEY_MINOR,
		8|KEY_MAJOR,
		9|KEY_MINOR,
		9|KEY_MAJOR,
		10|KEY_MINOR,
		10|KEY_MAJOR,
		11|KEY_MINOR,
		11|KEY_MAJOR,
		12|KEY_MINOR,
		12|KEY_MAJOR,
		1|KEY_MINOR,
		1|KEY_MAJOR,
		2|KEY_MINOR,
		2|KEY_MAJOR,
		3|KEY_MINOR,
		3|KEY_MAJOR,
		4|KEY_MINOR,
		4|KEY_MAJOR,
		5|KEY_MINOR,
		5|KEY_MAJOR,
};

enum HarmonicTypes {
			sameKey=0,
			nextKey,
			minorMajorSwitch,
			minorMajorJump,
			semitoneUpOrDown,
		};

class Harmonics
{
	public:
		enum HarmonicTypes {
			sameKey=0,
			previousKey,		// -1 on wheel
			nextKey,			// +1 on wheel
			minorMajorSwitch,
			minorMajorJump,     // from major -3 on wheel to minor
								// from minor +3 on wheel to major
			semitoneUp,			// +7 on wheel
			semitoneDown,		// -7 on wheel
			twoSemitoneUp,		// +2 on wheel
			twoSemitoneDown		// -2 on wheel
		};
		int key;
		HarmonicTypes type;

};


static Harmonics newharms[24][9] = {
		{	// 1: G#m, 6m
				{1,Harmonics::sameKey},
				{23,Harmonics::previousKey},
				{3,Harmonics::nextKey},
				{2,Harmonics::minorMajorSwitch},
				{8,Harmonics::minorMajorJump},
				{15,Harmonics::semitoneUp},
				{11,Harmonics::semitoneDown},
				{5,Harmonics::twoSemitoneUp},
				{21,Harmonics::twoSemitoneDown},
		},
		{	// 2: B, 6d
				{2,Harmonics::sameKey},
				{24,Harmonics::previousKey},
				{4,Harmonics::nextKey},
				{1,Harmonics::minorMajorSwitch},
				{19,Harmonics::minorMajorJump},
				{16,Harmonics::semitoneUp},
				{12,Harmonics::semitoneDown},
				{6,Harmonics::twoSemitoneUp},
				{22,Harmonics::twoSemitoneDown},
		},

};


void getHarmonicKeys(std::map<int,int> &harmonics, int key)
{
	if (key<1 || key>24) return;
	int openkey=wm2openkey[key]&31;
	int minor=wm2openkey[key]&KEY_MINOR;
	harmonics.clear();
}


bool saveCover(const ppl6::CString &filename, const QPixmap &Cover)
{
	if (!wm_main->conf.bWriteID3Tags) return false;
	if (filename.IsEmpty()) return false;
	if (Cover.isNull()) return false;
	ppl6::CID3Tag Tag;
	Tag.Load(filename);
	// Fehler beim Laden wird ignoriert, es koennte eine Datei ohne ID3-Tags sein
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	Cover.save(&buffer, "JPEG",wm_main->conf.JpegQualityCover);
	ppl6::CBinary bin;
	bin.Copy(bytes.data(),bytes.size());
	Tag.SetPicture(3,bin,"image/jpeg");
	return Tag.Save();
}

void getIconFromCover(ppl6::CBinary &bin, const QPixmap &Cover)
{
	QPixmap icon;
	icon=Cover.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation);
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	icon.save(&buffer, "JPEG",70);
	bin.Copy(bytes.data(),bytes.size());
}

void loadCoverToClipboard(const ppl6::CString &Filename)
{
	ppl6::CID3Tag Tag;
	if (Tag.Load(Filename)) {
		ppl6::CBinary Cover;
		Tag.GetPicture(3,Cover);
		if (Cover.Size()>0) {
			QPixmap pix;
			if (pix.loadFromData((const uchar*)Cover.GetPtr(),Cover.GetSize())) {
				QClipboard *clipboard = QApplication::clipboard();
				if (!clipboard) return;
				clipboard->setPixmap(pix);
			}
		}
	}
}

void setReadableLength(QLabel *label, int length)
{
	int hh=0;
	int mm=(int)(length/60);
	int ss=length%60;
	if (mm>59) {
		hh=(int)(mm/60);
		mm=mm%60;
	}
	label->setText(ppl6::ToString("%0d:%02d:%02d",hh,mm,ss));
}


void SetWindowGeometry(QWidget *widget, const ppl6::CString &name)
{
	widget->restoreGeometry(wm_main->GetGeometry(name));
	if (widget->pos().x()<0) widget->move(0,widget->pos().y());
	if (widget->pos().y()<0) widget->move(widget->pos().x(),0);
}

void SaveWindowGeometry(QWidget *widget, const ppl6::CString &name)
{
	wm_main->SaveGeometry(name,widget->saveGeometry());
}


