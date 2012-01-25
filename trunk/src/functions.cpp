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

static bool CopyFromID3v1Tag(TrackInfo &info, const ppl6::CString &Filename, ppl6::CFile &File)
{
	ppl6::CString Tmp,Title;
	// ID3v1-Tag einlesen falls vorhanden
	const char *buffer=File.Map(File.Lof()-128,128);
	if (buffer[0]=='T' && buffer[1]=='A' && buffer[2]=='G') {
		ID3TAG id3;
		memcpy (&id3,buffer,128);
		Tmp.Set(id3.Artist,30);
		Tmp.Trim();
		info.Ti.SetArtist(Tmp);
		Tmp.Set(id3.SongName,30);
		Tmp.Trim();
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
		Tmp.Trim();
		info.Ti.SetAlbum(Tmp);
		// ReleaseDate
		info.Ti.ReleaseDate=ppl6::atoi(id3.Year)*10000;
		Tmp=id3.Comment;
		Tmp.Trim();
		info.Ti.SetRemarks(Tmp);
		info.Genre=ppl6::GetID3GenreName(id3.Genre);
		info.Genre.Trim();
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
		Title.Replace("_"," ");
		Title.Trim();

		Artist=Tag.GetArtist();
		Artist.Replace("_"," ");
		Artist.Trim();

		info.Ti.SetArtist(Artist);

		// Album
		Tmp=Tag.GetAlbum();
		Tmp.Replace("_"," ");
		Tmp.Trim();
		info.Ti.SetAlbum(Tmp);

		// BPM
		Tmp=Tag.GetBPM();
		Tmp.Replace("_"," ");
		Tmp.Trim();
		info.Ti.BPM=Tmp.ToInt();

		// ReleaseDate
		QDate Date;
		Tmp=Tag.GetYear();
		Tmp.Replace("_"," ");
		Tmp.Trim();
		info.Ti.ReleaseDate=Tmp.ToInt()*10000;

		// Comment
		Comment=Tag.GetComment();
		Comment.Replace("_"," ");
		Comment.Trim();

		Tmp=Tag.GetGenre();
		Tmp.Replace("_"," ");
		Tmp.Trim();
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
		Tmp=Tag.GetRemixer();
		Tmp.Replace("_"," ");
		Tmp.Trim();
		// Falls wir keinen Remixer haben, schauen wir im Titel nach
		if (!Tmp.Len()) {
			if (Title.PregMatch("/^(.*)\\((.*)\\).*$/")) {
				Tmp=Title.GetMatch(2);
				Tmp.Trim();
				Title=Title.GetMatch(1);
			} else if (Title.PregMatch("/^(.*)\\[(.*)\\].*$/")) {
				Tmp=Title.GetMatch(2);
				Tmp.Trim();
				Title=Title.GetMatch(1);
			}
		}
		Tmp.PregReplace("/\\brmx\\b/i"," Remix");
		Tmp.Trim();
		Version=Tmp;
		info.Version=Version;

		// Label
		Tmp=Tag.GetLabel();
		Tmp.Replace("_"," ");
		Tmp.Trim();
		info.Label=Tmp;

		// Cover?
		ppl6::CBinary Cover;
		Tag.GetPicture(3,Cover);
		if (Cover.Size()>0) {
			QPixmap pix, icon;
			pix.loadFromData((const uchar*)Cover.GetPtr(),Cover.GetSize());
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
	Name.Replace("_"," ");
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

void getTrackInfoFromFile(TrackInfo &info, const ppl6::CString &Filename, int preferedId3Version)
{
	ppl6::CDirEntry de;
	if (ppl6::CFile::Stat(Filename,de)) {
		info.Ti.Size=de.Size;
	}
	info.Ti.Channels=2;
	info.Ti.Flags=1+2;

	ppl6::PPL_MPEG_HEADER pmp3;
	ppl6::CFile File;
	//printf ("Oeffne File: %s\n",(const char*)Filename);
	if (!File.Open("%s","rb", (const char*) Filename)) return;
	//printf ("Ok. rufe Ident auf\n");
	if (!ppl6::IdentMPEG(&File,&pmp3)) return;
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
		}
	}
}

