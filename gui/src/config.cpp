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
#include "exceptions.h"
#include <QSettings>
#include <QDir>
#include <QLocale>

Config::Config()
{
	Locale=QLocale::system().name();;
	TmpPath=QDir::tempPath();
	LastCoverPath=QDir::homePath();
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, WM_ORGANISATION, WM_APPNAME);
	ConfigFile=settings.fileName();
#ifdef _WIN32
	DataPath=ppl7::File::getPath(ConfigFile);
#else
	DataPath=QDir::homePath();
#endif
	DataPath.replace("\\", "/");
	DataPath.trimRight("/");
	DataPath+="/WinMusik3";
	bPrintColors=true;
	bShowSplashScreen=true;
	bCheckForUpdatesOnStartup=true;
	bAutomaticArtistSuggestion=true;
	bAutomaticTitleSuggestion=true;
	bCacheDirectorySearch=false;
	bAutomaticEditSearch=true;
	ReadId3Tag=2;
	bWriteID3Tags=true;
	bRemoveOriginalId3Tags=false;
	ID3v2Padding=1024;
	LogfileSize=10;
	LogfileGenerations=5;
	Debuglevel=1;
	bSaveOriginalMp3Tags=true;
	bSaveOriginalMp3TagsOnAutoImport=true;
	bNormalizeFilename=true;
	TagSaverRetryIntervall=60;
	for (int i=0;i < MAX_DEVICE_TYPES;i++) {
		SearchDevice[i]=false;
		VisibleDevice[i]=false;
	}
	SearchDevice[1]=true;
	VisibleDevice[1]=true;
	SearchDevice[2]=true;
	VisibleDevice[2]=true;
	SearchDevice[7]=true;
	VisibleDevice[7]=true;

	SearchFast=true;
	bEnableDebug=false;
	bserverEnabled=false;
	bserverEnableSSL=false;
	serverPort=8030;
	musicKeyDisplay=musicKeyTypeOpenKey;
	musicKeyTag=musicKeyTypeMusicalSharps;
	customMusicKeyName=tr("custom format");
	for (int i=0;i < 26;i++) customMusicKey[i]=wm_main->MusicKeys.keyName(i, musicKeyTypeMusicalSharps);
	playlistView=0;

	JpegQualityCover=95;
	JpegQualityPreview=90;
	MaxFilenameLength=255;

}

Config::CDDB::CDDB()
{
	port=80;
	server="freedb.org";
	proxy_port=8080;
	username="anonymous";
	hostname="localhost";
	querypath="/~cddb/cddb.cgi";
	useProxy=false;
}

Config::PlaylistExport::PlaylistExport()
{
	TargetPath=QDir::homePath();
	option_copy_files=true;
	option_prepend_tracknumber=true;
	export_m3u=true;
	export_pls=true;
	export_xspf=true;
	export_txt=true;
	with_start_times=true;
	export_json=false;
	with_cover_icons=false;

}


void Config::setConfigFile(const ppl7::String& filename)
{
	ppl7::DirEntry res;
	if (ppl7::File::exists(filename)) {
		ppl7::DirEntry res=ppl7::File::statFile(filename);
		if (!res.isFile()) {
			throw InvalidConfigurationFile(ToString(tr("alternative configuration file (Parameter -c) is not a regular file! [%s]"), (const char*)filename));
		}
	} else {
		ppl7::String Path=ppl7::File::getPath(filename);
		if (Path.isEmpty()) {
			throw InvalidConfigurationFile(ToString(tr("alternative configuration file (Parameter -c) not found! [%s]"), (const char*)filename));
		}
		if (!ppl7::File::exists(Path)) {
			throw InvalidConfigurationFile(ToString(tr("Path of configuration file does not exist! [%s]"), (const char*)Path));
		}
		ppl7::DirEntry res=ppl7::File::statFile(Path);
		if (!res.isDir()) {
			throw InvalidConfigurationFile(ToString(tr("Path of configuration file is not a valid directory! [%s]"), (const char*)Path));
		}
	}
	ConfigFile=filename;
}

void Config::trySave()
{
	try {
		save();
	} catch (...) {}
}

