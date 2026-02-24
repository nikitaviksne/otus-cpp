#include <iostream>
#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <gtest/gtest.h>  
#include <sstream> 
#include "sfinae.h"

TEST(TestSFINAE, int255) {
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( int8_t{-1} );                             // 255 
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "255\n"); // Проверяем захваченный выв
}

TEST(TestSFINAE, int00) {
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( int16_t{0} );                            // 0.0 
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "0.0\n"); // Проверяем захваченный выв
}

TEST(TESTSFINAE, int127){
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( int32_t{2130706433} );                   // 127.0.0.1 
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "127.0.0.1\n"); // Проверяем захваченный выв
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
}

TEST(TESTSFINAE, int123){
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( int64_t{8875824491850138409} );          // 123.45.67.89.101.112.131.41 
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "123.45.67.89.101.112.131.41\n"); // Проверяем захваченный выв
}

TEST(TESTSFINAE, str){
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( std::string{"Hello, World!"} );          // Hello, World! 
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "Hello, World!\n"); // Проверяем захваченный выв
}

TEST(TESTSFINAE, vec){
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( std::vector<int>{100, 200, 300, 400} );  // 100.200.300.400 
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "100.200.300.400\n"); // Проверяем захваченный выв
}

TEST(TESTSFINAE, lst){
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( std::make_tuple(123, 456, 789, 0) );     // 123.456.789.0
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "123.456.789.0\n"); // Проверяем захваченный выв
}

TEST(TESTSFINAE, tpl){
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    	print_ip( std::list<short>{400, 300, 200, 100} );  // 400.300.200.100
    	std::cout.rdbuf(old); // Восстанавливаем оригинальный stdout
    	EXPECT_EQ(buffer.str(), "400.300.200.100\n"); // Проверяем захваченный выв
}
int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
