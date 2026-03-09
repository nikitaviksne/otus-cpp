#include <iostream>
#include <chrono>
#include <gtest/gtest.h>  
#include "MyAllocator.h"
#include "MyContainer.h"

TEST(TestAllocator, SpeedMap) {
	std::map<int, int, std::less<int>, MyAllocator<std::pair<const int, int>, 10>> myMap;
	//заполнение map
	auto start = std::chrono::steady_clock::now();
	for (int i=0; i<10; ++i)
	{
		myMap[i] = i;
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> customMapTime = end-start;
	
	//map со стандартным аллокатором
	std::map<int, int, std::less<int>> stdMap;
	//заполнение map
	start = std::chrono::steady_clock::now();
	for (int i=0; i<10; ++i)
	{
		stdMap[i] = i;
	}
	end = std::chrono::steady_clock::now();
	std::chrono::duration<double> stdMapTime = end-start;
	
	std::cout << "Время заполнения std::map с std::allocator " << stdMapTime.count() << " мс \n";
	std::cout << "Время заполнения std::map с custom allocator " << customMapTime.count() << " мс \n";
	EXPECT_LT(customMapTime, stdMapTime); // 
}

TEST(TestMatrix, SpeedContainer) {
	MyContainer <int, MyAllocator<int, 10> > customContainer;
	//заполнение контейнера
	auto start = std::chrono::steady_clock::now();
	for (int i=0; i<10; ++i)
	{
		customContainer.push_back(i);
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> customContTime = end-start;
	
	//контейнер со стандартным аллокатором
	MyContainer <int > justContainer;
	//заполнение map
	start = std::chrono::steady_clock::now();
	for (int i=0; i<10; ++i)
	{
		justContainer.push_back(i);
	}
	end = std::chrono::steady_clock::now();
	std::chrono::duration<double> stdContTime = end-start;
	
	std::cout << "Время заполнения MyContainer с std::allocator " << stdContTime.count() << " мс \n";
	std::cout << "Время заполнения MyContainer с custom allocator " << customContTime.count() << " мс \n";
	EXPECT_LT(customContTime, stdContTime); // 
}


int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
