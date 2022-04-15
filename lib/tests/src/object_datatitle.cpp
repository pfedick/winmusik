#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <gtest/gtest.h>
#include "libwinmusik3.h"

#include "../../../ppl6/include/ppl7.h"
#include "wmlib-tests.h"

namespace {

using namespace de::pfp::winmusik;


class DataTitleTest : public ::testing::Test {
protected:
    DataTitleTest() {
        if (setlocale(LC_ALL,"")==NULL) {
            printf ("setlocale fehlgeschlagen\n");
            throw std::exception();
        }
    }
    ~DataTitleTest()
    {

    }
};

TEST_F(DataTitleTest, Constructor) {
    DataTitle Ti;
    ASSERT_EQ((uint32_t)0, Ti.getFilePos());
    ASSERT_EQ((uint32_t)0, Ti.getLastchange());
    ASSERT_EQ((uint32_t)0, Ti.getVersion());

}





} // EOF namespace
