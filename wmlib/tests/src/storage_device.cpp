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


class StorageDeviceTest : public ::testing::Test {
protected:
	StorageDeviceTest() {
		ppl7::Dir::mkDir("tmp/device", true);

	}
	~StorageDeviceTest()
	{

	}
};

TEST_F(StorageDeviceTest, DataDevice_CopyFrom) {
	DataDevice d1;
	d1.Title="Title";
	d1.SubTitle="SubTitle";
	d1.DeviceId=1;
	d1.Length=2;
	d1.Recorded=3;
	d1.LabelId=4;
	d1.PurchaseDate=5;
	d1.DateCreated=6;
	d1.FirstDate=7;
	d1.LastDate=8;
	d1.PurchasePrice=9.65f;
	d1.PurchaseId=10;
	d1.DeviceType=11;
	d1.Pages=12;


	DataDevice d2;
	d2.CopyFrom(d1);
	ASSERT_EQ(d1.Title, d2.Title);
	ASSERT_EQ(d1.SubTitle, d2.SubTitle);
	ASSERT_EQ(d1.DeviceId, d2.DeviceId);
	ASSERT_EQ(d1.Length, d2.Length);
	ASSERT_EQ(d1.Recorded, d2.Recorded);
	ASSERT_EQ(d1.LabelId, d2.LabelId);
	ASSERT_EQ(d1.PurchaseDate, d2.PurchaseDate);
	ASSERT_EQ(d1.DateCreated, d2.DateCreated);
	ASSERT_EQ(d1.FirstDate, d2.FirstDate);
	ASSERT_EQ(d1.LastDate, d2.LastDate);
	ASSERT_EQ(d1.PurchasePrice, d2.PurchasePrice);
	ASSERT_EQ(d1.PurchaseId, d2.PurchaseId);
	ASSERT_EQ(d1.DeviceType, d2.DeviceType);
	ASSERT_EQ(d1.Pages, d2.Pages);
}

TEST_F(StorageDeviceTest, DataDevice_Constructor) {
	DataDevice d1;
	ASSERT_TRUE(d1.Title.isEmpty());
	ASSERT_TRUE(d1.SubTitle.isEmpty());
	ASSERT_EQ((uint32_t)0, d1.DeviceId);
	ASSERT_EQ((uint32_t)0, d1.Length);
	ASSERT_EQ((uint32_t)0, d1.Recorded);
	ASSERT_EQ((uint32_t)0, d1.LabelId);
	ASSERT_EQ((uint32_t)0, d1.PurchaseDate);
	ASSERT_EQ((uint32_t)0, d1.DateCreated);
	ASSERT_EQ((uint32_t)0, d1.FirstDate);
	ASSERT_EQ((uint32_t)0, d1.LastDate);
	ASSERT_EQ((uint32_t)0, d1.NumTracks);
	ASSERT_EQ((float)0.0f, d1.PurchasePrice);
	ASSERT_EQ((uint16_t)0, d1.PurchaseId);
	ASSERT_EQ((uint8_t)0, d1.DeviceType);
	ASSERT_EQ((uint8_t)0, d1.Pages);
}

TEST_F(StorageDeviceTest, DataDevice_Clear) {
	DataDevice d1;
	d1.Title="Title";
	d1.SubTitle="SubTitle";
	d1.DeviceId=1;
	d1.Length=2;
	d1.Recorded=3;
	d1.LabelId=4;
	d1.PurchaseDate=5;
	d1.DateCreated=6;
	d1.FirstDate=7;
	d1.LastDate=8;
	d1.PurchasePrice=9.65f;
	d1.PurchaseId=10;
	d1.DeviceType=11;
	d1.Pages=12;
	d1.Clear();
	ASSERT_TRUE(d1.Title.isEmpty());
	ASSERT_TRUE(d1.SubTitle.isEmpty());
	ASSERT_EQ((uint32_t)0, d1.DeviceId);
	ASSERT_EQ((uint32_t)0, d1.Length);
	ASSERT_EQ((uint32_t)0, d1.Recorded);
	ASSERT_EQ((uint32_t)0, d1.LabelId);
	ASSERT_EQ((uint32_t)0, d1.PurchaseDate);
	ASSERT_EQ((uint32_t)0, d1.DateCreated);
	ASSERT_EQ((uint32_t)0, d1.FirstDate);
	ASSERT_EQ((uint32_t)0, d1.LastDate);
	ASSERT_EQ((uint32_t)0, d1.NumTracks);
	ASSERT_EQ((float)0.0f, d1.PurchasePrice);
	ASSERT_EQ((uint16_t)0, d1.PurchaseId);
	ASSERT_EQ((uint8_t)0, d1.DeviceType);
	ASSERT_EQ((uint8_t)0, d1.Pages);
}


