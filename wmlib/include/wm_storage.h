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

#ifndef WM_STORAGE_H
#define WM_STORAGE_H

#include <ppl7.h>
#include "wm_cwmfile.h"
#include "wm_tracklist.h"
#include "wm_normalizer.h"

namespace de {
namespace pfp {
namespace winmusik {

class CStorage;
class CProgressUpdate;
class CTrackStore;
class CTrackList;

class CStorageItem
{
	friend class CStorage;
private:
	uint32_t filepos;
	uint32_t lastchange;
	uint32_t version;
protected:
	uint8_t  formatversion;

public:
	CStorageItem();
	CStorageItem(const CStorageItem& other);
	~CStorageItem();
	void Clear();
	void ClearStorageData();
	void CopyStorageFrom(const CStorageItem& other);
	void CopyStorageFrom(const CWMFileChunk& chunk);
	uint32_t GetVersion() const;
	uint32_t GetLastChange() const;
	uint32_t GetFilePos() const;
	uint8_t GetFormatVersion() const;
	void PrintStorageData() const;
};


class CStorageBase
{
	friend class CStorage;
	CStorage* Storage;
public:
	CStorageBase();
	CStorage& getStorage() const;
	virtual void Clear()=0;
	virtual const char* GetChunkName()=0;
	virtual void LoadChunk(const CWMFileChunk& chunk)=0;
};


class CStorage
{
private:
	ppl7::Mutex Mutex;
	ppl7::String DataPath;
	ppl7::String DatabaseFile;
	std::map<uint32_t, CStorageBase*> StorageClasses;
	bool	bLoadDatabaseRunning;
	CWMFile	StorageFile;
	ppl7::Logger* logger;

	void ClearWithoutMutex();

public:
	CStorage();
	virtual ~CStorage();
	void Clear();
	void SetLogger(ppl7::Logger* logger);
	void Init(const ppl7::String& path, const ppl7::String& file=ppl7::String("winmusik.dat"));
	void RegisterStorageClass(CStorageBase* storageclass);
	CStorageBase* FindStorageClass(const char* name);
	void Save(CStorageBase* type, CStorageItem* item, const ppl7::ByteArrayPtr& bin);
	void Delete(CStorageBase* type, CStorageItem* item);

	void LoadDatabase(CProgressUpdate* progressCallback=NULL);
	void DeleteDatabase();
	bool isDatabaseLoading() { return bLoadDatabaseRunning; }

	//int CreateDatabase();
};


/*******************************************************
 * TITLES                                              *
 *******************************************************/


class DataTitle : public CStorageItem
{
public:
	ppl7::String	Artist;
	ppl7::String	Title;
	ppl7::String	Remarks;
	ppl7::String	Album;
	ppl7::String	Tags;
	ppl7::ByteArray	CoverPreview;
	uint32_t		TitleId;
	uint32_t		DeviceId;
	uint32_t		Length;
	uint32_t		VersionId;
	uint32_t		LabelId;
	uint32_t		BPM;
	uint32_t		RecordDate;
	uint32_t		ReleaseDate;
	uint32_t		ImportData;
	uint32_t		Size;
	uint16_t		RecordSourceId;
	uint16_t		Track;
	uint16_t		Bitrate;
	uint16_t		GenreId;
	uint16_t		RecordDeviceId;
	uint8_t		DeviceType;
	uint8_t		Page;
	uint8_t		Channels;
	uint8_t		Quality;
	uint8_t		Rating;
	uint8_t		Flags;
	uint8_t		Key;
	uint8_t		EnergyLevel;

	DataTitle();
	DataTitle(const DataTitle& other);
	~DataTitle();
	void Clear();

	void CopyFrom(const DataTitle& other);
	void CopyDataFrom(const DataTitle& other);
	void SetTitle(const ppl7::String& title);
	void SetArtist(const ppl7::String& artist);
	void SetRemarks(const ppl7::String& remarks);
	void SetTags(const ppl7::String& tags);
	void SetAlbum(const ppl7::String& album);

	void Export(ppl7::ByteArray& bin);
	void Import(const ppl7::ByteArrayPtr& bin, int version);

	DataTitle& operator=(const DataTitle& other);
	bool operator==(const DataTitle& other) const;
	bool operator!=(const DataTitle& other) const;
};



class CTitleStore : public CStorageBase
{
private:
	DataTitle** TitleIndex;
	uint32_t max;
	uint32_t highestId;
	uint32_t size;

	void Increase(uint32_t maxid);
	void SaveToStorage(DataTitle& t);
	DataTitle* SaveToMemory(const DataTitle& t);
	void AddArtist(const ppl7::String& artist);
	void RemoveArtist(const ppl7::String& artist);

public:
	CTitleStore();
	virtual ~CTitleStore();
	virtual const char* GetChunkName();
	virtual void Clear();
	virtual void LoadChunk(const CWMFileChunk& chunk);

