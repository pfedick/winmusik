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
#ifdef HAVE_STRING_H
	#include <string.h>
#endif
#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN		// Keine MFCs
	#include <windows.h>
#endif

#include "ppl6.h"
#include "internal.h"

namespace ppl6 {


//!\brief Interner Heap-Speicher für Elemente in CAssocArray
static CHeap *Heap;


CArrayItem::CArrayItem()
/*!\brief Konstruktor des Array-Elements
 *
 * \desc
 * Der Konstruktor setz die internen Variablen auf 0
 */
{
	type=0;
	value=NULL;
}

CArrayItem::~CArrayItem()
/*!\brief Destruktor eines Array-Elements
 *
 * \desc
 * Der Destruktor stellt sicher, dass das Element keine Daten mehr enthält. Dazu wird die
 * Clear-Funktion aufgerufen
 */
{
	Clear();
}

void *CArrayItem::operator new(size_t size)
/*!\brief Speicher allokieren
 *
 * \desc
 * Der New-Operator dieser Klasse wurde überschrieben, damit der Speicher nicht mehr aus dem
 * Hauptspeicher allokoert wird und diesem somit schnell fragmentiert, sondern aus dem internen
 * Heap.
 *
 * \param[in] size Größe des benötigten Speicherblocks. Dieser muss identisch sein mit
 * sizeof(CArrayItem)
 *
 * \returns bei Erfolg gibt die Funktion einen Pointer auf den Beginn des Speicherblocks zurück,
 * im Fehlerfall wird eine Exception generiert
 *
 * \see
 * - Cppl6Core::GetHeap_CArrayItem
 *
 */
{
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_CArrayItem();
			if (!Heap) {
				throw ppl6::GetError();
			}
		}
	}
	void *p=Heap->Malloc();
	if (!p) {
		throw ppl6::GetError();
	}
	return p;

}

void CArrayItem::operator delete(void *ptr, size_t size)
/*!\brief Speicher freigeben
 *
 * \desc
 * Die Klasse verwendet Speicher aus dem internen Heap, der durch Aufruf dieses überladenen
 * delete-Operators wieder freigegeben wird
 *
 * \param[in] ptr Zeiger auf den freizugebenden Speicherblock
 * \param[in] size Größe des Speicherblocks.
 */
{
	if (ptr) Heap->Free(ptr);
}

int CArrayItem::CompareNode(CTreeItem *item2)
{
	if (key.IsNumeric()==1 && (static_cast<CArrayItem*>(item2))->key.IsNumeric()==1) {
		pplint64 v1=key.ToInt64();
		//pplint64 v2=((CArrayItem*)item2)->key.ToInt64();
		pplint64 v2=(static_cast<CArrayItem*>(item2))->key.ToInt64();
		if (v2>v1) return 1;
		if (v2<v1) return -1;
		return 0;
	}
	int cmp=key.StrCaseCmp((static_cast<CArrayItem*>(item2))->key);
	if (cmp==0) return 0;
	if (cmp<0) return 1;
	return -1;
}

int CArrayItem::CompareValue(void *value)
{
	CString value2=(const char*)value;
	if (key.IsNumeric()==1 && value2.IsNumeric()==1) {
		pplint64 v1=key.ToInt64();
		pplint64 v2=value2.ToInt64();
		if (v2>v1) return 1;
		if (v2<v1) return -1;
		return 0;
	}
	int cmp=key.StrCaseCmp((const char*)value);
	if (cmp==0) return 0;
	if (cmp<0) return 1;
	return -1;
}

void CArrayItem::Clear()
/*!\brief Inhalt des Elements löschen
 *
 * \desc
 * Diese Funktion prüft, ob das Element Daten enthält und löscht diese. Dazu wird der delete-Operator
 * des jeweiligen Datentyps aufgerufen.
 */
{
	if (!type) return;
	if (type==datatype::ARRAY) delete (static_cast<CAssocArray*>(value));
	else if (type==datatype::CSTRING) delete (static_cast<CString*>(value));
	else if (type==datatype::CWSTRING) delete (static_cast<CWString*>(value));
	else if (type==datatype::BINARY) delete (static_cast<CBinary*>(value));
	else if (type==datatype::DATETIME) delete (static_cast<CDateTime*>(value));
	type=0;
	value=0;
}


/*!\class CAssocArray
 * \ingroup PPLGroupDataTypes
 * \brief Komplexes mehrdimensionales Array mit Strings als Schlüssel
 *
 * \desc
 * Die Klasse CAssocArray dient als Container für beliebige Key-Value-Paare. Ein Schlüssel
 * (Key) besteht aus einem String, der aus beliebigen Zeichen bestehen kann. Ein Value kann
 * veschiedene Datentypen enthalten. Gegenwärtig werden neben Strings auch Arrays, Binaries und
 * Pointer unterstützt.
 * \par
 * Die Schlüssel werden sortiert in einem AVL-Baum verwaltet (siehe CTree, CTreeItem, CArrayItem), so dass auch bei
 * sehr großen Arrays eine schnelle Verarbeitung gewährleistet ist. Gross-/Kleinschreibung wird dabei
 * ignoriert, der Schlüssel "TEST" wäre also identisch mit "test" oder "Test".
 * \par
 * Mehrdimensionale Arrays sind möglich, indem einem Schlüssel als Wert einfach ein anderes Array
 * zugeordnet wird. In einem solchen Array kann jedes Element direkt angesprochen werden, indem man
 * die einzelnen Schlüssel durch Slash (/) getrennt zu einem einzigen Schlüssel zusammenfasst.
 * \par
 * Mehrdimensionale Arrays werden automatisch generiert. Gibt man bei einem leeren Array dem Schlüssel
 * <tt>"ebene1/ebene2/key"</tt> einen Wert, werden automatisch folgende Aktionen ausgeführt:
 * - Es wird ein neues Array generiert und mit dem Schlüssel "ebene1" in das Array eingefügt
 * - In das Array "ebene1" wird ein weiteres neues Array mit dem Schlüssel "ebene2" eingefügt
 * - In das Array "ebene2" wird der eigentliche Wert unter dem Schlüssel "key" eingefügt
 *
 * \par Beispiel:
 * \code
 * ppl6::CAssocArray a;
 * // Wert setzen
 * a.Set("ebene1/ebene2/key","Ein Wert");
 * // Wert auslesen
 * const char *wert=a.Get("ebene1/ebene2/key");
 * \endcode
 *
 */

/*!\var CAssocArray::Tree
 * \brief Der Baum, in dem die Elemente abgelegt werden
 *
 */

/*!\var CAssocArray::maxint
 * \brief Enthält die höchste bisher als Schlüssel verwendete Ziffer
 *
 */

/*!\var CAssocArray::num
 * \brief Anzahl Elemente in diesem Array
 *
 */


CAssocArray::CAssocArray()
/*!\brief Konstruktor des Assoziativen Arrays
 *
 * \desc
 * Der Konstruktor prüft zuerst, ob das interne globale Initialisierungsflag für die Klasse
 * schon gesetzt ist. Ist dies nicht der Fall, wird die Funktion PPLInit aufgerufen, die zum einen
 * sicherstellt, dass die Funktion nicht parallel durch mehrere Threads aufgerufen wird und zum
 * andern dafür sorgt, dass der Heap-Speicher für die Arrays initialisiert wird.
 */
{
	type=CVar::CASSOCARRAY;
	num=0;
	maxint=0;
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_CArrayItem();
			if (!Heap) {
				throw ppl6::GetError();
			}
		}
	}
}

CAssocArray::CAssocArray(const CAssocArray &a)
/*!\brief Konstruktor des Assoziativen Arrays
 *
 * \desc
 * Der Konstruktor prüft zuerst, ob das interne globale Initialisierungsflag für die Klasse
 * schon gesetzt ist. Ist dies nicht der Fall, wird die Funktion PPLInit aufgerufen, die zum einen
 * sicherstellt, dass die Funktion nicht parallel durch mehrere Threads aufgerufen wird und zum
 * andern dafür sorgt, dass der Heap-Speicher für die Arrays initialisiert wird.
 *
 * \param[in] a Ein anderes CAssocArray, dessen Inhalt kopiert werden soll
 */
{
	type=CVar::CASSOCARRAY;
	num=0;
	maxint=0;
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_CArrayItem();
			if (!Heap) {
				throw ppl6::GetError();
			}
		}
	}
	Copy(a);
}

CAssocArray::CAssocArray(const CArray &a)
/*!\brief Konstruktor des Assoziativen Arrays
 *
 * \desc
 * Der Konstruktor prüft zuerst, ob das interne globale Initialisierungsflag für die Klasse
 * schon gesetzt ist. Ist dies nicht der Fall, wird die Funktion PPLInit aufgerufen, die zum einen
 * sicherstellt, dass die Funktion nicht parallel durch mehrere Threads aufgerufen wird und zum
 * andern dafür sorgt, dass der Heap-Speicher für die Arrays initialisiert wird.
 *
 * \param[in] a Ein CArray, dessen Inhalt kopiert werden soll
 */
{
	type=CVar::CASSOCARRAY;
	num=0;
	maxint=0;
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_CArrayItem();
			if (!Heap) {
				throw ppl6::GetError();
			}
		}
	}
	Copy(a);
}


CAssocArray::CAssocArray(const CAssocArray *a)
/*!\brief Konstruktor des Assoziativen Arrays
 *
 * \desc
 * Der Konstruktor prüft zuerst, ob das interne globale Initialisierungsflag für die Klasse
 * schon gesetzt ist. Ist dies nicht der Fall, wird die Funktion PPLInit aufgerufen, die zum einen
 * sicherstellt, dass die Funktion nicht parallel durch mehrere Threads aufgerufen wird und zum
 * andern dafür sorgt, dass der Heap-Speicher für die Arrays initialisiert wird.
 *
 * \param[in] a Ein anderes CAssocArray, dessen Inhalt kopiert werden soll
 */
{
	type=CVar::CASSOCARRAY;
	num=0;
	maxint=0;
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_CArrayItem();
			if (!Heap) {
				throw ppl6::GetError();
			}
		}
	}
	Copy(a);
}


CAssocArray::~CAssocArray()
/*!\brief Destruktor der Klasse
 *
 * \desc
 * Der Destruktor ruft die Funktion CAssocArray::Clear auf, um alle vorhandenen Elemente zu
 * löschen. Ein eventuell vorher gesetzter Fehlercode bleibt dabei erhalten.
 */
{
	PushError();
	Clear();
	PopError();
}



ARRAY_RESULT CAssocArray::FindInternal(const char *key) const
/*!\brief Interne Funktion zum Suchen eines Elements
 *
 * \desc
 * Diese Funktion zerlegt den angegebenen Schlüssel (\p key) in seine einzelnen Elemente.
 * Als Trennzeichen wird wie bei einer Unix-Pfadangabe das Slash (/) verwendet. Die Funktion
 * sucht zunächst auf das erste Element im eigenen Baum. Ist dies vorhanden und handelt es sich
 * bei dessen Datentyp wieder um ein CAssocArray, wird deren FindInternal-Funktion mit dem
 * restlichen Schlüssel rekursiv aufgerufen. Dies geschieht solange, bis das letzte Element des
 * Keys gefunden wurde.
 *
 * \param[in] key Pointer auf den String mit dem gesuchten Schlüssel
 * \returns Konnte der Schlüssel gefunden werden, wir der Pointer auf das Element (CArrayItem)
 * zurückgegeben. Wurde der Schlüssel nicht gefunden, wird ein Fehlercode gesetzt und NULL
 * zurückgeliefert.
 *
 * Die Funktion wird von allen Get...- und Concat-Funktionen verwendet.
 */
{
	CTok tok((char*)key,"/");
	if (tok.Num()==0) {
		SetError(302,(char*)key);
		return NULL;
	}
	const char *firstkey=tok.GetFirst();
	CString rest;
	tok.GetRest(rest,"/");
	CArrayItem *p=static_cast<CArrayItem *>(Tree.Find((void*)firstkey));
	if (!p) {
		SetError(294);
		return NULL;
	}
	// Ist noch was im Pfad rest?
	if (tok.Num()>1) {			// Ja, koennen wir iterieren?
		if (p->type==datatype::ARRAY) {
			p=(CArrayItem*)(static_cast<CAssocArray*>(p->value))->FindInternal((const char*)rest.GetPtr());
		} else {
			p=NULL;
			SetError(294);
		}
	}
	return (ARRAY_RESULT)p;
}

ARRAY_RESULT CAssocArray::CreateTree(const char *key)
/*!\brief Interne Funktion, die ein Element im Baum sucht oder anlegt
 *
 * \desc
 * Diese Funktion durchsucht den Baum nach dem gewünschten Element. Ist es vorhanden, wird dessen Pointer
 * zurückgeliefert, wenn nicht, wird es angelegt, jedoch ohne Daten. Bei verschachtelten Schlüsseln wird
 * die Funktion rekursiv aufgerufen, bis das letzte Element erreicht ist. Die Funktion wird intern von allen
 * Funktionen verwendet, die Daten in das Array speichern.
 *
 * \param[in] key Pointer auf den Namen des Schlüssels
 * \returns Bei Erfolg liefert die Funktion einen Pointer auf das gewünschte Element zurück. Im Fehlerfall wird
 * ein Fehlercode gesetzt und NULL zurückgegeben.
 *
 * \remarks Bei der Angabe eines verschachtelten Schlüssels kann es vorkommen, dass bereits vorhandene
 * Elemente überschrieben werden. Beispiel:
 *
 * Das Element <tt>ebene1/schlüssel1</tt> ist im Baum bereits vorhanden und beinhaltet einen String. Nun wird
 * das neue Element <tt>ebene1/schlüssel1/unterschlüssel1</tt> angelegt. Da Schlüssel eindeutig sein müssen,
 * wird der String <tt>ebene1/schlüssel1</tt> gelöscht und in ein Array umgewandelt.
 */
{
	PPL_ERROR er;
	StoreError(&er);

	CTok tok((char*)key,"/");
	if (tok.Num()==0) {
		SetError(302,(char*)key);
		return NULL;
	}
	CString firstkey;
	firstkey.Set(tok.GetFirst());
	CString rest;
	tok.GetRest(rest,"/");
	if (firstkey=="[]") {
		firstkey.Setf("%llu",maxint);
		maxint++;
	}
	// Begint Firstkey mit einer Zahl?
	if (firstkey[0]>='0' && firstkey[0]<='9') {
		ppluint64 keyint=firstkey.ToInt64();
		if (keyint>=maxint) maxint=keyint+1;
	}

	CArrayItem *p;
	Tree.Reset();
	p=(CArrayItem*)Tree.Find((void*)firstkey.GetPtr());
	if (p) {
		// Ist noch was im Pfad rest?
		if (tok.Num()>1) {          // Ja, koennen wir iterieren?
			if (p->type!=datatype::ARRAY) {
				p->Clear();		// Nein, wir loeschen daher diesen Zweig und machen ein Array draus
				p->value=new CAssocArray;
				if (!p->value) {
					SetError(2);
					return NULL;
				}
				p->type=datatype::ARRAY;
			}
			p=(CArrayItem*)(static_cast<CAssocArray*>(p->value))->CreateTree((const char*)rest.GetPtr());
		}
		// Nein, wir haben die Zielposition gefunden
		RestoreError(&er);
		return (ARRAY_RESULT)p;
	}

	// Key ist nicht in diesem Array, wir legen ihn an
	p=new CArrayItem;
	if (!p) {
		SetError(2);
		return NULL;
	}
	p->key.Set(&firstkey);
	// Key in die Liste haengen
	if (!Tree.Add(p)) {
		PushError();
		delete p;
		PopError();
		return NULL;
	}

	// Ist noch was im Pfad rest?
	if (tok.Num()>1) {          // Ja, wir erstellen ein Array und iterieren
		p->value=new CAssocArray;
		if (!p->value) {
			SetError(2);
			return NULL;
		}
		p->type=datatype::ARRAY;
		p=(CArrayItem*)((CAssocArray*)p->value)->CreateTree((const char*)rest.GetPtr());
	}
	num++;
	RestoreError(&er);
	return (ARRAY_RESULT)p;
}



