/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.4 $
 * $Date: 2010/11/13 21:38:58 $
 * $Id: wmstorage.h,v 1.4 2010/11/13 21:38:58 pafe Exp $
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


#ifndef WMSTORAGE_H_
#define WMSTORAGE_H_


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
		ppluint32 filepos;
		ppluint8  fileid;
		ppluint32 lastchange;
		ppluint32 version;
	protected:
		ppluint8  formatversion;

	public:
		CStorageItem();
		CStorageItem(const CStorageItem &other);
		~CStorageItem();
		void Clear();
		void ClearStorageData();
		void CopyStorageFrom(const CStorageItem &other);
		void CopyStorageFrom(const CStorageItem *item);
		void CopyStorageFrom(CWMFileChunk *chunk);
		ppluint32 GetVersion();
		ppluint32 GetLastChange();
		ppluint8 GetFormatVersion();
		void PrintStorageData();
};

class CStorageType : public ppl6::CTreeItem
{
	friend class CStorage;
	private:
		const char *type;
	public:
		CStorageType();
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);
		virtual void Clear();

		CStorage *Storage;
		virtual const char *GetChunkName();
		virtual int LoadChunk(CWMFileChunk *chunk);
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
		int Init(const char *path);
		int RegisterStorageType(CStorageType *type);
		CStorageType *FindStorageType(const char *name);
		int Save(CStorageType *type, CStorageItem *item, ppl6::CBinary *bin);
		int Delete(CStorageType *type, CStorageItem *item);

		int LoadDatabase(CCallback *c);
		int LoadOimpRecord(DataOimp *item);
		int DeleteDatabase();

		int StartImport();
		int FinishImport();

		bool isDatabaseLoading() { return bLoadDatabaseRunning; };

		//int CreateDatabase();
};


/*******************************************************
 * TITLES                                              *
 *******************************************************/

typedef struct {
	DataTitle *t;
} TITLE;


class CTitleStore : public CStorageType
{
	private:
		ppl6::CMutex Mutex;
		TITLE *TitleIndex;
		ppluint32 max;
		ppluint32 highestId;
		ppluint32 highestOimp;


		int Increase(ppluint32 maxid);
		int Save(DataTitle *t);

	public:
		CTitleStore();
		virtual ~CTitleStore();
		virtual const char *GetChunkName();
		virtual void Clear();

		ppl6::CTree Artists;

		int Put(DataTitle *title);
		int Delete(DataTitle *entry);
		DataTitle *Get(ppluint32 id);
		int GetCopy(ppluint32 id, DataTitle *t);
		virtual int LoadChunk(CWMFileChunk *chunk);
		ppluint32 MaxId();

		ppluint32 GetHighestImportDataId();
};



class DataTitle : public CStorageItem
{
	public:
		char*			Artist;
		char*			Title;
		char*			Remarks;
		char*			Album;
		char*			Tags;
		ppl6::CBinary	CoverPreview;
		ppluint32		TitleId;
		ppluint32		DeviceId;
		ppluint32		Length;
		ppluint32		VersionId;
		ppluint32		LabelId;
		ppluint32		BPM;
		ppluint32		RecordDate;
		ppluint32		ReleaseDate;
		ppluint32		ImportData;
		ppluint32		Size;
		ppluint16		RecordSourceId;
		ppluint16		Track;
		ppluint16		Bitrate;
		ppluint16		GenreId;
		ppluint16		RecordDeviceId;
		ppluint8		DeviceType;
		ppluint8		Page;
		ppluint8		Channels;
		ppluint8		Quality;
		ppluint8		Rating;
		ppluint8		Flags;
		ppluint8		Key;
		ppluint8		EnergyLevel;
		//void *operator new (size_t size);
		//void operator delete (void *ptr, size_t size);

		DataTitle();
		DataTitle(const DataTitle &other);
		~DataTitle();
		void Clear();

		int CopyFrom(const DataTitle *t);
		int CopyFrom(const DataTitle &other);
		void SetTitle(const char *title);
		void SetArtist(const char *artist);
		void SetRemarks(const char *remarks);
		void SetTags(const char *tags);
		void SetAlbum(const char *album);

		void SetTitle(const ppl6::CString &title);
		void SetArtist(const ppl6::CString &artist);
		void SetRemarks(const ppl6::CString &remarks);
		void SetTags(const ppl6::CString &tags);
		void SetAlbum(const ppl6::CString &album);
		void SetKey(const ppl6::CString &key);
		ppl6::CString getKeyName(MusicKeyType type);

