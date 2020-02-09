/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
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
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
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

#ifndef _PPL7_INCLUDE_AUDIO
#define _PPL7_INCLUDE_AUDIO
#ifndef _PPL7_INCLUDE
    #ifdef PPL7LIB
        #include "ppl7.h"
    #else
        #include <ppl7.h>
    #endif
#endif

#ifndef PPL7INET_H_
    #ifdef PPL7LIB
		#include "ppl7-inet.h"
    #else
		#include <ppl7-inet.h>
    #endif
#endif


#include <stdlib.h>
#include <list>


namespace ppl7 {

PPL7EXCEPTION(InvalidGenreException,Exception);
PPL7EXCEPTION(UnsupportedAudioFormatException,Exception);
PPL7EXCEPTION(UnsupportedID3TagVersionException,Exception);
PPL7EXCEPTION(FilenameNotSetException,Exception);
PPL7EXCEPTION(NoID3TagFoundException,Exception);

PPL7EXCEPTION(EncoderException,Exception);
PPL7EXCEPTION(EncoderAlreadyStartedException,EncoderException);
PPL7EXCEPTION(EncoderNotStartedException,EncoderException);
PPL7EXCEPTION(EncoderBufferTooSmallException,EncoderException);
PPL7EXCEPTION(EncoderPsychoAcousticException,EncoderException);
PPL7EXCEPTION(EncoderInitializationException,EncoderException);
PPL7EXCEPTION(EncoderAbortedException,EncoderException);
PPL7EXCEPTION(EncoderAudioFormatMismatchException,EncoderException);

PPL7EXCEPTION(DecoderException,Exception);
PPL7EXCEPTION(DecoderInitializationException,DecoderException);


//!\brief Struktur zum Speichern eines MP3-Headers
typedef struct tagMPEGHeader{
	uint64_t start,end;		// Beginn und Ende der Daten
	uint64_t size,filesize;
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
	uint64_t mslength;			// Laenge in ms
	uint64_t length;			// Laenge in s
	uint64_t samples;
	bool vbr;
} PPL_MPEG_HEADER;

//!\brief Identifizieren von MP3-Dateien und Einlesen der Header
bool IdentMPEG(FileObject &file, PPL_MPEG_HEADER * mpg);

void GetMP3Frame(FileObject &file, PPL_MPEG_HEADER &mpg, ByteArray &buffer);

class AudioInfo
{
	public:
		AudioInfo();

		enum AudioFormat {
			UNKNOWN,
			WAVE,
			AIFF,
			MP3,
			OGG
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
		uint32_t 	ID3v2TagStart;
		uint32_t	FileSize;
		uint32_t	AudioStart;
		uint32_t	AudioEnd;
		uint32_t	AudioSize;
		uint32_t	Samples;
		uint32_t	Length;		// Length in ms
		uint32_t	Frequency;
		uint16_t	Bitrate;
		uint8_t	BitsPerSample;
		uint8_t	BytesPerSample;
		uint8_t	Channels;
		ChannelMode	Mode;
};

bool IdentAudioFile(FileObject &file, AudioInfo &info);
AudioInfo::AudioFormat IdentAudioFile(FileObject &file);
String GetID3GenreName(int id);

class ID3Frame
{
	friend class ID3Tag;
	friend class ID3TagTranscode;
	private:
		String ID;
		int Flags;
		size_t Size;
		char *data;
		ID3Frame *nextFrame, *previousFrame;

	public:
		ID3Frame();
		ID3Frame(const String &name);
		~ID3Frame();

		void setData(const ByteArrayPtr &data);
		void setFlags(int flags);
		void hexDump() const;
		const String& name() const;
		int flags() const;
		size_t size() const;
		void getData(ByteArray &data) const;
		bool hasData() const;
};

class ID3TagTranscode
{
	public:
		static void	copyAndDecodeText(String &s, const ID3Frame *frame, int offset);
		static int	decode(const ID3Frame *frame, int offset, int encoding, String &target);
};

class ID3Tag
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
			AF_AIFF,
			AF_WAVE
		};