void CAssocArray::Clear()
/*!\brief Inhalt des Arrays löschen
 *
 * \desc
 * Mit dieser Funktion wird der komplette Inhalt des Arrays gelöscht. Dabei der Destruktor für jedes
 * vorhandene Element aufgerufen, der wiederum sicherstellt, dass die darin enthaltenen Daten
 * ordnungsgemäß gelöscht werden.
 *
 */
{
	Tree.Clear(true);
	num=0;
	maxint=0;
}

void CAssocArray::List(const char *prefix) const
/*!\brief Inhalt des Arrays ausgeben
 *
 * \desc
 * Diese Funktion dient Debugging-Zwecken. Der Aufruf bewirkt, dass der Inhalt des kompletten Arrays auf
 * STDOUT ausgegeben wird.
 *
 * \param[in] prefix Optionaler Text, der bei der Ausgabe jedem Element vorangestellt wird
 *
 * \par Beispiel:
 * \code
ppl6::CAssocArray a;
ppl6::CBinary bin;
bin.Load("main.cpp");

a.Set("key1","value1");
a.Set("array1/unterkey1","value2");
a.Set("array1/unterkey2","value3");
a.Set("array1/noch ein array/unterkey1","value4");
a.Set("array1/unterkey2","value5");
a.Set("key2","value6");
a.Set("dateien/main.cpp",&bin);
a.Set("array2/unterkey1","value7");
a.Set("array2/unterkey2","value8");
a.Set("array2/unterkey1","value9");
a.SetPointer("pointer1",&bin);
a.List("prefix");
\endcode
	Ausgabe:
\code
prefix/array1/noch ein array/unterkey1=value4
prefix/array1/unterkey1=value2
prefix/array1/unterkey2=value5
prefix/array2/unterkey1=value9
prefix/array2/unterkey2=value8
prefix/dateien/main.cpp=CBinary, 806 Bytes
prefix/key1=value1
prefix/key2=value6
prefix/pointer1=POINTER 3217221904 (0xbfc2e910)
\endcode
 *
 * \remarks Die Funktion gibt nur "lesbare" Element aus. Enthält das Array Pointer oder Binaries, wird das Element zwar
 * ausgegeben, jedoch werden als Wert nur Meta-Informationen ausgegeben (Datentyp, Pointer, Größe).
 */
{
	CString key;
	CString pre;
	if (prefix) key.Sprintf("%s/",prefix);
	CTreeWalker walk;
	Tree.Reset(walk);
	CArrayItem *p;
	while ((p=(CArrayItem*)Tree.GetNext(walk))) {
		if (p->type==datatype::CSTRING) {
			PrintDebug("%s%s=%s\n",(const char*)key,(const char*)p->key.GetPtr(),(const char*)((CString*)p->value)->GetPtr());
		} else if (p->type==datatype::CWSTRING) {
			PrintDebug("%s%s=%s\n",(const char*)key,(const char*)p->key.GetPtr(),(const char*)((CWString*)p->value)->GetPtr());
		} else if (p->type==datatype::DATETIME) {
			PrintDebug("%s%s=CDateTime: %s\n",(const char*)key,(const char*)p->key.GetPtr(),(const char*)((CDateTime*)p->value)->getISO8601withMsec());
		} else if (p->type==datatype::BINARY) {
			PrintDebug("%s%s=CBinary, %llu Bytes\n",(const char*)key,(const char*)p->key.GetPtr(),(ppluint64)((CBinary*)p->value)->Size());
		} else if (p->type==datatype::ARRAY) {
			pre.Sprintf("%s%s",(const char*)key,(const char*)p->key.GetPtr());
			((CAssocArray*)p->value)->List((const char*)pre);
		} else if (p->type==datatype::POINTER) {
			PrintDebug("%s%s=POINTER %llu (0x%llx)\n",(const char*)key,(const char*)p->key.GetPtr(),(ppluint64)(size_t)(p->value), (ppluint64)(size_t)(p->value));
		}
	}
}

int CAssocArray::Validate(const char *prefix)
/*!\brief Array überprüfen
 *
 * \desc
 * Diese Funktion kann aufgerufen werden, um die Integrität der internen AVL-Bäume
 * des Arrays
 * zu überprüfen. Sie hangelt sich durch den sämtliche Bäume und überprüft
 * für jeden Knoten, ob die Verbindungen (left, right, parent) sowie die
 * Balance in Ordnung sind. Außerdem wird die tatsächliche Anzahl Knoten
 * ermittelt, die mit dem Sollwert übereinstimmen muß.
 *
 * \returns Wird ein Fehler festgestellt, wird eine Beschreibung auf STDOUT
 * ausgegeben und die Funktion gibt false (0) zurück. Wird kein Fehler festgestellt,
 * gibt die Funktion true (1) zurück.
 *
 * \since
 * Die Funktion wurde in Version 6.2.0 eingeführt.
 */
{
	CString key;
	CString pre;
	if (prefix) {
		printf ("Prüfe Baum: %s\n",prefix);
		key.Sprintf("%s/",prefix);
	}

	// Eigenen Baum prüfen
	int err=Tree.Validate();
	// Unterbäume prüfen
	Tree.Reset();
	CArrayItem *p;
	while ((p=(CArrayItem*)Tree.GetNext())) {
		if (p->type==datatype::ARRAY) {
			pre.Sprintf("%s%s",(const char*)key,(const char*)p->key.GetPtr());
			err+=((CAssocArray*)p->value)->Validate((const char*)pre);
		}
	}
	Tree.Reset();


	return err;
}


void CAssocArray::ToTemplate(CString &s, const char *prefix, const char *linedelimiter, const char *splitchar)
/*!\brief Inhalt des Arrays in ein Template exportieren
 *
 * \desc
 * Mit dieser Funktion wird der textuelle Inhalt des Arrays als Template im Key-Value-Format in einem String
 * abgelegt.
 * Pointer oder Binäre Daten werden ignoriert.
 *
 * \param[out] s String, in dem das Template gespeichert werden soll. Der String wird von der Funktion nicht gelöscht,
 * der Inhalt des Arrays wird angehangen!
 * \param[in] prefix Optionaler Prefix, der jedem Key vorangestellt werden soll
 * \param[in] linedelimiter Optionaler Pointer auf einen String, der am Zeilenende ausgegeben werden soll. Der
 *            Default ist ein einzelnes Newline.
 * \param[in] splitchar Optionaler Pointer auf einen String, der als Trennzeichen zwischen Schlüssel und Wert
 *            verwendet werden soll. Der Default ist ein Gleichheitszeichen.
 * \par Beispiel
\code
#include <stdio.h>
#include <string.h>
#include <ppl6.h>

int main(int argc, char **argv)
{
	ppl6::CAssocArray a;
	ppl6::CBinary bin;
	ppl6::CString out;
	bin.Load("main.cpp");
	a.Set("key1","Dieser Wert geht über\nmehrere Zeilen");
	a.Set("array1/unterkey1","value2");
	a.Set("array1/unterkey2","value3");
	a.Set("array1/noch ein array/unterkey1","value4");
	a.Set("array1/unterkey2","value5");
	a.Set("key2","value6");
	a.Set("dateien/main.cpp",&bin);
	a.Set("array2/unterkey1","value7");
	a.Set("array2/unterkey2","value8");
	a.Set("array2/unterkey1","value9");
	a.SetPointer("pointer1",&bin);
	a.ToTemplate(&out,"foo");
	out.Print();
}
\endcode
Ergebnis:
\code
foo/array1/noch ein array/unterkey1=value4
foo/array1/unterkey1=value2
foo/array1/unterkey2=value5
foo/array2/unterkey1=value9
foo/array2/unterkey2=value8
foo/key1=Dieser Wert geht über
foo/key1=mehrere Zeilen
foo/key2=value6
\endcode
	An diesem Beispiel sieht man, dass Pointer- und CBinary-Werte nicht exportiert werden und Werte, die Zeilenumbrüche
	enthalten, werden auf mehrere Key-Value-Paare aufgesplittet. Die Importfunktion (CAssocArray::CreateFromTemplate,
	CAssocArray::CreateFromConfig) fügen diese wieder zu einer einzelnen Variable mit Zeilenumbruch
	zusammen.
 */
{
	CString	key;
	CString	pre;
	const char 		*value;
	CTok		Tok;
	if (prefix) key.Sprintf("%s/",prefix);
	Tree.Reset();
	CArrayItem *p;
	while ((p=(CArrayItem*)Tree.GetNext())) {
		if (p->type==datatype::CSTRING || p->type==datatype::CWSTRING) {
			if (p->type==datatype::CSTRING)
				value=(char*)((CString*)p->value)->GetPtr();
			else
				value=(char*)((CWString*)p->value)->GetPtr();
			Tok.Split(value,"\n");
			while ((value=Tok.GetNext())) {
				s.Concatf("%s%s%s%s%s",(const char*)key,(const char*)p->key.GetPtr(),splitchar,value,linedelimiter);
			}
		//} else if (p->type==datatype::BINARY) {
		//	s->Concatf("%s%s%sCBinary, %llu Bytes%s",(char*)key,(char*)p->key.GetPtr(),splitchar,(ppluint64)((CBinary*)p->value)->Size(),linedelimiter);
		} else if (p->type==datatype::ARRAY) {
			pre.Sprintf("%s%s",(const char*)key,(const char*)p->key.GetPtr());
			((CAssocArray*)p->value)->ToTemplate(s,(const char*)pre,linedelimiter,splitchar);
		//} else if (p->type==datatype::POINTER) {
		//	s->Concatf("%s%s%sptr %llu%s",(char*)key,(char*)p->key.GetPtr(),splitchar,(ppluint64)(size_t)(p->value),linedelimiter);
		}
	}
}

int CAssocArray::Size() const
/*!\brief Liefert Anzahl Bytes, die für ExportBinary erforderlich sind
 *
 * \desc
 * Diese Funktion liefert die Anzahl Bytes zurück, die für den Buffer der Funktion CAssocArray::ExportBinary
 * erforderlich sind. Es kann dadurch ein ausreichend großer Puffer vor Aufruf der Funktion ExportBinary
 * angelegt werden.
 *
 * \returns Anzahl Bytes oder 0 im Fehlerfall
 *
 * \see
 * - CAssocArray::ExportBinary
 * - CAssocArray::ImportBinary
 */
{
	int size;
	if (ExportBinary(NULL,0, &size)) return size;
	return 0;
}

CBinary *CAssocArray::ExportBinary() const
{
	CBinary *bin=NULL;
	void *buffer;
	int realsize=0;
	int size=Size();
	if (!size) return NULL;
	buffer=malloc(size);
	if (!ExportBinary(buffer,size,&realsize)) {
		free(buffer);
		return NULL;
	}
	bin=new CBinary;
	if (!bin) {
		free(buffer);
		SetError(2);
		return NULL;
	}
	if (!bin->Set(buffer,size)) {
		free(buffer);
		delete bin;
		return NULL;
	}
	bin->ManageMemory();
	return bin;
}

int CAssocArray::ExportBinary(void *buffer, int buffersize, int *realsize) const
/*!\brief Inhalt des Arrays in einem plattform-unabhängigen Binären-Format exportieren
 *
 * \desc
 * Mit dieser Funktion kann der komplette Inhalt des Arrays in einem plattform-unabhängigem binären Format abgelegt
 * werden, das sich zum Speichern
 * in einer Datei oder zum Übertragen über das Internet eignet.
 *
 * \param[in] buffer Pointer auf einen ausreichend großen Puffer. Die Größe des benötigten Puffers
 *            kann zuvor mit der Funktion CAssocArray::Size ermittelt werden. Wird als Buffer NULL
 *            übergeben, wird in der Variable \p realsize ebenfalls die Anzahl Bytes zurückgegeben
 * \param[in] buffersize Die Größe des Puffers in Bytes
 * \param[out] realsize In dieser Variable wird gespeichert, wieviele Bytes tatsächlich für den Export
 *            verwendet wurden
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 *
 * \attention
 * Es muss daran gedacht werden, dass nicht alle Datentypen exportiert werden können. Gegenwärtig werden folgende Typen
 * unterstützt:
 * - char
 * - AssocArray
 * - CString
 * - Binary
 * \see
 * - CAssocArray::Size
 * - CAssocArray::ImportBinary
 */
{
	char *ptr=(char*)buffer;
	*realsize=0;
	int p=0;
	int keylen;
	int vallen=0;
	if (!buffer) buffersize=0;
	if (buffersize<0) buffersize=0;
	CArrayItem *a;
	CIconv iconv(ICONV_UNICODE,"UTF-8");
	if (p+7<buffersize) strncpy(ptr,"PPLASOC",7);
	p+=7;
	CTreeWalker walk;
	Tree.Reset(walk);
	while ((a=(CArrayItem*)Tree.GetNext(walk))) {
		if (p<buffersize) ppl6::PokeN8(ptr+p,a->type);
		p++;
		keylen=a->key.Len();
		if (p+4<buffersize) ppl6::PokeN16(ptr+p,keylen);
		p+=2;
		if (p+keylen<buffersize) strncpy(ptr+p,a->key.GetPtr(),keylen);
		p+=keylen;
		if (a->type==datatype::CSTRING) {
			vallen=((CString*)a->value)->Size();
			if (p+4<buffersize) ppl6::PokeN32(ptr+p,vallen);
			p+=4;
			if (p+vallen<buffersize) strncpy(ptr+p,((CString*)a->value)->GetPtr(),vallen);
			p+=vallen;
		} else if (a->type==datatype::CWSTRING) {
#ifdef HAVE_ICONV
			CBinary utf8;
			if(!iconv.Transcode((const char*)((CWString*)a->value)->GetWPtr(),((CWString*)a->value)->Size(),utf8)) {
				return 0;
			}
			vallen=utf8.Size();
			if (p+4<buffersize) ppl6::PokeN32(ptr+p,vallen);
			p+=4;
			if (p+vallen<buffersize) strncpy(ptr+p,(const char*)utf8.GetPtr(),vallen);
			p+=vallen;
#else
			CString utf8=*((CWString*)a);
			vallen=utf8.Size();
			if (p+4<buffersize) ppl6::PokeN32(ptr+p,vallen);
			p+=4;
			if (p+vallen<buffersize) strncpy(ptr+p,utf8.GetPtr(),vallen);
			p+=vallen;
#endif
		} else if (a->type==datatype::ARRAY) {
			int asize=0;
			((CAssocArray*)a->value)->ExportBinary(ptr+p,buffersize-p,&asize);
			p+=asize;
		} else if (a->type==datatype::CHAR) {
			vallen=strlen((char*)a->value);
			if (p+4<buffersize) ppl6::PokeN32(ptr+p,vallen);
			p+=4;
			if (p+vallen<buffersize) strncpy(ptr+p,(char*)a->value,vallen);
			p+=vallen;
		} else if (a->type==datatype::DATETIME) {
			vallen=8;
			if (p+4<buffersize) ppl6::PokeN32(ptr+p,vallen);
			p+=4;
			if (p+vallen<buffersize) PokeN64(ptr+p,((CDateTime*)a->value)->longInt());
			p+=vallen;
		} else if (a->type==datatype::BINARY) {
			vallen=((CBinary*)a->value)->Size();
			if (p+4<buffersize) ppl6::PokeN32(ptr+p,vallen);
			p+=4;
			if (p+vallen<buffersize) memcpy(ptr+p,((CBinary*)a->value)->GetPtr(),vallen);
			p+=vallen;
		} else {
			vallen=0;
			if (p+4<buffersize) ppl6::PokeN32(ptr+p,0);
			p+=4;
		}
	}
	if (p<buffersize) ppl6::PokeN8(ptr+p,0);
	p++;
	if (realsize)*realsize=p;
	if (buffersize==0) return 1;
	if (buffersize<=p) return 1;
	SetError(140,"%u",p);
	return 0;
}

