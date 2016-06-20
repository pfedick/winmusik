/*
 * libwinmusik3.h
 *
 *  Created on: 20.06.2016
 *      Author: patrick
 */

#ifndef LIBWINMUSIK3_H_
#define LIBWINMUSIK3_H_

#include <ppl7.h>
#include <ppl7-config.h>

namespace de {
namespace pfp {
namespace winmusik {

class CStorage;

enum MusicKeyType
{
	musicKeyTypeNone=0,
	musicKeyTypeMusicalSharps,
	musicKeyTypeOpenKey,
	musicKeyTypeCustom=99
};


class CStorageData
{
		friend class CStorage;
	private:
		ppluint32 filepos;
		ppluint32 lastchange;
		ppluint32 version;

	public:
		CStorageData() {
			filepos=lastchange=version=0;
		}
};


class DataTitle : public CStorageData
{
	public:
		char*			Artist;
		char*			Title;
		char*			Remarks;
		char*			Album;
		char*			Tags;
		ppl7::ByteArray	CoverPreview;
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

		DataTitle();
		DataTitle(const DataTitle &other);
		~DataTitle();
		void Clear();

		int CopyFrom(const DataTitle &other);

		void SetTitle(const ppl7::String &title);
		void SetArtist(const ppl7::String &artist);
		void SetRemarks(const ppl7::String &remarks);
		void SetTags(const ppl7::String &tags);
		void SetAlbum(const ppl7::String &album);
		void SetKey(const ppl7::String &key);
		ppl7::String getKeyName(MusicKeyType type);

		void Export(ppl7::ByteArray &buffer);
		void Import(const ppl7::ByteArrayPtr &buffer);

		DataTitle & operator=(const DataTitle &other);

		//static ppl6::CString keyName(int id, MusicKeyType type);
		//static int keyId(const ppl6::CString &name);
};



class CStorage
{
	private:
	public:


};

}
}
}



#endif /* LIBWINMUSIK3_H_ */
