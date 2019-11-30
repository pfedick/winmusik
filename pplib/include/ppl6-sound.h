/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 6 (PPL6).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#ifndef _PPL6_INCLUDE_SOUND
#define _PPL6_INCLUDE_SOUND
#ifndef _PPL6_INCLUDE
    #ifdef PPL6LIB
        #include "ppl6.h"
    #else
        #include <ppl6.h>
    #endif
#endif


#include <stdlib.h>
#include <list>
#include <map>

namespace ppl6 {

namespace MP3 {
	//!\brief Namespace für MP3-spezifische Definitionen
	namespace MODE {
		//!\brief Channel-Mode für MP3-Dateien
		enum {
			STEREO=0,
			MONO,
			DUAL_CHANNEL,
			JOINT_STEREO
		};
	}
}

// TODO: Defines umwandeln in Enumerations im Namespace MP3
#define         MPG_MD_STEREO           0
#define         MPG_MD_JOINT_STEREO     1
#define         MPG_MD_DUAL_CHANNEL     2
#define         MPG_MD_MONO             3

#define         MPG_MD_LR_LR             0
#define         MPG_MD_LR_I              1
#define         MPG_MD_MS_LR             2
#define         MPG_MD_MS_I              3


#define SAMPLE	pplint16
//!\brief Struktur zum Speichern eines Stereo-Samples
typedef struct tagSTEREOSAMPLE{
    pplint16 left;
    pplint16 right;
} STEREOSAMPLE;


//!\brief Struktur zum Speichern eines WAVE-Headers
typedef struct tagWAVEHeader {
	ppldd	datastart;
	ppldd	numSamples;
	ppldd	bytes;
	ppldd	frequency;
	ppldd	bytespersample;
	ppldd	bytespersecond;
	ppldd	seconds;
	ppldb	channels;
	ppldb	bitdepth;
} PPL_WAVEHEADER;

//!\brief Struktur zum Speichern eines MP3-Headers
typedef struct tagMPEGHeader{
	ppldd start,end;		// Beginn und Ende der Daten
	ppldd size,filesize;
	int version;
    int layer;
    int error_protection;
    int bitrate_index;
	int bitrate;
    int frequency_index;
	int frequency;
    int padding;
    int extension;
    int mode;
    int mode_ext;
    int copyright;
    int original;
    int emphasis;
    int stereo;
	int	framesize;
	int frames;
	ppldd mslength;			// Laenge in ms
	ppldd length;			// Laenge in s
	ppldd samples;
	bool vbr;
} PPL_MPEG_HEADER;

void MpegHeader2Array(ppl6::CAssocArray &a, const PPL_MPEG_HEADER *mpg);
void PrintMpegHeader(const PPL_MPEG_HEADER *mpg);

//!\brief Struktur zum Speichern des Fortschritts bei einem MP3-Encode-Vorgang
typedef struct tagSOUNDPROGRESS{
	void	*data;
	//ppldd	tickspersecond;
	double	timestarted;
	double	timeend;
	double	now;
	double	past;
	float	percent;
	float	faktor;
	double	eta;
	ppld64	position;
	ppld64	bytes;
	ppld64	position_thisfile;
	ppld64	bytes_thisfile;
	PPL_WAVEHEADER *wav;
	PPL_MPEG_HEADER *mpg;
} PPL_SOUNDPROGRESS;


const char *GetID3GenreName(int id);

class CMP3Decode;

class CMP3Encode
//!\brief Klasse zum Encoden von MP3-Files
{
	private:
		void * gfp;
		PPL_WAVEHEADER firstwave;
		PPL_SOUNDPROGRESS progress;
		CFileObject *out;
		char			*readcache;
		int				mp3bufsize;
		int				encoderdelay;
		unsigned char	*mp3buf;

		ppldd	samples;

		void (*ProgressFunc) (PPL_SOUNDPROGRESS *prog);

		bool have_firstwave;
		bool started;
		bool bStopEncode;

		void WriteEncodedBytes(char *buffer, ppldd bytes);

		CThread *WatchThread;
		int HandleWatchThread();

	public:
		CMP3Encode();
		~CMP3Encode();


		void SetBitrate(int kbps=192);
		void SetVBR(int min=32, int max=320, int quality=2);
		void SetCBR(int kbps=192, int quality=2);
		void SetABR(int kbps=192, int quality=2);
		void SetQuality(int quality=2);			// 1=best, 10=lowest
		void SetMode(int mode=0);				// 0=Stereo
		void SetLowpassFreq(int freq=-1);		// -1=Disabled
		void SetHighpassFreq(int freq=-1);		// -1=Disabled

		void SetProgressFunction(void (*Progress) (PPL_SOUNDPROGRESS *prog), void *data);
		void SetEncoderDelay(int milliseconds);
		void SetWatchThread(CThread *t);

		int StartEncode(CFileObject *output=NULL);
		int StartEncode(int frequency, int channels);
		int EncodeFile(CFileObject *file);
		int EncodeFile(CMP3Decode *file);
		int EncodeBuffer(SAMPLE *left, SAMPLE *right, int num, void *mp3buf, int buffersize);
		int EncodeBuffer(STEREOSAMPLE *buffer, int num, void *mp3buf, int buffersize);
		int FlushBuffer(void *mp3buf, int buffersize);
		void StopEncode();

		int FinishEncode();

		int ReadWaveHeader(CFileObject *ff, PPL_WAVEHEADER *wav);

		const char *GetLameVersion();
		const char *GetPSYVersion();


};


//!\brief Identifizieren von MP3-Dateien und Einlesen der Header
bool IdentMPEG(CFileObject * file, PPL_MPEG_HEADER * mpg);

class AudioInfo
{
	public:
		AudioInfo();

