/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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


CCallback::~CCallback()
{

}

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

/*!\var CStorageBase::fileid
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
	lastchange=0;
	version=0;
	formatversion=0;
}

CStorageItem::CStorageItem(const CStorageItem& other)
/*!\brief Copy-Konstruktor der Klasse
 *
 * Copy-Konstruktor der Klasse
 */
{
	filepos=other.filepos;
	lastchange=other.lastchange;
	version=other.version;
	formatversion=other.formatversion;
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
	ClearStorageData();
}

void CStorageItem::ClearStorageData()
/*!\brief Setzt alle Variablen der Klasse auf 0
 *
 * Setzt alle Variablen der Klasse auf 0.
 */
{
	filepos=0;
	lastchange=0;
	version=0;
	formatversion=0;
}

void CStorageItem::CopyStorageFrom(const CStorageItem& other)
/*!\brief Storage Daten kopieren
 *
 * Diese Funktion übernimmt die Storage Daten des angegebenen Datensatzes.
 *
 * \param[in] item Pointer auf einen anderen Datensatz
 */
{
	filepos=other.filepos;
	lastchange=other.lastchange;
	version=other.version;
	formatversion=other.formatversion;
}

void CStorageItem::CopyStorageFrom(const CWMFileChunk& chunk)
/*!\brief Storage Daten kopieren
 *
 * Diese Funktion übernimmt die Storage Daten des angegebenen Chunks.
 *
 * \param[in] chunk Pointer auf einen WinMusik Chunk vom Typ CWMFileChunk
 */
{
	filepos=chunk.GetFilepos();
	lastchange=chunk.GetTimestamp();
	version=chunk.GetVersion();
	formatversion=chunk.GetFormatVersion();
}

uint32_t CStorageItem::GetVersion()
/*!\brief Version auslesen
 *
 * Mit dieser Funktion kann die Version des Datensatzes ausgelesen werden
 *
 * \returns Version des Datensatzes
 */
{
	return version;
}

uint32_t CStorageItem::GetLastChange()
/*!\brief Timestamp der letzten Änderung auslesen
 *
 * Mit dieser Funktion kann der Timestamp der letzten Änderung des Datensatzes ausgelesen werden
 *
 * \returns Timestamp
 */
{
	return lastchange;
}

uint8_t CStorageItem::GetFormatVersion()
/*!\brief Version des Datensatz-Formates auslesen
 *
 * Mit dieser Funktion kann die Version des Datensatz-Formates ausgelesen werden
 *
 * \returns Version des Formates
 */
{
	return formatversion;
}

void CStorageItem::PrintStorageData()
{
	printf("filepos: %u, lastchange: %u, version: %u, format: %u\n",
		filepos, lastchange, version, formatversion);
}

/*!\class CStorageType
 * \ingroup WM3StorageClasses
 * \brief Basisklasse für die verschiedenen Storage-Klassen
 *
 * Dies ist die Basisklasse für die verschiedenen Storage Klassen, die WinMusik verwendet.
 *
 */

CStorageBase::CStorageBase()
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor setz die Variablen CStorageType::type und CStorageType::Storage
 * auf NULL.
 */
{
	Storage=NULL;
}

CStorage& CStorageBase::getStorage() const
{
	if (!Storage) throw StorageClassNotInitialized();
	return *Storage;
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
}

CStorage::~CStorage()
{
	Clear();
}

void CStorage::Clear()
{
	Mutex.lock();
	ClearWithoutMutex();
	Mutex.unlock();
}

void CStorage::ClearWithoutMutex()
{
	CStorageBase* t;
	// Wir gehen alle Storage Klassen durch und übergeben ein Clear
	std::map<int, CStorageBase*>::const_iterator it;
	for (it=StorageClasses.begin();it != StorageClasses.end();++it) {
		it->second->Clear();
	}
}

void CStorage::Init(const ppl7::String& path)
{
	DataPath=path;
}

void CStorage::RegisterStorageClass(CStorageBase* storageclass)
{
	if (!storageclass) {
		throw InvalidStorageClass();
	}
	const char* chunkname=storageclass->GetChunkName();
	if (!chunkname) {
		throw InvalidStorageClass();
	}
	int chunkid=ppl7::Peek32(chunkname);
	if (!chunkname) {
		throw InvalidStorageClass();
	}
	Mutex.lock();
	storageclass->Storage=this;
	// Haben wir schon so einen StorageType?
	std::map<int, CStorageBase*>::iterator it;
	it=StorageClasses.find(chunkid);
	if (it != StorageClasses.end()) {
		it->second=storageclass;
	} else {
		StorageClasses.insert(std::pair<int, CStorageBase*>(chunkid, storageclass));
	}
	Mutex.unlock();
}

