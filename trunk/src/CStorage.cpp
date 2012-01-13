/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.1 $
 * $Date: 2010/05/16 12:23:30 $
 * $Id: CStorage.cpp,v 1.1 2010/05/16 12:23:30 pafe Exp $
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

/*!\class CCallback
 * \brief Callback-Klasse für Rückmeldung während des Datenbank-Loads
 *
 * Diese Klasse wird verwendet, um während des Ladens der Datenbank Feedback an das
 * Userinterface zu geben.
 */

void CCallback::Update()
/*!\brief Callback-Funktion
 *
 * Diese Funktion wird während des Ladevorgangs der Datenbank regelmäßig aufgerufen, um
 * über das Userinterface ein Feedback über den Fortschritt geben zu können. Sie ist als
 * virtuell impelentiert und muss von der abgeleiteten Klasse reimplementiert werden.
 */
{

}

/*!\class CStorageItem
 * \ingroup WM3DataTypes
 * \brief Basisklasse für alle Datenelemente, die in der WinMusik-Datenbank gespeichert werden müssen
 *
 * Dies ist die Basisklasse für alle Datenelemente, die in der WinMusik-Datenbank gespeichert werden müssen.
 * Sie verwaltet die ID der Datenbank-Datei, die Position des Datensatzes in der Datei, Timestamp der letzten
 * Änderung, sowie die Version.
 */

/*!\var CStorageItem::fileid
 * \brief ID der Datenbank-Datei
 */

/*!\var CStorageItem::filepos
 * \brief Position des Datensatzes innerhalb der Datenbank-Datei
 */

/*!\var CStorageItem::lastchange
 * \brief Timestamp der letzten Änderung
 */

/*!\var CStorageItem::version
 * \brief Versionsnummer des Datensatzen
 *
 * Diese Variable enthält die Versionsnummer des Datensatzen. Bei jedem Speichern wird sie um 1 hochgezählt, um bei
 * Nutzung einer Netzwerk-Datenbank feststellen zu können, ob der Datensatz zwischenzeitlich parallel durch einen
 * anderen Client verändert wurde.
 */

/*!\var CStorageItem::formatversion
 * \brief Version des Datensatz-Formates
 *
 * Diese Variable enthält die Version des Datensatz-Formates. Die Version ändert sich nur, wenn
 * am Format des Datensatzes etwas verändert wird. Sie wird dann jeweils um 1 hochgezählt.
 * Die Export-Funktionen der Datentypen speichern jeweils immer im Format der aktuellsten
 * Version, während die Import-Funktionen in der Lage sind auch alle älteren Formate richtig
 * einzulesen. Dies hat den Vorteil, dass bei einer Änderung am Format eines Datentyps die
 * Datenbank nicht aktualisiert werden muss.
 */

CStorageItem::CStorageItem()
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor der Klasse initialisiert alle Variablen mit 0.
 */
{
	filepos=0;
	fileid=0;
	lastchange=0;
	version=0;
	formatversion=0;
}

CStorageItem::~CStorageItem()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse
 */
{
}

void CStorageItem::Clear()
/*!\brief Setzt alle Variablen der Klasse auf 0
 *
 * Setzt alle Variablen der Klasse auf 0. Die Clear-Funktion einer abgeleiteten Klasse muss
 * sicherstellen, dass am Ende diese Funktion ebenfalls aufgerufen wird.
 */
{
	filepos=0;
	fileid=0;
	lastchange=0;
	version=0;
	formatversion=0;
}

void CStorageItem::CopyStorageFrom(CStorageItem *item)
/*!\brief Storage Daten kopieren
 *
 * Diese Funktion übernimmt die Storage Daten des angegebenen Datensatzes.
 *
 * \param[in] item Pointer auf einen anderen Datensatz
 */
{
	if (!item) return;
	filepos=item->filepos;
	fileid=item->fileid;
	lastchange=item->lastchange;
	version=item->version;
	formatversion=item->formatversion;
}

void CStorageItem::CopyStorageFrom(CWMFileChunk *chunk)
/*!\brief Storage Daten kopieren
 *
 * Diese Funktion übernimmt die Storage Daten des angegebenen Chunks.
 *
 * \param[in] chunk Pointer auf einen WinMusik Chunk vom Typ CWMFileChunk
 */
{
	if (!chunk) return;
	filepos=chunk->GetFilepos();
	fileid=1;
	lastchange=chunk->GetTimestamp();
	version=chunk->GetVersion();
	formatversion=chunk->GetFormatVersion();
}

