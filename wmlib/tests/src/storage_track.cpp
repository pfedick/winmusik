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


class StorageTrackTest : public ::testing::Test {
protected:
	StorageTrackTest() {
		ppl7::Dir::mkDir("tmp/track", true);

	}
	~StorageTrackTest()
	{

	}
};

TEST_F(StorageTrackTest, DataTrack_Constructor) {
	DataTrack d1;
	ASSERT_EQ((uint32_t)0, d1.DeviceId);
	ASSERT_EQ((uint32_t)0, d1.TitleId);
	ASSERT_EQ((uint16_t)0, d1.Track);
	ASSERT_EQ((uint8_t)0, d1.Device);
	ASSERT_EQ((uint8_t)0, d1.Page);

}

TEST_F(StorageTrackTest, DataTrack_CopyFrom) {
	DataTrack d1;
	d1.SetValue(1, 2, 3, 4, 1234);

	DataTrack d2;
	d2.CopyFrom(d1);
	ASSERT_EQ((uint32_t)2, d2.DeviceId);
	ASSERT_EQ((uint32_t)1234, d2.TitleId);
	ASSERT_EQ((uint16_t)4, d2.Track);
	ASSERT_EQ((uint8_t)1, d2.Device);
	ASSERT_EQ((uint8_t)3, d2.Page);
}

TEST_F(StorageTrackTest, DataTrack_Clear) {
	DataTrack d1;
	d1.SetValue(1, 2, 3, 4, 1234);
	d1.Clear();
	ASSERT_EQ((uint32_t)0, d1.DeviceId);
	ASSERT_EQ((uint32_t)0, d1.TitleId);
	ASSERT_EQ((uint16_t)0, d1.Track);
	ASSERT_EQ((uint8_t)0, d1.Device);
	ASSERT_EQ((uint8_t)0, d1.Page);
}

TEST_F(StorageTrackTest, DataTrack_ExportImport) {
	DataTrack d1;
	d1.SetValue(1, 2, 3, 4, 1234);
	ppl7::ByteArray bin;
	d1.Export(bin);
	ASSERT_EQ((uint8_t)1, d1.GetFormatVersion());
	DataTrack d2;
	d2.Import(bin, d1.GetFormatVersion());
	ASSERT_EQ((uint32_t)2, d2.DeviceId);
	ASSERT_EQ((uint32_t)1234, d2.TitleId);
	ASSERT_EQ((uint16_t)4, d2.Track);
	ASSERT_EQ((uint8_t)1, d2.Device);
	ASSERT_EQ((uint8_t)3, d2.Page);
}




} // EOF namespace
