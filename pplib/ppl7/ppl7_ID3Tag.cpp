/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *******************************************************************************
 * Copyright (c) 2017, Patrick Fedick <patrick@pfp.de>
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

#include "prolog_ppl7.h"
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif



#include "ppl7.h"
#include "ppl7-audio.h"


//#define ID3DEBUG

namespace ppl7 {

static const char * genres[] = {
	"Blues",					// 0
	"Classic Rock",
	"Country",
	"Dance",
	"Disco",
	"Funk",
	"Grunge",
	"Hip-Hop",
	"Jazz",
	"Metal",
	"New Age",
	"Oldies",
	"Other",
	"Pop",
	"R&B",
	"Rap",
	"Reggae",
	"Rock",
	"Techno",
	"Industrial",
	"Alternative",
	"Ska",
	"Death Metal",
	"Pranks",
	"Soundtrack",
	"Euro-Techno",
	"Ambient",
	"Trip-Hop",
	"Vocal",
	"Jazz+Funk",				// 29
 	"Fusion",					// 30
	"Trance",					// 31
	"Classical",				// 32
	"Instrumental",				// 33
	"Acid",						// 34
	"House",					// 35
	"Game",
	"Sound Clip",
	"Gospel",
	"Noise",
	"AlternRock",				// 40
	"Bass",
	"Soul",
	"Punk",
	"Space",
	"Meditative",
	"Instrumental Pop",
	"Instrumental Rock",
	"Ethnic",
	"Gothic",
	"Darkwave",
	"Techno-Industrial",
	"Electronic",
	"Pop-Folk",
	"Eurodance",
	"Dream",
	"Southern Rock",
	"Comedy",
	"Cult",
	"Gangsta",
	"Top 40",
	"Christian Rap",
	"Pop/Funk",
	"Jungle",
	"Native American",
	"Cabaret",
	"New Wave",
	"Psychadelic",
	"Rave",
	"Showtunes",
	"Trailer",
	"Lo-Fi",
	"Tribal",
	"Acid Punk",
	"Acid Jazz",
	"Polka",
	"Retro",
	"Musical",
	"Rock & Roll",
	"Hard Rock",
	"Folk",
	"Folk-Rock",
	"National Folk",
	"Swing",
	"Fast Fusion",
	"Bebob",
	"Latin",
	"Revival",
	"Celtic",
	"Bluegrass",
	"Avantgarde",
	"Gothic Rock",
	"Progressive Rock",
	"Psychedelic Rock",
	"Symphonic Rock",
	"Slow Rock",
	"Big Band",
	"Chorus",
	"Easy Listening",
	"Acoustic",
	"Humour",
	"Speech",
	"Chanson",
	"Opera",
	"Chamber Music",
	"Sonata",
	"Symphony",
	"Booty Bass",
	"Primus",
	"Porn Groove",
	"Satire",
	"Slow Jam",
	"Club",
	"Tango",
	"Samba",
	"Folklore",
	"Ballad",
	"Power Ballad",
	"Rhythmic Soul",
	"Freestyle",
	"Duet",
	"Punk Rock",
	"Drum Solo",
	"A capella",
	"Euro-House",
	"Dance Hall",			// 125
	"Goa",
	"Drum’n’Bass",			// 127
	"Club-House",			// 128
	"Hardcore",
	"Terror",
	"Indie",
	"BritPop",
	"Negerpunk",
	"Polsk Punk",
	"Beat",
	"Christian Gangsta",
	"Heavy Metal",
	"Black Metal",
	"Crossover",
	"Contemporary Christian",
	"Christian Rock",
	"Merengue",
	"Salsa",
	"Thrash Metal",
	"Anime",
	"JPop",
	"SynthPop",				// 147
	NULL
};

static size_t strlen16(const char *data)
{
	size_t l=0;
	uint16_t *d=(uint16_t*)data;
	while (d[l]!=0) l++;
	return l;
}

/*!\ingroup PPLGroupSound
 * \brief Genre-Bezeichnung
 *
 * \desc
 * Mit dieser Funktion kann die Bezeichnung eines Genres anhand der Genre-ID ausgelesen werden.
 * Die Werte 0-79 wurden durch die ID3v1-Spezifikation definiert, die Werte 80-147 sind eine
 * Erweiterung von WinAmp (siehe http://www.mpx.cz/mp3manager/tags.htm).
 *
 * @param id Zahl von 0 bis 147, die den Index des Genres darstellt
 * @return Pointer auf einen mit 0-terminierten String mit der englischen Genre-Bezeichnung.
 * Falls die \p id ungültig ist, gibt die Funktion \c NULL zurück, es wird aber kein Fehlercode
 * gesetzt.
 */
String GetID3GenreName(int id)
{
	int max=sizeof(genres) / sizeof(char *)-2;
	if (id<0 || id>max) throw InvalidGenreException("Genre-ID: %d",id);
	return String(genres[id]);
}

/*!\class CID3Frame
 * \ingroup PPLGroupSound
 * \brief Klasse zum Speichern eines einzelnen ID3-Frames
 */

/*!\brief Konstruktor ohne Parameter
 *
 */
ID3Frame::ID3Frame()
{
	Flags=0;
	Size=0;
	data=NULL;
	nextFrame=previousFrame=NULL;
}

/*!\brief Konstruktor mit Framenamen
 *
 * \desc
 * Konstruktor mit Framenamen
 * @param name Der 4-stellige Name des Frames
 */
ID3Frame::ID3Frame(const String &name)
{
	ID.set(name,4);
	Flags=0;
	Size=0;
	data=NULL;
	nextFrame=previousFrame=NULL;
}


/*!\brief Destruktor
 *
 */
ID3Frame::~ID3Frame()
{
	if(data) free(data);
}

void ID3Frame::setData(const ByteArrayPtr &data)
{
	if (this->data) free(this->data);
	this->data=NULL;
	Size=0;
	if (data.size()>0) {
		this->data=(char*)malloc(data.size());
		if (!this->data) throw OutOfMemoryException();
		Size=data.size();
		memcpy(this->data,data.ptr(),Size);
	}
}

void ID3Frame::setFlags(int flags)
{
	this->Flags=flags;
}

void ID3Frame::hexDump() const
{
	if (!data) return;
	HexDump(data,Size);
}

const String& ID3Frame::name() const
{
	return ID;
}

int ID3Frame::flags() const
{
	return Flags;
}

size_t ID3Frame::size() const
{
	return Size;
}

void ID3Frame::getData(ByteArray &data) const
{
	data.copy(this->data,Size);
}

bool ID3Frame::hasData() const
{
	if (!data) return false;
	if (!Size) return false;
	return true;
}


/*!\class ID3TagTranscode
 * \ingroup PPLGroupSound
 * \brief Klasse mit Hilfsfunktionen für ID3Tag
 *
 * \desc
 * Eine Klasse mit Hilfsfunktionen, die intern von ID3Tag verwendet werden.
 */

/*!\brief Text eines Frames dekodieren und in einen String kopieren
 *
 * \desc
 * Mit dieser internen Funktion wird der Text eines Frames ab der Position
 * \p data und einer Länge von \p size Bytes zunächst nach in das
 * lokale Format des Systems konvertiert und dann im String \p s gespeichert.
 *
 * @param[out] s String, in dem der Text gespeichert werden soll
 * @param[in] data Speicherbereich, an dem der Text beginnt
 * @param[in] size Länge des Strings in Bytes
 */
void ID3TagTranscode::copyAndDecodeText(String &s, const ID3Frame *frame, int offset)
{
	if (!frame) throw ppl7::InvalidArgumentsException();
	if (!frame->data) {
		s.clear();
		return;
	}
	int encoding=Peek8(frame->data+offset);
	if (encoding<32) {
		ID3TagTranscode::decode(frame,offset+1,encoding,s);
	} else {
		String from(frame->data+offset,frame->Size-offset);
		Iconv iconv("ISO-8859-1",Iconv::getLocalCharset());
		iconv.transcode(from,s);
	}
}


int ID3TagTranscode::decode(const ID3Frame *frame, int offset, int encoding, String &target)
{
	size_t size=0;
	const char *data=frame->data+offset;
	//printf ("frame->data=%tx, offset=%d\n", (std::ptrdiff_t)frame->data, offset);
	if (encoding==0) {
		//frame->hexDump();
		size=strlen(data);
		if (size+offset>frame->Size) size=frame->Size-offset;
		Iconv iconv("ISO-8859-1",Iconv::getLocalCharset());
		target.set(Transcode(data,size,"ISO-8859-1",Iconv::getLocalCharset()));
		return offset+size+1;
	} else if (encoding==1) {
		size=strlen16(data)*2;
		if (size+offset>frame->Size) size=frame->Size-offset;
		target.set(Transcode(data,size,"UTF-16",Iconv::getLocalCharset()));
		return offset+size+2;
	} else if (encoding==2) {
		size=strlen16(data)*2;
		if (size+offset>frame->Size) size=frame->Size-offset;
		target.set(Transcode(data,size,"UTF-16BE",Iconv::getLocalCharset()));
		return offset+size+2;
	} else if (encoding==3) {
		size=strlen(data);
		if (size+offset>frame->Size) size=frame->Size-offset;
		target.set(data,size);
		return offset+size+1;
	} else if (encoding>31) {
		size=strlen(data);
		if (size+offset>frame->Size) size=frame->Size-offset;
		target.set(Transcode(data,size,"ISO-8859-1",Iconv::getLocalCharset()));
		return offset+size+1;
	}
	return offset+size+1;
}


/*!\class CID3Tag
 * \ingroup PPLGroupSound
 * \brief Klasse zum Parsen und Verändern von ID3 v1 und v2 Tags
 *
 * \desc
 * Mit dieser Klasse können ID3v2-Tags aus Audio-Dateien ausgelesen und geschrieben werden.
 * Beim Schreiben von MP3-Dateien kann zusätzlich noch ein ID3v1-Tag erzeugt werden.
 * \par
 * Dazu muß zunächst die gewünschte Datei mit CID3Tag::Load geöffnet werden. Mit den
 * verschiedenen "Get"-Befehlen können verschiedene vorgegebene Frames aus dem ID3v2-Tag
 * ausgelesen werden (z.B. CID3Tag::GetArtist um den Namen des Interpreten auszulesen),
 * mit "Set" werden sie verändert (z.B. CID3Tag::SetArtist). Mit der Funktionen
 * CID3Tag::FindFrame kann auch jedes beliebige andere Frame ausgelesen werden, jedoch
 * gibt es zur Zeit keine generische Funktion zum Speichern von beliebigen Frames.
 * \par
 * Alle Operationen finden nur im Hauptspeicher statt. Erst durch Aufruf der
 * Funktion CID3Tag::Save werden sie zurück in die Datei geschrieben.
 *
 *
 */

/*!\brief Konstruktor ohne Dateinamen
 *
 * \desc
 * Durch Verwendung dieses Konstruktors wird die Klasse initialisiert, ohne dass
 * eine Datei geladen wird. Dies kann mit CID3Tag::Load später nachgeholt werden.
 *
 */
ID3Tag::ID3Tag()
{
	firstFrame=lastFrame=NULL;
	PaddingSize=1024;
	PaddingSpace=128;
	MaxPaddingSpace=1024;
	numFrames=0;
	Size=0;
	Flags=0;
	myAudioFormat=AF_UNKNOWN;
}

/*!\brief Konstruktor mit Dateinamen
 *
 * \desc
 * Durch Verwendung dieses Konstruktors wird die Klasse initialisiert und die
 * angegebene Date \p filename geladen. Dazu wird die Funktion CID3Tag::Load
 * aufgerufen.
 *
 * \param File Name der Audio-Datei, deren Tags geladen werden soll.
 *
 */
ID3Tag::ID3Tag(const String &File)
{
	firstFrame=lastFrame=NULL;
	PaddingSize=1024;
	PaddingSpace=128;
	MaxPaddingSpace=1024;
	numFrames=0;
	Size=0;
	Flags=0;
	myAudioFormat=AF_UNKNOWN;
	load(File);
}

/*!\brief Destruktor der Klasse
 *
 * \desc
 * Der Destruktor ruft die Funktion CID3Tag::Clear auf, wodurch sämtlicher durch die
 * Klasse reservierter Speicher wieder freigegeben wird.
 *
 */
ID3Tag::~ID3Tag()
{
	clear();
}
/*!\brief To be done
 *
 * \desc
 * Default=1024
 *
 * @param bytes Anzahl Bytes
 */
void ID3Tag::setPaddingSize(int bytes)
{
	PaddingSize=bytes;
}

void ID3Tag::setPaddingSpace(int bytes)
{
	PaddingSpace=bytes;
}

void ID3Tag::setMaxPaddingSpace(int bytes)
{
	MaxPaddingSpace=bytes;
}


/*!\brief Speicher freigeben und Klasse in den Ausgangszustand versetzen
 *
 * \desc
 * Durch Aufruf dieser Funktion wird der komplette durch diese Klasse reservierte
 * Speicher freigegeben und die Klasse in den Ausgangszustand zurückversetzt.
 * Die Funktion wird automatisch vor dem Laden einer Datei mit CID3Tag::Load
 * aufgerufen.
 */
void ID3Tag::clear()
{
	ID3Frame *frame=NULL;
	while ((frame=firstFrame)) {
		firstFrame=frame->nextFrame;
		delete frame;
	}
	firstFrame=lastFrame=NULL;
	Filename.clear();
	numFrames=0;
	myAudioFormat=AF_UNKNOWN;
}

/*!\brief Alle Tags löschen
 *
 * \desc
 * Durch Aufruf dieser Funktion werden alle Frames des geladenen Titels im Speicher gelöscht.
 * Die Datei bleibt unverändert, erst durch Aufruf der Funktion CID3Tag::Save werden
 * auch die ID3-Tags in der Datei aktualisiert und somit gelöscht.
 */
void ID3Tag::clearTags()
{
	ID3Frame *frame=NULL;
	while ((frame=firstFrame)) {
		firstFrame=frame->nextFrame;
		delete frame;
	}
	firstFrame=lastFrame=NULL;
	numFrames=0;
}


ID3Tag::AudioFormat ID3Tag::identAudioFormat(FileObject &File)
{
	if (File.size()<12) return AF_UNKNOWN;
	const char *adr=File.map(0,12);
	if (!adr) return AF_UNKNOWN;
	//HexDump((void*)adr,12);
	//printf ("PeekN32(adr+4)=%d, File.Size=%d\n",ppl6::PeekN32(adr+4), (uint32_t)File.Size() );
	if (PeekN32(adr+4)<File.size()
			&& PeekN32(adr+0)==0x464F524D
			&& PeekN32(adr+8)==0x41494646) return AF_AIFF;
	if (IdentMPEG(File,NULL)) return AF_MP3;
	return AF_UNKNOWN;
}

uint64_t ID3Tag::findId3Tag(FileObject &File)
{
	myAudioFormat=identAudioFormat(File);
	if (myAudioFormat==AF_UNKNOWN) throw ppl7::UnsupportedAudioFormatException();
	else if (myAudioFormat==AF_MP3) return 0;
	else if (myAudioFormat==AF_AIFF) {
		uint64_t p=12;
		while (p<File.size()) {
			const char *adr=File.map(p,8);
			if (!adr) break;
			if (PeekN32(adr)==0x49443320) {
				return p+8;
			}
			p+=PeekN32(adr+4)+8;
		}
	}
	return (uint64_t)-1;
}

/*!\brief ID3-Tags aus einer Audio-Datei laden
 *
 * \desc
 * Mit dieser Funktion werden die ID3-Tags aus der Audio-Datei \p file geladen.
 *
 * @param filename Dateiname
 */
void ID3Tag::load(const String &filename)
{
	File ff;
	ff.open(filename,File::READ);
	load(ff);
	Filename=filename;
}

/*!\brief ID3-Tags aus einem CFileObject laden
 *
 * \desc
 * Mit dieser Funktion werden die ID3-Tags einer bereits geöffneten Audio-Datei,
 * die durch das FileObject \p file repräsentiert wird, in den Hauptspeicher geladen.
 *
 * @param[in] file Geöffnete Datei
 */
void ID3Tag::load(FileObject &file)
{
	clear();
	// ID3V2 Header einlesen (10 Byte)
	uint64_t p=findId3Tag(file);
	if (p==(uint64_t)-1) {
		return;
	}
	const char *adr=file.map(p,10);
	if (strncmp(adr,"ID3",3)!=0) {
		return;
	}
	int version=0;
	if (Peek8(adr+3)==3 && Peek8(adr+4)==0) version=3;			// Version 2.3
	else if (Peek8(adr+3)==4 && Peek8(adr+4)==0) version=4;		// Version 2.4
	if (version==0) {
		throw UnsupportedID3TagVersionException();
	}
	Flags=Peek8(adr+5);
	bool extendedHeader=false;
	if (Flags&128) {		// Unsynchonisation-Flag gesetzt
		throw UnsupportedID3TagVersionException("Unsynchonisation-Flag is not supported");
	}
	if (Flags&64) {			// Extended Header Flag
		extendedHeader=true;
	}
	Size=Peek8(adr+9);
	int s=Peek8(adr+8);
	Size|=s<<7;
	s=Peek8(adr+7);
	Size|=s<<14;
	s=Peek8(adr+6);
	Size|=s<<21;

	p+=10;
	if (extendedHeader) {
		adr=file.map(p,4);
#ifdef ID3DEBUG
		printf ("Extended Header detected:\n");
		HexDump(adr,4);
#endif
		int exHdrSize=Peek8(adr+3);
		exHdrSize|=(Peek8(adr+2))<<7;
		exHdrSize|=(Peek8(adr+1))<<14;
		exHdrSize|=(Peek8(adr+0))<<21;
		p+=exHdrSize;
	}

	#ifdef ID3DEBUG
		printf ("ID3 V2-Tag gefunden, Flags: %i, Länge: %i Bytes\n",Flags,Size);
	#endif
	// Jetzt lesen wir alle Frames in den Speicher
	while (p<file.size()-10) {
		adr=file.map(p,10);
		if (!adr) break;
#ifdef ID3DEBUG
		HexDump((void*)adr,10);
#endif
		if (Peek32(adr)==0) break;
		ID3Frame *Frame=new ID3Frame;
		Frame->ID.set(adr,4);
		Frame->Flags=PeekN16(adr+8);
		if (version==4) {
			Frame->Size=Peek8(adr+7)|(Peek8(adr+6)<<7)|(Peek8(adr+5)<<14)|(Peek8(adr+4)<<21);
		} else {
			Frame->Size=Peek8(adr+7)|(Peek8(adr+6)<<8)|(Peek8(adr+5)<<16)|(Peek8(adr+4)<<24);
		}
		if (!Frame->Size) {
			delete Frame;
			break;
		}
		adr=file.map(p+10,Frame->Size);
		if (adr) {
			//HexDump(adr,Frame->Size);
			Frame->data=(char*)malloc(Frame->Size);
			if (Frame->data) memcpy(Frame->data,adr,Frame->Size);
			addFrame(Frame);
		} else {
			delete Frame;
		}
		p=p+10+Frame->Size;
	}
}

/*!\brief Frame hinzufügen
 *
 * \desc
 * Mit dieser internen Funktion wird ein neues Frame zu den ID3v2-Tags
 * hinzugefügt.
 *
 * @param Frame Pointer auf ein neues CID3Frame
 */
void ID3Tag::addFrame(ID3Frame *Frame)
{
	if (!lastFrame) {		// Erster Eintrag
		firstFrame=lastFrame=Frame;
		Frame->previousFrame=NULL;
		Frame->nextFrame=NULL;
	} else {
		lastFrame->nextFrame=Frame;
		Frame->previousFrame=lastFrame;
		Frame->nextFrame=NULL;
		lastFrame=Frame;
	}
	numFrames++;
	#ifdef ID3DEBUG
		char *content="";
		char *content2="";
		if (strcmp(Frame->ID,"TXXX")==0) {
			content=((char*)Frame->data)+1;
			content2=((char*)Frame->data)+1+strlen(content)+1;
		} else if (strcmp(Frame->ID,"COMM")==0) {
			content=((char*)Frame->data)+4;
			content2=((char*)Frame->data)+4+strlen(content)+1;

		}

		printf ("Adding Frame %s, %i Bytes, Flags: %i: %s=%s\n",Frame->ID, Frame->Size, Frame->Flags, content, content2);
	#endif
}

void ID3Tag::removeFrame(ID3Frame *frame)
{
	if (!frame) return;
	if (frame->previousFrame) frame->previousFrame->nextFrame=frame->nextFrame;
	if (frame->nextFrame) frame->nextFrame->previousFrame=frame->previousFrame;
	if (frame==firstFrame) firstFrame=frame->nextFrame;
	if (frame==lastFrame) lastFrame=frame->previousFrame;
	if (numFrames>0) numFrames--;
	return;
}

void ID3Tag::deleteFrame(ID3Frame *frame)
{
	if (!frame) return;
	removeFrame(frame);
	delete frame;
}

size_t ID3Tag::frameCount() const
{
	return numFrames;
}

/*!\brief Frames auf STDOUT auflisten
 *
 * \desc
 * Mit dieser Funktion werden alle Frames auf STDOUT ausgegeben. Sie ist zu
 * Debug-Zwecken gedacht.
 */
void ID3Tag::listFrames(bool hexdump) const
{
	ID3Frame *frame=firstFrame;
	while (frame) {
		printf ("Frame: %s, Size: %zu, Flags: %u\n",(const char*)frame->ID, frame->Size, frame->Flags);
		if (hexdump) HexDump(frame->data,frame->Size);
		frame=frame->nextFrame;
	}
}

/*!\brief Bestimmtes Frame finden
 *
 * \desc
 * Mit dieser Funktion wird nach einem Frame mit der ID \p name
 * gesucht und ein Pointer darauf zurückgegeben.
 *
 * @param name String mit der 4-stelligen ID des gesuchten Frames.
 * @return Wurde das gewünschte Frame gefunden, gibt die Funktion einen
 * Pointer auf die CID3Frame-Klasse zurück, im Fehlerfall \c NULL.
 */
ID3Frame *ID3Tag::findFrame(const String &name) const
{
	ID3Frame *frame=firstFrame;
	while (frame) {
		if (frame->ID==name) return frame;
		frame=frame->nextFrame;
	}
	return NULL;
}

/*!\brief Benutzerdefinierten Text in einem TXXX-Frame finden
 *
 * \desc
 * Mit dieser Funktion wird nach einem benutzerdefinierten Text
 * in einem TXXX-Frame mit dem Namen \p description gesucht
 * und ein Pointer darauf zurückgegeben.
 *
 * @param name String mit der Description des gesuchten Frames.
 * @return Wurde das gewünschte Frame gefunden, gibt die Funktion einen
 * Pointer auf die CID3Frame-Klasse zurück, im Fehlerfall \c NULL.
 */
ID3Frame *ID3Tag::findUserDefinedText(const String &description) const
{
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(frame->ID=="TXXX") {
			String c;
			ID3TagTranscode::decode(frame,+1,Peek8(frame->data),c);
			if (c.strcmp(description)==0) return frame;
		}
		frame=frame->nextFrame;
	}
	return NULL;
}


