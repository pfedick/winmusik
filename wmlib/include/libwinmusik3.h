/*
 * libwinmusik3.h
 *
 *  Created on: 20.06.2016
 *      Author: patrick
 */

#ifndef LIBWINMUSIK3_H_
#define LIBWINMUSIK3_H_

#include <wm_musicalkeys.h>
#include <wm_dataobjects.h>
#include <ppl7.h>
#include <ppl7-config.h>
#include <map>

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


class CStorage;




class CProgressUpdate
{
public:
	virtual ~CProgressUpdate()=0;
	virtual void Update(int progress)=0;
};

class CDataBase
{
private:
public:
	CDataBase();
	~CDataBase();
	void clear();
	void load(const ppl7::String& filename);
	static void update(const ppl7::String& oldfile, const ppl7::String& newfile);

};

class CStorage
{
private:
public:
	CStorage();
	~CStorage();

	void loadDatabase(const ppl7::String& filename, CDataBase& db);


};


}
}
}



#endif /* LIBWINMUSIK3_H_ */
