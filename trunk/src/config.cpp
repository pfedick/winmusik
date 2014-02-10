/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/10/10 10:31:01 $
 * $Id: config.cpp,v 1.5 2010/10/10 10:31:01 pafe Exp $
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
#include <QSettings>
#include <QDir>

Config::Config()
{
	Locale=QLocale::system().name();;
	TmpPath=QDir::tempPath();
	LastCoverPath=QDir::homePath();
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, WM_ORGANISATION,WM_APPNAME);
	ConfigFile=settings.fileName();
#ifdef _WIN32
	DataPath=ppl6::GetPath(ConfigFile);
#else
	DataPath=QDir::homePath();
#endif
	DataPath.Replace("\\","/");
	DataPath.RTrim("/");
	DataPath+="/WinMusik3";
	bPrintColors=true;
	bShowSplashScreen=true;
	bCheckForUpdatesOnStartup=true;
	bAutomaticArtistSuggestion=true;
	bAutomaticTitleSuggestion=true;
	bCacheDirectorySearch=true;
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
	for (int i=0;i<MAX_DEVICE_TYPES;i++) {
		SearchDevice[i]=false;
		VisibleDevice[i]=true;
	}
	SearchFast=true;
	bEnableDebug=false;
	bserverEnabled=false;
	bserverEnableSSL=false;
	serverPort=8030;
	musicKeyDisplay=musicKeyTypeOpenKey;
	musicKeyTag=musicKeyTypeOpenKey;
	customMusicKeyName=tr("custom format");
	for (int i=0;i<26;i++) customMusicKey[i]=DataTitle::keyName(i,musicKeyTypeMusicalSharps);
	playlistView=0;
}

int Config::setConfigFile(const ppl6::CString &filename)
{
	ppl6::CDirEntry res;
	if (ppl6::CFile::Stat(filename,res)) {
		if (!res.IsFile()) {
			ppl6::CString e="alternative configurationfile (Parameter -c) is not a regular file! ";
			e+="("+filename+")";
			ppl6::SetError(20043,e);
			return 0;
		}
	} else {
		ppl6::CString Path=ppl6::GetPath(filename);
		if (ppl6::CFile::Stat(Path,res)) {
			if (res.IsDir()!=1) {
				ppl6::CString e="Path of configuration file does not exist or is not a valid directory! ";
				e+="("+filename+")";
				ppl6::SetError(20043,e);
				return 0;
			}
		} else {
			ppl6::CString e="Path of configuration file does not exist or is not a valid directory! ";
			e+="("+filename+")";
			ppl6::SetError(20043,e);
			return 0;
		}
	}
	ConfigFile=filename;
	return 1;
}


