//#define BOOST_TEST_MODULE test_version
#include <gtest/gtest.h>  
#include "lib.h"

TEST(TestVersion, PositiveVersion) {
	EXPECT_GT(version(), 0);
}

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