int CAssocArray::ImportBinary(const CBinary &bin)
/*!\brief Daten aus einem vorherigen Export wieder importieren
 *
 * \desc
 * Mit dieser Funktion kann ein zuvor mit CAssocArray::ExportBinary exportiertes Assoziatives Array wieder
 * importiert werden. Falls im Array bereits Daten vorhanden sind, werden diese nicht gelöscht, können aber
 * überschrieben werden, wenn es im Export gleichnamige Schlüssel gibt.
 *
 * \param[in] bin Pointer auf eine CBinary Klassem die die zu importierenden Daten enthält
  * \returns Wenn die Daten erfolgreich importiert werden konnten, gibt die Funktion true (1) zurück, im
 * Fehlerfall false (0).
 *
 * \see
 * - CAssocArray::ExportBinary
 * - CAssocArray::Size
 */
{

	void *buffer=bin.GetPtr();
	size_t size=bin.GetSize();
	return ImportBinary(buffer,size);
}

int CAssocArray::ImportBinary(const void *buffer, int buffersize)
/*!\brief Daten aus einem vorherigen Export wieder importieren
 *
 * \desc
 * Mit dieser Funktion kann ein zuvor mit CAssocArray::ExportBinary exportiertes Assoziatives Array wieder
 * importiert werden. Falls im Array bereits Daten vorhanden sind, werden diese nicht gelöscht, können aber
 * überschrieben werden, wenn es im Export gleichnamige Schlüssel gibt.
 *
 * \param[in] buffer Pointer auf den Puffer, der die zu importierenden Daten enthält
 * \param[in] buffersize Größe des Puffers
 * \returns Wenn die Daten erfolgreich importiert werden konnten, gibt die Funktion true (1) zurück, im
 * Fehlerfall false (0).
 *
 * \see
 * - CAssocArray::ExportBinary
 * - CAssocArray::Size
 */
{
	if (!buffer) {
		SetError(194,"void *buffer");
		return 0;
	}
	if (buffersize<=0) {
		SetError(194,"int buffersize");
		return 0;
	}
	char *ptr=(char*)buffer;
	int p=0;
	if (buffersize<8 || ppl6::strncmp((const char*)ptr,"PPLASOC",7)!=0) {
		SetError(339);
		return 0;
	}
	p+=7;
	int type,keylen,vallen,bytes;
#ifdef HAVE_ICONV
	CIconv iconv("UTF-8",ICONV_UNICODE);
#endif
	CString key,str;
	CWString ws;
	CDateTime dt;
	CAssocArray *na;
	CBinary *nb,bin;
	while (p<buffersize && (type=PeekN8(ptr+p))!=0) {
		p++;
		keylen=PeekN16(ptr+p);
		p+=2;
		key.Set(ptr+p,keylen);
		p+=keylen;
		switch (type) {
			case datatype::CSTRING:
			case datatype::CHAR:
				vallen=PeekN32(ptr+p);
				p+=4;
				Set(key,(const char*)ptr+p,vallen);
				p+=vallen;
				break;
			case datatype::CWSTRING:
				vallen=PeekN32(ptr+p);
				p+=4;
#ifdef HAVE_ICONV
				if (!iconv.Transcode((const char*)ptr+p,vallen,bin)) {
					return 0;
				}
				ws.Set((const wchar_t*)bin.GetPtr(),bin.Size()/sizeof(wchar_t));
#else
				ws.Set((const char*)ptr+p,vallen);
#endif
				Set(key,ws);
				p+=vallen;
				break;
			case datatype::ARRAY:
				na=new CAssocArray;
				bytes=na->ImportBinary(ptr+p,buffersize-p);
				if (!bytes) return 0;
				p+=bytes;
				Set(key,na,false);
				break;
			case datatype::CARRAY:
				{
					int elements=PeekN32(ptr+p);
					p+=4;
					CArray stringarray;
					stringarray.Reserve(elements);
					for (int i=0;i<elements;i++) {
						str.Set(ptr+p+4,PeekN32(ptr+p));
						p+=PeekN32(ptr+p)+4;
						stringarray.Add(str);
					}
					Set(key,stringarray);
				}
				break;
			case datatype::BINARY:
				vallen=PeekN32(ptr+p);
				p+=4;
				nb=new CBinary;
				nb->Copy(ptr+p,vallen);
				p+=vallen;
				Set(key,nb,false);
				break;
			case datatype::DATETIME:
				vallen=PeekN32(ptr+p);
				p+=4;
				dt.setLongInt(PeekN64(ptr+p));
				p+=vallen;
				Set(key,dt);
				break;
			default:
				return 0;
		};
	}
	p++;
	return p;
}

int CAssocArray::Count(bool recursive) const
/*!\brief Anzahl Schlüssel zählen
 *
 * \desc
 * Diese Funktion liefert die Anzahl Schlüssel auf dieser Ebene des Array zurück.
 *
 * \param[in] recursive Falls recursive auf true gesetzt wird die Funktion rekusriv für jeden
 * Schlüssel aufgerufen, dessen Wert ebenfalls ein Array ist.
 *
 * \returns Anzahl Schlüssel
 */
{
	if (!recursive) return num;
	CTreeWalker walk;
	CArrayItem *p;
	Tree.Reset(walk);
	int c=num;
	while ((p=(CArrayItem*)Tree.GetNext(walk))) {
		if (p->type==datatype::ARRAY) c+=((CAssocArray*)p->value)->Count(recursive);
	}
	return c;
}

int CAssocArray::Count(const char *key, bool recursive) const
/*!\brief Anzahl Schlüssel für ein bestimmtes Element zählen
 *
 * \desc
 * Diese Funktion liefert die Anzahl Schlüssel zurück, die in dem angegebenen Key enthalten sind.
 *
 * \param[in] key Schlüssel-Name eines Assoziativen Arrays innerhalb dieses Arrays
 * \param[in] recursive Falls recursive auf true gesetzt wird die Funktion rekusriv für jeden
 * Schlüssel aufgerufen, dessen Wert ebenfalls ein Array ist.
 *
 * \returns Anzahl Schlüssel
 */
{
	int ret=0;
	CArrayItem *a=(CArrayItem *)FindInternal(key);
	if (a) {
		if (a->type==datatype::ARRAY && a->value!=NULL) ret=((CAssocArray*)a->value)->Count(recursive);
		else ret=1;
	}
	return ret;
}



int CAssocArray::Add (const char *key, const char *value, int size)
/*!\brief String hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt eines Strings dem Array hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] value Pointer auf den Wert für den Schlüssel
 * \param[in] size Optionaler Parameter, der die Anzahl Bytes angibt, die aus dem \p value Wert
 * übernommen werden sollen
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 * \remarks
 * Diese Funktion ist identisch mit CAssocArray::Set(const char *key, const char *value, int size)
 */
{
	return Set((const char *)key, (const char*)value,size);
}

int CAssocArray::Add (const char *key, ppldds value)
/*!\brief Integer hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Wert einer Integer-Variable dem Array hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] value Integer-Wert
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 * \remarks
 * Da das Assoziative Array keine Integer verwalten kann wird der Wert zuerst in einen
 * CString gewandelt und dieser eingefügt.
 */
{
	CString buf;
	buf.Sprintf("%i",value);
	return Set(key, buf);
}

int CAssocArray::Set(const char *key, const char *value, int size)
/*!\brief String hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt eines Strings dem Array hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] value Pointer auf den Wert für den Schlüssel
 * \param[in] size Optionaler Parameter, der die Anzahl Bytes angibt, die aus dem \p value Wert
 * übernommen werden sollen
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 * \remarks
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	if (!value) {
		SetError(194,"const char *value");
		return 0;
	}
	CString *str=new CString;
	if (!str) {
		SetError(2);
		return 0;
	}
	str->Set(value,size);
	CArrayItem *ptr=(CArrayItem *)CreateTree(key);
	if (!ptr) {
		PushError();
		delete str;
		PopError();
		return 0;
	}
	ptr->Clear();
	ptr->value=str;
	ptr->type=datatype::CSTRING;
	return 1;
}


int CAssocArray::Set(const char *key, const CString &value, int size)
/*!\brief CString hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt eines CStrings dem Array hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] value Pointer auf den Wert für den Schlüssel
 * \param[in] size Optionaler Parameter, der die Anzahl Bytes angibt, die aus dem \p value Wert
 * übernommen werden sollen
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	return Set(key,value.GetPtr(),size);
}

int CAssocArray::Set(const char *key, const CWString &value, int size)
/*!\brief Unicode CWString hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt eines CWStrings (Unicode) dem Array hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] value Pointer auf den Wert für den Schlüssel
 * \param[in] size Optionaler Parameter, der die Anzahl Zeichen angibt, die aus dem \p value Wert
 * übernommen werden sollen
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	CWString *str=new CWString;
	if (!str) {
		SetError(2);
		return 0;
	}
	str->Set(value,size);
	CArrayItem *ptr=(CArrayItem *)CreateTree(key);
	if (!ptr) {
		PushError();
		delete str;
		PopError();
		return 0;
	}
	ptr->Clear();
	ptr->value=str;
	ptr->type=datatype::CWSTRING;
	return 1;
}


int CAssocArray::Setf(const char *key, const char *fmt, ...)
/*!\brief Formatierten String hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt eines formatierten Strings dem Array hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] fmt Pointer auf den Format-String des Wertes
 * \param[in] ... Beliebig viele Parameter, die vom Formatstring verwendet werden
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	if (!fmt) {
		SetError(194,"const char *fmt");
		return 0;
	}
	CString value;
	va_list args;
	va_start(args, fmt);
	value.Vasprintf(fmt,args);
	va_end(args);
	return Set(key,(const char*)value.GetPtr());
}

int CAssocArray::Set(const char *key, const CAssocArray &array)
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)CreateTree(key);
	if (!ptr) {
		return 0;
	}
	ptr->Clear();
	CAssocArray *a=new CAssocArray;
	if (!a) {
		SetError(2);
		return 0;
	}
	ptr->type=datatype::ARRAY;
	ptr->value=a;
	int ret=a->Copy(array);
	if (!ret) {
		PushError();
		delete a;
		PopError();
		ptr->value=NULL;
		ptr->type=0;
	}
	return ret;
}

int CAssocArray::Set(const char *key, CAssocArray *array, bool copy)
/*!\brief Assoziatives Array hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt eines anderen Assoziativen Arrays hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] array Pointer auf das hinzuzufügende CAssocArray
 * \param[in] copy Optionaler Parameter der angibt, ob das Array kopiert werden soll (true) oder
 * nur der Pointer auf das existierende Array übernommen werden soll (false)
 *
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 * \remarks
 * Wird das als Parameter angegebene Array nicht kopiert, geht dessen Verwaltung dennoch in
 * dieses Array über und darf von der Anwendung nicht selbst gelöscht werden.
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	if (!array) {
		SetError(194,"CAssocArray *array");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)CreateTree(key);
	if (!ptr) {
		return 0;
	}
	ptr->Clear();
	if (!copy) {
		ptr->value=array;
		ptr->type=datatype::ARRAY;
		return 1;
	}
	CAssocArray *a=new CAssocArray;
	if (!a) {
		SetError(2);
		return 0;
	}
	ptr->type=datatype::ARRAY;
	ptr->value=a;
	int ret=a->Copy(*array);
	if (!ret) {
		PushError();
		delete a;
		PopError();
		ptr->value=NULL;
		ptr->type=0;
	}
	return ret;
}


int CAssocArray::Set(const char *key, const CArray &array)
/*!\brief Einfaches CArray hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt des angegebenen CArray hinzu.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] array Pointer auf das hinzuzufügende CArray
 *
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 * \remarks
 * Gegenwärtig kann CAssocArray keine CArray verwalten, daher wird das CArray vor dem
 * Einfügen in ein CAssocArray gewandelt, dessen Schlüssel durchnummeriert sind.
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)CreateTree(key);
	if (!ptr) {
		return 0;
	}
	ptr->Clear();
	CAssocArray *a=new CAssocArray;
	if (!a) {
		SetError(2);
		return 0;
	}
	ptr->type=datatype::ARRAY;
	ptr->value=a;
	int ret=a->Copy(array);
	return ret;
}

int CAssocArray::Set(const char *key, const CBinary &bin)
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)CreateTree(key);
	if (!ptr) {
		return 0;
	}
	ptr->Clear();
	CBinary *a=new CBinary;
	if (!a) {
		SetError(2);
		return 0;
	}
	ptr->type=datatype::BINARY;
	ptr->value=a;
	int ret=a->Copy(bin);
	if (!ret) {
		PushError();
		delete a;
		PopError();
		ptr->value=NULL;
		ptr->type=0;
	}
	return ret;
}

int CAssocArray::Set(const char *key, CBinary *bin, bool copy)
/*!\brief CBinary hinzufügen
 *
 * \desc
 * Diese Funktion fügt den Inhalt des CBinary-Objekts dem Array hinzu
 *
 * \param[in] key Name des Schlüssels
 * \param[in] bin Pointer auf das hinzuzufügende CBinary
 * \param[in] copy Optionaler Parameter der angibt, ob das Binary kopiert werden soll (true) oder
 * nur dessen Pointer übernommen werden soll (false)
 *
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 * \remarks
 * Wird das als Parameter angegebene Binary nicht kopiert, geht dessen Verwaltung dennoch in
 * dieses Array über und darf von der Anwendung nicht selbst gelöscht werden.
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	if (!bin) {
		SetError(194,"CBinary *bin");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)CreateTree(key);
	if (!ptr) {
		return 0;
	}
	ptr->Clear();
	if (!copy) {
		ptr->value=bin;
		ptr->type=datatype::BINARY;
		return 1;
	}
	CBinary *a=new CBinary;
	if (!a) {
		SetError(2);
		return 0;
	}
	ptr->type=datatype::BINARY;
	ptr->value=a;
	int ret=a->Copy(*bin);
	if (!ret) {
		PushError();
		delete a;
		PopError();
		ptr->value=NULL;
		ptr->type=0;
	}
	return ret;
}

int CAssocArray::SetPointer(const char *key, const void *pointer)
/*!\brief Pointer hinzufügen
 *
 * \desc
 * Mit dieser Funktion kann ein Pointer auf eine beliebige Datenstruktur dem Array
 * hinzugefügt werden.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] pointer Pointer auf die Daten
 *
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 * \remarks
 * Der Pointer wid von der CAssocArray-Klasse nicht verwaltet, das heisst die Anwendung muss selbst
 * dafür sorgen, dass ggfs. Speicher wieder freigegeben wird.
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)CreateTree(key);
	if (!ptr) {
		return 0;
	}
	ptr->Clear();
	ptr->value=(void*)pointer;
	ptr->type=datatype::POINTER;
	return 1;
}

int	CAssocArray::Set(const char *key, const CDateTime &date)
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	CDateTime *var=new CDateTime;
	if (!var) {
		SetError(2);
		return 0;
	}
	var->set(date);
	CArrayItem *ptr=(CArrayItem *)CreateTree(key);
	if (!ptr) {
		PushError();
		delete var;
		PopError();
		return 0;
	}
	ptr->Clear();
	ptr->value=var;
	ptr->type=datatype::DATETIME;
	return 1;
}

int CAssocArray::Concat(const char *key, const char *value, const char *concat, int size)
/*!\brief String-Wert eines Schlüssels erweitern
 *
 * \desc
 * Mit dieser Funktion kann der String-Wert eines Schlüssels erweitert werden.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] value String, der an den vorhandenen Wert angefügt werden soll
 * \param[in] concat Optionales Trennzeichen. Wurde es angegeben und enthält der Schlüssel bereits
 * einen Wert, wird zunächst das Trennzeichen \p concat angefügt und danach erst der neue
 * String \p value.
 * \param[in] size Optionaler Parameter, der die Anzahl anzufügender Bytes angibt. Wird er
 * nicht angegeben, wird der komplette String aus \p value übernommen
 *
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	if (!value) {
		SetError(194,"const char *value");
		return 0;
	}
	CString v;
	v.Set(value,size);
	CArrayItem *ptr=(CArrayItem*)CreateTree(key);
	if (!ptr) {
		return 0;
	}
	if (ptr->type!=datatype::CSTRING) {
		ptr->Clear();
	}
	CString *str=(CString*)ptr->value;
	if (!str) {
		str=new CString;
		if (!str) {
			SetError(2);
			return 0;
		}
		ptr->value=str;
		ptr->type=datatype::CSTRING;
	}
	if (str->Len()) str->Concat(concat);
	str->Concat(v);
	return 1;
}

int CAssocArray::Concatf(const char *key, const char *concat, const char *fmt, ...)
/*!\brief String-Wert eines Schlüssels um einen formatierten String erweitern
 *
 * \desc
 * Mit dieser Funktion kann der String-Wert eines Schlüssels um einen formatierten String
 * erweitert werden.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] concat Optionales Trennzeichen. Wurde es angegeben und enthält der Schlüssel bereits
 * einen Wert, wird zunächst das Trennzeichen \p concat angefügt und danach erst der neue
 * String \p value.
 * \param[in] fmt Format-String, der an den vorhandenen Wert angefügt werden soll
 * \param[in] ... Beliebig viele Werte für den Format-String \p fmt
 *
 * \returns Konnte der Wert dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 *
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	if (!fmt) {
		SetError(194,"const char *fmt");
		return 0;
	}
	if (!concat) concat="\n";
	CString value;
	va_list args;
	va_start(args, fmt);
	value.Vasprintf(fmt,args);
	va_end(args);
	return Concat(key,(const char*)value.GetPtr(),concat);
}

int CAssocArray::Copy(const CAssocArray &a)
/*!\brief CAssocArray kopieren
 *
 * \desc
 * Mit dieser Funktion wird der komplette Inhalt eines CAssocArray in dieses hineinkopiert.
 * Das Array wird vorher nicht gelöscht, so dass vorhandene Schlüssel erhalten bleiben, jedoch
 * können diese überschrieben werden, wenn das hinzuzufügende Array gleichnamige Schlüssel
 * enthält.
 *
 * \param[in] a Das zu kopierende CAssocArray
 *
 * \returns Bei Erfolg liefert die die Funktion true (1) zurück, im Fehlerfall false (0).
 */
{
	return Copy(&a);
}

