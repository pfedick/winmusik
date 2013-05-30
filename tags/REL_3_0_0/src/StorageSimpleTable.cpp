/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:30 $
 * $Id: StorageSimpleTable.cpp,v 1.1 2010/05/16 12:23:30 pafe Exp $
 *
 *
 * Copyright (c) 2009 Patrick Fedick
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

#include "winmusik3.h"

/*!\class DataVersion
 * \brief Datentyp für Musikversion
 * \ingroup WM3DataTypes
 */

/*!\class DataRecordSource
 * \brief Datentyp für Aufnahmequellen
 * \ingroup WM3DataTypes
 */

/*!\class DataLabel
 * \brief Datentyp für Plattenlabels
 * \ingroup WM3DataTypes
 */

/*!\class DataPurchaseSource
 * \brief Datentyp für Kaufquellen
 * \ingroup WM3DataTypes
 */

/*!\class DataRecordDevice
 * \brief Datentyp für Aufnahmegeräte
 * \ingroup WM3DataTypes
 */

/*!\class DataGenre
 * \brief Datentyp für Musik Genres
 * \ingroup WM3DataTypes
 */


/*!\class CSimpleTable
 * \brief Basisklasse für verschiedene "Index-Wert" Datentypen
 *
 * Dies ist die Basisklasse für verschiedene "Index-Wert" Datentypen. Dazu zählen:
 * - CVersion
 * - CRecordSource
 * - CLabel
 * - CPurchaseSource
 * - CRecordDevice
 * - CGenre
 *
 * Es handelt sich hier um einen einfachen Datentyp, der nur aus einem nummerischen Index und einem String
 * als Wert besteht. Der Datentyp ist von ppl6::CTreeItem abgeleitet und kann somit in einem Binären Baum
 * verwaltet werden. Als Schlüssel dient dazu der Wert des Strings.
 */


/*!\var CSimpleTable::Id
 * \brief ID des Datensatzes
 */

/*!\var CSimpleTable::References
 * \brief Anzahl Referenzierungen auf diesen Datensatz
 */

/*!\var CSimpleTable::Value
 * \brief Pointer auf den Text dieses Datensatzes
 */

CSimpleTable::CSimpleTable()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse
 */
{
	Id=0;
	References=0;
	Value=NULL;
	formatversion=1;
}

CSimpleTable::~CSimpleTable()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Er sorgt dafür, dass der durch den Wert allokierte Speicher wieder freigegeben wird.
 */
{
	Clear();
}

int CSimpleTable::CompareNode(CTreeItem *item)
/*!\brief Elemente vergleichen
 *
 * \desc
 * Funktion zum Vergleichen zweier Baumelemente.
 * Bei dieser Funktion wird als Parameter ein Pointer auf ein Baum-Element erwartet.
 *
 * \param[in] item Pointer auf ein Baum-Element, mit dem dieses Element verglichen werden soll
 *
 * \returns Die Funktion liefert einen der folgenden Werte zurück:
 * - 0: Der Wert in \p item ist identisch mit dem Wert dieses Elements
 * - +1: Der Wert in \p item ist größer als der Wert dieses Elements
 * - -1: Der Wert in \p item ist kleiner als der Wert dieses Elements
 */
{
	CSimpleTable *n=(CSimpleTable *)item;
	int ret=strcasecmp(n->Value,Value);
	if (ret<0) return -1;
	if (ret>0) return 1;
	return 0;
}

int CSimpleTable::CompareValue(void *value)
/*!\brief Elemente vergleichen
 *
 * \desc
 * Funktion zum Vergleichen zweier Baumelemente.
 * Bei dieser Funktion wird als Parameter ein void Pointer auf einen beliebigen
 * Wert erwartet.
 *
 * \param[in] value Pointer auf einen String.
 *
 * \returns Die Funktion liefert einen der folgenden Werte zurück:
 * - 0: Der Wert in \p value ist identisch mit dem Wert dieses Elements
 * - +1: Der Wert in \p value ist größer als der Wert dieses Elements
 * - -1: Der Wert in \p value ist kleiner als der Wert dieses Elements
 */
{
	const char *v=(const char *)value;
	int ret=strcasecmp(v,Value);
	if (ret<0) return -1;
	if (ret>0) return 1;
	return 0;
}


void CSimpleTable::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion wird der durch diese Klasse allokierte Speicher wieder freigegeben und
 * alle internen Variablen deinitialisiert.
 */
{
	Id=0;
	References=0;
	if (Value) free(Value);
	Value=NULL;
	CStorageItem::Clear();
	formatversion=1;
}

