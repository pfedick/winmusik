/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: StorageOriginalImportData.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
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

/*!\class DataOimp
 * \ingroup WM3DataTypes
 * \brief Klasse zum Speichern von Original MP3-Informationen
 *
 * Diese Datenklasse wird verwendet, um beim Import von MP3-Dateien die Originalinformationen
 * der ID3-Tags und den Dateinamen zu sichern. Im Unterschied zu allen anderen Datenklassen
 * werden die Informationen nicht im Hauptspeicher gehalten, sondern nur bei Bedarf
 * eingelesen.
 */


/*!\var DataOimp::Id
 * \brief ID des Datensatzes
 */

/*!\var DataOimp::Filename
 * \brief Original Dateiname
 */

/*!\var DataOimp::ID3v1
 * \brief Original ID3v1-Tag
 */

/*!\var DataOimp::ID3v2
 * \brief Original ID3v2-Tag
 */

DataOimp::DataOimp()
/*!\brief Konstruktor der Klasse
 *
 * Konstruktor der Klasse
 */
{
	Id=0;
}

DataOimp::~DataOimp()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Er sorgt dafür, dass der durch den Wert allokierte Speicher wieder freigegeben wird.
 */
{
	Clear();
}

void DataOimp::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion wird der durch diese Klasse allokierte Speicher wieder freigegeben und
 * alle internen Variablen deinitialisiert.
 */
{
	Id=0;
	Filename.Clear();
	ID3v1.Clear();
	ID3v2.Clear();
	CStorageItem::Clear();
	formatversion=1;
}

int DataOimp::CopyFrom(const DataOimp *t)
/*!\brief Daten kopieren
 *
 * Mit dieser Funktion werden die Daten einers anderen DataOimp Datensatzes in diesen hineinkopiert.
 * Bereits vorhandene Daten werden dabei überschrieben.
 *
 * \param[in] t Pointer auf einen anderen Datensatz vom Typ DataOimp
 *
 * \returns Konnten die Daten erfolgreich kopiert werden, liefert die Funktion 1 zurück, im Fehlerfall 0.
 */
{
	if (!t) {
		ppl6::SetError(194,"int DataOimp::CopyFrom(==> DataOimp *t <==)");
		return 0;
	}
	Clear();
	Id=t->Id;
	Filename=t->Filename;
	ID3v1=t->ID3v1;
	ID3v2=t->ID3v2;
	CopyStorageFrom(t);
	return 1;
}

ppl6::CBinary *DataOimp::Export()
/*!\brief Binäre Exportfunktion
 *
 * Mit dieser Funktion werden die Daten der Klasse in binärer Form exportiert. Das Format ist
 * folgendermassen:
 *
 * \copydoc WM3File_Chunk_OIMP
 *
 * \returns Im Erfolgsfall liefert die Funktion einen Pointer auf eine neue ppl6::CBinary Klasse zurück.
 * Diese muss nach Gebrauch vom aufrufenden Programm selbst gelöscht werden. Im Fehlerfall wird NULL
 * zurückgegeben.
 */
{
	// Zunächst den benötigten Speicher berechnen
	int size=10+Filename.Len();
	ppl6::CBinary *v1=ID3v1.ExportBinary();
	if (v1) size+=v1->Len();
	ppl6::CBinary *v2=ID3v2.ExportBinary();
	if (v2) size+=v2->Len();
	char *a=(char*)malloc(size);
	if (!a) {
		delete v1;
		delete v2;
		ppl6::SetError(2);
		return NULL;
	}
	int p=4;
	ppl6::Poke32(a,Id);
	ppl6::Poke16(a+p,Filename.Len());
	p+=2;
	memcpy(a+p,Filename.GetPtr(),Filename.Len());
	p+=Filename.Len();
	if (v1!=NULL && v1->Len()>0) {
		ppl6::Poke16(a+p,v1->Len());
		memcpy(a+p+2,v1->GetPtr(),v1->Size());
		p+=v1->Size();
	} else {
		ppl6::Poke16(a+p,0);
	}
	p+=2;
	if (v2!=NULL && v2->Len()>0) {
		ppl6::Poke16(a+p,v2->Len());
		memcpy(a+p+2,v2->GetPtr(),v2->Size());
		p+=v2->Size();
	} else {
		ppl6::Poke16(a+p,0);
	}
	p+=2;

	/*
	ppl6::CCompression Comp;
	if (!Comp.CompressZlib(a,size)) {
		free(a);
		return NULL;
	}
	ppl6::CBinary *bin=Comp.GetCompressedData();
	*/
	ppl6::CBinary *bin=new ppl6::CBinary;
	if (!bin) return NULL;
	bin->Set(a,size);
	bin->ManageMemory();
	delete v1;
	delete v2;
	return bin;
}

