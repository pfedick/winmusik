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
#ifdef HAVE_MATH_H
#include <math.h>
#endif

#include "ppl6.h"
#include "ppl6-exceptions.h"
#include "ppl6-sound.h"

namespace ppl6 {

bool CDDB::isSupported()
{
	return ppl6::CCurl::isSupported();
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


unsigned int CDDB::calcDiscId(ppl6::AudioCD &cd)
{
	int	i,
		t = 0,
		n = 0;

	/* For backward compatibility this algorithm must not change */

	i = cd.firstTrack();
	int tot_trks=cd.numTotalTracks();

	while (i <= tot_trks) {
		ppl6::AudioCD::Track track=cd.getTrack(i);
		ppl6::AudioCD::Toc toc=track.start_toc();
		n = n + cddb_sum((toc.min * 60) + toc.sec);
		i++;
	}
	ppl6::AudioCD::Track track=cd.getTrack(1);
	ppl6::AudioCD::Toc first_toc=track.start_toc();
	ppl6::AudioCD::Toc last_toc=ppl6::AudioCD::lsn2toc(cd.lastLsn());
	t = ((last_toc.min * 60) + last_toc.sec) -
	    ((first_toc.min * 60) + first_toc.sec);

	return ((n % 0xff) << 24 | t << 8 | tot_trks);
}




CDDB::CDDB()
{
	QueryPath="/~cddb/cddb.cgi";
	Server="freedb.freedb.org";
	ClientName="pplib";
	ClientVersion.Setf("%i.%i.%i",
			PPL_VERSION_MAJOR,
			PPL_VERSION_MINOR,
			PPL_VERSION_BUILD);
	UserName="anonymous";
	Hostname="localhost";
	port=80;
}

CDDB::~CDDB()
{
}

void CDDB::setHttpServer(const ppl6::CString &server, int port)
{
	this->Server=server;
	this->port=port;
}

void CDDB::setQueryPath(const ppl6::CString &path)
{
	this->QueryPath=path;
}

void CDDB::setProxy(const ppl6::CString &hostname, int port)
{
	if (!curl.SetProxy(hostname,port)) {
		ppl6::CString e;
		ppl6::Error2String(e);
		throw ppl6::OperationFailedException("%s",(const char*)e);
	}
}

void CDDB::setClient(const ppl6::CString &name, const ppl6::CString &version)
{
	ClientName=name;
	ClientVersion=version;
}

void CDDB::setUser(const ppl6::CString &username, const ppl6::CString &hostname)
{
	UserName=username;
	Hostname=hostname;
}

static unsigned int	hex2int(const ppl6::CString &s)
{
	const unsigned char *p=(const unsigned char *)s.GetPtr();
	unsigned char *t;
	size_t bytes=s.Len();
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

ppl6::CString CDDB::buildUri(const ppl6::CString &cmd)
{
	ppl6::CString Tmp,Uri;
	Uri="http://"+Server;
	if (port!=80) Uri.Concatf(":%i",port);
	Uri+=QueryPath;
	Uri+="?cmd="+ppl6::UrlEncode(cmd);
	Tmp=UserName+" "+Hostname+" "+ClientName+" "+ClientVersion;
	Uri+="&hello="+ppl6::UrlEncode(Tmp);
	Uri+="&proto=6";
	return Uri;
}

int CDDB::query(ppl6::AudioCD &cd, std::list<Disc> &list)
{
	list.clear();
	ppl6::CString cmd,Uri,Tmp;
	cmd.Setf("cddb query %08x %i ",calcDiscId(cd), (int)cd.numTotalTracks());
	for (size_t i=cd.firstTrack();i<=cd.lastTrack();i++) {
		ppl6::AudioCD::Track track=cd.getTrack(i);
		cmd.Concatf("%i ",(int)track.start()+150);
	}
	cmd.Concatf("%i",(int)(cd.lastLsn()+150)/75);
	Uri=buildUri(cmd);

	//printf ("Uri: %s\n",(const char*)Uri);

	curl.SetURL(Uri);

	if (!curl.Get()) {
		ppl6::Error2String(Tmp);
		Tmp.Print(true);
		throw QueryFailed("%s",(const char*)Tmp);
	}
	ppl6::CString header=curl.GetHeader();
	ppl6::CString payload=curl.GetResultBuffer();
	payload.Replace("\n\r","\n");
	payload.Replace("\r\n","\n");


	//header.Print(true);
	//payload.Print(true);

	if (!header.PregMatch("/^.*200\\s+OK/m")) {
		return 0;
	}
	ppl6::CArray rows(payload,"\n");
	bool multiResults=false;
	for (int r=0;r<rows.Num();r++) {
		ppl6::CString row=rows[r];
		ppl6::CArray matches;
		if (row.PregMatch("/^200\\s+(.*?)\\s+([0-9a-f]{8})\\s(.*?)\\/(.*?)$/",matches)) {
			Disc disc;
			unsigned int discid=hex2int(matches[2]);
			getDisc(discid,matches[1],disc);
			list.push_back(disc);
		} else if (row.PregMatch("/^210 Found exact matches.*$/")) {
			multiResults=true;
		} else if (multiResults==true &&
				row.PregMatch("/^(.*?)\\s+([0-9a-f]{8})\\s(.*?)\\/(.*?)$/",matches)) {
			Disc disc;
			unsigned int discid=hex2int(matches[2]);
			getDisc(discid,matches[1],disc);
			list.push_back(disc);
		}
	}
	return list.size();
}

static void parseOffsets(const ppl6::CString &payload, ppl6::CArray &offsets)
{
	bool headerFound=false;
	bool offsetsFound=false;
	ppl6::CArray a(payload,"\n");
	for (int i=0;i<a.Num();i++) {
		ppl6::CString row=a[i];
		row.Trim();
		//printf ("Parsing row: %s\n",(const char*)row);
		if (row.Left(6)=="# xmcd") headerFound=true;
		else if (headerFound) {
			if (row=="# Track frame offsets:") offsetsFound=true;
			else if (offsetsFound) {
				if (row.PregMatch("/^#\\s+([0-9]+)$/")) {
					offsets.Add(row.GetMatch(1));
				} else break;
			}
		}
	}
	//offsets.List();
}

static void storeDisc(CDDB::Disc &disc, const ppl6::CString &payload)
{
	ppl6::CAssocArray a;
	a.CreateFromTemplate(payload,"\n","=","");
	ppl6::CString Tmp=a["DTITLE"];
	if (Tmp.PregMatch("/^(.*?)\\s\\/\\s(.*)$/")) {
		disc.Artist=Tmp.GetMatch(1);
		disc.Title=Tmp.GetMatch(2);
	} else {
		disc.Artist=Tmp;
		disc.Title=Tmp;
	}
	disc.Extra=a["EXTD"];
	disc.Artist.Trim();
	disc.Title.Trim();
	disc.Extra.Trim();
	disc.Extra.Replace("\\n","\n");
	disc.year=a.ToInt("DYEAR");
	disc.genre=a["DGENRE"];
	disc.genre.Trim();

	ppl6::CArray matches;
	if (payload.PregMatch("/#\\sDisc\\slength:\\s+([0-9]+)/m",matches)) {
		disc.length=matches.GetString(1).ToInt();
	}
	ppl6::CArray offsets;
	parseOffsets(payload,offsets);

	for (int i=0;i<99;i++) {
		Tmp.Setf("TTITLE%i",i);
		if (!a.HaveKey(Tmp)) break;
		CDDB::Track t;
		t.number=i+1;
		t.frame_offset=offsets.GetString(i).ToInt();
		t.length=(offsets.GetString(i+1).ToInt()-t.frame_offset)/75;
		if (t.length<0) {
			t.length=(disc.length*75+150-t.frame_offset)/75;
		}
		t.Artist=disc.Artist;
		t.Title=a[Tmp];
		if (t.Title.PregMatch("/^(.*?)\\s\\/\\s(.*)$/")) {
			t.Artist=t.Title.GetMatch(1);
			t.Title=t.Title.GetMatch(2);
		}
		t.Artist.Trim();
		t.Title.Trim();

		Tmp.Setf("EXTT%i",i);
		t.Extra=a[Tmp];
		t.Extra.Trim();
		t.Extra.Replace("\\n","\n");
		disc.Tracks.push_back(t);
	}

}

void CDDB::getDisc(unsigned int discId, const ppl6::CString &category, Disc &d)
{
	ppl6::CString cmd,Uri,Tmp;
	cmd.Setf("cddb read %s %08x",(const char*)category, discId);
	Uri=buildUri(cmd);
	curl.SetURL(Uri);
	if (!curl.Get()) {
		ppl6::Error2String(Tmp);
		Tmp.Print(true);
		throw QueryFailed("%s",(const char*)Tmp);
	}
	ppl6::CString header=curl.GetHeader();
	ppl6::CString payload=curl.GetResultBuffer();
	payload.Replace("\n\r","\n");
	payload.Replace("\r\n","\n");

	if (!header.PregMatch("/^.*200\\s+OK/m")) {
		throw QueryFailed("%s",(const char*)header);
	}
	if (!payload.PregMatch("/^210\\s/m")) {
		throw QueryFailed("%s",(const char*)header);
	}

	//payload.Print(true);
	d.category=category;
	d.discId=discId;
	storeDisc(d,payload);
}



}	// EOF namespace ppl6