int CSimpleTable::CopyFrom(CSimpleTable *t)
/*!\brief Daten kopieren
 *
 * Mit dieser Funktion werden die Daten einers anderen CSimpleTable Datensatzes in diesen hineinkopiert.
 * Bereits vorhandene Daten werden dabei überschrieben.
 *
 * \param[in] t Pointer auf einen anderen Datensatz vom Typ CSimpleTable
 *
 * \returns Konnten die Daten erfolgreich kopiert werden, liefert die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	if (!t) {
		ppl6::SetError(194,"int CSimpleTable::CopyFrom(==> CSimpleTable *t <==)");
		return 0;
	}
	Clear();
	Id=t->Id;
	References=t->References;
	if (t->Value) {
		Value=strdup(t->Value);
		if (!Value) {	// Out of Memory
			Clear();
			if (wmlog) {
				wmlog->Printf(ppl6::LOG::ERROR,2,"CSimpleTable","CopyFrom",__FILE__,__LINE__,"this: %llu, strdup failed, id: %u, references: %u, *Value=%llu",(ppluint64)((ppliptr)this),Id,References,(ppluint64)((ppliptr)t->Value));
				wmlog->HexDump(t->Value,32);
			}

			ppl6::SetError(2);
			return 0;
		}
	} else {
		Value=strdup("");
		if (!Value) {	// Out of Memory
			Clear();
			ppl6::SetError(2);
			return 0;
		}
	}
	CopyStorageFrom(t);
	return 1;
}

int CSimpleTable::SetValue(const char *value)
/*!\brief Wert setzen
 *
 * Mit dieser Funktion wird der Wert der Klasse verändert.
 *
 * \param[in] value Pointer auf den String, der übernommen werden soll
 * \returns Im Erfolgsfall wird 1 zurückgegeben, sonst 0
 *
 * \attention Der Wert darf nur geändert werden, wenn die Klasse noch nicht Teil eines Trees ist,
 * da sonst die Sortierung durcheinander gerät.
 */
{
	if (Value) free(Value);
	Value=NULL;
	if (value) {
		Value=strdup(value);
		if (!Value) {	// Out of Memory
			ppl6::SetError(2);
			return 0;
		}
	}
	return 1;
}

ppl6::CBinary *CSimpleTable::Export()
/*!\brief Binäre Exportfunktion
 *
 * Mit dieser Funktion werden die Daten der Klasse in binärer Form exportiert. Das Format ist
 * folgendermassen:
 *
 * \copydoc WM3File_Chunk_TABL
 *
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf eine neue ppl6::CBinary Klasse zurück.
 * Diese muss nach Gebrauch vom aufrufenden Programm selbst gelöscht werden. Im Fehlerfall wird NULL
 * zurückgegeben.
 */
{
	// Zunächst den benötigten Speicher berechnen
	int size=9;
	if (Value) size+=strlen(Value);
	char *a=(char*)malloc(size);
	if (!a) {
		ppl6::SetError(2);
		return NULL;
	}
	ppl6::Poke32(a,Id);
	ppl6::Poke32(a+4,References);
	if (Value) strcpy(a+8,Value);
	else ppl6::Poke8(a+8,0);
	ppl6::CBinary *bin=new ppl6::CBinary;
	if (!bin) {
		ppl6::SetError(2);
		return NULL;
	}
	bin->Set(a,size);
	// CBinary ist verantwortlich den Speicher wieder freizugeben
	bin->ManageMemory();
	return bin;
}

int CSimpleTable::Import(ppl6::CBinary *bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion werden binäre gespeicherte Daten in die Klasse importiert. Eine Beschreibung des
 * Binären Formats ist in der Funktion CSimpleTable::Export zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (!bin) {
		ppl6::SetError(194,"int CSimpleTable::Import(==> ppl6::CBinary *bin <==)");
		return 0;
	}
	if (version<1 || version>1) {
		ppl6::SetError(20023,"%i",version);
		return 0;
	}
	int size=bin->Size();
	const char *a=(char*)bin->GetPtr();
	// Die Größe muss mindestens 9 Byte betragen
	if (size<9 || a==NULL) {
		ppl6::SetError(20013);
		return 0;
	}
	Clear();
	Id=ppl6::Peek32(a);
	References=ppl6::Peek32(a+4);
	if (ppl6::Peek8(a+8)!=0) Value=ppl6::strndup(a+8,size-8);
	return 1;
}


/*!\class CTableStore
 * \brief Klasse zum Verwalten von CSimpleTable Datensätzen
 *
 * Klasse zum Verwalten von CSimpleTable Datensätzen
 *
 */

