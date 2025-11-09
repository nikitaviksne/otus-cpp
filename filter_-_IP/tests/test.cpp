//#define BOOST_TEST_MODULE test_version
#include <gtest/gtest.h>  
#include <vector>
#include "process_ip.h"
#include "lib.h"

TEST(TestVersion, PositiveVersion)
{
	EXPECT_GT(version(), 0);
}


TEST(TestIP, ReverseLexicographySort) {
	std::vector<uint32_t> input = {12, 40, 2, 8, 1, 40, 124, 255, 2, 1, 2, 49}; //для хранения входных значений
	std::vector<uint32_t> etalon = {255, 124, 49, 40, 40, 12, 8, 2, 2, 2, 1, 1};// -10}; //С чем сравниваем, сначала, специально ломаем тест, чтобыузнать, что он не проходит
	std::vector<uint32_t> output; //выходной отсортированный вектор
	for(int iii= 0; iii<input.size(); ++iii)
	{
		appendItem(output, input[iii]);
	}
	EXPECT_EQ(etalon, output); 
	
	output.clear();
	for (uint32_t iii = 0; iii<input.size(); iii++)
	{
		input[iii] = iii;
		appendItem(output, iii);
	}
	EXPECT_NE(etalon, output);
}

TEST(TestIP, FirstByte) {
	std::vector<uint32_t> input = {(uint32_t) (255<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + 1,
		(uint32_t) (45<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1
	};
	std::vector<uint32_t> etalon = {
		(uint32_t) (46<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1
		}; //С чем сравниваем, сначала, специально ломаем тест, чтобыузнать, что он не проходит
	std::vector<uint32_t> output = grepByFirstByte(input, 46); //выходной отсортированный вектор
	
	EXPECT_EQ(etalon, output); 
}

TEST(TestIP, SecondByte) {
	std::vector<uint32_t> input = {(uint32_t) (255<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (5<<8) + (uint32_t) 1,
		(uint32_t) (45<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1
	};
	std::vector<uint32_t> etalon = {
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (5<<8) + (uint32_t) 1
		}; //С чем сравниваем, сначала, специально ломаем тест, чтобыузнать, что он не проходит
	std::vector<uint32_t> output = grepByTwoFirstByte(input, 46, 28); //выходной отсортированный вектор
	
	EXPECT_EQ(etalon, output); 
}


TEST(TestIP, AnyByte) {
	std::vector<uint32_t> input = {(uint32_t) (255<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (5<<8) + (uint32_t) 1,
		(uint32_t) (40<<24) + (uint32_t) (46<<16) + (uint32_t) (5<<8) + (uint32_t) 1,
		(uint32_t) (40<<24) + (uint32_t) (36<<16) + (uint32_t) (46<<8) + (uint32_t) 1,
		(uint32_t) (40<<24) + (uint32_t) (36<<16) + (uint32_t) (26<<8) + (uint32_t) 46,
		(uint32_t) (45<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1
	};
	std::vector<uint32_t> etalon = {
		(uint32_t) (46<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (5<<8) + (uint32_t) 1,
		(uint32_t) (40<<24) + (uint32_t) (46<<16) + (uint32_t) (5<<8) + (uint32_t) 1,
		(uint32_t) (40<<24) + (uint32_t) (36<<16) + (uint32_t) (46<<8) + (uint32_t) 1,
		(uint32_t) (40<<24) + (uint32_t) (36<<16) + (uint32_t) (26<<8) + (uint32_t) 46
		}; //С чем сравниваем
	std::vector<uint32_t> output = grepByAnyByte(input, 46); //выходной отсортированный вектор
	EXPECT_EQ(etalon, output); 
}
int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