int Config::Save()
{
	ppl6::CConfig c;
	//c.Load(ConfigFile);	// Bloss nicht, "Add" hängt die Werte dran, wenn der Key schon da ist!
	c.Add("client","locale",(const char*)Locale);
	c.Add("client","tmppath",(const char*)TmpPath);
	c.Add("client","datapath",(const char*)DataPath);
	c.Add("client","LastCoverPath",(const char*)LastCoverPath);
	c.Add("client","user",(const char*)UserName);
	c.Add("client","company",(const char*)UserCompany);
	c.Add("client","currency",(const char*)Currency);
	c.Add("client","serial",(const char*)Serial);
	c.Add("client","showsplash",bShowSplashScreen);
	c.Add("client","checkupdatestartup",bCheckForUpdatesOnStartup);
	c.Add("client","suggestartist",bAutomaticArtistSuggestion);
	c.Add("client","suggesttitle",bAutomaticTitleSuggestion);
	c.Add("client","automaticsearch",bAutomaticEditSearch);
	c.Add("client","coverpath",(const char*)CoverPath);
	c.Add("client","LastPlaylistPath",(const char*)LastPlaylistPath);
	c.Add("client","playlistView",playlistView);

	c.Add("printer","printername",(const char*)PrinterName);
	c.Add("printer","printerfont",(const char*)PrinterFont);
	c.Add("printer","printcolors",bPrintColors);
	c.Add("printer","defaultprintpath",(const char*)DefaultPrintPath);

	//c.Add("mp3","mp3path",(const char*)DevicePathMP3);
	c.Add("mp3","mp3player",(const char*)MP3Player);
	c.Add("mp3","readid3tag",ReadId3Tag);
	c.Add("mp3","bWriteID3Tags",bWriteID3Tags);
	c.Add("mp3","removeoriginalid3tags",bRemoveOriginalId3Tags);
	c.Add("mp3","id3v2padding",ID3v2Padding);
	c.Add("mp3","normalizefilename",bNormalizeFilename);
	c.Add("mp3","saveoriginalmp3tags",bSaveOriginalMp3Tags);
	c.Add("mp3","saveoriginalmp3tagsonautoimport",bSaveOriginalMp3TagsOnAutoImport);
	c.Add("mp3","retryintervall",TagSaverRetryIntervall);
	c.Add("mp3","maxfilenamelength",MaxFilenameLength);

	ppl6::CString Key;
	for (int i=1;i<MAX_DEVICE_TYPES;i++) {
		Key.Setf("device_%i",i);
		c.Add("devicepath",Key,(const char*)DevicePath[i]);
	}


	c.Add("debug","enabledebug",bEnableDebug);
	c.Add("debug","logfile",(const char*)Logfile);
	c.Add("debug","logfilesize",LogfileSize);
	c.Add("debug","logfilegenerations",LogfileGenerations);
	c.Add("debug","debuglevel",Debuglevel);


	for (int i=1;i<MAX_DEVICE_TYPES;i++) {
		Key.Setf("device_%i",i);
		c.Add("search",Key,SearchDevice[i]);
	}
	c.Add("search","searchfast",SearchFast);
	for (int i=1;i<MAX_DEVICE_TYPES;i++) {
		Key.Setf("devicevisible_%i",i);
		c.Add("menue",Key,VisibleDevice[i]);
	}

	// Harddisk search
	c.Add("directorysearch","cachedirectorysearch",bCacheDirectorySearch);
	for (int i=0;i<DirectorySearch.Num();i++) {
		Key.Setf("dir_%i",i);
		c.Add("directorysearch",Key,DirectorySearch[i]);
	}


	ppl6::CString Path=ppl6::GetPath(ConfigFile);
	if (!ppl6::IsDir(Path)) {
		QDir dir;
		dir.mkpath(Path);
	}

	// Server
	c.Add("server","enableserver",bserverEnabled);
	c.Add("server","enablessl",bserverEnableSSL);
	c.Add("server","host",(const char*)serverHost);
	c.Add("server","port",serverPort);
	c.Add("server","sslkeyfile",(const char*)serverSSLKeyfile);
	c.Add("server","sslpassword",(const char*)serverSSLPassword);

	// Playlists
	for (int i=0;i<WM_NUM_LASTPLAYLISTS;i++) {
		c.Add("playlists",ppl6::ToString("%i",i+1),(const char*)LastPlaylists[i]);
	}

	// MusicKey
	c.Add("musicKey","musicKeyDisplay",musicKeyDisplay);
	c.Add("musicKey","musicKeyTag",musicKeyTag);
	c.Add("musicKey","customMusicKeyName",(const char*)customMusicKeyName);
	for (int i=0;i<26;i++) c.Add("musicKey",(const char*)ppl6::ToString("customMusicKey[%i]",i),(const char*)customMusicKey[i]);


	if (!c.Save(ConfigFile)) return 0;
	return 1;
}

