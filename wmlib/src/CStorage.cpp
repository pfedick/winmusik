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

#include "wm_cwmfile.h"
#include "wm_storage.h"
#include "wm_exceptions.h"
#include "libwinmusik3.h"

namespace de {
namespace pfp {
namespace winmusik {


/*!\class CStorageBase
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
	if (!Storage) throw StorageClassNotInitializedException();
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
	logger=NULL;
	DatabaseFile="winmusik.dat";
}

CStorage::~CStorage()
{
	Clear();
}

void CStorage::SetLogger(ppl7::Logger* logger)
{
	this->logger=logger;
}

void CStorage::Clear()
{
	Mutex.lock();
	ClearWithoutMutex();
	Mutex.unlock();
}

void CStorage::ClearWithoutMutex()
{
	// Wir gehen alle Storage Klassen durch und übergeben ein Clear
	std::map<uint32_t, CStorageBase*>::const_iterator it;
	for (it=StorageClasses.begin();it != StorageClasses.end();++it) {
		it->second->Clear();
	}
}

void CStorage::Init(const ppl7::String& path, const ppl7::String& file)
{
	DataPath=path;
	DatabaseFile=file;
}

void CStorage::RegisterStorageClass(CStorageBase* storageclass)
{
	if (!storageclass) {
		throw InvalidStorageClassException();
	}
	const char* chunkname=storageclass->GetChunkName();
	if (!chunkname) {
		throw InvalidStorageClassException();
	}
	uint32_t chunkid=ppl7::Peek32(chunkname);
	if (!chunkname) {
		throw InvalidStorageClassException();
	}
	Mutex.lock();
	storageclass->Storage=this;
	// Haben wir schon so einen StorageType?
	std::map<uint32_t, CStorageBase*>::iterator it;
	it=StorageClasses.find(chunkid);
	if (it != StorageClasses.end()) {
		it->second=storageclass;
	} else {
		StorageClasses.insert(std::pair<uint32_t, CStorageBase*>(chunkid, storageclass));
	}
	Mutex.unlock();
}

CStorageBase* CStorage::FindStorageClass(const char* name)
{
	if (!name) return NULL;
	uint32_t chunkid=ppl7::Peek32(name);
	std::map<uint32_t, CStorageBase*>::const_iterator it;
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
		throw StorageClassNotInitializedException();
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
		File+="/" + DatabaseFile;
	}
	ff.Open(File);
	ff.SaveChunk(chunk);
	item->filepos=chunk.GetFilepos();
	item->lastchange=chunk.GetTimestamp();
	item->version=chunk.GetVersion();
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
		throw StorageClassNotInitializedException();
	}
	const char* t=type->GetChunkName();
	CWMFile ff;
	CWMFileChunk chunk;
	chunk.SetChunkData(t, NULL, 0, item->filepos, item->version, item->formatversion);
	File=DataPath;
	if (strcmp(t, "OIMP") == 0) {
		File+="/wmoimp.dat";
	} else {
		File+="/" + DatabaseFile;
	}

	ff.Open(File);
	ff.DeleteChunk(chunk);
	item->filepos=0;
	item->lastchange=0;
	item->version=0;
}

void CStorage::LoadDatabase(CProgressUpdate* progressCallback)
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
	if (logger) logger->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Start loading Database");
	Clear();
	if (DataPath.size() == 0) {
		throw StorageClassNotInitializedException();
	}
	ppl7::String File;
	CWMFile ff;
	CWMFileChunk chunk;
	File=DataPath + "/" + DatabaseFile;
	if (logger) logger->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Loading Database from File: %s", (const char*)File);
	ff.Open(File);
	uint32_t size=ff.GetFileSize();
	uint32_t pos=ff.GetFilePosition();
	Mutex.lock();
	try {
		// Wir müssen verhindern, dass die StorageTypen versuchen den Datensatz beim Import gleich
		// wieder zu speichern
		bLoadDatabaseRunning=true;
		int progress=0;
		if (progressCallback) progressCallback->ProgressUpdate(progress);
		// **************************************************************************************
		// Datenbank
		if (logger) logger->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Loading %u Bytes", size);
		while (ff.GetNextChunk(chunk)) {
			if (progressCallback) {
				int newprogress=ff.GetFilePosition() * 90 / size;
				if (newprogress != progress) {
					progress=newprogress;
					progressCallback->ProgressUpdate(progress);
				}
			}
			uint32_t chunkid=ppl7::Peek32(chunk.GetChunkName());
			std::map<uint32_t, CStorageBase*>::const_iterator it=StorageClasses.find(chunkid);
			if (it != StorageClasses.end()) {
				CStorageBase* t=it->second;
				t->LoadChunk(chunk);

			}
			pos=ff.GetFilePosition();
		}
	} catch (const ppl7::Exception& exp) {
		bLoadDatabaseRunning=false;
		Mutex.unlock();
		if (logger) {
			logger->printf(ppl7::Logger::ERR, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Failed loading Chunk at Position: %u", pos);
			logger->printException(exp);
			chunk.HexDump(logger);
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
	if (logger) logger->printf(ppl7::Logger::DEBUG, 2, "CStorage", "LoadDatabase", __FILE__, __LINE__, "Done");
	if (progressCallback) progressCallback->ProgressUpdate(100);
}

void CStorage::DeleteDatabase()
{
	ppl7::String File;
	if (DataPath.size() == 0) {
		throw StorageClassNotInitializedException();
	}
	Mutex.lock();
	ClearWithoutMutex();
	try {
		File=DataPath + "/" + DatabaseFile;
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


}
}
}	// EOF Namespace de.pfp.winmusik
