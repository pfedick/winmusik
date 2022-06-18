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


#include "winmusik3.h"
#include <QWidget>
#include <QPixmap>
#include <QBuffer>
#include <QLabel>
#include <QClipboard>
#include <QString>
#include "ppl7-audio.h"


static bool CopyFromFilename(TrackInfo& info, const ppl7::String& Filename);
static bool CopyFromID3v1Tag(TrackInfo& info, const ppl7::String& Filename, ppl7::File& File);
static bool CopyFromID3v2Tag(TrackInfo& info, const ppl7::String& Filename, ppl7::File& File);

uint32_t Time2Int(const ppl7::String& Time)
{
	ppl7::String t=Time;
	t.replace(",", ":");
	t.replace(".", ":");
	t.trim();
	if (t.isEmpty()) return 0;
	ppl7::Array a;
	a.explode(Time, ":");
	return a.get(0).toInt() * 60 + a.get(1).toInt();
}

ppl7::String Int2Time(uint32_t Time)
{
	ppl7::String a;
	a.setf("%0i:%02i", (int)(Time / 60), Time % 60);
	return a;
}

ppl7::String Long2Date(const ppl7::String& Format, uint32_t Date)
{
	ppl7::String ret, Tmp;
	if (!Date) return ret;
	Tmp.setf("%u", Date);
	int day=Tmp.right(2).toInt();
	Tmp.chopRight(2);
	int month=Tmp.right(2).toInt();
	Tmp.chopRight(2);
	int year=Tmp.toInt();
	ret=Format;
	Tmp.setf("%02i", day);
	ret.replace("%d", Tmp);
	Tmp.setf("%02i", month);
	ret.replace("%m", Tmp);
	Tmp.setf("%04i", year);
	ret.replace("%Y", Tmp);
	Tmp.setf("%02i", year % 100);
	ret.replace("%y", Tmp);
	return ret;
}


void NormalizeImportString(ppl7::String& Buffer)
{
	Buffer.replace("_", " ");
	Buffer.replace("'", "'");
	Buffer.replace("`", "'");
	Buffer.trim();
}


static void fixArtistAndTitle(const ppl7::String& prefix, TrackInfo& info, const ppl7::String prefix_replace)
{
	ppl7::String regex="/(.*?)\\s" + prefix + "\\s+(.*)$/i";
	ppl7::Array Matches;
	ppl7::String Title=info.Ti.Title;
	ppl7::String Artist=info.Ti.Artist;
	if (Title.pregMatch(regex, Matches)) {
		ppl7::String feat=Matches.get(2);
		feat.trim();
		Title=Matches[1];
		Artist.replace(feat, "");
		Artist.trim();
		Artist.trim(",");
		Artist.replace(",,", ",");
		Artist+=" " + prefix_replace + " " + Matches.get(2);
		info.Ti.SetArtist((const char*)Artist);
		info.Ti.SetTitle((const char*)Title);
	}
}

static void fixIt(TrackInfo& info)
{
	fixArtistAndTitle("feat\\.", info, "feat.");
	fixArtistAndTitle("featuring", info, "feat.");
	fixArtistAndTitle("ft", info, "feat.");
	fixArtistAndTitle("ft.", info, "feat.");
	fixArtistAndTitle("pres\\.", info, "pres.");
	fixArtistAndTitle("presents", info, "pres.");
}

static bool CopyFromID3v1Tag(TrackInfo& info, const ppl7::String& Filename, ppl7::File& File)
{
	ppl7::String Tmp, Title;
	// ID3v1-Tag einlesen falls vorhanden
	if (File.size() < 128) return CopyFromFilename(info, Filename);
	const char* buffer=NULL;
	try {
		buffer=File.map(File.size() - 128, 128);
	} catch (...) {
		return CopyFromFilename(info, Filename);
	}
	if (buffer[0] == 'T' && buffer[1] == 'A' && buffer[2] == 'G') {
		ID3TAG id3;
		memcpy(&id3, buffer, 128);
		Tmp.set(id3.Artist, 30);
		NormalizeImportString(Tmp);

		info.Ti.SetArtist(Tmp);
		Tmp.set(id3.SongName, 30);
		NormalizeImportString(Tmp);
		// Sind Klammern enthalten?
		ppl7::Array matches;
		if (Tmp.pregMatch("/^(.*)\\((.*)\\).*$/", matches)) {
			Title=matches.get(1);
			Title.trim();
			info.Ti.SetTitle(Title);
			info.Version=matches.get(2);
			info.Version.trim();
		} else if (Tmp.pregMatch("/^(.*)\\[(.*)\\].*$/", matches)) {
			Title=matches.get(1);
			Title.trim();
			info.Ti.SetTitle(Title);
			info.Version=matches.get(2);
			info.Version.trim();
		} else {
			info.Ti.SetTitle(Tmp);
		}
		Tmp=id3.Album;
		NormalizeImportString(Tmp);
		info.Ti.SetAlbum(Tmp);
		// ReleaseDate
		info.Ti.ReleaseDate=atoi(id3.Year) * 10000;
		Tmp.set(id3.Comment, 29);
		NormalizeImportString(Tmp);
		info.Ti.SetRemarks(Tmp);
		info.Genre=ppl7::GetID3GenreName(id3.Genre);
		NormalizeImportString(info.Genre);
		fixIt(info);
		return true;
	}
	return CopyFromFilename(info, Filename);
}