int DataOimp::Import(ppl6::CBinary *bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion werden binäre gespeicherte Daten in die Klasse importiert. Eine Beschreibung des
 * Binären Formats ist in der Funktion DataOimp::Export zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (!bin) {
		ppl6::SetError(194,"int DataOimp::Import(==> ppl6::CBinary *bin <==, int version)");
		return 0;
	}
	if (version<1 || version>1) {
		ppl6::SetError(20023,"%i",version);
		return 0;
	}
	int size=bin->Size();
	const char *a=(char*)bin->GetPtr();
	// Die Größe muss mindestens 9 Byte betragen
	if (size<4 || a==NULL) {
		ppl6::SetError(20024);
		return 0;
	}
	Clear();
	int p=4;
	Id=ppl6::Peek32(a);
	int len;
	// Dateiname
	len=ppl6::Peek16(a+p);
	if (len) Filename.Strncpy(a+p+2,len);
	p=p+2+len;
	len=ppl6::Peek16(a+p);
	if (len) {
		ID3v1.ImportBinary((char*)a+p+2,len);
		p+=len;
	}
	p+=2;
	len=ppl6::Peek16(a+p);
	if (len) {
		ID3v2.ImportBinary((char*)a+p+2,len);
	}
	/*
	printf ("Folgende Daten wurden eingelesen:\n");
	printf ("Filename: %s\n",(char*)Filename);
	printf ("ID3v1-Tag:\n");
	ID3v1.List("ID3v1");
	printf ("ID3v2-Tag:\n");
	ID3v2.List("ID3v2");
	*/

	return 1;
}

int DataOimp::ImportId(ppl6::CBinary *bin, int version)
/*!\brief Binäre Importfunktion
 *
 * Mit dieser Funktion wird nur die Id des Datensatzes aus den binäre gespeicherte Daten in die
 * Klasse importiert. Eine Beschreibung des binären Formats ist in der Funktion DataOimp::Export
 * zu finden.
 *
 * \param[in] bin Pointer auf eine ppl6::CBinary Klasse mit den zu importierenden Daten
 * \param[in] version Version des Datensatzformates
 * \returns Konnten die Daten erfolgreich importiert werden, wird 1 zurückgegeben, sonst 0.
 */
{
	if (!bin) {
		ppl6::SetError(194,"int DataOimp::Import(==> ppl6::CBinary *bin <==, int version)");
		return 0;
	}
	if (version<1 || version>1) {
		ppl6::SetError(20023,"%i",version);
		return 0;
	}
	int size=bin->Size();
	char *a=(char*)bin->GetPtr();
	// Die Größe muss mindestens 9 Byte betragen
	if (size<4 || a==NULL) {
		ppl6::SetError(20024);
		return 0;
	}
	Clear();
	Id=ppl6::Peek32(a);
	return 1;
}


/*!\class COimpDataStore
 * \ingroup WM3StorageClasses
 * \brief Klasse zum Verwalten von DataOimp Datensätzen
 *
 * Klasse zum Verwalten von DataOimp Datensätzen
 *
 */

/*!\var COimpDataStore::Mutex
 * \brief Mutex der Klasse
 *
 * Die Klasse verwendet einen Mutex bei allen Zugriffen und ist somit Threadsicher.
 */


