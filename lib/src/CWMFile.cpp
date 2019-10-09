/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: CWmFile.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libwinmusik3.h"
#include "wm_exceptions.h"
#include "cwmfile.h"

namespace de {
namespace pfp {
namespace winmusik {


/*!\class CWMFileChunk
 * \ingroup WM3StorageClasses
 * \brief Einzelner Chunk aus einem CWMFile
 *
 */

CWMFileChunk::CWMFileChunk()
{
	memset(chunkname,0,5);
	filepos=0;
	size=0;
	timestamp=0;
	datasize=0;
	formatversion=1;
	data=NULL;
	version=0;
}

CWMFileChunk::~CWMFileChunk()
{
	Clear();
}

void CWMFileChunk::Clear()
{
	memset(chunkname,0,5);
	filepos=0;
	size=0;
	timestamp=0;
	datasize=0;
	formatversion=1;
	data=NULL;
}

ppluint32 CWMFileChunk::GetChunkDataSize()
{
	return datasize;
}

const char *CWMFileChunk::GetChunkData()
{
    return static_cast<const char*>(data);
}

const char	*CWMFileChunk::GetChunkName()
{
	return chunkname;
}

int CWMFileChunk::SetChunkData(const char *chunkname, const char *data, ppluint32 size, ppluint32 oldfilepos, ppluint32 version, ppluint8 formatversion)
{
	Clear();
	if (chunkname==NULL || strlen(chunkname)!=4) {
		return 0;
	}
	strcpy(this->chunkname,chunkname);
    this->data=static_cast<const char*>(data);
	this->datasize=size;
	filepos=oldfilepos;
	this->version=version;
	this->formatversion=formatversion;
	return 1;
}

void CWMFileChunk::SetFormatVersion(ppluint8 v)
{
	formatversion=v;
}

ppluint8 CWMFileChunk::GetFormatVersion()
{
	return formatversion;
}


ppluint32 CWMFileChunk::GetFilepos()
{
	return filepos;
}

ppluint32 CWMFileChunk::GetVersion()
{
	return version;
}

ppluint32 CWMFileChunk::GetTimestamp()
{
	return timestamp;
}


/*!\class CWMFile
 * \ingroup WM3StorageClasses
 * \brief WinMusik Datenbank-Datei
 *
 */

CWMFile::CWMFile()
{
	version=subversion=0;
	timestamp=lastchange=0;
	pos=eof=first=0;
}

CWMFile::~CWMFile()
{
}

void CWMFile::Close()
{
    ff.close();
	version=subversion=0;
	timestamp=lastchange=0;
	pos=eof=first=0;
}

bool CWMFile::IsOpen() const
{
    return ff.isOpen();
}

void CWMFile::Open(const char *filename)
{
    Close();
    if (!ppl7::File::exists(filename)) {
		// Wenn die Datei noch nicht existiert, legen wir sie an
        ff.open(filename,ppl7::File::WRITE);
		// Header erstellen
        char *header=static_cast<char*>(malloc(32));
		if (!header) {
            ff.close();
            ppl7::File::unlink(filename);
            throw ppl7::OutOfMemoryException();
		}
		strcpy(header,"PFP-File");
        ppl7::Poke8(header+8,3);
        ppl7::Poke8(header+9,32);
		strcpy(header+10,"PFWM");
        ppl7::Poke8(header+14,0);
        ppl7::Poke8(header+15,3);
        ppl7::Poke8(header+16,0);
        ppl7::Poke8(header+17,0);
        ppl7::Poke8(header+18,0);
        ppl7::Poke8(header+19,0);
        ppluint32 timestamp=static_cast<ppluint32>(ppl7::GetTime());
        ppl7::Poke32(header+20,timestamp);
        ppl7::Poke32(header+24,timestamp);
        ppl7::Poke32(header+28,32);
        ff.write(header,32);
		// Ersten Chunk erstellen
		strcpy(header,"ENDF");
        ppl7::Poke32(header+4,0);
        ppl7::Poke32(header+8,0);
        ppl7::Poke8(header+12,0);
        ff.write(header,13);
        ff.close();
		free(header);
	}
    ff.open(filename,ppl7::File::READWRITE);
    ff.setMapReadAhead(1024*1024*10);
	// Header einlesen und prüfen
    const char *header;
    header=static_cast<const char*>(ff.map(0,32));
    if (strncmp(header,"PFP-File",8)!=0) throw InvalidDatabaseFileException(filename);
    if (ppl7::Peek8(header+8)!=3) throw InvalidDatabaseFileException(filename);
    if ((pos=ppl7::Peek8(header+9))<32) throw InvalidDatabaseFileException(filename);
    if (strncmp(header+10,"PFWM",4)!=0) throw InvalidDatabaseFileException(filename);

    subversion=static_cast<unsigned char>(ppl7::Peek8(header+14));
    version=static_cast<unsigned char>(ppl7::Peek8(header+15));
    if (version!=3 || subversion!=0) throw InvalidDatabaseFileException(filename);
    if (ppl7::Peek8(header+16)!=0) throw InvalidDatabaseFileException(filename);
	first=pos;
    timestamp=ppl7::Peek32(header+20);
    lastchange=ppl7::Peek32(header+24);
    eof=ppl7::Peek32(header+28);
}

void CWMFile::read_chunk(CWMFileChunk &chunk, ppluint32 filepos, const char *ptr)
{
    // Daten einlesen
    chunk.filepos=filepos;
    strncpy(chunk.chunkname,ptr,4);
    chunk.chunkname[4]=0;
    chunk.size=ppl7::Peek32(ptr+4);
    chunk.datasize=chunk.size-17;
    chunk.timestamp=ppl7::Peek32(ptr+8);
    chunk.version=ppl7::Peek32(ptr+12);
    chunk.formatversion=static_cast<unsigned char>(ppl7::Peek8(ptr+16));
    chunk.data=ff.map(filepos+17,chunk.datasize);
}

bool CWMFile::GetNextChunk(CWMFileChunk &chunk)
{
    chunk.Clear();
    if (!pos) throw DatabaseNotOpenException();
    if (pos>=eof) return false;
    // Header des nächsten Chunks einlesen
    const char *ptr=ff.map(pos,17);
    if (strncmp(ptr,"ENDF",4)==0) return false;
    read_chunk(chunk,pos,ptr);
    pos+=chunk.size;
    return true;
}

void CWMFile::GetChunk(CWMFileChunk &chunk, ppluint32 filepos)
{
    if (filepos==0) throw ppl7::InvalidArgumentsException();
    ff.seek(filepos);
    const char *ptr=ff.map(filepos,17);
	char tmp[5];
	strncpy(tmp,ptr,4);
	tmp[4]=0;
    if (!IsValidChunkName(tmp)) throw UnknownDataChunkException("Chunkname: %s",tmp);
    read_chunk(chunk,filepos,ptr);
}



bool CWMFile::IsValidChunkName(const char *name)
{
	if (name!=NULL &&strlen(name)==4) {
        if (strcmp(name,"TITL")==0) return true;
        if (strcmp(name,"VERS")==0) return true;
        if (strcmp(name,"DEVI")==0) return true;
        if (strcmp(name,"LABL")==0) return true;
        if (strcmp(name,"RSRC")==0) return true;
        if (strcmp(name,"GENR")==0) return true;
        if (strcmp(name,"RDEV")==0) return true;
        if (strcmp(name,"TRAK")==0) return true;
        if (strcmp(name,"PCHS")==0) return true;
        if (strcmp(name,"SHRT")==0) return true;
        if (strcmp(name,"OIMP")==0) return true;
	}
    return false;
}

void CWMFile::SaveChunk(CWMFileChunk &chunk)
{
	char timestamp[4];
	char header[18];
    if (!pos) throw DatabaseNotOpenException();
    if (!IsValidChunkName(chunk.chunkname)) throw UnknownDataChunkException("Chunkname: %s",chunk.chunkname);
    if (chunk.filepos) {
		// Haben wir genug Platz, um den alten Chunk zu überschreiben?
        if (ff.read(header,17,chunk.filepos)!=17) throw CouldNotReadDatabaseRecordException();
        ppluint32 oldsize=ppl7::Peek32(header+4);
        ppluint32 oldversion=ppl7::Peek32(header+12);
        if (chunk.version!=oldversion)
            throw DatabaseRecordWasModifiedException("Gespeicherte Version: %i, diese Version: %i",oldversion, chunk.version);
        ff.unmap();
        if (oldsize>=(chunk.datasize+17)) {
            chunk.version++;
            chunk.timestamp=static_cast<ppluint32>(ppl7::GetTime());
            strncpy(header,chunk.chunkname,4);
            ppl7::Poke32(header+4,oldsize);
            ppl7::Poke32(header+8,chunk.timestamp);
            ppl7::Poke32(header+12,chunk.version);
            ppl7::Poke8(header+16,chunk.formatversion);
            ff.write(header,17,chunk.filepos);
            ff.write(chunk.data,chunk.datasize,chunk.filepos+17);
            ppl7::Poke32(timestamp,static_cast<ppluint32>(ppl7::GetTime()));
            ff.write(timestamp,4,24);
            return;
		}
		// Nein, wir kennzeichnen den alten Datensatz als gelöscht
		strncpy(header,"FREE",4);
        ff.write(header,4,chunk.filepos);
	}
    chunk.filepos=eof;
    chunk.size=chunk.datasize+17;
    chunk.timestamp=static_cast<ppluint32>(ppl7::GetTime());
    chunk.version=1;
    strncpy(header,chunk.chunkname,4);
    ppl7::Poke32(header+4,chunk.size);
    ppl7::Poke32(header+8,chunk.timestamp);
    ppl7::Poke32(header+12,chunk.version);
    ppl7::Poke8(header+16,chunk.formatversion);
    ff.write(header,17,eof);
    ff.write(chunk.data,chunk.datasize,eof+17);

	// Neuen ENDF-Chunk schreiben
    eof=eof+17+chunk.datasize;
	pos=eof;
	strcpy(header,"ENDF");
    ppl7::Poke32(header+4,0);
    ppl7::Poke32(header+8,0);
    ppl7::Poke32(header+12,0);
    ppl7::Poke8(header+16,0);
    ff.write(header,17,eof);

	// Position des ENDF-Chunks in den Header schreiben
    ppl7::Poke32(header,eof);
    ff.write(header,4,28);

	// Timestamp in den Header schreiben
    ppl7::Poke32(header,static_cast<ppluint32>(ppl7::GetTime()));
    ff.write(header,4,24);
}

void CWMFile::DeleteChunk(const CWMFileChunk &chunk)
{
	char header[18];
    if (!pos) throw DatabaseNotOpenException();
    if (!IsValidChunkName(chunk.chunkname)) throw UnknownDataChunkException("Chunkname: %s",chunk.chunkname);
    if (chunk.filepos) {
        ff.unmap();
        if (ff.read(header,17,chunk.filepos)!=17) throw CouldNotReadDatabaseRecordException();
		strncpy(header,"FREE",4);
        ff.write(header,4,chunk.filepos);
        // Timestamp in den Header schreiben
        ppl7::Poke32(header,static_cast<ppluint32>(ppl7::GetTime()));
        ff.write(header,4,24);
    }
}

void CWMFile::Reset()
{
	pos=first;
}

void CWMFile::ListChunks()
{
    if (!pos) return;
	char name[5];
	const char *header;
	pos=first;
    while (pos<ff.size()) {
        header=ff.map(pos,17);
		if (!header) return;
		strncpy(name,header,4);
		name[4]=0;
        printf ("%s: %i Bytes, Timestamp: %u, Version: %u, Format: %u\n",name, ppl7::Peek32(header+4),
                ppl7::Peek32(header+8), ppl7::Peek32(header+12), ppl7::Peek8(header+16));
		if (strncmp(header,"ENDF",4)==0) break;
        pos+=ppl7::Peek32(header+4);
	}
}

ppluint32 CWMFile::GetFileSize() const
{
    return static_cast<ppluint32>(ff.size());
}
ppluint32 CWMFile::GetFilePosition() const
{
	return pos;
}

void CWMFile::CopyDatabase(CWMFile &oldfile, CWMFile &newfile, CProgressUpdate *callback)
{
	oldfile.Reset();
	CWMFileChunk chunk;
    while (oldfile.GetNextChunk(chunk)) {
        if (callback) {
            callback->Update(static_cast<int>(oldfile.GetFilePosition()*100/
                                              oldfile.GetFileSize()));
        }
		chunk.filepos=0;
		if (strncmp(chunk.chunkname,"FREE",4)!=0) {
			if (strncmp(chunk.chunkname,"ENDF",4)!=0) {
                newfile.SaveChunk(chunk);
			}
		}
	}
}

}}}	// EOF Namespace de.pfp.winmusik