ppluint32 CStorageItem::GetVersion()
/*!\brief Version auslesen
 *
 * Mit dieser Funktion kann die Version des Datensatzes ausgelesen werden
 *
 * \returns Version des Datensatzes
 */
{
	return version;
}

ppluint32 CStorageItem::GetLastChange()
/*!\brief Timestamp der letzten Änderung auslesen
 *
 * Mit dieser Funktion kann der Timestamp der letzten Änderung des Datensatzes ausgelesen werden
 *
 * \returns Timestamp
 */
{
	return lastchange;
}

ppluint8 CStorageItem::GetFormatVersion()
/*!\brief Version des Datensatz-Formates auslesen
 *
 * Mit dieser Funktion kann die Version des Datensatz-Formates ausgelesen werden
 *
 * \returns Version des Formates
 */
{
	return version;
}

void CStorageItem::PrintStorageData()
{
	printf ("filepos: %u, fileid: %u, lastchange: %u, version: %u, format: %u\n",
			filepos,fileid,lastchange,version,formatversion);
}

/*!\class CStorageType
 * \ingroup WM3StorageClasses
 * \brief Basisklasse für die verschiedenen Storage-Klassen
 *
 * Dies ist die Basisklasse für die verschiedenen Storage Klassen, die WinMusik verwendet.
 *
 */

CStorageType::CStorageType()
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor setz die Variablen CStorageType::type und CStorageType::Storage
 * auf NULL.
 */
{
	type=NULL;
	Storage=NULL;
}

void CStorageType::Clear()
/*!\brief Speicher freigeben
 *
 * Bei Aufruf dieser Funktion muss die Klasse sämtlichen Speicher freigeben, der durch
 * die verwalteten Datensätze allokiert wurde. Die Funktion ist rein virtuell und muss
 * daher von einer abgeleiteten Klasse reimplementiert werden.
 */
{

}

const char *CStorageType::GetChunkName()
/*!\brief Liefert den Chunknamen dieser Storage Klasse zurück
 *
 * Diese Funktion ist als virtuell deklariert und liefert den Namen des Chunks
 * dieser Klasse zurück. Sie muss in abgeleiteten Klassen reimplementiert werden.
 *
 * \returns Liefert einen Pointer auf den Chunknamen dieser Storage Klasse zurück.
 * Dieser ist immer 4 Byte lang und mit 0 terminiert.
 */
{
	return "INVL";
}

int CStorageType::CompareNode(CTreeItem *item)
{
	CStorageType *s=(CStorageType*)item;
	int cmp=strcmp(s->type,type);
	if (cmp==0) return 0;
	if (cmp>0) return 1;
	return -1;
}

int CStorageType::CompareValue(void *value)
{
	char *s=(char*)value;
	int cmp=strcmp(s,type);
	if (cmp==0) return 0;
	if (cmp>0) return 1;
	return -1;
}

int CStorageType::LoadChunk(CWMFileChunk *chunk)
/*!\brief Datensatz laden
 *
 * Mit dieser Funktion wird ein Chunk aus der Datenbank in den Speicher geladen.
 * Sie ist als virtuell definiert und muss von der ableitenden Klasse reimplementiert
 * werden.
 *
 * \param[in] chunk Pointer auf einen WinMusik Chunk vom Typ CWMFileChunk
 * \returns Konnte der Chunk erfolgreich importiert werden, muss die Funktion 1 zurückgeben, sonst 0.
 */
{
	return 1;
}

/*!\class CStorage
 * \ingroup WM3StorageClasses
 * \brief Speicherklasse für alle WinMusik Daten
 *
 * Dies ist die Hauptklasse zum Speichern aller WinMusik Datentypen auf der Festplatte.
 * Alle verfügbaren Datenklassen müssen bei Programmstart mit der Funktion
 * CStorage::RegisterStorageType registriert werden. Ferner muss der Datenpfad
 * von WinMusik über die Funktion CStorage::Init übergeben werden.
 *
 * Die verschiedenen Storageklassen rufen später die Save-Funktion dieser Klasse auf, um
 * Daten auf die Festplatte zu schreiben.
 */

