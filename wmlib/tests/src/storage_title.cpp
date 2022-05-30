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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <gtest/gtest.h>
#include "libwinmusik3.h"
#include "wm_storage.h"
#include "wm_playlist.h"
#include "wm_exceptions.h"
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class StorageTitleTest : public ::testing::Test {
protected:
	StorageTitleTest() {
		ppl7::Dir::mkDir("tmp/title", true);

	}
	~StorageTitleTest()
	{

	}
};


class DataTitleTest : public ::testing::Test {
protected:
	DataTitleTest() {

	}
	~DataTitleTest() {

	}
};

TEST_F(DataTitleTest, DefaultConstructor) {
	DataTitle t1;
	ASSERT_EQ((uint32_t)0, t1.TitleId);
	ASSERT_EQ((uint32_t)0, t1.DeviceId);
	ASSERT_EQ((uint32_t)0, t1.Length);
	ASSERT_EQ((uint32_t)0, t1.VersionId);
	ASSERT_EQ((uint32_t)0, t1.LabelId);
	ASSERT_EQ((uint32_t)0, t1.BPM);
	ASSERT_EQ((uint32_t)0, t1.RecordDate);
	ASSERT_EQ((uint32_t)0, t1.ReleaseDate);
	ASSERT_EQ((uint32_t)0, t1.ImportData);
	ASSERT_EQ((uint32_t)0, t1.Size);

	ASSERT_EQ((uint16_t)0, t1.RecordSourceId);
	ASSERT_EQ((uint16_t)0, t1.Track);
	ASSERT_EQ((uint16_t)0, t1.Bitrate);
	ASSERT_EQ((uint16_t)0, t1.GenreId);
	ASSERT_EQ((uint16_t)0, t1.RecordDeviceId);

	ASSERT_EQ((uint8_t)0, t1.DeviceType);
	ASSERT_EQ((uint8_t)0, t1.Page);
	ASSERT_EQ((uint8_t)0, t1.Channels);
	ASSERT_EQ((uint8_t)0, t1.Quality);
	ASSERT_EQ((uint8_t)0, t1.Rating);
	ASSERT_EQ((uint8_t)0, t1.Flags);
	ASSERT_EQ((uint8_t)0, t1.Key);
	ASSERT_EQ((uint8_t)0, t1.EnergyLevel);

	ASSERT_EQ((size_t)0, t1.CoverPreview.size());
}

TEST_F(DataTitleTest, Setter) {
	DataTitle t1;
	t1.SetTitle("Title");
	t1.SetArtist("Artist");
	t1.SetRemarks("Remarks");
	t1.SetTags("Tags");
	t1.SetAlbum("Album");

	ASSERT_EQ(ppl7::String("Title"), t1.Title);
	ASSERT_EQ(ppl7::String("Artist"), t1.Artist);
	ASSERT_EQ(ppl7::String("Remarks"), t1.Remarks);
	ASSERT_EQ(ppl7::String("Album"), t1.Album);
	ASSERT_EQ(ppl7::String("Tags"), t1.Tags);
}

static void getDefaultTitle(DataTitle& t1)
{
	t1.SetTitle("Title");
	t1.SetArtist("Artist");
	t1.SetRemarks("Remarks");
	t1.SetTags("Tags");
	t1.SetAlbum("Album");
	t1.TitleId=1234000;
	t1.DeviceId=400;
	t1.Length=9641;
	t1.VersionId=4711;
	t1.LabelId=6431;
	t1.BPM=138;
	t1.RecordDate=20220530;
	t1.ReleaseDate=20220527;
	t1.ImportData=7;
	t1.Size=154564744;
	t1.RecordSourceId=756;
	t1.Track=16;
	t1.Bitrate=320;
	t1.GenreId=24;
	t1.RecordDeviceId=17;
	t1.DeviceType=3;
	t1.Page=1;
	t1.Channels=2;
	t1.Quality=8;
	t1.Rating=46;
	t1.Flags=124;
	t1.Key=37;
	t1.EnergyLevel=78;
	ppl7::File::load(t1.CoverPreview, "testdata/testcover.png");

}