TEST_F(StorageDeviceTest, DataDevice_CopyConstructor) {
	DataDevice d1;
	d1.Title="Title";
	d1.SubTitle="SubTitle";
	d1.DeviceId=1;
	d1.Length=2;
	d1.Recorded=3;
	d1.LabelId=4;
	d1.PurchaseDate=5;
	d1.DateCreated=6;
	d1.FirstDate=7;
	d1.LastDate=8;
	d1.PurchasePrice=9.65f;
	d1.PurchaseId=10;
	d1.DeviceType=11;
	d1.Pages=12;


	DataDevice d2(d1);
	ASSERT_EQ(d1.Title, d2.Title);
	ASSERT_EQ(d1.SubTitle, d2.SubTitle);
	ASSERT_EQ(d1.DeviceId, d2.DeviceId);
	ASSERT_EQ(d1.Length, d2.Length);
	ASSERT_EQ(d1.Recorded, d2.Recorded);
	ASSERT_EQ(d1.LabelId, d2.LabelId);
	ASSERT_EQ(d1.PurchaseDate, d2.PurchaseDate);
	ASSERT_EQ(d1.DateCreated, d2.DateCreated);
	ASSERT_EQ(d1.FirstDate, d2.FirstDate);
	ASSERT_EQ(d1.LastDate, d2.LastDate);
	ASSERT_EQ(d1.PurchasePrice, d2.PurchasePrice);
	ASSERT_EQ(d1.PurchaseId, d2.PurchaseId);
	ASSERT_EQ(d1.DeviceType, d2.DeviceType);
	ASSERT_EQ(d1.Pages, d2.Pages);
}

TEST_F(StorageDeviceTest, DataDevice_CopyAssignment) {
	DataDevice d1;
	d1.Title="Title";
	d1.SubTitle="SubTitle";
	d1.DeviceId=1;
	d1.Length=2;
	d1.Recorded=3;
	d1.LabelId=4;
	d1.PurchaseDate=5;
	d1.DateCreated=6;
	d1.FirstDate=7;
	d1.LastDate=8;
	d1.PurchasePrice=9.65f;
	d1.PurchaseId=10;
	d1.DeviceType=11;
	d1.Pages=12;


	DataDevice d2;
	d2=d1;
	ASSERT_EQ(d1.Title, d2.Title);
	ASSERT_EQ(d1.SubTitle, d2.SubTitle);
	ASSERT_EQ(d1.DeviceId, d2.DeviceId);
	ASSERT_EQ(d1.Length, d2.Length);
	ASSERT_EQ(d1.Recorded, d2.Recorded);
	ASSERT_EQ(d1.LabelId, d2.LabelId);
	ASSERT_EQ(d1.PurchaseDate, d2.PurchaseDate);
	ASSERT_EQ(d1.DateCreated, d2.DateCreated);
	ASSERT_EQ(d1.FirstDate, d2.FirstDate);
	ASSERT_EQ(d1.LastDate, d2.LastDate);
	ASSERT_EQ(d1.PurchasePrice, d2.PurchasePrice);
	ASSERT_EQ(d1.PurchaseId, d2.PurchaseId);
	ASSERT_EQ(d1.DeviceType, d2.DeviceType);
	ASSERT_EQ(d1.Pages, d2.Pages);
}