void Config::save()
{
	ppl7::ConfigParser c;
	//c.Load(ConfigFile);	// Bloss nicht, "Add" hängt die Werte dran, wenn der Key schon da ist!
	c.add("client", "locale", (const char*)Locale);
	c.add("client", "tmppath", (const char*)TmpPath);
	c.add("client", "datapath", (const char*)DataPath);
	c.add("client", "LastCoverPath", (const char*)LastCoverPath);
	c.add("client", "user", (const char*)UserName);
	c.add("client", "company", (const char*)UserCompany);
	c.add("client", "currency", (const char*)Currency);
	c.add("client", "serial", (const char*)Serial);
	c.add("client", "showsplash", bShowSplashScreen);
	c.add("client", "checkupdatestartup", bCheckForUpdatesOnStartup);
	c.add("client", "suggestartist", bAutomaticArtistSuggestion);
	c.add("client", "suggesttitle", bAutomaticTitleSuggestion);
	c.add("client", "automaticsearch", bAutomaticEditSearch);
	c.add("client", "coverpath", (const char*)CoverPath);
	c.add("client", "LastPlaylistPath", (const char*)LastPlaylistPath);
	c.add("client", "playlistView", playlistView);

	c.add("printer", "printername", (const char*)PrinterName);
	c.add("printer", "printerfont", (const char*)PrinterFont);
	c.add("printer", "printcolors", bPrintColors);
	c.add("printer", "defaultprintpath", (const char*)DefaultPrintPath);

	//c.Add("mp3","mp3path",(const char*)DevicePathMP3);
	c.add("mp3", "mp3player", (const char*)MP3Player);
	c.add("mp3", "aiffplayer", (const char*)AIFFPlayer);
	c.add("mp3", "readid3tag", ReadId3Tag);
	c.add("mp3", "bWriteID3Tags", bWriteID3Tags);
	c.add("mp3", "removeoriginalid3tags", bRemoveOriginalId3Tags);
	c.add("mp3", "id3v2padding", ID3v2Padding);
	c.add("mp3", "normalizefilename", bNormalizeFilename);
	c.add("mp3", "saveoriginalmp3tags", bSaveOriginalMp3Tags);
	c.add("mp3", "saveoriginalmp3tagsonautoimport", bSaveOriginalMp3TagsOnAutoImport);
	c.add("mp3", "retryintervall", TagSaverRetryIntervall);
	c.add("mp3", "maxfilenamelength", MaxFilenameLength);

	c.add("mp3", "JpegQualityCover", JpegQualityCover);
	c.add("mp3", "JpegQualityPreview", JpegQualityPreview);

	ppl7::String Key;
	for (int i=1;i < MAX_DEVICE_TYPES;i++) {
		Key.setf("device_%i", i);
		c.add("devicepath", Key, (const char*)DevicePath[i]);
	}


	c.add("debug", "enabledebug", bEnableDebug);
	c.add("debug", "logfile", (const char*)Logfile);
	c.add("debug", "logfilesize", LogfileSize);
	c.add("debug", "logfilegenerations", LogfileGenerations);
	c.add("debug", "debuglevel", Debuglevel);


	for (int i=1;i < MAX_DEVICE_TYPES;i++) {
		Key.setf("device_%i", i);
		c.add("search", Key, SearchDevice[i]);
	}
	c.add("search", "searchfast", SearchFast);
	for (int i=1;i < MAX_DEVICE_TYPES;i++) {
		Key.setf("devicevisible_%i", i);
		c.add("menue", Key, VisibleDevice[i]);
	}

	// Harddisk search
	c.add("directorysearch", "cachedirectorysearch", bCacheDirectorySearch);
	std::list<ppl7::String>::const_iterator it;
	int i;
	for (it=DirectorySearch.begin(), i=0;it != DirectorySearch.end();++it, i++) {
		Key.setf("dir_%d", i);
		c.add("directorysearch", Key, (*it));

	}

	// Server
	c.add("server", "enableserver", bserverEnabled);
	c.add("server", "enablessl", bserverEnableSSL);
	c.add("server", "host", (const char*)serverHost);
	c.add("server", "port", serverPort);
	c.add("server", "sslkeyfile", (const char*)serverSSLKeyfile);
	c.add("server", "sslpassword", (const char*)serverSSLPassword);

	// Playlists
	for (int i=0;i < WM_NUM_LASTPLAYLISTS;i++) {
		c.add("playlists", ppl7::ToString("%i", i + 1), (const char*)LastPlaylists[i]);
	}

	// MusicKey
	c.add("musicKey", "musicKeyDisplay", musicKeyDisplay);
	c.add("musicKey", "musicKeyTag", musicKeyTag);
	c.add("musicKey", "customMusicKeyName", (const char*)customMusicKeyName);
	for (int i=0;i < 26;i++) c.add("musicKey", ppl7::ToString("customMusicKey[%i]", i), (const char*)customMusicKey[i]);

	// CDDB
	c.add("cddb", "cddevice", (const char*)cddb.cddevice);
	c.add("cddb", "server", (const char*)cddb.server);
	c.add("cddb", "port", cddb.port);
	c.add("cddb", "useProxy", cddb.useProxy);
	c.add("cddb", "proxy_server", (const char*)cddb.proxy_server);
	c.add("cddb", "proxy_port", cddb.proxy_port);
	c.add("cddb", "username", (const char*)cddb.username);
	c.add("cddb", "hostname", (const char*)cddb.hostname);
	c.add("cddb", "querypath", (const char*)cddb.querypath);

	// PlaylistExport
	c.add("playlistExport", "TargetPath", (const char*)playlist_export.TargetPath);
	c.add("playlistExport", "option_copy_files", playlist_export.option_copy_files);
	c.add("playlistExport", "option_prepend_tracknumber", playlist_export.option_prepend_tracknumber);
	c.add("playlistExport", "export_m3u", playlist_export.export_m3u);
	c.add("playlistExport", "export_pls", playlist_export.export_pls);
	c.add("playlistExport", "export_xspf", playlist_export.export_xspf);
	c.add("playlistExport", "export_txt", playlist_export.export_txt);
	c.add("playlistExport", "with_start_times", playlist_export.with_start_times);
	c.add("playlistExport", "export_json", playlist_export.export_json);
	c.add("playlistExport", "with_cover_icons", playlist_export.with_cover_icons);

	ppl7::String Path=ppl7::File::getPath(ConfigFile);

	QDir dir;
	if (!dir.exists(Path)) {
		dir.mkpath(Path);
	}
	c.save(ConfigFile);
}

