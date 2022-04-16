/*
 * cwmfile.h
 *
 *  Created on: 20.06.2016
 *      Author: patrick
 */

#ifndef LIB_INCLUDE_CWMFILE_H_
#define LIB_INCLUDE_CWMFILE_H_

#include <stdlib.h>
#include <stdio.h>
#include "libwinmusik3.h"
#include "ppl7.h"

namespace de {
namespace pfp {
namespace winmusik {

class CWMFileChunk
{
	friend class CWMFile;
	friend class CStorage;
	private:
		char		chunkname[5];
		uint32_t	filepos;
		uint32_t	size;
		uint32_t	timestamp;
		uint32_t	version;
		uint32_t	datasize;
		const char	*data;

	public:
		CWMFileChunk();
		~CWMFileChunk();
		void		Clear();
		uint32_t	GetChunkDataSize() const;
        const char	*GetChunkData() const;
        const char	*GetChunkName() const;
        int			SetChunkData(const char *chunkname, const char *data, uint32_t size, uint32_t oldfilepos=0, uint32_t version=0);
        uint32_t	GetFilepos() const;
        uint32_t	GetTimestamp() const;
        uint32_t	GetVersion() const;
        ppl7::ByteArrayPtr GetData() const;
};

class CWMFile
{
	private:
		ppl7::File ff;
		uint8_t		version, subversion;
		uint32_t	timestamp, lastchange;
		uint32_t   pos, first, eof;

        void read_chunk(CWMFileChunk &chunk, uint32_t filepos, const char *hdr);

	public:
		CWMFile();
		~CWMFile();
		bool IsOpen() const;
        void Open(const char *filename);
		void Close();
		void Reset();
        bool GetNextChunk(CWMFileChunk &chunk);
        void GetChunk(CWMFileChunk &chunk, uint32_t filepos);
        void SaveChunk(CWMFileChunk &chunk);
        void DeleteChunk(const CWMFileChunk &chunk);
		void ListChunks();
		void EnableCompression(bool flag);
        bool IsValidChunkName(const char *name);
        uint32_t GetFileSize() const;
        uint32_t GetFilePosition() const;
        static void CopyDatabase(CWMFile &oldfile, CWMFile &newfile, CProgressUpdate *callback=NULL);
};


}}}	// EOF Namespace de.pfp.winmusik

#endif /* LIB_INCLUDE_CWMFILE_H_ */
