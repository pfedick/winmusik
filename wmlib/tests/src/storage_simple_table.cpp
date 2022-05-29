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
#include "../include/wm_storage.h"
#include "wm_playlist.h"
#include "wm_exceptions.h"
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class StorageSimpleTableTest : public ::testing::Test {
protected:
	StorageSimpleTableTest() {
		ppl7::Dir::mkDir("tmp/simpletable", true);

	}
	~StorageSimpleTableTest()
	{

	}
};

static void AddDefaults(CVersionStore& vstore)
{
	vstore.Put(CSimpleTable(1, "Single", 100));
	vstore.FindOrAdd("Single");
	vstore.Put(CSimpleTable(2, "Maxi", 5));
	vstore.FindOrAdd("Extended Version");
	vstore.FindOrAdd("Extended Mix");
	vstore.FindOrAdd("Extended Mix");
	vstore.FindOrAdd("12\" Version");
	vstore.FindOrAdd("Maxi");
	vstore.FindOrAdd("Album Version");
	vstore.FindOrAdd("Radio Edit");
	vstore.FindOrAdd("Original Version");
	vstore.FindOrAdd("Original Mix");
	vstore.FindOrAdd("Radio Mix");
	vstore.Put(CSimpleTable(16, "Test Mix 1"));
	vstore.FindOrAdd("Test Mix 2");
	vstore.Put(CSimpleTable(20, "Delete Test 1"));
	vstore.Put(CSimpleTable(21, "Delete Test 2"));
	vstore.Put(CSimpleTable(22, "No Delete Test 3"));
}

static void PrepareCut(CStorage& storage, CVersionStore& vstore, const ppl7::String db_file=ppl7::String("winmusik.dat"))
{
	storage.Init("tmp/simpletable", db_file);
	storage.RegisterStorageClass(&vstore);
	storage.DeleteDatabase();
	AddDefaults(vstore);
}

TEST_F(StorageSimpleTableTest, DataObject) {
	CSimpleTable t1;
	t1.Id=1;
	t1.References=10;
	t1.Value.set("Test");

	CSimpleTable t2;
	t2=t1;
	ppl7::ByteArray bin;

	t2.Export(bin);
	ASSERT_EQ((size_t)13, bin.size());

	CSimpleTable t3;
	t3.Import(bin, 1);
	ASSERT_EQ((uint32_t)1, t3.Id);
	ASSERT_EQ((uint32_t)10, t3.References);
	ASSERT_EQ(ppl7::String("Test"), t3.Value);
}

TEST_F(StorageSimpleTableTest, PutAndFindOrAdd) {
	CStorage storage;
	storage.Init("tmp/simpletable", "PutAndFindOrAdd.dat");
	CVersionStore vstore;
	storage.RegisterStorageClass(&vstore);
	storage.DeleteDatabase();
	try {
		ASSERT_EQ((uint32_t)1, vstore.Put(CSimpleTable(1, "Single", 1)));
		ASSERT_EQ((uint32_t)1, vstore.FindOrAdd("Single"));
		ASSERT_EQ((uint32_t)2, vstore.Put(CSimpleTable(2, "Maxi", 5)));
		ASSERT_EQ((uint32_t)3, vstore.FindOrAdd("Extended Version"));
		vstore.FindOrAdd("Extended Mix");
		vstore.FindOrAdd("Extended Mix");
		vstore.FindOrAdd("12\" Version");
		ASSERT_EQ((uint32_t)2, vstore.FindOrAdd("Maxi"));
		vstore.FindOrAdd("Album Version");
		vstore.FindOrAdd("Radio Edit");
		vstore.FindOrAdd("Original Version");
		vstore.FindOrAdd("Original Mix");
		vstore.FindOrAdd("Radio Mix");
		ASSERT_EQ((uint32_t)16, vstore.Put(CSimpleTable(16, "Test Mix 1")));
		ASSERT_EQ((uint32_t)17, vstore.FindOrAdd("Test Mix 2"));
	} catch (const ppl7::Exception& exp) {
		exp.print();
		throw;
	}
	ASSERT_EQ((uint32_t)17, vstore.MaxId());
	ASSERT_EQ((uint32_t)10001, vstore.Capacity());
	ASSERT_EQ((uint32_t)12, vstore.Size());
}

TEST_F(StorageSimpleTableTest, Delete) {
	CStorage storage;
	CVersionStore vstore;
	PrepareCut(storage, vstore, "Delete.dat");
	ASSERT_EQ((uint32_t)15, vstore.Size());
	ASSERT_EQ((uint32_t)22, vstore.MaxId());
	vstore.Delete(22);
	ASSERT_EQ((uint32_t)14, vstore.Size());
	ASSERT_EQ((uint32_t)21, vstore.MaxId());
	vstore.Delete(21);
	vstore.Delete(20);
	ASSERT_EQ((uint32_t)12, vstore.Size());
	ASSERT_EQ((uint32_t)17, vstore.MaxId());

	ASSERT_TRUE(NULL == vstore.GetPtr(22));
	ASSERT_TRUE(NULL == vstore.GetPtr(21));
	ASSERT_TRUE(NULL == vstore.GetPtr(20));
	ASSERT_TRUE(NULL != vstore.GetPtr(17));
}

