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

#include "ppl6.h"

namespace ppl6 {


static CHeap *Heap=NULL;


/*!\class CTreeController
 * \ingroup PPLGroupDataTypes
 * \brief Virtuelle Basisklasse für Trees
 *
 * \desc
 * Dies ist eine rein virtuelle Basisklasse, die von CAVLTree reimplementiert wird.
 * @return
 */

/*!\fn CTreeController::Compare
 */

/*!\class CAVLTree
 * \brief AVL-Bäume mit nichtypisierten Elementen
 * \ingroup PPLGroupDataTypes
 *
 * \desc
 * Diese Klasse kann zur Verwaltung beliebiger Elemente in einem sortierten AVL-Baum
 * verwendet werden. Die Elemente sind dabei beliebig und werden nur durch ihren
 * Pointer referenziert. Im Gegensatz zu CTree werden die zur Baumstruktur benötigten Daten
 * von der Klasse selbst verwaltet und sind nicht Bestandteil der einzelnen Elemente.
 * \par
 * Dafür kann die Klasse aber nicht direkt verwendet werden, es muss zunächst eine
 * Ableitung erstellt werden, in der mindestens die virtuelle Funktion CAVLTree::Compare
 * implementiert werden muss. Optional können auch noch CAVLTree::DestroyValue und
 * CAVLTree::GetValue implementiert werden.
 * \par
 * Als Alternative kann auch eine Klasse von CTreeController abgeleitet werden, in der die
 * genannten Funktionen implementiert werden. Diese wird über CAVLTree::SetTreeController
 * der AVL-Klasse mitgeteilt, wodurch fortan deren Methoden verwendet werden.
 *
 * \attention Die Klasse verwaltet keinen eigenen Mutex und ist somit nicht Thread-sicher.
 * Die Aufrufende Anwendung muss selbst sicherstellen, dass die Klasse nicht gleichzeitig
 * von mehreren Threads verwendet wird.
 *
 */

/*!\var CAVLTree::root
 * \brief Pointer auf die Wurzel des Baums
 */

/*!\var CAVLTree::count
 * \brief Anzahl Knoten im Baum
 */

/*!\var CAVLTree::current
 * \brief Aktueller Knoten beim Durchwandern des Baums
 */

/*!\var CAVLTree::stack
 * \brief Alle Knoten oberhalb von current
 */

/*!\var CAVLTree::stack_height
 * \brief Anzahl Knoten im Stack
 */

/*!\var CAVLTree::controller
 * \brief Externes von CTreeController abgeleitetes Steuermodul
 */


CAVLTree::CAVLTree()
/*!\brief Konstruktor
 *
 * \desc
 * Im Konstruktor wird der Baum mit NULL initialisiert und ein gemeinsamer
 * Heap-Speicher für die Verwaltung der Knoten-Elemente wird eingerichtet.
 */
{
	if (!Heap) {
		Cppl6Core *core=PPLInit();
		if (!core) {
			throw ppl6::GetError();
		} else {
			Heap=core->GetHeap_TREEITEMS();
			if (!Heap) {
				throw ppl6::GetError();
			}
		}
	}
	controller=0;
	count=0;
	root=NULL;
	current=NULL;
	stack_height=0;
	dupes=false;
}

CAVLTree::~CAVLTree()
/*!\brief Destruktor
 *
 * \desc
 * Der Destruktor ruft die CTree::Clear Funktion auf.
 * \attention
 * Bei einer abgeleiteten Klasse muss der Destruktor ebenfalls implementiert werden und die Funktion
 * CAVLTree::Clear aufrufen, um sicherzustellen, dass alle Elemente gelöscht werden.
 */
{
	//printf ("Destruktor CAVLTree::~CAVLTree\n");
	Clear();
}


int	CAVLTree::Compare(const void *value1, const void *value2) const
/*!\brief Zwei Elemente des Baums vergleichen
 *
 * \desc
 * Damit Elemente sortiert in den Baum eingehangen werden können, muss eine Möglichkeit
 * bestehen zwei Elemente zu vergleichen. Dies wird mit dieser Methode realisiert.
 * Da jeder Baum andere Daten enthalten kann, muss die Methode für jeden Datentyp
 * reimplementiert werden.
 *
 * \param[in] value1 Pointer auf das erste Element. Der Pointer zeigt direkt auf die
 * Daten des Knotens, nicht auf die Verwaltungsdaten des Knotens.
 *
 * \param[in] value2 Pointer auf das zweite Element. Der Pointer zeigt direkt auf die
 * Daten des Knotens, nicht auf die Verwaltungsdaten des Knotens.
 *
 * \returns Die Funktion muss einen der folgenden 4 Werte zurückliefern:
 * - 0: Ist der Wert in \p value2 identisch mit \p value1, muss 0 zurückgegeben werden.
 * - +1: Ist der Wert in \p value2 größer als der Wert in \p value1, muss +1 zurückgegeben werden
 * - -1: Ist der Wert in \p value2 kleiner als der Wert in \p value1, muss -1 zurückgegeben werden
 * - -2: Dieser Wert wird im Fehlerfall zurückgegeben und es wird ein
 *   Fehlercode gesetzt. Die Tree-Operation muss dann ebenfalls mit einem
 *   Fehler abbrechen und diesen Fehlercode zurückgeben.
 *
 * \attention Beim Vergleich zweier Strings kann die Funktion \c strcmp nicht direkt
 * verwendet werden, da sie laut Definition Werte kleiner oder größer 0 liefert, aber
 * nicht exakt -1 oder +1.
 *
 * \example
 * Beispiel für eine Implementierung:
 \code
class MyItem
{
	public:
		MyItem(const char *name) {
			Name=name;
		}
		ppl6::CString Name;
};

class MyTree : public ppl6::CAVLTree
{
	public:
		~MyTree() {
			Clear();
		}
		virtual int	Compare(const void *value1, const void *value2) {
			MyItem *i1=(MyItem*)value1;
			MyItem *i2=(MyItem*)value2;
			int ret=i2->Name.StrCmp(i1->Name);
			if (ret<0) return -1;
			if (ret>0) return 1;
			return 0;
		}
}
\endcode
*/
{
	if (controller) return controller->Compare(value1, value2);
	return -2;
}


int CAVLTree::GetValue(const void *item, CString &buffer) const
/*!\brief Daten als String ausgeben
 *
 * Diese Funktion wird durch CAVLTree::PrintNodes aufgerufen, um den Inhalt des Baums
 * anzeigen zu können. Da jeder Baum andere Daten enthalten kann,
 * muss die Methode für jeden Datentyp reimplementiert werden. Sie bekommt als Eingabe
 * einen Pointer \p item auf die Daten des Knotens und einen String \p buffer, in dem
 * eine textuelle Darstellung des Dateninhalts abgelegt werden soll.
 * \par
 * Die Implementierung der Funktion ist optional.
 *
 * @param[in] item Pointer auf die Daten des Knotens
 * @param[out] buffer String, in dem die textuelle Darstellung der Daten erfolgen soll.
 * @return Bei Erfolg soll die Funktion 1 zurückgeben, im Fehlerfall 0. Wird 0 zurückgegeben,
 * wird der Knoten nicht ausgegeben.
 */
{
	if (controller) return controller->GetValue(item, buffer);
	return 0;
}

int CAVLTree::DestroyValue(void *item) const
/*!\brief Datenelement löschen
 *
 * \desc
 * Diese Funktion wird aufgerufen, wenn ein Wert aus dem Baum gelöscht werden soll,
 * bzw. wenn der komplette Baum gelöscht wird. Da jeder Baum andere Daten enthalten kann,
 * muss die Methode für jeden Datentyp reimplementiert werden.
 *
 * @param item Pointer auf die zu löschenden Daten. Der Pointer zeigt direkt auf die
 * Daten des Knotens, nicht auf die Verwaltungsdaten des Knotens.
 * @return Die Funktion sollte 1 bei erfolgreichem Löschen zurückgeben, im Fehlerfall 0.
 * Allerdings wird der Rückgabewert gegenwärtig nicht geprüft, so dass gegebenenfalls
 * unbemerkt ein Memory-Leak entstehen kann.
 */
{
	//printf ("CAVLTree::DestroyValue der Basisklasse für %u\n",item);
	if (controller) return controller->DestroyValue(item);
	return 0;
}

/*!\brief Duplikate erlauben
 *
 * \desc
 * Mit dieser Funktion kann festgelegt werden, ob Elemente mit gleichem Schlüssel im Baum
 * erlaubt sind. Normalerweise ist dies nicht der Fall. Dabei gilt zu beachten, dass einige
 * Funktionen möglicherweise unerwartet funktionieren. So wird CAVLTree::Find immer nur das
 * erste Element finden können, ebenso CAVLTree::Delete oder CAVLTree::Remove.
 *
 * @param allow Mit \c true werden Duplikate erlaubt, mit \c false nicht. Werden bei einem
 * bereits gefüllten Baum nachträglich Duplikate verboten, hat dies keine Auswirkung auf
 * bereits vorhandene Duplikate.
 */
void CAVLTree::AllowDupes(bool allow)
{
	dupes=allow;
}

/*!\brief Kontrollklasse festlegen
 *
 * \desc
 * Bei Verwendung der CAVLTree-Klasse wird üblicherweise eine eigene Klasse davon abgeleitet und
 * mindestens die virtuelle Funktion CAVLTree::Compare, optional auch CAVLTree::DestroyValue und
 * CAVLTree::GetValue reimplementiert. Es gibt jedoch auch die Möglichkeit CAVLTree ohne
 * Ableitung zu verwenden. Dazu muss die eigene Klasse von CTreeController abgeleitet werden
 * und dessen Pointer mit dieser Funktion an die CAVLTree-Klasse übergeben werden.
 *
 * @param[in] c Pointer auf eine von CTreeController abgeleitete Klasse, die mindestens die
 * virtuelle Funktion CTreeController::Compare implementiert hat.
 */
void CAVLTree::SetTreeController(CTreeController *c)
{
	controller=c;
}


int CAVLTree::Num() const
/*!\brief Anzahl Elemente im Baum
 *
 * \desc
 * Diese Funktion liefert die Anzahl Elemente im Baum zurück.
 *
 * \return Anzahl Elemente oder 0, wenn der Baum leer ist. Bei einem leeren Baum wird
 * ausserdem der Fehlercode 347 gesetzt.
 */
{
	//Heap->Dump();
	if (!count) SetError(347);
	return count;
}

void CAVLTree::Clear()
/*!\brief Baum leeren
 *
 * \desc
 * Mit dieser Funktion wird der Inhalt des Baums gelöscht und sämtlicher durch die Elemente
 * belegter Speicher wieder freigegeben. Dazu wird für jedes Element zunächst CAVLTree::DeleteNode und
 * dann die virtuelle Funktion CAVLTree::DestroyValue aufgerufen. Zuletzt wiird noch der duch den
 * Knoten selbst belegte Speicher wieder freigegeben.
 * \attention
 * Bei einer abgeleiteten Klasse muss diese Funktion durch den Destruktor aufgerufen werden,
 * um sicherzustellen, dass alle Elemente gelöscht werden.
 *
 */
{
	//printf ("Clear wurde aufgerufen, root ist: %u\n",root);
	TREEITEM *node;
	while (root) {
		//printf ("Root ist: %u\n",root);
		node=root;
		DeleteNode(node);		// Element aus dem Baum entfernen
		if (node->data)	DestroyValue((void*)node->data);	// Vom Inhalt belegten Speicher freigeben
		Heap->Free(node);			// Vom Knoten belegten Speicher freigeben
	}
	root=NULL;
	count=0;
	current=NULL;
	stack_height=0;
}

void CAVLTree::Reset()
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

void CAVLTree::PrintNodes(const TREEITEM *node) const
{
	CString Buffer;
	if (!node) {
		if (!root) {
			printf ("CAVLTree is empty\n");
			return;
		}
		node=root;
	}
	if (node->left) PrintNodes(node->left);
	if (GetValue(node->data,Buffer)) Buffer.Print(true);
	if (node->right) PrintNodes(node->right);

}

void *CAVLTree::GetFirst()
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
	TREEITEM *node;
	node = root;
	if (node != NULL)
		while (node->left != NULL) {
    		stack[stack_height++] = node;
			node = node->left;
		}
	current = node;
	if (node) return (void*)node->data;
	SetError(347);
	return NULL;
}