static bool CopyFromID3v2Tag(TrackInfo& info, const ppl7::String& Filename, ppl7::File& File)
{
	ppl7::String Tmp;
	ppl7::ID3Tag Tag;
	ppl7::String Title, Version, Genre, Comment, Artist;
	try {
		Tag.load(File);
	} catch (...) {
		return CopyFromFilename(info, Filename);
	}
	Title=Tag.getTitle();
	NormalizeImportString(Title);

	Artist=Tag.getArtist();
	NormalizeImportString(Artist);
	info.Ti.SetArtist(Artist);

	// Album
	Tmp=Tag.getAlbum();
	NormalizeImportString(Tmp);
	info.Ti.SetAlbum(Tmp);

	// POPM
	info.Ti.Rating=(int)Tag.getPopularimeter("winmusik@pfp.de") * 5 / 255;
	if (!info.Ti.Rating) info.Ti.Rating=(int)Tag.getPopularimeter() * 5 / 255;
	if (info.Ti.Rating > 0) info.Ti.Rating+=1;

	// BPM
	Tmp=Tag.getBPM();
	NormalizeImportString(Tmp);
	info.Ti.BPM=Tmp.toInt();

	// Music Key
	info.Ti.Key=wm_main->MusicKeys.keyId(Tag.getKey());

	// EnergyLevel
	info.Ti.EnergyLevel=(char)(Tag.getEnergyLevel().toInt() & 255);

	// ReleaseDate
	Tmp=Tag.getYear();
	NormalizeImportString(Tmp);
	info.Ti.ReleaseDate=Tmp.toInt() * 10000;

	// Comment
	Comment=Tag.getComment();
	NormalizeImportString(Comment);

	// Genre
	Genre=Tag.getGenre();
	NormalizeImportString(Genre);
	if (Genre.notEmpty()) {
		info.Genre=Genre;
	}

	// Remixer
	ppl7::Array matches;
	if (Title.pregMatch("/^(.*?)\\s*\\((.*?)\\)\\s*\\((.*?)\\).*$/", matches)) {
		//printf ("Match 0\n");
		ppl7::String Tmp1=matches.get(2);
		ppl7::String Tmp2=matches.get(3);
		Title=matches.get(1);
		Tmp1.trim();
		Tmp2.trim();
		Title.trim();
		//printf ("Tmp1=%s, Tmp2=%s\n",(const char*)Tmp1,(const char*)Tmp2);
		if (Tmp1.pregMatch("/feat\\.\\s+/i")) {
			//printf ("xxx 1\n");
			Tmp=Tmp2;
			Artist+=" " + Tmp1;
			info.Ti.SetArtist(Artist);
		} else if (Tmp2.pregMatch("/feat\\.\\s+/i")) {
			//printf ("xxx 2\n");
			Tmp=Tmp1;
			Artist+=" " + Tmp2;
			info.Ti.SetArtist(Artist);
		} else {
			//printf ("xxx 3\n");
			Title+=" (" + Tmp1 + ")";
			Tmp=Tmp2;
		}
	} else if (Title.pregMatch("/^(.*)\\((.*?)\\).*$/", matches)) {
		//printf ("Match 1\n");
		Tmp=matches.get(2);
		Title=matches.get(1);
		Tmp.trim();
		Title.trim();
	} else if (Title.pregMatch("/^(.*)\\[(.*?)\\].*$/", matches)) {
		//printf ("Match 2\n");
		Tmp=matches.get(2);
		Title=matches.get(1);
		Tmp.trim();
		Title.trim();
	} else {
		//printf ("Match 3\n");
		Tmp=Tag.getRemixer();
		NormalizeImportString(Tmp);
	}
	Tmp.pregReplace("/\\brmx\\b/i", " Remix");
	Tmp.trim();
	Version=Tmp;
	info.Version=Version;

	// Label
	Tmp=Tag.getLabel();
	NormalizeImportString(Tmp);
	info.Label=Tmp;

	// Cover?
	Tag.getPicture(3, info.Cover);
	if (info.Cover.size() > 0) {
		QPixmap pix, icon;
		pix.loadFromData((const uchar*)info.Cover.ptr(), info.Cover.size());
		icon=pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QByteArray bytes;
		QBuffer buffer(&bytes);
		buffer.open(QIODevice::WriteOnly);
		icon.save(&buffer, "JPEG", wm_main->conf.JpegQualityPreview);
		info.Ti.CoverPreview.copy(bytes.data(), bytes.size());
	}

	// Falls die Version auch im Titel vorhanden ist, entfernen wir sie
	Title.replace(Version, "");
	Title.replace("()", "");
	Title.trim();
	// Falls die Version auch im Comment vorhanden ist, entfernen wir sie
	Comment.replace(Version, "");
	Comment.replace("()", "");
	Comment.trim();
	info.Ti.SetTitle(Title);
	info.Ti.SetRemarks(Comment);
	fixIt(info);
	if (Title.size() > 0 && Artist.size() > 0) return true;
	return CopyFromFilename(info, Filename);
}