void ID3Tag::setTextFrame(const String &framename, const String &text, TextEncoding enc)
{
	if (enc==ENC_USASCII || enc==ENC_ISO88591) setTextFrameISO88591(framename,text);
	else if (enc==ENC_UTF16) setTextFrameUtf16(framename,text);
	else setTextFrameUtf8(framename,text);
}

void ID3Tag::setTextFrameUtf8(const String &framename, const String &text)
{
	bool exists=false;
	ByteArray enc;
	enc=text.toEncoding("UTF-8");
	ID3Frame *frame=findFrame(framename);
	if (frame) {
		exists=true;
	} else {
		frame=new ID3Frame(framename);
		if (!frame) {
			throw ppl7::OutOfMemoryException();
		}
	}
	frame->Flags=0;
	frame->Size=enc.size()+2;
	//printf ("Frame-Size: %i\n",frame->Size);
	if (frame->data) free(frame->data);
	frame->data=(char*)malloc(frame->Size);
	if (!frame->data)  {
		frame->Size=0;
		if (!exists) delete frame;
		throw OutOfMemoryException();
	}

	Poke8(frame->data,3);
	Poke16(frame->data+1+enc.size(),0);	// Terminierendes 0-Byte
	memcpy(frame->data+1,enc.ptr(),enc.size());
	if (!exists) {
		addFrame(frame);
	}
}