void *CAVLTree::GetNext()
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
	TREEITEM *node=current, *tmp;
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
	if (node) return (void*)node->data;
	SetError(422);
	return NULL;
}

void *CAVLTree::GetLast()
/*!\brief Letztes Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das letzte Element des Baums zurückgeliefert.
 *
 * \return Pointer auf das letzte Element des Baums oder NULL, wenn der Baum leer ist
 */
{
	TREEITEM *node;
	stack_height = 0;
	node = root;
	if (node != NULL)
		while (node->right != NULL) {
			stack[stack_height++] = node;
			node = node->right;
		}
	current = node;
	if (node) return (void*)node->data;
	SetError(347);
	return NULL;
}

void *CAVLTree::GetPrevious()
/*!\brief Vorheriges Element aus dem Baum
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das vorherige Element des Baums zurückgeliefert.
 * Somit kann der Baum sortiert rückwärts durchwandert werden.
 *
 * \return Pointer auf das vorherige Element des Baums oder NULL, wenn keine weiteren
 * Elemente vorhanden sind. In diesesm Fall wird ausserdem der Fehlercode 422 gesetzt.
 */
{
	TREEITEM *node, *y;
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
	if (node) return (void*)node->data;
	SetError(422);
	return NULL;
}

void *CAVLTree::GetCurrent()
/*!\brief Aktuelles Element des Baums
 *
 * \desc
 * Mit dieser Funktion wird ein Pointer auf das aktuelle Element des Baums zurückgeliefert.
 * Dabei wird der Pointer nicht verändert.
 *
 * \return Pointer auf das aktuelle Element des Baums oder NULL, wenn kein
 * Element mehr vorhanden ist. In diesesm Fall wird ausserdem der Fehlercode 422 gesetzt.
 */
{
	if (current) return (void*)current->data;
	SetError(422);
	return NULL;
}