/*!\var COimpDataStore::TableIndex
 * \brief Tabelle mit Pointern auf die eigentlichen Datensätze.
 *
 * Tabelle mit Pointern auf die eigentlichen Datensätze vom Typ DataOimp.
 * Diese Tabelle wird intern von der Klasse verwaltet und automatisch erweitert.
 * Jede ID entspricht einem Eintrag in der Tabelle. Ein NULL-Wert bedeutet, dass
 * es zu dieser ID noch keinen Datensatz gibt.
 */

/*!\var COimpDataStore::max
 * \brief Größe der internen Tabelle COimpDataStore::TableIndex
 *
 * Dieser Wert gibt an, wieviele Einträge die interne Pointer-Tabelle maximal enthalten kann.
 * Der Wert ist somit identisch mit der höchst möglichen ID. Soll ein Datensatz mit einer höheren
 * ID gespeichert werden, wird die Tabelle automatisch erweitert.
 */

/*!\var COimpDataStore::highestId
 * \brief Enthält die höchste bisher verwendete ID in der Tabelle.
 *
 * Enthält die höchste bisher verwendete ID in der Tabelle.
 */

COimpDataStore::COimpDataStore()
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

COimpDataStore::~COimpDataStore()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse. Stellt sicher, dass sämtlicher durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen wird. Dazu wird die Member-Funktion COimpDataStore::Clear
 * aufgerufen.
 */
{
	Clear();
}

void COimpDataStore::Clear()
/*!\brief Speicher freigeben
 *
 * Mit dieser Funktion kann der komplette durch diese Klasse und seiner Elemente
 * allokierter Speicher wieder freigegen werden.
 */
{
	for (ppluint32 i=0;i<max;i++) {
		if (TableIndex[i].t!=NULL) delete TableIndex[i].t;
	}
	free(TableIndex);
	TableIndex=NULL;
	max=0;
	highestId=0;
}

const char *COimpDataStore::GetChunkName()
/*!\brief Chunkname dieses Datentypes auslesen
 *
 * Diese Funktion liefert einen Pointer auf den Chunknamen dieses Datentypes zurück.
 *
 * \returns Pointer auf einen 4 Byte langen String mit 0-Byte terminiert.
 */
{
	return "OIMP";
}

int COimpDataStore::Increase(ppluint32 maxid)
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
	void *t=calloc(sizeof(OIMPDATA),h);
	if (!t) {
		ppl6::SetError(2,"int COimpDataStore::Increase(ppluint32 maxid)");
		return 0;
	}
	memcpy(t,TableIndex,max*sizeof(OIMPDATA));
	free(TableIndex);
	TableIndex=(OIMPDATA*)t;
	max=h;
	return 1;
}

int COimpDataStore::LoadChunk(CWMFileChunk *chunk)
{
	DataOimp data;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk->GetChunkData(),chunk->GetChunkDataSize())) return 0;
	if (!data.ImportId(&bin,chunk->GetFormatVersion())) return 0;
	data.CopyStorageFrom(chunk);
	/*
	printf ("Oimp %i importiert\n",data.Id);
	data.PrintStorageData();
	*/
	return Put(&data);
}

void COimpDataStore::SetHighestImportDataId(ppluint32 id)
{
	if (id>highestId) {
		if (Increase(id)) highestId=id;
	}
	//printf ("COimpDataStore: hoechste Id: %u\n",highestId);
}

