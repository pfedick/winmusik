/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

#include "wm_cwmfile.h"
#include "wm_exceptions.h"

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
	memset(chunkname, 0, 5);
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
	memset(chunkname, 0, 5);
	filepos=0;
	size=0;
	timestamp=0;
	datasize=0;
	formatversion=1;
	data=NULL;
}

uint32_t CWMFileChunk::GetChunkDataSize() const
{
	return datasize;
}

const char* CWMFileChunk::GetChunkData() const
{
	return (const char*)data;
}

const char* CWMFileChunk::GetChunkName() const
{
	return chunkname;
}

uint32_t CWMFileChunk::GetChunkId() const
{
	return ppl7::Peek32(chunkname);
}

int CWMFileChunk::SetChunkData(const char* chunkname, const char* data, uint32_t size, uint32_t oldfilepos, uint32_t version, uint8_t formatversion)
{
	Clear();
	if (chunkname == NULL || strlen(chunkname) != 4) {
		return 0;
	}
	strcpy(this->chunkname, chunkname);
	this->data=(char*)data;
	this->datasize=size;
	filepos=oldfilepos;
	this->version=version;
	this->formatversion=formatversion;
	return 1;
}

void CWMFileChunk::SetFormatVersion(uint8_t v)
{
	formatversion=v;
}

uint8_t CWMFileChunk::GetFormatVersion() const
{
	return formatversion;
}


uint32_t CWMFileChunk::GetFilepos() const
{
	return filepos;
}

uint32_t CWMFileChunk::GetVersion() const
{
	return version;
}

uint32_t CWMFileChunk::GetTimestamp() const
{
	return timestamp;
}

