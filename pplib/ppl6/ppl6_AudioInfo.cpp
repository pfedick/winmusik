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
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "ppl6.h"
#include "ppl6-sound.h"

namespace ppl6 {

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
	Length=0;
	Frequency=0;
	Bitrate=0;
	Channels=0;
	Mode=STEREO;
}

static bool IdentAIFF(CFileObject &file, AudioInfo &info)
{
	info.Format=AudioInfo::AIFF;
	info.IsVBR=false;
	info.FileSize=file.Size();
	info.Length=0;	// will be calculated later

	const char *adr;
	ppluint32 p=12;
	ppluint32 size;

	//printf ("Checking AIFF, p=12, file.Size=%u\n",file.Size());

	while (p<file.Size()) {
		adr=file.Map(p,32);
		if (!adr) break;
		size=ppl6::PeekN32(adr+4);
		ppl6::CString ChunkName;
		ChunkName.Set(adr,4);
		//printf ("Frame: %s, Size: %u\n",(const char*)ChunkName,size);
		if (ppl6::PeekN32(adr)==0x434f4d4d) {			// COMM-Chunk gefunden
			//printf ("COMM-Chunk gefunden\n");
			//ppl6::HexDump((void*)(adr+8),size);
			info.Channels=ppl6::PeekN16(adr+8);
			if (info.Channels==1) info.Mode=AudioInfo::MONO;
			else if (info.Channels==2) info.Mode=AudioInfo::STEREO;
			else return false;	// Not supported
			info.Samples=ppl6::PeekN32(adr+10);
			info.BytesPerSample=(ppl6::PeekN16(adr+14)*info.Channels)/8;
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
			info.Frequency=(ppluint32)frequency;
			if (info.Frequency>0 && info.Samples>0) info.Length=(ppluint32)((ppluint64)info.Samples*1000/info.Frequency);
			info.Bitrate=((ppluint64)info.Frequency*(ppluint64)info.BytesPerSample*8/1000);

		} else if (ppl6::PeekN32(adr)==0x53534e44) {	// SSND-Chunk gefunden
			info.AudioStart=p+8;
			info.AudioSize=size-8;
			info.AudioEnd=info.AudioStart+info.AudioSize-1;
		} else if (ppl6::PeekN32(adr)==0x49443320) {	// ID3-Chunk gefunden
			info.HaveID3v2Tag=true;
			info.ID3v2TagStart=p+8;
		}
		p+=size+8;
	}

	return true;
}

static bool IdentWave(CFileObject &file, AudioInfo &info)
{
	info.Format=AudioInfo::WAVE;
	info.IsVBR=false;
	info.FileSize=file.Size();
	info.Length=0;	// will be calculated later
	const char *buffer=file.Map(0,64);
	const char *fmt=buffer+12;

	ppldd fmtchunklen=peekd(fmt+4);
	info.Channels=peekw(fmt+0x0a);
	if (info.Channels==1) info.Mode=AudioInfo::MONO;
	else if (info.Channels==2) info.Mode=AudioInfo::STEREO;
	else return false;	// Not supported

	info.Frequency=peekd(fmt+0x0c);
	int bitdepth=peekw(fmt+0x16);
	// we support only 8bit and 16bit
	if (bitdepth!=8 && bitdepth!=16) return false;

	buffer=file.Map(fmtchunklen+20,8);				// read datachunk
	if (!buffer) return false;						// File too small
	if (strncmp(buffer,"data",4)!=0) return false;	// no data chunk

	info.Samples=0;
	info.BytesPerSample=(bitdepth/8)*info.Channels;
	info.AudioStart=fmtchunklen+28;
	info.AudioSize=peekd(buffer+4);
	info.AudioEnd=info.AudioStart+info.AudioSize-1;
	info.Samples=info.AudioSize/info.BytesPerSample;
	if (info.Frequency>0 && info.Samples>0) info.Length=(ppluint32)((ppluint64)info.Samples*1000/info.Frequency);
	info.Bitrate=((ppluint64)info.Frequency*(ppluint64)info.BytesPerSample*8/1000);
	info.HaveID3v2Tag=false;
	info.ID3v2TagStart=0;
	return true;
}



static bool IdentMP3(CFileObject &file, AudioInfo &info, PPL_MPEG_HEADER &mp3)
{
	info.Format=AudioInfo::MP3;
	info.IsVBR=mp3.vbr;
	info.FileSize=file.Size();
	info.AudioStart=mp3.start;
	info.AudioEnd=mp3.end;
	info.AudioSize=info.AudioEnd-info.AudioStart+1;
	info.Samples=mp3.samples;
	info.Channels=mp3.stereo;
	info.Bitrate=mp3.bitrate;
	info.Frequency=mp3.frequency;
	info.BytesPerSample=mp3.stereo*2;
	info.Length=mp3.mslength;
	switch (mp3.mode) {
		case 0: info.Mode=AudioInfo::STEREO; break;
		case 1: info.Mode=AudioInfo::JOINT_STEREO; break;
		case 2: info.Mode=AudioInfo::DUAL_CHANNEL; break;
		case 3: info.Mode=AudioInfo::MONO; break;
		default: return false;
	}

	const char *adr=file.Map(0,10);
	info.HaveID3v2Tag=false;
	info.ID3v2TagStart=0;
	if (strncmp(adr,"ID3",3)==0) {
		if (peek8(adr+3)==3 && peek8(adr+4)==0) info.HaveID3v2Tag=true;			// Version 2.3
		else if (peek8(adr+3)==4 && peek8(adr+4)==0) info.HaveID3v2Tag=true;	// Version 2.4
	}
	return true;
}

bool IdentAudioFile(CFileObject &file, AudioInfo &info)
{
	const char *adr=file.Map(0,12);
	if (!adr) return false;
	// AIFF
	if (ppl6::PeekN32(adr+4)<file.Size()
				&& ppl6::PeekN32(adr+0)==0x464F524D
				&& ppl6::PeekN32(adr+8)==0x41494646) return IdentAIFF(file,info);
	// RIFF / WAV
	if (strncmp(adr,"RIFF",4)==0) {
		if (strncmp(adr+8,"WAVE",4)==0) {
			if (strncmp(adr+12,"fmt ",4)==0) {
				if (peekw(adr+20)==1) return IdentWave(file,info);
			}
		}
	}

	PPL_MPEG_HEADER mp3;
	if (IdentMPEG(&file,&mp3)) return IdentMP3(file,info,mp3);
	return false;

}

} // EOF namespace ppl6