		enum PictureType {
			PIC_COVER_FRONT=3,
			PIC_COVER_BACK=4,
		};
	private:
		String	Filename;
		int		Flags;
		size_t	numFrames;
		int		Size;
		AudioFormat	myAudioFormat;
		uint32_t	PaddingSize, PaddingSpace, MaxPaddingSpace;
		ID3Frame	*firstFrame, *lastFrame;
		void setTextFrameUtf16(const String &framename, const String &text);
		void setTextFrameISO88591(const String &framename, const String &text);
		void setTextFrameUtf8(const String &framename, const String &text);
		AudioFormat identAudioFormat(FileObject &File);
		uint64_t findId3Tag(FileObject &File);
		void saveMP3();
		void saveAiff();
		void saveWave();
		bool trySaveAiffInExistingFile(FileObject &o, ByteArrayPtr &tagV2);
		bool trySaveWaveInExistingFile(FileObject &o, ByteArrayPtr &tagV2);
		void copyAiffToNewFile(FileObject &o, FileObject &n, ByteArrayPtr &tagV2);
		void copyWaveToNewFile(FileObject &o, FileObject &n, ByteArrayPtr &tagV2);
		String getNullPaddedString(ID3Frame *frame, size_t offset=0) const;

	public:
		ID3Tag();
		ID3Tag(const String &File);
		~ID3Tag();

		void load(const String &filename);
		void load(FileObject &file);
		void clearTags();
		void clear();
		void save();

		void setPaddingSize(int bytes);
		void setPaddingSpace(int bytes);
		void setMaxPaddingSpace(int bytes);

		void	addFrame(ID3Frame *Frame);
		void	removeFrame(ID3Frame *frame);
		void 	deleteFrame(ID3Frame *frame);
		ID3Frame	*findFrame(const String &name) const;
		ID3Frame	*findUserDefinedText(const String &description) const;
		void listFrames(bool hexdump=false) const;
		size_t frameCount() const;

		void setArtist(const String &artist);
		void setTitle(const String &title);
		void setGenre(const String &genre);
		void setRemixer(const String &remixer);
		void setLabel(const String &label);
		void setComment(const String &comment);
		void setComment(const String &description, const String &comment);
		void setYear(const String &year);
		void setAlbum(const String &album);
		void setTrack(const String &track);
		void setBPM(const String &bpm);
		void setKey(const String &key);
		void setEnergyLevel(const String &energy);
		void setTextFrame(const String &framename, const String &text, TextEncoding enc=ENC_UTF16);
		void setPicture(int type, const ByteArrayPtr &bin, const String &MimeType);
		void setPopularimeter(const String &email, unsigned char rating);

		void generateId3V2Tag(ByteArray &tag) const;
		void generateId3V1Tag(ByteArray &tag) const;


		String getArtist() const;
		String getTitle() const;
		String getGenre() const;
		String getYear() const;
		String getComment() const;
		String getComment(const String &description) const;
		String getRemixer() const;
		String getLabel() const;
		String getAlbum() const;
		String getTrack() const;
		String getBPM() const;
		String getKey() const;
		String getEnergyLevel() const;
		ByteArray getPicture(int type) const;
		bool getPicture(int type, ByteArray &bin) const;
		bool hasPicture(int type) const;
		void removePicture(int type);

		bool getPrivateData(ByteArray &bin, const String &identifier) const;
		ByteArrayPtr getPrivateData(const String &identifier) const;

		unsigned char getPopularimeter(const String &email) const;
		unsigned char GetPopularimeter() const;
		bool hasPopularimeter(const String &email) const;
		void getAllPopularimeters(std::map<String,unsigned char> &data) const;
		void removePopularimeter();
		bool hasPopularimeter() const;
};

class Icecast
{
	private:
		void	*shout;
		bool    bconnected;
	public:
		Icecast();
		~Icecast();
		String	getVersion(int *major=NULL, int *minor=NULL, int *patch=NULL) const;
		String version() const;
		bool	connected();
		void	setConnection(const String &host, int port, const String &password);
		String host() const;
		int		port() const;
		String	password() const;
		void	setMount(const String &mount);
		String mount() const;
		void	setName(const String &name);
		String name() const;
		void	setUrl(const String &url);
		String url() const;
		void	setGenre(const String &genre);
		String genre() const;
		void	setUser(const String &user);
		String user() const;
		void	setAgent(const String &agent);
		String agent() const;
		void	setDescription(const String &description);
		String description() const;
		void	setDumpfile(const String &file);
		String dumpfile() const;
		void	setAudioInfo(const String &name, const String &value);
		String audioInfo(const String &name) const;
		void	setPublic(bool makepublic);
		void	setFormatMP3();
		void	setFormatOGG();
		void	setNonBlocking(bool flag);

		void	connect();
		void	disconnect();

		void	send(const void *buffer, size_t bytes);
		int		delay();
		void	sync();

		void	sendMetadata(const String &name, const String &value);
		void	setTitle(const String &title);

};