CStorage::CStorage()
{
	bLoadDatabaseRunning=false;
	bKeepFileOpen=false;
}

CStorage::~CStorage()
{
	Clear();
}

int CStorage::StartImport()
{
	bKeepFileOpen=true;
	StorageFile.Close();
}

int CStorage::FinishImport()
{
	bKeepFileOpen=false;
	StorageFile.Close();
}


void CStorage::Clear()
{
	Mutex.Lock();
	ClearWithoutMutex();
	Mutex.Unlock();
}

void CStorage::ClearWithoutMutex()
{
	CStorageType *t;
	// Wir gehen alle Storage Klassen durch und übergeben ein Clear
	StorageClasses.Reset();
	while ((t=(CStorageType*)StorageClasses.GetNext())) {
		t->Clear();
	}
}

int CStorage::Init(const char *path)
{
	DataPath=path;
	return 1;
}

int CStorage::RegisterStorageType(CStorageType *type)
{
	if (!type) {
		return 0;
	}
	const char *t=type->GetChunkName();
	if (!t) {
		return 0;
	}
	Mutex.Lock();
	// Haben wir schon so einen StorageType?
	if (StorageClasses.Find((void*)&t)) {
		Mutex.Unlock();
		return 0;
	}
	type->type=t;
	if (!StorageClasses.Add(type)) {
		type->type=0;
		Mutex.Unlock();
		return 0;
	}
	type->Storage=this;
	Mutex.Unlock();
	return 1;
}

CStorageType *CStorage::FindStorageType(const char *name)
{
	CStorageType *t=NULL;
	if (!name) return NULL;
	Mutex.Lock();
	t=(CStorageType *)StorageClasses.Find((void*)name);
	Mutex.Unlock();
	return t;
}

int CStorage::Save(CStorageType *type, CStorageItem *item, ppl6::CBinary *bin)
/*!\brief Datensatz speichern
 *
 * Mit dieser Funktion wird ein von CStorageType abgeleiteter Datensatz auf der Festplatte
 * gespeichert.
 *
 * Es wird dabei unterschieden, ob es sich um den StorageType OIMP handelt oder nicht. Beim Typ OIMP
 * wird nicht in der eigentlichen Datenbank gespeichert, sondern in einer zweiten Datei namens
 * wmoimp.dat.
 *
 * \param[in] type Pointer auf die Storage-Klasse, die den Datensatz speichern möchte
 * \param[in,out] item Pointer auf den zu speichernden Datensatz
 * \param[in] bin Pointer auf die zu speichernden Daten
 * \returns Bei Erfolg liefert die Funktion 1 zurück und aktualisiert die Storage-Daten in \p item.
 * Bei einem Fehler wird 0 zurückgegeben.
 */
{
	// Falls ein Datenbank Load im Gange ist, brauchen wir nichts zu machen
	if (bLoadDatabaseRunning) return 1;
	ppl6::CString File;
	if (DataPath.Len()==0) {
		ppl6::SetError(20014);
		return 0;
	}

	const char *t=type->type;
	int size=bin->GetSize();
	const char *ptr=(const char *)bin->GetPtr();
	CWMFile ff;
	CWMFileChunk chunk;
	chunk.SetChunkData(t,ptr,size,item->filepos,item->version, item->formatversion);
	File=DataPath;
	if (strcmp(t,"OIMP")==0) {
		File.Concatf("/wmoimp.dat");
	} else {
		File.Concatf("/winmusik.dat");
	}
	if (bKeepFileOpen) {
		if (!StorageFile.IsOpen()) {
			if (!StorageFile.Open(File)) return 0;
		}
		if (!StorageFile.SaveChunk(&chunk)) return 0;
	} else {
		if (!ff.Open(File)) return 0;
		if (!ff.SaveChunk(&chunk)) return 0;
	}
	item->fileid=1;
	item->filepos=chunk.filepos;
	item->lastchange=chunk.timestamp;
	item->version=chunk.version;
	return 1;
}

