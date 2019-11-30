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
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_MATH_H
#include <math.h>
#endif

#ifdef HAVE_LIBCDIO
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_VERSION
#include <cdio/cdio.h>
#endif

#include "ppl7.h"
#include "ppl7-audio.h"

namespace ppl7 {

bool AudioCD::isSupported()
{
#ifdef HAVE_LIBCDIO
	return true;
#else
	return false;
#endif
}

std::list<String> AudioCD::getDevices()
{
	std::list<String> devices;
#ifdef HAVE_LIBCDIO
	char** d=cdio_get_devices (DRIVER_DEVICE);
	if (d) {
		for (int i=0;d[i]!=NULL;i++) {
			String name;
			name.setf("%s",d[i]);
			devices.push_back(name);
		}
	}

#endif
	return devices;
}

AudioCD::AudioCD()
{
	cdio=NULL;
	num_audio_tracks=0;
	i_tracks=0;
	first_track_num=0;
	audio_frames=0;
	last_lsn=0;
}

AudioCD::~AudioCD()
{
	closeDevice();
}

void AudioCD::closeDevice()
{
#ifndef HAVE_LIBCDIO
	throw UnsupportedFeatureException("cdio");
#else

	if (cdio) cdio_destroy((CdIo_t *)cdio);
	cdio=NULL;
	num_audio_tracks=0;
	i_tracks=0;
	first_track_num=0;
#endif
}

void AudioCD::openDevice(const String &device)
{
#ifndef HAVE_LIBCDIO
	throw UnsupportedFeatureException("cdio");
#else
	if (cdio) closeDevice();

	cdio=NULL;
	myDevice=device;
	if (myDevice.isEmpty()) {
		CdIo_t *p_cdio = cdio_open (NULL, DRIVER_DEVICE);
		if (NULL != p_cdio) {
			myDevice=cdio_get_default_device(p_cdio);
			cdio_destroy(p_cdio);
		}
	}
	cdio=cdio_open((const char*)myDevice, DRIVER_DEVICE);
	if (!cdio) throw DeviceOpenFailed("%s",(const char*)myDevice);

	first_track_num = (size_t)cdio_get_first_track_num((CdIo_t *)cdio);
	i_tracks = (size_t)cdio_get_num_tracks((CdIo_t *)cdio);
	last_lsn=cdio_get_disc_last_lsn((CdIo_t *)cdio);
	countAudioTracks();
#endif
}

void AudioCD::countAudioTracks()
{
#ifndef HAVE_LIBCDIO
	throw UnsupportedFeatureException("cdio");
#else
	num_audio_tracks=0;
	audio_frames=0;
	for (size_t i = first_track_num; i <= i_tracks; i++) {
		if (TRACK_FORMAT_AUDIO == cdio_get_track_format((CdIo_t *)cdio, i)) {
			num_audio_tracks++;
			audio_frames+= cdio_get_track_last_lsn((CdIo_t *)cdio, i) -
					cdio_get_track_lsn((CdIo_t *)cdio, i)+1;

		}
	}
#endif
}

size_t AudioCD::lastLsn() const
{
	return last_lsn;
}

size_t AudioCD::totalAudioFrames() const
{
	return audio_frames;
}

size_t AudioCD::totalAudioLength() const
{
#ifndef HAVE_LIBCDIO
	return 0;
#else
	return audio_frames/CDIO_CD_FRAMES_PER_SEC;
#endif
}


const String &AudioCD::deviceName() const
{
	return myDevice;
}

size_t AudioCD::firstTrack() const
{
	return first_track_num;
}

size_t AudioCD::lastTrack() const
{
	return first_track_num+i_tracks-1;
}

size_t AudioCD::numTotalTracks() const
{
	return i_tracks;
}

size_t AudioCD::numAudioTracks() const
{
	return num_audio_tracks;
}

AudioCD::Track AudioCD::getTrack(int track)
{
#ifndef HAVE_LIBCDIO
	throw UnsupportedFeatureException("cdio");
#else
	if (!cdio) throw DeviceNotOpen();
	AudioCD::Track t;
	t._isAudioTrack=true;
	if (cdio_get_track_format((CdIo_t *)cdio,track)!=TRACK_FORMAT_AUDIO) t._isAudioTrack=false;
	t._track=track;
	t._start=(size_t)cdio_get_track_lsn((CdIo_t *)cdio,track);
	t._end=t._start+(size_t)cdio_get_track_sec_count((CdIo_t *)cdio,track)-1;
	t._channels=cdio_get_track_channels((CdIo_t *)cdio,track);
	t._hasPreemphasis=(cdio_get_track_preemphasis((CdIo_t *)cdio,track)==CDIO_TRACK_FLAG_TRUE?true:false);
	t._hasCopyPermit=(cdio_get_track_copy_permit((CdIo_t *)cdio,track)==CDIO_TRACK_FLAG_TRUE?true:false);
	return t;
#endif
}

bool AudioCD::isAudioTrack(int track)
{
#ifndef HAVE_LIBCDIO
	throw UnsupportedFeatureException("cdio");
#else
	if (!cdio) throw DeviceNotOpen();
	if (cdio_get_track_format((CdIo_t *)cdio,track)==TRACK_FORMAT_AUDIO) return true;
	return false;
#endif
}

/*
static inline uint8_t ppl_to_bcd8 (uint8_t n)
{
  return ((n/10)<<4) | (n%10);
}
*/

AudioCD::Toc AudioCD::lsn2toc(size_t lsn)
{
#ifndef HAVE_LIBCDIO
	throw UnsupportedFeatureException("cdio");
#else
	lsn_t l=(lsn_t) lsn;
	int m, s, f;
	if ( l >= -CDIO_PREGAP_SECTORS ){
		m    = (l + CDIO_PREGAP_SECTORS) / CDIO_CD_FRAMES_PER_MIN;
		l -= m * CDIO_CD_FRAMES_PER_MIN;
		s    = (l + CDIO_PREGAP_SECTORS) / CDIO_CD_FRAMES_PER_SEC;
		l -= s * CDIO_CD_FRAMES_PER_SEC;
		f    = l + CDIO_PREGAP_SECTORS;
	} else {
		m    = (l + CDIO_CD_MAX_LSN)     / CDIO_CD_FRAMES_PER_MIN;
		l -= m * (CDIO_CD_FRAMES_PER_MIN);
		s    = (l+CDIO_CD_MAX_LSN)       / CDIO_CD_FRAMES_PER_SEC;
		l -= s * CDIO_CD_FRAMES_PER_SEC;
		f    = l + CDIO_CD_MAX_LSN;
	}

	if (m > 99) {
		m=99;
	}
	Toc toc;
	toc.min=m;
	toc.sec=s;
	toc.frames=f;
	return toc;
#endif
}


AudioCD::Track::Track()
{
	_track=0;
	_start=0;
	_end=0;
	_hasPreemphasis=false;
	_hasCopyPermit=false;
	_isAudioTrack=false;
	_channels=0;
}

int AudioCD::Track::track() const
{
	return _track;
}

size_t AudioCD::Track::start() const
{
	return _start;
}

AudioCD::Toc AudioCD::Track::start_toc() const
{
	return AudioCD::lsn2toc(_start);
}

size_t AudioCD::Track::end() const
{
	return _end;
}

size_t AudioCD::Track::size() const
{
	return _end-_start+1;
}

size_t AudioCD::Track::seconds() const
{
	return round((float)(_end-_start+1)/75.0f);
}

bool AudioCD::Track::hasPreemphasis() const
{
	return _hasPreemphasis;
}

bool AudioCD::Track::hasCopyPermit() const
{
	return _hasCopyPermit;
}

bool AudioCD::Track::isAudioTrack() const
{
	return _isAudioTrack;
}

int AudioCD::Track::channels() const
{
	return _channels;
}


}	// EOF namespace ppl6
