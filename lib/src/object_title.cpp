#include "wm_dataobjects.h"
#include "wm_cwmfile.h"
#include "wm_functions.h"

namespace de {
namespace pfp {
namespace winmusik {

void DataTitle::copyFrom(const DataTitle &other)
{
    clear();
    CStorageData::copyFrom(other);
    Artist=other.Artist;
    Title=other.Title;
    Remarks=other.Remarks;
    Album=other.Album;
    Tags=other.Tags;
    TitleId=other.TitleId;
    DeviceId=other.DeviceId;
    Length=other.Length;
    VersionId=other.VersionId;
    GenreId=other.GenreId;
    RecordSourceId=other.RecordSourceId;
    RecordDeviceId=other.RecordDeviceId;
    LabelId=other.LabelId;
    BPM=other.BPM;
    RecordDate=other.RecordDate;
    ReleaseDate=other.ReleaseDate;
    Track=other.Track;
    Bitrate=other.Bitrate;
    DeviceType=other.DeviceType;
    Page=other.Page;
    Channels=other.Channels;
    Quality=other.Quality;
    Rating=other.Rating;
    Flags=other.Flags;
    ImportData=other.ImportData;
    CoverPreview=other.CoverPreview;
    Size=other.Size;
    Key=other.Key;
    EnergyLevel=other.EnergyLevel;
}

void DataTitle::initialize()
{
    TitleId=0;
    DeviceId=0;
    Length=0;
    VersionId=0;
    LabelId=0;
    BPM=0;
    RecordDate=0;
    ReleaseDate=0;
    ImportData=0;
    Size=0;
    RecordSourceId=0;
    Track=0;
    Bitrate=0;
    GenreId=0;
    RecordDeviceId=0;
    DeviceType=0;
    Page=0;
    Channels=0;
    Quality=0;
    Rating=0;
    Flags=0;
    Key=0;
    EnergyLevel=0;
}

DataTitle::DataTitle()
{
    initialize();
}



DataTitle::DataTitle(const DataTitle &other)
{
    initialize();
    copyFrom(other);
}

DataTitle::DataTitle(const CWMFileChunk &chunk)
{
    initialize();
    CStorageData::copyFrom(chunk);
    Import(chunk.GetData());
}

DataTitle::~DataTitle()
{
    clear();
}

void DataTitle::clear()
{
    CStorageData::clear();
    Artist.clear();
    Title.clear();
    Remarks.clear();
    Album.clear();
    Tags.clear();
    CoverPreview.clear();
    initialize();
}

void DataTitle::Export(ppl7::ByteArray &buffer) const
{
    // Zunächst den benötigten Speicher berechnen
    size_t size=71;
    size_t p=0;
    size_t lenArtist=Artist.size();
    size_t lenTitle=Title.size();
    size_t lenRemarks=Remarks.size();
    size_t lenTags=Tags.size();
    size_t lenAlbum=Album.size();
    size_t lenCoverPreview=CoverPreview.size();
    size=size+lenArtist+lenTitle+lenRemarks+lenAlbum+lenTags+lenCoverPreview;
    char *a=static_cast<char *>(buffer.malloc(size));


    ppl7::Poke8(a+0,5);     // Formatversion=5
    ppl7::Poke32(a+1,TitleId);
    ppl7::Poke8(a+5,Flags);
    ppl7::Poke32(a+6,DeviceId);
    ppl7::Poke32(a+10,Length);
    ppl7::Poke32(a+14,VersionId);
    ppl7::Poke32(a+18,LabelId);
    ppl7::Poke32(a+22,BPM);
    ppl7::Poke32(a+26,RecordDate);
    ppl7::Poke32(a+30,ReleaseDate);
    ppl7::Poke16(a+34,RecordSourceId);
    ppl7::Poke16(a+36,Track);
    ppl7::Poke16(a+38,Bitrate);
    ppl7::Poke16(a+40,GenreId);
    ppl7::Poke16(a+42,RecordDeviceId);
    ppl7::Poke8(a+44,DeviceType);
    ppl7::Poke8(a+45,Page);
    ppl7::Poke8(a+46,Channels);
    ppl7::Poke8(a+47,Quality);
    ppl7::Poke8(a+48,Rating);
    ppl7::Poke32(a+49,ImportData);
    ppl7::Poke32(a+53,Size);
    ppl7::Poke8(a+57,Key);
    ppl7::Poke8(a+58,EnergyLevel);
    p=60;

    ppl7::Poke16(a+p,static_cast<ppluint16>(lenArtist));
    if (lenArtist) strncpy(a+p+2,Artist.getPtr(),lenArtist);
    p+=lenArtist+2;
    ppl7::Poke16(a+p,static_cast<ppluint16>(lenTitle));
    if (lenTitle) strncpy(a+p+2,Title.getPtr(),lenTitle);
    p+=lenTitle+2;
    ppl7::Poke16(a+p,static_cast<ppluint16>(lenRemarks));
    if (lenRemarks) strncpy(a+p+2,Remarks.getPtr(),lenRemarks);
    p+=lenRemarks+2;
    ppl7::Poke16(a+p,static_cast<ppluint16>(lenAlbum));
    if (lenAlbum) strncpy(a+p+2,Album.getPtr(),lenAlbum);
    p+=lenAlbum+2;
    ppl7::Poke16(a+p,static_cast<ppluint16>(lenTags));
    if (lenTags) strncpy(a+p+2,Tags.getPtr(),lenTags);
    p+=lenTags+2;
    ppl7::Poke16(a+p,static_cast<ppluint16>(lenCoverPreview));
    if (lenCoverPreview) memcpy(a+p+2,CoverPreview.ptr(),lenCoverPreview);
    p+=lenCoverPreview+2;
}

void DataTitle::Import(const ppl7::ByteArrayPtr &buffer)
{
    ppluint8 version=Peek8(buffer,0);
    if (version<1 || version>5) throw UnknownDataFormatVersion("%hhu",version);
    // Die Größe muss mindestens 60 Byte betragen
    if (buffer.size()<60) throw InvalidDataRecord();

    TitleId=Peek32(buffer,1);
    Flags=Peek8(buffer,5);
    DeviceId=Peek32(buffer,6);
    Length=Peek32(buffer,10);
    VersionId=Peek32(buffer,14);
    LabelId=Peek32(buffer,18);
    BPM=Peek32(buffer,22);
    RecordDate=Peek32(buffer,26);
    ReleaseDate=Peek32(buffer,30);
    if (ReleaseDate<10000000) {			// Korrektur fehlerhafter Timestamps
        if (ReleaseDate>1900 && ReleaseDate<2015) {
            ReleaseDate=ReleaseDate*10000;
        } else {
            ReleaseDate=RecordDate;
            if (ReleaseDate>1900 && ReleaseDate<2015) {
                ReleaseDate=ReleaseDate*10000;
            }
        }
    }
    if (RecordDate<10000000) {			// Korrektur fehlerhafter Timestamps
        if (RecordDate>1900 && RecordDate<2015) {
            RecordDate=RecordDate*10000+0101;
        } else {
            RecordDate=ReleaseDate;
        }
    }
    RecordSourceId=Peek16(buffer,34);
    Track=Peek16(buffer,36);
    Bitrate=Peek16(buffer,38);
    GenreId=Peek16(buffer,40);
    RecordDeviceId=Peek16(buffer,42);
    DeviceType=Peek8(buffer,44);
    Page=Peek8(buffer,45);
    Channels=Peek8(buffer,46);
    Quality=Peek8(buffer,47);
    Rating=Peek8(buffer,48);
    ImportData=Peek32(buffer,49);
    size_t p=0;
    if (version==1) {
        p=53;
        Size=0;
    } else if (version>=2) {
        Size=Peek32(buffer,53);
        p=57;
    }
    if (version >= 4) {
        Key=Peek8(buffer,57);
        p=58;
    }
    if (version >= 5) {
        EnergyLevel=Peek8(buffer,58);
        p=59;
    }
    size_t len=Peek16(buffer,p);
    if (len) Artist=PeekString(buffer,p+2,len);
    p+=len+2;
    len=Peek16(buffer,p);
    if (len) Title=PeekString(buffer,p+2,len);
    p+=len+2;
    len=Peek16(buffer,p);
    if (len) Remarks=PeekString(buffer,p+2,len);
    p+=len+2;
    len=Peek16(buffer,p);
    if (len) Album=PeekString(buffer,p+2,len);
    p+=len+2;
    if (version>=3) {
        len=Peek16(buffer,p);
        if (len) Tags=PeekString(buffer,p+2,len);
        p+=len+2;
    }
    if (version>=2) {
        len=Peek16(buffer,p);
        if (len) CoverPreview.copy(SubBuffer(buffer,p+2,len));
        p+=len+2;
    }
}

}}}	// EOF Namespace de.pfp.winmusik