TREEITEM *CAVLTree::FindNode(const void *value) const
/*!\brief Wert im Baum finden
 *
 * \desc
 * Mit dieser Funktion wird der Wert \p value innerhalb des Baums gesucht und dessen
 * Baumelement (TREEITEM) zurückgegeben.
 *
 * \param[in] value Der zu suchende Wert
 * \returns Wird der Wert im Baum gefunden, gibt die Funktion einen Pointer auf den
 * Wert des Baum-Elements zurück. Falls nicht, wird NULL zurückgegeben und der Fehlercode
 * 421 gesetzt.
 */
{
	TREEITEM *cur=root;
	int cmp;
	while (cur) {
		cmp=Compare(cur->data,value);
		if (cmp==-1) cur=cur->left;
		else if (cmp==1) cur=cur->right;
		else if (cmp==-2) return NULL;
		else return cur;
	}
	SetError(421);
	return NULL;
}

void *CAVLTree::Find(const void *value) const
/*!\brief Element im Baum finden
 *
 * \desc
 * Mit dieser Funktion wird ein Element innerhalb des Baums gesucht. Dazu wird
 * die Funktion CTree::CompareItems verwendet.
 *
 * \param[in] value Der zu suchende Wert
 * \returns Wird das Element im Baum gefunden, gibt die Funktion einen Pointer auf den
 * Wert des Baum-Elements zurück. Falls nicht, wird NULL zurückgegeben und der Fehlercode
 * 421 gesetzt.
 */
{
	TREEITEM *cur=root;
	int cmp;
	while (cur) {
		cmp=Compare(cur->data,value);
		if (cmp==-1) cur=cur->left;
		else if (cmp==1) cur=cur->right;
		else if (cmp==-2) return NULL;
		else return (void*)cur->data;
	}
	SetError(421);
	return NULL;
}