int CStorage::Delete(CStorageType *type, CStorageItem *item)
/*!\brief Datensatz löschen
 *
 * Mit dieser Funktion wird ein von CStorageType abgeleiteter Datensatz auf der Festplatte
 * gelöscht.
 *
 * Es wird dabei unterschieden, ob es sich um den StorageType OIMP handelt oder nicht. Beim Typ OIMP
 * wird nicht in der eigentlichen Datenbank gespeichert, sondern in einer zweiten Datei namens
 * wmoimp.dat.
 *
 * \param[in] type Pointer auf die Storage-Klasse, die den Datensatz löschen möchte
 * \param[in,out] item Pointer auf den zu löschenden Datensatz
 * \returns Bei Erfolg liefert die Funktion 1 zurück und aktualisiert die Storage-Daten in \p item.
 * Bei einem Fehler wird 0 zurückgegeben.
 */
{
	// Falls ein Datenbank Load im Gange ist, brauchen wir nichts zu machen
	if (bLoadDatabaseRunning) return 1;
	ppl6::CString File;
	if (DataPath.Len()==0) {
		ppl6::SetError(20014);
		return 0;
	}

	const char *t=type->type;
	CWMFile ff;
	CWMFileChunk chunk;
	chunk.SetChunkData(t,NULL,0,item->filepos,item->version, item->formatversion);
	File=DataPath;
	if (strcmp(t,"OIMP")==0) {
		File.Concatf("/wmoimp.dat");
	} else {
		File.Concatf("/winmusik.dat");
	}

	if (!ff.Open(File)) return 0;
	if (!ff.DeleteChunk(&chunk)) return 0;
	item->fileid=0;
	item->filepos=0;
	item->lastchange=0;
	item->version=0;
	return 1;
}

int CStorage::LoadDatabase(CCallback *c)
/*!\brief Datenbank laden
 *
 * Mit dieser Funktion wird die Datenbank aus dem Filesystem in den Hauptspeicher geladen.
 * Optional kann eine Callback-Klasse angegeben werden, die während des Ladevorgangs
 * regelmäßig aufgerufen wird und den Prozentsatz des Fortschritts enthält.
 *
 * \param[in] c Optionaler Pointer auf eine Callback-Klasse
 *
 * \returns Konnte die Datenbank erfolgreich geladen werden, gibt die Funktion 1 zurück,
 * sonst 0.
 *
 * \note Vor dem laden der Datenbank wird eine möglicherweise schon im Speicher vorhandene
 * Datenbank durch Aufruf von CStorage::Clear gelöscht.
 */
{
	// We need to clear the database first
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,2,"CStorage","LoadDatabase",__FILE__,__LINE__,"Start loading Database");
	Clear();

	if (DataPath.Len()==0) {
		ppl6::SetError(20014);
		return 0;
	}
	ppl6::CString File;
	CWMFile ff;
	CWMFileChunk chunk;
	File=DataPath;
	File.Concatf("/winmusik.dat");
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,2,"CStorage","LoadDatabase",__FILE__,__LINE__,"Loading Database from File: %s",(const char*)File);

	if (!ff.Open(File)) return 0;
	Mutex.Lock();
	// Wir müssen verhindern, dass die StorageTypen versuchen den Datensatz beim Import gleich
	// wieder zu speichern
	bLoadDatabaseRunning=true;
	int progress=0;
	if (c) {
		c->progress=0;
		c->Update();
	}
	// **************************************************************************************
	// Datenbank
	ppluint32 size=ff.GetFileSize();
	CStorageType *t=NULL;
	ppluint32 pos=ff.GetFilePosition();
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,2,"CStorage","LoadDatabase",__FILE__,__LINE__,"Loading %u Bytes",size);
	while (ff.GetNextChunk(&chunk)) {
		progress=ff.GetFilePosition()*90/size;
		if (c) {
			if (c->progress!=progress) {
				c->progress=progress;
				c->Update();
			}
		}
		t=(CStorageType *)StorageClasses.Find((void*)chunk.chunkname);
		if (t) {
			if (!t->LoadChunk(&chunk)) {
				if (wmlog) {
					wmlog->Printf(ppl6::LOG::ERROR,2,"CStorage","LoadDatabase",__FILE__,__LINE__,"Failed loading Chunk at Position: %u",pos);
					wmlog->LogError();
					ppl6::PushError();
					chunk.HexDump(wmlog);
					ppl6::PopError();
				}
				ppl6::PrintError();
				bLoadDatabaseRunning=false;
				Mutex.Unlock();
				return 0;
			}
		}
		pos=ff.GetFilePosition();
	}
	ppluint32 highest=0;
	t=(CStorageType *)StorageClasses.Find((void*)"TITL");
	if (t) highest=((CTitleStore*)t)->GetHighestImportDataId();
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,2,"CStorage","LoadDatabase",__FILE__,__LINE__,"Done");

	// **************************************************************************************
	// MP3 Import Daten
	t=(CStorageType *)StorageClasses.Find((void*)"OIMP");
	if (t) {
		File=DataPath;
		File.Concatf("/wmoimp.dat");
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,2,"CStorage","LoadDatabase",__FILE__,__LINE__,"Loading ID3Data from File: %s",(const char*)File);
		if (ff.Open(File)) {
			size=ff.GetFileSize();


			while (ff.GetNextChunk(&chunk)) {
				progress=90+(ff.GetFilePosition()*10/size);
				if (c) {
					if (c->progress!=progress) {
						c->progress=progress;
						c->Update();
					}
				}
				if (!t->LoadChunk(&chunk)) {
					if (wmlog) {
						wmlog->Printf(ppl6::LOG::ERROR,2,"CStorage","LoadDatabase",__FILE__,__LINE__,"Failed loading Chunk");
						wmlog->LogError();
					}
					ppl6::PrintError();
					bLoadDatabaseRunning=false;
					Mutex.Unlock();
					return 0;
				}
			}
		}
		((COimpDataStore*)t)->SetHighestImportDataId(highest);
	}




	bLoadDatabaseRunning=false;
	Mutex.Unlock();
	if (c) {
		c->progress=100;
		c->Update();
		ppl6::MSleep(200);
	}
	return 1;
}