static void toUtf16LE(const String &text, ByteArray &enc)
{
	ByteArray buffer;
	//printf ("Converting >>%s<< from %s to UTF-16\n",(const char*)text,GetGlobalEncoding());
	buffer=text.toEncoding("UTF-16LE");
	char *b=(char*)enc.malloc(2+buffer.size());
	if (!b) throw ppl7::OutOfMemoryException();
	b[0]=0xff;
	b[1]=0xfe;
	memcpy (b+2,buffer.ptr(),buffer.size());
}



void ID3Tag::setTextFrameUtf16(const String &framename, const String &text)
{
	bool exists=false;
	ByteArray enc;
	toUtf16LE(text,enc);
	ID3Frame *frame=findFrame(framename);
	if (frame) {
		exists=true;
	} else {
		frame=new ID3Frame(framename);
		if (!frame) throw ppl7::OutOfMemoryException();
	}
	frame->Flags=0;
	frame->Size=enc.size()+3;
	//printf ("Frame-Size: %i\n",frame->Size);
	if (frame->data) free(frame->data);
	frame->data=(char*)malloc(frame->Size);
	if (!frame->data)  {
		frame->Size=0;
		if (!exists) delete frame;
		throw ppl7::OutOfMemoryException();
	}
	Poke8(frame->data,1);
	Poke16(frame->data+1+enc.size(),0);	// Terminierendes 0-Byte
	memcpy(frame->data+1,enc.ptr(),enc.size());
	if (!exists) {
		addFrame(frame);
	}
}