static bool CopyFromFilename(TrackInfo& info, const ppl7::String& Filename)
{
	// Vielleicht können wir mit dem Dateinamen was anfangen
	ppl7::String Name=ppl7::File::getFilename(Filename);
	ppl7::String Tmp;
	ppl7::String Title, Version;
	NormalizeImportString(Name);
	Name.pregReplace("/.mp3$/i", "");
	Name.pregReplace("/.wav$/i", "");
	Name.pregReplace("/.aif$/i", "");
	Name.pregReplace("/.aiff$/i", "");
	if (Name.pregMatch("/^[0-9]{3}-/")) {
		ppl7::Array matches;
		Name.shl(0, 4);
		if (Name.pregMatch("/^(.*)-(.*)$/u", matches)) {
			Tmp=matches.get(1);
			Tmp.trim();
			info.Ti.SetArtist(Tmp);
			Title=matches.get(2);
			// Hier könnte die Version drinstehen
			if (Title.pregMatch("/^(.*)\\((.*)\\)", matches)) {
				Tmp=matches.get(1);
				Tmp.trim();
				info.Ti.SetTitle(Tmp);
				info.Version=matches.get(2);
				info.Version.trim();
			} else if (Title.pregMatch("/^(.*)\\[(.*)\\]", matches)) {
				Tmp=matches.get(1);
				Tmp.trim();
				info.Ti.SetTitle(Tmp);
				info.Version=matches.get(2);
				info.Version.trim();
			} else {
				Tmp=matches.get(2);
				Tmp.trim();
				info.Ti.SetTitle(Tmp);
			}
		}
	}
	fixIt(info);
	return true;
}

bool getTrackInfoFromFile(TrackInfo& info, const ppl7::String& Filename, int preferedId3Version)
{
	ppl7::DirEntry de;
	if (ppl7::File::tryStatFile(Filename, de)) {
		info.Ti.Size=de.Size;
	} else return false;
	//printf ("debug 1\n");
	info.Ti.Channels=2;
	info.Ti.Flags=1 + 2;
	info.Ti.ReleaseDate=0;
	info.Ti.LabelId=0;

	ppl7::File File;
	try {
		File.open(Filename);
	} catch (...) {
		return false;
	}
	ppl7::AudioInfo ai;
	if (!ppl7::IdentAudioFile(File, ai)) return false;
	//printf ("Ok\n");
	info.Ti.Length=ai.Length / 1000;
	info.Ti.Bitrate=ai.Bitrate;
	//info.Ti.Channels=pmp3.
	int ret=0;
	if (ai.Format == ppl7::AudioInfo::MP3 && preferedId3Version == 1) ret=CopyFromID3v1Tag(info, Filename, File);
	else if (preferedId3Version == 2) ret=CopyFromID3v2Tag(info, Filename, File);
	else ret=CopyFromFilename(info, Filename);
	if (ret) {
		if (info.Version.isEmpty()) {
			if (info.Ti.Length < 5 * 60) info.Version="Single";
			else info.Version="Maxi";
		}
	}
	return true;
}