int CAssocArray::Copy(const CAssocArray *a)
/*!\brief CAssocArray kopieren
 *
 * \desc
 * Mit dieser Funktion wird der komplette Inhalt eines CAssocArray in dieses hineinkopiert.
 * Das Array wird vorher nicht gelöscht, so dass vorhandene Schlüssel erhalten bleiben, jedoch
 * können diese überschrieben werden, wenn das hinzuzufügende Array gleichnamige Schlüssel
 * enthält.
 *
 * \param[in] a Pointer auf das zu kopierende CAssocArray
 *
 * \returns Bei Erfolg liefert die die Funktion true (1) zurück, im Fehlerfall false (0).
 */
{
	CArrayItem *ptr;
	if (!a) {
		SetError(194,"int CAssocArray::Copy(const CAssocArray *a)");
		return 0;
	}
	CTreeWalker walk;
	a->Reset(walk);
	while ((ptr=(CArrayItem *)a->GetNext(walk))) {
		if (ptr->value!=NULL) {
			if (ptr->type==datatype::ARRAY) {
				if (!Set(ptr->key,(CAssocArray *)ptr->value,true)) return 0;
			} else if (ptr->type==datatype::CSTRING) {
				if (!Set(ptr->key,*(CString *)ptr->value)) return 0;
			} else if (ptr->type==datatype::CWSTRING) {
				if (!Set(ptr->key,*(CWString *)ptr->value)) return 0;
			} else if (ptr->type==datatype::BINARY) {
				if (!Set(ptr->key,(CBinary *)ptr->value,true)) return 0;
			} else if (ptr->type==datatype::POINTER) {
				if (!SetPointer(ptr->key,ptr->value)) return 0;
			}
		}
	}
	return 1;
}
int CAssocArray::Merge(const char *key, const CAssocArray &a)
/*!\brief CAssocArray kopieren
 *
 * \desc
 * Mit dieser Funktion wird der komplette Inhalt eines CAssocArray in den angegebenen
 * Schlüssel hineinkopiert. Falls der Schlüssel bereits vorhanden ist, bleiben dessen
 * Werte erhalten, können jedoch überschrieben werden, wenn das hinzuzufügende Array
 * gleichnamige Schlüssel enthält.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] a Pointer auf das zu kopierende CAssocArray
 *
 * \returns Bei Erfolg liefert die die Funktion true (1) zurück, im Fehlerfall false (0).*
 */
{
	CAssocArray *my=GetArray(key);
	if (my) return my->Copy(a);
	return Set(key,a);
}

int CAssocArray::Copy (const char *key, const CAssocArray &a, const char *sourcekey)
/*!\brief Einzelnen Schlüssel eines CAssocArray kopieren
 *
 * \desc
 * Mit dieser Funktion wird ein einzelner Schlüssel eines CAssocArray in den angegebenen
 * Schlüssel hineinkopiert.
 *
 * \param[in] key Name des Ziel-Schlüssels in diesem Array
 * \param[in] a Pointer auf das zu kopierende CAssocArray
 * \param[in] sourcekey Optionaler Name des Schlüssels im angegebenen Array, der kopiert werden
 * soll. Wird er nicht angegeben wird als Quell-Schlüssel der Wert von \p key übernommen.
 *
 * \returns Bei Erfolg liefert die die Funktion true (1) zurück, im Fehlerfall false (0).
 */
{
    if (!key) return 0;
    if (!sourcekey) sourcekey=key;
	CArrayItem *ptr=(CArrayItem *)a.GetValue(sourcekey);
	if (!ptr) return 0;
	if (!ptr->value) return 0;
	if (ptr->type==datatype::CSTRING) {
		return Set(key,*(CString*)ptr->value);
	} else if (ptr->type==datatype::CWSTRING) {
		return Set(key,*(CWString*)ptr->value);
	} else if (ptr->type==datatype::ARRAY) {
		return Set(key,(CAssocArray*)ptr->value,true);
	} else if (ptr->type==datatype::BINARY) {
		return Set(key,(CBinary*)ptr->value,true);
	} else if (ptr->type==datatype::POINTER) {
		return SetPointer(key,ptr->value);
	}
	return 0;
}

int CAssocArray::Copy(const CArray &a)
/*!\brief CArray kopieren
 *
 * \desc
 * Mit dieser Funktion wird der komplette Inhalt eines CArray in dieses hineinkopiert.
 * Das CAssocArray wird vorher nicht gelöscht, so dass vorhandene Schlüssel erhalten bleiben, jedoch
 * können diese überschrieben werden, wenn das hinzuzufügende Array gleichnamige Schlüssel
 * enthält.
 *
 * Da CAssocArray als Datentyp kein CArray unterstützt, wird dieses automatisch in ein CAssocArray
 * umgewandelt.
 *
 * \param[in] a Pointer auf das zu kopierende CArray
 *
 * \returns Bei Erfolg liefert die die Funktion true (1) zurück, im Fehlerfall false (0).
 *
 */
{
	CArrayItem *ptr;
	CString key;
	const char *value;
	int num=a.Num();
	for (int i=0;i<num;i++) {
		value=a[i];
		key.Sprintf("%i",i);
		ptr=(CArrayItem*)CreateTree((const char*)key.GetPtr());
		if (!ptr) {
			return 0;
		}
		ptr->Clear();
		CString *str=new CString;
		if (!str) {
			SetError(2);
			return 0;
		}
		ptr->value=str;
		ptr->type=datatype::CSTRING;
		str->Set(value);
	}
	return 1;
}

int CAssocArray::Deletef(const char *fmt, ...)
/*!\brief Einzelnen Schlüssel löschen
 *
 * \desc
 * Mit dieser Funktion wird ein einzelner Schlüssel aus dem Array gelöscht.
 *
 * \param[in] fmt Pointer auf einen Formatstring, den den Namen des zu löschenden Schlüssels
 * enthält.
 * \param[in] ... Beliebig viele Parameter für den Formatstring
 *
 * \returns Bei Erfolg liefert die die Funktion true (1) zurück, im Fehlerfall false (0).*
 */
{
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return Delete((const char*)key);
}

int CAssocArray::Delete(const char *key)
/*!\brief Einzelnen Schlüssel löschen
 *
 * \desc
 * Mit dieser Funktion wird ein einzelner Schlüssel aus dem Array gelöscht.
 *
 * \param[in] key Pointer auf den Namen des zu löschenden Schlüssels
 *
 * \returns Bei Erfolg liefert die die Funktion true (1) zurück, im Fehlerfall false (0).*
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		return 0;
	}
	delete ptr;
	num--;
	return 1;
}

void CAssocArray::Reset()
/*!\brief Zeiger für das Durchwandern des Arrays zurücksetzen
 *
 * \desc
 * Mit dieser Funktion wird der interne Pointer, der zum Durchwandern des Arrays mit den
 * Funktion GetNext, GetPrevious, usw. auf den Anfang zurückgesetzt.
 *
 */
{
	Tree.Reset();
}

ARRAY_RESULT CAssocArray::GetFirst()
/*!\brief Erstes Element aus dem Array holen
 *
 * \desc
 * Diese Funktion liefert das erste Element aus dem Array zurück. Mit folgenden Funktion kann der
 * Datentyp ermittelt werden:
 * - CAssocArray::IsCString
 * - CAssocArray::IsCWString
 * - CAssocArray::IsArray
 * - CAssocArray::IsBinary
 * - CAssocArray::IsPointer
 * - CAssocArray::GetDatatype
 *
 * Mit folgenden Funktionen kann der Inhalt des Wertes ausgelesen werden:
 * - CAssocArray::GetString
 * - CAssocArray::GetCString
 * - CAssocArray::GetCWString
 * - CAssocArray::GetChar
 * - CAssocArray::GetArray
 * - CAssocArray::GetPointer
 * - CAssocArray::GetBinary
 *
 * \result Pointer auf das erste Element des Arrays oder NULL, wenn das Array leer ist.
 *
 * \see
 * - CAssocArray::GetNext
 * - CAssocArray::GetLast
 * - CAssocArray::GetPrevious
 * - CAssocArray::Reset
 */
{
	Tree.Reset();
	return GetNext();
}

ARRAY_RESULT CAssocArray::GetNext()
/*!\brief Nächstes Element aus dem Array holen
 *
 * \desc
 * Diese Funktion liefert das nächste Element aus dem Array zurück.
 *
 * \result Pointer auf das letzte Element des Arrays oder NULL, wenn kein weiteres
 * Element vorhanden ist.
 *
 * \see
 * - CAssocArray::GetFirst
 * - CAssocArray::GetLast
 * - CAssocArray::GetPrevious
 * - CAssocArray::Reset
 */
{
	return (ARRAY_RESULT)Tree.GetNext();
}

ARRAY_RESULT CAssocArray::GetLast()
/*!\brief Letztes Element aus dem Array holen
 *
 * \desc
 * Diese Funktion liefert das letzte Element aus dem Array zurück.
 *
 * \result Pointer auf das letzte Element des Arrays oder NULL, wenn das Array leer ist.
 *
 * \see
 * - CAssocArray::GetFirst
 * - CAssocArray::GetNext
 * - CAssocArray::GetPrevious
 * - CAssocArray::Reset
 *
 */
{
	Tree.Reset();
	return GetPrevious();
}

ARRAY_RESULT CAssocArray::GetPrevious()
/*!\brief Vorhergehendes Element aus dem Array holen
 *
 * \desc
 * Diese Funktion liefert das vorhergehende Element aus dem Array zurück.
 *
 * \result Pointer auf das vorhergehende Element des Arrays oder NULL, wenn kein weiteres
 * Element vorhanden ist.
 *
 * \see
 * - CAssocArray::GetFirst
 * - CAssocArray::GetNext
 * - CAssocArray::GetPrevious
 * - CAssocArray::Reset
 *
 */
{
	return (ARRAY_RESULT)Tree.GetPrevious();
}

void CAssocArray::Reset(CTreeWalker &walk) const
{
	Tree.Reset(walk);
}

ARRAY_RESULT CAssocArray::GetFirst(CTreeWalker &walk) const
{
	Tree.Reset(walk);
	return GetNext(walk);
}

ARRAY_RESULT CAssocArray::GetNext(CTreeWalker &walk) const
{
	return (ARRAY_RESULT)Tree.GetNext(walk);
}

int CAssocArray::GetFirstArray(CAssocArray &res)
{
	Tree.Reset();
	return GetNextArray(res);
}

int CAssocArray::GetNextArray(CAssocArray &res)
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetNext())) {
		if (ptr->type==datatype::ARRAY) {
			res.Clear();
			res.Copy((CAssocArray*)ptr->value);
			return 1;
		}
	}
	return 0;
}

int CAssocArray::GetLastArray(CAssocArray &res)
{
	Tree.Reset();
	return GetPreviousArray(res);
}

int CAssocArray::GetPreviousArray(CAssocArray &res)
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetPrevious())) {
		if (ptr->type==datatype::ARRAY) {
			res.Clear();
			res.Copy((CAssocArray*)ptr->value);
			return 1;
		}
	}
	return 0;
}


CAssocArray *CAssocArray::GetFirstArray()
/*!\brief Erstes CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das erste Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das erste Array oder NULL, wenn das Array leer ist oder kein CAssocArray
 * enthält.
 *
 * \see
 * - CAssocArray::GetNextArray
 * - CAssocArray::GetLastArray
 * - CAssocArray::GetPreviousArray
 * - CAssocArray::Reset
 */
{
	Tree.Reset();
	return GetNextArray();
}