TEST_F(DataTitleTest, ExportImport) {
	DataTitle t1;
	getDefaultTitle(t1);
	ppl7::ByteArray bin;
	t1.Export(bin);
	ASSERT_EQ((size_t)2961, bin.size());
	ASSERT_EQ(5, t1.GetFormatVersion());


	DataTitle t2;
	t2.Import(bin, t1.GetFormatVersion());
	ASSERT_EQ(t1.Artist, t2.Artist);
	ASSERT_EQ(t1.Title, t2.Title);
	ASSERT_EQ(t1.Remarks, t2.Remarks);
	ASSERT_EQ(t1.Album, t2.Album);
	ASSERT_EQ(t1.Tags, t2.Tags);
	ASSERT_EQ(t1.CoverPreview, t2.CoverPreview);
	ASSERT_EQ(t1.TitleId, t2.TitleId);
	ASSERT_EQ(t1.DeviceId, t2.DeviceId);
	ASSERT_EQ(t1.Length, t2.Length);
	ASSERT_EQ(t1.VersionId, t2.VersionId);
	ASSERT_EQ(t1.LabelId, t2.LabelId);
	ASSERT_EQ(t1.BPM, t2.BPM);
	ASSERT_EQ(t1.RecordDate, t2.RecordDate);
	ASSERT_EQ(t1.ReleaseDate, t2.ReleaseDate);
	ASSERT_EQ(t1.ImportData, t2.ImportData);
	ASSERT_EQ(t1.Size, t2.Size);
	ASSERT_EQ(t1.RecordSourceId, t2.RecordSourceId);
	ASSERT_EQ(t1.Track, t2.Track);
	ASSERT_EQ(t1.Bitrate, t2.Bitrate);
	ASSERT_EQ(t1.GenreId, t2.GenreId);
	ASSERT_EQ(t1.RecordDeviceId, t2.RecordDeviceId);
	ASSERT_EQ(t1.DeviceType, t2.DeviceType);
	ASSERT_EQ(t1.Page, t2.Page);
	ASSERT_EQ(t1.Channels, t2.Channels);
	ASSERT_EQ(t1.Quality, t2.Quality);
	ASSERT_EQ(t1.Rating, t2.Rating);
	ASSERT_EQ(t1.Flags, t2.Flags);
	ASSERT_EQ(t1.Key, t2.Key);
	ASSERT_EQ(t1.EnergyLevel, t2.EnergyLevel);
}

TEST_F(DataTitleTest, CopyConstructor) {
	DataTitle t1;
	getDefaultTitle(t1);
	DataTitle t2(t1);
	ASSERT_EQ(t1.Artist, t2.Artist);
	ASSERT_EQ(t1.Title, t2.Title);
	ASSERT_EQ(t1.Remarks, t2.Remarks);
	ASSERT_EQ(t1.Album, t2.Album);
	ASSERT_EQ(t1.Tags, t2.Tags);
	ASSERT_EQ(t1.CoverPreview, t2.CoverPreview);
	ASSERT_EQ(t1.TitleId, t2.TitleId);
	ASSERT_EQ(t1.DeviceId, t2.DeviceId);
	ASSERT_EQ(t1.Length, t2.Length);
	ASSERT_EQ(t1.VersionId, t2.VersionId);
	ASSERT_EQ(t1.LabelId, t2.LabelId);
	ASSERT_EQ(t1.BPM, t2.BPM);
	ASSERT_EQ(t1.RecordDate, t2.RecordDate);
	ASSERT_EQ(t1.ReleaseDate, t2.ReleaseDate);
	ASSERT_EQ(t1.ImportData, t2.ImportData);
	ASSERT_EQ(t1.Size, t2.Size);
	ASSERT_EQ(t1.RecordSourceId, t2.RecordSourceId);
	ASSERT_EQ(t1.Track, t2.Track);
	ASSERT_EQ(t1.Bitrate, t2.Bitrate);
	ASSERT_EQ(t1.GenreId, t2.GenreId);
	ASSERT_EQ(t1.RecordDeviceId, t2.RecordDeviceId);
	ASSERT_EQ(t1.DeviceType, t2.DeviceType);
	ASSERT_EQ(t1.Page, t2.Page);
	ASSERT_EQ(t1.Channels, t2.Channels);
	ASSERT_EQ(t1.Quality, t2.Quality);
	ASSERT_EQ(t1.Rating, t2.Rating);
	ASSERT_EQ(t1.Flags, t2.Flags);
	ASSERT_EQ(t1.Key, t2.Key);
	ASSERT_EQ(t1.EnergyLevel, t2.EnergyLevel);
}