void Config::load()
{
	ppl7::String Tmp;
	ppl7::ConfigParser c;
	c.load(ConfigFile);
	Locale=c.getFromSection("client", "locale", "en");
	Tmp=QDir::tempPath();
	DataPath=c.getFromSection("client", "datapath", Tmp);
	TmpPath=c.getFromSection("client", "tmppath", Tmp);
	UserName=c.getFromSection("client", "user", "");
	UserCompany=c.getFromSection("client", "company", "");
	Currency=c.getFromSection("client", "currency", "");
	Serial=c.getFromSection("client", "serial", "");
	CoverPath=c.getFromSection("client", "coverpath", "");

	Tmp=QDir::homePath();
	LastCoverPath=c.getFromSection("client", "LastCoverPath", Tmp);
	LastPlaylistPath=c.getFromSection("client", "LastPlaylistPath", Tmp);
	playlistView=(MusicKeyType)c.getIntFromSection("client", "playlistView", 0);

	bShowSplashScreen=c.getBoolFromSection("client", "showsplash", true);
	bCheckForUpdatesOnStartup=c.getBoolFromSection("client", "checkupdatestartup", true);
	bAutomaticArtistSuggestion=c.getBoolFromSection("client", "suggestartist", true);
	bAutomaticTitleSuggestion=c.getBoolFromSection("client", "suggesttitle", true);
	bAutomaticEditSearch=c.getBoolFromSection("client", "automaticsearch", true);

	PrinterName=c.getFromSection("printer", "printername", "");
	PrinterFont=c.getFromSection("printer", "printerfont", "");
	DefaultPrintPath=c.getFromSection("printer", "defaultprintpath", "");
	bPrintColors=c.getBoolFromSection("printer", "printcolors", true);

	ppl7::String Key;
	for (int i=1;i < MAX_DEVICE_TYPES;i++) {
		Key.setf("device_%i", i);
		DevicePath[i]=c.getFromSection("devicepath", Key, "");
	}

	Tmp=c.getFromSection("mp3", "mp3path", "");
	if (Tmp.notEmpty()) DevicePath[7]=Tmp;	// Kompatibilität mit Version 3.0.0

	MP3Player=c.getFromSection("mp3", "mp3player", "");
	AIFFPlayer=c.getFromSection("mp3", "aiffplayer", "");
	ReadId3Tag=c.getIntFromSection("mp3", "readid3tag", 2);
	bWriteID3Tags=c.getBoolFromSection("mp3", "bWriteID3Tags", true);
	bRemoveOriginalId3Tags=c.getBoolFromSection("mp3", "removeoriginalid3tags", false);
	ID3v2Padding=c.getIntFromSection("mp3", "id3v2padding", 1024);
	bNormalizeFilename=c.getBoolFromSection("mp3", "normalizefilename", true);
	bSaveOriginalMp3Tags=c.getBoolFromSection("mp3", "saveoriginalmp3tags", true);
	bSaveOriginalMp3TagsOnAutoImport=c.getBoolFromSection("mp3", "saveoriginalmp3tagsonautoimport", true);
	TagSaverRetryIntervall=c.getIntFromSection("mp3", "retryintervall", 60);
	MaxFilenameLength=c.getIntFromSection("mp3", "maxfilenamelength", 64);
	if (MaxFilenameLength < 64) MaxFilenameLength=64;

	JpegQualityCover=c.getIntFromSection("mp3", "JpegQualityCover", 80);
	JpegQualityPreview=c.getIntFromSection("mp3", "JpegQualityPreview", 80);
	if (JpegQualityCover > 100) JpegQualityCover=100;
	if (JpegQualityPreview > 100) JpegQualityPreview=100;
	if (JpegQualityCover < 30) JpegQualityCover=30;
	if (JpegQualityPreview < 30) JpegQualityPreview=30;

	bEnableDebug=c.getBoolFromSection("debug", "enabledebug", false);
	Logfile=c.getFromSection("debug", "logfile", "");
	LogfileSize=c.getIntFromSection("debug", "logfilesize", 10);
	LogfileGenerations=c.getIntFromSection("debug", "logfilegenerations", 5);
	Debuglevel=c.getIntFromSection("debug", "debuglevel", 1);

	for (int i=1;i < MAX_DEVICE_TYPES;i++) {
		Key.setf("device_%i", i);
		SearchDevice[i]=c.getBoolFromSection("search", Key, false);
	}
	SearchFast=c.getBoolFromSection("search", "searchfast", true);

	for (int i=1;i < MAX_DEVICE_TYPES;i++) {
		Key.setf("devicevisible_%i", i);
		VisibleDevice[i]=c.getBoolFromSection("menue", Key, true);
	}


	bCacheDirectorySearch=c.getBoolFromSection("directorysearch", "cachedirectorysearch", false);
	DirectorySearch.clear();
	for (int i=0;;i++) {
		Key.setf("dir_%i", i);
		Tmp=c.getFromSection("directorysearch", Key, "");
		if (Tmp.isEmpty()) break;
		DirectorySearch.push_back(Tmp);
	}

	// Server
	bserverEnabled=c.getBoolFromSection("server", "enableserver", false);
	bserverEnableSSL=c.getBoolFromSection("server", "enablessl", false);
	serverHost=c.getFromSection("server", "host", "");
	serverPort=c.getIntFromSection("server", "port", 8030);
	serverSSLKeyfile=c.getFromSection("server", "sslkeyfile", "");
	serverSSLPassword=c.getFromSection("server", "sslpassword", "");

	// Playlists
	for (int i=0;i < WM_NUM_LASTPLAYLISTS;i++) {
		Key.setf("%i", i + 1);
		LastPlaylists[i]=c.getFromSection("playlists", Key, "");
	}

	// MusicKey
	musicKeyDisplay=(MusicKeyType)c.getIntFromSection("musicKey", "musicKeyDisplay", musicKeyTypeOpenKey);
	musicKeyTag=(MusicKeyType)c.getIntFromSection("musicKey", "musicKeyTag", musicKeyTypeOpenKey);
	Tmp=tr("custom format");
	customMusicKeyName=c.getFromSection("musicKey", "customMusicKeyName", Tmp);

	for (int i=0;i < 26;i++) customMusicKey[i]=c.getFromSection("musicKey",
		(const char*)ppl7::ToString("customMusicKey[%i]", i),
		(const char*)wm_main->MusicKeys.keyName(i, musicKeyTypeMusicalSharps));

	// CDDB
	cddb.cddevice=c.getFromSection("cddb", "cddevice", "");
	cddb.server=c.getFromSection("cddb", "server", "freedb.freedb.org");
	cddb.port=c.getIntFromSection("cddb", "port", 80);
	cddb.useProxy=c.getBoolFromSection("cddb", "useProxy", false);
	cddb.proxy_server=c.getFromSection("cddb", "proxy_server", "");
	cddb.proxy_port=c.getIntFromSection("cddb", "proxy_port", 8080);
	cddb.username=c.getFromSection("cddb", "username", "anonymous");
	cddb.hostname=c.getFromSection("cddb", "hostname", "localhost");
	cddb.querypath=c.getFromSection("cddb", "querypath", "/~cddb/cddb.cgi");

	// PlaylistExport
	Tmp=QDir::homePath();
	playlist_export.TargetPath=c.getFromSection("playlistExport", "TargetPath", Tmp);
	playlist_export.option_copy_files=c.getBoolFromSection("playlistExport", "option_copy_files", true);
	playlist_export.option_prepend_tracknumber=c.getBoolFromSection("playlistExport", "option_prepend_tracknumber", true);
	playlist_export.export_m3u=c.getBoolFromSection("playlistExport", "export_m3u", true);
	playlist_export.export_pls=c.getBoolFromSection("playlistExport", "export_pls", true);
	playlist_export.export_xspf=c.getBoolFromSection("playlistExport", "export_xspf", true);
	playlist_export.export_txt=c.getBoolFromSection("playlistExport", "export_txt", true);
	playlist_export.with_start_times=c.getBoolFromSection("playlistExport", "with_start_times", true);
	playlist_export.export_json=c.getBoolFromSection("playlistExport", "export_json", false);
	playlist_export.with_cover_icons=c.getBoolFromSection("playlistExport", "with_cover_icons", false);

}