int CStorage::DeleteDatabase()
{
	ppl6::CString File;
	if (DataPath.Len()==0) {
		ppl6::SetError(20014);
		return 0;
	}
	Mutex.Lock();
	ClearWithoutMutex();
	File=DataPath;
	File.Concatf("/winmusik.dat");
	if (ppl6::FileExists(File)) {
		ppl6::CFile::DeleteFile(File);
	}
	File.Concatf("/wmoimp.dat");
	if (ppl6::FileExists(File)) {
		ppl6::CFile::DeleteFile(File);
	}
	Mutex.Unlock();
	return 1;
}

int CStorage::LoadOimpRecord(DataOimp *item)
{
	if (!item) {
		ppl6::SetError(194);
		return 0;
	}
	if (item->fileid==0 || item->filepos==0) {
		ppl6::SetError(20025);
		return 0;
	}
	ppl6::CString File;
	CWMFile ff;
	CWMFileChunk chunk;
	File=DataPath;
	File.Concatf("/wmoimp.dat");
	if (!ff.Open(File)) {
		ppl6::SetError(20025);
		return 0;
	}
	if (!ff.GetChunk(&chunk,item->filepos)) return 0;
	ppl6::CBinary bin;
	if (!bin.Set((void*)chunk.GetChunkData(),chunk.GetChunkDataSize())) return 0;
	if (!item->Import(&bin,chunk.GetFormatVersion())) return 0;
	item->CopyStorageFrom(&chunk);
	return 1;
}


/*
int CStorage::CreateDatabase()
{
	ppl6::CString File;
	CStorageType *type;
	CWMFile ff;
	CWMFileChunk chunk;
	if (DataPath.Len()==0) {
		ppl6::SetError(20014);
		return 0;
	}

	Mutex.Lock();
	ClearWithoutMutex();
	File=DataPath;
	File.Concatf("/data%04i.wm3",1);
	if (ppl6::FileExists(File)) {
		ppl6::DeleteFile(File);
	}
	if (!ff.Open(File)) {
		Mutex.Unlock();
		return 0;
	}
	ff.Close();
	Mutex.Unlock();
	// We now have a fresh but empty database. So lets put something into it...
	type=FindStorageType("VERS");
	if (type) {
		CVersionStore *vs=(CVersionStore *)type;
		DataVersion v;
		v.SetValue("Single");
		vs->Put(&v);
		v.Clear();
		v.SetValue("Maxi");
		vs->Put(&v);
		v.SetValue("Album");
		vs->Put(&v);
		v.Clear();

	}
	//Save(&v);



	return 1;

}
*/