CAssocArray *CAssocArray::GetNextArray()
/*!\brief Nächstes CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das nächste Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das nächste Array oder NULL, wenn kein weiteres Array vorhanden ist.
 *
 * \see
 * - CAssocArray::GetFirstArray
 * - CAssocArray::GetLastArray
 * - CAssocArray::GetPreviousArray
 * - CAssocArray::Reset
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetNext())) {
		if (ptr->type==datatype::ARRAY) return (CAssocArray*)ptr->value;
	}
	return NULL;
}

CAssocArray *CAssocArray::GetLastArray()
/*!\brief Letztes CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das letzte Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das letzte Array oder NULL, wenn das Array leer ist oder kein CAssocArray
 * enthält.
 *
 * \see
 * - CAssocArray::GetFirstArray
 * - CAssocArray::GetNextArray
 * - CAssocArray::GetPreviousArray
 * - CAssocArray::Reset
 */
{
	Tree.Reset();
	return GetPreviousArray();
}

CAssocArray *CAssocArray::GetPreviousArray()
/*!\brief Vorheriges CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das vorhergehende Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das vorhergehende Array oder NULL, wenn kein weiteres Array vorhanden ist.
 *
 * \see
 * - CAssocArray::GetFirstArray
 * - CAssocArray::GetNextArray
 * - CAssocArray::GetLastArray
 * - CAssocArray::Reset
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetPrevious())) {
		if (ptr->type==datatype::ARRAY) return (CAssocArray*)ptr->value;
	}
	return NULL;
}


CAssocArray *CAssocArray::GetFirstArray(CTreeWalker &walk) const
/*!\brief Erstes CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das erste Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das erste Array oder NULL, wenn das Array leer ist oder kein CAssocArray
 * enthält.
 *
 * \see
 * - CAssocArray::GetNextArray
 * - CAssocArray::GetLastArray
 * - CAssocArray::GetPreviousArray
 * - CAssocArray::Reset
 */
{
	Tree.Reset(walk);
	return GetNextArray(walk);
}

CAssocArray *CAssocArray::GetNextArray(CTreeWalker &walk) const
/*!\brief Nächstes CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das nächste Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das nächste Array oder NULL, wenn kein weiteres Array vorhanden ist.
 *
 * \see
 * - CAssocArray::GetFirstArray
 * - CAssocArray::GetLastArray
 * - CAssocArray::GetPreviousArray
 * - CAssocArray::Reset
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetNext(walk))) {
		if (ptr->type==datatype::ARRAY) return (CAssocArray*)ptr->value;
	}
	return NULL;
}

CAssocArray *CAssocArray::GetLastArray(CTreeWalker &walk) const
/*!\brief Letztes CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das letzte Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das letzte Array oder NULL, wenn das Array leer ist oder kein CAssocArray
 * enthält.
 *
 * \see
 * - CAssocArray::GetFirstArray
 * - CAssocArray::GetNextArray
 * - CAssocArray::GetPreviousArray
 * - CAssocArray::Reset
 */
{
	Tree.Reset(walk);
	return GetPreviousArray(walk);
}

CAssocArray *CAssocArray::GetPreviousArray(CTreeWalker &walk) const
/*!\brief Vorheriges CAssocArray innerhalb dieses Arrays finden
 *
 * \desc
 * Diese Funktion liefert das vorhergehende Element aus dem Array zurück, das vom Datentyp CAssocArray
 * ist.
 *
 * \result Pointer auf das vorhergehende Array oder NULL, wenn kein weiteres Array vorhanden ist.
 *
 * \see
 * - CAssocArray::GetFirstArray
 * - CAssocArray::GetNextArray
 * - CAssocArray::GetLastArray
 * - CAssocArray::Reset
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetPrevious(walk))) {
		if (ptr->type==datatype::ARRAY) return (CAssocArray*)ptr->value;
	}
	return NULL;
}


int CAssocArray::GetFirst(char **key, char **value)
/*!\brief Ersten String im Array finden und Key und Value zurückgeben
 *
 * \desc
 * Diese Funktion sucht den ersten String im Array und speichert Pointer auf dessen Key und Value an den
 * angegebenen Adressen.
 *
 * \param[in] key Pointer auf einen Charpointer, in dem der Pointer auf den Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen Charpointer, in dem der Pointer auf den Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind aufsteigen sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	Reset();
	return GetNext(key,value);
}

int CAssocArray::GetFirst(CString *key, CString *value)
/*!\brief Ersten String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den ersten String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Pointer auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind aufsteigen sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	Reset();
	return GetNext(key,value);
}

int CAssocArray::GetFirst(CString &key, CString &value)
/*!\brief Ersten String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den ersten String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Pointer auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind aufsteigen sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	Reset();
	return GetNext(key,value);
}
int CAssocArray::GetNext(char **key, char **value)
/*!\brief Nächsten String im Array finden und Key und Value zurückgeben
 *
 * \desc
 * Diese Funktion sucht den nächsten String im Array und speichert Pointer auf dessen Key und Value an den
 * angegebenen Adressen.
 *
 * \param[in] key Pointer auf einen Charpointer, in dem der Pointer auf den Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen Charpointer, in dem der Pointer auf den Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind aufsteigen sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetNext())) {
		if (ptr->type==datatype::CSTRING) {
			if (key) *key=(char*)ptr->key.GetPtr();
			if (value) *value=(char*)((CString*)ptr->value)->GetPtr();
			return 1;
		} else if (ptr->type==datatype::CWSTRING) {
			if (key) *key=(char*)ptr->key.GetPtr();
			if (value) *value=(char*)((CWString*)ptr->value)->GetPtr();
			return 1;
		}
	}
	return 0;
}

int CAssocArray::GetNext(CString *key, CString *value)
/*!\brief Ersten String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den ersten String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Pointer auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind aufsteigen sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetNext())) {
		if (ptr->type==datatype::CSTRING) {
			if (key) key->Set(ptr->key.GetPtr());
			if (value) value->Set(((CString*)ptr->value)->GetPtr());
			return 1;
		} else if (ptr->type==datatype::CWSTRING) {
				if (key) key->Set(ptr->key.GetPtr());
				if (value) value->Set(((CWString*)ptr->value)->GetPtr());
				return 1;
		}
	}
	return 0;
}

int CAssocArray::GetNext(CString &key, CString &value)
/*!\brief Ersten String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den ersten String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Pointer auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind aufsteigen sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetNext())) {
		if (ptr->type==datatype::CSTRING) {
			key.Set(ptr->key.GetPtr());
			value.Set(((CString*)ptr->value)->GetPtr());
			return 1;
		} else if (ptr->type==datatype::CWSTRING) {
			key.Set(ptr->key.GetPtr());
			value.Set(((CWString*)ptr->value)->GetPtr());
			return 1;
		}
	}
	return 0;
}

int CAssocArray::GetLast(char **key, char **value)
/*!\brief Letzten String im Array finden und Key und Value zurückgeben
 *
 * \desc
 * Diese Funktion sucht den letzten String im Array und speichert Pointer auf dessen Key und Value an den
 * angegebenen Adressen.
 *
 * \param[in] key Pointer auf einen Charpointer, in dem der Pointer auf den Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen Charpointer, in dem der Pointer auf den Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind absteigend sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	Reset();
	return GetPrevious(key,value);
}

int CAssocArray::GetLast(CString *key, CString *value)
/*!\brief Letzten String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den letzten String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Pointer auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind absteigend sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	Reset();
	return GetPrevious(key,value);
}

int CAssocArray::GetLast(CString &key, CString &value)
/*!\brief Letzten String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den letzten String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Referenz auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Referenz auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind absteigend sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	Reset();
	return GetPrevious(key,value);
}

int CAssocArray::GetPrevious(char **key, char **value)
/*!\brief Vorherigen String im Array finden und Key und Value zurückgeben
 *
 * \desc
 * Diese Funktion sucht den vorherigen String im Array und speichert Pointer auf dessen Key und Value an den
 * angegebenen Adressen.
 *
 * \param[in] key Pointer auf einen Charpointer, in dem der Pointer auf den Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen Charpointer, in dem der Pointer auf den Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind absteigend sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetPrevious())) {
		if (ptr->type==datatype::CSTRING) {
			if (key) *key=(char*)ptr->key.GetPtr();
			if (value) *value=(char*)((CString*)ptr->value)->GetPtr();
			return 1;
		} else if (ptr->type==datatype::CWSTRING) {
				if (key) *key=(char*)ptr->key.GetPtr();
				if (value) *value=(char*)((CWString*)ptr->value)->GetPtr();
				return 1;
		}
	}
	return 0;
}

int CAssocArray::GetPrevious(CString *key, CString *value)
/*!\brief Vorherigen String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den vorherigen String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Pointer auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Pointer auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind absteigend sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetPrevious())) {
		if (ptr->type==datatype::CSTRING) {
			if (key) key->Set(ptr->key.GetPtr());
			if (value) value->Set(((CString*)ptr->value)->GetPtr());
			return 1;
		} else if (ptr->type==datatype::CWSTRING) {
				if (key) key->Set(ptr->key.GetPtr());
				if (value) value->Set(((CWString*)ptr->value)->GetPtr());
				return 1;
		}
	}
	return 0;
}

int CAssocArray::GetPrevious(CString &key, CString &value)
/*!\brief Vorherigen String im Array finden und Key und Value in CStrings speichern
 *
 * \desc
 * Diese Funktion sucht den vorherigen String im Array und speichert dessen Key und Value in den übergebenen
 * CStrings.
 *
 * \param[in] key Referenz auf einen CString in dem der Schlüsselname abgelegt werden soll
 * \param[in] value Referenz auf einen CString in dem der Wert abgelegt werden soll
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0).
 * \remarks
 * Die Funktion findet alle CStrings und CWStrings im Array. Der Rückgabewert \p value wird jedoch bei
 * CWString-Werten von Unicode nach UTF-8 konvertiert. Die Rückgabewerte sind absteigend sortiert nach
 * Schlüssel.
 *
 * \see
 * - int CAssocArray::GetFirst(char **key, char **value)
 * - int CAssocArray::GetNext(char **key, char **value)
 * - int CAssocArray::GetFirst(CString *key, CString *value)
 * - int CAssocArray::GetNext(CString *key, CString *value)
 * - int CAssocArray::GetLast(char **key, char **value)
 * - int CAssocArray::GetPrevious(char **key, char **value)
 * - int CAssocArray::GetLast(CString *key, CString *value)
 * - int CAssocArray::GetPrevious(CString *key, CString *value)
 */
{
	CArrayItem *ptr;
	while ((ptr=(CArrayItem*)Tree.GetPrevious())) {
		if (ptr->type==datatype::CSTRING) {
			key.Set(ptr->key.GetPtr());
			value.Set(((CString*)ptr->value)->GetPtr());
			return 1;
		} else if (ptr->type==datatype::CWSTRING) {
			key.Set(ptr->key.GetPtr());
			value.Set(((CWString*)ptr->value)->GetPtr());
			return 1;
		}
	}
	return 0;
}