void ID3Tag::setTextFrameISO88591(const String &framename, const String &text)
{
	bool exists=false;
	ByteArray enc;
	enc=text.toEncoding("ISO8859-1");
	ID3Frame *frame=findFrame(framename);
	if (frame) {
		exists=true;
	} else {
		frame=new ID3Frame(framename);
		if (!frame) throw ppl7::OutOfMemoryException();
	}
	frame->Flags=0;
	frame->Size=enc.size()+2;
	//printf ("Frame-Size: %i\n",frame->Size);
	if (frame->data) free(frame->data);
	frame->data=(char*)malloc(frame->Size);
	if (!frame->data)  {
		frame->Size=0;
		if (!exists) delete frame;
		throw ppl7::OutOfMemoryException();
	}

	Poke8(frame->data,0);
	Poke8(frame->data+1+enc.size(),0);	// Terminierendes 0-Byte
	memcpy(frame->data+1,enc.ptr(),enc.size());
	if (!exists) {
		addFrame(frame);
	}
}


/*!\brief Name des Interpreten setzen
 *
 * \desc
 * Mit dieser Funktion wird der Name des Interpreten (Artist) festgelegt.
 * Der Wert wird im Frame "TPE1" gespeichert.
 *
 * @param[in] artist String mit dem Namen des Interpreten
 */
void ID3Tag::setArtist(const String &artist)
{
	setTextFrame("TPE1",artist);
}

/*!\brief Titel des Songs setzen
 *
 * \desc
 * Mit dieser Funktion wird der Titel des Songs festgelegt.
 * Der Wert wird im Frame "TIT2" gespeichert.
 *
 * @param[in] title String mit dem Titel des Songs
 */
void ID3Tag::setTitle(const String &title)
{
	setTextFrame("TIT2",title);
}

/*!\brief Name des Albums setzen
 *
 * \desc
 * Mit dieser Funktion wird der Name des Albums festgelegt, aus dem der Titel stammt.
 * Der Wert wird im Frame "TALB" gespeichert.
 *
 * @param[in] album String mit dem Namen des Albums
 */
void ID3Tag::setAlbum(const String &album)
{
	setTextFrame("TALB",album);
}

/*!\brief Name des Remixers oder des Mixes
 *
 * \desc
 * Mit dieser Funktion wird der Name des Remixers oder des Mixes festgelegt.
 * Der Wert wird im Frame "TPE4" gespeichert, sowie in einem benutzerdefinierten
 * "TXXX"-Frame mit dem Namen "TraktorRemixer". Letzterer wird von Traktor
 * von Native Instruments verwendet.
 *
 * @param[in] remixer String mit dem Namen des Remixers oder des Mixes.
 */
void ID3Tag::setRemixer(const String &remixer)
{
	setTextFrame("TPE4",remixer);
	bool exists=false;
	ByteArray enc, udf;
	String udfstring="TraktorRemixer";
	toUtf16LE(remixer,enc);
	toUtf16LE(udfstring,udf);
	ID3Frame *frame;
	frame=findUserDefinedText("TraktorRemixer");
	if (frame) {
		exists=true;
	} else {
		frame=new ID3Frame("TXXX");
		if (!frame) throw ppl7::OutOfMemoryException();
	}
	frame->Flags=0;
	frame->Size=enc.size()+5+udf.size();
	if (frame->data) free(frame->data);
	frame->data=(char*)malloc(frame->Size);
	if (!frame->data) {
		if(!exists) delete frame;
		throw ppl7::OutOfMemoryException();
	}
	Poke8(frame->data,1);
	memcpy(frame->data+1,udf.ptr(),udf.size());
	Poke16(frame->data+1+udf.size(),0);	// Terminierendes 0-Byte
	memcpy(frame->data+3+udf.size(),enc.ptr(),enc.size());
	Poke16(frame->data+3+udf.size()+enc.size(),0);	// Terminierendes 0-Byte
	if (!exists) {
		addFrame(frame);
	}
}

/*!\brief Name des Plattenlabels setzen
 *
 * \desc
 * Mit dieser Funktion wird der Name des Plattenlabels festgelegt,
 * auf dem der Titel erschienen ist.
 * Der Wert wird im Frame "TPUB" gespeichert.
 *
 * @param[in] label String mit dem Namen des Plattenlabels
 */
void ID3Tag::setLabel(const String &label)
{
	setTextFrame("TPUB",label);
}