int CAVLTree::Add(const void *value)
/*!\brief Element hinzufügen
 *
 * \desc
 * Diese Funktion fügt ein neues Element in den Baum ein. Dabei ist sichergestellt,
 * dass der Baum stets sortiert und ausgewogen ist. Im Gegensatz zu CTree erlaub CAVLTree
 * auch mehrere Elemente mit dem gleichen Schlüssel. Dieses Feature muss jedoch explizit
 * durch Aufruf der Funktion CAVLTree::AllowDupes aktiviert werden.
 *
 * \param[in] value Pointer auf den hinzuzufügenden Wert
 * \returns Wurde das Element erfolgreich hinzugefügt, gibt die Funktion true (1) zurück,
 * sonst false (0) und ein entsprechender Fehlercode wird gesetzt,
 */
{
	TREEITEM *cur=root;
	TREEITEM *node=NULL;
	int cmp=0;
	if (!value) {
		SetError(194);
		return 0;
	}

	TREEITEM *item=(TREEITEM *)Heap->Malloc();
	if (!item) return 0;
	item->data=value;
	item->right=item->left=NULL;
	item->balance=0;
	if (!root) {
		root=item;
		item->parent=NULL;
		count++;
		Reset();
		return 1;
	}
	while (cur) {
		node=cur;
		cmp=Compare(cur->data,value);
		if (cmp==-2) {
			ppl6::PushError();
			Heap->Free(item);
			ppl6::PopError();
			return 0;
		} else if (cmp==0 && dupes==false) {
			ppl6::SetError(418);
			return 0;
		} else if (cmp<0) cur=cur->left;
		else cur=cur->right;
	}
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

int CAVLTree::Delete(const void *value)
/*!\brief Element anhand eines Wertes löschen
 *
 * \desc
 * Diese Funktion sucht zunächst ob der angegebene Wert im Baum enthalten ist und
 * löscht anschließend das gefundene Element. Dabei ist sichergestellt,
 * dass der Baum stets sortiert und ausgewogen ist.
 * \par
 * Beim Löschen des Elements wird auch die virtuelle Funktion CAVLTree::DestroyValue
 * aufgerufen. Diese muss sicherstellen, dass der durch das Element belegte Speicher
 * ebenfalls freigegeben wird.
 * \par Falls das Element nur aus dem Baum entfernt werden, aber der durch das Element
 * belegte Speicher erhalten bleiben soll, kann stattdessen die Funktion CAVLTree::Remove
 * aufgerufen werden.
 *
 * \param[in] value Pointer auf den zu löschenden Wert
 * \returns Wurde das Element erfolgreich gelöscht, gibt die Funktion true (1) zurück,
 * sonst false (0) und ein entsprechender Fehlercode wird gesetzt.
 */
{
	TREEITEM *item=FindNode(value);
	if (!item) {
		SetError(421);
		return 0;
	}
	if (DeleteNode(item)) {
		if (item->data)	DestroyValue((void*)item->data);
		Heap->Free(item);
		return 1;
	}
	return 0;
}

int CAVLTree::Remove(const void *value)
/*!\brief Element aus dem AVL-Baum entfernen
 *
 * \desc
 * Diese Funktion sucht zunächst ob der angegebene Wert im Baum enthalten ist und
 * löscht anschließend das gefundene Element aus dem Baum. Dabei ist sichergestellt,
 * dass der Baum stets sortiert und ausgewogen ist.
 * \par
 * Mit dieser Funktion wird das Element nur aus dem AVL-Baum entfernt, der durch das Element
 * belegte Speicher wird jedoch nicht freigegeben. Falls dies gewünscht ist, kann stattdessen
 * die Funktion CAVLTree::Delete aufgerufen werden.
 *
 * \param[in] value Pointer auf den zu entfernenden Wert
 * \returns Wurde das Element erfolgreich entfernt, gibt die Funktion true (1) zurück,
 * sonst false (0) und ein entsprechender Fehlercode wird gesetzt.
 */
{
	TREEITEM *item=FindNode(value);
	if (!item) {
		SetError(421);
		return 0;
	}
	if (DeleteNode(item)) {
		Heap->Free(item);
		return 1;
	}
	return 0;
}

int CAVLTree::DeleteNode(TREEITEM *item)
/*!\brief Interne Funktion, die das tatsächliche Löschen vornimmt
 *
 * Dies ist eine interne Funktion, die den tatsächlichen Löschvorgang aus dem AVL-Baum vornimmt.
 * Der Inhalt des Knotens \p item und auch der Knoten selbst werden dabei nicht
 * gelöscht, dies muss die Aufrufende Funktion vornehmen. Sie wird von CAVLTree::Delete aufgerufen.
 *
 * \param[in] item Pointer auf die Verwaltungsstruktur des Baumelements
 * \return Wurde das Element erfolgreich gelöscht, gibt die Funktion true (1) zurück,
 * sonst false (0) und ein entsprechender Fehlercode wird gesetzt,
 *
 */
{
	TREEITEM *father;
	if((item->left)&&(item->right)) {
		TREEITEM *sys;
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
			//DestroyValue((void*)item->data);
			//Heap->Free(item);
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
			//DestroyValue((void*)item->data);
			//Heap->Free(item);
			count--;
			Reset();
			return(1);
		}
	}
	TREEITEM *sohn;
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
	//DestroyValue((void*)item->data);
	//Heap->Free(item);
	count--;
	Reset();
	return(1);
}


