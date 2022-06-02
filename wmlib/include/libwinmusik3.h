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

#ifndef LIBWINMUSIK3_H_
#define LIBWINMUSIK3_H_

#include <ppl7.h>
#include <ppl7-config.h>
#include <map>
#include <wm_musicalkeys.h>
#include <wm_cwmfile.h>
#include <wm_storage.h>


namespace de {
namespace pfp {
namespace winmusik {


#define WM_NUM_LASTPLAYLISTS	5
class Config
{
private:
	ppl7::String	ConfigFile;

public:
	Config();
	void load();
	void save();
	void setConfigFile(const ppl7::String& filename);

	ppl7::String	DataPath;
	ppl7::String	TmpPath;
	ppl7::String	LastCoverPath;


	ppl7::String	MP3Player;
	ppl7::String	AIFFPlayer;
	ppl7::String	Locale;
	ppl7::String	UserName;
	ppl7::String	UserCompany;
	ppl7::String	Serial;
	ppl7::String	Currency;
	ppl7::String	PrinterName;
	ppl7::String	PrinterFont;
	ppl7::String	Logfile;
	ppl7::String	DefaultPrintPath;
	ppl7::String	CoverPath;
	ppl7::String	LastPlaylistPath;
	ppl7::String	LastPlaylists[WM_NUM_LASTPLAYLISTS];

	std::map <int, ppl7::String>	DevicePath;

	ppl7::Array		DirectorySearch;
	int				LogfileSize;
	int				LogfileGenerations;
	int				Debuglevel;
	bool			bPrintColors;
	bool			bShowSplashScreen;
	bool			bCheckForUpdatesOnStartup;
	bool			bAutomaticArtistSuggestion;
	bool			bAutomaticTitleSuggestion;
	int				ReadId3Tag;
	bool			bWriteID3Tags;
	bool			bRemoveOriginalId3Tags;
	int				ID3v2Padding;
	bool			bSaveOriginalMp3Tags;
	bool			bSaveOriginalMp3TagsOnAutoImport;
	bool			bNormalizeFilename;
	int				TagSaverRetryIntervall;
	std::map<int, bool>	SearchDevice;
	bool			SearchFast;
	std::map<int, bool>	VisibleDevice;
	bool			bEnableDebug;
	bool			bCacheDirectorySearch;
	bool			bAutomaticEditSearch;
	int				MaxFilenameLength;

	int				JpegQualityCover;
	int				JpegQualityPreview;

	// Server
	bool			bserverEnabled;
	bool			bserverEnableSSL;
	ppl7::String	serverHost;
	int				serverPort;
	ppl7::String	serverSSLKeyfile;
	ppl7::String	serverSSLPassword;

	ppl7::String	customMusicKey[26];
	ppl7::String	customMusicKeyName;
	MusicKeyType	musicKeyDisplay;
	MusicKeyType	musicKeyTag;
	int				playlistView;


	/*
			class CDDB {
			public:
				CDDB();
				ppl7::String	cddevice;
				ppl7::String	server;
				int				port;
				bool			useProxy;
				ppl7::String	proxy_server;
				int				proxy_port;
				ppl7::String	username;
				ppl7::String	hostname;
				ppl7::String	querypath;


			};
			CDDB			cddb;

			class PlaylistExport {
			public:
				PlaylistExport();
				ppl7::String	TargetPath;
				bool option_copy_files;
				bool option_prepend_tracknumber;
				bool export_m3u;
				bool export_pls;
				bool export_xspf;
				bool export_txt;
			};
			PlaylistExport playlist_export;
		*/
};




class CProgressUpdate
{
public:
	virtual ~CProgressUpdate()=0;
	virtual void ProgressUpdate(int progress)=0;
};





}
}
}



#endif /* LIBWINMUSIK3_H_ */