/*!\brief Nummer des Tracks setzen
 *
 * \desc
 * Mit dieser Funktion wird die Tracknummer des Titels innerhalb des
 * Datenträgers oder Sets gesetzt. Der String muss eine Ziffer enthalten,
 * die die Position des Tracks innerhalb des Datenträgers oder Sets angibt.
 * Optional kann durch Slash getrennt noch die Gesamtzahl der Tracks folgen,
 * also z.B. "4/9".

 * Der Wert wird im Frame "TRCK" gespeichert.
 *
 * @param[in] track String mit der Nummer des Tracks.
 */
void ID3Tag::setTrack(const String &track)
{
	setTextFrame("TRCK",track);
}

void ID3Tag::setBPM(const String &bpm)
{
	setTextFrame("TBPM",bpm,ENC_USASCII);
}

void ID3Tag::setKey(const String &key)
{
	setTextFrame("TKEY",key,ENC_USASCII);
}

/*!\brief Erscheinungsjahr des Titels setzen
 *
 * \desc
 * Mit dieser Funktion wird das Erscheinungsjahr des Titel festgelegt.
 * Der Wert wird im Frame "TYER" gespeichert.
 *
 * @param[in] year String mit dem Erscheinungsjahr des Titels
 */
void ID3Tag::setYear(const String &year)
{
	setTextFrame("TYER",year);
}

/*!\brief Musikgenre setzen
 *
 * \desc
 * Mit dieser Funktion wird das Musikgenre des Titels festgelegt. Falls
 * das Genre eine offizielle ID hat, wird diese im Frame in Klammern vorangestellt
 * (z.B. <tt>(13)Pop</tt>).
 * Der Wert wird im Frame "TCON" gespeichert.
 *
 * @param[in] genre String mit dem Namen des Musikgenres
 */
void ID3Tag::setGenre(const String &genre)
{
	String gen=genre;
	// Gibt es in der Genre-Liste einen passenden Eintrag?
	int genreid=0;
	while(genres[genreid]!=NULL) {
		if (genre.strcmp((char*)genres[genreid])==0) {
			gen.setf("(%i)",genreid);
			gen+=genre;
			break;
		}
		genreid++;
	}
	setTextFrame("TCON",gen);
}

/*!\brief Kommentar setzen
 *
 * \desc
 * Mit dieser Funktion wird ein Kommentar zum Titel festgelegt.
 * Der Wert wird im Frame "COMM" gespeichert.
 *
 * @param[in] comment String mit dem Kommentar
 */
void ID3Tag::setComment(const String &comment)
{
	setComment("",comment);
}

/*!\brief Kommentar setzen
 *
 * \desc
 * Mit dieser Funktion wird ein Kommentar zum Titel festgelegt.
 * Der Wert wird im Frame "COMM" gespeichert.
 *
 * @param[in] comment String mit dem Kommentar
 * @param[in] shortcomment String mit dem Kommentar
 */
void ID3Tag::setComment(const String &description, const String &comment)
{
	const char *framename="COMM";
	bool exists=false;
	ByteArray enc;
	ByteArray shortenc;
	toUtf16LE(description,shortenc);
	toUtf16LE(comment,enc);
	ID3Frame *frame=findFrame(framename);
	if (frame) {
		exists=true;
		// Es könnte andere COMM-Frames geben, die löschen wir
		ID3Frame *nf,*f=firstFrame;
		while (f) {
			if(f->ID==framename && f!=frame) {
				// Frame löschen
				nf=f->nextFrame;
				deleteFrame(f);
				f=nf;
			} else {
				f=f->nextFrame;
			}
		}

	} else {
		frame=new ID3Frame(framename);
		if (!frame) throw ppl7::OutOfMemoryException();
	}
	frame->Flags=0;
	frame->Size=enc.size()+3+3+shortenc.size()+2;
	//printf ("Frame-Size: %i\n",frame->Size);
	if (frame->data) free(frame->data);
	frame->data=(char*)malloc(frame->Size);
	if (!frame->data)  {
		frame->Size=0;
		if (!exists) delete frame;
		throw ppl7::OutOfMemoryException();
	}
	Poke8(frame->data,1);
	//strncpy(frame->data+1,"eng",3);
	memset(frame->data+1,0,3);
	memcpy(frame->data+4,shortenc.ptr(),shortenc.size());
	Poke16(frame->data+4+shortenc.size(),0);	// Terminierendes 0-Byte
	memcpy(frame->data+6+shortenc.size(),enc.ptr(),enc.size());
	Poke16(frame->data+6+shortenc.size()+enc.size(),0);	// Terminierendes 0-Byte
	if (!exists) {
		addFrame(frame);
	}
}

/*!\brief Tag speichern
 *
 * \desc
 * Bei Aufruf dieser Funktion wird der ID3-Tag in der zuvor mit CID3Tag::Load
 * geladenen Datei gespeichert. Es wird sowohl ein ID3v2- als auch ain ID3v1-Tag
 * geschrieben.
 */
void ID3Tag::save()
{
	if (Filename.isEmpty()) {
		throw FilenameNotSetException();
	}
	if (myAudioFormat==AF_MP3) saveMP3();
	else if (myAudioFormat==AF_AIFF) saveAiff();
	else throw UnsupportedAudioFormatException("FormatId=%d",myAudioFormat);
}

/*!\brief ID3 Version 1 Tag erstellen
 *
 * \desc
 * Diese Funktion erstellt einen 128 Byte langen ID3v1-Tag im Speicher und gibt einen
 * Pointer darauf zurück.
 *
 * @param tag Speicher-Objekt, in dem der Tag gespeichert werden soll
 *
 */
void ID3Tag::generateId3V1Tag(ByteArray &tag) const
{
	if (firstFrame==NULL) {		// Keine Tags vorhanden
		tag.clear();
		return;
	}
	char *buffer=(char*)tag.calloc(128);
	if (!buffer) throw OutOfMemoryException();
	String text;
	strcpy(buffer,"TAG");
	ID3Frame *frame=findFrame("TIT2");
	if (frame) {
		text.clear();
		if (frame->data[0]==0) text.set(frame->data+1,frame->Size-1);
		if (frame->data[0]==1) text=Transcode(frame->data+1,frame->Size-1,"UTF-16","ISO-8859-1");
		if (text.size()) strncpy(buffer+3,text,30);
	}
	frame=findFrame("TPE1");
	if (frame) {
		text.clear();
		if (frame->data[0]==0) text.set(frame->data+1,frame->Size-1);
		if (frame->data[0]==1) text=Transcode(frame->data+1,frame->Size-1,"UTF-16","ISO-8859-1");
		if (text.size()) strncpy(buffer+33,text,30);
	}
	frame=findFrame("TYER");
	if (frame) {
		text.clear();
		if (frame->data[0]==0) text.set(frame->data+1,frame->Size-1);
		if (frame->data[0]==1) text=Transcode(frame->data+1,frame->Size-1,"UTF-16","ISO-8859-1");
		if (text.size()) strncpy(buffer+93,text,4);
	}

	frame=findFrame("TPE4");
	if (frame) {
		text.clear();
		if (frame->data[0]==0) text.set(frame->data+1,frame->Size-1);
		if (frame->data[0]==1) text=Transcode(frame->data+1,frame->Size-1,"UTF-16","ISO-8859-1");
		if (text.size()) strncpy(buffer+97,text,30);
	}
	frame=findFrame("TCON");
	if (frame) {
		text.clear();
		if (frame->data[0]==0) text.set(frame->data+1,frame->Size-1);
		if (frame->data[0]==1) text=Transcode(frame->data+1,frame->Size-1,"UTF-16","ISO-8859-1");
		Array matches;
		if (text.pregMatch("/^\\(([0-9]+)\\).*$/",matches)) {
			Poke8(buffer+127,matches[1].toInt());
		} else {
			Poke8(buffer+127,255);
		}
	}

}

