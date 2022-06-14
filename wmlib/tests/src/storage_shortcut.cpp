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
#include "wm_storage.h"
#include "libwinmusik3.h"
#include "wm_playlist.h"
#include "wm_exceptions.h"
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class StorageShortcutTest : public ::testing::Test {
protected:
	StorageShortcutTest() {
		ppl7::Dir::mkDir("tmp/shortcut", true);

	}
	~StorageShortcutTest()
	{

	}
};


TEST_F(StorageShortcutTest, DataObject) {
	DataShortcut t1;
	t1.SetValue("psb", "Pet Shop Boys");

	DataShortcut t2;
	t2=t1;
	ppl7::ByteArray bin;

	t2.Export(bin);
	//bin.hexDump();
	ASSERT_EQ((size_t)20, bin.size());

	DataShortcut t3;
	t3.Import(bin, 1);
	ASSERT_EQ(ppl7::String("psb"), t3.shortcut);
	ASSERT_EQ(ppl7::String("Pet Shop Boys"), t3.artist);
}

TEST_F(StorageShortcutTest, PutAndGet) {
	CShortcutStore store;
	CStorage storage;
	//PrepareCut(storage, store, "PutAndGet.dat");
	storage.Init("tmp/shortcut", "PutAndGet.dat");
	storage.RegisterStorageClass(&store);
	storage.DeleteDatabase();
	try {
		store.Put(DataShortcut("psb", "Pet Shop Boys"));
		store.Put(DataShortcut("oMd", "Orchestral Manoeuvres in the Dark"));
		store.Put(DataShortcut("elo", "Electric Light Orchestra"));
	} catch (const ppl7::Exception& exp) {
		exp.print();
		throw;
	}
	ASSERT_EQ((size_t)3, store.Size());

	ASSERT_THROW(store.Get("abc"), RecordDoesNotExistException);
	ASSERT_EQ(ppl7::String("Orchestral Manoeuvres in the Dark"), store.Get("OMD").artist);
	ASSERT_EQ(NULL, store.GetPtr("abc"));

	// Overwrite
	store.Put(DataShortcut("psb", "Pet Boys Shop"));
	ASSERT_EQ((size_t)3, store.Size());
	ASSERT_EQ(ppl7::String("Pet Boys Shop"), store.Get("psb").artist);
}



} // EOF namespace