		enum AudioFormat {
			WAVE,
			AIFF,
			MP3
		};
		enum ChannelMode {
			STEREO,
			MONO,
			DUAL_CHANNEL,
			JOINT_STEREO
		};

		AudioFormat	Format;
		bool		HaveID3v2Tag;
		bool		IsVBR;
		ppluint32 	ID3v2TagStart;
		ppluint32	FileSize;
		ppluint32	AudioStart;
		ppluint32	AudioEnd;
		ppluint32	AudioSize;
		ppluint32	Samples;
		ppluint8	BytesPerSample;
		ppluint32	Length;		// Length in ms
		ppluint32	Frequency;
		ppluint16	Bitrate;
		ppluint8	Channels;
		ChannelMode	Mode;
};

bool IdentAudioFile(CFileObject &file, AudioInfo &info);

// Basisklasse
class CMP3Decode
{
	private:
		CMP3Decode	*decoder;
	public:
		CMP3Decode();
		virtual ~CMP3Decode();
		int Init(const char *engine=NULL);
		virtual void Clear();
		virtual int LoadFile(const CString &filename);
		virtual int LoadFile(CFileObject &file);
		virtual int GetMpegHeader(PPL_MPEG_HEADER *mpg);
		virtual int Start();
		virtual int Stop();
		virtual int Decode(int num, SAMPLE *left, SAMPLE *right);
};

class CMP3DecodeLibMad : public CMP3Decode
{
//!\brief Klasse zum Dekodieren von MP3-Files
	private:
		PPL_MPEG_HEADER	mpg;
		CMutex	mutex;
		CFileObject	*ff;
		CFile		*file;
		void		*mad;
		ppluint32	FrameCount;
		ppluint8	*readbuffer;
		SAMPLE		*out_left, *out_right;
		int			out_offset, out_samples, out_size;
		bool		isRunning;
		bool		loop;

		int FillDecodeBuffer();

	public:
		CMP3DecodeLibMad();
		~CMP3DecodeLibMad();