		ppl6::CBinary *Export();
		int Import(ppl6::CBinary *bin, int version);

		DataTitle & operator=(const DataTitle &other);

		static ppl6::CString keyName(int id, MusicKeyType type);
		static int keyId(const ppl6::CString &name);
};


/*******************************************************
 * DEVICES                                             *
 *******************************************************/


typedef struct {
	ppluint32		DeviceId;
	ppluint8		DeviceType;
} DATADEVICE;

class DataDevice : public CStorageItem, public ppl6::CTreeItem
{
	private:
	public:
		char *Title,*SubTitle;
		ppluint32		DeviceId;
		ppluint32		Length;
		ppluint32		Recorded;
		ppluint32		LabelId;
		ppluint32		PurchaseDate;
		ppluint32		DateCreated;
		ppluint32		FirstDate;
		ppluint32		LastDate;
		ppluint32		NumTracks;
		float			PurchasePrice;
		ppluint16		PurchaseId;
		ppluint8		DeviceType;
		ppluint8		Pages;

		DataDevice();
		virtual ~DataDevice();
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);
		void Clear();

		int CopyFrom(DataDevice *t);
		int SetTitle(const char *title);
		int SetSubTitle(const char *artist);
		ppl6::CBinary *Export();
		int Import(ppl6::CBinary *bin, int version);
};

class CDeviceStore : public CStorageType
{
	private:
		ppl6::CMutex Mutex;
		ppl6::CTree Tree;
		int Save(DataDevice *t);
		ppluint32 highest[MAX_DEVICE_TYPES+1];

	public:
		CDeviceStore();
		virtual ~CDeviceStore();
		virtual void Clear();
		virtual const char *GetChunkName();
		int Put(DataDevice *entry);
		DataDevice *Get(ppluint8 DeviceType, ppluint32 DeviceId);
		int GetCopy(ppluint8 DeviceType, ppluint32 DeviceId, DataDevice *t);
		virtual int LoadChunk(CWMFileChunk *chunk);
		ppluint32 GetHighestDevice(int DeviceType);
		int Update(ppluint8 DeviceType, ppluint32 DeviceId);
};

/*******************************************************
 * TRACKS                                              *
 *******************************************************/


class DataTrack : public CStorageItem, public ppl6::CTreeItem
{
	private:
		//ppluint64	Key;
	public:
		ppluint32	DeviceId;
		ppluint32	TitleId;
		ppluint16	Track;
		ppluint8	Device;
		ppluint8	Page;

		DataTrack();
		virtual ~DataTrack();
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);

		void Clear();
		int CopyFrom(DataTrack *t);
		int SetValue(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track, ppluint32 TitleId);
		ppl6::CBinary *Export();
		int Import(ppl6::CBinary *bin, int version);
};


class DataTrackHint : public ppl6::CTreeItem
{
	public:
		ppluint32	DeviceId;
		ppluint8	Device;
		ppluint8	Page;
		int			min,max;

		DataTrackHint();
		virtual ~DataTrackHint();
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);
};

class CTrackStore : public CStorageType
{
	private:
		ppl6::CMutex Mutex;
		ppl6::CTree Tree;
		ppl6::CTree TrackHints;
		int Save(DataTrack *t);
		int UpdateHints(DataTrack *track);
		DataTrackHint *GetHints(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page);

	public:
		CTrackStore();
		virtual ~CTrackStore();
		virtual void Clear();
		virtual const char *GetChunkName();
		int Put(DataTrack *entry);
		int Delete(DataTrack *entry);
		DataTrack *Get(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track);
		int GetCopy(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page, ppluint16 Track, DataTrack *t);
		CTrackList *GetTracklist(ppluint8 Device, ppluint32 DeviceId, ppluint8 Page);
		virtual int LoadChunk(CWMFileChunk *chunk);

};

/*******************************************************
 * SHORTCUTS                                           *
 *******************************************************/

class DataShortcut : public CStorageItem, public ppl6::CTreeItem
{
	friend class CShortcutStore;
	private:
		char	*shortcut;
		char	*artist;


	public:
		DataShortcut();
		DataShortcut(const DataShortcut &other);
		DataShortcut & operator = (const DataShortcut &other);

		virtual ~DataShortcut();
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);

		void Clear();
		int CopyFrom(const DataShortcut *t);
		int CopyFrom(const DataShortcut &t);
		int SetValue(const char *shortcut, const char *artist);
		const char *GetShortcut();
		const char *GetArtist();
		ppl6::CBinary *Export();
		int Import(ppl6::CBinary *bin, int version);
};

