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

#include <QPixmap>
#include <QBuffer>


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
		return true;
	}
	return CopyFromFilename(info,Filename);
}

static bool CopyFromID3v2Tag(TrackInfo &info, const ppl6::CString &Filename, ppl6::CFile &File)
{
	ppl6::CString Tmp;
	ppl6::CID3Tag Tag;
	ppl6::CString Title, Version, Genre, Comment, Artist;
	if (Tag.Load(&File)) {
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

		// ReleaseDate
		Tmp=Tag.GetYear();
		NormalizeImportString(Tmp);
		info.Ti.ReleaseDate=Tmp.ToInt()*10000;

		// Comment
		Comment=Tag.GetComment();
		NormalizeImportString(Comment);

		// Genre
		Tmp=Tag.GetGenre();
		NormalizeImportString(Tmp);
		// Manchmal beginnt das Genre mit einer in Klemmern gesetzten Ziffer.
		// Diese entspricht der GenreId des ID3v1-Tags
		if (Tmp.PregMatch("/^\\(([0-9]+)\\)(.*)$/")) {
			// Wir bevorzugen den Text nach der Klammer
			Genre=Tmp.GetMatch(2);
			Genre.Trim();
			if (Genre.IsEmpty()) {
				// Nur wenn er leer ist, verwenden wir die GenreId
				Genre=Tmp.GetMatch(1);
				Genre.Trim();
				Genre=ppl6::GetID3GenreName(Genre.ToInt());
			}
		} else if (Tmp.PregMatch("/^([0-9]+)$/")) {
			// Manchmal haben wir aber auch nur eine Genre-Ziffer
			Genre=Tmp.GetMatch(1);
			Genre.Trim();
			Genre=ppl6::GetID3GenreName(Genre.ToInt());
		} else {
			Genre=Tmp;
		}
		Genre.Trim();
		if (Genre.NotEmpty()) {
			info.Genre=Genre;
		}

		// Remixer
		if (Title.PregMatch("/^(.*)\\((.*?)\\).*$/")) {
			Tmp=Title.GetMatch(2);
			Title=Title.GetMatch(1);
			Tmp.Trim();
			Title.Trim();
		} else if (Title.PregMatch("/^(.*)\\[(.*?)\\].*$/")) {
			Tmp=Title.GetMatch(2);
			Title=Title.GetMatch(1);
			Tmp.Trim();
			Title.Trim();
		} else {
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
			icon.save(&buffer, "JPEG",70);
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

	ppl6::PPL_MPEG_HEADER pmp3;
	ppl6::CFile File;
	//printf ("Oeffne File: %s\n",(const char*)Filename);
	if (!File.Open(Filename,"rb")) return false;
	//printf ("Ok. rufe Ident auf\n");
	if (!ppl6::IdentMPEG(&File,&pmp3)) return false;
	//printf ("Ok\n");
	info.Ti.Length=pmp3.length;
	info.Ti.Bitrate=pmp3.bitrate;
	//info.Ti.Channels=pmp3.
	int ret=0;
	if (preferedId3Version==1) ret=CopyFromID3v1Tag(info,Filename,File);
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
