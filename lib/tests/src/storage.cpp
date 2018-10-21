#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ppl7.h>
#include <gtest/gtest.h>
#include "../../include/libwinmusik3.h"
#include "../wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class StorageTest : public ::testing::Test {
	protected:
		StorageTest() {
			if (setlocale(LC_CTYPE,DEFAULT_LOCALE)==NULL) {
				printf ("setlocale fehlgeschlagen\n");
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