void ID3Tag::generateId3V2Tag(ByteArray &tag) const
{
	if (firstFrame==NULL) {		// Keine Tags vorhanden
		tag.clear();
		return;
	}
	// Calculate required Memory
	size_t size=0;
	size+=10;	// ID3-Header
	ID3Frame *Frame=firstFrame;
	while (Frame) {
		size+=10+Frame->Size;
		Frame=Frame->nextFrame;
	}
	size+=10;	// Empty Frame at End

	// Allocate Buffer
	char *buffer=(char*)tag.malloc(size);
	if (!buffer) throw OutOfMemoryException();

	// Write Tags
	char *header=buffer;
	header[0]='I';
	header[1]='D';
	header[2]='3';
	header[3]=4;
	header[4]=0;
	header[5]=0;
	Frame=firstFrame;
	char *frame=buffer+10;
	while (Frame) {
		//printf ("Save: %s, %u Bytes\n",Frame->ID,Frame->Size);
		frame[0]=Frame->ID[0];
		frame[1]=Frame->ID[1];
		frame[2]=Frame->ID[2];
		frame[3]=Frame->ID[3];
		// Descynchronisieren
		uint32_t s=Frame->Size;
		Poke8(frame+7,(s&127));
		Poke8(frame+6,((s>>7)&127));
		Poke8(frame+5,((s>>14)&127));
		Poke8(frame+4,((s>>21)&127));
		PokeN16(frame+8,Frame->Flags);
		memcpy(frame+10,Frame->data,Frame->Size);
		frame+=10+Frame->Size;
		Frame=Frame->nextFrame;
	}
	memset(frame,0,10);
	size-=10;	// Leeres End-Frame abziehen und Größe in den Header schreiben
	header[9]=size&127;
	header[8]=(size>>7)&127;
	header[7]=(size>>14)&127;
	header[6]=(size>>21)&127;
}

void ID3Tag::saveMP3()
{
	String tmpfile=Filename;
	tmpfile+=".rename.tmp";
	File n;
	File o;
	n.open(&tmpfile,File::WRITE);
	o.open(Filename,File::READWRITE);
	// Wir benötigen exklusiven Zugriff auf das File
	//if (!o.LockExclusive(false)) return 0;		// TODO: Hat unter Windows keine Wirkung

	PPL_MPEG_HEADER mpg;
	if (!IdentMPEG(o, &mpg)) {
		throw UnsupportedAudioFormatException();
	}
	// Tags generieren
	ByteArray tagV1, tagV2;
	generateId3V1Tag(tagV1);
	generateId3V2Tag(tagV2);

	size_t pn=tagV2.size();
	size_t rest=0;
	bool useoldfile=false;
	// Falls der neue Tag noch vor den ersten Frame passt und MaxPaddingSpace nicht
	// überschritten wird, schreiben wir ihn in das Originalfile
	if (pn==0) {
		// Sonderfall, keine Tags vorhanden

	} else if (pn>0 && pn<=mpg.start && mpg.start-pn<MaxPaddingSpace) {
		o.seek(0);
		o.write(tagV2);
		// Mit Nullen auffüllen bis zum Start der Frames
		rest=mpg.start-pn;
		useoldfile=true;
		//printf ("Keine Verschiebung nötig, verwende altes File\n");
	} else {
		n.write(tagV2);
		if (pn<PaddingSize) {
			// Wir füllen mit Nullen auf
			rest=PaddingSize-pn;
		} else {
			// Wir lassen mindestens "PaddingSpace" frei
			rest=PaddingSpace;
		}
	}
	if (rest>0) {
		char *space=(char*)calloc(1024,1);
		if (!space) throw ppl7::OutOfMemoryException();
		try {
			while (rest) {
				uint32_t bytes=rest;
				if (bytes>1024) bytes=1024;
				if (useoldfile)	o.write(space,bytes,pn);
				else n.write(space,bytes,pn);
				pn+=bytes;
				rest-=bytes;
			}
		} catch (...) {
			free(space);
			throw;
		}
		free(space);
	}
	if (useoldfile) {
		// Am Ende noch den v1-Tag reinschreiben
		o.write(tagV1.ptr(),tagV1.size(),mpg.end);
		n.close();
		o.close();
		File::remove(tmpfile);
	} else {
		// Nun kopieren wir die Musikframes
		n.copyFrom(o,(uint64_t)mpg.start,(uint64_t)mpg.size,(uint64_t)pn);
		// Und am Ende noch den v1-Tag
		if (tagV1.size()>0) n.write(tagV1);
		n.close();
		o.close();
		File::remove(Filename);
		File::rename(tmpfile,Filename);
	}
}

bool ID3Tag::trySaveAiffInExistingFile(FileObject &o, ByteArrayPtr &tagV2)
{
	uint32_t qp=12;
	while (qp<o.size()) {
		const char *adr=o.map(qp,32);
		if (!adr) break;
		uint32_t size=PeekN32(adr+4);
		if (PeekN32(adr)==0x49443320) {	// ID3-Chunk gefunden
			//printf ("Found ID3-Chunk with size: %u, Tag is: %u\n",size,tagV2.Size());
			if (size>tagV2.size()) {
				// Reuse old slot
				uint32_t maximumsize=tagV2.size();
				if (maximumsize+PaddingSpace<PaddingSize) maximumsize=PaddingSize;
				else maximumsize+=PaddingSpace;
				maximumsize+=(maximumsize/10);
				if (maximumsize<size) {
					//printf ("Old tag too big, rewriting, maximum=%u\n",maximumsize);
					break;
				}
				//printf ("Reuse former Tag in old file\n");
				o.write(tagV2.ptr(),tagV2.size(),qp+8);
				void *space=calloc(size-tagV2.size(),1);
				if (space) {
					//memset(space,1,size-tagV2.Size());
					o.write(space,size-tagV2.size());
					free(space);
				}
				o.close();
				return true;
			} else {
				break;
			}
		}
		qp+=size+8;
	}
	return false;
}

void ID3Tag::copyAiffToNewFile(FileObject &o, FileObject &n, ByteArrayPtr &tagV2)
{
	uint32_t qp=12;
	uint32_t tp=12;
	uint32_t size;
	uint32_t formsize=4;
	n.copyFrom(o,0,12,0);	// Header kopieren
	while (qp+8<o.size()) {
		const char *adr=o.map(qp,8);
		if (!adr) break;
		size=PeekN32(adr+4);
		if (PeekN32(adr)!=0x49443320) {	// ignore former ID3-chunk
			n.copyFrom(o,qp,size+8,tp);				// append chunk to temporary file
			tp+=size+8;
			formsize+=size+8;
		}
		qp+=size+8;
	}

	// append new ID3-chunk
	size=tagV2.size();
	if (size>0) {
		char buffer[8];
		if (size+PaddingSpace<PaddingSize) size=PaddingSize;
		else size+=PaddingSpace;
		PokeN32(buffer,0x49443320);
		PokeN32(buffer+4,size);
		n.write(&buffer,8,tp);
		n.write(tagV2.ptr(),tagV2.size(),tp+8);
		// Padding
		void *space=calloc(size-tagV2.size(),1);
		if (!space) {
			throw OutOfMemoryException();
		}
		try {
			n.write(space,size-tagV2.size());
		} catch (...) {
			free(space);
			throw;
		}
		free(space);
		formsize+=size+8;
	}
	char buffer[12];
	n.read(buffer,12,0);
	PokeN32(buffer+4,formsize);
	n.write(buffer,12,0);
}