	std::map<ppl7::String, uint32_t> Artists;

	uint32_t Put(const DataTitle& title);
	bool Exists(uint32_t id) const;
	void Delete(uint32_t id);
	const DataTitle& Get(uint32_t id) const;
	const DataTitle* GetPtr(uint32_t id) const;
	uint32_t MaxId() const;
	uint32_t Capacity() const;
	uint32_t Size() const;
};



/*******************************************************
 * DEVICES                                             *
 *******************************************************/

class DataDevice : public CStorageItem
{
private:
public:
	ppl7::String	Title;
	ppl7::String	SubTitle;
	uint32_t		DeviceId;
	uint32_t		Length;
	uint32_t		Recorded;
	uint32_t		LabelId;
	uint32_t		PurchaseDate;
	uint32_t		DateCreated;
	uint32_t		FirstDate;
	uint32_t		LastDate;
	uint32_t		NumTracks;
	float			PurchasePrice;
	uint16_t		PurchaseId;
	uint8_t		DeviceType;
	uint8_t		Pages;

	DataDevice();
	virtual ~DataDevice();
	void Clear();

	void CopyFrom(const DataDevice& other);
	void CopyDataFrom(const DataDevice& other);
	void SetTitle(const ppl7::String& title);
	void SetSubTitle(const ppl7::String& subtitle);
	const ppl7::String& GetTitle() const;
	const ppl7::String& GetSubTitle() const;

	void Export(ppl7::ByteArray& bin) const;
	void Import(const ppl7::ByteArrayPtr& bin, int version);
};

class CDeviceStore : public CStorageBase
{
private:
	typedef std::map<uint32_t, DataDevice> DeviceTree;
	std::map<uint8_t, DeviceTree>Tree;

	void SaveToStorage(DataDevice& t);
	DataDevice* SaveToMemory(const DataDevice& t);

public:
	CDeviceStore();
	virtual ~CDeviceStore();
	virtual const char* GetChunkName();
	virtual void Clear();
	virtual void LoadChunk(const CWMFileChunk& chunk);
	void Renumber(uint8_t DeviceType, uint32_t oldId, uint32_t newId);
	uint32_t GetHighestDevice(uint8_t DeviceType) const;
	void Update(uint8_t DeviceType, uint32_t DeviceId);

	const DataDevice& Put(const DataDevice& entry);
	bool Exists(uint8_t DeviceType, uint32_t DeviceId) const;
	void Delete(uint8_t DeviceType, uint32_t DeviceId);
	const DataDevice& Get(uint8_t DeviceType, uint32_t DeviceId) const;
	const DataDevice* GetPtr(uint8_t DeviceType, uint32_t DeviceId) const;

};

/*******************************************************
 * TRACKS                                              *
 *******************************************************/


class DataTrack : public CStorageItem
{
public:
	uint32_t	DeviceId;
	uint32_t	TitleId;
	uint16_t	Track;
	uint8_t		Device;
	uint8_t		Page;

	DataTrack();
	DataTrack(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track, uint32_t TitleId);

