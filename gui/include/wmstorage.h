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

#ifndef WMSTORAGE_H_
#define WMSTORAGE_H_

#ifndef WINMUSIK3_H_
#include "winmusik3.h"
#endif
 /*******************************************************
  * Storage Classes                                     *
  *******************************************************/
class CStorage;
class CWMFileChunk;
class CTrackStore;
class CTitleStore;
class DataTitle;
class CTrackList;
class CCallback;
class DataOimp;

class CStorageItem
{
	friend class CStorage;
private:
	u_int32_t filepos;
	u_int8_t  fileid;
	u_int32_t lastchange;
	u_int32_t version;
protected:
	u_int8_t  formatversion;

public:
	CStorageItem();
	CStorageItem(const CStorageItem& other);
	~CStorageItem();
	void Clear();
	void ClearStorageData();
	void CopyStorageFrom(const CStorageItem& other);
	void CopyStorageFrom(const CStorageItem* item);
	void CopyStorageFrom(CWMFileChunk* chunk);
	u_int32_t GetVersion();
	u_int32_t GetLastChange();
	u_int8_t GetFormatVersion();
	void PrintStorageData();
};

class CStorageType : public ppl6::CTreeItem
{
	friend class CStorage;
private:
	const char* type;
public:
	CStorageType();
	virtual int CompareNode(CTreeItem* item);
	virtual int CompareValue(void* value);
	virtual void Clear();

	CStorage* Storage;
	virtual const char* GetChunkName();
	virtual int LoadChunk(CWMFileChunk* chunk);
};


class CStorage
{
private:
	ppl6::CMutex Mutex;
	ppl6::CString DataPath;
	ppl6::CTree StorageClasses;
	ppl6::CTree Files;
	bool	bLoadDatabaseRunning;
	bool	bKeepFileOpen;
	CWMFile	StorageFile;

	void ClearWithoutMutex();



public:
	CStorage();
	virtual ~CStorage();
	void Clear();
	int Init(const char* path);
	int RegisterStorageType(CStorageType* type);
	CStorageType* FindStorageType(const char* name);
	int Save(CStorageType* type, CStorageItem* item, ppl6::CBinary* bin);
	int Delete(CStorageType* type, CStorageItem* item);

	int LoadDatabase(CCallback* c);
	int LoadOimpRecord(DataOimp* item);
	int DeleteDatabase();

	int StartImport();
	int FinishImport();

	bool isDatabaseLoading() { return bLoadDatabaseRunning; }

	//int CreateDatabase();
};


/*******************************************************
 * TITLES                                              *
 *******************************************************/

typedef struct {
	DataTitle* t;
} TITLE;


class CTitleStore : public CStorageType
{
private:
	ppl6::CMutex Mutex;
	TITLE* TitleIndex;
	u_int32_t max;
	u_int32_t highestId;
	u_int32_t highestOimp;


	int Increase(u_int32_t maxid);
	int Save(DataTitle* t);

public:
	CTitleStore();
	virtual ~CTitleStore();
	virtual const char* GetChunkName();
	virtual void Clear();

	ppl6::CTree Artists;

	int Put(DataTitle* title);
	int Delete(DataTitle* entry);
	DataTitle* Get(u_int32_t id);
	int GetCopy(u_int32_t id, DataTitle* t);
	virtual int LoadChunk(CWMFileChunk* chunk);
	u_int32_t MaxId();

	u_int32_t GetHighestImportDataId();
};



class DataTitle : public CStorageItem
{
public:
	ppl7::String	Artist;
	ppl7::String	Title;
	ppl7::String	Remarks;
	ppl7::String	Album;
	ppl7::String	Tags;
	ppl7::ByteArray	CoverPreview;
	u_int32_t		TitleId;
	u_int32_t		DeviceId;
	u_int32_t		Length;
	u_int32_t		VersionId;
	u_int32_t		LabelId;
	u_int32_t		BPM;
	u_int32_t		RecordDate;
	u_int32_t		ReleaseDate;
	u_int32_t		ImportData;
	u_int32_t		Size;
	u_int16_t		RecordSourceId;
	u_int16_t		Track;
	u_int16_t		Bitrate;
	u_int16_t		GenreId;
	u_int16_t		RecordDeviceId;
	u_int8_t		DeviceType;
	u_int8_t		Page;
	u_int8_t		Channels;
	u_int8_t		Quality;
	u_int8_t		Rating;
	u_int8_t		Flags;
	u_int8_t		Key;
	u_int8_t		EnergyLevel;
	//void *operator new (size_t size);
	//void operator delete (void *ptr, size_t size);

	DataTitle();
	DataTitle(const DataTitle& other);
	~DataTitle();
	void Clear();

	int CopyFrom(const DataTitle* t);
	int CopyFrom(const DataTitle& other);
	void SetTitle(const char* title);
	void SetArtist(const char* artist);
	void SetRemarks(const char* remarks);
	void SetTags(const char* tags);
	void SetAlbum(const char* album);