int Config::Load()
{
	ppl6::CString Tmp;
	ppl6::CConfig c;
	if (!c.Load("%s",(const char *)ConfigFile)) return 0;
	Locale=c.Get("client","locale","en");
	Tmp=QDir::tempPath();
	DataPath=c.Get("client","datapath",Tmp);
	TmpPath=c.Get("client","tmppath",Tmp);
	UserName=c.Get("client","user",NULL);
	UserCompany=c.Get("client","company",NULL);
	Currency=c.Get("client","currency",NULL);
	Serial=c.Get("client","serial",NULL);
	CoverPath=c.Get("client","coverpath",NULL);

	Tmp=QDir::homePath();
	LastCoverPath=c.Get("client","LastCoverPath",Tmp);
	LastPlaylistPath=c.Get("client","LastPlaylistPath",Tmp);
	playlistView=(MusicKeyType)c.GetInt("client","playlistView",0);

	bShowSplashScreen=c.GetBool("client","showsplash",true);
	bCheckForUpdatesOnStartup=c.GetBool("client","checkupdatestartup",true);
	bAutomaticArtistSuggestion=c.GetBool("client","suggestartist",true);
	bAutomaticTitleSuggestion=c.GetBool("client","suggesttitle",true);
	bAutomaticEditSearch=c.GetBool("client","automaticsearch",true);

	PrinterName=c.Get("printer","printername",NULL);
	PrinterFont=c.Get("printer","printerfont",NULL);
	DefaultPrintPath=c.Get("printer","defaultprintpath",NULL);
	bPrintColors=c.GetBool("printer","printcolors",true);

	ppl6::CString Key;
	for (int i=1;i<MAX_DEVICE_TYPES;i++) {
		Key.Setf("device_%i",i);
		DevicePath[i]=c.Get("devicepath",Key,NULL);
	}

	Tmp=c.Get("mp3","mp3path",NULL);
	if (Tmp.NotEmpty()) DevicePath[7]=Tmp;	// Kompatibilität mit Version 3.0.0

	MP3Player=c.Get("mp3","mp3player",NULL);
	ReadId3Tag=c.GetInt("mp3","readid3tag",2);
	bWriteID3Tags=c.GetBool("mp3","bWriteID3Tags",true);
	bRemoveOriginalId3Tags=c.GetBool("mp3","removeoriginalid3tags",false);
	ID3v2Padding=c.GetInt("mp3","id3v2padding",1024);
	bNormalizeFilename=c.GetBool("mp3","normalizefilename",true);
	bSaveOriginalMp3Tags=c.GetBool("mp3","saveoriginalmp3tags",true);
	bSaveOriginalMp3TagsOnAutoImport=c.GetBool("mp3","saveoriginalmp3tagsonautoimport",true);
	TagSaverRetryIntervall=c.GetInt("mp3","retryintervall",60);
	MaxFilenameLength=c.GetInt("mp3","maxfilenamelength",64);
	if (MaxFilenameLength<64) MaxFilenameLength=64;

	bEnableDebug=c.GetBool("debug","enabledebug",false);
	Logfile=c.Get("debug","logfile","");
	LogfileSize=c.GetInt("debug","logfilesize",10);
	LogfileGenerations=c.GetInt("debug","logfilegenerations",5);
	Debuglevel=c.GetInt("debug","debuglevel",1);

	for (int i=1;i<MAX_DEVICE_TYPES;i++) {
		Key.Setf("device_%i",i);
		SearchDevice[i]=c.GetBool("search",Key,false);
	}
	SearchFast=c.GetBool("search","searchfast",true);

	for (int i=1;i<MAX_DEVICE_TYPES;i++) {
		Key.Setf("devicevisible_%i",i);
		VisibleDevice[i]=c.GetBool("menue",Key,true);
	}


	bCacheDirectorySearch=c.GetBool("directorysearch","cachedirectorysearch",false);
	DirectorySearch.Clear();
	for (int i=0;;i++) {
		Key.Setf("dir_%i",i);
		Tmp=c.Get("directorysearch",Key,"");
		if (Tmp.IsEmpty()) break;
		DirectorySearch.Add(Tmp);
	}

	// Server
	bserverEnabled=c.GetBool("server","enableserver",false);
	bserverEnableSSL=c.GetBool("server","enablessl",false);
	serverHost=c.Get("server","host","");
	serverPort=c.GetInt("server","port",8030);
	serverSSLKeyfile=c.Get("server","sslkeyfile","");
	serverSSLPassword=c.Get("server","sslpassword","");

	// Playlists
	for (int i=0;i<WM_NUM_LASTPLAYLISTS;i++) {
		Key.Setf("%i",i+1);
		LastPlaylists[i]=c.Get("playlists",Key,"");
	}

	// MusicKey
	musicKeyDisplay=(MusicKeyType)c.GetInt("musicKey","musicKeyDisplay",musicKeyTypeOpenKey);
	musicKeyTag=(MusicKeyType)c.GetInt("musicKey","musicKeyTag",musicKeyTypeOpenKey);
	Tmp=tr("custom format");
	customMusicKeyName=c.Get("musicKey","customMusicKeyName",(const char*)Tmp);

	for (int i=0;i<26;i++) customMusicKey[i]=c.Get("musicKey",
			(const char*)ppl6::ToString("customMusicKey[%i]",i),
			(const char*)DataTitle::keyName(i,musicKeyTypeMusicalSharps));


	return 1;
}