		bool DoFilter;

		virtual void Clear();
		virtual int LoadFile(const CString &filename);
		virtual int LoadFile(CFileObject &file);
		virtual int GetMpegHeader(PPL_MPEG_HEADER *mpg);
		virtual int Start();
		virtual int Stop();
		virtual int Decode(int num, SAMPLE *left, SAMPLE *right);
			// Falls der Returnwert < num ist, sind keine weiteren
			// Daten mehr vorhanden und die restlichen Samples sind 0.
};

class CMP3DecodeLibMpg123 : public CMP3Decode
{
	private:
		PPL_MPEG_HEADER	mpg;
		CFileObject	*ff;
		CFile		*file;
		void		*hh;
		ppluint8	*readbuffer;
		ppluint8	*outbuffer;
		size_t		out_offset, out_size;

		bool isInitialized;
		bool		isRunning;
		bool		needInput;

		int FillDecodeBuffer();

	public:
		CMP3DecodeLibMpg123();
		~CMP3DecodeLibMpg123();
		virtual void Clear();
		virtual int LoadFile(const CString &filename);
		virtual int LoadFile(CFileObject &file);
		virtual int GetMpegHeader(PPL_MPEG_HEADER *mpg);
		virtual int Start();
		virtual int Stop();
		virtual int Decode(int num, SAMPLE *left, SAMPLE *right);
};

/*
class CMP3DecodeLibLame : public CMP3Decode
{
	private:
		CMutex		Mutex;
		PPL_MPEG_HEADER	mpg;
		CFileObject	*ff;
		CFile		*file;
		void		*lame;
		ppluint8	*readbuffer;
		SAMPLE		*out_left, *out_right;
		int			out_offset, out_samples, out_size;

		bool isInitialized;
		bool		isRunning;
		bool		needInput;

	public:
		CMP3DecodeLibLame();
		~CMP3DecodeLibLame();
		virtual void Clear();
		virtual int LoadFile(const CString &filename);
		virtual int LoadFile(CFileObject &file);
		virtual int GetMpegHeader(PPL_MPEG_HEADER *mpg);
		virtual int Start();
		virtual int Stop();
		virtual int Decode(int num, SAMPLE *left, SAMPLE *right);
};
*/

class CID3Frame
{
	friend class CID3Tag;
	private:
		char ID[5];
		int Flags;
		size_t Size;
		char *data;
		CID3Frame *nextFrame, *previousFrame;

	public:
		CID3Frame();
		CID3Frame(const char *name);
		~CID3Frame();
};

class CID3Tag
{
	public:
		enum TextEncoding {
			ENC_USASCII,
			ENC_ISO88591,
			ENC_UTF16,
			ENC_UTF8
		};

		enum AudioFormat {
			AF_UNKNOWN=0,
			AF_MP3,
			AF_AIFF
		};
	private:
		CString	Filename;
		CLog	*Log;
		int		Flags;
		size_t	numFrames;
		int		Size;
		AudioFormat	myAudioFormat;
		ppluint32	PaddingSize, PaddingSpace, MaxPaddingSpace;
		CID3Frame	*firstFrame, *lastFrame;
		int		AddFrame(CID3Frame *Frame);
		void	CopyAndDecodeText(CString &s, CID3Frame *frame, int offset) const;
		int	Decode(CID3Frame *frame, int offset, int encoding, CString &target) const;
		int SetTextFrameUtf16(const char *framename, const CString &text);
		int SetTextFrameISO88591(const char *framename, const CString &text);
		int SetTextFrameUtf8(const char *framename, const CString &text);
		CString getNullPaddedString(CID3Frame *frame, size_t offset=0) const;

		AudioFormat identAudioFormat(CFileObject &File);
		ppluint32 findId3Tag(CFileObject &File);