void CAVLTree::UpInsert(TREEITEM *node)
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

void CAVLTree::UpDelete(TREEITEM *node)
{
	TREEITEM *parent=node->parent;
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

TREEITEM *CAVLTree::Rotate(TREEITEM *kn)
{
	TREEITEM *child;
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

void CAVLTree::SwapNodes(TREEITEM *item1, TREEITEM *item2)
{
	signed char	balance;
	TREEITEM *left, *right, *parent;
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
}



CAVLTree::Walker::Walker()
{
	current=NULL;
	stack_height=0;
}

void CAVLTree::Reset(Walker &walk) const
{
	walk.current=NULL;
	walk.stack_height=0;
}

void *CAVLTree::GetFirst(Walker &walk) const
{
	walk.current=NULL;
	walk.stack_height=0;
	TREEITEM *node;
	node = root;
	if (node != NULL)
		while (node->left != NULL) {
    		walk.stack[walk.stack_height++] = node;
			node = node->left;
		}
	walk.current = node;
	if (node) return (void*)node->data;
	SetError(347);
	return NULL;
}

void *CAVLTree::GetNext(Walker &walk) const
{
	TREEITEM *node=walk.current, *tmp;
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
	if (node) return (void*)node->data;
	SetError(422);
	return NULL;
}

void *CAVLTree::GetLast(Walker &walk) const
{
	TREEITEM *node;
	walk.stack_height = 0;
	node = root;
	if (node != NULL)
		while (node->right != NULL) {
			walk.stack[walk.stack_height++] = node;
			node = node->right;
		}
	walk.current = node;
	if (node) return (void*)node->data;
	SetError(347);
	return NULL;
}

void *CAVLTree::GetPrevious(Walker &walk) const
{
	TREEITEM *node, *y;
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
	if (node) return (void*)node->data;
	SetError(422);
	return NULL;
}

void *CAVLTree::GetCurrent(Walker &walk) const
{
	if (walk.current) return (void*)walk.current->data;
	SetError(422);
	return NULL;
}


}	// EOF namespace ppl6
