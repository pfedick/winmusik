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
#include <ppl7.h>
#include "libwinmusik3.h"

namespace de {
namespace pfp {
namespace winmusik {

class CWMFileChunk
{
	friend class CWMFile;
	friend class CStorage;
	private:
		char		chunkname[5];
		ppluint32	filepos;
		ppluint32	size;
		ppluint32	timestamp;
		ppluint32	version;
		ppluint32	datasize;
		const char	*data;

	public:
		CWMFileChunk();
		~CWMFileChunk();
		void		Clear();
        ppluint32	GetChunkDataSize() const;
        const char	*GetChunkData() const;
        const char	*GetChunkName() const;
        int			SetChunkData(const char *chunkname, const char *data, ppluint32 size, ppluint32 oldfilepos=0, ppluint32 version=0);
        ppluint32	GetFilepos() const;
        ppluint32	GetTimestamp() const;
        ppluint32	GetVersion() const;
        ppl7::ByteArrayPtr GetData() const;
};

class CWMFile
{
	private:
		ppl7::File ff;
		ppluint8	version, subversion;
		ppluint32	timestamp, lastchange;
        ppluint32   pos, first, eof;

        void read_chunk(CWMFileChunk &chunk, ppluint32 filepos, const char *hdr);

	public:
		CWMFile();
		~CWMFile();
		bool IsOpen() const;
        void Open(const char *filename);
		void Close();
		void Reset();
        bool GetNextChunk(CWMFileChunk &chunk);
        void GetChunk(CWMFileChunk &chunk, ppluint32 filepos);
        void SaveChunk(CWMFileChunk &chunk);
        void DeleteChunk(const CWMFileChunk &chunk);
		void ListChunks();
		void EnableCompression(bool flag);
        bool IsValidChunkName(const char *name);
        ppluint32 GetFileSize() const;
        ppluint32 GetFilePosition() const;
        static void CopyDatabase(CWMFile &oldfile, CWMFile &newfile, CProgressUpdate *callback=NULL);
};


}}}	// EOF Namespace de.pfp.winmusik

#endif /* LIB_INCLUDE_CWMFILE_H_ */
