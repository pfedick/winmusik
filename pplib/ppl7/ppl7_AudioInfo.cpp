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
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "ppl7.h"
#include "ppl7-audio.h"

namespace ppl7 {


AudioDecoder *GetAudioDecoder(FileObject &file)
{
	AudioInfo info;
	if (!IdentAudioFile(file, info)) return NULL;
	AudioDecoder *decoder=NULL;
	if (info.Format==AudioInfo::AIFF) decoder=(AudioDecoder *)new AudioDecoder_Aiff();
	if (!decoder) return NULL;
	decoder->open(file,&info);
	return decoder;
}


AudioInfo::AudioInfo()
{
	Format=WAVE;
	HaveID3v2Tag=false;
	IsVBR=false;
	ID3v2TagStart=0;
	FileSize=0;
	AudioStart=0;
	AudioEnd=0;
	AudioSize=0;
	Samples=0;
	BytesPerSample=0;
	BitsPerSample=0;
	Length=0;
	Frequency=0;
	Bitrate=0;
	Channels=0;
	Mode=STEREO;
}

static bool IdentAIFF(FileObject &file, AudioInfo &info)
{
	info.Format=AudioInfo::AIFF;
	info.IsVBR=false;
	info.FileSize=file.size();
	info.Length=0;	// will be calculated later

	const char *adr;
	uint32_t p=12;
	uint32_t size;

	//printf ("Checking AIFF, p=12, file.Size=%u\n",file.Size());

	while (p+8<file.size()) {
		adr=file.map(p,8);
		if (!adr) break;
		size=PeekN32(adr+4);
		String ChunkName;
		ChunkName.set(adr,4);
		//printf ("Frame: %s, Size: %u\n",(const char*)ChunkName,size);
		if (PeekN32(adr)==0x434f4d4d) {			// COMM-Chunk gefunden
			//printf ("COMM-Chunk gefunden\n");
			//ppl6::HexDump((void*)(adr+8),size);
			adr=file.map(p,32);
			info.Channels=PeekN16(adr+8);
			if (info.Channels==1) info.Mode=AudioInfo::MONO;
			else if (info.Channels==2) info.Mode=AudioInfo::STEREO;
			else return false;	// Not supported
			info.Samples=PeekN32(adr+10);
			info.BytesPerSample=(PeekN16(adr+14)*info.Channels)/8;
			info.BitsPerSample=PeekN16(adr+14);
			long double frequency=44100.0;	// 80 bit IEEE Standard 754 floating point number
			//ppl6::HexDump(&frequency,sizeof(long double));
			frequency=0.0;
			char *f=(char*)&frequency;	// Umwandlung von BE nach LE
			f[0]=adr[25];
			f[1]=adr[24];
			f[2]=adr[23];
			f[3]=adr[22];
			f[4]=adr[21];
			f[5]=adr[20];
			f[6]=adr[19];
			f[7]=adr[18];
			f[8]=adr[17];
			f[9]=adr[16];
			//ppl6::HexDump(f,sizeof(long double));
			info.Frequency=(uint32_t)frequency;
			if (info.Frequency>0 && info.Samples>0) info.Length=(uint32_t)((uint64_t)info.Samples*1000/info.Frequency);
			info.Bitrate=((uint64_t)info.Frequency*(uint64_t)info.BytesPerSample*8/1000);

		} else if (PeekN32(adr)==0x53534e44) {	// SSND-Chunk gefunden
			adr=file.map(p,16);
			if (PeekN32(adr+8)!=0) return false;
			if (PeekN32(adr+12)!=0) return false;
			info.AudioStart=p+16;
			info.AudioSize=size-8;
			info.AudioEnd=info.AudioStart+info.AudioSize-1;
		} else if (PeekN32(adr)==0x49443320) {	// ID3-Chunk gefunden
			info.HaveID3v2Tag=true;
			info.ID3v2TagStart=p+8;
		}
		p+=size+8;
	}

	return true;
}

static bool IdentWave(FileObject &file, AudioInfo &info)
{
	info.Format=AudioInfo::WAVE;
	info.IsVBR=false;
	info.FileSize=file.size();
	info.Length=0;	// will be calculated later
	const char *buffer=file.map(0,64);
	const char *fmt=buffer+12;

	uint32_t fmtchunklen=Peek32(fmt+4);
	info.Channels=Peek16(fmt+0x0a);
	if (info.Channels==1) info.Mode=AudioInfo::MONO;
	else if (info.Channels==2) info.Mode=AudioInfo::STEREO;
	else return false;	// Not supported

	info.Frequency=Peek32(fmt+0x0c);
	int bitdepth=Peek16(fmt+0x16);
	// we support only 8bit and 16bit
	if (bitdepth!=8 && bitdepth!=16) return false;

	buffer=file.map(fmtchunklen+20,8);				// read datachunk
	if (!buffer) return false;						// File too small
	if (strncmp(buffer,(const char*)"data",4)!=0) return false;	// no data chunk

	info.Samples=0;
	info.BitsPerSample=bitdepth;
	info.BytesPerSample=(bitdepth/8)*info.Channels;
	info.AudioStart=fmtchunklen+28;
	info.AudioSize=Peek32(buffer+4);
	info.AudioEnd=info.AudioStart+info.AudioSize-1;
	info.Samples=info.AudioSize/info.BytesPerSample;
	if (info.Frequency>0 && info.Samples>0) info.Length=(uint32_t)((uint64_t)info.Samples*1000/info.Frequency);
	info.Bitrate=((uint64_t)info.Frequency*(uint64_t)info.BytesPerSample*8/1000);
	info.HaveID3v2Tag=false;
	info.ID3v2TagStart=0;
	return true;
}



static bool IdentMP3(FileObject &file, AudioInfo &info, PPL_MPEG_HEADER &mp3)
{
	info.Format=AudioInfo::MP3;
	info.IsVBR=mp3.vbr;
	info.FileSize=file.size();
	info.AudioStart=mp3.start;
	info.AudioEnd=mp3.end;
	info.AudioSize=info.AudioEnd-info.AudioStart+1;
	info.Samples=mp3.samples;
	info.Channels=mp3.stereo;
	info.Bitrate=mp3.bitrate;
	info.Frequency=mp3.frequency;
	info.BytesPerSample=mp3.stereo*2;
	info.BitsPerSample=16;
	info.Length=mp3.mslength;
	switch (mp3.mode) {
		case 0: info.Mode=AudioInfo::STEREO; break;
		case 1: info.Mode=AudioInfo::JOINT_STEREO; break;
		case 2: info.Mode=AudioInfo::DUAL_CHANNEL; break;
		case 3: info.Mode=AudioInfo::MONO; break;
		default: return false;
	}

	const char *adr=file.map(0,10);
	info.HaveID3v2Tag=false;
	info.ID3v2TagStart=0;
	if (strncmp(adr,"ID3",3)==0) {
		if (Peek8(adr+3)==3 && Peek8(adr+4)==0) info.HaveID3v2Tag=true;			// Version 2.3
		else if (Peek8(adr+3)==4 && Peek8(adr+4)==0) info.HaveID3v2Tag=true;	// Version 2.4
	}
	return true;
}

bool IdentAudioFile(FileObject &file, AudioInfo &info)
{
	const char *adr=file.map(0,24);
	if (!adr) return false;
	// AIFF
	if (PeekN32(adr+4)<file.size()
				&& PeekN32(adr+0)==0x464F524D
				&& PeekN32(adr+8)==0x41494646) return IdentAIFF(file,info);
	// RIFF / WAV
	if (strncmp(adr,"RIFF",4)==0) {
		if (strncmp(adr+8,"WAVE",4)==0) {
			if (strncmp(adr+12,"fmt ",4)==0) {
				if (Peek16(adr+20)==1) return IdentWave(file,info);
			}
		}
	}

	PPL_MPEG_HEADER mp3;
	if (IdentMPEG(file,&mp3)) return IdentMP3(file,info,mp3);
	return false;
}

AudioInfo::AudioFormat IdentAudioFile(FileObject &file)
{
	const char *adr=file.map(0,24);
	if (!adr) return AudioInfo::UNKNOWN;
	// AIFF
	if (PeekN32(adr+4)<file.size()
				&& PeekN32(adr+0)==0x464F524D
				&& PeekN32(adr+8)==0x41494646) return AudioInfo::AIFF;
	// RIFF / WAV
	if (strncmp(adr,"RIFF",4)==0) {
		if (strncmp(adr+8,"WAVE",4)==0) {
			if (strncmp(adr+12,"fmt ",4)==0) {
				if (Peek16(adr+20)==1) return AudioInfo::WAVE;
			}
		}
	}

	PPL_MPEG_HEADER mp3;
	if (IdentMPEG(file,&mp3)) return AudioInfo::MP3;
	return AudioInfo::UNKNOWN;
}

} // EOF namespace ppl7