class AudioCD
{
	private:
		void *cdio;
		String myDevice;
		size_t first_track_num;
		size_t i_tracks;
		size_t num_audio_tracks;
		size_t audio_frames;
		size_t last_lsn;
		void countAudioTracks();

	public:
		PPL7EXCEPTION(DeviceOpenFailed,Exception);
		PPL7EXCEPTION(DeviceNotOpen,Exception);
		PPL7EXCEPTION(InvalidAudioTrack,Exception);

		class Toc
		{
			public:
				uint8_t min,sec,frames;
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

		void openDevice(const String &device=String());
		void closeDevice();
		const String &deviceName() const;

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
		static std::list<String> getDevices();
		static Toc lsn2toc(size_t lsn);
};


class CDDB
{
	public:
		PPL7EXCEPTION(QueryFailed, Exception);
		PPL7EXCEPTION(InvalidDiscId, Exception);
		class Track
		{
			public:
				int number;
				int frame_offset;
				int length;
				String Artist;
				String Title;
				String Extra;
		};
		class Disc
		{
			public:
				typedef std::list<Track> TrackList;

				unsigned int	discId;
				String		category;
				String		genre;
				int			length;
				int			year;
				String		Artist;
				String		Title;
				String		Extra;
				TrackList	Tracks;
		};

		typedef std::list<Disc> Matches;

	private:
		ppl7::Curl curl;
		String QueryPath, Server;
		String ClientName, ClientVersion;
		String UserName, Hostname;
		int port;
		String buildUri(const String &cmd);

	public:
		CDDB();
		~CDDB();

		void	setHttpServer(const String &server, int port=80);
		void	setQueryPath(const String &path);
		void	setProxy(const String &hostname, int port);
		void	setClient(const String &name, const String &version);
		void	setUser(const String &username, const String &hostname);

		int		query(AudioCD &cd, Matches &list);
		void	getDisc(unsigned int discId, const String &category, Disc &d);

		static bool isSupported();
		static unsigned int calcDiscId(AudioCD &cd);

};

//!\brief Struktur zum Speichern eines WAVE-Headers
typedef struct tagWAVEHeader {
	uint32_t	datastart;
	uint32_t	numSamples;
	uint32_t	bytes;
	uint32_t	frequency;
	uint32_t	bytespersample;
	uint32_t	bytespersecond;
	uint32_t	seconds;
	uint8_t	channels;
	uint8_t	bitdepth;
} WAVEHEADER;

typedef int16_t SAMPLE16;

//!\brief Struktur zum Speichern eines Stereo-Samples
typedef struct tagSTEREOSAMPLE16{
	SAMPLE16 left;
	SAMPLE16 right;
} STEREOSAMPLE16;

class AudioDecoder
{
public:
	virtual ~AudioDecoder() {};
	virtual void open(FileObject &file, const AudioInfo *info=NULL)=0;
	virtual const AudioInfo & getAudioInfo() const=0;
	virtual void getAudioInfo(AudioInfo &info) const=0;
	virtual void seekSample(size_t sample)=0;
	virtual size_t getPosition() const=0;
	virtual size_t getSamples(size_t num, STEREOSAMPLE16 *buffer)=0;
	virtual size_t getSamples(size_t num, float *left, float *right)=0;
	virtual size_t getSamples(size_t num, SAMPLE16 *left, SAMPLE16 *right)=0;
};

AudioDecoder *GetAudioDecoder(FileObject &file);

class AudioDecoder_Wave: public AudioDecoder
{
	private:
		FileObject		*ff;
		AudioInfo		info;
		size_t position;
		size_t samplesize;
		void readWaveHeader(FileObject &file, WAVEHEADER &header);

	public:
		AudioDecoder_Wave();
		~AudioDecoder_Wave();
		void open(FileObject &file, const AudioInfo *info=NULL);
		const AudioInfo & getAudioInfo() const;
		void getAudioInfo(AudioInfo &info) const;
		void seekSample(size_t sample);
		size_t getPosition() const;
		size_t getSamples(size_t num, STEREOSAMPLE16 *buffer);
		size_t getSamples(size_t num, float *left, float *right);
		size_t getSamples(size_t num, SAMPLE16 *left, SAMPLE16 *right);
};

class AudioDecoder_Aiff : public AudioDecoder
{
	private:
		FileObject		*ff;
		AudioInfo		info;
		size_t position;
		size_t samplesize;