/*!\var CTableStore::Mutex
 * \brief Mutex der Klasse
 *
 * Die Klasse verwendet einen Mutex bei allen Zugriffen und ist somit Threadsicher.
 */

/*!\var CTableStore::Tree
 * \brief Interner Binary Tree zur Optimierung der Textsuche
 *
 * Interner Binary Tree zur Optimierung der Textsuche
 */

/*!\var CTableStore::TableIndex
 * \brief Tabelle mit Pointern auf die eigentlichen Datensätze.
 *
 * Tabelle mit Pointern auf die eigentlichen Datensätze vom Typ CSimpleTable.
 * Diese Tabelle wird intern von der Klasse verwaltet und automatisch erweitert.
 * Jede ID entspricht einem Eintrag in der Tabelle. Ein NULL-Wert bedeutet, dass
 * es zu dieser ID noch keinen Datensatz gibt.
 */

/*!\var CTableStore::max
 * \brief Größe der internen Tabelle CTableStore::TableIndex
 *
 * Dieser Wert gibt an, wieviele Einträge die interne Pointer-Tabelle maximal enthalten kann.
 * Der Wert ist somit identisch mit der höchst möglichen ID. Soll ein Datensatz mit einer höheren
 * ID gespeichert werden, wird die Tabelle automatisch erweitert.
 */

/*!\var CTableStore::highestId
 * \brief Enthält die höchste bisher verwendete ID in der Tabelle.
 *
 * Enthält die höchste bisher verwendete ID in der Tabelle.
 */

CTableStore::CTableStore()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse
 */
{
	TableIndex=NULL;
	max=0;
	highestId=0;
	Storage=NULL;
}

CTableStore::~CTableStore()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Stellt sicher, dass sämtlicher durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen wird. Dazu wird die Member-Funktion CTableStore::Clear
 * aufgerufen.
 */
{
	Clear();
}

void CTableStore::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion kann der komplette durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen werden.
 */
{
	/*
	for (ppluint32 i=0;i<max;i++) {
		if (TableIndex[i].t!=NULL) delete TableIndex[i].t;
	}
	*/
	Tree.Clear(true);
	free(TableIndex);
	TableIndex=NULL;
	max=0;
	highestId=0;
}

const char *CTableStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return NULL;
}

int CTableStore::Increase(ppluint32 maxid)
/*!\brief Interne Tabelle vergrößern
 *
 * Diese Funktion wird intern aufgerufen um die Pointertabelle mit den Datensätzen
 * zu vergrößern.
 *
 * \param[in] maxid Die höchste ID, die benötigt wird. Die Funktion rechnet zusätzlich noch
 * 100 Datensätze hinzu, so dass die Tabelle nicht so schnell erneut erweitert werden
 * muss.
 * \returns Im Erfolgsfall liefert die Funktion 1 zurück, sonst 0. Ein Fehler kann nur
 * auftreten, wenn kein Speicher mehr zur Verfügung steht.
 */
{
	ppluint32 h=maxid+100;
	void *t=calloc(sizeof(TABLE),h);
	if (!t) {
		ppl6::SetError(2,"int CTableStore::Increase(ppluint32 maxid)");
		wmlog->Printf(ppl6::LOG::ERROR,2,"CTableStore","Increase",__FILE__,__LINE__,"Faild to increase Table, could not alloc %u*%u Bytes (=%u) ",sizeof(TABLE),h,sizeof(TABLE)*h);
		return 0;
	}
	memcpy(t,TableIndex,max*sizeof(TABLE));
	free(TableIndex);
	TableIndex=(TABLE*)t;
	max=h;
	return 1;
}

int CTableStore::Save(CSimpleTable *t)
/*!\brief Datensatz auf Festplatte schreiben
 *
 * Diese Funktion speichert den übergebenen Datensatz auf die Festplatte. Dazu wird der Inhalt
 * zunächst als Binary exportiert (CSimpleTable::Export) und damit CStorage::Save aufgerufen.
 * Die Funktion wird intern von CTableStore::Put verwendet.
 *
 * \param[in] t Pointer auf einen CSimpleTable Datensatz
 * \returns Konnte der Datensatz erfolgreich gespeichert werden, liefert die Funktion 1 zurück, sonst
 * 0.
 */
{
	if (!Storage) {
		return 0;
	}
	if (!t) {
		ppl6::SetError(194,"int CTableStore::Save(==> CSimpleTable *t <==)");
		return 0;
	}
	ppl6::CBinary *bin=t->Export();
	if (!bin) return 0;
	if (!Storage->Save(this,t,bin)) {
		ppl6::PushError();
		delete bin;
		ppl6::PopError();
		return 0;
	}
	delete bin;
	return 1;
}