CStorageBase* CStorage::FindStorageClass(const char* name)
{
	if (!name) return NULL;
	int chunkid=ppl7::Peek32(name);
	std::map<int, CStorageBase*>::const_iterator it;
	Mutex.lock();
	it=StorageClasses.find(chunkid);
	if (it == StorageClasses.end()) {
		Mutex.unlock();
		return NULL;
	}
	Mutex.unlock();
	return it->second;
}

void CStorage::Save(CStorageBase* type, CStorageItem* item, const ppl7::ByteArrayPtr& bin)
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
	if (bLoadDatabaseRunning) return;
	ppl7::String File;
	if (DataPath.size() == 0) {
		throw StorageClassNotInitialized();
	}

	const char* t=type->GetChunkName();
	size_t size=bin.size();
	const char* ptr=(const char*)bin.ptr();
	CWMFile ff;
	CWMFileChunk chunk;
	//printf ("CStorage::Save, formatversion=%i, type: %s\n",item->formatversion,t);
	chunk.SetChunkData(t, ptr, size, item->filepos, item->version, item->formatversion);
	File=DataPath;
	if (strcmp(t, "OIMP") == 0) {
		File+="/wmoimp.dat";
	} else {
		File+="/winmusik.dat";
	}
	ff.Open(File);
	ff.SaveChunk(chunk);
	item->filepos=chunk.filepos;
	item->lastchange=chunk.timestamp;
	item->version=chunk.version;
}

void CStorage::Delete(CStorageBase* type, CStorageItem* item)
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
	if (bLoadDatabaseRunning) return;
	ppl7::String File;
	if (DataPath.size() == 0) {
		throw StorageClassNotInitialized();
	}
	const char* t=type->GetChunkName();
	CWMFile ff;
	CWMFileChunk chunk;
	chunk.SetChunkData(t, NULL, 0, item->filepos, item->version, item->formatversion);
	File=DataPath;
	if (strcmp(t, "OIMP") == 0) {
		File+="/wmoimp.dat";
	} else {
		File+="/winmusik.dat";
	}

	ff.Open(File);
	ff.DeleteChunk(chunk);
	item->filepos=0;
	item->lastchange=0;
	item->version=0;
}

void CStorage::LoadDatabase(CCallback* c)
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
	if (wmlog) wmlog->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Start loading Database");
	Clear();
	if (DataPath.size() == 0) {
		throw StorageClassNotInitialized();
	}
	ppl7::String File;
	CWMFile ff;
	CWMFileChunk chunk;
	File=DataPath + "/winmusik.dat";
	if (wmlog) wmlog->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Loading Database from File: %s", (const char*)File);
	ff.Open(File);
	uint32_t size=ff.GetFileSize();
	uint32_t pos=ff.GetFilePosition();
	Mutex.lock();
	try {
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
		if (wmlog) wmlog->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Loading %u Bytes", size);
		while (ff.GetNextChunk(chunk)) {
			if (c) {
				progress=ff.GetFilePosition() * 90 / size;
				if (c->progress != progress) {
					c->progress=progress;
					c->Update();
				}
			}
			int chunkid=ppl7::Peek32(chunk.chunkname);
			std::map<int, CStorageBase*>::const_iterator it=StorageClasses.find(chunkid);
			if (it != StorageClasses.end()) {
				CStorageBase* t=it->second;
				t->LoadChunk(chunk);

			}
			pos=ff.GetFilePosition();
		}
	} catch (const ppl7::Exception& exp) {
		bLoadDatabaseRunning=false;
		Mutex.unlock();
		if (wmlog) {
			wmlog->printf(ppl7::Logger::ERR, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Failed loading Chunk at Position: %u", pos);
			wmlog->printException(exp);
			chunk.HexDump(wmlog);
		}
		exp.print();
		throw;
	}
	bLoadDatabaseRunning=false;
	Mutex.unlock();
	/*
	uint32_t highest=0;
	CStorageBase* t=FindStorageType("TITL");
	if (t) highest=((CTitleStore*)t)->GetHighestImportDataId();
	*/
	if (wmlog) wmlog->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Done");
	if (c) {
		c->progress=100;
		c->Update();
	}
}

void CStorage::DeleteDatabase()
{
	ppl7::String File;
	if (DataPath.size() == 0) {
		throw StorageClassNotInitialized();
	}
	Mutex.lock();
	ClearWithoutMutex();
	try {
		File=DataPath + "/winmusik.dat";
		if (ppl7::File::exists(File)) {
			ppl7::File::remove(File);
		}
		File=DataPath + "/wmoimp.dat";
		if (ppl7::File::exists(File)) {
			ppl7::File::remove(File);
		}
	} catch (...) {
		Mutex.unlock();
		throw;
	}
	Mutex.unlock();
}
