//#define BOOST_TEST_MODULE test_version
#include <gtest/gtest.h>  
#include "lib.h"

TEST(TestVersion, NonNegativeVersion) {
	EXPECT_GT(version(), 0);
}