TEST_F(DataTitleTest, CopyOperator) {
	DataTitle t1;
	getDefaultTitle(t1);
	DataTitle t2;

	t2=t1;
	ASSERT_EQ(t1.Artist, t2.Artist);
	ASSERT_EQ(t1.Title, t2.Title);
	ASSERT_EQ(t1.Remarks, t2.Remarks);
	ASSERT_EQ(t1.Album, t2.Album);
	ASSERT_EQ(t1.Tags, t2.Tags);
	ASSERT_EQ(t1.CoverPreview, t2.CoverPreview);
	ASSERT_EQ(t1.TitleId, t2.TitleId);
	ASSERT_EQ(t1.DeviceId, t2.DeviceId);
	ASSERT_EQ(t1.Length, t2.Length);
	ASSERT_EQ(t1.VersionId, t2.VersionId);
	ASSERT_EQ(t1.LabelId, t2.LabelId);
	ASSERT_EQ(t1.BPM, t2.BPM);
	ASSERT_EQ(t1.RecordDate, t2.RecordDate);
	ASSERT_EQ(t1.ReleaseDate, t2.ReleaseDate);
	ASSERT_EQ(t1.ImportData, t2.ImportData);
	ASSERT_EQ(t1.Size, t2.Size);
	ASSERT_EQ(t1.RecordSourceId, t2.RecordSourceId);
	ASSERT_EQ(t1.Track, t2.Track);
	ASSERT_EQ(t1.Bitrate, t2.Bitrate);
	ASSERT_EQ(t1.GenreId, t2.GenreId);
	ASSERT_EQ(t1.RecordDeviceId, t2.RecordDeviceId);
	ASSERT_EQ(t1.DeviceType, t2.DeviceType);
	ASSERT_EQ(t1.Page, t2.Page);
	ASSERT_EQ(t1.Channels, t2.Channels);
	ASSERT_EQ(t1.Quality, t2.Quality);
	ASSERT_EQ(t1.Rating, t2.Rating);
	ASSERT_EQ(t1.Flags, t2.Flags);
	ASSERT_EQ(t1.Key, t2.Key);
	ASSERT_EQ(t1.EnergyLevel, t2.EnergyLevel);
}

TEST_F(DataTitleTest, EqualOperator) {
	DataTitle t1;
	getDefaultTitle(t1);
	DataTitle t2;

	ASSERT_NE(t1, t2);
	t2=t1;
	ASSERT_EQ(t1, t2);
}

TEST_F(DataTitleTest, Clear) {
	DataTitle t1;
	getDefaultTitle(t1);
	DataTitle t2;

	t1.Clear();
	ASSERT_EQ(t1, t2);
}


static void PrepareCut(CStorage& storage, CTitleStore& store, const ppl7::String db_file=ppl7::String("winmusik.dat"))
{
	storage.Init("tmp/title", db_file);
	storage.RegisterStorageClass(&store);
	storage.DeleteDatabase();
}