int CTableStore::Put(CSimpleTable *entry)
/*!\brief Datensatz speichern
 *
 * Mit dieser Funktion wird ein veränderter oder neuer Datensatz im Speicher der Anwendung und
 * auf Festplatte gespeichert.
 *
 * \param[in] entry Pointer auf den gewünschten Datensatz vom Typ CSimpleTable. Dieser wird nicht
 * selbst gespeichert, sondern es wird eine Kopie erstellt. Der Wert muss daher von der Anwendung
 * nach Gebrauch wieder freigegeben werden. Ist der Wert CSimpleTable::Id == 0 wird automatisch eine
 * freie ID vergeben. Andernfalls wird der vorhandene Datensatz überschrieben.
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 * \note Es ist sichergestellt, dass der interne Suchbaum stets sortiert ist, auch wenn sich der
 * Text des Elements geändert hat.
 */
{
	if (!entry) {
		ppl6::SetError(194,"int CTableStore::Put(==> CSimpleTable *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014,"CTableStore");
		return 0;
	}
	ppluint32 save_highestId=highestId;
	ppluint32 id=0;
	Mutex.Lock();
	if (entry->Id==0) {
		// Wir haben einen neuen Eintrag und vergeben eine Id
		highestId++;
		id=highestId;
	} else {
		id=entry->Id;
		if (id>highestId) highestId=id;
	}

	if (id>=max) {
		if (!Increase(id)) {
			highestId=save_highestId;
			Mutex.Unlock();
			return 0;
		}
	}
	// Gibt's den Titel schon?
	if (TableIndex[id].t) {
		// CopyFrom führt ein Clear aus, daher müssen wir die Storage Daten retten
		CStorageItem ssave;
		ssave.CopyStorageFrom(TableIndex[id].t);
		// Nun können wir die Daten kopieren
		if (!TableIndex[id].t->CopyFrom(entry)) {
			wmlog->Printf(ppl6::LOG::ERROR,2,"CTableStore","Put",__FILE__,__LINE__,"CopyFrom failed, existing record");
			highestId=save_highestId;
			Mutex.Unlock();
			return 0;
		}
		// StorageDaten wieder herstellen
		TableIndex[id].t->CopyStorageFrom(&ssave);
		// Datensatz zunächst aus dem Baum löschen, da sich der Text geändert haben kann
		Tree.Delete(TableIndex[id].t);
		// Dann mit dem neuen Text wieder einfügen
		Tree.Add(TableIndex[id].t);

		TableIndex[id].t->Id=id;
		if (!Save(TableIndex[id].t)) {
			wmlog->Printf(ppl6::LOG::ERROR,2,"CTableStore","Put",__FILE__,__LINE__,"Save failed, existing record");
			highestId=save_highestId;
			Mutex.Unlock();
			return 0;
		}
		// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
		entry->CopyStorageFrom(TableIndex[id].t);
		Mutex.Unlock();
		return 1;
	}
	// Nein, neuer Titel
	TableIndex[id].t=new CSimpleTable;
	if (!TableIndex[id].t) {
		wmlog->Printf(ppl6::LOG::ERROR,2,"CTableStore","Put",__FILE__,__LINE__,"out of memory, could not create object");
		highestId=save_highestId;
		Mutex.Unlock();
		ppl6::SetError(2);
		return 0;
	}
	if (!TableIndex[id].t->CopyFrom(entry)) {
		wmlog->Printf(ppl6::LOG::ERROR,2,"CTableStore","Put",__FILE__,__LINE__,"CopyFrom failed, new record");
		ppl6::PushError();
		delete TableIndex[id].t;
		TableIndex[id].t=NULL;
		highestId=save_highestId;
		Mutex.Unlock();
		ppl6::PopError();
		return 0;
	}
	TableIndex[id].t->Id=id;
	if (!Save(TableIndex[id].t)) {
		wmlog->Printf(ppl6::LOG::ERROR,2,"CTableStore","Put",__FILE__,__LINE__,"Save failed, new record");
		ppl6::PushError();
		delete TableIndex[id].t;
		TableIndex[id].t=NULL;
		highestId=save_highestId;
		Mutex.Unlock();
		ppl6::PopError();
		return 0;
	}
	// Den internen Datensatz in den Tree hängen
	Tree.Add(TableIndex[id].t);
	// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
	entry->CopyStorageFrom(TableIndex[id].t);
	entry->Id=id;
	Mutex.Unlock();
	return 1;
}

CSimpleTable *CTableStore::Get(ppluint32 id)
/*!\brief Datensatz auslesen
 *
 * Mit dieser Funktion kann der zu \p id zugehörige Datensatz ausgelesen werden.
 *
 * \param[in] id ID des gewünschten Datensatzes
 * \returns Liefert einen Pointer auf den gewünschten Datensatz zurück, oder NULL, wenn
 * er nicht vorhanden ist.
 *
 * \attention Der Datensatz darf nicht selbst verändert werden, da sonst die interne
 * Sortierung der Texte durcheinander gerät. Ist dies beabsichtigt, muss entwerder
 * eine Kopie gemacht werden (CSimpleTable::CopyFrom) oder gleich eine Kopie
 * geholt werden (CTableStore::GetCopy).
 */
{
	if (id>highestId) {
		ppl6::SetError(20015,"%u",id);
		return NULL;
	}
	if (TableIndex==NULL || TableIndex[id].t==NULL) {
		ppl6::SetError(20015,"%u",id);
		return NULL;
	}
	return TableIndex[id].t;
}

CSimpleTable *CTableStore::Find(const char *value)
/*!\brief Datensatz finden
 *
 * Mit dieser Funktion kann ein Datensatz anhand seines Textwertes gefunden werden.
 *
 * \param[in] value Pointer auf den zu suchenden String
 * \returns Wurde der angegebene String \p value gefunden, wird ein Pointer auf den
 * CSimpleTable Datensatz zurückgeliefert, der den Text enthält. Wurde er nicht
 * gefunden, wird NULL zurückgegeben.
 */
{
	CSimpleTable *t;
	Mutex.Lock();
	t=(CSimpleTable *)Tree.Find((void *)value);
	Mutex.Unlock();
	return t;
}

int CTableStore::FindAll(ppl6::CWString &value, ppl6::CTree &Result)
{
	Result.Clear(true);
	value.Trim();
	value.LCase();
	Mutex.Lock();
	CSimpleTable *t, *copy;
	ppl6::CWString Tmp;
	Tree.Reset();
	while ((t=(CSimpleTable *)Tree.GetNext())) {
		if (t->Value) {
			Tmp=t->Value;
			Tmp.LCase();
			if (Tmp.Instr(value)>=0) {
				copy=new CSimpleTable;
				copy->CopyFrom(t);
				Result.Add(copy);
			}
		}
	}
	Mutex.Unlock();
	return Result.Num();
}

int CTableStore::GetCopy(ppluint32 id, CSimpleTable *t)
/*!\brief Kopie eines Datensatzes erstellen
 *
 * Mit dieser Funktion kann eine Kopie eines vorhandenen Datensatzes erstellt werden.
 *
 * \param[in] id ID des gewünschten Datensatzes
 * \param[in,out] t Pointer auf eine Klasse vom Typ CSimpleTable
 *
 * \returns Ist der Datensatz vorhanden und konnte kopiert werden, liefert die Funktion 1
 * zurück, sonst 0.
 */
{
	CSimpleTable *entry=Get(id);
	if (!entry) return 0;
	t->CopyFrom(entry);
	return 1;
}

int CTableStore::LoadChunk(CWMFileChunk *chunk)
{
	CSimpleTable data;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk->GetChunkData(),chunk->GetChunkDataSize())) {
		return 0;
	}
	if (!data.Import(&bin,chunk->GetFormatVersion())) {
		return 0;
	}
	data.CopyStorageFrom(chunk);
	if (!Put(&data)) {
		return 0;
	}
	return 1;
}


/*!\class CVersionStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Musikversionen
 *
 * Klasse zum Verwalten von Musikversionen
 */

const char *CVersionStore::GetChunkName()
{
	return "VERS";
}

/*!\class CRecordSourceStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Aufnahmequellen
 *
 * Klasse zum Verwalten von Aufnahmequellen
 */

const char *CRecordSourceStore::GetChunkName()
{
	return "RSRC";
}

/*!\class CLabelStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Plattenlabels
 *
 * Klasse zum Verwalten von Plattenlabels
 */

const char *CLabelStore::GetChunkName()
{
	return "LABL";
}

/*!\class CPurchaseSourceStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Kaufquellen
 *
 * Klasse zum Verwalten von Kaufquellen
 */

const char *CPurchaseSourceStore::GetChunkName()
{
	return "PCHS";
}

/*!\class CRecordDeviceStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Aufnahmegeräten
 *
 * Klasse zum Verwalten von Aufnahmegeräten
 */

const char *CRecordDeviceStore::GetChunkName()
{
	return "RDEV";
}

/*!\class CGenreStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von Musik Genres
 *
 * Klasse zum Verwalten von Musik Genres
 */

const char *CGenreStore::GetChunkName()
{
	return "GENR";
}