	void SetTitle(const ppl7::String& title);
	void SetArtist(const ppl7::String& artist);
	void SetRemarks(const ppl7::String& remarks);
	void SetTags(const ppl7::String& tags);
	void SetAlbum(const ppl7::String& album);
	void SetKey(const ppl7::String& key);
	ppl7::String getKeyName(MusicKeyType type);

	ppl6::CBinary* Export();
	int Import(ppl6::CBinary* bin, int version);

	DataTitle& operator=(const DataTitle& other);
	bool operator==(const DataTitle& other) const;
	bool operator!=(const DataTitle& other) const;

	static ppl7::String keyName(u_int8_t id, MusicKeyType type);
	static u_int8_t keyId(const ppl7::String& name);
};


/*******************************************************
 * DEVICES                                             *
 *******************************************************/


typedef struct {
	u_int32_t		DeviceId;
	u_int8_t		DeviceType;
} DATADEVICE;

class DataDevice : public CStorageItem, public ppl6::CTreeItem
{
private:
public:
	char* Title, * SubTitle;
	u_int32_t		DeviceId;
	u_int32_t		Length;
	u_int32_t		Recorded;
	u_int32_t		LabelId;
	u_int32_t		PurchaseDate;
	u_int32_t		DateCreated;
	u_int32_t		FirstDate;
	u_int32_t		LastDate;
	u_int32_t		NumTracks;
	float			PurchasePrice;
	u_int16_t		PurchaseId;
	u_int8_t		DeviceType;
	u_int8_t		Pages;

	DataDevice();
	virtual ~DataDevice();
	virtual int CompareNode(CTreeItem* item);
	virtual int CompareValue(void* value);
	void Clear();

	int CopyFrom(DataDevice* t);
	int SetTitle(const char* title);
	int SetSubTitle(const char* artist);
	ppl6::CString GetTitle() const;
	ppl6::CString GetSubTitle() const;

	ppl6::CBinary* Export();
	int Import(ppl6::CBinary* bin, int version);
};

class CDeviceStore : public CStorageType
{
private:
	ppl6::CMutex Mutex;
	ppl6::CTree Tree;
	int Save(DataDevice* t);
	u_int32_t highest[MAX_DEVICE_TYPES + 1];

public:
	CDeviceStore();
	virtual ~CDeviceStore();
	virtual void Clear();
	virtual const char* GetChunkName();
	int Put(DataDevice* entry);
	int Renumber(u_int8_t DeviceType, u_int32_t oldId, u_int32_t newId);
	DataDevice* Get(u_int8_t DeviceType, u_int32_t DeviceId);
	int GetCopy(u_int8_t DeviceType, u_int32_t DeviceId, DataDevice* t);
	virtual int LoadChunk(CWMFileChunk* chunk);
	u_int32_t GetHighestDevice(int DeviceType);
	int Update(u_int8_t DeviceType, u_int32_t DeviceId);
};

/*******************************************************
 * TRACKS                                              *
 *******************************************************/


class DataTrack : public CStorageItem, public ppl6::CTreeItem
{
private:
	//ppluint64	Key;
public:
	u_int32_t	DeviceId;
	u_int32_t	TitleId;
	u_int16_t	Track;
	u_int8_t	Device;
	u_int8_t	Page;

	DataTrack();
	virtual ~DataTrack();
	virtual int CompareNode(CTreeItem* item);
	virtual int CompareValue(void* value);

	void Clear();
	int CopyFrom(DataTrack* t);
	int SetValue(u_int8_t Device, u_int32_t DeviceId, u_int8_t Page, u_int16_t Track, u_int32_t TitleId);
	ppl6::CBinary* Export();
	int Import(ppl6::CBinary* bin, int version);
};


class DataTrackHint : public ppl6::CTreeItem
{
public:
	u_int32_t	DeviceId;
	u_int8_t	Device;
	u_int8_t	Page;
	int			min, max;

	DataTrackHint();
	virtual ~DataTrackHint();
	virtual int CompareNode(CTreeItem* item);
	virtual int CompareValue(void* value);
};

class CTrackStore : public CStorageType
{
private:
	ppl6::CMutex Mutex;
	ppl6::CTree Tree;
	ppl6::CTree TrackHints;
	int Save(DataTrack* t);
	int UpdateHints(DataTrack* track);
	DataTrackHint* GetHints(u_int8_t Device, u_int32_t DeviceId, u_int8_t Page);

public:
	CTrackStore();
	virtual ~CTrackStore();
	virtual void Clear();
	virtual const char* GetChunkName();
	int Put(DataTrack* entry);
	int Delete(DataTrack* entry);
	DataTrack* Get(u_int8_t Device, u_int32_t DeviceId, u_int8_t Page, u_int16_t Track);
	int GetCopy(u_int8_t Device, u_int32_t DeviceId, u_int8_t Page, u_int16_t Track, DataTrack* t);
	CTrackList* GetTracklist(u_int8_t Device, u_int32_t DeviceId, u_int8_t Page);
	virtual int LoadChunk(CWMFileChunk* chunk);

};

/*******************************************************
 * SHORTCUTS                                           *
 *******************************************************/

