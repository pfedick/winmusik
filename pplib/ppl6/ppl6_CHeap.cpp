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
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif
#include <string.h>
#ifndef _WIN32
#include <strings.h>
#endif
/*
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif
*/

#include "ppl6.h"

namespace ppl6 {

typedef struct tagHeapElement {
	struct tagHeapElement	*next, *previous;
	void					*ptr;
} HEAPELEMENT;

typedef struct tagHeapBlock {
	struct tagHeapBlock	*previous, *next;
	void				*buffer;
	void				*bufferend;
	int					elements;
	int					num_free;
	HEAPELEMENT			*free;
	HEAPELEMENT			*elbuffer;

} HEAPBLOCK;


/*!\class CHeap
 * \ingroup PPLGroupMemory
 * \brief Speicherverwaltung in Heaps
 *
 */


CHeap::CHeap()
{
	IsInit=false;
	blocks=NULL;
	elementsize=0;
	increase=0;
	inccount=0;
	mem_allocated=sizeof(CHeap);
	mem_used=sizeof(CHeap);
}

CHeap::CHeap(size_t elementsize, int startnum, int increase, CString name)
{
	IsInit=false;
	blocks=NULL;
	elementsize=0;
	increase=0;
	inccount=0;
	mem_allocated=sizeof(CHeap);
	mem_used=sizeof(CHeap);
	Name=name;
	Init(elementsize, startnum, increase);
}

CHeap::~CHeap()
{
	Clear();
}

void CHeap::Clear()
{
	mutex.Lock();
	HEAPBLOCK *next, *bl=(HEAPBLOCK*)blocks;
	while (bl) {
		next=bl->next;
		free(bl->elbuffer);
		free(bl->buffer);
		free(bl);
		bl=next;
	}
	blocks=NULL;
	IsInit=false;
	mem_allocated=sizeof(CHeap);
	mem_used=sizeof(CHeap);
	mutex.Unlock();
}

void CHeap::Dump()
{
	mutex.Lock();
	HEAPBLOCK *bl=(HEAPBLOCK*)blocks;
	ppl_printf ("Dump CHeap [%s] (0x%tx, ",(const char*)Name,(ptrdiff_t) this);
	ppl_printf ("Elementsize: %i):\n", elementsize);
	ppl_printf ("Memory allocated: %zu Bytes, Memory used: %zu Bytes, Memory free: %zu Bytes\n",
			(size_t)mem_allocated, (size_t)mem_used, (size_t)(mem_allocated-mem_used));
	while (bl) {
		ppl_printf ("HEAPBLOCK: elements: %u, free: %u, Bytes allocated: %u\n",bl->elements, bl->num_free, bl->elements*elementsize);
		bl=bl->next;
	}
	mutex.Unlock();
}

int CHeap::Init(size_t elementsize, int startnum, int increase)
{
	mutex.Lock();
	if (IsInit) {
		SetError(254);
		mutex.Unlock();
		return 0;
	}
	//Elementsize auf 4 Byte aufrunden
	if (!elementsize) {
		mutex.Unlock();
		return 0;
	}
	elementsize=(elementsize+3)&0xfffffffc;

	this->elementsize=elementsize;
	this->increase=increase;
	if (!Increase(startnum)) {
		mutex.Unlock();
		return 0;
	}
	IsInit=true;
	mutex.Unlock();
	return 1;
}

int CHeap::Increase(int num)
{
	HEAPBLOCK *bl=(HEAPBLOCK*)malloc(sizeof(HEAPBLOCK));
	if (!bl) {
		SetError(2,1);
		return 0;
	}
	bl->elements=num;
	bl->next=NULL;
	bl->num_free=num;
	bl->previous=NULL;
	bl->buffer=malloc(elementsize*num);
	if (!bl->buffer) {
		free(bl);
		SetError(2,2);
		return 0;
	}
	bl->bufferend=(ppluint8*)bl->buffer+elementsize*num;
	bl->free=(HEAPELEMENT *)malloc(sizeof(HEAPELEMENT)*num);
	if (!bl->free) {
		free(bl->buffer);
		free(bl);
		SetError(2,3);
		return 0;
	}
	bl->elbuffer=bl->free;
	HEAPELEMENT *t,*prev=NULL;
	ppluint8 *buffer=(ppluint8*)bl->buffer;
	for(int i=0;i<num;i++) {
		t=&bl->free[i];
		t->previous=prev;
		t->next=&bl->free[i+1];
		t->ptr=buffer;
		buffer+=elementsize;
		prev=t;
	}
	bl->free[num-1].next=NULL;
	bl->next=(HEAPBLOCK*)blocks;
	if (bl->next) bl->next->previous=bl;
	blocks=bl;
	inccount++;
	increase+=(increase*30/100);
	mem_allocated+=sizeof(HEAPBLOCK)+elementsize*num+sizeof(HEAPELEMENT)*num;
	mem_used+=sizeof(HEAPBLOCK);
	return 1;
}

void *CHeap::Calloc()
{
	void *block=Malloc();
	if (!block) return NULL;
	memset(block,0,elementsize);
	return block;
}

void *CHeap::Malloc()
{
	mutex.Lock();
	if (!IsInit) {
		SetError(255);
		mutex.Unlock();
		return NULL;
	}
	tryagain:
	// Den nächsten freien Block suchen
	HEAPBLOCK *bl=(HEAPBLOCK*)blocks;
	while (bl) {
		if (bl->num_free) {
			HEAPELEMENT *el=bl->free;
			// Element aus der Free-Kette nehmen
			bl->free=bl->free->next;
			if(bl->free) bl->free->previous=NULL;
			bl->num_free--;
			mem_used+=elementsize+sizeof(HEAPELEMENT);
			mutex.Unlock();
			return el->ptr;
		}
		bl=bl->next;
	}
	// Speicher muss vergroessert werden
	if (Increase(increase)) goto tryagain;
	mutex.Unlock();
	return NULL;
}

int CHeap::Free(void *mem)
{
	mutex.Lock();
	if (!IsInit) {
		mutex.Unlock();
		SetError(255);
		return 0;
	}
	HEAPBLOCK *bl=(HEAPBLOCK*)blocks;
	while (bl) {
		if (mem>=bl->buffer && mem<=bl->bufferend) {
			// Nummer des Blocks errechnen
			HEAPELEMENT *el=bl->elbuffer;
			int element=(ppluint32)((ppluint8*)mem-(ppluint8*)bl->buffer)/elementsize;
			if(el[element].ptr!=mem) {
				// Hier stimmt was nicht!!!!
				SetError(256);
				mutex.Unlock();
				return 0;
			}
			// Element in die Free-Kette hängen
			el[element].next=bl->free;
			el[element].previous=NULL;
			//VALGRIND_HG_CLEAN_MEMORY(el[element].ptr,elementsize);
			if (bl->free) bl->free->previous=&el[element];
			bl->free=&el[element];
			bl->num_free++;
			mem_used-=(elementsize+sizeof(HEAPELEMENT));
			if (bl->num_free==bl->elements) Cleanup();
			mutex.Unlock();
			return 1;
		}
		bl=bl->next;
	}

	mutex.Unlock();
	return 0;
}

void CHeap::Cleanup()
{
	// Wenn mehr als ein Block komplett leer ist, geben wir ihn frei
	HEAPBLOCK *next, *bl=(HEAPBLOCK*)blocks;
	bool flag=false;
	while (bl) {
		next=bl->next;
		if (bl->num_free==bl->elements) {
			if (flag) {
				// Block wird gelöscht
				if (bl->previous) bl->previous->next=bl->next;
				if (bl->next) bl->next->previous=bl->previous;
				mem_allocated-=sizeof(HEAPBLOCK)+elementsize*bl->elements+sizeof(HEAPELEMENT)*bl->elements;
				mem_used-=sizeof(HEAPBLOCK);
				free(bl->buffer);
				free(bl->elbuffer);
				free(bl);
			}
			flag=true;
		}
		bl=next;
	}

}



}	// EOF namespace ppl6