CString *CAssocArray::GetString(ARRAY_RESULT res) const
/*!\brief CString eines Ergebnisses zurückgeben
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegeben Ergebnis um einen
 * CString handelt und gibt dessen Pointer zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \returns Falls es sich bei dem Ergebnis um einen CString gehandelt hat, wird dessen Pointer zurückgegeben.
 * Ist das nicht der Fall, wird NULL zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"CString *CAssocArray::GetString(ARRAY_RESULT res)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::CSTRING) {
		SetError(449);
		return NULL;
	}
	if (!ptr->value) {
		SetError(450);
		return NULL;
	}
	return (CString*)ptr->value;
}

CString *CAssocArray::GetCString(ARRAY_RESULT res) const
/*!\brief CString eines Ergebnisses zurückgeben
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegeben Ergebnis um einen
 * CString handelt und gibt dessen Pointer zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \returns Falls es sich bei dem Ergebnis um einen CString gehandelt hat, wird dessen Pointer zurückgegeben.
 * Ist das nicht der Fall, wird NULL zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"CString *CAssocArray::GetCString(ARRAY_RESULT res)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::CSTRING) {
		SetError(449);
		return NULL;
	}
	if (!ptr->value) {
		SetError(450);
		return NULL;
	}
	return (CString*)ptr->value;
}

CWString *CAssocArray::GetCWString(ARRAY_RESULT res) const
/*!\brief CWString eines Ergebnisses zurückgeben
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegeben Ergebnis um einen
 * CWString handelt und gibt dessen Pointer zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \returns Falls es sich bei dem Ergebnis um einen CWString gehandelt hat, wird dessen Pointer zurückgegeben.
 * Ist das nicht der Fall, wird NULL zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"CWString *CAssocArray::GetCWString(ARRAY_RESULT res)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::CWSTRING) {
		SetError(449);
		return NULL;
	}
	if (!ptr->value) {
		SetError(450);
		return NULL;
	}
	return (CWString*)ptr->value;
}

CAssocArray *CAssocArray::GetArray(ARRAY_RESULT res) const
/*!\brief CAssocArray eines Ergebnisses zurückgeben
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegeben Ergebnis um ein
 * CAssocArray handelt und gibt dessen Pointer zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \returns Falls es sich bei dem Ergebnis um ein CAssocArray gehandelt hat, wird dessen Pointer zurückgegeben.
 * Ist das nicht der Fall, wird NULL zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"CAssocArray *CAssocArray::GetArray(ARRAY_RESULT res)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::ARRAY) {
		SetError(449);
		return NULL;
	}
	if (!ptr->value) {
		SetError(450);
		return NULL;
	}
	return (CAssocArray*)ptr->value;
}

CBinary *CAssocArray::GetBinary(ARRAY_RESULT res) const
/*!\brief CBinary eines Ergebnisses zurückgeben
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegeben Ergebnis um ein
 * CBinary handelt und gibt dessen Pointer zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \returns Falls es sich bei dem Ergebnis um ein CBinary gehandelt hat, wird dessen Pointer zurückgegeben.
 * Ist das nicht der Fall, wird NULL zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"CBinary *CAssocArray::GetBinary(ARRAY_RESULT res)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::BINARY) {
		SetError(449);
		return NULL;
	}
	if (!ptr->value) {
		SetError(450);
		return NULL;
	}
	return (CBinary*)ptr->value;
}

const char *CAssocArray::GetChar(ARRAY_RESULT res) const
/*!\brief String eines Ergebnisses zurückgeben
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegeben Ergebnis um einen String handelt und gibt dessen
 * Char-Pointer Pointer zurück. Dabei kann es sich um einen direkten Char-Pointer, einen CString oder
 * CWString handeln.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \returns Falls es sich bei dem Ergebnis um einen String gehandelt hat, wird dessen Char-Pointer zurückgegeben.
 * Ist das nicht der Fall, wird NULL zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"const char *CAssocArray::GetChar(ARRAY_RESULT res)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (!ptr->value) {
		SetError(450);
		return NULL;
	}
	if (ptr->type==datatype::CSTRING) return ((CString*)ptr->value)->GetPtr();
	if (ptr->type==datatype::CWSTRING) return ((CWString*)ptr->value)->GetPtr();;
	if (ptr->type==datatype::CHAR) return (const char*)ptr->value;
	SetError(449);
	return NULL;
}

void *CAssocArray::GetPointer(ARRAY_RESULT res) const
/*!\brief Void-Pointer eines Ergebnisses zurückgeben
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegeben Ergebnis um einen
 * Void-Pointer handelt und gibt diesen zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \returns Falls es sich bei dem Ergebnis um einen Void-Pointer gehandelt hat, wird dessen Pointer zurückgegeben.
 * Ist das nicht der Fall, wird NULL zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"void *CAssocArray::GetPointer(ARRAY_RESULT res)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::POINTER) {
		SetError(449);
		return NULL;
	}
	if (!ptr->value) {
		SetError(450);
		return NULL;
	}
	return ptr->value;
}



const char *CAssocArray::GetKey(ARRAY_RESULT res, CString *str) const
/*!\brief Liefert den Schlüsselnamen eines Ergebnisses zurück
 *
 * \desc
 * Diese Funktion liefert den Schlüsselnamen des angegebenen Ergebnisses zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \param[in] str Optionaler Pointer auf einen CString, in dem der Schlüssel gespeichert werden soll. Wird er
 * nicht angegeben, wird nur der Pointer auf den Schlüsselnamen zurückgegeben.
 * \returns Gibt einen Pointer auf den Namen des Schlüssels zurück.
 */
{
	if (!res) {
		SetError(194,"const char *CAssocArray::GetKey(==> ARRAY_RESULT res <==, CString *str)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	//if (!ptr->value) return NULL;
	if (str) str->Set(ptr->key);
	return (const char*)ptr->key.GetPtr();
}

const char *CAssocArray::GetKey(ARRAY_RESULT res, CString &key) const
/*!\brief Liefert den Schlüsselnamen eines Ergebnisses zurück
 *
 * \desc
 * Diese Funktion liefert den Schlüsselnamen des angegebenen Ergebnisses zurück.
 *
 * \param[in] res Ergebnis aus einer vorhergehenden GetFirst*, GetNext*, etc. Funktion, die zum Durchwandern
 * des Arrays verwendet wurde.
 * \param[in] key Ein CString, in dem der Schlüssel gespeichert werden soll.
 * \returns Gibt einen Pointer auf den Namen des Schlüssels zurück.
 */
{
	if (!res) {
		SetError(194,"const char *CAssocArray::GetKey(==> ARRAY_RESULT res <==, CString *str)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	//if (!ptr->value) return NULL;
	key.Set(ptr->key);
	return (const char*)key.GetPtr();
}


const char *CAssocArray::GetCurrentKey(CString *str) const
/*!\brief Liefert den Namen des aktuellen Schlüssels zurück
 *
 * \desc
 * Die Funktion liefert den Namen des aktuellen Schlüssels zurück.
 *
 * \param[in] str Optionaler Pointer auf einen CString, in dem der Schlüssel gespeichert werden soll. Wird er
 * nicht angegeben, wird nur der Pointer auf den Schlüsselnamen zurückgegeben.
 * \returns Gibt einen Pointer auf den Namen des Schlüssels zurück.
 */
{
	CArrayItem *ptr=(CArrayItem*)Tree.GetCurrent();
	if (!ptr) return NULL;
	if (!ptr->value) return NULL;
	if (str) str->Set(ptr->key);
	return (const char*)ptr->key.GetPtr();
}

const char *CAssocArray::GetCurrentKey(CString &key) const
/*!\brief Liefert den Namen des aktuellen Schlüssels zurück
 *
 * \desc
 * Die Funktion liefert den Namen des aktuellen Schlüssels zurück.
 *
 * \param[in] key Ein CString, in dem der Schlüssel gespeichert werden soll.
 * \returns Gibt einen Pointer auf den Namen des Schlüssels zurück.
 */
{
	CArrayItem *ptr=(CArrayItem*)Tree.GetCurrent();
	if (!ptr) return NULL;
	if (!ptr->value) return NULL;
	key.Set(ptr->key);
	return (const char*)key.GetPtr();
}


const char *CAssocArray::GetChar(const char *key) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert einen Char-Pointer auf den Wert des angegebenen Schlüssels zurück. Der Datentyp im Array
 * kann sowohl CString als auch CWString sein, der Rückgabewert zeigt jedoch immer auf einen UTF-8-String.
 *
 * \param[in] key Pointer auf den Namen des gewünschten Schlüssels
 * \returns Liefert einen char-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 *
 */
{
	if (!key) {
		SetError(194,"CString *CAssocArray::GetString(==> const char *key <==)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		SetError(294,"%s",key);
		return NULL;
	}
	if (ptr->value==NULL) {
		SetError(450);
		return NULL;
	}
	if (ptr->type==datatype::CSTRING) {
		return ((CString*)ptr->value)->GetPtr();
	} else if (ptr->type==datatype::CWSTRING) {
		return ((CWString*)ptr->value)->GetPtr();
	} else if (ptr->type==datatype::CHAR) {
		return (const char *)ptr->value;
	}
	SetError(303);
	return NULL;
}

const char *CAssocArray::Get(const char *key) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert einen Char-Pointer auf den Wert des angegebenen Schlüssels zurück. Der Datentyp im Array
 * kann sowohl CString als auch CWString sein, der Rückgabewert zeigt jedoch immer auf einen UTF-8-String.
 *
 * \param[in] key Pointer auf den Namen des gewünschten Schlüssels
 * \returns Liefert einen char-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!key) {
		SetError(194,"CString *CAssocArray::GetString(==> const char *key <==)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		SetError(294,"%s",key);
		return NULL;
	}
	if (ptr->value==NULL) {
		SetError(450);
		return NULL;
	}
	if (ptr->type==datatype::CSTRING) {
		return (char*)((CString*)ptr->value)->GetPtr();
	} else if (ptr->type==datatype::CWSTRING) {
		return (char*)((CWString*)ptr->value)->GetPtr();
	} else if (ptr->type==datatype::CHAR) {
		return (char *)ptr->value;
	}
	SetError(303);
	return NULL;
}

const char *CAssocArray::Getf(const char *fmt, ...) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert einen Char-Pointer auf den Wert des angegebenen Schlüssels zurück. Der Datentyp im Array
 * kann sowohl CString als auch CWString sein, der Rückgabewert zeigt jedoch immer auf einen UTF-8-String.
 *
 * \param[in] fmt Pointer auf einen Formatstring, der den Namen des gewünschten Schlüssels ergibt
 * \param[in] ... Beliebig viele weitere Parameter, die im Formatstring benötigt werden.
 *
 * \returns Liefert einen char-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!fmt) {
		SetError(194,"char *CAssocArray::Getf(==> const char *fmt <==, ...)");
	}
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return Get((const char*)key);
}

CString *CAssocArray::GetStringf(const char *fmt, ...) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf den CString des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
  *
 * \param[in] fmt Pointer auf einen Formatstring, der den Namen des gewünschten Schlüssels ergibt
 * \param[in] ... Beliebig viele weitere Parameter, die im Formatstring benötigt werden.
 *
 * \returns Liefert einen CString-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return GetString((const char*)key);
}

CString *CAssocArray::GetString(const char *key) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf den CString des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] key Pointer auf den Namen des Schlüssels
 *
 * \returns Liefert einen CString-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!key) {
		SetError(194,"CString *CAssocArray::GetString(==> const char *key <==)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		SetError(294,"%s",key);
		return NULL;
	}
	if (ptr->type!=datatype::CSTRING) {
		SetError(303,"%s",key);
		return NULL;
	}
	if (ptr->value!=NULL) {
		return (CString*)ptr->value;
	}
	SetError(294,"%s",key);
	return NULL;
}

CString *CAssocArray::GetCStringf(const char *fmt, ...) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \copydoc CAssocArray::GetStringf(const char *fmt, ...)
 */
{
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return GetString(key);
}

CString *CAssocArray::GetCString(const char *key) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \copydoc CAssocArray::GetString(const char *key) const
 */
{
	return GetString(key);
}

CWString *CAssocArray::GetCWStringf(const char *fmt, ...) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf den CWString des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
  *
 * \param[in] fmt Pointer auf einen Formatstring, der den Namen des gewünschten Schlüssels ergibt
 * \param[in] ... Beliebig viele weitere Parameter, die im Formatstring benötigt werden.
 *
 * \returns Liefert einen CWString-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return GetCWString((const char *)key);
}

CWString *CAssocArray::GetCWString(const char *key) const
/*!\brief Liefert den Wert zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf den CWString des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] key Pointer auf den Namen des Schlüssels
 *
 * \returns Liefert einen CString-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!key) {
		SetError(194,"CString *CAssocArray::GetString(==> const char *key <==)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		SetError(294,"%s",key);
		return NULL;
	}
	if (ptr->type!=datatype::CWSTRING) {
		SetError(303,"%s",key);
		return NULL;
	}
	if (ptr->value!=NULL) {
		return (CWString*)ptr->value;
	}
	SetError(294,"%s",key);
	return NULL;
}

CAssocArray *CAssocArray::GetArrayf(const char *fmt, ...) const
/*!\brief Liefert das Assoziative Array zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf das CAssocArray des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] fmt Pointer auf einen Formatstring, der den Namen des gewünschten Schlüssels ergibt
 * \param[in] ... Beliebig viele weitere Parameter, die im Formatstring benötigt werden.
 *
 * \returns Liefert einen CAssocArray-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!fmt) {
		SetError(194,"const char *fmt");
		return NULL;
	}
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return GetArray(key);
}

CAssocArray *CAssocArray::GetArray(const char *key) const
/*!\brief Liefert das Assoziative Array zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf das CAssocArray des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] key Pointer auf den Namen des Schlüssels
 * \returns Liefert einen CAssocArray-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!key) {
		SetError(194,"CAssocArray *CAssocArray::GetArray(==> const char *key <==)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		SetError(294,"%s",key);
		return NULL;
	}
	if (ptr->type!=PPL_ARRAY_ARRAY) {
		SetError(304,"%s",key);
		return NULL;
	}
	if (ptr->value!=NULL) {
		return (CAssocArray*)ptr->value;
	}
	SetError(294,"%s",key);
	return NULL;
}

CBinary *CAssocArray::GetBinary(const char *key) const
/*!\brief Liefert das Binary-Object zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf das CBinary-Objekt des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] key Pointer auf den Namen des Schlüssels
 * \returns Liefert einen CBinary-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!key) {
		SetError(194,"CBinary *CAssocArray::GetBinary(==> const char *key <==)");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		SetError(294,"%s",key);
		return NULL;
	}
	if (ptr->type!=datatype::BINARY) {
		SetError(358,"%s",key);
		return NULL;
	}
	if (ptr->value!=NULL) {
		return (CBinary*)ptr->value;
	}
	SetError(294,"%s",key);
	return NULL;
}

CBinary *CAssocArray::GetBinaryf(const char *fmt, ...) const
/*!\brief Liefert das Binary-Object zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf das CBinary-Objekt des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] fmt Pointer auf einen Formatstring, der den Namen des gewünschten Schlüssels ergibt
 * \param[in] ... Beliebig viele weitere Parameter, die im Formatstring benötigt werden.
 * \returns Liefert einen CBinary-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return GetBinary(key);
}

ARRAY_RESULT CAssocArray::GetValue(const char *key) const
/*!\brief Liefert einen Pointer auf das Element zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Pointer auf Element zum angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist. Der Inhalt des Elements kann mit den verschiedenen Is...-Funktionen
 * geprüft (z.B. IsString) und mit Get... ausgelesen werden (z.B. GetString).
 *
 * \param[in] key Pointer auf den Namen des Schlüssels
 * \returns Liefert einen Pointer von Typ ARRAY_RESULT auf den Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden ist.
 */
{
	if (!key) {
		SetError(194,"ARRAY_RESULT CAssocArray::GetValue(==> const char *key <==)");
		return NULL;
	}
	return FindInternal(key);
}

int CAssocArray::GetKey(CString &key, const char *value, bool recursive)
/*!\brief Liefert den ersten passenden Key, auf den value matched
 *
 * \desc
 * Diese Funktion durchsucht alle Strings im Array und prüft, ob dessen Wert mit dem gewünschten
 * \p value übereinstimmt. Der Schlüssel-Name des ersten passenden Elements wird zurückgegeben.
 *
 * \param[out] key In diesem String wird der gefundene Schlüssel gespeichert
 * \param[in] value Der gesuchte Wert
 * \param[in] recursive Optionaler Parameter. Wenn gesetzt, werden alle Ebenen des Arrays
 * rekursiv durchsucht. Wenn nicht (=default), wird nur die erste Ebene durchsucht.
 * \returns Wurde der Wert gefunden, gibt die Funktion true (1) zurück und der Name des
 * Schlüssels wird in \p key gespeichert. Konnte der Wert nicht gefunden werden, gibt die
 * Funktion false (0) zurück und \p key bleibt unverändert.
 */
{
	if (!value) {
		SetError(194,"int CAssocArray::GetKey(CString *key, ==> const char *value <==, bool recursive)");
		return 0;
	}

	CArrayItem *p=(CArrayItem*)Tree.GetFirst();
	while (p) {
		if (p->type==datatype::CSTRING) {
			CString *str=(CString*)p->value;
			if (str->StrCaseCmp((char*)value)==0) {
				key.Set(p->key);
				return 1;
			}
		} else if (p->type==datatype::CWSTRING) {
			CWString *str=(CWString*)p->value;
			if (str->StrCaseCmp((char*)value)==0) {
				key.Set(p->key);
				return 1;
			}
		} else if (p->type==datatype::ARRAY && recursive==true) {
			CString k;
			if (((CAssocArray *)p->value)->GetKey(k,value,recursive)) {
				key.Setf("%s/%s",p->key.GetPtr(),(const char*)k);
				return 1;
			}
		}
		p=(CArrayItem*)Tree.GetNext();
	}
	return 0;
}


void *CAssocArray::GetPointer(const char *key) const
/*!\brief Liefert den Void-Pointer zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Void-Pointer des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] key Pointer auf den Namen des Schlüssels
 * \returns Liefert einen Void-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!key) {
		SetError(194,"const char *key");
		return NULL;
	}
	CArrayItem *ptr=(CArrayItem*)FindInternal(key);
	if (!ptr) {
		SetError(294,"%s",key);
		return NULL;
	}
	if (ptr->type!=datatype::POINTER) {
		SetError(340,"%s",key);
		return NULL;
	}
	SetError(0);
	return ptr->value;
}

void *CAssocArray::GetPointerf(const char *fmt, ...) const
/*!\brief Liefert den Void-Pointer zum angegebenen Schlüssel zurück
 *
 * \desc
 * Diese Funktion liefert den Void-Pointer des angegebenen Schlüssels zurück, sofern
 * der Schlüssel vorhanden ist und diesem Datentyp entspricht.
 *
 * \param[in] fmt Pointer auf einen Formatstring, der den Namen des gewünschten Schlüssels ergibt
 * \param[in] ... Beliebig viele weitere Parameter, die im Formatstring benötigt werden.
 * \returns Liefert einen Void-Pointer auf den Wert des Schlüssels zurück, oder NULL, wenn
 * der Schlüssel nicht vorhanden, leer oder ein anderer Datentyp ist.
 */
{
	if (!fmt) {
		SetError(194,"const char *fmt");
		return NULL;
	}
	CString key;
	va_list args;
	va_start(args, fmt);
	key.Vasprintf(fmt,args);
	va_end(args);
	return GetPointer(key);
}