void ID3Tag::saveAiff()
{
	String tmpfile=Filename;
	tmpfile+=".rename.tmp";
	File n;
	File o;

	// Tags generieren
	ByteArray tagV2;
	generateId3V2Tag(tagV2);

	o.open(Filename,File::READWRITE);
	if (tagV2.size()>0) {
		try {
			if (trySaveAiffInExistingFile(o,tagV2)) return;
		} catch (...) {

		}
	}
	// create temporary file for new tag
	n.touch(tmpfile);
	n.open(tmpfile,File::READWRITE);
	try {
		copyAiffToNewFile(o,n,tagV2);
	} catch (...) {
		n.close();
		o.close();
		File::remove(tmpfile);
		throw;
	}
	o.close();
	n.close();
	File::remove(Filename);
	File::rename(tmpfile,Filename);
}

/*!\brief Name des Interpreten auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Name des Interpreten aus dem Frame "TPE1"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Namen des Interpreten zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getArtist() const
{
	String r;
	ID3Frame *frame=findFrame("TPE1");
	if (frame) {
		//frame->hexDump();
		ID3TagTranscode::copyAndDecodeText(r,frame,0);
	}
	return r;
}

/*!\brief Name des Titels auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Name des Titels aus dem Frame "TIT2"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Titel des Songs zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getTitle() const
{
	String r;
	ID3Frame *frame=findFrame("TIT2");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}

/*!\brief Genre auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Name des Musikgenres aus dem Frame "TCON"
 * ausgelesen.
 *
 * \since Ab Version 6.4.18 liefert die Funktion nicht mehr den ungeparsten Inhalt
 * des TCON-Tags zurück (z.B. "(1)Classic Rock", sondern nurnoch den String, der das
 * Genre spezifiziert (z.B. "Classic Rock").
 *
 * @return Bei Erfolg wird ein String mit dem Namen des Genres zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getGenre() const
{
	String r;
	String Tmp;
	ID3Frame *frame=findFrame("TCON");
	if (frame) ID3TagTranscode::copyAndDecodeText(Tmp,frame,0);

	// Manchmal beginnt das Genre mit einer in Klammern gesetzten Ziffer.
	// Diese entspricht der GenreId des ID3v1-Tags
	Array matches;
	if (Tmp.pregMatch("/^\\(([0-9]+)\\)(.*)$/",matches)) {
		// Wir bevorzugen den Text nach der Klammer
		r=matches[2];
		r.trim();
		if (r.isEmpty()) {
			// Nur wenn er leer ist, verwenden wir die GenreId
			r=matches[1];
			r.trim();
			r=GetID3GenreName(r.toInt());
		}
	} else if (Tmp.pregMatch("/^([0-9]+)$/",matches)) {
		// Manchmal haben wir aber auch nur eine Genre-Ziffer
		r=matches[1];
		r.trim();
		r=GetID3GenreName(r.toInt());
	} else {
		r=Tmp;
	}
	return r;
}

/*!\brief Kommentar auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Kommentar des Titels aus dem Frame "COMM"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Kommentar zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getComment() const
{
	return getComment("");
}

/*!\brief Kommentar auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Kommentar des Titels aus dem Frame "COMM"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Kommentar zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getComment(const String &description) const
{
	String r, desc, best;
	ID3Frame *frame=firstFrame;
	ID3Frame *bestframe=NULL;
	size_t size;
	while (frame) {
		if(frame->ID=="COMM") {
			// Wir haben ein Comment-Feld
			int encoding=Peek8(frame->data);
			if (encoding<4) {
				if (encoding==0 || encoding==3) {
					size=strlen(frame->data+4);
					ID3TagTranscode::decode(frame,4,encoding,desc);
					ID3TagTranscode::decode(frame,5+size,encoding,r);
				} else {
					size=strlen16(frame->data+4)*2;
					ID3TagTranscode::decode(frame,4,encoding,desc);
					ID3TagTranscode::decode(frame,6+size,encoding,r);
				}
				//printf ("Found COMM-Frame with desc=%s => %s\n",(const char *)desc,(const char*)r);
				if (description.size()>0 && description==desc) return r;
				if (!bestframe) bestframe=frame;
				if (description.size()==0 && desc.size()==0) best=r;
			}
		}
		frame=frame->nextFrame;
	}
	if (bestframe) {
		return best;
	} else {
		r.clear();
	}
	return r;
}

/*!\brief Name des Remixers oder des Mixes auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Names des Remixers oder des Mixes aus dem Frame "TPE4"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Namen des Remixers oder Mixes zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getRemixer() const
{
	String r;
	ID3Frame *frame=findFrame("TPE4");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}

/*!\brief Erscheinungsjahr auslesen
 *
 * \desc
 * Mit dieser Funktion wird das Erscheinungsjahr des Titels aus dem Frame "TYER"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Erscheinungsjahr des Titels zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getYear() const
{
	String r;
	ID3Frame *frame=findFrame("TYER");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}

/*!\brief Name des Plattenlabels auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Name des Plattenlabels aus dem Frame "TPUB"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Namen des Plattenlabels zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getLabel() const
{
	String r;
	ID3Frame *frame=findFrame("TPUB");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}

/*!\brief Name des Albums auslesen
 *
 * \desc
 * Mit dieser Funktion wird der Names des Albums aus dem Frame "TALB"
 * ausgelesen.
 *
 * @return Bei Erfolg wird ein String mit dem Namen des Albums zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getAlbum() const
{
	String r;
	ID3Frame *frame=findFrame("TALB");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}

/*!\brief Track-Nummer auslesen
 *
 * \desc
 * Mit dieser Funktion wird die Tracknummer aus dem Frame "TRCK"
 * ausgelesen. Der String enthält eine Ziffer, die die Position des Tracks
 * innerhalb des Datenträgers oder Sets angibt. Optional kann durch Slash getrennt
 * noch die Gesamtzahl der Tracks folgen, also z.B. "4/9".
 *
 * @return Bei Erfolg wird ein String mit der Tracknummer zurückgegeben,
 * im Fehlerfall ein leerer String.
 */
String ID3Tag::getTrack() const
{
	String r;
	ID3Frame *frame=findFrame("TRCK");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}

String ID3Tag::getBPM() const
{
	String r;
	ID3Frame *frame=findFrame("TBPM");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}

String ID3Tag::getKey() const
{
	String r;
	ID3Frame *frame=findFrame("TKEY");
	if (frame) ID3TagTranscode::copyAndDecodeText(r,frame,0);
	return r;
}


bool ID3Tag::getPicture(int type, ByteArray &bin) const
{
	String name="APIC";
	ID3Frame *frame=firstFrame;
	bin.clear();
	while (frame) {
		if(frame->ID==name) {
			// Wir haben ein Picture
			String MimeType;
			int encoding=Peek8(frame->data);
			int offset=ID3TagTranscode::decode(frame,1,0,MimeType);
			//printf ("Offset: %i, Type=%i, encoding=%i\n",offset, (int)Peek8(frame->data+offset),encoding);
			if ((int)Peek8(frame->data+offset)==type) {
				String Description;
				offset=ID3TagTranscode::decode(frame,offset+1,encoding,Description);
				//printf ("Mimetype: >>>%s<<<, offset: %i\n",(const char*)MimeType,offset);
				bin.copy(frame->data+offset,frame->Size-offset);
				return true;
			}
			return false;
		}
		frame=frame->nextFrame;
	}
	return false;
}

/*!\brief Prüft, ob ein Bild eines bestimmten Typs in den Tags enthalten ist
 *
 * \desc
 * Mit dieser Funktion kann geprüft werden, ob ein Bild vom Typ \p type in den Tags
 * vorhanden ist.
 *
 * @param type Integer mit dem gewünschten Bild-Typ
 * @return true oder false
 */
