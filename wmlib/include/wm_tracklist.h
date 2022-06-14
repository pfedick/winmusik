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

#ifndef WM_TRACKLIST_H
#define WM_TRACKLIST_H

#include <ppl7.h>
#include <map>



namespace de {
namespace pfp {
namespace winmusik {

class DataTrack;
class CTrackStore;
class CTitleStore;

class CTrackList
{
    friend class CTrackStore;
private:
    std::map<int, DataTrack> Tracks;
    CTrackStore* storage;
    uint32_t	DeviceId;
    uint8_t	DeviceType;
    uint8_t	Page;

    DataTrack* SaveToMemory(const DataTrack& t);
    void SaveToStorage(DataTrack& t);
    void Add(uint16_t Track, const DataTrack& t);
    void copy(const CTrackList& other);


public:
    typedef std::map<int, DataTrack>::iterator iterator;
    typedef std::map<int, DataTrack>::const_iterator const_iterator;

    CTrackList();
    CTrackList(CTrackStore* storage);
    CTrackList(const CTrackList& other);
    ~CTrackList();
    CTrackList& operator=(const CTrackList& other);
    void Clear();
    int GetMin() const;
    int GetMax() const;
    int Num() const;
    size_t Size() const;
    const DataTrack& Get(int track) const;
    const DataTrack* GetPtr(int track) const;
    bool Exists(int track) const;
    const DataTrack& Put(const DataTrack& entry);
    void Delete(int track);
    //void DeleteShift(int track, CTitleStore* tistore);
    //void InsertShift(int track, CTitleStore* tistore);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;


};


}
}
}

#endif // WM_TRACKLIST_H