int CAssocArray::GetDatatype(ARRAY_RESULT res) const
/*!\brief Datentyp eines Elements zurückliefern
 *
 * \desc
 * Diese Funktion liefert den Datentyps eines Elements zurück, dass zuvor über die Auslesefunktionen
 * GetFirst, GetNext ausgelesen wurde.
 *
 * \param[in] res Ergebnis einer vorherigen Abfrage mit GetFirst, GetNext, GetLast oder
 * GetPrevious
 * \returns Datentyp ppl6::datatype
 */
{
	if (!res) {
		SetError(194,"int CAssocArray::GetDatatype(==> void *res <==)");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (!ptr->value) return 0;
	return ptr->type;
}

int CAssocArray::IsString(ARRAY_RESULT res) const
/*!\brief Prüft, ob es sich bei dem Ergebnis um einen CString handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegebenen Erfebnis um einen
 * CString handelt.
 *
 * \param[in] res Ergebnis einer vorherigen Abfrage mit GetFirst, GetNext, GetLast oder
 * GetPrevious
 * \returns Handelt es sich um den gewünschten Datentyp, gibt die Funktion true (1)
 * zurück. Wenn nicht, wird false (0) zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"int CAssocArray::IsString(==> void *res <==)");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::CSTRING) {
		SetError(449);
		return 0;
	}
	if (!ptr->value) {
		SetError(450);
		return 0;
	}
	return 1;
}

int CAssocArray::IsCString(ARRAY_RESULT res) const
/*!\brief Prüft, ob es sich bei dem Ergebnis um einen CString handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegebenen Erfebnis um einen
 * CString handelt.
 *
 * \param[in] res Ergebnis einer vorherigen Abfrage mit GetFirst, GetNext, GetLast oder
 * GetPrevious
 * \returns Handelt es sich um den gewünschten Datentyp, gibt die Funktion true (1)
 * zurück. Wenn nicht, wird false (0) zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"int CAssocArray::IsCString(==> void *res <==)");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::CSTRING) {
		SetError(449);
		return 0;
	}
	if (!ptr->value) {
		SetError(450);
		return 0;
	}
	return 1;
}

int CAssocArray::IsCWString(ARRAY_RESULT res) const
/*!\brief Prüft, ob es sich bei dem Ergebnis um einen CWString handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegebenen Erfebnis um einen
 * CWString handelt.
 *
 * \param[in] res Ergebnis einer vorherigen Abfrage mit GetFirst, GetNext, GetLast oder
 * GetPrevious
 * \returns Handelt es sich um den gewünschten Datentyp, gibt die Funktion true (1)
 * zurück. Wenn nicht, wird false (0) zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"int CAssocArray::IsCWString(==> void *res <==)");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::CWSTRING) {
		SetError(449);
		return 0;
	}
	if (!ptr->value) {
		SetError(450);
		return 0;
	}
	return 1;
}

int CAssocArray::IsArray(ARRAY_RESULT res) const
/*!\brief Prüft, ob es sich bei dem Ergebnis um einen CAssocArray handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegebenen Erfebnis um ein
 * CAssocArray handelt.
 *
 * \param[in] res Ergebnis einer vorherigen Abfrage mit GetFirst, GetNext, GetLast oder
 * GetPrevious
 * \returns Handelt es sich um den gewünschten Datentyp, gibt die Funktion true (1)
 * zurück. Wenn nicht, wird false (0) zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"int CAssocArray::IsArray(==> void *res <==)");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::ARRAY) {
		SetError(449);
		return 0;
	}
	if (!ptr->value) {
		SetError(450);
		return 0;
	}
	return 1;
}

int CAssocArray::IsPointer(ARRAY_RESULT res) const
/*!\brief Prüft, ob es sich bei dem Ergebnis um einen Pointer handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegebenen Erfebnis um einen
 * Pointer handelt.
 *
 * \param[in] res Ergebnis einer vorherigen Abfrage mit GetFirst, GetNext, GetLast oder
 * GetPrevious
 * \returns Handelt es sich um den gewünschten Datentyp, gibt die Funktion true (1)
 * zurück. Wenn nicht, wird false (0) zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"int CAssocArray::IsPointer(==> void *res <==)");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::POINTER) {
		SetError(449);
		return 0;
	}
	if (!ptr->value) {
		SetError(450);
		return 0;
	}
	return 1;
}

int CAssocArray::IsBinary(ARRAY_RESULT res) const
/*!\brief Prüft, ob es sich bei dem Ergebnis um ein CBinary handelt
 *
 * \desc
 * Diese Funktion prüft, ob es sich bei dem angegebenen Erfebnis um einen
 * CBinary handelt.
 *
 * \param[in] res Ergebnis einer vorherigen Abfrage mit GetFirst, GetNext, GetLast oder
 * GetPrevious
 * \returns Handelt es sich um den gewünschten Datentyp, gibt die Funktion true (1)
 * zurück. Wenn nicht, wird false (0) zurückgegeben und ein Fehlercode gesetzt.
 */
{
	if (!res) {
		SetError(194,"int CAssocArray::IsBinary(==> void *res <==)");
		return 0;
	}
	CArrayItem *ptr=(CArrayItem*)res;
	if (ptr->type!=datatype::BINARY) {
		SetError(449);
		return 0;
	}
	if (!ptr->value) {
		SetError(450);
		return 0;
	}
	return 1;
}

int CAssocArray::IsTrue(const char *key) const
/*!\brief Prüft, ob der Schlüssel vorhanden und "wahr" ist
 *
 * \desc
 * Diese Funktion prüft, ob der angegeben Schlüssel im Array vorhanden ist und dessen
 * Wert "wahr" ist. Dies ist der Fall, wenn eine der folgenden Bedingungen erfüllt ist:
 * - Der String enthält eine Ziffer ungleich 0
 * - Der String enthält das Wort "true" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "wahr" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "yes" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "ja" (Gross- oder Kleingeschrieben)
 * - Der String enthält das Wort "on" (Gross- oder Kleingeschrieben)
 *
 * \param[in] key Pointer auf den gewünschten Schlüssel
 * \returns Liefert true (1) zurück, wenn der Schlüssel "wahr" ist, oder false, wenn
 * er entweder nicht vorhanden ist, keinen Wert enthält oder nicht den oben
 * genannten Bedingungen entspricht. Ein Fehlercode wird nicht gesetzt.
 */
{
	const CArrayItem *ptr=(const CArrayItem*)GetValue(key);
	if (!ptr) return 0;
	if (ptr->type==datatype::ARRAY) return 1;
	if (ptr->value==NULL) return 0;
	if (ptr->type==datatype::CSTRING) {
		CString *str=(CString*)ptr->value;
		if (str->Len()==0) return 0;
		if (str->ToInt()!=0) return 1;
		if (str->StrCaseCmp((char*)"true")==0) return 1;
		if (str->StrCaseCmp((char*)"wahr")==0) return 1;
		if (str->StrCaseCmp((char*)"yes")==0) return 1;
		if (str->StrCaseCmp((char*)"ja")==0) return 1;
		if (str->StrCaseCmp((char*)"on")==0) return 1;
		if (str->StrCaseCmp((char*)"t")==0) return 1;
	} else if (ptr->type==datatype::CWSTRING) {
		CWString *str=(CWString*)ptr->value;
		if (str->Len()==0) return 0;
		if (str->ToInt()!=0) return 1;
		if (str->StrCaseCmp((char*)"true")==0) return 1;
		if (str->StrCaseCmp((char*)"wahr")==0) return 1;
		if (str->StrCaseCmp((char*)"yes")==0) return 1;
		if (str->StrCaseCmp((char*)"ja")==0) return 1;
		if (str->StrCaseCmp((char*)"on")==0) return 1;
		if (str->StrCaseCmp((char*)"t")==0) return 1;
	}
	return 0;
}

int CAssocArray::IsFalse(const char *key) const
/*!\brief Prüft, ob der Schlüssel vorhanden und "falsch" ist
 *
 * \desc
 * Diese Funktion prüft, ob der angegeben Schlüssel im Array vorhanden ist und dessen
 * Wert "falsch" ist. Dies ist der Fall, wenn eine der folgenden Bedingungen erfüllt ist:
 * - Der String ist leer
 * - Der String enthält die Ziffer 0
 * - Der String enthält kein der Worte "true", "wahr", "yes", "ja", "on", wobei Gross-/Kleingeschrieben
 *   keine Rolle spielt
 *
 * \param[in] key Pointer auf den gewünschten Schlüssel
 * \returns Liefert true (1) zurück, wenn der Schlüssel "falsch" oder nicht vorhanden ist,
 * andernfalls false.
 */
{
	if (IsTrue(key)) return 0;
	return 1;
}

int CAssocArray::Set(const char *key, const CDirEntry &d)
/*!\brief Verzeichnis-Eintrag in das Array kopieren
 *
 * \desc
 * Mit dieser Funktion wird ein Vezeichnis-Eintrag aus der CDirEntry bzw. CDir Klasse in
 * einen Schlüssel des CAssocArray kopiert. Dazu wird die Funktion CDirEntry::ToArray
 * verwendet.
 *
 * \param[in] key Name des Schlüssels
 * \param[in] d Pointer auf eine CDirEntry oder CDir-Klasse
 *
 * \returns Konnte der Verzeichnis-Eintrag dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 */
{
	if (!key) {
		SetError(194,"int CAssocArray::Set(==> const char *key <==, CDirEntry *d)");
		return 0;
	}
	CAssocArray a;
	if (!d.ToArray(&a)) return 0;
	return Set(key,&a);
}

int CAssocArray::Copy(const CDirEntry &d)
/*!\brief Verzeichnis-Eintrag in das Array kopieren
 *
 * \desc
 * Mit dieser Funktion wird ein Vezeichnis-Eintrag aus der CDirEntry bzw. CDir Klasse in
 * die oberste Ebene der Array-Klasse kopiert. Dazu wird die Funktion CDirEntry::ToArray
 * verwendet.
 *
 * \param[in] d Pointer auf eine CDirEntry oder CDir-Klasse
 *
 * \returns Konnte der Verzeichnis-Eintrag dem Array hinzugefügt werden, liefert die Funktion true (1) zurück,
 * im Fehlerfall false (0).
 */
{
	return d.ToArray(this);
}


int CAssocArray::CreateFromTemplate(const char *templ, const char *linedelimiter, const char *splitchar, const char *concat, bool dotrim)
/*! \brief Wandelt ein Key-Value Template in ein Assoziatives Array um
 *
 * \desc
 * Die Member-Funktion \c CreateFromTemplate wandelt einen Text im Key-Value-Format in ein
 * Assoziatives Array um. Leere Zeilen oder Zeilen mit Raute (#) am Anfang (Kommentarzeilen) werden ignoriert.
 *
 * \param[in] templ Entweder ein char-Pointer auf einen Null-Terminierten String oder ein Pointer auf die
 * String-Klasse CString
 * \param[in] linedelimiter Das Zeichen, was als Zeilenende interpretiert werden soll. Default ist \c Newline
 * \param[in] splitchar Das Zeichen, was als Trennzeichen zwischen Schlüssel (Key) und Wert (Value)
 * interpretiert werden soll. Der Default ist das Gleichheitszeichen (=)
 * \param[in] concat Ist concat gesetzt und kommen im Text mehrere identische Schlüssel vor, werden die Werte
 * zu einem String zusammengeführt, wobei als Trennzeichen \c concat verwendet wird. Ist concat NULL,
 * wird ein vorhandener Schlüssel überschrieben. Der Default ist, dass gleiche Schlüssel mit Newline
 * aneinander gehangen werden.
 * \param[in] dotrim Ist \c dotrim=true, werden einzelnen Werte vor dem Einfügen ins Array mit der Funktion
 * ppl6::trim getrimmt, also Leerzeilen, Tabs und Zeilenumbrüche am Anfang und Ende gelöscht. Der Default
 * ist \c false.
 *
 * \return Die Funktion gibt die Anzahl gelesener Key-Value-Paare zurück, oder 0, wenn der Text
 * keine verwertbaren Zeilen enthielt.
 *
 * \note Falls das Array vor dem Aufruf dieser Funktion bereits Datensätze enthielt, werden diese
 * nicht gelöscht. Die Funktion kann also benutzt werden, um Werte aus verschiedenen Templates in ein
 * einziges Array einzulesen. Soll das Array geleert werden, muß vorher die Funktion CAssocArray::Clear
 * aufgerufen werden.
 *
 * \see Um Konfigurationsdateien mit verschiedenen Abschnitten (z.B. .ini-Dateien) in ein
 * Assoziatives Array einzulesen, gibt es die Funktion ppl6::Ini2Array und die Member-Funktion
 * CAssocArray::CreateFromConfig
 *
 */
{
	CString Row;
	CArray a;
	const char *line;
	char *value;
	int rows=0;
	int p;
	size_t ssc=strlen(splitchar);
	a.Text2Array((char*)templ,(char*)linedelimiter);
	//a.List();
	// TODO: Flag, ob Leerzeilen ignoriert werden sollen
	a.Reset();
	while ((line=a.GetNext())) {
		Row=line;
		Row.Trim();
		if (Row.Len()>0 && Row[0]!='#') { // Leere Zeilen und Kommentare ignorieren
			Row=line;
			char *l=(char*)Row.GetPtr();
			p=instr((char*)l,(char*)splitchar,0);
			if (p>0) {
				l[p]=0;
				l=trim(l);
				value=l+p+ssc;
				//printf ("Key=%s\nValue=%s\n",l,value);
				if (dotrim) value=trim(value);
				if (concat) {
					Concat((char*)l,(char*)value,(char*)concat);
				} else {
					Set((char*)l,(char*)value);
				}
				rows++;
			}
		}
	}
	return rows;
}

int CAssocArray::CreateFromConfig(const char *content, const char *splitchar, const char *concat, bool dotrim)
/*! \brief Wandelt eine Konfigurationsdatei in ein Assoziatives Array um
 *
 * \desc
 * Die Member-Funktion \c CreateFromConfig wandelt einen Konfigurations-Text mit mehreren Abschnitten
 * im Key-Value-Format in ein Assoziatives Array um. Ein Abschnitt beginnt immer mit einem Keywort
 * in Eckigen klammern und enthält Key-Value-Paare. Zeilen mit Raute (#) am Anfang werden als
 * Kommentarzeilen interpretiert und ignoriert.
 *
 * \par Beispiel einer Konfigurationsdatei
 * \code
[Abschnitt_1]
# Kommentarzeile, die überlesen wird
key1: value1
key2: value2
[Abschnitt_2]
key1: value1
key2: value2
\endcode
 *
 * \param[in] content Ein Char-Pointer auf einen Null-Terminierten String, der die Konfiguration
 * enthält.
 * \param[in] splitchar Das Zeichen, was als Trennzeichen zwischen Schlüssel (Key) und Wert (Value)
 * interpretiert werden soll. Der Default ist der Doppelpunkt (:)
 * \param[in] concat Ist concat gesetzt und kommen im Text mehrere identische Schlüssel vor, werden die Werte
 * zu einem String zusammengeführt, wobei als Trennzeichen \c concat verwendet wird. Ist concat NULL,
 * wird ein vorhandener Schlüssel überschrieben. Der Default ist, dass gleiche Schlüssel mit Newline
 * aneinander gehangen werden.
 * \param[in] dotrim Ist \c dotrim=true, werden einzelnen Werte vor dem Einfügen ins Array mit der Funktion
 * ppl6::trim getrimmt, also Leerzeilen, Tabs und Zeilenumbrüche am Anfang und Ende gelöscht. Der Default
 * ist \c false.
 *
 * \return Die Funktion gibt die Anzahl gelesener Key-Value-Paare zurück, oder 0, wenn der Text
 * keine verwertbaren Zeilen enthielt.
 *
 * \note Falls das Array vor dem Aufruf dieser Funktion bereits Datensätze enthielt, werden diese
 * nicht gelöscht. Die Funktion kann also benutzt werden, um Werte aus verschiedenen Templates in ein
 * einziges Array einzulesen. Soll das Array geleert werden, muß vorher die Funktion CAssocArray::Clear
 * aufgerufen werden.
 *
 * \see Alternativ kann auch die identisch arbeitende Funktion ppl6::Ini2Array verwendet werden.
 * Enthält der Text keine Abschnitte, kann auch die Member-Funktion CAssocArray::CreateFromTemplate
 * verwendet werden, bei der neben dem Trennzeichen der Key-Value-Paare auch das Zeichen für Zeilenende
 * angegeben werden kann.
 */
{
	// String in ein Array wandeln
	CArray a;
	a.Explode(content,"\n");
	int nr=a.Num();
	if (!nr) return 0;
	int l,p;
	int count=0;
	const char *line;
	CString Row, Section, Key, Value, Token;
	a.Reset();
	while ((line=a.GetNext())) {
		Row=line;
		Row.Trim();
		l=Row.Len();
		if (l>0 && Row[0]!='#') {	// Leere Zeilen und Kommentare ignorieren
			if (Row[0]=='[' && Row[l-1]==']') {
				Section=Row.SubStr(1,l-2);
				Section.LCase();
				Section.Trim();
			} else {
				Row=line;
				p=Row.Instr((char*)splitchar);
				if (p) {
					Key=Row.SubStr(0,p);
					Key.LCase();
					Key.Trim();
					Value=Row.SubStr(p+1);
					if (dotrim) Value.Trim();
					//printf ("Section: %s, Key: %s, Value: %s\n",(char*)Section,(char*)Key,(char *)Value);
					if (Key.Len()) {
						if (Section.Len()) Token.Sprintf("%s/%s",(const char*)Section,(const char*)Key);
						else Token.Sprintf("%s",(const char*)Key);
						if (concat) {
							Concat(Token,Value,concat);
						} else {
							Set(Token,Value);
						}
						count++;
					}
				}
			}
		}
	}
	return count;
}

const char *CAssocArray::operator[](const char *key) const
/*!\brief String zu einem Key auslesen
 *
 * \desc
 * Mit diesem Operator kann der String-Wert eines gewünschten Schlüssels ausgelesen werden. Er entspricht der
 * Funktion CAssocArray::GetChar(const char *key).
 *
 * \param[in] key Name des gewünschten Schlüssels
 * \returns bei Erfolg liefert die Funktion einen Pointer auf den String-Wert des Schlüssels zurück, im
 * Fehlerfall (Schlüssel nicht gefunden oder kein String-Element) NULL.
 */
{
	return GetChar(key);
}

CAssocArray& CAssocArray::operator=(const CAssocArray& a)
/*!\brief CAssocArray kopieren
 *
 * \desc
 * Mit diesem Operator wird der Inhalt eines anderen CAssocArray übernommen. Die Funktion ruft zunächst
 * CAssocArray::Clear auf, so dass ebventuell bereits vorhandene Schlüssel gelöscht werden. Anschließend
 * wird die Funktion CAssocArray::Copy aufgerufen.
 *
 * \param[in] a Referenz auf das Array, das kopiert werden soll
 */
{
	Clear();
	Copy(&a);
	return *this;
}

CAssocArray& CAssocArray::operator=(const CArray& a)
/*!\brief CArray kopieren
 *
 * \desc
 * Mit diesem Operator wird der Inhalt eines CArray übernommen. Die Funktion ruft zunächst
 * CAssocArray::Clear auf, so dass ebventuell bereits vorhandene Schlüssel gelöscht werden. Anschließend
 * wird die Funktion CAssocArray::Copy aufgerufen.
 *
 * \param[in] a Referenz auf das Array, das kopiert werden soll
 */
{
	Clear();
	Copy(a);
	return *this;
}

CAssocArray& CAssocArray::operator=(const CAssocArray *a)
/*!\brief CAssocArray kopieren
 *
 * \desc
 * Mit diesem Operator wird der Inhalt eines anderen CAssocArray übernommen. Die Funktion ruft zunächst
 * CAssocArray::Clear auf, so dass ebventuell bereits vorhandene Schlüssel gelöscht werden. Anschließend
 * wird die Funktion CAssocArray::Copy aufgerufen.
 *
 * \param[in] a Pointer auf das Array, das kopiert werden soll
 */
{
	Clear();
	if (a) Copy(a);
	return *this;
}

CAssocArray& CAssocArray::operator=(CDirEntry& d)
/*!\brief CDirEntry oder CDir kopieren
 *
 * \desc
 * Mit diesem Operator wird der Inhalt eines CDirEntry oder CDir übernommen. Die Funktion ruft zunächst
 * CAssocArray::Clear auf, so dass ebventuell bereits vorhandene Schlüssel gelöscht werden. Anschließend
 * wird die Funktion CDirEntry::Copy aufgerufen, um die Daten in das Array zu kopieren.
 *
 * \param[in] d Referenz auf den Verzeichniseintrag CDirEntry oder CDir
 */
{
	Clear();
	d.ToArray((CAssocArray*)this);
	return *this;
}

/*!\brief CDirEntry oder CDir kopieren
 *
 * \desc
 * Mit diesem Operator wird der Inhalt eines CDirEntry oder CDir übernommen. Die Funktion ruft zunächst
 * CAssocArray::Clear auf, so dass ebventuell bereits vorhandene Schlüssel gelöscht werden. Anschließend
 * wird die Funktion CDirEntry::Copy aufgerufen, um die Daten in das Array zu kopieren.
 *
 * \param[in] d Pointer auf den Verzeichniseintrag CDirEntry oder CDir
 */
CAssocArray& CAssocArray::operator=(CDirEntry *d)
{
	Clear();
	if (d) d->ToArray((CAssocArray*)this);
	return *this;
}

int CAssocArray::ToInt(const char *key) const
{
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return 0;
	if (ptr->value==NULL) {
		SetError(450);
		return 0;
	}
	if (ptr->type==datatype::CSTRING) {
		return ppl6::atoi(((CString*)ptr->value)->GetPtr());
	} else if (ptr->type==datatype::CWSTRING) {
		return ppl6::atoi(((CWString*)ptr->value)->GetPtr());
	} else if (ptr->type==datatype::CHAR) {
		return ppl6::atoi((const char*)ptr->value);
	}
	SetError(303);
	return 0;
}

long CAssocArray::ToLong(const char *key) const
{
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return 0;
	if (ptr->value==NULL) {
		SetError(450);
		return 0;
	}
	if (ptr->type==datatype::CSTRING) {
		return ppl6::atol(((CString*)ptr->value)->GetPtr());
	} else if (ptr->type==datatype::CWSTRING) {
		return ppl6::atol(((CWString*)ptr->value)->GetPtr());
	} else if (ptr->type==datatype::CHAR) {
		return ppl6::atol((const char*)ptr->value);
	}
	SetError(303);
	return 0;
}

long long CAssocArray::ToLongLong(const char *key) const
{
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return 0;
	if (ptr->value==NULL) {
		SetError(450);
		return 0;
	}
	if (ptr->type==datatype::CSTRING) {
		return ppl6::atoll(((CString*)ptr->value)->GetPtr());
	} else if (ptr->type==datatype::CWSTRING) {
		return ppl6::atoll(((CWString*)ptr->value)->GetPtr());
	} else if (ptr->type==datatype::CHAR) {
		return ppl6::atoll((const char*)ptr->value);
	}
	SetError(303);
	return 0;
}

bool CAssocArray::ToBool(const char *key) const
{
	return IsTrue(key);
}

CString CAssocArray::ToCString(const char *key) const
{
	CString r;
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return r;
	if (ptr->value==NULL) {
		SetError(450);
		return r;
	}
	if (ptr->type==datatype::CSTRING) {
		r=(CString*)ptr->value;
		return r;
	} else if (ptr->type==datatype::CWSTRING) {
		r=(CWString*)ptr->value;
		return r;
	} else if (ptr->type==datatype::CHAR) {
		r=(const char *)ptr->value;
		return r;
	}
	SetError(303);
	return r;
}

CDateTime CAssocArray::GetDateTime(const char *key) const
{
	CDateTime r;
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return r;
	if (ptr->value==NULL) {
		SetError(450);
		return r;
	}
	if (ptr->type==datatype::DATETIME) {
		r=*(CDateTime*)ptr->value;
		return r;
	}
	SetError(303);
	return r;
}


CWString CAssocArray::ToCWString(const char *key) const
{
	CWString r;
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return r;
	if (ptr->value==NULL) {
		SetError(450);
		return r;
	}
	if (ptr->type==datatype::CSTRING) {
		r=(CString*)ptr->value;
		return r;
	} else if (ptr->type==datatype::CWSTRING) {
		r=(CWString*)ptr->value;
		return r;
	} else if (ptr->type==datatype::CHAR) {
		r=(const char *)ptr->value;
		return r;
	}
	SetError(303);
	return r;
}

CAssocArray CAssocArray::ToCAssocArray(const char *key) const
{
	CAssocArray r;
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return r;
	if (ptr->value==NULL) {
		SetError(450);
		return r;
	}
	if (ptr->type!=datatype::ARRAY) {
		SetError(303);
		return r;
	}
	r=(CAssocArray*)ptr->value;
	return r;
}

CArray CAssocArray::ToCArray(const char *key) const
{
	CArray r;
	GetCArray(r,key);
	return r;
}

int CAssocArray::GetCArray(CArray &a, const char *key) const
{
	a.Clear();
	CArrayItem *ptr;
	if (key) {
		ptr=(CArrayItem *)GetValue(key);
		if (!ptr) return 0;
		if (ptr->value==NULL) {
			SetError(450);
			return 0;
		}
		if (ptr->type!=datatype::ARRAY) {
			SetError(303);
			return 0;
		}
		return ((CAssocArray *)ptr->value)->GetCArray(a,NULL);
	}
	ppl6::CTreeWalker walk;
	Tree.Reset(walk);
	CArrayItem *p;
	while ((p=(CArrayItem*)Tree.GetNext(walk))) {
		if (p->type==datatype::CSTRING) {
			a.Add((const char*)((CString*)p->value)->GetPtr());
		} else if (p->type==datatype::CWSTRING) {
			a.Add((const char*)((CWString*)p->value)->GetPtr());
		} else if (p->type==datatype::CHAR) {
			a.Add((const char*)p->value);
		}
	}
	return 1;
}

CBinary CAssocArray::ToCBinary(const char *key) const
{
	CBinary r;
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return r;
	if (ptr->value==NULL) {
		SetError(450);
		return r;
	}
	if (ptr->type!=datatype::BINARY) {
		SetError(303);
		return r;
	}
	r=(CBinary*)ptr->value;
	return r;
}

bool CAssocArray::HaveKey(const char *key) const
{
	CArrayItem *ptr=(CArrayItem *)GetValue(key);
	if (!ptr) return false;
	if (ptr->value==NULL) return false;
	return true;
}







int Count(CAssocArray *a, bool recursive)
/*!\brief Anzahl Schlüssel in einem CAssocArray zählen
 *
 * \desc
 * Diese Funktion liefert die Anzahl Schlüssel zurück, die in dem angegebenen CAssocArray enthalten sind.
 * Die Funktion entspricht der Member-Function CAssocArray::Count(bool recursive)
 *
 * \param[in] a Pointer auf das Assoziative Array
 * \param[in] recursive Falls recursive auf true gesetzt wird die Funktion rekusriv für jeden
 * Schlüssel aufgerufen, dessen Wert ebenfalls ein Array ist.
 *
 * \returns Anzahl Schlüssel
 */
{
	if (!a) return 0;
	return a->Count(recursive);
}

CAssocArray *Ini2Array(const char *content, const char *splitchar, const char *concat, CAssocArray *res, bool dotrim)
/*!\brief Wandelt eine Konfigurationsdatei in ein Assoziatives Array um
 *
 * \desc
 * Diese Funktion wandelt einen Konfigurations-Text mit mehreren Abschnitten
 * im Key-Value-Format in ein Assoziatives Array um. Ein Abschnitt beginnt immer mit einem Keywort
 * in Eckigen klammern und enthält Key-Value-Paare. Zeilen mit Raute (#) am Anfang werden als
 * Kommentarzeilen interpretiert und ignoriert.
 *
 * \par Beispiel einer Konfigurationsdatei
 * \code
[Abschnitt_1]
# Kommentarzeile, die überlesen wird
key1: value1
key2: value2
[Abschnitt_2]
key1: value1
key2: value2
\endcode
 *
 * \param content Ein Char-Pointer auf einen Null-Terminierten String, der die Konfiguration
 * enthält.
 * \param splitchar Das Zeichen, was als Trennzeichen zwischen Schlüssel (Key) und Wert (Value)
 * interpretiert werden soll. Der Default ist der Doppelpunkt (:)
 * \param concat Ist concat gesetzt und kommen im Text mehrere identische Schlüssel vor, werden die Werte
 * zu einem String zusammengeführt, wobei als Trennzeichen \c concat verwendet wird. Ist concat NULL,
 * wird ein vorhandener Schlüssel überschrieben. Der Default ist, dass gleiche Schlüssel mit Newline
 * aneinander gehangen werden.
 * \param res Pointer auf ein Assoziatives Array, in dem die Ergebnisse gespeichert werden sollen.
 * Ist der Parameter NULL, wird ein neues Array erzeugt.
 * \param dotrim Ist \c dotrim=true, werden einzelnen Werte vor dem Einfügen ins Array mit der Funktion
 * ppl6::trim getrimmt, also Leerzeilen, Tabs und Zeilenumbrüche am Anfang und Ende gelöscht. Der Default
 * ist \c true.
 *
 * \return Die Funktion gibt einen Pointer auf das Assoziative Array zurück, oder NULL,
 * wenn der Text keine verwertbaren Zeilen enthielt.
 *
 * \note Falls das Array vor dem Aufruf dieser Funktion bereits Datensätze enthielt, werden diese
 * nicht gelöscht. Die Funktion kann also benutzt werden, um Werte aus verschiedenen Templates in ein
 * einziges Array einzulesen. Soll das Array geleert werden, muß vorher die Funktion CAssocArray::Clear
 * aufgerufen werden.
 *
 * \see Alternativ kann auch die identisch arbeitende Member-Funktion CAssocArray::CreateFromConfig
 * verwendet werden.
 * Enthält der Text keine Abschnitte, kann auch die Member-Funktion CAssocArray::CreateFromTemplate
 * verwendet werden, bei der neben dem Trennzeichen der Key-Value-Paare auch das Zeichen für Zeilenende
 * angegeben werden kann.
 */
{
	if (!res) {
		res=new CAssocArray;
		if (!res->CreateFromConfig(content,splitchar,concat,dotrim)) {
			delete res;
			return NULL;
		}
	} else {
		if (!res->CreateFromConfig(content,splitchar,concat,dotrim)) return NULL;
	}
	return res;
}



}	// EOF namespace ppl6