TEST_F(StorageTitleTest, PutGet) {
	CTitleStore store;
	CStorage storage;
	PrepareCut(storage, store, "PutGet.dat");
	DataTitle t1;
	getDefaultTitle(t1);
	t1.TitleId=0;
	uint32_t id=store.Put(t1);
	ASSERT_EQ((uint32_t)1, id);

	const DataTitle& t2=store.Get(1);
	ASSERT_EQ(t1.Artist, t2.Artist);
	ASSERT_EQ(t1.Title, t2.Title);
	ASSERT_EQ(t1.Remarks, t2.Remarks);
	ASSERT_EQ(t1.Album, t2.Album);
	ASSERT_EQ(t1.Tags, t2.Tags);
	ASSERT_EQ(t1.CoverPreview, t2.CoverPreview);
	ASSERT_EQ((uint32_t)1, t2.TitleId);
	ASSERT_EQ(t1.DeviceId, t2.DeviceId);
	ASSERT_EQ(t1.Length, t2.Length);
	ASSERT_EQ(t1.VersionId, t2.VersionId);
	ASSERT_EQ(t1.LabelId, t2.LabelId);
	ASSERT_EQ(t1.BPM, t2.BPM);
	ASSERT_EQ(t1.RecordDate, t2.RecordDate);
	ASSERT_EQ(t1.ReleaseDate, t2.ReleaseDate);
	ASSERT_EQ(t1.ImportData, t2.ImportData);
	ASSERT_EQ(t1.Size, t2.Size);
	ASSERT_EQ(t1.RecordSourceId, t2.RecordSourceId);
	ASSERT_EQ(t1.Track, t2.Track);
	ASSERT_EQ(t1.Bitrate, t2.Bitrate);
	ASSERT_EQ(t1.GenreId, t2.GenreId);
	ASSERT_EQ(t1.RecordDeviceId, t2.RecordDeviceId);
	ASSERT_EQ(t1.DeviceType, t2.DeviceType);
	ASSERT_EQ(t1.Page, t2.Page);
	ASSERT_EQ(t1.Channels, t2.Channels);
	ASSERT_EQ(t1.Quality, t2.Quality);
	ASSERT_EQ(t1.Rating, t2.Rating);
	ASSERT_EQ(t1.Flags, t2.Flags);
	ASSERT_EQ(t1.Key, t2.Key);
	ASSERT_EQ(t1.EnergyLevel, t2.EnergyLevel);

	const DataTitle* t3=store.GetPtr(1);
	ASSERT_TRUE(t3 != NULL);

}

TEST_F(StorageTitleTest, PutOverwriteGet) {
	CTitleStore store;
	CStorage storage;
	PrepareCut(storage, store, "PutGet.dat");
	ASSERT_EQ((uint32_t)0, store.Capacity());
	ASSERT_EQ((uint32_t)0, store.Size());
	ASSERT_EQ((uint32_t)0, store.MaxId());
	DataTitle t1;
	getDefaultTitle(t1);
	t1.TitleId=0;
	ASSERT_EQ((size_t)0, store.Artists.size());
	uint32_t id=store.Put(t1);
	ASSERT_EQ((uint32_t)1, id);
	ASSERT_EQ((uint32_t)2, store.Put(t1));
	ASSERT_EQ((uint32_t)3, store.Put(t1));
	t1.TitleId=10;
	ASSERT_EQ((uint32_t)10, store.Put(t1));

	ASSERT_EQ((size_t)1, store.Artists.size());
	ASSERT_EQ((uint32_t)10001, store.Capacity());
	ASSERT_EQ((uint32_t)4, store.Size());
	ASSERT_TRUE(store.Exists(2));
	ASSERT_FALSE(store.Exists(4));
	ASSERT_FALSE(store.Exists(10002));
	ASSERT_FALSE(store.Exists(0));

	ASSERT_THROW(store.Get(4), RecordDoesNotExistException);



	DataTitle t2=store.Get(2);
	t2.Artist="Tsitra";
	t2.Title="Eltit";
	ASSERT_EQ((uint32_t)2, store.Put(t2));

	const DataTitle& t3=store.Get(2);

	ASSERT_EQ((uint32_t)10, store.MaxId());
	ASSERT_EQ((size_t)2, store.Artists.size());

	ASSERT_EQ(t2.GetFilePos(), t3.GetFilePos());

	store.Delete(1);
	store.Delete(3);
	store.Delete(10);

	ASSERT_EQ((uint32_t)10001, store.Capacity());
	ASSERT_EQ((uint32_t)1, store.Size());
	ASSERT_EQ((uint32_t)2, store.MaxId());


}



} // EOF namespace