TEST_F(StorageDeviceTest, DataDevice_ExportImport) {
	DataDevice d1;
	d1.SetTitle("Title");
	d1.SetSubTitle("SubTitle");
	d1.DeviceId=1;
	d1.Length=2;
	d1.Recorded=3;
	d1.LabelId=4;
	d1.PurchaseDate=5;
	d1.DateCreated=6;
	d1.FirstDate=7;
	d1.LastDate=8;
	d1.PurchasePrice=9.65f;
	d1.PurchaseId=10;
	d1.DeviceType=11;
	d1.Pages=12;

	ppl7::ByteArray bin;
	d1.Export(bin);
	ASSERT_EQ((size_t)61, bin.size());
	ASSERT_EQ((uint8_t)1, d1.GetFormatVersion());

	DataDevice d2;
	d2.Import(bin, d1.GetFormatVersion());
	ASSERT_EQ(ppl7::String("Title"), d2.GetTitle());
	ASSERT_EQ(ppl7::String("SubTitle"), d2.GetSubTitle());
	ASSERT_EQ(d1.DeviceId, d2.DeviceId);
	ASSERT_EQ(d1.Length, d2.Length);
	ASSERT_EQ(d1.Recorded, d2.Recorded);
	ASSERT_EQ(d1.LabelId, d2.LabelId);
	ASSERT_EQ(d1.PurchaseDate, d2.PurchaseDate);
	ASSERT_EQ(d1.DateCreated, d2.DateCreated);
	ASSERT_EQ(d1.FirstDate, d2.FirstDate);
	ASSERT_EQ(d1.LastDate, d2.LastDate);
	ASSERT_EQ(d1.PurchasePrice, d2.PurchasePrice);
	ASSERT_EQ(d1.PurchaseId, d2.PurchaseId);
	ASSERT_EQ(d1.DeviceType, d2.DeviceType);
	ASSERT_EQ(d1.Pages, d2.Pages);
}


TEST_F(StorageDeviceTest, PutGetExists) {
	CDeviceStore store;
	CStorage storage;
	storage.Init("tmp/device", "PutAndGet.dat");
	storage.RegisterStorageClass(&store);
	storage.DeleteDatabase();

	DataDevice d1;
	d1.SetTitle("Title 1");
	d1.SetSubTitle("SubTitle 1");
	d1.DeviceType=1;
	d1.DeviceId=1;

	d1.Length=2;
	d1.Recorded=3;
	d1.LabelId=4;
	d1.PurchaseDate=5;
	d1.DateCreated=6;
	d1.FirstDate=7;
	d1.LastDate=8;
	d1.PurchasePrice=9.65f;
	d1.PurchaseId=10;
	d1.Pages=1;
	{
		const DataDevice& dnew=store.Put(d1);
		ASSERT_EQ((uint8_t)1, dnew.DeviceType);
		ASSERT_EQ((uint32_t)1, dnew.DeviceId);
	}

	d1.DeviceId=0;
	d1.SetTitle("Title 2");
	d1.SetSubTitle("SubTitle 2");
	{
		const DataDevice& dnew=store.Put(d1);
		ASSERT_EQ((uint8_t)1, dnew.DeviceType);
		ASSERT_EQ((uint32_t)2, dnew.DeviceId);
	}
	d1.DeviceId=10;
	d1.DeviceType=2;
	d1.SetTitle("DeviceType 2, Title 10");
	d1.SetSubTitle("SubTitle");
	{
		const DataDevice& dnew=store.Put(d1);
		ASSERT_EQ((uint8_t)2, dnew.DeviceType);
		ASSERT_EQ((uint32_t)10, dnew.DeviceId);
	}

	ASSERT_EQ((uint32_t)2, store.GetHighestDevice(1));
	ASSERT_EQ((uint32_t)10, store.GetHighestDevice(2));
	ASSERT_EQ((uint32_t)0, store.GetHighestDevice(3));

	ASSERT_TRUE(store.Exists(1, 1));
	ASSERT_TRUE(store.Exists(1, 2));
	ASSERT_FALSE(store.Exists(1, 3));
	ASSERT_TRUE(store.Exists(2, 10));
	ASSERT_FALSE(store.Exists(2, 1));

	{
		const DataDevice& found=store.Get(1, 1);
		ASSERT_EQ(ppl7::String("Title 1"), found.Title);
		ASSERT_EQ((uint32_t)1, found.DeviceId);
	}
	ASSERT_THROW(store.Get(1, 3), RecordDoesNotExistException);
	{
		const DataDevice& found=store.Get(2, 10);
		ASSERT_EQ(ppl7::String("DeviceType 2, Title 10"), found.Title);
		ASSERT_EQ((uint32_t)10, found.DeviceId);
	}
	{
		ASSERT_EQ(NULL, store.GetPtr(1, 3));
		const DataDevice* found=store.GetPtr(2, 10);
		ASSERT_EQ(ppl7::String("DeviceType 2, Title 10"), found->Title);
		ASSERT_EQ((uint32_t)10, found->DeviceId);
	}


}

