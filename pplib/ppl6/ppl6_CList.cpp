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
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif

//#define DEBUGLOG
//#define _USE_PPL_MALLOC

#include "ppl6.h"

namespace ppl6 {

/*!\brief Struktur für CPointerList
 * */
typedef struct tagListEntry {
	struct tagListEntry *next, *last;
	void *data;
	bool malloced;
} LISTENTRY;


/*! \class CListItem
 * \ingroup PPLGroupDataTypes
 * \desc
 * Diese Klasse dient als Basisklasse für beliebige ListenElemente, die über die Klasse
 * ppl6::CList verwaltet werden.
 */

/*!\brief Konstruktor
 *
 * \desc
 * Im Konstruktor wird das Listenelement initialisiert und die internen Pointer auf NULL
 * gesetzt
 */
CListItem::CListItem()
{
	//printf ("CListItem::CListItem() 0x%x\n",this);
	previous=next=NULL;
	list=NULL;
}

/*!\brief Destruktor
 *
 * \desc
 * Im Destruktor wird geprüft, ob das Element noch Teil einer Liste ist. Ist dies deer Fall,
 * wird die Delete-Funktion der Liste aufgerufen, um das Element aus der Liste zu entfernen
 */
CListItem::~CListItem()
{
	//printf ("CListItem::~CListItem() 0x%x, list=0x%x\n",this,list);
	if (list) list->Delete(this);
}


/*! \class CList
 * \ingroup PPLGroupDataTypes
 * \desc
 * Diese Klasse kann zur Verwaltung beliebiger Elemente in einer Liste verwendet werden. Die
 * Elemente müssen dabei von der Basisklasse ppl6::CListItem abgeleitet sein.
 *
 * \attention Die Klasse verwaltet keinen eigenen Mutex und ist somit nicht Thread-sicher.
 * Die Aufrufende Anwendung muss selbst sicherstellen, dass die Klasse nicht gleichzeitig
 * von mehreren Threads verwendet wird.
 */

/*!\brief Konstruktor
 *
 * \desc
 * Im Konstruktor wird die Liste mit NULL initialisiert.
 */
CList::CList()
{
	first=last=pointer=NULL;
	counter=0;
}

/*!\brief Destruktor
 *
 * \desc
 * Der Destruktor ruft die CList::Clear Funktion auf.
 */
CList::~CList()
{
	Clear();
}

/*!\brief Liste löschen
 *
 * \desc
 * Mit dieser Funktion wird die Liste gelöscht.
 *
 *\param deleteitems Wird der optionale Parameter auf "true" gesetzt, wird der Destruktur
 * jedes Listenelements aufgerufen.
 */
void CList::Clear(bool deleteitems)
{
	if (deleteitems) {
		while (first) {
			delete first;
		}
	}
	first=last=pointer=NULL;
	counter=0;
}

/*!\brief Element in die Liste hinzufügen
 *
 * \desc
 * Mit dieser Funktion werden Elemente an das Ende der Liste hinzugefügt
 *
 * \param item Ist ein Pointer auf eine von CListItem abgeleitete Klasse. Diese darf noch nicht
 * Teil einer anderen Liste sein.
 *
 * \return Wurde das Element erfolgreich in die Liste eingefügt, liefert die Funktion true (1)
 * zurück. Im Fehlerfall wird false (0) zurückgegeben und der Fehler kann über die
 * Fehlerfunktionen ausgelesen werden.
 */
int CList::Add(CListItem *item)
{
	if (!item) {
		SetError(194,"int CList::Add(==> CListItem *item <==)");
		return 0;
	}
	if (item->list) {
		SetError(413);
		return 0;
	}
	item->list=this;
	item->next=NULL;
	item->previous=last;
	if (last) {
		last->next=item;
		last=item;
	} else {
		first=last=item;
	}
	counter++;
	return 1;
}

/*!\brief Element aus der Liste entfernen
 *
 * \desc
 * Mit dieser Funktion werden Elemente aus der Liste entfernt. Dabei ist zu beachten,
 * dass nur das Element aus der Liste gelöscht wird, nicht aber das Element selbst. Um
 * ein Element zu löschen, genügt es das Element mit "delete" zu löschen. Der Konstruktor
 * des Elements sorgt dafür, dass es auch aus der Liste entfernt wird.
 *
 * \param item Ist ein Pointer auf eine von CListItem abgeleitete Klasse. Diese darf noch nicht
 * Teil einer anderen Liste sein.
 *
 * \return Wurde das Element erfolgreich in die Liste eingefügt, liefert die Funktion true (1)
 * zurück. Im Fehlerfall wird false (0) zurückgegeben und der Fehler kann über die
 * Fehlerfunktionen ausgelesen werden.
 */
int CList::Delete(CListItem *item)
{
	if (!item) {
		SetError(194,"int CList::Delete(==> CListItem *item <==)");
		return 0;
	}
	if (item->list!=this) {
		SetError(414);
		return 0;
	}
	if (item->previous) item->previous->next=item->next;
	if (item->next) item->next->previous=item->previous;
	if (item==first) first=item->next;
	if (item==last) last=item->previous;
	item->next=item->previous=NULL;
	item->list=NULL;
	pointer=NULL;
	counter--;
	if (counter<0) counter=0;
	return 1;
}

/*!\brief Internen Pointer für Walk-Funktionen zurücksetzen
 *
 * \desc
 * Mit dieser Funktion wird der interne Pointer zurückgesetzt, der für die Walk-Funktionen GetNext und
 * GetPrevious verwendet wird.
 */
void CList::Reset()
{
	pointer=NULL;
}

/*!\brief Erstes Element aus der Liste
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das erste Element der Liste zurückgeliefert.
 *
 * \return Pointer auf das erste Element der Liste oder NULL, wenn die Liste leer ist
 */
CListItem *CList::GetFirst()
{
	pointer=NULL;
	return GetNext();
}

/*!\brief Nächstes Element aus der Liste
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das nächste Element der Liste zurückgeliefert.
 * Somit kann eine Liste vorwärts durchwandert werden.
 *
 * \return Pointer auf das nächste Element der Liste oder NULL, wenn keine weiteren
 * Elemente vorhanden sind. In diesesm Fall wird ausserdem der Fehlercode 415 gesetzt.
 */
CListItem *CList::GetNext()
{
	CListItem *item;
	if (pointer==(CListItem *)1) {			// Am Ende der Liste?
		SetError(415);
		return NULL;
	}
	if (!pointer) pointer=first;
	item=pointer;
	if (pointer) {
		pointer=pointer->next;
		if (!pointer) pointer=(CListItem *)1;
	}
	if (!item) SetError(415);
	return item;
}

/*!\brief Letztes Element aus der Liste
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das letzte Element der Liste zurückgeliefert.
 *
 * \return Pointer auf das letzte Element der Liste oder NULL, wenn die Liste leer ist
 */
CListItem *CList::GetLast()
{
	pointer=NULL;
	return GetPrevious();
}

/*!\brief Vorheriges Element aus der Liste
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das vorherige Element der Liste zurückgeliefert.
 * Somit kann eine Liste rückwärts durchwandert werden.
 *
 * \return Pointer auf das vorherige Element der Liste oder NULL, wenn keine weiteren
 * Elemente vorhanden sind. In diesesm Fall wird ausserdem der Fehlercode 415 gesetzt.
 */
CListItem *CList::GetPrevious()
{
	CListItem *item;
	if (pointer==(CListItem *)1) {			// Am Ende der Liste?
		SetError(415);
		return NULL;
	}
	if (!pointer) pointer=last;
	item=pointer;
	if (pointer) {
		pointer=pointer->previous;
		if (!pointer) pointer=(CListItem *)1;
	}
	if (!item) SetError(415);
	return item;
}

/*!\brief Anzahl Elemente in der Liste
 *
 * \desc
 * Diese Funktion liefert die Anzahl Elemente in der Liste zurück.
 *
 * \return Anzahl Elemente oder 0, wenn die Liste leer ist. Bei einer leeren Liste wird
 * ausserdem der Fehlercode 416 gesetzt.
 */
int CList::Num() const
{
	if (!counter) SetError(416);
	return counter;
}

/*!\brief Konsistenz der Liste prüfen
 *
 * \desc
 * Diese Funktion prüft, ob die Verkettung der einzelnen Listenelemente in Ordnung ist
 * und es keine Loops gibt. Dabei wird die Liste sowohl vorwärts als auch rückwärts durchwandert.
 * Ausserdem wird geprüft, ob auch tatsächlich jedes Element dieser Liste angehört.
 *
 *\return Wenn die Liste fehlerfrei ist, wird true (1) zurückgegeben, sonst false (0) und
 * der Fehlercode 417 mit einem erklärenden Text gesetzt.
 */
int CList::CheckConsistency()
{
	if (first==NULL && last==NULL) return 1;
	if (first==NULL) {
		SetError(417,"'last' zeigt auf ein Element obwohl 'first' NULL ist");
		return 0;
	}
	if (last==NULL) {
		SetError(417,"'first' zeigt auf ein Element obwohl 'last' NULL ist");
		return 0;
	}
	int num=0;
	CListItem *item;

	// Vorwärtsliste prüfen
	item=first;
	while (item) {
		if (item->list!=this) {
			SetError(417,"Ein Element gehört nicht dieser Liste an");
			return 0;
		}
		num++;
		if (num>counter) {
			SetError(417,"Liste enthält in der Vorwärtskette mehr Elemente als es sein dürften, möglicherweise existiert ein Loop");
			return 0;
		}
		if (item==last && item->next!=NULL) {
			SetError(417,"'last'-Pointer zeigt nicht auf das letzte Element der Liste");
			return 0;
		}
		item=item->next;
	}
	// Rückwärtsliste prüfen
	item=last;
	num=0;
	while (item) {
		if (item->list!=this) {
			SetError(417,"Ein Element gehört nicht dieser Liste an");
			return 0;
		}
		num++;
		if (num>counter) {
			SetError(417,"Liste enthält in der Rückwärtskette mehr Elemente als es sein dürften, möglicherweise existiert ein Loop");
			return 0;
		}
		if (item==first && item->previous!=NULL) {
			SetError(417,"'first'-Pointer zeigt nicht auf das erste Element der Liste");
			return 0;
		}
		item=item->previous;
	}
	return 1;
}

/***********************************************************************************
 * CPointerList
 ***********************************************************************************/

/*!\class CGenericList
 * \ingroup PPLGroupDataTypes
 */

CGenericList::CGenericList()
{
	first=last=pointer=NULL;
	counter=0;
	DestroyFunction=NULL;
	DestroyData=NULL;
}

CGenericList::~CGenericList()
{
	Clear();
}

void CGenericList::Clear(bool deleteitems)
{
	LISTENTRY *entry;
	while (first) {
		entry=(LISTENTRY *)first;
		first=entry->next;
		if (entry->malloced) free(entry->data);
		else if(deleteitems) Destroy(entry->data);
		free(entry);
	}
	first=last=pointer=NULL;
	counter=0;
}

void *CGenericList::Add(const void *data)
{
	if (!data) return NULL;
	LISTENTRY *entry=(LISTENTRY *)malloc(sizeof(LISTENTRY));
	if (!entry) return NULL;
	entry->data=(void*)data;
	entry->malloced=false;
	entry->last=(LISTENTRY *)last;
	entry->next=NULL;
	if (last) ((LISTENTRY *)last)->next=entry;
	last=entry;
	if (!first) first=entry;
	counter++;
	return entry->data;
}

void *CGenericList::Copy(const void *data, size_t size)
{
	if ((!data) || (!size)) return NULL;
	LISTENTRY *entry=(LISTENTRY *)malloc(sizeof(LISTENTRY));
	if (!entry) return NULL;
	if (!(entry->data=(void *)malloc(size))) {
		free(entry);
		return NULL;
	}
	entry->malloced=true;
	memcpy(entry->data,data,size);
	entry->last=(LISTENTRY *)last;
	entry->next=NULL;
	if (last) ((LISTENTRY *)last)->next=entry;
	last=entry;
	if (!first) first=entry;
	counter++;
	return entry->data;
}

void *CGenericList::Find(const void *data) {
	LISTENTRY *entry=(LISTENTRY *)first;
	while (entry) {
		if (entry->data==data) {
			return(entry);
		}
		entry=entry->next;
	}
	return NULL;
}

int CGenericList::HasObject(const void *data)
{
	LISTENTRY *entry=(LISTENTRY *)Find(data);
	if (entry) return 1;
	return 0;
}

int CGenericList::Delete(const void *data) {
	LISTENTRY *entry=(LISTENTRY *)Find(data);
	if (!entry) return(0);
	if (entry->last) entry->last->next=entry->next;
	if (entry->next) entry->next->last=entry->last;
	if ((LISTENTRY *)first==entry) first=entry->next;
	if ((LISTENTRY *)last==entry) last=entry->last;
	//free(entry->data);	// Darum muss sich die Anwendung kümmern
	// Es kann sich bei entry->data auch um eine Klasse handeln, was wir aber nicht wissen können
	free(entry);
	counter--;
	return(1);
}

void CGenericList::Reset()
{
	pointer=NULL;
}


void *CGenericList::GetFirst()
{
	pointer=NULL;
	return GetNext();
}

void *CGenericList::GetNext()
{
	if (pointer==(void*)1) return NULL;	// Am Ende der Liste
	if (!pointer) pointer=first;
	if (!pointer) return NULL;			// Leere Liste
	LISTENTRY *entry=(LISTENTRY *)pointer;
	pointer=entry->next;				// Pointer auf nächstes Element
	if (!pointer) pointer=(void *)1;	// Keine weiteren Elemente mehr
	void *ret=entry->data;
	return ret;
}

void *CGenericList::GetLast()
{
	pointer=NULL;
	return GetPrevious();
}

void *CGenericList::GetPrevious()
{
	if (pointer==(void*)1) return NULL;	// Am Ende der Liste
	if (!pointer) pointer=last;
	if (!pointer) return NULL;			// Leere Liste
	LISTENTRY *entry=(LISTENTRY *)pointer;
	pointer=entry->last;				// Pointer auf vorhergehendes Element
	if (!pointer) pointer=(void *)1;	// Keine weiteren Elemente mehr
	void *ret=entry->data;
	return ret;
}

int CGenericList::Num() const
{
	return counter;
}

int CGenericList::Destroy(void *data)
{
	if (!DestroyFunction) {
		return 0;
	}
	return DestroyFunction(data,DestroyData);
}


int CGenericList::SetDestroyFunction(int DestroyFunction(void *item,void *data),void *data)
{
	this->DestroyFunction=DestroyFunction;
	this->DestroyData=data;
	return 1;
}

CGenericList::Walker::Walker()
{
	pointer=NULL;
}

void CGenericList::Reset(Walker &walk) const
{
	walk.pointer=NULL;
}


void *CGenericList::GetFirst(Walker &walk) const
{
	walk.pointer=NULL;
	return GetNext(walk);
}

void *CGenericList::GetNext(Walker &walk) const
{
	if (walk.pointer==(void*)1) return NULL;	// Am Ende der Liste
	if (!walk.pointer) walk.pointer=first;
	if (!walk.pointer) return NULL;			// Leere Liste
	LISTENTRY *entry=(LISTENTRY *)walk.pointer;
	walk.pointer=entry->next;				// Pointer auf nächstes Element
	if (!walk.pointer) walk.pointer=(void *)1;	// Keine weiteren Elemente mehr
	void *ret=entry->data;
	return ret;
}

void *CGenericList::GetLast(Walker &walk) const
{
	walk.pointer=NULL;
	return GetPrevious(walk);
}

void *CGenericList::GetPrevious(Walker &walk) const
{
	if (walk.pointer==(void*)1) return NULL;	// Am Ende der Liste
	if (!walk.pointer) walk.pointer=last;
	if (!walk.pointer) return NULL;			// Leere Liste
	LISTENTRY *entry=(LISTENTRY *)walk.pointer;
	walk.pointer=entry->last;				// Pointer auf vorhergehendes Element
	if (!walk.pointer) walk.pointer=(void *)1;	// Keine weiteren Elemente mehr
	void *ret=entry->data;
	return ret;
}


} // end of namespace ppl

