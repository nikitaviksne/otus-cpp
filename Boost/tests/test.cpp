#include <iostream>
#include <gtest/gtest.h>  
#if 0
TEST(TestMatrix, DefaultSize) {
    	Matrix<int, 0> matrix;
	EXPECT_EQ(matrix.size(), 0); // 
}

TEST(TestMatrix, Canonical) {
    	Matrix<int, -1> matrix;
	((matrix[100][100] = -102) = 0) = 534;
	EXPECT_EQ(matrix[100][100], 534); // 
}

TEST(TestMatrix, 3DMatrix) {
    	NMatrix<int, -1, 3> matrix;
	((matrix[100][100][0] = 6) = 6) = 7;
	EXPECT_EQ(matrix[100][100][0], 7); // 
}
#endif
int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