void CWMFileChunk::HexDump(ppl7::Logger* log) const
{
	if (log) {
		log->printf(ppl7::Logger::DEBUG, 2, "CWMFileChunk", "HexDump", __FILE__, __LINE__,
			"Chunk: %s, Filepos: %u, Size: %u, Timestamp: %u", chunkname, filepos, size, timestamp);
		log->printf(ppl7::Logger::DEBUG, 2, "CWMFileChunk", "HexDump", __FILE__, __LINE__,
			"Version: %u, Datasize: %u, formatversion: %u", version, datasize, formatversion);
		log->hexDump(ppl7::Logger::DEBUG, 2, data, datasize);
	} else {
		ppl7::HexDump((void*)data, datasize);
	}
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

void CWMFile::Open(const char* filename)
{
	ff.close();
	version=subversion=0;
	timestamp=lastchange=0;
	pos=eof=first=0;
	if (!ppl7::File::exists(filename)) {
		ppl7::ByteArray Buffer(32);
		// Wenn die Datei noch nicht existiert, legen wir sie an
		ff.open(filename, ppl7::File::WRITE);
		try {
			char* header=(char*)Buffer.ptr();
			memcpy(header, "PFP-File", 8);
			ppl7::Poke8(header + 8, 3);
			ppl7::Poke8(header + 9, 32);
			memcpy(header + 10, "PFWM", 4);
			ppl7::Poke8(header + 14, 0);
			ppl7::Poke8(header + 15, 3);
			ppl7::Poke8(header + 16, 0);
			ppl7::Poke8(header + 17, 0);
			ppl7::Poke8(header + 18, 0);
			ppl7::Poke8(header + 19, 0);
			uint32_t timestamp=(uint32_t)ppl7::GetTime();
			ppl7::Poke32(header + 20, timestamp);
			ppl7::Poke32(header + 24, timestamp);
			ppl7::Poke32(header + 28, 32);
			ff.write(header, 32);
			// Ersten Chunk erstellen
			memcpy(header, "ENDF", 4);
			ppl7::Poke32(header + 4, 0);
			ppl7::Poke32(header + 8, 0);
			ppl7::Poke8(header + 12, 0);
			ff.write(header, 13);
			ff.close();
		} catch (...) {
			ff.close();
			ppl7::File::remove(filename);
			throw;
		}
	}
	ff.open(filename, ppl7::File::READWRITE);
	ff.setMapReadAhead(1024 * 1024 * 10);
	// Header einlesen und prüfen

	const char* header=(const char*)ff.map(0, 32);
	if (strncmp(header, "PFP-File", 8) != 0) {
		throw InvalidDatabaseFileException(ppl7::String("Header not found [%s]"), (const char*)filename);
	}
	if (ppl7::Peek8(header + 8) != 3) {
		throw InvalidDatabaseFileException(ppl7::String("Invalid PPLibFile version [%d!=3]"), ppl7::Peek8(header + 8));
	}
	if ((pos=ppl7::Peek8(header + 9)) < 32) {
		throw InvalidDatabaseFileException(ppl7::String("Unexpected offset [%u]"), pos);
	}
	if (strncmp(header + 10, "PFWM", 4) != 0) {
		throw InvalidDatabaseFileException(ppl7::String("Invalid Filetype"));
	}
	subversion=ppl7::Peek8(header + 14);
	version=ppl7::Peek8(header + 15);
	if (version != 3 || subversion != 0) {
		throw InvalidDatabaseFileException(ppl7::String("Invalid WinMusik version [%d.%d != 3.0]"), version, subversion);
	}
	if (ppl7::Peek8(header + 16) != 0) {
		throw InvalidDatabaseFileException(ppl7::String("Compression not supported"));
	}
	first=pos;
	timestamp=ppl7::Peek32(header + 20);
	lastchange=ppl7::Peek32(header + 24);
	eof=ppl7::Peek32(header + 28);
}

bool CWMFile::GetNextChunk(CWMFileChunk& chunk)
{
	chunk.Clear();
	if (!pos) {
		throw DatabaseFileNotOpenException();
	}
	if (pos > eof) {
		throw DatabaseFileCorruptException(ppl7::ToString("read position behind end of file [%u>%u]", pos, eof));
	}
	// Header des nächsten Chunks einlesen
	const char* ptr=ff.map(pos, 17);
	if (pos == eof || strncmp(ptr, "ENDF", 4) == 0) {
		return false;
	}
	chunk.filepos=pos;
	memcpy(chunk.chunkname, ptr, 4);
	chunk.size=ppl7::Peek32(ptr + 4);
	chunk.datasize=chunk.size - 17;
	chunk.timestamp=ppl7::Peek32(ptr + 8);
	chunk.version=ppl7::Peek32(ptr + 12);
	chunk.formatversion=ppl7::Peek8(ptr + 16);
	chunk.data=ff.map(pos + 17, chunk.size - 17);
	pos+=chunk.size;
	return true;
}

void CWMFile::GetChunk(CWMFileChunk& chunk, uint32_t filepos)
{
	if (filepos == 0) {
		throw ppl7::InvalidArgumentsException();
	}
	ff.seek(filepos);
	const char* ptr=ff.map(filepos, 17);
	char tmp[5];
	strncpy(tmp, ptr, 4);
	tmp[4]=0;
	if (!IsValidChunkName(tmp)) {
		throw InvalidDatabaseChunkException(ppl7::ToString("Database contains unknown chunks[%s]"), tmp);
	}
	// Daten einlesen
	chunk.filepos=filepos;
	memcpy(chunk.chunkname, ptr, 4);
	chunk.size=ppl7::Peek32(ptr + 4);
	chunk.datasize=chunk.size - 17;
	chunk.timestamp=ppl7::Peek32(ptr + 8);
	chunk.version=ppl7::Peek32(ptr + 12);
	chunk.formatversion=ppl7::Peek8(ptr + 16);
	chunk.data=ff.map(filepos + 17, chunk.size - 17);
}



bool CWMFile::IsValidChunkName(const char* name)
{
	if (name != NULL && strlen(name) == 4) {
		if (strcmp(name, "TITL") == 0) return true;
		if (strcmp(name, "VERS") == 0) return true;
		if (strcmp(name, "DEVI") == 0) return true;
		if (strcmp(name, "LABL") == 0) return true;
		if (strcmp(name, "RSRC") == 0) return true;
		if (strcmp(name, "GENR") == 0) return true;
		if (strcmp(name, "RDEV") == 0) return true;
		if (strcmp(name, "TRAK") == 0) return true;
		if (strcmp(name, "PCHS") == 0) return true;
		if (strcmp(name, "SHRT") == 0) return true;
		if (strcmp(name, "OIMP") == 0) return true;
	}
	return false;
}

void CWMFile::SaveChunk(CWMFileChunk& chunk)
{
	char timestamp[4];
	char header[18];
	if (!pos) {
		throw DatabaseFileNotOpenException();
	}
	if (!IsValidChunkName(chunk.chunkname)) {
		throw InvalidDatabaseChunkException(ppl7::ToString("unknown chunk type [%s]"), chunk.chunkname);
	}
	if (chunk.filepos) {
		// Haben wir genug Platz, um den alten Chunk zu überschreiben?
		if (ff.read(header, 17, chunk.filepos) != 17) throw CouldNotReadDatabaseFileException();
		uint32_t oldsize=ppl7::Peek32(header + 4);
		uint32_t oldversion=ppl7::Peek32(header + 12);
		if (chunk.version != oldversion) {
			throw DatabaseModifiedException(ppl7::ToString("Database record is newer than the loaded record [%u>%u]"), chunk.version, oldversion);
		}
		if (oldsize >= (chunk.datasize + 17)) {
			ff.unmap();
			chunk.version++;
			chunk.timestamp=(uint32_t)ppl7::GetTime();
			memcpy(header, chunk.chunkname, 4);
			ppl7::Poke32(header + 4, oldsize);
			ppl7::Poke32(header + 8, chunk.timestamp);
			ppl7::Poke32(header + 12, chunk.version);
			ppl7::Poke8(header + 16, chunk.formatversion);
			ff.write(header, 17, chunk.filepos);
			ff.write(chunk.data, chunk.datasize, chunk.filepos + 17);
			ppl7::Poke32(timestamp, (uint32_t)ppl7::GetTime());
			ff.write(timestamp, 4, 24);
			return;
		}
		// Nein, wir kennzeichnen den alten Datensatz als gelöscht
		ff.unmap();
		memcpy(header, "FREE", 4);
		ff.write(header, 4, chunk.filepos);
	}
	chunk.filepos=eof;
	chunk.size=chunk.datasize + 17;
	chunk.timestamp=(uint32_t)ppl7::GetTime();
	chunk.version=1;
	memcpy(header, chunk.chunkname, 4);
	ppl7::Poke32(header + 4, chunk.size);
	ppl7::Poke32(header + 8, chunk.timestamp);
	ppl7::Poke32(header + 12, chunk.version);
	ppl7::Poke8(header + 16, chunk.formatversion);
	ff.write(header, 17, eof);
	ff.write(chunk.data, chunk.datasize, eof + 17);

	// Neuen ENDF-Chunk schreiben
	eof=eof + 17 + chunk.datasize;
	pos=eof;
	memcpy(header, "ENDF", 4);
	ppl7::Poke32(header + 4, 0);
	ppl7::Poke32(header + 8, 0);
	ppl7::Poke32(header + 12, 0);
	ppl7::Poke8(header + 16, 0);
	ff.write(header, 17, eof);

	// Position des ENDF-Chunks in den Header schreiben
	ppl7::Poke32(header, eof);
	ff.write(header, 4, 28);

	// Timestamp in den Header schreiben
	ppl7::Poke32(header, (uint32_t)ppl7::GetTime());
	ff.write(header, 4, 24);
}

void CWMFile::DeleteChunk(CWMFileChunk& chunk)
{
	char header[18];
	if (!pos) {
		throw DatabaseFileNotOpenException();
	}
	if (!IsValidChunkName(chunk.chunkname)) {
		throw InvalidDatabaseChunkException(ppl7::ToString("unknown chunk type [%s]"), chunk.chunkname);
	}
	if (chunk.filepos) {
		ff.setMapReadAhead(0);
		ff.read(header, 4, chunk.filepos);
		// Wir kennzeichnen den alten Datensatz als gelöscht
		memcpy(header, "FREE", 4);
		ff.write(header, 4, chunk.filepos);
		//ff.Unmap();
	}
	chunk.filepos=0;
	chunk.size=0;
	chunk.timestamp=0;
	chunk.version=0;
	// Timestamp in den Header schreiben
	ppl7::Poke32(header, (uint32_t)ppl7::GetTime());
	ff.write(header, 4, 24);
}

void CWMFile::Reset()
{
	pos=first;
}

void CWMFile::ListChunks()
{
	if (!pos) {
		throw DatabaseFileNotOpenException();
	}
	char name[5];
	const char* header;
	pos=first;
	while (pos < ff.size()) {
		header=ff.map(pos, 17);
		if (!header) return;
		strncpy(name, header, 4);
		name[4]=0;
		printf("%s: %i Bytes, Timestamp: %u, Version: %u, Format: %u\n", name, ppl7::Peek32(header + 4),
			ppl7::Peek32(header + 8), ppl7::Peek32(header + 12), ppl7::Peek8(header + 16));
		if (strncmp(header, "ENDF", 4) == 0) break;
		pos+=ppl7::Peek32(header + 4);
	}
}

uint32_t CWMFile::GetFileSize() const
{
	return (uint32_t)ff.size();
}
uint32_t CWMFile::GetFilePosition() const
{
	return pos;
}

int CWMFile::CopyDatabase(CWMFile& oldfile, CWMFile& newfile)
{
	oldfile.Reset();
	CWMFileChunk chunk;
	while (oldfile.GetNextChunk(chunk)) {
		chunk.filepos=0;
		if (strncmp(chunk.chunkname, "FREE", 4) != 0) {
			if (strncmp(chunk.chunkname, "ENDF", 4) != 0) {
				newfile.SaveChunk(chunk);
			}
		}
	}
	return 1;
}

}
}
}	// EOF Namespace de.pfp.winmusik