	public:
		AudioDecoder_Aiff();
		~AudioDecoder_Aiff();
		void open(FileObject &file, const AudioInfo *info=NULL);
		const AudioInfo & getAudioInfo() const;
		void getAudioInfo(AudioInfo &info) const;
		void seekSample(size_t sample);
		size_t getPosition() const;
		size_t getSamples(size_t num, STEREOSAMPLE16 *buffer);
		size_t getSamples(size_t num, float *left, float *right);
		size_t getSamples(size_t num, SAMPLE16 *left, SAMPLE16 *right);
};

class AudioDecoder_MP3 : public AudioDecoder
{
	private:
		void			*decoder;
		FileObject		*ff;
		uint8_t		*readbuffer;
		uint8_t		*outbuffer;

		AudioInfo		info;
		size_t			position;
		size_t			samplesize;
		size_t			out_offset, out_size;
		bool			isRunning;
		bool			needInput;

		size_t fillDecodeBuffer();

	public:
		AudioDecoder_MP3();
		~AudioDecoder_MP3();
		void open(FileObject &file, const AudioInfo *info=NULL);
		const AudioInfo & getAudioInfo() const;
		void getAudioInfo(AudioInfo &info) const;
		void seekSample(size_t sample);
		size_t getPosition() const;
		size_t getSamples(size_t num, STEREOSAMPLE16 *buffer);
		size_t getSamples(size_t num, float *left, float *right);
		size_t getSamples(size_t num, SAMPLE16 *left, SAMPLE16 *right);
};

class AudioDecoder_Ogg : public AudioDecoder
{
	private:
		FileObject		*ff;
		AudioInfo		info;
		size_t position;
		size_t samplesize;

	public:
		AudioDecoder_Ogg();
		~AudioDecoder_Ogg();
		void open(FileObject &file, const AudioInfo *info=NULL);
		const AudioInfo & getAudioInfo() const;
		void getAudioInfo(AudioInfo &info) const;
		void seekSample(size_t sample);
		size_t getPosition() const;
		size_t getSamples(size_t num, STEREOSAMPLE16 *buffer);
		size_t getSamples(size_t num, float *left, float *right);
		size_t getSamples(size_t num, SAMPLE16 *left, SAMPLE16 *right);
};




class AudioEncoder
{

};

//!\brief Struktur zum Speichern des Fortschritts bei einem MP3-Encode-Vorgang
typedef struct tagSOUNDPROGRESS{
	double	timestarted;
	double	timeend;
	double	now;
	double	past;
	float	percent;
	float	faktor;
	double	eta;
	uint64_t	position;
	uint64_t	bytes;
	uint64_t	position_thisfile;
	uint64_t	bytes_thisfile;
	//PPL_WAVEHEADER *wav;
	//PPL_MPEG_HEADER *mpg;
} PPL_SOUNDPROGRESS;

class AudioEncoder_MP3
{
private:
	class FirstAudio
	{
	public:
		int frequency;
		int channels;
	};
	void * gfp;
	FirstAudio		firstAudio;
	//PPL_SOUNDPROGRESS progress;
	FileObject		*out;
	char			*readcache;
	int				mp3bufsize;
	unsigned char	*mp3buf;

	uint32_t	samples;

	void (*ProgressFunc) (int progress, void *priv);
	void *ProgressFuncPrivData;

	bool have_firstaudio;
	bool started;
	bool bStopEncode;

	void writeEncodedBytes(const char *buffer, size_t bytes);

public:
	AudioEncoder_MP3();
	~AudioEncoder_MP3();
	void setVBR(int min=32, int max=320, int quality=2);
	void setCBR(int kbps=320, int quality=2);
	void setABR(int kbps=192, int quality=2);
	void setQuality(int quality=2);
	void setStereoMode(const AudioInfo::ChannelMode mode=AudioInfo::JOINT_STEREO);
	void setLowpassFreq(int freq=-1);		// -1=Disabled
	void setHighpassFreq(int freq=-1);		// -1=Disabled
	void setProgressFunction(void (*ProgressFunc) (int progress, void *priv), void *priv);
	void startEncode(FileObject &output);
	void writeID3v2Tag(const ID3Tag &tag);
	void writeID3v1Tag(const ID3Tag &tag);
	void encode(AudioDecoder &decoder);
	void finish();
	void stop();

	void startEncode(int frequency, int channels);
	int encodeBuffer(SAMPLE16 *left, SAMPLE16 *right, int num, void *mp3buf, size_t buffersize);
	int encodeBuffer(STEREOSAMPLE16 *buffer, int num, void *mp3buf, size_t buffersize);
	int flushBuffer(void *mp3buf, size_t buffersize);

	static const char *getLameVersion();
	static const char *getPSYVersion();

};


} // end of namespace ppl7


#endif // _PPL7_INCLUDE_SOUND


