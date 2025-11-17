#include <gtest/gtest.h>  
#include <vector>
#include <fstream>
#include <iostream>
#include <tuple>
#include "process_ip.h"

TEST(TestIP, ReadFromStream) {
	//проверка правильности чтения IP адреса со стандартного потока ввода
	std::ifstream in;
	in.open("../../tests/test.input", std::ios::in);
	EXPECT_TRUE(in.is_open());
	uint8_t f1,f2,f3,f4;
	uint32_t ip;
	int res;
	IpFilter ip_filter;
	//первый тест
	std::tie(res, f1, f2, f3, f4) = ip_filter.readFromStream(in);
	ip = (uint32_t) f4 + (f3<<8) + (f2<<16) + (f1<<24);
	std::cout << "Первый тест: f1 = "<< (int) f1 << " f2 = " << (int) f2 << " f3 = " << (int) f3 << " f4 = " << (int) f4 << " ip = " << ip << "\n";
	EXPECT_NE(res,0); 
	EXPECT_EQ(ip, 5 + (4<<8) + (3<<16) + (2<<24));
	//второй тест
	std::tie(res, f1, f2, f3, f4) = ip_filter.readFromStream(in);
	ip = (uint32_t) f4 + (f3<<8) + (f2<<16) + (f1<<24);
	std::cout << "Второй тест: f1 = " << f1 << " f2 = " << f2 <<" f3 = " << f3 << " f4 = " << f4 << " ip = " << ip << "\n";
	EXPECT_NE(res,0);
	EXPECT_EQ(ip, 6 + (5<<8) + (4<<16) + (3<<24));
	//третий тест
	std::tie(res, f1, f2, f3, f4) = ip_filter.readFromStream(in);
	ip = (uint32_t) f4 + (f3<<8) + (f2<<16) + (f1<<24);
	std::cout << "Третий тест: f1 = "<<f1<<" f2 = " << f2 <<" f3 = " << f3 << " f4 = " << f4 << "\n";
	EXPECT_NE(res,0);
	EXPECT_EQ(ip, 7 + (6<<8) + (5<<16) + (4<<24));
	//четвертый тест
	std::tie(res, f1, f2, f3, f4) = ip_filter.readFromStream(in);
	ip = (uint32_t) f4 + (f3<<8) + (f2<<16) + (f1<<24);
	std::cout << "Четвертый тест: f1 = "<<f1<<" f2 = " << f2 <<" f3 = " << f3 << " f4 = " << f4 << "\n";
	EXPECT_NE(res,0);
	EXPECT_EQ(ip, 8 + (7<<8) + (6<<16) + (5<<24));
	//пятый тест (проверка перегруженного оператора)
	in >> ip_filter;
	std::vector<uint32_t> test5 = {(uint32_t) 9 + (uint32_t) (90<<8) + (uint32_t) (128<<16) + (uint32_t) (192<<24),
	 (uint32_t) 1 + (uint32_t) (2<<8) + (uint32_t) (45<<16) + (uint32_t) (128<<24)};
	EXPECT_EQ(ip_filter.arrIP, test5);
	
	in.close();
}
TEST(TestIP, ReverseLexicographySort) {
	IpFilter ip_filter;
	std::vector<uint32_t> input = {12, 40, 2, 8, 1, 40, 124, 255, 2, 1, 2, 49}; //для хранения входных значений
	std::vector<uint32_t> etalon = {255, 124, 49, 40, 40, 12, 8, 2, 2, 2, 1, 1};// -10}; //С чем сравниваем, сначала, специально ломаем тест, чтобыузнать, что он не проходит
	for(int iii= 0; iii<input.size(); ++iii)
	{
		ip_filter.appendItem(input[iii]);
	}
	EXPECT_EQ(etalon, ip_filter.arrIP); 
	
	ip_filter.arrIP.clear();
	for (uint32_t iii = 0; iii<input.size(); iii++)
	{
		input[iii] = iii;
		ip_filter.appendItem(iii);
	}
	EXPECT_NE(etalon, ip_filter.arrIP);
}

TEST(TestIp, FirstsByte)
{
	std::vector<uint32_t> input;
	std::vector<uint32_t> etalon;
	std::vector<uint32_t> output;
	//фильтрация по первому и второму байту
	input = {(uint32_t) (255<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1, // 4286578945
		(uint32_t) (46<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1, // 780140801
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + 1, // 773586945
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (5<<8) + (uint32_t) 1, // 773588225
		(uint32_t) (45<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1 // 756809729
	};
	etalon = {
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1, // 773586945
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (5<<8) + (uint32_t) 1 // 773588225
		}; //С чем сравниваем, сначала, специально ломаем тест, чтобыузнать, что он не проходит
	IpFilter ip_filter(input);
	output = ip_filter.grepByFirstsByte((uint32_t) 46, (uint32_t) 28); //выходной отсортированный вектор
	
	EXPECT_EQ(etalon, output); 
	//фильтрация по первому байту
	input = {(uint32_t) (255<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (146<<24) + (uint32_t) (128<<16) + (uint32_t) (1<<8) + 1,
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + 1,
		(uint32_t) (45<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1
	};
	etalon = {
		(uint32_t) (46<<24) + (uint32_t) (28<<16) + (uint32_t) (0<<8) + (uint32_t) 1
		}; //С чем сравниваем, сначала, специально ломаем тест, чтобыузнать, что он не проходит
	
	IpFilter ip_filter2(input);
	output = ip_filter2.grepByFirstsByte((uint32_t) 46); //выходной отсортированный вектор
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
	IpFilter ip_filter(input);
	std::vector<uint32_t> output = ip_filter.grepByAnyByte(46); //выходной отсортированный вектор
	EXPECT_EQ(etalon, output); 
}
int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
