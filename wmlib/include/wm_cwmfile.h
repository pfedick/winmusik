/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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


#ifndef WM_CWMFILE_H
#define WM_CWMFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <ppl7.h>

namespace de {
namespace pfp {
namespace winmusik {


class CWMFileChunk
{
	friend class CWMFile;
	//friend class CStorage;
private:
	const char* data;
	uint32_t	filepos;
	uint32_t	size;
	uint32_t	timestamp;
	uint32_t	version;
	uint32_t	datasize;
	uint8_t		formatversion;
	char		chunkname[5];

public:
	CWMFileChunk();
	~CWMFileChunk();
	void		Clear();
	uint32_t	GetChunkDataSize() const;
	const char* GetChunkData() const;
	const char* GetChunkName() const;
	uint32_t	GetChunkId() const;
	int			SetChunkData(const char* chunkname, const char* data, uint32_t size, uint32_t oldfilepos=0, uint32_t version=0, uint8_t formatversion=1);
	void		SetFormatVersion(uint8_t v);
	uint32_t	GetFilepos() const;
	uint32_t	GetTimestamp() const;
	uint32_t	GetVersion() const;
	uint8_t		GetFormatVersion() const;
	void		HexDump(ppl7::Logger* log=nullptr) const;
};

class CWMFile
{
private:
	ppl7::File ff;
	uint8_t	version, subversion;
	uint32_t	timestamp, lastchange;
	uint32_t	pos, first, eof;
	bool		enableCompression;

public:
	CWMFile();
	~CWMFile();
	bool IsOpen() const;
	void Open(const char* filename);
	void Close();
	void Reset();
	bool GetNextChunk(CWMFileChunk& chunk);
	void GetChunk(CWMFileChunk& chunk, uint32_t filepos);
	void SaveChunk(CWMFileChunk& chunk);
	void DeleteChunk(CWMFileChunk& chunk);
	void ListChunks();
	void EnableCompression(bool flag);
	static bool IsValidChunkName(const char* name);
	uint32_t GetFileSize() const;
	uint32_t GetFilePosition() const;
	static int CopyDatabase(CWMFile& oldfile, CWMFile& newfile);
};



}
}
}	// EOF Namespace de.pfp.winmusik

#endif /* WM_CWMFILE_H */