	void Clear();
	void CopyFrom(const DataTrack& other);
	void CopyDataFrom(const DataTrack& other);
	void SetValue(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track, uint32_t TitleId);
	void Export(ppl7::ByteArray& bin) const;
	void Import(const ppl7::ByteArrayPtr& bin, int version);
};


class CTrackStore : public CStorageBase
{
private:
	typedef std::map<uint16_t, DataTrack> DataTrackMap;
	std::map<uint64_t, DataTrackMap> Tracks;
	void SaveToStorage(DataTrack& t);
	DataTrack* SaveToMemory(const DataTrack& t);
	DataTrack* Find(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const;


public:
	CTrackStore();
	virtual ~CTrackStore();
	virtual const char* GetChunkName();
	virtual void Clear();
	virtual void LoadChunk(const CWMFileChunk& chunk);
	const  DataTrack& Put(const DataTrack& entry);
	bool Exists(uint8_t Device, uint32_t DeviceId, uint8_t Page) const;
	bool Exists(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const;
	void Delete(const DataTrack& entry);
	void Delete(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track);

	const DataTrack& Get(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const;
	const DataTrack* GetPtr(uint8_t Device, uint32_t DeviceId, uint8_t Page, uint16_t Track) const;
	CTrackList GetTracklist(uint8_t Device, uint32_t DeviceId, uint8_t Page) const;
};


/*******************************************************
 * SHORTCUTS                                           *
 *******************************************************/

class DataShortcut : public CStorageItem
{
	friend class CShortcutStore;
private:

public:
	ppl7::String	shortcut;
	ppl7::String	artist;

	DataShortcut();
	DataShortcut(const DataShortcut& other);
	DataShortcut(const ppl7::String& shortcut, const ppl7::String& artist);
	DataShortcut& operator = (const DataShortcut& other);
	void Clear();
	void CopyFrom(const DataShortcut& other);
	void CopyDataFrom(const DataShortcut& other);
	void Export(ppl7::ByteArray& bin) const;
	void Import(const ppl7::ByteArrayPtr& bin, int version);
	void SetValue(const ppl7::String& shortcut, const ppl7::String& artist);
	const ppl7::String& GetShortcut() const;
	const ppl7::String& GetArtist() const;
};

class CShortcutStore : public CStorageBase
{
private:
	std::map<ppl7::String, DataShortcut> Tree;

	void SaveToStorage(DataShortcut& t);
	DataShortcut* SaveToMemory(const DataShortcut& t);

public:
	typedef std::map<ppl7::String, DataShortcut>::iterator iterator;
	typedef std::map<ppl7::String, DataShortcut>::const_iterator const_iterator;


	CShortcutStore();
	virtual ~CShortcutStore();
	virtual const char* GetChunkName();
	virtual void Clear();
	virtual void LoadChunk(const CWMFileChunk& chunk);
	void Put(const DataShortcut& entry);
	const DataShortcut& Get(const ppl7::String& shortcut) const;
	const DataShortcut* GetPtr(const ppl7::String& shortcut) const;
	void Delete(const ppl7::String& shortcut);
	size_t Size() const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
};

/*******************************************************
 * TABLES                                              *
 *******************************************************/


class CSimpleTable : public CStorageItem {
public:
	uint32_t		Id;
	uint32_t		References;
	ppl7::String	Value;
	CSimpleTable();
	CSimpleTable(uint32_t id, const ppl7::String& value, uint32_t references=0);
	CSimpleTable(const CSimpleTable& other);
	CSimpleTable& operator = (const CSimpleTable& other);
	virtual ~CSimpleTable();

	void Clear();
	void CopyFrom(const CSimpleTable& other);
	void CopyDataFrom(const CSimpleTable& other);
	void Export(ppl7::ByteArray& bin) const;
	void Import(const ppl7::ByteArrayPtr& bin, int version);
	void SetValue(const ppl7::String& value);
	const ppl7::String& GetValue() const;
};

class DataVersion : public CSimpleTable {};
class DataRecordSource : public CSimpleTable {};
class DataLabel : public CSimpleTable {};
class DataPurchaseSource : public CSimpleTable {};
class DataRecordDevice : public CSimpleTable {};
class DataGenre : public CSimpleTable {};


class CTableStore : public CStorageBase
{
public:
	typedef std::set<uint32_t> IndexTree;
private:
	ppl7::String unknown;
	std::map<ppl7::String, IndexTree >	Words;
	std::map<ppl7::String, uint32_t> Tree;
	CSimpleTable** TableIndex;
	uint32_t max;
	uint32_t highestId;
	size_t size;

	void Increase(uint32_t maxid);
	void SaveToStorage(CSimpleTable& t);
	CSimpleTable* SaveToMemory(const CSimpleTable& t);

	void removeFromWordTree(uint32_t id);
	void addToWordTree(uint32_t id);
	void removeFromTree(uint32_t id);
	void makeUnion(IndexTree& Result, const IndexTree& Tree1, const IndexTree& Tree2);
	void copy(IndexTree& Result, const IndexTree& src);


public:
	CTableStore();
	virtual ~CTableStore();
	virtual const char* GetChunkName();
	virtual void Clear();
	virtual void LoadChunk(const CWMFileChunk& chunk);

	uint32_t Put(const CSimpleTable& entry);
	bool Exists(uint32_t id) const;
	void Delete(uint32_t id);
	const CSimpleTable& Get(uint32_t id) const;
	const CSimpleTable* GetPtr(uint32_t id) const;
	const ppl7::String& GetValue(uint32_t id) const;
	uint32_t MaxId() const;
	uint32_t Capacity() const;
	uint32_t Size() const;
	const CSimpleTable* Find(const ppl7::String& value) const;
	uint32_t GetId(const ppl7::String& value) const;
	uint32_t FindOrAdd(const ppl7::String& value);
	size_t FindAll(const ppl7::String& value, IndexTree& Result);
	size_t FindWords(const ppl7::String& words, IndexTree& tree);

};

class CVersionStore : public CTableStore
{
public:
	virtual const char* GetChunkName();
};

class CRecordSourceStore : public CTableStore
{
public:
	virtual const char* GetChunkName();
};

class CLabelStore : public CTableStore
{
public:
	virtual const char* GetChunkName();
};

class CPurchaseSourceStore : public CTableStore
{
public:
	virtual const char* GetChunkName();
};

class CRecordDeviceStore : public CTableStore
{
public:
	virtual const char* GetChunkName();
};

class CGenreStore : public CTableStore
{
public:
	virtual const char* GetChunkName();
};


}
}
}

#endif // WM_STORAGE_H