TEST_F(StorageSimpleTableTest, Get) {
	CStorage storage;
	CVersionStore vstore;
	PrepareCut(storage, vstore);

	const CSimpleTable& t1=vstore.Get(1);
	ASSERT_EQ(ppl7::String("Single"), t1.Value);
	ASSERT_EQ((uint32_t)100, t1.References);
	const CSimpleTable& t2=vstore.Get(2);
	ASSERT_EQ(ppl7::String("Maxi"), t2.Value);
	ASSERT_EQ((uint32_t)5, t2.References);

	ASSERT_THROW(vstore.Get(30), RecordDoesNotExistException);

	ASSERT_EQ(NULL, vstore.GetPtr(30));


}

TEST_F(StorageSimpleTableTest, GetId) {
	CStorage storage;
	CVersionStore vstore;
	PrepareCut(storage, vstore);
	ASSERT_EQ((uint32_t)0, vstore.GetId("Unknown Entry"));
	ASSERT_EQ((uint32_t)1, vstore.GetId("Single"));
	ASSERT_EQ((uint32_t)2, vstore.GetId("Maxi"));
	ASSERT_EQ((uint32_t)4, vstore.GetId("Extended Mix"));
}

TEST_F(StorageSimpleTableTest, Find) {
	CStorage storage;
	CVersionStore vstore;
	PrepareCut(storage, vstore);
	const CSimpleTable* t;
	t=vstore.Find("Unknown Entry");
	ASSERT_EQ(NULL, t);
	t=vstore.Find("Extended Mix");
	ASSERT_FALSE(t == NULL);
	ASSERT_EQ((uint32_t)4, t->Id);
	ASSERT_EQ(ppl7::String("Extended Mix"), t->Value);
}

TEST_F(StorageSimpleTableTest, FindAll) {
	CStorage storage;
	CVersionStore vstore;
	PrepareCut(storage, vstore);
	CVersionStore::IndexTree Result;
	ASSERT_EQ((size_t)5, vstore.FindAll("Mix", Result));
	//4, 9, 10, 16, 17
	/*
	CVersionStore::IndexTree::const_iterator it;
	for (it=Result.begin();it != Result.end();++it) {
		printf("found: %d = %s\n", (*it), (const char*)vstore.GetValue((*it)));
	}
	*/

	ASSERT_TRUE(Result.find(4) != Result.end());
	ASSERT_TRUE(Result.find(9) != Result.end());
	ASSERT_TRUE(Result.find(10) != Result.end());
	ASSERT_TRUE(Result.find(16) != Result.end());
	ASSERT_TRUE(Result.find(17) != Result.end());
}


TEST_F(StorageSimpleTableTest, Rename) {
	CStorage storage;
	CVersionStore vstore;
	PrepareCut(storage, vstore, "Rename.dat");
	CSimpleTable t=vstore.Get(4);
	t.Value="Extended Cut";
	vstore.Put(t);
	CVersionStore::IndexTree Result;
	ASSERT_EQ((size_t)4, vstore.FindAll("Mix", Result));
	ASSERT_EQ(NULL, vstore.Find("Extended Mix"));

	t=vstore.Get(2);
	t.Value="Maxi Version";
	vstore.Put(t);
	ASSERT_TRUE(NULL != vstore.Find("Maxi Version"));
	ASSERT_EQ((uint32_t)2, vstore.GetId("Maxi Version"));
	ASSERT_EQ((uint32_t)0, vstore.GetId("Maxi"));

}

TEST_F(StorageSimpleTableTest, FindWords) {
	CStorage storage;
	CVersionStore vstore;
	PrepareCut(storage, vstore);
	vstore.Put(CSimpleTable(19, "Test Mix 1"));
	vstore.Put(CSimpleTable(23, "Delete Mix 1"));
	CVersionStore::IndexTree Result;
	ASSERT_EQ((size_t)3, vstore.FindWords("Test Delete", Result));
	/*
	CVersionStore::IndexTree::const_iterator it;
	for (it=Result.begin();it != Result.end();++it) {
		printf("found: %d = %s\n", (*it), (const char*)vstore.GetValue((*it)));
	}
	*/

	ASSERT_TRUE(Result.find(20) != Result.end());
	ASSERT_TRUE(Result.find(21) != Result.end());
	ASSERT_TRUE(Result.find(22) != Result.end());
}








} // EOF namespace
