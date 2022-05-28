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
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class StorageSimpleTableTest : public ::testing::Test {
protected:
	StorageSimpleTableTest() {

	}
	~StorageSimpleTableTest()
	{

	}
};

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

TEST_F(StorageSimpleTableTest, Put) {
	CStorage storage;
	ppl7::Dir::mkDir("tmp");
	storage.Init("tmp");
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

}





} // EOF namespace