class CShortcutStore : public CStorageType
{
	private:
		ppl6::CMutex Mutex;
		ppl6::CTree Tree;
		int Save(DataShortcut *t);

	public:
		CShortcutStore();
		virtual ~CShortcutStore();
		virtual void Clear();
		virtual const char *GetChunkName();
		int Put(DataShortcut *entry);
		DataShortcut *Get(const char *shortcut);
		int Delete(const char *shortcut);
		int GetCopy(const char *shortcut, DataShortcut *t);
		void List();
		virtual int LoadChunk(CWMFileChunk *chunk);
		void Reset();
		DataShortcut *GetFirst();
		DataShortcut *GetNext();
};


/*******************************************************
 * TABLES                                              *
 *******************************************************/


class CSimpleTable : public CStorageItem, public ppl6::CTreeItem
{
	public:
		ppluint32		Id;
		ppluint32		References;
		char			*Value;
		CSimpleTable();
		CSimpleTable(const CSimpleTable &other);
		CSimpleTable & operator = (const CSimpleTable &other);
		virtual ~CSimpleTable();
		virtual int CompareNode(CTreeItem *item);
		virtual int CompareValue(void *value);

		void Clear();
		int CopyFrom(const CSimpleTable *t);
		int CopyFrom(const CSimpleTable &other);
		int SetValue(const char *value);
		ppl6::CBinary *Export();
		int Import(ppl6::CBinary *bin, int version);
};

class DataVersion : public CSimpleTable {};
class DataRecordSource : public CSimpleTable {};
class DataLabel : public CSimpleTable {};
class DataPurchaseSource : public CSimpleTable {};
class DataRecordDevice : public CSimpleTable {};
class DataGenre : public CSimpleTable {};

typedef struct {
	CSimpleTable *t;
} TABLE;

class CTableStore : public CStorageType
{
	private:
		ppl6::CMutex Mutex;
		ppl6::CTree Tree;
		TABLE *TableIndex;
		ppluint32 max;
		ppluint32 highestId;
		int Increase(ppluint32 maxid);
		int Save(CSimpleTable *t);

	public:
		CTableStore();
		virtual ~CTableStore();
		virtual void Clear();
		virtual const char *GetChunkName();
		virtual int LoadChunk(CWMFileChunk *chunk);
		int Put(CSimpleTable *entry);
		CSimpleTable *Get(ppluint32 id);
		CSimpleTable *Find(const char *value);
		int GetId(const char *value);
		int FindOrAdd(const char *value);
		int FindAll(ppl6::CWString &value, ppl6::CTree &Result);
		int GetCopy(ppluint32 id, CSimpleTable *t);
};

class CVersionStore : public CTableStore
{
	public:
		virtual const char *GetChunkName();
};

class CRecordSourceStore : public CTableStore
{
	public:
		virtual const char *GetChunkName();
};

class CLabelStore : public CTableStore
{
	public:
		virtual const char *GetChunkName();
};

class CPurchaseSourceStore : public CTableStore
{
	public:
		virtual const char *GetChunkName();
};

class CRecordDeviceStore : public CTableStore
{
	public:
		virtual const char *GetChunkName();
};

class CGenreStore : public CTableStore
{
	public:
		virtual const char *GetChunkName();
};

/*******************************************************
 * Original Import Daten                               *
 *******************************************************/

class DataOimp : public CStorageItem
{
	public:
		ppluint32			Id;
		ppl6::CString		Filename;
		ppl6::CAssocArray	ID3v1;
		ppl6::CAssocArray	ID3v2;


		DataOimp();
		virtual ~DataOimp();

		void Clear();
		int CopyFrom(const DataOimp *t);
		ppl6::CBinary *Export();
		int Import(ppl6::CBinary *bin, int version);
		int ImportId(ppl6::CBinary *bin, int version);
};


typedef struct {
	DataOimp *t;
} OIMPDATA;

class COimpDataStore : public CStorageType
{
	private:
		ppl6::CMutex Mutex;
		OIMPDATA *TableIndex;
		ppluint32 max;
		ppluint32 highestId;
		int Increase(ppluint32 maxid);
		int Save(DataOimp *t);

	public:
		COimpDataStore();
		virtual ~COimpDataStore();
		virtual void Clear();
		virtual const char *GetChunkName();
		virtual int LoadChunk(CWMFileChunk *chunk);
		int Put(DataOimp *entry);

		int GetCopy(ppluint32 id, DataOimp *t);

		void SetHighestImportDataId(ppluint32 id);

		void List();
};


#endif	// WMSTORAGE