		int SaveMP3();
		int SaveAiff();
		int TrySaveAiffInExistingFile(CFile &o, CBinary &tagV2);
		int CopyAiffToNewFile(CFile &o, CFile &n, CBinary &tagV2);

	public:
		CID3Tag();
		CID3Tag(const CString &File);

		void SetLogfile(CLog *log);

		int Load(const CString &File);
		int Load(CFileObject &File);
		void ClearTags();

		int Save();

		void SetPaddingSize(int bytes);
		void SetPaddingSpace(int bytes);
		void SetMaxPaddingSpace(int bytes);

		int SetArtist(const CString &artist);
		int SetTitle(const CString &title);
		int SetGenre(const CString &genre);
		int SetRemixer(const CString &remixer);
		int SetLabel(const CString &label);
		int SetComment(const CString &comment);
		int SetComment(const CString &description, const CString &comment);
		int SetYear(const CString &year);
		int SetAlbum(const CString &album);
		int SetTrack(const CString &track);
		int SetBPM(const CString &bpm);
		int SetKey(const CString &key);
		int SetEnergyLevel(const CString &energy);
		int SetTextFrame(const char *framename, const CString &text, TextEncoding enc=ENC_UTF16);
		int			RemoveFrame(CID3Frame *frame);
		int			DeleteFrame(CID3Frame *frame);
		CID3Frame	*FindFrame(const char *name) const;
		CID3Frame	*FindFrame(const CString &name) const;
		CID3Frame	*FindUserDefinedText(const char *description) const;
		CID3Frame	*FindUserDefinedText(const CString &description) const;
		void generateId3V2Tag(CBinary &tag);
		void generateId3V1Tag(CBinary &tag);

		void Clear();
		void ListFrames(int hexdump=0) const;
		size_t FrameCount() const;

		CString GetArtist() const;
		CString GetTitle() const;
		CString GetGenre() const;
		CString GetYear() const;
		CString GetComment() const;
		CString GetComment(const CString &description) const;
		CString GetRemixer() const;
		CString GetLabel() const;
		CString GetAlbum() const;
		CString GetTrack() const;
		CString GetBPM() const;
		CString GetKey() const;
		CString GetEnergyLevel() const;
		int GetPrivateData(CBinary &bin, const CString &identifier) const;
		CMemoryReference GetPrivateData(const CString &identifier) const;
		int GetPicture(int type, CBinary &bin) const;
		int SetPicture(int type, const CBinary &bin, const CString &MimeType);
		void RemovePicture(int type);

		unsigned char GetPopularimeter(const CString &email) const;
		bool HasPopularimeter(const CString &email) const;
		void SetPopularimeter(const CString &email, unsigned char rating);
		void GetAllPopularimeters(std::map<CString,unsigned char> &data) const;
		void RemovePopularimeter();
		bool HasPopularimeter() const;
		unsigned char GetPopularimeter() const;

		~CID3Tag();
};


class CIcecast
{
	private:
		void	*shout;
		bool    bconnected;
	public:
		CIcecast();
		~CIcecast();
		void    translateError() const throw();
		CString	getVersion(int *major=NULL, int *minor=NULL, int *patch=NULL) const;
		CString version() const;
		bool	connected();
		void	setConnection(const CString &host, int port, const CString &password);
		CString host() const;
		int		port() const;
		CString	password() const;
		void	setMount(const CString &mount);
		CString mount() const;
		void	setName(const CString &name);
		CString name() const;
		void	setUrl(const CString &url);
		CString url() const;
		void	setGenre(const CString &genre);
		CString genre() const;
		void	setUser(const CString &user);
		CString user() const;
		void	setAgent(const CString &agent);
		CString agent() const;
		void	setDescription(const CString &description);
		CString description() const;
		void	setDumpfile(const CString &file);
		CString dumpfile() const;
		void	setAudioInfo(const CString &name, const CString &value);
		CString audioInfo(const CString &name) const;
		void	setPublic(bool makepublic);
		void	setFormatMP3();
		void	setFormatOGG();
		void	setNonBlocking(bool flag);

