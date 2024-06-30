/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

#include "ppl7.h"
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class StorageTest : public ::testing::Test {
protected:
	StorageTest() {
		if (setlocale(LC_CTYPE, DEFAULT_LOCALE) == NULL) {
			printf("setlocale fehlgeschlagen\n");
			throw std::exception();
		}
	}
	~StorageTest()
	{

	}
};

TEST_F(StorageTest, ConstructorWithoutParam) {
	ASSERT_NO_THROW({
		CStorage storage;
		});
}

TEST_F(StorageTest, loadDatabase) {
	CTitleStore titlestore;
	CDeviceStore devicestore;
	CTrackStore trackstore;
	CShortcutStore shortcutstore;
	CVersionStore versionstore;
	CRecordSourceStore recordsourcestore;
	CLabelStore labelstore;
	CPurchaseSourceStore purchasesourcestore;
	CRecordDeviceStore recorddevicestore;
	CGenreStore genrestore;
	CStorage storage;

	storage.RegisterStorageClass(&titlestore);
	storage.RegisterStorageClass(&devicestore);
	storage.RegisterStorageClass(&trackstore);
	storage.RegisterStorageClass(&shortcutstore);
	storage.RegisterStorageClass(&versionstore);
	storage.RegisterStorageClass(&recordsourcestore);
	storage.RegisterStorageClass(&labelstore);
	storage.RegisterStorageClass(&purchasesourcestore);
	storage.RegisterStorageClass(&recorddevicestore);
	storage.RegisterStorageClass(&genrestore);

	ppl7::File::copy("testdata/winmusik.dat", "tmp/loadtest.dat");
	storage.Init("tmp", "loadtest.dat");
	storage.LoadDatabase();

	ASSERT_EQ((uint32_t)22,
		static_cast<CTitleStore&>(storage.GetStorageClass("TITL")).Size());

	/*
	for (uint32_t i=1;i < titlestore.MaxId();i++) {
		const DataTitle* ti=titlestore.GetPtr(i);
		if (ti) {
			printf("%s - %s (%s)\n", (const char*)ti->Artist, (const char*)ti->Title,
				(const char*)versionstore.GetValue(ti->VersionId));
		}
	}
	*/

	ASSERT_EQ((uint32_t)16,
		static_cast<CVersionStore&>(storage.GetStorageClass("VERS")).Size());
	ASSERT_EQ((uint32_t)148,
		static_cast<CGenreStore&>(storage.GetStorageClass("GENR")).Size());
	ASSERT_EQ((uint32_t)0,
		static_cast<CRecordSourceStore&>(storage.GetStorageClass("RSRC")).Size());
	ASSERT_EQ((uint32_t)7,
		static_cast<CLabelStore&>(storage.GetStorageClass("LABL")).Size());
	ASSERT_EQ((uint32_t)0,
		static_cast<CPurchaseSourceStore&>(storage.GetStorageClass("PCHS")).Size());
	ASSERT_EQ((uint32_t)0,
		static_cast<CRecordDeviceStore&>(storage.GetStorageClass("RDEV")).Size());

	ASSERT_EQ((uint32_t)2,
		static_cast<CShortcutStore&>(storage.GetStorageClass("SHRT")).Size());

	storage.DeleteDatabase();



}

TEST_F(StorageTest, loadRealDatabase) {
	CTitleStore titlestore;
	CDeviceStore devicestore;
	CTrackStore trackstore;
	CShortcutStore shortcutstore;
	CVersionStore versionstore;
	CRecordSourceStore recordsourcestore;
	CLabelStore labelstore;
	CPurchaseSourceStore purchasesourcestore;
	CRecordDeviceStore recorddevicestore;
	CGenreStore genrestore;
	CStorage storage;

	storage.RegisterStorageClass(&titlestore);
	storage.RegisterStorageClass(&devicestore);
	storage.RegisterStorageClass(&trackstore);
	storage.RegisterStorageClass(&shortcutstore);
	storage.RegisterStorageClass(&versionstore);
	storage.RegisterStorageClass(&recordsourcestore);
	storage.RegisterStorageClass(&labelstore);
	storage.RegisterStorageClass(&purchasesourcestore);
	storage.RegisterStorageClass(&recorddevicestore);
	storage.RegisterStorageClass(&genrestore);


	if (ppl7::File::exists("/home/patrickf/Nextcloud/WinMusik3/winmusik.dat")) {

		ppl7::File::copy("/home/patrickf/Nextcloud/WinMusik3/winmusik.dat", "tmp/loadtest.dat");
		storage.Init("tmp", "loadtest.dat");
		storage.LoadDatabase();
		storage.DeleteDatabase();
	}



}

} // EOF namespace