class DataShortcut : public CStorageItem, public ppl6::CTreeItem
{
	friend class CShortcutStore;
private:
	char* shortcut;
	char* artist;


public:
	DataShortcut();
	DataShortcut(const DataShortcut& other);
	DataShortcut& operator = (const DataShortcut& other);

	virtual ~DataShortcut();
	virtual int CompareNode(CTreeItem* item);
	virtual int CompareValue(void* value);

	void Clear();
	int CopyFrom(const DataShortcut* t);
	int CopyFrom(const DataShortcut& t);
	int SetValue(const char* shortcut, const char* artist);
	const char* GetShortcut();
	const char* GetArtist();
	ppl6::CBinary* Export();
	int Import(ppl6::CBinary* bin, int version);
};

class CShortcutStore : public CStorageType
{
private:
	ppl6::CMutex Mutex;
	ppl6::CTree Tree;
	int Save(DataShortcut* t);

public:
	CShortcutStore();
	virtual ~CShortcutStore();
	virtual void Clear();
	virtual const char* GetChunkName();
	int Put(DataShortcut* entry);
	DataShortcut* Get(const char* shortcut);
	int Delete(const char* shortcut);
	int GetCopy(const char* shortcut, DataShortcut* t);
	void List();
	virtual int LoadChunk(CWMFileChunk* chunk);
	void Reset();
	DataShortcut* GetFirst();
	DataShortcut* GetNext();
};


/*******************************************************
 * TABLES                                              *
 *******************************************************/


class CSimpleTable : public CStorageItem, public ppl6::CTreeItem
{
public:
	u_int32_t		Id;
	u_int32_t		References;
	char* Value;
	CSimpleTable();
	CSimpleTable(const CSimpleTable& other);
	CSimpleTable& operator = (const CSimpleTable& other);
	virtual ~CSimpleTable();
	virtual int CompareNode(CTreeItem* item);
	virtual int CompareValue(void* value);

	void Clear();
	int CopyFrom(const CSimpleTable* t);
	int CopyFrom(const CSimpleTable& other);
	int SetValue(const char* value);
	ppl6::CBinary* Export();
	int Import(ppl6::CBinary* bin, int version);
};

class DataVersion : public CSimpleTable {};
class DataRecordSource : public CSimpleTable {};
class DataLabel : public CSimpleTable {};
class DataPurchaseSource : public CSimpleTable {};
class DataRecordDevice : public CSimpleTable {};
class DataGenre : public CSimpleTable {};

typedef struct {
	CSimpleTable* t;
} TABLE;

class CTableStore : public CStorageType
{
public:
	typedef std::set<u_int32_t> IndexTree;
private:
	ppl6::CMutex Mutex;
	ppl6::CTree Tree;
	TABLE* TableIndex;
	u_int32_t max;
	u_int32_t highestId;
	int Increase(u_int32_t maxid);
	int Save(CSimpleTable* t);
	typedef std::map<ppl7::String, IndexTree >	WordTree;

	void removeFromWordTree(u_int32_t id);
	void addToWordTree(u_int32_t id);
	void makeUnion(IndexTree& Result, const IndexTree& Tree1, const IndexTree& Tree2);
	void copy(IndexTree& Result, const IndexTree& src);

	WordTree Words;

public:
	CTableStore();
	virtual ~CTableStore();
	virtual void Clear();
	virtual const char* GetChunkName();
	virtual int LoadChunk(CWMFileChunk* chunk);
	int Put(CSimpleTable* entry);
	CSimpleTable* Get(u_int32_t id);
	CSimpleTable* Find(const char* value);
	int GetId(const char* value);
	int FindOrAdd(const char* value);
	int FindAll(ppl6::CWString& value, ppl6::CTree& Result);
	int GetCopy(u_int32_t id, CSimpleTable* t);

	u_int32_t findWords(IndexTree& tree, const ppl7::String& words);
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

/*******************************************************
 * Original Import Daten                               *
 *******************************************************/

class DataOimp : public CStorageItem
{
public:
	u_int32_t			Id;
	ppl7::String		Filename;
	ppl7::AssocArray	ID3v1;
	ppl7::AssocArray	ID3v2;


	DataOimp();
	virtual ~DataOimp();

	void Clear();
	int CopyFrom(const DataOimp* t);
	ppl6::CBinary* Export();
	int Import(ppl6::CBinary* bin, int version);
	int ImportId(ppl6::CBinary* bin, int version);
};


typedef struct {
	DataOimp* t;
} OIMPDATA;

class COimpDataStore : public CStorageType
{
private:
	ppl6::CMutex Mutex;
	OIMPDATA* TableIndex;
	u_int32_t max;
	u_int32_t highestId;
	int Increase(u_int32_t maxid);
	int Save(DataOimp* t);

public:
	COimpDataStore();
	virtual ~COimpDataStore();
	virtual void Clear();
	virtual const char* GetChunkName();
	virtual int LoadChunk(CWMFileChunk* chunk);
	int Put(DataOimp* entry);

	int GetCopy(u_int32_t id, DataOimp* t);

	void SetHighestImportDataId(u_int32_t id);

	void List();
};


#endif	// WMSTORAGE