int COimpDataStore::Save(DataOimp *t)
/*!\brief Datensatz auf Festplatte schreiben
 *
 * Diese Funktion speichert den übergebenen Datensatz auf die Festplatte. Dazu wird der Inhalt
 * zunächst als Binary exportiert (DataOimp::Export) und damit CStorage::Save aufgerufen.
 * Die Funktion wird intern von COimpDataStore::Put verwendet.
 *
 * \param[in] t Pointer auf einen DataOimp Datensatz
 * \returns Konnte der Datensatz erfolgreich gespeichert werden, liefert die Funktion 1 zurück, sonst
 * 0.
 */
{
	if (!Storage) {
		return 0;
	}
	if (!t) {
		ppl6::SetError(194,"int COimpDataStore::Save(==> DataOimp *t <==)");
		return 0;
	}
	if (Storage->isDatabaseLoading()) return 1;
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

int COimpDataStore::Put(DataOimp *entry)
/*!\brief Datensatz speichern
 *
 * Mit dieser Funktion wird ein veränderter oder neuer Datensatz im Speicher der Anwendung und
 * auf Festplatte gespeichert.
 *
 * \param[in] entry Pointer auf den gewünschten Datensatz vom Typ DataOimp. Dieser wird nicht
 * selbst gespeichert, sondern es wird eine Kopie erstellt. Der Wert muss daher von der Anwendung
 * nach Gebrauch wieder freigegeben werden. Ist der Wert DataOimp::Id == 0 wird automatisch eine
 * freie ID vergeben. Andernfalls wird der vorhandene Datensatz überschrieben.
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 */
{
	if (!entry) {
		ppl6::SetError(194,"int COimpDataStore::Put(==> DataOimp *entry <==)");
		return 0;
	}
	if (!Storage) {
		ppl6::SetError(20014,"COimpDataStore");
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
	//printf ("Speichere OIMP %i\n",id);
	// Gibt's den Eintrag schon?
	if (TableIndex[id].t) {
		// CopyFrom führt ein Clear aus, daher müss/home/patrick/atisysteminfo-report.txten wir die Storage Daten retten
		CStorageItem ssave;
		ssave.CopyStorageFrom(TableIndex[id].t);
		// Nun können wir die Daten kopieren
		if (!TableIndex[id].t->CopyFrom(entry)) {
			highestId=save_highestId;
			Mutex.Unlock();
			return 0;
		}
		// StorageDaten wieder herstellen
		TableIndex[id].t->CopyStorageFrom(&ssave);
		TableIndex[id].t->Id=id;
		if (!Save(TableIndex[id].t)) {
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
	TableIndex[id].t=new DataOimp;
	if (!TableIndex[id].t) {
		highestId=save_highestId;
		Mutex.Unlock();
		ppl6::SetError(2);
		return 0;
	}
	if (!TableIndex[id].t->CopyFrom(entry)) {
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
		ppl6::PushError();
		delete TableIndex[id].t;
		TableIndex[id].t=NULL;
		highestId=save_highestId;
		Mutex.Unlock();
		ppl6::PopError();
		return 0;
	}
	// Wir müssen die Storagedaten aus dem internen Datensatz kopieren
	entry->CopyStorageFrom(TableIndex[id].t);
	entry->Id=id;
	Mutex.Unlock();
	return 1;
}

int COimpDataStore::GetCopy(ppluint32 id, DataOimp *t)
/*!\brief Kopie eines Datensatzes erstellen
 *
 * Mit dieser Funktion wird ein vorhandener Datensatz ausgelesen. Im Unterschied zu den anderen
 * Datentypen gibt es in dieser Klasse keine Get-Funktion. Dies liegt daran, dass die Daten
 * nicht im Hauptspeicher gehalten werden, sondern nur auf der Festplatte. Im Speicher wird gibt
 * es nur die Information, wo der Datensatz auf der Festplatte zu finden ist. Die GetCopy-Funktion
 * holt sich diese Information und läd die eigentlichen Daten nach.
 *
 * \param[in] id ID des gewünschten Datensatzes
 * \param[in,out] t Pointer auf eine Klasse vom Typ DataOimp
 *
 * \returns Ist der Datensatz vorhanden und konnte kopiert werden, liefert die Funktion 1
 * zurück, sonst 0.
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
	DataOimp *entry=TableIndex[id].t;
	t->CopyFrom(entry);
	return Storage->LoadOimpRecord(t);
}


void COimpDataStore::List()
{
	DataOimp *t;
	printf ("Oimpdata-List START\n");
	if (TableIndex) {
		for (ppluint32 i=0;i<=highestId;i++) {
			t=TableIndex[i].t;
			if (t) {
				printf ("Datensatz %i: ",t->Id);
				t->PrintStorageData();
			}
		}
	}
	printf ("Oimpdata-List END\n");

}
