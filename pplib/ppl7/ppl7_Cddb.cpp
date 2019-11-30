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

#include "ppl7.h"
#include "ppl7-audio.h"

namespace ppl7 {

bool CDDB::isSupported()
{
	return Curl::isSupported();
}

static inline int cddb_sum(int n)
{
	int	ret;
	/* For backward compatibility this algorithm must not change */
	ret = 0;
	while (n > 0) {
		ret = ret + (n % 10);
		n = n / 10;
	}
	return (ret);
}


unsigned int CDDB::calcDiscId(AudioCD &cd)
{
	int	i,
		t = 0,
		n = 0;

	/* For backward compatibility this algorithm must not change */

	i = cd.firstTrack();
	int tot_trks=cd.numTotalTracks();

	while (i <= tot_trks) {
		AudioCD::Track track=cd.getTrack(i);
		AudioCD::Toc toc=track.start_toc();
		n = n + cddb_sum((toc.min * 60) + toc.sec);
		i++;
	}
	AudioCD::Track track=cd.getTrack(1);
	AudioCD::Toc first_toc=track.start_toc();
	AudioCD::Toc last_toc=AudioCD::lsn2toc(cd.lastLsn());
	t = ((last_toc.min * 60) + last_toc.sec) -
	    ((first_toc.min * 60) + first_toc.sec);

	return ((n % 0xff) << 24 | t << 8 | tot_trks);
}




CDDB::CDDB()
{
	QueryPath="/~cddb/cddb.cgi";
	Server="freedb.freedb.org";
	ClientName="pplib";
	ClientVersion.setf("%i.%i.%i",
			PPL7_VERSION_MAJOR,
			PPL7_VERSION_MINOR,
			PPL7_VERSION_BUILD);
	UserName="anonymous";
	Hostname="localhost";
	port=80;
}

CDDB::~CDDB()
{
}

void CDDB::setHttpServer(const String &server, int port)
{
	this->Server=server;
	this->port=port;
}

void CDDB::setQueryPath(const String &path)
{
	this->QueryPath=path;
}

void CDDB::setProxy(const String &hostname, int port)
{
	curl.setProxy(hostname, port);
}

void CDDB::setClient(const String &name, const String &version)
{
	ClientName=name;
	ClientVersion=version;
}

void CDDB::setUser(const String &username, const String &hostname)
{
	UserName=username;
	Hostname=hostname;
}

static unsigned int	hex2int(const String &s)
{
	const unsigned char *p=(const unsigned char *)s.getPtr();
	unsigned char *t;
	size_t bytes=s.len();
	if (bytes!=8) {
		throw CDDB::InvalidDiscId("%s",(const char*)s);
	}
	int ret=0;
	t=(unsigned char*)&ret;
	unsigned char value;
	for (size_t source=0, target=0;source<bytes;source+=2,target++) {
		unsigned char first=p[source];
		unsigned char second=p[source+1];
		if (first>='0' && first<='9') value=(first-'0');
		else if (first>='a' && first<='f') value=(first-'a'+10);
		else if (first>='A' && first<='F') value=(first-'A'+10);
		else {
			throw CDDB::InvalidDiscId("%s",(const char*)s);
		}
		value=value<<4;
		if (second>='0' && second<='9') value|=(second-'0');
		else if (second>='a' && second<='f') value|=(second-'a'+10);
		else if (second>='A' && second<='F') value|=(second-'A'+10);
		else {
			throw CDDB::InvalidDiscId("%s",(const char*)s);
		}
		// TODO: big endian beruecksichtigen
		t[3-target]=value;
	}
	return ret;
}

String CDDB::buildUri(const String &cmd)
{
	String Tmp,Uri;
	Uri="http://"+Server;
	if (port!=80) Uri.appendf(":%i",port);
	Uri+=QueryPath;
	Uri+="?cmd="+UrlEncode(cmd);
	Tmp=UserName+" "+Hostname+" "+ClientName+" "+ClientVersion;
	Uri+="&hello="+UrlEncode(Tmp);
	Uri+="&proto=6";
	return Uri;
}

int CDDB::query(AudioCD &cd, std::list<Disc> &list)
{
	list.clear();
	String cmd,Uri,Tmp;
	cmd.setf("cddb query %08x %i ",calcDiscId(cd), (int)cd.numTotalTracks());
	for (size_t i=cd.firstTrack();i<=cd.lastTrack();i++) {
		AudioCD::Track track=cd.getTrack(i);
		cmd.appendf("%i ",(int)track.start()+150);
	}
	cmd.appendf("%i",(int)(cd.lastLsn()+150)/75);
	Uri=buildUri(cmd);

	//printf ("Uri: %s\n",(const char*)Uri);

	curl.setURL(Uri);

	curl.get();
	String header=curl.getHeader();
	String payload=curl.getResultBufferAsString();
	payload.replace("\n\r","\n");
	payload.replace("\r\n","\n");


	//header.Print(true);
	//payload.Print(true);

	if (!header.pregMatch("/^.*200\\s+OK/m")) {
		return 0;
	}
	Array rows(payload,"\n");
	bool multiResults=false;
	for (size_t r=0;r<rows.size();r++) {
		String row=rows[r];
		Array matches;
		if (row.pregMatch("/^200\\s+(.*?)\\s+([0-9a-f]{8})\\s(.*?)\\/(.*?)$/",matches)) {
			Disc disc;
			unsigned int discid=hex2int(matches[2]);
			getDisc(discid,matches[1],disc);
			list.push_back(disc);
		} else if (row.pregMatch("/^210 Found exact matches.*$/")) {
			multiResults=true;
		} else if (multiResults==true &&
				row.pregMatch("/^(.*?)\\s+([0-9a-f]{8})\\s(.*?)\\/(.*?)$/",matches)) {
			Disc disc;
			unsigned int discid=hex2int(matches[2]);
			getDisc(discid,matches[1],disc);
			list.push_back(disc);
		}
	}
	return list.size();
}

static void parseOffsets(const String &payload, Array &offsets)
{
	bool headerFound=false;
	bool offsetsFound=false;
	Array a(payload,"\n");
	for (size_t i=0;i<a.size();i++) {
		String row=a[i];
		row.trim();
		//printf ("Parsing row: %s\n",(const char*)row);
		if (row.left(6)=="# xmcd") headerFound=true;
		else if (headerFound) {
			if (row=="# Track frame offsets:") offsetsFound=true;
			else if (offsetsFound) {
				Array matches;
				if (row.pregMatch("/^#\\s+([0-9]+)$/",matches)) {
					offsets.add(matches[1]);
				} else break;
			}
		}
	}
	//offsets.List();
}

static void storeDisc(CDDB::Disc &disc, const String &payload)
{
	AssocArray a;
	Array matches;
	a.fromTemplate(payload,"\n","=","");
	String Tmp=a["DTITLE"];
	if (Tmp.pregMatch("/^(.*?)\\s\\/\\s(.*)$/",matches)) {
		disc.Artist=matches[1];
		disc.Title=matches[2];
	} else {
		disc.Artist=Tmp;
		disc.Title=Tmp;
	}
	disc.Extra=a["EXTD"].toString();
	disc.Artist.trim();
	disc.Title.trim();
	disc.Extra.trim();
	disc.Extra.replace("\\n","\n");
	disc.year=a["DYEAR"].toString().toInt();
	disc.genre=a["DGENRE"].toString();
	disc.genre.trim();

	if (payload.pregMatch("/#\\sDisc\\slength:\\s+([0-9]+)/m",matches)) {
		disc.length=matches.get(1).toInt();
	}
	Array offsets;
	parseOffsets(payload,offsets);

	for (int i=0;i<99;i++) {
		Tmp.setf("TTITLE%i",i);
		if (!a.exists(Tmp)) break;
		CDDB::Track t;
		t.number=i+1;
		t.frame_offset=offsets.get(i).toInt();
		t.length=(offsets.get(i+1).toInt()-t.frame_offset)/75;
		if (t.length<0) {
			t.length=(disc.length*75+150-t.frame_offset)/75;
		}
		t.Artist=disc.Artist;
		t.Title=a[Tmp];
		Array tmatches;
		if (t.Title.pregMatch("/^(.*?)\\s\\/\\s(.*)$/",tmatches)) {
			t.Artist=tmatches.get(1);
			t.Title=tmatches.get(2);
		}
		t.Artist.trim();
		t.Title.trim();

		Tmp.setf("EXTT%i",i);
		t.Extra=a[Tmp];
		t.Extra.trim();
		t.Extra.replace("\\n","\n");
		disc.Tracks.push_back(t);
	}

}

void CDDB::getDisc(unsigned int discId, const String &category, Disc &d)
{
	String cmd,Uri,Tmp;
	cmd.setf("cddb read %s %08x",(const char*)category, discId);
	Uri=buildUri(cmd);
	curl.setURL(Uri);
	curl.get();
	String header=curl.getHeader();
	String payload=curl.getResultBufferAsString();
	payload.replace("\n\r","\n");
	payload.replace("\r\n","\n");

	if (!header.pregMatch("/^.*200\\s+OK/m")) {
		throw QueryFailed("%s",(const char*)header);
	}
	if (!payload.pregMatch("/^210\\s/m")) {
		throw QueryFailed("%s",(const char*)header);
	}

	//payload.Print(true);
	d.category=category;
	d.discId=discId;
	storeDisc(d,payload);
}


}	// EOF namespace ppl7