uint32_t findTitleIdByFilename(const ppl7::String& Filename)
{
	ppl7::String LowerFilename=Filename;
	LowerFilename.lowerCase();
	for (int DeviceType=0;DeviceType < MAX_DEVICE_TYPES;DeviceType++) {
		ppl7::String path=wm_main->conf.DevicePath[DeviceType];
		ppl7::String LowerPath=path;
		LowerPath.lowerCase();
		if (path.notEmpty()) {
			int p=LowerFilename.instr(LowerPath);
			if (p >= 0) {
				ppl7::String f=Filename.mid(p);
				f.replace(path, "");
				//printf ("versuche match: %s\n",(const char*)f);
				ppl7::Array matches;
				if (f.pregMatch("/\\/([0-9]{3})\\/([0-9]+)\\/([0-9]{3})[^0-9]+.*$/", matches)) {
					int myDeviceId=matches.get(1).toInt();
					int myPage=matches.get(2).toInt();
					int myTrack=matches.get(3).toInt();
					//printf ("myDeviceId=%i, myPage=%i, myTrack=%i\n",myDeviceId,myPage,myTrack);
					const DataTrack* tr=wm_main->GetTrack(DeviceType, myDeviceId, myPage, myTrack);
					if (tr) {
						return tr->TitleId;
					}
				} else if (f.pregMatch("/\\/([0-9]{3})\\/([0-9]{3})[^0-9]+.*$/", matches)) {
					int myDeviceId=matches.get(1).toInt();
					int myTrack=matches.get(2).toInt();
					//printf ("myDeviceId=%i, myTrack=%i\n",myDeviceId,myTrack);
					const DataTrack* tr=wm_main->GetTrack(DeviceType, myDeviceId, 1, myTrack);
					if (tr) {
						return tr->TitleId;
					}
				}
			}
		}
	}
	return 0;
}


void saveCover(const ppl7::String& filename, const QPixmap& Cover)
{
	if (!wm_main->conf.bWriteID3Tags) return;
	if (filename.isEmpty()) return;
	if (Cover.isNull()) return;
	ppl7::ID3Tag Tag;
	try {
		Tag.load(filename);
	} catch (...) {
		// Fehler beim Laden wird ignoriert, es koennte eine Datei ohne ID3-Tags sein
	}
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	Cover.save(&buffer, "JPEG", wm_main->conf.JpegQualityCover);
	ppl7::ByteArray bin;
	bin.copy(bytes.data(), bytes.size());
	Tag.setPicture(3, bin, "image/jpeg");
	try {
		Tag.save();
	} catch (const ppl7::Exception& exp) {
		ShowException(exp, QObject::tr("Could not save cover!"));
	}
}

void getIconFromCover(ppl7::ByteArray& bin, const QPixmap& Cover)
{
	QPixmap icon;
	icon=Cover.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	icon.save(&buffer, "JPEG", 70);
	bin.copy(bytes.data(), bytes.size());
}

void loadCoverToClipboard(const ppl7::String& Filename)
{
	ppl7::ID3Tag Tag;
	try {
		Tag.load(Filename);
		ppl7::ByteArray Cover;
		Tag.getPicture(3, Cover);
		if (Cover.size() > 0) {
			QPixmap pix;
			if (pix.loadFromData((const uchar*)Cover.ptr(), Cover.size())) {
				QClipboard* clipboard = QApplication::clipboard();
				if (!clipboard) return;
				clipboard->setPixmap(pix);
			}
		}
	} catch (...) {

	}
}

void setReadableLength(QLabel* label, int length)
{
	label->setText(getReadableTimeFromSeconds(length));
}


void SetWindowGeometry(QWidget* widget, const ppl7::String& name)
{
	widget->restoreGeometry(wm_main->GetGeometry(name));
	if (widget->pos().x() < 0) widget->move(0, widget->pos().y());
	if (widget->pos().y() < 0) widget->move(widget->pos().x(), 0);
}

void SaveWindowGeometry(QWidget* widget, const ppl7::String& name)
{
	wm_main->SaveGeometry(name, widget->saveGeometry());
}

ppl7::String getReadableTimeFromSeconds(int seconds)
{
	ppl7::String Tmp;
	int hours=(int)(seconds / 3600);
	int rest=seconds - (hours * 3600);
	int minutes=(int)(rest / 60);
	rest-=minutes * 60;
	Tmp.setf("%02d:%02d:%02d", hours, minutes, rest);
	return Tmp;
}


ppl7::String ToString(const QString& fmt, ...)
{
	const char* utf8fmt=fmt.toUtf8().constData();
	ppl7::String s;
	va_list args;
	va_start(args, fmt);
	s.vasprintf(utf8fmt, args);
	va_end(args);
	return s;
}

void ShowException(const ppl7::Exception& exp, const QString& msg)
{
	QString m;
	if (!msg.isEmpty()) {
		m="<b>" + msg.trimmed() + "</b><p>\n\n";
	}
	QString info=exp.what();
	m+=QObject::tr("Exception:") + " " + info + "<br>\n";
	const char* additional=exp.text();
	if (additional) {
		info=additional;
		m+=QObject::tr("Description:") + " " + info;
	}

	QMessageBox::critical(NULL, QObject::tr("WinMusik Error"),
		m,
		QMessageBox::Ok);
}

void ShowError(const QString& msg)
{
	QString m;
	if (!msg.isEmpty()) {
		m="<b>" + msg.trimmed() + "</b>\n";
	} else {
		m="<b>" + QObject::tr("an error occured") + "</b>\n";
	}
	QMessageBox::critical(NULL, QObject::tr("WinMusik Error"),
		m,
		QMessageBox::Ok);
}
