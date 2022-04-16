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
    uint32_t filepos;
    uint32_t lastchange;
    uint32_t version;

protected:
    void copyFrom(const CStorageData &other);
    void copyFrom(const CWMFileChunk &chunk);

public:
    CStorageData();
    void clear();
    uint32_t getFilePos() const;
    uint32_t getLastchange() const;
    uint32_t getVersion() const;
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
    uint8_t			DeviceType;
    uint8_t			Page;
    uint8_t			Channels;
    uint8_t			Quality;
    uint8_t			Rating;
    uint8_t			Flags;
    uint8_t			Key;
    uint8_t			EnergyLevel;

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
