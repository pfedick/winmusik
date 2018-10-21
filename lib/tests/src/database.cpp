#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ppl7.h>
#include <gtest/gtest.h>
#include "libwinmusik3.h"
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class DatabaseTest : public ::testing::Test {
protected:
	DatabaseTest() {
		if (setlocale(LC_ALL,"")==NULL) {
			printf ("setlocale fehlgeschlagen\n");
			throw std::exception();
		}
	}
	~DatabaseTest()
	{

	}
};

TEST_F(DatabaseTest, UpdateDatabase) {
	/*
	de::pfp::winmusik::CDataBase wmdb;
	ppl7::String Path=ppl7::Dir::homePath()+"/ownCloud/WinMusik3/";
	wmdb.update(Path+"winmusik.dat",Path+"winmusik4.dat");
	*/
}





} // EOF namespace