TEST_F(StorageDeviceTest, Renumber) {
	CDeviceStore store;
	CStorage storage;
	storage.Init("tmp/device", "Renumber.dat");
	storage.RegisterStorageClass(&store);
	storage.DeleteDatabase();

	DataDevice d1;
	d1.SetTitle("Title 1");
	d1.SetSubTitle("SubTitle 1");
	d1.DeviceType=1;
	d1.DeviceId=1;
	store.Put(d1);

	d1.DeviceId=0;
	d1.SetTitle("Title 2");
	d1.SetSubTitle("SubTitle 2");
	store.Put(d1);

	d1.DeviceId=10;
	d1.DeviceType=2;
	d1.SetTitle("DeviceType 2, Title 10");
	d1.SetSubTitle("SubTitle");
	store.Put(d1);

	store.Renumber(1, 2, 5);
	ASSERT_TRUE(store.Exists(1, 1));
	ASSERT_FALSE(store.Exists(1, 2));
	ASSERT_TRUE(store.Exists(1, 5));
	ASSERT_TRUE(store.Exists(2, 10));
	ASSERT_EQ((uint32_t)5, store.GetHighestDevice(1));

	store.Renumber(1, 5, 4);
	ASSERT_TRUE(store.Exists(1, 4));
	ASSERT_FALSE(store.Exists(1, 5));
	ASSERT_EQ((uint32_t)4, store.GetHighestDevice(1));

	ASSERT_THROW(store.Renumber(1, 1, 4), DeviceAlreadyExistsException);
	ASSERT_THROW(store.Renumber(1, 3, 7), RecordDoesNotExistException);

}

TEST_F(StorageDeviceTest, Delete) {
	CDeviceStore store;
	CStorage storage;
	storage.Init("tmp/device", "Delete.dat");
	storage.RegisterStorageClass(&store);
	storage.DeleteDatabase();

	DataDevice d1;
	d1.SetTitle("Title 1");
	d1.SetSubTitle("SubTitle 1");
	d1.DeviceType=1;
	d1.DeviceId=1;
	store.Put(d1);

	d1.DeviceId=0;
	d1.SetTitle("Title 2");
	d1.SetSubTitle("SubTitle 2");
	store.Put(d1);

	d1.DeviceId=0;
	d1.SetTitle("Title 3");
	d1.SetSubTitle("SubTitle 3");
	store.Put(d1);

	d1.DeviceId=10;
	d1.DeviceType=2;
	d1.SetTitle("DeviceType 2, Title 10");
	d1.SetSubTitle("SubTitle");
	store.Put(d1);


	ASSERT_TRUE(store.Exists(1, 1));
	ASSERT_TRUE(store.Exists(1, 2));
	ASSERT_TRUE(store.Exists(1, 3));
	ASSERT_TRUE(store.Exists(2, 10));
	ASSERT_EQ((uint32_t)3, store.GetHighestDevice(1));

	store.Delete(1, 2);
	ASSERT_TRUE(store.Exists(1, 1));
	ASSERT_FALSE(store.Exists(1, 2));
	ASSERT_TRUE(store.Exists(1, 3));
	ASSERT_TRUE(store.Exists(2, 10));
	ASSERT_EQ((uint32_t)3, store.GetHighestDevice(1));

	store.Delete(1, 3);
	ASSERT_TRUE(store.Exists(1, 1));
	ASSERT_FALSE(store.Exists(1, 2));
	ASSERT_FALSE(store.Exists(1, 3));
	ASSERT_TRUE(store.Exists(2, 10));
	ASSERT_EQ((uint32_t)1, store.GetHighestDevice(1));

}

} // EOF namespace
