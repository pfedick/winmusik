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
		//CStorage storage;
		});
}

TEST_F(StorageTest, loadDatabase) {
	//CDataBase db;
	//CStorage storage;
	//storage.loadDatabase("/home/patrick/ownCloud/WinMusik3/winmusik.dat",db);

}




} // EOF namespace
