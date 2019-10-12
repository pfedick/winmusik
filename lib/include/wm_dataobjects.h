#ifndef WM_DATAOBJECTS_H
#define WM_DATAOBJECTS_H

#include <ppl7.h>

namespace de {
namespace pfp {
namespace winmusik {


class CStorage;
class CWMFileChunk;

PPL7EXCEPTION(UnknownDataFormatVersion,Exception)
PPL7EXCEPTION(InvalidDataRecord,Exception)


class CStorageData
{
    friend class CStorage;
private:
    ppluint32 filepos;
    ppluint32 lastchange;
    ppluint32 version;

protected:
    void copyFrom(const CStorageData &other);
    void copyFrom(const CWMFileChunk &chunk);

public:
    CStorageData();
    void clear();
    ppluint32 getFilePos() const;
    ppluint32 getLastchange() const;
    ppluint32 getVersion() const;
};


class DataTitle : public CStorageData
{
private:
    void copyFrom(const DataTitle &other);
    void initialize();

public:
    ppl7::String	Artist;
    ppl7::String	Title;
    ppl7::String	Remarks;
    ppl7::String	Album;
    ppl7::String	Tags;
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
    DataTitle(const CWMFileChunk &chunk);
    ~DataTitle();
    void clear();

    void Export(ppl7::ByteArray &buffer) const;
    void Import(const ppl7::ByteArrayPtr &buffer);

    DataTitle & operator=(const DataTitle &other);
};


}
}
}

#endif // WM_DATAOBJECTS_H
