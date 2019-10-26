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

namespace ppl6 {


CTreeWalker::CTreeWalker()
{
	current=NULL;
	stack_height=0;
}

/*! \class CTreeItem
 * \brief Klasse für Tree-Elemente
 * \ingroup PPLGroupDataTypes
 *
 * \desc
 * Diese Klasse dient als Basisklasse für beliebige BaumElemente, die über die Klasse
 * ppl6::CTree innerhalb eines AVL-Baums verwaltet werden.
 *
 * Bei Verwendung dieser Klasse, müssen die beiden virtuellen Funktionen CTreeItem::CompareNode
 * und CTreeItem::CompareValue implementiert werden.
 *
 * \par Beispiel 1
\code
class MyItem : public ppl6::CTreeItem
{
	private:
		ppl6::CString data;
	public:
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);
};

int MyItem::CompareNode(CTreeItem *item)
{
	MyItem *other=(MyTree*)item;
	int ret=other->data.StrCmp(&data);
	if (ret>0) return 1;
	if (ret<0) return -1;
	return 0;
}

int MyItem::CompareValue(void *value)
{
	ppl6::CString *other=(ppl6::CString*) value;
	int ret=other->StrCmp(&data);
	if (ret>0) return 1;
	if (ret<0) return -1;
	return 0;
}
\endcode
 *
 * \par Beispiel 2
 *
 * \include CTreeItem_2.cpp
 *
 * \see
 * - ppl6::CTree
 *
 */


/*!\var CTreeItem::tree
 * \brief Zeiger auf den Baum, dessen Element dieser Knoten ist
 */

/*!\var CTreeItem::left
 * \brief Zeiger auf den linken Kindknoten
 */

/*!\var CTreeItem::right
 * \brief Zeiger auf den rechten Kindknoten
 */

/*!\var CTreeItem::parent
 * \brief Zeiger auf den Vaterknoten
 */

/*!\var CTreeItem::balance
 * \brief Die Balance des Knotens
 */

CTreeItem::CTreeItem()
/*!\brief Konstruktor
 *
 * \desc
 * Im Konstruktor wird das Treeelement initialisiert und die internen Pointer auf NULL
 * gesetzt
 */
{
	left=NULL;
	right=NULL;
	balance=0;
	tree=NULL;
}

CTreeItem::~CTreeItem()
/*!\brief Destruktor
 *
 * \desc
 * Im Destruktor wird geprüft, ob das Element noch Teil eines Baums ist. Ist dies deer Fall,
 * wird die Delete-Funktion des Baums aufgerufen, um das Element aus dem Baum zu entfernen.
 */
{
	if (tree) tree->Delete(this);
}


int CTreeItem::CompareNode(CTreeItem *item)
/*!\brief Elemente vergleichen
 *
 * \desc
 * Damit Elemente sortiert in den Baum eingehangen werden können, werden die
 * Vergleichsfunktionen CompareNode und CompareValue benötigt. Diese sind in der
 * Basisklasse als "virtuell" deklariert und müssen von der abgeleiteten Klasse
 * überschrieben werden.
 * Bei dieser Funktion wird als Parameter ein Pointer auf ein Baum-Element erwartet.
 *
 * \param[in] item Pointer auf ein Baum-Element, mit dem dieses Element verglichen werden soll
 *
 * \returns Die Funktion muss einen der folgenden 4 Werte zurückliefern:
 * - 0: Ist der Wert in \p item identisch mit dem Wert dieses Elements, muss
 * 0 zurückgegeben werden.
 * - +1: Ist der Wert in \p item größer als der Wert dieses Elements, muss +1 zurückgegeben werden
 * - -1: Ist der Wert in \p item kleiner als der Wert dieses Elements, muss -1 zurückgegeben werden
 * - -2: Dieser Wert wird im Fehlerfall zurückgegeben und es wird ein
 *   Fehlercode gesetzt. Die Tree-Operation muss dann ebenfalls mit einem
 *   Fehler abbrechen und diesen Fehlercode zurückgeben.
 *
 * \attention Beim Vergleich zweier Strings kann die Funktion \c strcmp nicht direkt
 * verwendet werden, da sie laut Definition Werte kleiner oder größer 0 liefert, aber
 * nicht exakt -1 oder +1.
 */
{
	ppl6::SetError(180,"CTreeItem::CompareNode");
	return -2;
}

int CTreeItem::CompareValue(void *value)
/*!\brief Elemente vergleichen
 *
 * \desc
 * Damit Elemente sortiert in den Baum eingehangen werden können, werden die
 * Vergleichsfunktionen CompareNode und CompareValue benötigt. Diese sind in der
 * Basisklasse als "virtuell" deklariert und müssen von der abgeleiteten Klasse
 * überschrieben werden.
 * Bei dieser Funktion wird als Parameter ein void Pointer auf einen beliebigen
 * Wert erwartet.
 *
 * \param[in] value Pointer auf einen beliebigen Wert. Die abgeleitete Klasse muss
 * wissen, wie die Daten zu interpretieren sind.
 *
 * \returns Die Funktion muss einen der folgenden 4 Werte zurückliefern:
 * - 0: Ist der Wert in \p value identisch mit dem Wert dieses Elements, muss
 * 0 zurückgegeben werden.
 * - +1: Ist der Wert in \p value größer als der Wert dieses Elements, muss +1 zurückgegeben werden
 * - -1: Ist der Wert in \p value kleiner als der Wert dieses Elements, muss -1 zurückgegeben werden
 * - -2: Dieser Wert wird im Fehlerfall zurückgegeben und es wird ein
 *   Fehlercode gesetzt. Die Tree-Operation muss dann ebenfalls mit einem
 *   Fehler abbrechen und diesen Fehlercode zurückgeben.
 */
{
	ppl6::SetError(180,"CTreeItem::CompareValue");
	return -2;
}

int CTreeItem::GetValue(ppl6::CString *buffer)
/*!\brief Wert eines Knotens in einem String speichern
 *
 * \desc
 * Diese Funktion ist als "virtuell" definiert und kann implementiert werden, um es zu ermöglichen
 * den Wert eines Knotens in einem String abzubilden. Je nach Datentyp kann dies einfach aber auch
 * unmöglich sein. Die Funktion wird in der Regel nur zu debugging-Zwecken verwende.
 *
 * \param[out] buffer Pointer auf einen String, in dem das Ergebnis gespeichert werden soll
 *
 * \returns Bei Erfolg liefert die Funktion true (1) zurück, im Fehlerfall false (0). Wurde die
 * Funktion von der Tree-Klasse nicht implementiert, wird immer false zurückgegeben und der
 * Fehlercode 180 gesetzt.
 */
{
	ppl6::SetError(180,"CTreeItem::GetValue");
	return 0;
}

CTreeItem *CTreeItem::Left()
/*!\brief Pointer auf linken Knoten auslesen
 *
 * Diese Funktion gibt einen Pointer auf den Knoten zurück, der links von diesem angehangen ist.
 *
 * @return Pointer auf linken Knoten oder NULL, wenn es keinen gibt.
 *
 * \since Eingeführt mit PPL 6.2.6
 */
{
	return left;
}

CTreeItem *CTreeItem::Right()
/*!\brief Pointer auf rechten Knoten auslesen
 *
 * Diese Funktion gibt einen Pointer auf den Knoten zurück, der rechts von diesem angehangen ist.
 *
 * @return Pointer auf rechten Knoten oder NULL, wenn es keinen gibt.
 *
 * \since Eingeführt mit PPL 6.2.6
 */
{
	return right;
}

CTreeItem *CTreeItem::Parent()
/*!\brief Pointer auf Parent Knoten auslesen
 *
 * Diese Funktion gibt einen Pointer auf den Vater dieses Knoten zurück.
 *
 * @return Pointer auf den parent Knoten oder NULL, wenn es keinen gibt.
 *
 * \since Eingeführt mit PPL 6.2.6
 */
{
	return parent;
}


/*! \class CTree
 * \brief AVL-Bäume mit CTreeItem
 * \ingroup PPLGroupDataTypes
 *
 * \desc
 * Diese Klasse kann zur Verwaltung beliebiger Elemente in einem sortierten AVL-Baum
 * verwendet werden. Die Elemente müssen dabei von der Basisklasse ppl6::CTreeItem abgeleitet sein.
 *
 * \attention Die Klasse verwaltet keinen eigenen Mutex und ist somit nicht Thread-sicher.
 * Die Aufrufende Anwendung muss selbst sicherstellen, dass die Klasse nicht gleichzeitig
 * von mehreren Threads verwendet wird.
 */


/*!\var CTree::root
 * \brief Pointer auf die Wurzel des Baums
 */

/*!\var CTree::count
 * \brief Anzahl Knoten im Baum
 */

/*!\var CTree::current
 * \brief Aktueller Knoten beim Durchwandern des Baums
 */

/*!\var CTree::stack
 * \brief Alle Knoten oberhalb von current
 */

/*!\var CTree::stack_height
 * \brief Anzahl Knoten im Stack
 */


CTree::CTree()
/*!\brief Konstruktor
 *
 * \desc
 * Im Konstruktor wird der Baum mit NULL initialisiert.
 */
{
	count=0;
	root=NULL;
	current=NULL;
	stack_height=0;
	memset(stack,0,AVL_MAX_HEIGHT*sizeof(CTreeItem*));
	//for (int i=0;i<AVL_MAX_HEIGHT;i++) stack[i]=NULL;
}

CTree::~CTree()
/*!\brief Destruktor
 *
 * \desc
 * Der Destruktor ruft die CTree::Clear Funktion auf.
 */
{
	Clear();
}


int CTree::Num() const
/*!\brief Anzahl Elemente im Baum
 *
 * \desc
 * Diese Funktion liefert die Anzahl Elemente im Baum zurück.
 *
 * \return Anzahl Elemente oder 0, wenn der Baum leer ist. Bei einem leeren Baum wird
 * ausserdem der Fehlercode 347 gesetzt.
 */
{
	if (!count) SetError(347);
	return count;
}

void CTree::Clear(bool deleteitems)
/*!\brief Baum leeren
 *
 * \desc
 * Mit dieser Funktion wird der Inhalt des Baums gelöscht.
 *
 *\param[in] deleteitems Wird der optionale Parameter auf "true" gesetzt, wird der Destruktur
 * jedes Baumelements aufgerufen.
 */
{
	if (deleteitems) {
		while (root) delete root;
	}
	root=NULL;
	count=0;
	current=NULL;
	stack_height=0;
}

CTreeItem *CTree::GetRootNode() const
/*!\brief Pointer auf Root-Knoten holen
 *
 * \desc
 * Diese Funktion liefert einen Pointer auf den obersten Knoten des Baums.
 *
 * \returns CTreeItem Pointer auf Root-Knoten. Ist der Baum leer, wird NULL zurückgegeben
 * und der Fehlercode 347 gesetzt.
 */
{
	if (!root) SetError(347);
	return root;
}

CTreeItem *CTree::Find(void *value) const
/*!\brief Wert im Baum finden
 *
 * \desc
 * Mit dieser Funktion wird ein beliebiger Wert innerhalb des Baums gesucht. Dazu wird
 * die Funktion CTreeItem::CompareValue verwendet.
 *
 * \param[in] value Beliebiger Pointer auf einen Wert
 * \returns Wird der Wert im Baum gefunden, gibt die Funktion einen Pointer auf das
 * Baum-Element zurück. Falls nicht, wird NULL zurückgegeben und der Fehlercode
 * 421 gesetzt.
 */
{
	CTreeItem *cur=root;
	int cmp;
	while (cur) {
		cmp=cur->CompareValue(value);
		if (cmp==-1) cur=cur->left;
		else if (cmp==1) cur=cur->right;
		else if (cmp==-2) return NULL;
		else return cur;
	}
	SetError(421);
	return NULL;
}

CTreeItem *CTree::Find(CTreeItem *item) const
/*!\brief Element im Baum finden
 *
 * \desc
 * Mit dieser Funktion wird ein Element innerhalb des Baums gesucht. Dazu wird
 * die Funktion CTree::CompareItems verwendet.
 *
 * \param[in] item Pointer auf ein CTreeItem Element
 * \returns Wird das Element im Baum gefunden, gibt die Funktion einen Pointer auf das
 * Baum-Element zurück. Falls nicht, wird NULL zurückgegeben und der Fehlercode
 * 421 gesetzt.
 */
{
	CTreeItem *cur=root;
	int cmp;
	while (cur) {
		cmp=cur->CompareNode(item);
		if (cmp==-1) cur=cur->left;
		else if (cmp==1) cur=cur->right;
		else if (cmp==-2) return NULL;
		else return cur;
	}
	SetError(421);
	return NULL;
}

CTreeItem *CTree::FindOrAdd(CTreeItem *item)
/*!\brief Element im Baum finden oder hinzufügen
 *
 * \desc
 * Mit dieser Funktion wird ein Element innerhalb des Baums gesucht. Dazu wird
 * die Funktion CTree::CompareItems verwendet. Wird es gefunden, wird der Pointer auf das
 * Element zurückgeliefert. Wird es nicht gefunden, wird automatisch ein neues Element
 * angelegt und dessen Pointer zurückgeliefert.
 *
 * \param[in] item Pointer auf ein CTreeItem Element
 * \returns Pointer auf das gefundene oder neu angelegte Element, oder NULL, wenn ein
 * Fehler aufgetreten ist.
 */
{
	CTreeItem *cur=root;
	int cmp;
	while (cur) {
		cmp=cur->CompareNode(item);
		if (cmp==-1) cur=cur->left;
		else if (cmp==1) cur=cur->right;
		else if (cmp==-2) return NULL;
		else return cur;
	}
	if (Add(item)) return item;
	return NULL;
}

int CTree::Add(CTreeItem *item)
/*!\brief Element hinzufügen
 *
 * \desc
 * Diese Funktion fügt ein neues Element in den Baum ein. Dabei ist sichergestellt,
 * dass der Baum stets sortiert und ausgewogen ist.
 *
 * \param[in] item Pointer auf das hinzuzufügende Element
 * \returns Wurde das Element erfolgreich hinzugefügt, gibt die Funktion true (1) zurück,
 * sonst false (0) und ein entsprechender Fehlercode wird gesetzt,
 */
{
	CTreeItem *cur=root;
	CTreeItem *node=NULL;
	int cmp=0;
	if (!item) {
		SetError(194);
		return 0;
	}
	if (item->tree) {
		SetError(420);
		return 0;
	}
	item->right=item->left=NULL;
	item->balance=0;
	if (!root) {
		root=item;
		item->tree=this;
		item->parent=NULL;
		count++;
		Reset();
		return 1;
	}
	while (cur) {
		node=cur;
		cmp=cur->CompareNode(item);
		if (cmp<0) cur=cur->left;
		else if (cmp>0) cur=cur->right;
		else {	// Es gibt bereits ein gleichnamiges Element
			ppl6::SetError(418);
			return 0;
		}
	}
	item->tree=this;
	item->parent=node;
	if (cmp>0) {
		node->right=item;
		node->balance++;
	} else {
		node->left=item;
		node->balance--;
	}
	count++;
	if (node->balance!=0) UpInsert(node);
	Reset();
	return 1;
}

void CTree::UpInsert(CTreeItem *node)
{
	if (((node->balance==-1) || (node->balance==1)) && (node!=root)) {
		if (node->parent->left==node)
			node->parent->balance--;
		else
			node->parent->balance++;
		UpInsert(node->parent);
		return;
	}
	if (node->balance==-2) {
		if (node->left->balance==-1) {
			Rotate(node);
			return;
		} else {
			Rotate(node->left);
			Rotate(node);
			return;
		}
	}
	if (node->balance==2) {
		if (node->right->balance==1) {
			Rotate(node);
			return;
		} else {
			Rotate(node->right);
			Rotate(node);
			return;
		}
	}
}

void CTree::UpDelete(CTreeItem *node)
{
	CTreeItem *parent=node->parent;
	if ((node->balance==-1) || (node->balance==1)) return;
	if ((node==root) && (node->balance==0)) return;
	if (node==root) {
		if(node->balance==-2) {
			if (node->left->balance<=0) Rotate(node);
			else {
				node=Rotate(node->left);
				Rotate(node);
			}
		} else {
			if (node->right->balance>=0) Rotate(node);
			else {
				node=Rotate(node->right);
				Rotate(node);
			}
		}
		return;
	}
	if (node->balance==2) {
		switch(node->right->balance) {
			case 0:
				Rotate(node);
				return;
			case 1:
				UpDelete(Rotate(node));
				return;
			case -1:
			Rotate(node->right);
			UpDelete(Rotate(node));
			return;
		}
	}
	if (node->balance==-2) {
		switch(node->left->balance) {
			case 0:
				Rotate(node);
				return;
			case -1:
			UpDelete(Rotate(node));
			return;
			case 1:
				Rotate(node->left);
				UpDelete(Rotate(node));
				return;
		}
	}
	if (parent->left==node) {
		parent->balance++;
		if (parent->balance<2) {
			UpDelete(parent);
			return;
		}
		switch(parent->right->balance) {
			case 0:
				Rotate(parent);
				return;
			case 1:
				UpDelete(Rotate(parent));
				return;
			case -1:
				Rotate(parent->right);
				UpDelete(Rotate(parent));
				return;
		}
	}
	if (parent->right==node) {
		parent->balance--;
		if (parent->balance>-2) {
			UpDelete(parent);
			return;
		}
		switch (parent->left->balance) {
			case 0:
				Rotate(parent);
				return;
			case -1:
			UpDelete(Rotate(parent));
			return;
			case 1:
				Rotate(parent->left);
				UpDelete(Rotate(parent));
				return;
		}
	}
}

CTreeItem *CTree::Rotate(CTreeItem *kn)
{

	CTreeItem *child;
	if (kn->balance<0) {
		child=kn->left;
		kn->left=child->right;
		if (child->right) child->right->parent=kn;
		child->right=kn;
		child->parent=kn->parent;
		kn->parent=child;
		if (child->parent) {
			if (child->parent->left==kn)
				child->parent->left=child;
			else
				child->parent->right=child;
		} else
			root=child;
		if (kn->balance==-1) {
			if(child->balance==1) {
				child->balance=2;
				kn->balance=0;
				return child;
			}
			if (child->balance==-1)
				kn->balance=1;
			else
				kn->balance=0;
			child->balance=1;
			return child;
		}
		if (kn->balance==-2) {
			if (child->balance==-1) {
				kn->balance=child->balance=0;
				return child;
			}
			if(child->balance==0) {
				kn->balance=-1;
				child->balance=1;
				return child;
			}
			if (child->balance==-2) {
				kn->balance=1;
				child->balance=0;
				return child;
			}
		}
	} else {
		child=kn->right;
		kn->right=child->left;
		if(child->left) child->left->parent=kn;
		child->left=kn;								// Step 3
		child->parent=kn->parent;					// Step 4
		kn->parent=child;							// Step 5
		if(child->parent) {
			if (child->parent->left==kn)
				child->parent->left=child;
			else
				child->parent->right=child;
		} else
			root=child;
		if (kn->balance==1) {
			if (child->balance==-1) {
				child->balance=-2;
				kn->balance=0;
				return child;
			}
			if (child->balance==1)
				kn->balance=-1;
			else
				kn->balance=0;
			child->balance=-1;
			return child;
		}
		if (kn->balance==2) {
			if (child->balance==1) {
				kn->balance=child->balance=0;
				return child;
			}
			if (child->balance==0) {
				kn->balance=1;
				child->balance=-1;
				return child;
			}
			if (child->balance==2) {
				kn->balance=-1;
				child->balance=0;
				return child;
			}
		}
	}
	return child;
}

void CTree::SwapNodes(CTreeItem *item1, CTreeItem *item2)
{
	signed char	balance;
	CTreeItem *left, *right, *parent;
	// Kopieren der Parameter von item1 nach tmp
	left=item1->left;
	right=item1->right;
	balance=item1->balance;
	parent=item1->parent;
	// Kopieren der Parameter von item2 nach item1
	item1->left=item2->left;
	item1->right=item2->right;
	item1->balance=item2->balance;
	item1->parent=item2->parent;
	// Kopieren der Parameter von tmp nach item2
	item2->left=left;
	item2->right=right;
	item2->balance=balance;
	item2->parent=parent;
	// Falls die Nodes direkt nebeneinander liegen, könnten die Child-Pointer auf sich selbst zeigen.
	// Das müssen wir korrigieren
	if (item1->left==item1) item1->left=item2;
	if (item1->right==item1) item1->right=item2;
	if (item2->left==item2) item2->left=item1;
	if (item2->right==item2) item2->right=item1;
	// Pointer der Kinder korrigieren
	if (item1->left) item1->left->parent=item1;
	if (item1->right) item1->right->parent=item1;
	if (item2->left) item2->left->parent=item2;
	if (item2->right) item2->right->parent=item2;
	// Pointer der Parents korrigieren
	if (item1->parent) {
		if (item1->parent->left==item2) item1->parent->left=item1;
		else if (item1->parent->right==item2) item1->parent->right=item1;
	}
	if (item2->parent) {
		if (item2->parent->left==item1) item2->parent->left=item2;
		else if (item2->parent->right==item1) item2->parent->right=item2;
	}
	/*
	// left oder right könnten vorher auf sys gezeigt haben und müssen nun auf item zeigen
	if (sys->left==sys) sys->left=item;
	else if (sys->right==sys) sys->right=item;
	item->parent=tmp.parent;
	if (item->parent==item) item->parent=sys;
	if (item->parent->left==sys) item->parent->left=item;
	else if (item->parent->right==sys) item->parent->right=item;

	item->balance=tmp.balance;
	item->right=tmp.right;
	item->left=tmp.left;
	*/
}



int CTree::Delete(void *value)
/*!\brief Element anhand eines Wertes löschen
 *
 * \desc
 * Diese Funktion sucht zunächst ob der angegebene Wert im Baum enthalten ist und
 * löscht anschließend das gefundene Element. Dabei ist sichergestellt,
 * dass der Baum stets sortiert und ausgewogen ist.
 *
 * \param[in] value Pointer auf den zu löschenden Wert
 * \returns Wurde das Element erfolgreich gelöscht, gibt die Funktion true (1) zurück,
 * sonst false (0) und ein entsprechender Fehlercode wird gesetzt,
 */
{
	CTreeItem *item=Find(value);
	if (!item) {
		SetError(421);
		return 0;
	}
	if (Delete(item)) {
		delete item;
		return 1;
	}
	return 0;
}


int CTree::Delete (CTreeItem *item)
/*!\brief Element löschen
 * \desc
 * Diese Funktion prüft zunächst ob der angegebene Wert im Baum enthalten ist und
 * löscht anschließend das gefundene Element. Dabei ist sichergestellt,
 * dass der Baum stets sortiert und ausgewogen ist.
 *
 * \param[in] item Pointer auf das zu löschende Element
 * \returns Wurde das Element erfolgreich gelöscht, gibt die Funktion true (1) zurück,
 * sonst false (0) und ein entsprechender Fehlercode wird gesetzt,
 */
{
	if (!item) {
		SetError(194);
		return 0;
	}
	// item muss Teil dieses Baums sein
	if (item->tree!=this) {
		SetError(419);
		return 0;
	}
	if (!root) {
		SetError(421);
		return 0;
	}
	CTreeItem *father;

	if((item->left)&&(item->right)) {
		CTreeItem *sys;
		if (item->balance<0) {
			// Wenn die Balance -1 ist, tauschen wir den Knoten mit dem am weitesten
			// rechts befindlichen Knoten des linken Teilbaums
			sys=item->left;	// Blatt finden
			while (sys->right) sys=sys->right;
		} else {
			// Wenn die Balance >=0 ist, tauschen wir den Knoten mit dem am weitesten
			// links befindlichen Knoten des rechten Teilbaums
			sys=item->right;	// Blatt finden
			while (sys->left) sys=sys->left;
		}
		SwapNodes(item,sys);
		if (item==root) root=sys;
	}
	father=item->parent;
	if((!item->left)&&(!item->right)) {
		if(item==root) {
			item->tree=NULL;
			root=NULL;
			count--;
			Reset();
			return(1);
		} else {
			if(father->left==item) {
				father->left=NULL;
				father->balance++;
			} else {
				father->right=NULL;
				father->balance--;
			}
			UpDelete(father);
			item->tree=NULL;
			count--;
			Reset();
			return(1);
		}
	}
	CTreeItem *sohn;
	if(item->left)
		sohn=item->left;
	else
		sohn=item->right;
	if(item!=root) {
		sohn->parent=father;
		if(father->left==item) {
			father->left=sohn;
			father->balance++;
		} else {
			father->right=sohn;
			father->balance--;
		}
		UpDelete(father);
	} else {
		sohn->parent=NULL;
		root=sohn;
	}
	item->tree=NULL;
	count--;
	Reset();
	return(1);
}

int CTree::Validate()
/*!\brief Baum überprüfen
 *
 * \desc
 * Diese Funktion kann aufgerufen werden, um die Integrität des Baumes
 * zu überprüfen. Sie hangelt sich durch den kompletten Baum und überprüft
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
	CString b1,b2;
	int ok=1;
	size_t nr=0;
	CTreeItem *node;
	Reset();
	while ((node=GetNext())) {
		// Kinder prüfen
		if (node->left) {
			if (node->left->parent!=node) {
				ok=0;
				node->GetValue(&b1);
				node->left->GetValue(&b2);
				printf ("Knoten \"%s\"->left zeigt auf Knoten \"%s\", dessen Parent aber nicht zurück!\n",(const char*)b1,(const char*)b2);
			}
		}
		if (node->right) {
			if (node->right->parent!=node) {
				ok=0;
				node->GetValue(&b1);
				node->right->GetValue(&b2);
				printf ("Knoten \"%s\"->right zeigt auf Knoten \"%s\", dessen Parent aber nicht zurück!\n",(const char*)b1,(const char*)b2);
			}
		}
		if (node->left==node) {
			ok=0;
			node->GetValue(&b1);
			printf ("Knoten \"%s\"->left zeigt auf sich selbst!\n",(const char*)b1);
		}
		if (node->right==node) {
			ok=0;
			node->GetValue(&b1);
			printf ("Knoten \"%s\"->right zeigt auf sich selbst!\n",(const char*)b1);
		}
		if (node->parent==node) {
			ok=0;
			node->GetValue(&b1);
			printf ("Knoten \"%s\"->parent zeigt auf sich selbst!\n",(const char*)b1);
		}

		// Balance prüfen
		if (node->balance<-1 || node->balance>1) {
			ok=0;
			node->GetValue(&b1);
			printf ("Knoten \"%s\"->balance=%i!\n",(const char*)b1,node->balance);
		}
		// Parent prüfen
		if (node->parent) {
			if (node->parent->left!=node && node->parent->right!=node) {
				ok=0;
				node->GetValue(&b1);
				node->parent->GetValue(&b2);
				printf ("Knoten \"%s\"->parent zeigt auf \"%s\", dieser zeigt aber nicht auf diesen Knoten!\n",(const char*)b1,(const char*)b2);
			}
		}

		nr++;
	}
	if (count!=nr) {
		ok=0;
		//printf ("Differenz bei Anzahl der Elemente. Soll: %i, Ist: %i\n",count,nr);
	}
	return ok;
}


void CTree::Reset()
/*!\brief Internen Pointer für Walk-Funktionen zurücksetzen
 *
 * \desc
 * Mit dieser Funktion wird der interne Pointer zurückgesetzt, der für die Walk-Funktionen GetNext und
 * GetPrevious verwendet wird.
 */
{
	current=NULL;
	stack_height=0;
}

void CTree::Reset(CTreeWalker &walk) const
/*!\brief CTreeWalker für Walk-Funktionen zurücksetzen
 *
 * \desc
 * Mit dieser Funktion wird der CTreeWalker zurückgesetzt, der für die Walk-Funktionen GetNext und
 * GetPrevious verwendet wird.
 */
{
	walk.current=NULL;
	walk.stack_height=0;
}


CTreeItem *CTree::GetFirst()
/*!\brief Erstes Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das erste Element des Baums zurückgeliefert.
 *
 * \return Pointer auf das erste Element des Baums oder NULL, wenn der Baum leer ist
 */
{
	current=NULL;
	stack_height=0;
	CTreeItem *node;
	node = root;
	if (node != NULL)
		while (node->left != NULL) {
    		stack[stack_height++] = node;
			node = node->left;
		}
	current = node;
	if (node) return node;
	SetError(347);
	return NULL;
}

CTreeItem *CTree::GetFirst(CTreeWalker &walk) const
{
	walk.current=NULL;
	walk.stack_height=0;
	CTreeItem *node;
	node = root;
	if (node != NULL)
		while (node->left != NULL) {
    		walk.stack[walk.stack_height++] = node;
			node = node->left;
		}
	walk.current = node;
	if (node) return node;
	SetError(347);
	return NULL;
}


CTreeItem *CTree::GetNext()
/*!\brief Nächstes Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das nächste Element des Baums zurückgeliefert.
 * Somit kann der Baum sortiert vorwärts durchwandert werden.
 *
 * \return Pointer auf das nächste Element des Baums oder NULL, wenn keine weiteren
 * Elemente vorhanden sind. In diesesm Fall wird ausserdem der Fehlercode 422 gesetzt.
 */
{
	CTreeItem *node=current, *tmp;
	if (node == NULL) {
		return GetFirst();
	} else if (node->right != NULL) {
		stack[stack_height++] = node;
		node = node->right;
		while (node->left != NULL) {
			stack[stack_height++] = node;
			node = node->left;
		}
	} else {
		do {
			if (stack_height == 0) {
				current = NULL;
				SetError(422);
				return NULL;
			}
			tmp=node;
			node = stack[--stack_height];
		} while (tmp == node->right);
	}
	current=node;
	if (node) return node;
	SetError(422);
	return NULL;
}

CTreeItem *CTree::GetNext(CTreeWalker &walk) const
{
	CTreeItem *node=walk.current, *tmp;
	if (node == NULL) {
		return GetFirst(walk);
	} else if (node->right != NULL) {
		walk.stack[walk.stack_height++] = node;
		node = node->right;
		while (node->left != NULL) {
			walk.stack[walk.stack_height++] = node;
			node = node->left;
		}
	} else {
		do {
			if (walk.stack_height == 0) {
				walk.current = NULL;
				SetError(422);
				return NULL;
			}
			tmp=node;
			node = walk.stack[--walk.stack_height];
		} while (tmp == node->right);
	}
	walk.current=node;
	if (node) return node;
	SetError(422);
	return NULL;
}


/*!\brief Letztes Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das letzte Element des Baums zurückgeliefert.
 *
 * \return Pointer auf das letzte Element des Baums oder NULL, wenn der Baum leer ist
 */
CTreeItem *CTree::GetLast()
{
	CTreeItem *node;
	stack_height = 0;
	node = root;
	if (node != NULL)
		while (node->right != NULL) {
			stack[stack_height++] = node;
			node = node->right;
		}
	current = node;
	if (node) return node;
	SetError(347);
	return NULL;
}

/*!\brief Vorheriges Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das vorherige Element des Baums zurückgeliefert.
 * Somit kann der Baum sortiert rückwärts durchwandert werden.
 *
 * \return Pointer auf das vorherige Element des Baums oder NULL, wenn keine weiteren
 * Elemente vorhanden sind. In diesesm Fall wird ausserdem der Fehlercode 422 gesetzt.
 */
CTreeItem *CTree::GetPrevious()
{
	CTreeItem *node, *y;
	node = current;
	if (node == NULL) {
		return GetLast();
	} else if (node->left != NULL) {
		stack[stack_height++] = node;
		node = node->left;
		while (node->right != NULL) {
			stack[stack_height++] = node;
			node=node->right;
		}
	} else {
		do {
			if (stack_height == 0) {
				current = NULL;
				SetError(422);
				return NULL;
			}
			y = node;
			node = stack[--stack_height];
		} while (y == node->left);
	}
	current = node;
	if (node) return node;
	SetError(422);
	return NULL;
}

/*!\brief Aktuelles Element des Baums
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das aktuelle Element des Baums zurückgeliefert.
 * Dabei wird der Pointer nicht verändert.
 *
 * \return Pointer auf das aktuelle Element des Baums oder NULL, wenn kein
 * Element mehr vorhanden ist. In diesesm Fall wird ausserdem der Fehlercode 422 gesetzt.
 */
CTreeItem *CTree::GetCurrent() const
{
	if (current) return current;
	SetError(422);
	return NULL;
}



/*!\brief Letztes Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das letzte Element des Baums zurückgeliefert.
 *
 * \return Pointer auf das letzte Element des Baums oder NULL, wenn der Baum leer ist
 */
CTreeItem *CTree::GetLast(CTreeWalker &walk) const
{
	CTreeItem *node;
	walk.stack_height = 0;
	node = root;
	if (node != NULL)
		while (node->right != NULL) {
			walk.stack[walk.stack_height++] = node;
			node = node->right;
		}
	walk.current = node;
	if (node) return node;
	SetError(347);
	return NULL;
}

/*!\brief Vorheriges Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das vorherige Element des Baums zurückgeliefert.
 * Somit kann der Baum sortiert rückwärts durchwandert werden.
 *
 * \return Pointer auf das vorherige Element des Baums oder NULL, wenn keine weiteren
 * Elemente vorhanden sind. In diesesm Fall wird ausserdem der Fehlercode 422 gesetzt.
 */
CTreeItem *CTree::GetPrevious(CTreeWalker &walk) const
{
	CTreeItem *node, *y;
	node = walk.current;
	if (node == NULL) {
		return GetLast(walk);
	} else if (node->left != NULL) {
		walk.stack[walk.stack_height++] = node;
		node = node->left;
		while (node->right != NULL) {
			walk.stack[walk.stack_height++] = node;
			node=node->right;
		}
	} else {
		do {
			if (walk.stack_height == 0) {
				walk.current = NULL;
				SetError(422);
				return NULL;
			}
			y = node;
			node = walk.stack[--walk.stack_height];
		} while (y == node->left);
	}
	walk.current = node;
	if (node) return node;
	SetError(422);
	return NULL;
}

/*!\brief Aktuelles Element des Baums
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das aktuelle Element des Baums zurückgeliefert.
 * Dabei wird der Pointer nicht verändert.
 *
 * \return Pointer auf das aktuelle Element des Baums oder NULL, wenn kein
 * Element mehr vorhanden ist. In diesesm Fall wird ausserdem der Fehlercode 422 gesetzt.
 */
CTreeItem *CTree::GetCurrent(CTreeWalker &walk) const
{
	if (walk.current) return walk.current;
	SetError(422);
	return NULL;
}

/*!\brief Elemente des Baums sortiert an eine Callback-Funktion geben
 *
 * \descr
 * \param callback Pointer auf eine von CpplCallback abgeleitete Klasse
 * \returns Die Funktion gibt im Erfolgsfall true (1) zurück, im Fehlerfall false (0).
 * Ein Fehler kann nur auftreten, wenn der Paramater auf NULL zeigt oder der Baum
 * leer ist.
 * \remarks Die Callbackfunktion bekommt das aktuelle Element als void* übergeben
 */
int CTree::ListNodes(CCallback *callback) const
{
	if (!callback) {
		SetError(194,"void CpplAVLTree::List(CpplCallback *callback)");
		return 0;
	}
	if (!root) {
		SetError(347);
		return 0;
	}
	ListNodes(callback,root);
	return 1;
}

/*!\brief Interne Funktion zum Auflisten des Baums
 */
void CTree::ListNodes(CCallback *callback, CTreeItem *node) const
{
	if (node->left) ListNodes(callback,node->left);
	callback->Callback(node);
	if (node->right) ListNodes(callback,node->right);
}

void CTree::PrintNodes(CTreeItem *node) const
{
	CString Buffer;
	if (!node) node=root;
	if (node->left) PrintNodes(node->left);
	if (node->GetValue(&Buffer)) Buffer.Print(true);
	if (node->right) PrintNodes(node->right);

}

void CTree::Graphviz(CString &s, CTreeItem *node) const
/*!\brief Baumdarstellung mit Graphviz
 *
 * Diese Funktion erzeugt einen textuellen graphen, der mit Hilfe von
 * graphviz (dot) grafisch dargestellt werden kann.
 *
 * \param[in] s String, in dem der Code gespeichert werden soll
 * \param[in] node Der Knoten, bei dem begonnen werden soll. Bei Angabe
 * von NULL (Default) wird mit dem root-Knoten begonnen.
 * \since Eingeführt mit PPL 6.2.6
 */
{
	if (!node) node=root;
	s="digraph G {\n";
	Graphviz(s,node,1);
	s+="}\n\n";
	return;
}


void CTree::Graphviz(CString &s, CTreeItem *node, int tiefe) const
/*!\brief Baumdarstellung mit Graphviz
 *
 * Diese Funktion wird intern rekursiv aufgerufen, um einen Graphen zu
 * erstellen, der mit Hilfe von graphviz (dot) grafisch dargestellt werden kann.
 *
 * \param[in] s String, in dem der Code gespeichert werden soll
 * \param[in] node Der Knoten, bei dem begonnen werden soll.
 * \param[in] tiefe Die Tiefe des Baums
 * \since Eingeführt mit PPL 6.2.6
 */

{
	CString b1,b2;
	node->GetValue(&b1);
	if (node->parent) node->parent->GetValue(&b2);
	if (tiefe>31) {
		// Hier stimmt was nicht
		s.Concatf("%llu [label=\"%s\\nparent: %s\\nbal: %i\\nERROR\", style=filled]\n",
				(ppluint64)node, (const char*)b1, (const char*)b2,node->balance);
		return;
	}
	if (node->left) Graphviz(s,node->left,tiefe+1);


	s.Concatf("%llu [label=\"%s\\nparent: %s\\nbal: %i\", style=filled]\n",
			(ppluint64)node, (const char*)b1, (const char*)b2,node->balance);
	if (node->left) {
		node->left->GetValue(&b2);
		s.Concatf("%llu -> %llu\n",(ppluint64)node,(ppluint64)node->left);
	}
	if (node->right) {
		node->right->GetValue(&b2);
		s.Concatf("%llu -> %llu\n",(ppluint64)node,(ppluint64)node->right);
	}

	if (node->right) Graphviz(s,node->right,tiefe+1);
}

} // end of namespace ppl6