bool ID3Tag::hasPicture(int type) const
{
	String name="APIC";
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(frame->ID==name) {
			// Wir haben ein Picture
			String MimeType;
			//int encoding=Peek8(frame->data);
			int offset=ID3TagTranscode::decode(frame,1,0,MimeType);
			//printf ("Offset: %i, Type=%i, encoding=%i\n",offset, (int)Peek8(frame->data+offset),encoding);
			if ((int)Peek8(frame->data+offset)==type) {
				return true;
			}
			return false;
		}
		frame=frame->nextFrame;
	}
	return false;
}

bool ID3Tag::getPrivateData(ByteArray &bin, const String &identifier) const
{
	ByteArrayPtr ref=getPrivateData(identifier);
	if (ref.isNull()) return false;
	bin.copy(ref);
	return true;
}

ByteArrayPtr ID3Tag::getPrivateData(const String &identifier) const
{
	String name="PRIV";
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(name==frame->ID) {
			// Wir haben ein PRIV-Frame
			if (identifier.strcmp(frame->data)==0) {
				return ByteArrayPtr(frame->data+identifier.size()+1,frame->Size-identifier.size()-1);
			}
		}
		frame=frame->nextFrame;
	}
	return ByteArrayPtr();
}

String ID3Tag::getEnergyLevel() const
{
	String energy;
	String name="TXXX";
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(name==frame->ID) {
			// Wir haben ein TXXX-Frame
			int encoding=Peek8(frame->data);
			String identifier;
			int offset=ID3TagTranscode::decode(frame,1,encoding,identifier);
			if (identifier=="EnergyLevel") {
				ID3TagTranscode::decode(frame,offset,encoding,energy);
				return energy;
			}
		}
		frame=frame->nextFrame;
	}
	return energy;
}

void ID3Tag::setEnergyLevel(const String &energy)
{
	String name="TXXX";
	ID3Frame *frame=firstFrame;
	bool exists=false;
	while (frame) {
		if(name==frame->ID) {
			// Wir haben ein TXXX-Frame
			int encoding=Peek8(frame->data);
			String identifier;
			ID3TagTranscode::decode(frame,1,encoding,identifier);
			if (identifier=="EnergyLevel") {
				exists=true;
				break;
			}
		}
		frame=frame->nextFrame;
	}
	// Bisher kein Frame vorhanden, wir legen ein neues an
	if (!frame)	{
		frame=new ID3Frame("TXXX");
		if (!frame) {
			throw ppl7::OutOfMemoryException();
		}
	}
	frame->Flags=0;
	frame->Size=14+energy.size();
	//printf ("Frame-Size: %i\n",frame->Size);
	if (frame->data) free(frame->data);
	frame->data=(char*)malloc(frame->Size);
	if (!frame->data)  {
		frame->Size=0;
		if (!exists) delete frame;
		throw ppl7::OutOfMemoryException();
	}
	Poke8(frame->data,3);
	strcpy(frame->data+1,"EnergyLevel");
	Poke8(frame->data+12,0);
	strncpy(frame->data+13,energy.c_str(),energy.size());
	Poke8(frame->data+13+energy.size(),0);
	if (!exists) addFrame(frame);
}


void ID3Tag::setPicture(int type, const ByteArrayPtr &bin, const String &MimeType)
{
	bool exists=false;
	String name="APIC";
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(name==frame->ID) {
			// Wir haben ein Picture
			String MimeType;
			//int encoding=Peek8(frame->data);
			int offset=ID3TagTranscode::decode(frame,1,0,MimeType);
			if ((int)Peek8(frame->data+offset)==type) {
				exists=true;
				delete (frame->data);
				frame->data=NULL;
				frame->Size=0;
				break;
			}
		}
		frame=frame->nextFrame;
	}
	if (!frame) {
		frame=new ID3Frame(name);
		if (!frame) throw ppl7::OutOfMemoryException();
	}
	frame->Flags=0;
	frame->data=(char*)malloc(bin.size()+MimeType.size()+4);
	if (!frame->data) {
		if (!exists) {
			delete frame;
			throw ppl7::OutOfMemoryException();
		}
	}
	frame->Size=bin.size()+MimeType.size()+4;
	Poke8(frame->data,0);
	strcpy(frame->data+1,MimeType.c_str());
	Poke8(frame->data+2+MimeType.size(),type);
	Poke8(frame->data+3+MimeType.size(),0);
	memcpy(frame->data+4+MimeType.size(),bin.ptr(),bin.size());
	if (!exists) {
		addFrame(frame);
	}
}

void ID3Tag::removePicture(int type)
{
	String name="APIC";
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(name==frame->ID) {
			// Wir haben ein Picture
			String MimeType;
			//int encoding=Peek8(frame->data);
			int offset=ID3TagTranscode::decode(frame,1,0,MimeType);
			if ((int)Peek8(frame->data+offset)==type) {
				free (frame->data);
				frame->data=NULL;
				frame->Size=0;
				deleteFrame(frame);
				return;
			}
		}
		frame=frame->nextFrame;
	}
}

String ID3Tag::getNullPaddedString(ID3Frame *frame, size_t offset) const
{
	size_t p=offset;
	while (p<frame->Size && frame->data[p]!=0) {
		p++;
	}
	String data;
	if (frame->data[p]==0) {
		data.set(frame->data,p);
		return data;
	}
	return data;
}

void ID3Tag::getAllPopularimeters(std::map<String,unsigned char> &data) const
{
	data.clear();
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(strcmp(frame->ID,"POPM")==0) {
			String email=getNullPaddedString(frame,0);
			if (email.notEmpty()) {
				unsigned char rating=ppl7::Peek8(frame->data+email.size()+1);
				data.insert(std::pair<String,unsigned char>(email,rating));
			}
		}
		frame=frame->nextFrame;
	}
}

bool ID3Tag::hasPopularimeter(const String &email) const
{
	if (email.isEmpty()) return false;
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(strcmp(frame->ID,"POPM")==0) {
			String existingemail=getNullPaddedString(frame,0);
			if (existingemail==email) {
				return true;
			}
		}
		frame=frame->nextFrame;
	}
	return false;
}

unsigned char ID3Tag::getPopularimeter(const String &email) const
{
	if (email.isEmpty()) return 0;
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(strcmp(frame->ID,"POPM")==0) {
			String existingemail=getNullPaddedString(frame,0);
			if (existingemail==email) {
				return ppl7::Peek8(frame->data+existingemail.size()+1);
			}
		}
		frame=frame->nextFrame;
	}
	return 0;
}

void ID3Tag::setPopularimeter(const String &email, unsigned char rating)
{
	if (email.isEmpty()) return;
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(strcmp(frame->ID,"POPM")==0) {
			String existingemail=getNullPaddedString(frame,0);
			if (existingemail==email) {
				if (rating==0) {
					deleteFrame(frame);
					return;
				}
				ppl7::Poke8(frame->data+existingemail.size()+1, rating);
				return;
			}
		}
		frame=frame->nextFrame;
	}
	frame=new ID3Frame("POPM");
	frame->Size=email.size()+6;
	frame->data=(char*)calloc(frame->Size,1);
	if (!frame->data) {
		delete frame;
		throw ppl7::OutOfMemoryException();
	}
	memcpy(frame->data,email.getPtr(),email.size());
	frame->data[email.size()+1]=rating;
	addFrame(frame);
}

void ID3Tag::removePopularimeter()
{
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(strcmp(frame->ID,"POPM")==0) {
			deleteFrame(frame);
			frame=firstFrame;
		} else {
			frame=frame->nextFrame;
		}
	}
}

bool ID3Tag::hasPopularimeter() const
{
	ID3Frame *frame=firstFrame;
	while (frame) {
		if(strcmp(frame->ID,"POPM")==0) {
			return true;
		}
		frame=frame->nextFrame;
	}
	return false;
}

}	// EOF namespace ppl7
