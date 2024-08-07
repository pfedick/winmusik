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
#include "wm_storage.h"
#include "wm_playlist.h"
#include "wm_exceptions.h"
#include "wmlib-tests.h"

namespace de {
namespace pfp {
namespace winmusik {




class StorageItemTest : public ::testing::Test {
protected:
	StorageItemTest() {
		//ppl7::Dir::mkDir("tmp/title", true);

	}
	~StorageItemTest()
	{

	}
};





}
}
} // EOF namespace
