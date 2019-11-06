/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "prolog_ppl7.h"
#include "ppl7.h"


namespace ppl7 {

class ResourceChunk
{
		friend class Resource;
	private:
		ResourceChunk *next;
		int		id;
		String	name;
		uint32_t	size_u;
		uint32_t	size_c;
		int			compression;
		void	*data;
		const void	*uncompressed;
		int			clearbuffer;

	public:
		ResourceChunk();
		~ResourceChunk();
};

ResourceChunk::ResourceChunk()
{
	next=NULL;
	id=0;
	size_u=0;
	size_c=0;
	compression=0;
	data=NULL;
	uncompressed=NULL;
	clearbuffer=0;
}

ResourceChunk::~ResourceChunk()
{
	if(clearbuffer) free(data);
}

Resource::Resource()
{
	firstchunk=NULL;
	count=0;
	maxid=minid=0;
	major=0;
	minor=0;
}

Resource::~Resource()
{
	clear();
}

void Resource::clear()
{
	ResourceChunk *next=static_cast<ResourceChunk *>(firstchunk);
	while (next) {
		ResourceChunk *res=next;
		next=next->next;
		delete (res);
	}
	memory.free();
	memref.use(NULL,0);
	firstchunk=NULL;
	minid=0;
	maxid=0;
	count=0;
}


void Resource::load(const String &filename)
{
	File ff;
	ff.open(filename,File::READ);
	load(ff);
}

void Resource::load(FileObject &file)
{
	clear();
	size_t size=(size_t)file.size();
	void *buffer=memory.malloc(size);
	if (!buffer) throw OutOfMemoryException();
	try {
		if (file.read(buffer,size,0)!=size) {
			memory.free();
			throw FailedToLoadResourceException();
		}
		checkResource(memory);
		memref=memory;
		parse();
	} catch (...) {
		clear();
		throw;
	}
}

void Resource::load(const ByteArrayPtr &memory)
{
	clear();
	checkResource(memory);
	this->memory.copy(memory);
	memref=this->memory;
	parse();
}

void Resource::useMemory(const ByteArrayPtr &memory)
{
	clear();
	checkResource(memory);
	this->memref=memory;
	parse();
}

void Resource::useMemory(void *ptr, size_t size)
{
	clear();
	memref.use(ptr,size);
	checkResource(memref);
	parse();
}

void Resource::checkResource(const ByteArrayPtr &memory)
{
	if (memory.size()<9) throw InvalidResourceException();
	// Die ersten 9 Bytes müssen folgende Werte haben:
	// 0x50,0x50,0x4c,0x52,0x45,0x53,0x00,0x06,0x00
	if (memory[(size_t)0]!=0x50) throw InvalidResourceException();
	if (memory[(size_t)1]!=0x50) throw InvalidResourceException();
	if (memory[(size_t)2]!=0x4c) throw InvalidResourceException();
	if (memory[(size_t)3]!=0x52) throw InvalidResourceException();
	if (memory[(size_t)4]!=0x45) throw InvalidResourceException();
	if (memory[(size_t)5]!=0x53) throw InvalidResourceException();
	if (memory[(size_t)6]!=0x00) throw InvalidResourceException();
	if (memory[(size_t)7]!=0x06) throw InvalidResourceException();
	if (memory[(size_t)8]!=0x00) throw InvalidResourceException();
}

void Resource::parse()
{
	const char *b=(const char*)memref.adr();
	// Welche Version haben wir?
	major=Peek8(b+7);
	minor=Peek8(b+8);
	b=b+9;
	maxid=0;
	count=0;
	minid=999999;
	int size=Peek32(b);
	while (size) {
		count++;
		b=b+size;
		size=Peek32(b);
	}
	b=(const char*)memref.adr()+9;
	size=Peek32(b);
	ResourceChunk *lastres=NULL;
	while (size) {
		int id=Peek16(b+4);
		if(id<minid) minid=id;
		if(id>maxid) maxid=id;
		int size_u=Peek32(b+6);
		int size_c=Peek32(b+10);
		int type=Peek8(b+14);
		int off=Peek8(b+15);
		const char *name=b+16;
		ResourceChunk *res=new ResourceChunk();
		if (!res) throw OutOfMemoryException();
		if (!firstchunk) firstchunk=res;
		if(lastres) lastres->next=res;
		res->id=id;
		res->name.set(name);
		res->data=NULL;
		res->size_c=size_c;
		res->size_u=size_u;
		res->compression=type;
		res->uncompressed=b+off;
		res->clearbuffer=0;
		res->next=NULL;
		if(type==0) res->data=(void*)res->uncompressed;	// Unkomprimiert
		lastres=res;
		b=b+size;
		size=Peek32(b);
	}
}

void Resource::list()
{
	ResourceChunk *res=static_cast<ResourceChunk *>(firstchunk);
	printf ("List of Ressources\n");
	if (!res) {
		printf ("  no entries\n");
		return;
	}
	while (res) {
		printf ("   - ID: %i, Name: %s, Size: %u\n",res->id, (const char*)res->name,res->size_u);
		res=res->next;
	}

}

void *Resource::find(int id)
{
	ResourceChunk *res=static_cast<ResourceChunk *>(firstchunk);
	if (!firstchunk) throw ResourceNotFoundException();
	while (res) {
		if (res->id==id) {
			if (!res->data) {
				uncompress(res);
			}
			return res;
		}
		res=res->next;
	}
	throw ResourceNotFoundException();
}

void *Resource::find(const String &name)
{
	ResourceChunk *res=static_cast<ResourceChunk *>(firstchunk);
	if (!firstchunk) throw ResourceNotFoundException();
	while (res) {
		if (res->name==name) {
			if (!res->data) {
				uncompress(res);
			}
			return res;
		}
		res=res->next;
	}
	throw ResourceNotFoundException();
}

ByteArrayPtr Resource::getMemory(int id)
{
	ByteArrayPtr m;
	ResourceChunk *res=static_cast<ResourceChunk *>(find(id));
	m.use(res->data,res->size_u);
	return m;
}

ByteArrayPtr Resource::getMemory(const String &name)
{
	ByteArrayPtr m;
	ResourceChunk *res=static_cast<ResourceChunk *>(find(name));
	m.use(res->data,res->size_u);
	return m;
}

FileObject *Resource::getFile(int id)
{
	ResourceChunk *res=static_cast<ResourceChunk *>(find(id));
	MemFile *ff=new MemFile();
	ff->open(res->data,res->size_u);
	ff->setFilename(res->name);
	return ff;
}

FileObject *Resource::getFile(const String &name)
{
	ResourceChunk *res=static_cast<ResourceChunk *>(find(name));
	MemFile *ff=new MemFile();
	ff->open(res->data,res->size_u);
	ff->setFilename(res->name);
	return ff;
}

void Resource::uncompress(void *resource)
{
	Compression comp;
	ResourceChunk *res=static_cast<ResourceChunk*>(resource);
	size_t bufferlen=res->size_u;
	void *buffer=malloc(bufferlen);
	if (!buffer) throw OutOfMemoryException();
	try {
		comp.uncompress(buffer, &bufferlen,res->uncompressed,res->size_c,(Compression::Algorithm)res->compression);
	} catch (...) {
		free(buffer);
		throw;
	}
	res->data=buffer;
	res->clearbuffer=1;
}

/*********************************************************************************
 * Resourcen generieren
 *********************************************************************************/

#ifdef DONE
static void includeHelp(FileObject &out, const String &configfile)
{
	DateTime now;
	now.setCurrentTime();
	out.putsf(
		"/*********************************************************\n"
		" * PPL7 Resourcen Generator Version %i.%i.%i\n"
		" * %s\n"
		"",PPL_VERSION_MAJOR,PPL_VERSION_MINOR,PPL_VERSION_BUILD,PPL_COPYRIGHT
	);

	out.putsf(
		" *\n"
		" * File generation: %s\n"
		" * Config: %s\n"
		" *********************************************************/\n\n",
		(const char*)now.get(), (const char*)configfile
	);
	out.puts(
		"/* File-Format:\n"
		" *    Byte 0-5: ID \"PPLRES\"                              (6 Byte)\n"
		" *    Byte 6:   0-Byte                                   (1 Byte)\n"
		" *    Byte 7:   Major Version (6)                        (1 Byte)\n"
		" *    Byte 8:   Minor Version (0)                        (1 Byte)\n"
		" *    Byte 9:   Start of Chunks\n"
		" *\n"
		" * Chunk-Format:\n"
		" *    Byte 0:  Size of Chunk incl. Header                (4 Byte)\n"
		" *    Byte 4:  ID of Resource                            (2 Byte)\n"
		" *    Byte 6:  Size of data uncompressed                 (4 Byte)\n"
		" *    Byte 10: Size of data compressed                   (4 Byte)\n"
		" *    Byte 14: Compression-Type (0=none,1=zlib,2=bzip2)  (1 Byte)\n"
		" *    Byte 15: Offset of Data from Chunk-Start           (1 Byte)\n"
		" *    Byte 16: Name of Resource with 0-Byte              (n Bytes)\n"
		" *    Byte n:  Data\n"
		" *\n"
		" * Chunks are repeated as often as required, followed by\n"
		" * a 4 Byte 0-Value (Chunk size 0), which marks the end of file\n"
		" */\n\n"
		);
}

static void bufferOut(FileObject &out, const char *buffer, int bytes)
{
	static int c=0;
	static char clear[25]="";
	for (int i=0;i<bytes;i++) {
		uint8_t byte=(uint8_t)buffer[i];
		out.putsf("0x%02x,",byte);
		if(byte>31 && byte<128 && byte!='\\' && byte!='/') clear[c]=byte;
		else clear[c]='.';
		c++;
		clear[c]=0;
		if (c>15) {
			c=0;
			out.putsf(" // %s\n    ",clear);
			clear[0]=0;
		}
	}
	if (!bytes) {
		for (int i=c;i<16;i++) {
			out.puts("     ");
		}
		c=0;
		out.putsf(" // %s\n    ",clear);
		clear[0]=0;
	}
}

static void output(FileObject &ff, int resid, const String &name, const String &filename, int size_u, char *buffer, int bytes, int compressiontype)
{
	char *buf=(char*)malloc(64);
	if (!buf) throw OutOfMemoryException();

	uint32_t chunksize=bytes+name.size()+17;
	Poke32(buf+0,chunksize);
	Poke16(buf+4,resid);
	Poke32(buf+6,size_u);
	Poke32(buf+10,bytes);
	Poke8(buf+14,compressiontype);
	Poke8(buf+15,17+name.size());
	bufferOut(ff,buf,16);
	bufferOut(ff,name,name.size()+1);
	//BufferOut(ff,filename,strlen(filename)+1);
	bufferOut(ff,buffer,bytes);
	free(buf);
}

static int compress(FileObject &ff, char **buffer, size_t *size, int *type)
{
	Compression comp;
	size_t size_u=(size_t)ff.size();
	char *source=(char*)ff.map();			// Komplette Datei mappen

	size_t size_c=size_u+2048;
	size_t size_zlib=size_c;
	size_t size_bz2=size_c;
	char *buf=(char*)malloc(size_c);
	if (!buf) throw OutOfMemoryException();
	// Zuerst Zlib
	comp.init(Compression::Algo_ZLIB,Compression::Level_High);
	comp.compress(buf,&size_zlib,source,size_u);
	// Dann Bzip2
	comp.init(Compression::Algo_BZIP2,Compression::Level_High);
	comp.compress(buf,&size_bz2,source,size_u);
	// Was war kleiner?
	if (size_u<=size_bz2 && size_u<=size_zlib) {
		free(buf);
		printf ("Using no compression: %u Bytes (Zlib: %u, BZ2: %u)\n",(uint32_t)size_u, (uint32_t)size_zlib, (uint32_t)size_bz2);
		*size=size_u;
		*type=0;
		*buffer=(char*)malloc(size_u);
		memcpy(*buffer,source,size_u);
		return 1;
	}
	if (size_bz2<size_zlib) {
		printf ("Using bzip2: %u Bytes von %u Bytes (zlib: %u Bytes)\n",(uint32_t)size_bz2,(uint32_t)size_u,(uint32_t)size_zlib);
		*buffer=buf;
		*size=size_bz2;
		*type=2;
		return 1;
	}
	size_zlib=size_c;
	comp.init(Compression::Algo_ZLIB,Compression::Level_High);
	comp.compress(buf,&size_zlib,source,size_u);
	printf ("Using zlib: %u Bytes von %u Bytes (bzip2: %u Bytes)\n",(uint32_t)size_zlib,(uint32_t)size_u,(uint32_t)size_bz2);
	*buffer=buf;
	*size=size_zlib;
	*type=1;
	return 1;
}

#endif

void Resource::generateResourceHeader(const String &basispfad, const String &configfile, const String &targetfile, const String &label)
{
	throw UnsupportedFeatureException("Resource::generateResourceHeader");
#ifdef DONE
	char section[12];
	if (configfile.isEmpty()) throw IllegalArgumentException();
	Config conf;
	if (!conf.Load(configfile)) {
		return 0;
	}
	if (conf.SelectSection("setup")) {
		if (!basispfad) basispfad=conf.Get("path");
		if (!targetfile) targetfile=conf.Get("targetfile");
		if (!label) label=conf.Get("label");
	}

	if ((!basispfad) || strlen(basispfad)==0) {
		SetError(194,"basispfad ist NULL oder leer");
		return 0;
	}
	if ((!targetfile) || strlen(targetfile)==0) {
		SetError(194,"targetfile ist NULL oder leer");
		return 0;
	}

	if ((!label) || strlen(label)==0) {
		SetError(194,"label ist NULL oder leer");
		return 0;
	}
	const char *prefix=conf.GetSection("prefix");
	const char *suffix=conf.GetSection("suffix");
	const char *path=basispfad;
	CFile out;
	if (!out.Open(targetfile,"wb")) {
		return 0;
	}
	printf ("Verarbeite Resourcen in: %s...\n",(const char*)configfile);
	if (prefix) out.Write((void*)prefix,strlen(prefix));
	IncludeHelp(&out, configfile);

	out.Puts("/**************************************************************\n");
	out.Puts(" * Resourcen:\n");
	out.Puts(" *\n");
	int havesection=conf.FirstSection();
	while(havesection) {
		sprintf(section,"%s",conf.GetSectionName());
		const char *id=conf.Get("ID");
		const char *name=conf.Get("Name");
		const char *filename=conf.Get("File");
		havesection=conf.NextSection();
		if (!filename) continue;
		if (strlen(filename)<2) continue;
		if (!id) continue;
		if (!name) continue;
		CFile ff;
		if (ff.Open("%s/%s","rb",path,filename)) {
			out.Putsf(" * %4i: %-20s (%s)\n",atoi(id),name,filename);
		}
	}
	out.Puts(" **************************************************************/\n\n");
	out.Putsf("static unsigned char %s []={\n    ",label);
	sprintf(section,"PPLRES");
	poke8(section+7,6);
	poke8(section+8,0);
	BufferOut(&out,section,9);

	havesection=conf.FirstSection();
	while(havesection) {
		sprintf(section,"%s",conf.GetSectionName());
		const char *id=conf.Get("ID");
		const char *name=conf.Get("Name");
		const char *filename=conf.Get("File");
		const char *compression=conf.Get("compression");
		havesection=conf.NextSection();
		if (!filename) continue;
		if (strlen(filename)<2) continue;
		if (!id) continue;
		if (!name) continue;
		CFile ff;
		if (!ff.Open("%s/%s","rb",path,filename)) {
			printf ("Konnte Resource %s nicht oeffnen\n",filename);
		} else {
			char *buffer=NULL;
			uint32_t size=0;
			int type=0;
			printf ("%s: ",filename);
			if (compression) {
				CString forcecomp=LCase(Trim(compression));
				if (forcecomp=="none") {
					buffer=ff.Load();
					printf ("Forced no compression: %u Bytes\n",(uint32_t)ff.Size());
					Output(&out,atoi(id),name,filename,(uint32_t)ff.Size(),buffer,(uint32_t)ff.Size(),0);
					free(buffer);
					continue;
				}
				printf ("Unbekannter Kompressionsalgorithmus für ID %s: >>%s<<\n",id,(const char*)forcecomp);
				return 0;
			}
			if (Compress(&ff,&buffer,&size,&type)) {
				Output(&out,atoi(id),name,filename,(uint32_t)ff.Size(),buffer,size,type);
				free(buffer);
			}
		}
	}
	poke32(section,0);
	BufferOut(&out,section,4);
	BufferOut(&out,NULL,0);
	out.Puts("0\n};\n");
	if (suffix) out.Write((void*)suffix,strlen(suffix));
#endif
}

Resource *Resource::getPPLResource()
{
	return GetPPLResource();
}


}	// EOF namespace ppl7
