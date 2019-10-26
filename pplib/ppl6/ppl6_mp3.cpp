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
 * Copyright (c) 2010, Patrick Fedick <patrick@pfp.de>
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

#include "prolog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_STRINGS_h
	#include <strings.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif

#include "ppl6.h"
#include "ppl6-sound.h"

namespace ppl6 {

static void CheckVBR(CFileObject *file, PPL_MPEG_HEADER *mpg);

static int MPEGVersion []= {3,0,2,1};
static int MPEGLayer []={0,3,2,1};


static int MP3Bitrates[3][3][15] =
{
    {
  {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},
     {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},
        {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}
    },
    {
     {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}
    },
    {
     {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}
    }
};

static unsigned int s_freq[3][4] =
{
    {44100, 48000, 32000, 0},
    {22050, 24000, 16000, 0},
    {11025, 8000, 8000, 0}
};

static const char *mpg_channels[]=
{
	"Stereo","Joint Stereo", "Dual Channel", "Mono"
};


static pplint64 FindNextHeader(CFileObject *file, pplint64 pos,PPL_MPEG_HEADER * mpg);


bool IdentMPEG(CFileObject * file, PPL_MPEG_HEADER * mpg)
/*!\ingroup PPLGroupSound
 */
{
	PPL_MPEG_HEADER mpgheader;
	pplint64 p=0;
	unsigned char *buffer;
	if (!mpg) mpg=&mpgheader;
	memset(mpg,0,sizeof(PPL_MPEG_HEADER));

	mpg->filesize=(ppldd)file->Lof();
	// Beginnt die Datei mit einem ID3-Tag?
	buffer=(unsigned char *)file->Map(0,1024);

	mpg->start=0;

	/*
	 * Ein ID3v2 Tag beginnt mit folgendem Header:
	 * 0x49 0x44 0x33 yy yy xx zz zz zz zz
	 * Die ersten 3 Bytes entsprechen den Buchstaben "ID3"
	 * yy ist immer < 0xff
	 * xx ist das "flags"-Byte und undefiniert
	 * zz ist kleiner als 0x80
	 */

	if (buffer[0]=='I' && buffer[1]=='D' && buffer[2]=='3'
		&& buffer[3]<0xff && buffer[4]<0xff
		&& buffer[6]<0x80 && buffer[7]<0x80 && buffer[8]<0x80 && buffer[9]<0x80) { // Jepp
		// Laenge des Headers berechnen;
		mpg->start=buffer[9] + (buffer[8]<<7) + (buffer[7]<<14) + (buffer[6]<<21) + 10;
		//printf ("   ID3v2-Tag gefunden, Start der Daten bei: %u\n",mpg->start);
	}

	//buffer=(unsigned char *)file->Map(mpg->start,1024);
	p=0;

	// Jetzt den ersten MP3-Header suchen
	p=FindNextHeader(file,mpg->start,mpg);
	if (p<0) return (false);
	if (mpg==&mpgheader) return true;
	mpg->start=(ppldd)p;
	//printf ("   MPEG-Kennung gefunden bei Pos. %u: MPEG %u, Layer %u\n",mpg->start,mpg->version,mpg->layer);

	// Jetzt noch das Ende der Daten suchen
	mpg->end=(ppldd)file->Lof()-1;
	buffer=(unsigned char *)file->Map(mpg->end-127,127);
	if (buffer[0]=='T' && buffer[1]=='A' && buffer[2]=='G') {
		mpg->end=mpg->end-127;
	}
	buffer=(unsigned char *)file->Map(mpg->end-128,128);
	if (buffer[0]=='T' && buffer[1]=='A' && buffer[2]=='G') {
		mpg->end=mpg->end-128;
	}

	mpg->size=mpg->end-mpg->start+1;
	if (mpg->size > mpg->filesize) mpg->size=mpg->filesize;

	if (mpg->layer==1) {
		if (mpg->frequency>0)
			mpg->framesize=12 * mpg->bitrate*1000 / (mpg->frequency + mpg->padding);
	} else {
		if (mpg->frequency>0)
			mpg->framesize=144 * mpg->bitrate*1000 / (mpg->frequency + mpg->padding);
	}
	if (mpg->framesize) mpg->frames=mpg->size/mpg->framesize;
	if (mpg->bitrate>0) {
		mpg->mslength=mpg->size*8/(mpg->bitrate);
		mpg->length=mpg->mslength/1000;
	}
	mpg->samples=(ppldd)((double)mpg->frequency*(double)mpg->mslength/1000.0);
	//mpg->samples=0;
	CheckVBR(file,mpg);

	/*
	printf ("   MPEG-Kennung gefunden bei Pos. %u: MPEG %u, Layer %u\n",mpg->start,mpg->version,mpg->layer);
	printf ("   Bitrate: %ukbit\n",mpg->bitrate);
	printf ("   Frequency: %u Hz, %s\n",mpg->frequency,mpg_channels[mpg->stereo]);
	printf ("   Framesize: %u = %u Frames\n",mpg->framesize, mpg->frames);
	printf ("   Length: %u ms, %u s\n",mpg->mslength,mpg->length);
	printf ("   Samples: %u, Datasize: %u\n",mpg->samples,mpg->size);
	*/
	return (true);
}

void MpegHeader2Array(ppl6::CAssocArray &a, const PPL_MPEG_HEADER *mpg)
{
	a.Clear();
	if (!mpg) return;
	a.Setf("start","%u",mpg->start);
	a.Setf("end","%u",mpg->end);
	a.Setf("size","%u",mpg->size);
	a.Setf("filesize","%u",mpg->filesize);
	a.Setf("version","%i",mpg->version);
	a.Setf("layer","%i",mpg->layer);
	a.Setf("error_protection","%i",mpg->error_protection);
	a.Setf("bitrate_index","%i",mpg->bitrate_index);
	a.Setf("bitrate","%i",mpg->bitrate);
	a.Setf("frequency_index","%i",mpg->frequency_index);
	a.Setf("frequency","%i",mpg->frequency);
	a.Setf("padding","%i",mpg->padding);
	a.Setf("extension","%i",mpg->extension);
	a.Setf("mode","%i",mpg->mode);
	a.Setf("mode_ext","%i",mpg->mode_ext);
	a.Setf("copyright","%i",mpg->copyright);
	a.Setf("original","%i",mpg->original);
	a.Setf("emphasis","%i",mpg->emphasis);
	a.Setf("stereo","%i",mpg->stereo);
	a.Setf("framesize","%i",mpg->framesize);
	a.Setf("frames","%i",mpg->frames);
	a.Setf("mslength","%u",mpg->mslength);
	a.Setf("length","%u",mpg->length);
	a.Setf("samples","%u",mpg->samples);
	a.Setf("vbr","%i",mpg->vbr);
}

void PrintMpegHeader(const PPL_MPEG_HEADER *mpg)
{
	printf ("   MPEG-Kennung gefunden bei Pos. %u: MPEG %u, Layer %u\n",mpg->start,mpg->version,mpg->layer);
	printf ("   Bitrate: %ukbit, %s\n",mpg->bitrate, (mpg->vbr?"VBR":"CBR"));
	printf ("   Frequency: %u Hz, %s\n",mpg->frequency,mpg_channels[mpg->stereo]);
	printf ("   Framesize: %u = %u Frames\n",mpg->framesize, mpg->frames);
	printf ("   Length: %u ms, %u s = %0d:%0d\n",mpg->mslength,mpg->length, mpg->length/60, mpg->length%60);
	printf ("   Samples: %u, Datasize: %u\n",mpg->samples,mpg->size);
	printf ("   Start: %u, End: %u\n",mpg->start,mpg->end);
}


bool IsMPEGHeader(char *header)
/*!\ingroup PPLGroupSound
 */
{
	ppldd temp;
	int version,layer,bitrate_index,frequency_index;
	if (!header) return false;
	temp=((header[0]&255)<<24)+((header[1]&255)<<16)+((header[2]&255)<<8)+(header[3]&255);
	if (((temp>>20)&0xfff)==0xfff) {		// 12 gesetzte Bits nacheinander
		// Ist der Header g�ltig?
		version=MPEGVersion[(header[1] >> 3) & 0x3];
		//layer = 4 - ((header[1] >> 1) & 0x3);
		layer = MPEGLayer[(header[1] >> 1) & 0x3];

		bitrate_index = (header[2] >> 4) & 0x0F;
		frequency_index = (header[2] >> 2) & 0x3;
		if (version>0 && layer>=2 && layer<=3 &&
			bitrate_index!=0 && bitrate_index!=15 &&
			frequency_index!=3) {
				return true;
		}
	}
	return false;
}



static pplint64 FindNextHeader(CFileObject *file, pplint64 pos,PPL_MPEG_HEADER * mpg)
{
	// Befindet sich an dieser Position ein MPEG-Header?
	const char *header,*buffer;
	ppldd p,temp;
	buffer=file->Map(pos,1024);
	if (!buffer) return -1;
	p=0;
	//printf ("FindNextHeader ab Pos: %llu\n",pos+p);
	// Jetzt den ersten MP3-Header suchen
	pplint64 filesize=file->Size();
	while (pos+p+4<filesize) {
		header=buffer+p;
		temp=((header[0]&255)<<24)+((header[1]&255)<<16)+((header[2]&255)<<8)+(header[3]&255);
		//printf ("Pos: %llu, Header: %X\n",pos+p,temp);
		// 11111111111110111011000000000100
		// aaaaaaaaaaabbccdeeeeff
		// a=11 Bits sync, alle gesetzt
		// b=Version: 00=MPEG 2.5, 01=resrved, 10=MPEG2, 11=MPEG1
		// c=Layer:   00=reserved, 01=Layer III, 10=Layer II, 11=Layer I
		// d=protection
		// e=bitrate index, wobei 0000 und 1111 nicht erlaubt sind
		// f=sample rate, wobei 11 nicht erlaubt ist
		if ((temp&0xffe00000)==0xffe00000) {		// 11 gesetzte Bits nacheinander
			//printf ("Match\n");
			// Ist der Header gültig?
			mpg->version=MPEGVersion[(header[1] >> 3 & 0x3)];
			mpg->layer = 4 - ((header[1] >> 1) & 0x3);
			mpg->bitrate_index = (header[2] >> 4) & 0x0F;
			mpg->frequency_index = (header[2] >> 2) & 0x3;
			if (mpg->version>0 && mpg->layer>=2 && mpg->layer<=3 &&
				mpg->bitrate_index!=0 && mpg->bitrate_index!=15 &&
				mpg->frequency_index!=3) {
					//printf ("mpg->start=%u, p=%u\n",mpg->start,p);
					pos+=p;
					//printf ("Position: %u, Version: %d, Layer: %d, Bitrate-Index: %d, FreqIndex: %d, Header: %X\n",
					//	pos, mpg->version, mpg->layer,
					//	mpg->bitrate_index, mpg->frequency_index);
					mpg->start=pos;
					mpg->size=mpg->end-mpg->start+1;
					if (mpg->size > mpg->filesize) mpg->size=mpg->filesize;
					mpg->vbr=false;
					mpg->error_protection = !(header[1] & 0x1);
					mpg->padding = (header[2] >> 1) & 0x01;
					mpg->extension = header[2] & 0x01;
					mpg->mode = (header[3] >> 6) & 0x3;
					mpg->mode_ext = (header[3] >> 4) & 0x03;
					mpg->copyright = (header[3] >> 3) & 0x01;
					mpg->original = (header[3] >> 2) & 0x1;
					mpg->emphasis = (header[3]) & 0x3;
					mpg->stereo = (mpg->mode == MPG_MD_MONO) ? 1 : 2;
					mpg->bitrate=MP3Bitrates[mpg->version-1][mpg->layer - 1][mpg->bitrate_index];
					mpg->frequency=s_freq[mpg->version-1][mpg->frequency_index];
					mpg->framesize=0;
					if (mpg->layer==1 && mpg->frequency>0)
						mpg->framesize=12 * mpg->bitrate*1000 / mpg->frequency + mpg->padding;
					else if (mpg->frequency>0)
						mpg->framesize=144 * mpg->bitrate*1000 / mpg->frequency + mpg->padding;

					if (mpg->framesize) mpg->frames=mpg->size/mpg->framesize;
					if (mpg->bitrate>0) {
						mpg->mslength=mpg->size*8/(mpg->bitrate);
						mpg->length=mpg->mslength/1000;
					}
					mpg->samples=(ppldd)((double)mpg->frequency*(double)mpg->mslength/1000.0);

					return pos;
			}
		}
		p++;
		if (p>1020) {
			pos+=p;
			buffer=file->Map(pos,1024);
			if (!buffer) return -1;
			p=0;
		}
	}
	return -1;
}

static void CheckVBR(CFileObject *file, PPL_MPEG_HEADER *mpg)
{
	// Wir müssen weitere MPG-Header einlesen und vergleichen. Falls die
	// Bitraten variieren, handelt es sich um ein MP3-File mit variabler
	// Bitrate. In diesem Fall muss das mpg->vbr Frag auf true gesetzt,
	// und die Länge manuell berechnet werden (alle Frames einlesen)
	PPL_MPEG_HEADER m;
	memset(&m,0,sizeof(m));
	m.start=mpg->start;
	m.end=mpg->end;
	m.size=mpg->size;
	m.filesize=mpg->filesize;

	ppldd frames=0;
	int lastbitrate;
	pplint64 pos;
	ppld64 avgbitrate=0;
	mpg->vbr=false;

	lastbitrate=mpg->bitrate;
	file->SetMapReadAhead(1024*1024);	// 1MB Read-Cache
	pos=FindNextHeader(file,mpg->start,&m);
	if (pos<0) return;
	while (pos<mpg->end) {
		//printf ("checkvbr: %i\n",frames);
		frames++;
		// Informationen im Header auswerten
		avgbitrate+=m.bitrate;
		if(lastbitrate!=m.bitrate) {
			mpg->vbr=true;
			lastbitrate=m.bitrate;
		}
		if (frames>50 && mpg->vbr==false) break;
		while (1) {
			pos+=m.framesize;
			if (!m.framesize) pos+=4;
			pos=FindNextHeader(file,pos,&m);
			if ((pos<0) || pos>mpg->end) break;
			if (m.layer==mpg->layer && m.version==mpg->version) break;
			else {
				m.end=mpg->end;
				memcpy(mpg,&m,sizeof(PPL_MPEG_HEADER));
				lastbitrate=m.bitrate;
				avgbitrate=m.bitrate;
				frames=0;
			}
		}
		if ((pos<0) || pos>mpg->end) break;
	}
	pos=0;
	if (mpg->vbr==true) {
		mpg->frames=frames;
		if (mpg->layer==1)
			mpg->samples=frames*384;
		else
			mpg->samples=frames*1152;
		mpg->mslength=(ppldd)(((ppld64)(mpg->samples)*1000)/(ppld64)(mpg->frequency));
		mpg->length=mpg->samples/mpg->frequency;
		mpg->bitrate=(ppldd)(avgbitrate/frames);
	}
}

}	// EOF namespace ppl6
