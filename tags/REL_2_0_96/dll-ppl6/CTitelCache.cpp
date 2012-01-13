//#define DEBUGLOG

#include <windows.h>
#include <ppl6.h>
#include "winmusik.h"

CTitelCache::CTitelCache(ppldd size)
{
	if (size) startsize=size;
	else startsize=1024*1024*4;
	first=last=NULL;
}

CTitelCache::~CTitelCache()
{
	MemSpace.Clear();
}

void CTitelCache::Clear()
{
	MemSpace.Clear();
	first=last=NULL;
}

void CTitelCache::SetStartBufferSize(ppldd size)
{
	startsize=size;
}

void *CTitelCache::Malloc(ppldd size)
{
	void *adr;
	void *mem;
	MEMBLOCK *block;
	if (!first) {		// Erster Aufruf
		mem=MemSpace.Malloc(startsize);
		if (!mem) return NULL;
		block=(MEMBLOCK*)mem;
		block->allocated=sizeof(MEMBLOCK);
		block->start=(char*)block+block->allocated;
		block->freestart=block->start;
		block->size=startsize;
		block->free=block->size-block->allocated;
		block->previous=NULL;
		block->next=NULL;
		first=last=block;
	}
	// Freien Block finden
	block=first;
	while (block) {
		if (block->free>=size) {		// Passt
			block->free-=size;
			block->allocated+=size;
			adr=block->freestart;
			block->freestart+=size;
			return adr;
		}
		block=block->next;
	}
	// Nix mehr frei, wir brauchen einen neuen Block

	ppldd newsize=size;
	if (newsize<1024*1024) newsize=1024*1024;		// Mindestens 1 MB
	mem=MemSpace.Malloc(newsize);
	if (!mem) return NULL;
	block=(MEMBLOCK*)mem;
	block->allocated=sizeof(MEMBLOCK);
	block->start=(char*)block+block->allocated;
	block->freestart=block->start;
	block->size=newsize;
	block->free=block->size-block->allocated;
	block->previous=last;
	if (last) last->next=block;
	block->next=NULL;
	last=block;

	block->free-=size;
	block->allocated+=size;
	adr=block->freestart;
	block->freestart+=size;
	return adr;
}