		void	connect();
		void	disconnect();

		void	send(const void *buffer, size_t bytes);
		int		delay();
		void	sync();

		void	sendMetadata(const CString &name, const CString &value);
		void	setTitle(const CString &title);

};

class AudioCD
{
	private:
		void *cdio;
		ppl6::CString myDevice;
		size_t first_track_num;
		size_t i_tracks;
		size_t num_audio_tracks;
		size_t audio_frames;
		size_t last_lsn;
		void countAudioTracks();

	public:
		PPLEXCEPTION(DeviceOpenFailed,ppl6::Exception);
		PPLEXCEPTION(DeviceNotOpen,ppl6::Exception);
		PPLEXCEPTION(InvalidAudioTrack,ppl6::Exception);

		class Toc
		{
			public:
				ppluint8 min,sec,frames;
		};

		class Track
		{
			friend class AudioCD;
			private:
				int _track;
				size_t _start, _end;
				bool _hasPreemphasis, _hasCopyPermit, _isAudioTrack;
				int _channels;

			public:
				Track();
				int track() const;
				size_t start() const;
				Toc start_toc() const;
				size_t end() const;
				size_t size() const;
				size_t seconds() const;
				bool hasPreemphasis() const;
				bool hasCopyPermit() const;
				bool isAudioTrack() const;
				int channels() const;
		};

		AudioCD();
		~AudioCD();

		void openDevice(const ppl6::CString &device=ppl6::CString());
		void closeDevice();
		const ppl6::CString &deviceName() const;

		size_t firstTrack() const;
		size_t lastTrack() const;
		size_t numTotalTracks() const;
		size_t numAudioTracks() const;
		size_t totalAudioFrames() const;
		size_t totalAudioLength() const;
		size_t lastLsn() const;

		AudioCD::Track getTrack(int track);
		bool	isAudioTrack(int track);

		static bool isSupported();
		static std::list<ppl6::CString> getDevices();
		static Toc lsn2toc(size_t lsn);
};

class CDDB
{
	public:
		PPLEXCEPTION(QueryFailed, ppl6::Exception);
		PPLEXCEPTION(InvalidDiscId, ppl6::Exception);
		class Track
		{
			public:
				int number;
				int frame_offset;
				int length;
				ppl6::CString Artist;
				ppl6::CString Title;
				ppl6::CString Extra;
		};
		class Disc
		{
			public:
				typedef std::list<Track> TrackList;

				unsigned int	discId;
				ppl6::CString	category;
				ppl6::CString	genre;
				int				length;
				int				year;
				ppl6::CString	Artist;
				ppl6::CString	Title;
				ppl6::CString	Extra;
				TrackList		Tracks;
		};

		typedef std::list<Disc> Matches;

	private:
		ppl6::CCurl curl;
		ppl6::CString QueryPath, Server;
		ppl6::CString ClientName, ClientVersion;
		ppl6::CString UserName, Hostname;
		int port;
		//void storeDisc(Disc &disc, const ppl6::CString &payload);
		ppl6::CString buildUri(const ppl6::CString &cmd);

	public:
		CDDB();
		~CDDB();

		void	setHttpServer(const ppl6::CString &server, int port=80);
		void	setQueryPath(const ppl6::CString &path);
		void	setProxy(const ppl6::CString &hostname, int port);
		void	setClient(const ppl6::CString &name, const ppl6::CString &version);
		void	setUser(const ppl6::CString &username, const ppl6::CString &hostname);

		int		query(ppl6::AudioCD &cd, Matches &list);
		void	getDisc(unsigned int discId, const ppl6::CString &category, Disc &d);

		static bool isSupported();
		static unsigned int calcDiscId(ppl6::AudioCD &cd);

};



} // end of namespace ppl


#endif // _PPL6_INCLUDE_SOUND

