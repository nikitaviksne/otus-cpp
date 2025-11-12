#include <stdio.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "process_ip.h"

std::tuple<int, uint8_t, uint8_t, uint8_t, uint8_t> readFromStream(std::istream& input)
{
	std::string inputStr;
	std::getline(input, inputStr);
	uint8_t f1,f2,f3,f4;
	std::regex pattern("^(\\d*)\\.(\\d*)\\.(\\d*)\\.(\\d*)"); //паттерн, в началае строки 4 числа разделенные точкой
	std::smatch result;
	if (std::regex_search(inputStr, result, pattern))
	{
		return std::make_tuple(4, (uint8_t) stoi(result[1]), (uint8_t) stoi(result[2]), (uint8_t)stoi(result[3]), (uint8_t)stoi(result[4]));
	}
	else
	{
		return std::make_tuple((int) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0);
	}
}

void appendItem(std::vector<uint32_t> &ip_pool, int item)
{
	//добавляет item в ip_pool сразу в обратном лексикографическом порядке
	int size = ip_pool.size();
		static int low; //нижняя граница поиска
		static int up; // верхняя граница поиска
		if (size == 0)
		{
			ip_pool.insert(ip_pool.begin(), item);
			low = 0;
			up = 1; //ip_pool.size();
		}
		else
		{	
			if (item <(uint32_t) ip_pool[low]) //если новый элемент меньше последнего, добавленного в массив, то рассматриваем правую часть
			{
				up = ip_pool.size() - 1;
			}
			else
			{
				low = 0;
			}
			
			while (low <= up)
			{
				int index = (low+up)/2;
				if ((uint32_t)ip_pool[index] < item) //значит надо вставлять в левую часть
				{
					up = index - 1;
					continue;
				}
				if ((uint32_t)ip_pool[index] >= item) //значит надо вставлять в правую часть от текущего индекса
				{
					low = index + 1;
					continue;
				}
			}//while
			//после этого цикла, если у меня такого же item не будет найдено в массиве, то возникнет ситуация, low >= up, а значит вставлять надо на позицию up+1 = low
			ip_pool.insert(ip_pool.begin() + low, item);
		}//else (if (size==0) )
}

std::vector<uint32_t> grepByFirstByte(std::vector<uint32_t> &ip_pool, uint32_t fByte)
{
	std::vector<uint32_t> ip_out;
	//ищем где начинается первый байт равнй fByte
	auto it = std::find_if(ip_pool.begin(), ip_pool.end(), [fByte](uint32_t ip){return (ip & (255<<24)) == (uint32_t) fByte << 24;});
	if (it!=ip_pool.end())
	{
		for (auto iii=it; iii!=ip_pool.end(); ++iii)
		{
			//Выделяю первую октеты IP и сравниваю ее с fByte
			if ((*iii & (255<<24)) == (uint32_t) fByte << 24)
			{
				ip_out.push_back(*iii);
			}
			else //т.к вектор отсортирован, значит дальше ничего уже быть не может
				break;
		}
	}
	return ip_out;
}

std::vector<uint32_t> grepByTwoFirstByte(std::vector<uint32_t> &ip_pool, uint32_t fByte, uint32_t sByte)
{
	std::vector<uint32_t> ip_out;
	//ищем где начинается первый байт равнй fByte
	auto it = std::find_if(ip_pool.begin(), ip_pool.end(), [fByte](uint32_t ip){return (ip & (255<<24)) == (uint32_t) fByte << 24;});
	if (it!=ip_pool.end())
	{
		bool flag (0); //найден второй байт
		for (auto iii=it; iii!=ip_pool.end(); ++iii)
		{
			//Выделяю первую октету IP и сравниваю ее с fByte
			if ((*iii & (255<<24) ) == (uint32_t) (fByte << 24) )
			{
				if ((*iii & (255<<16)) == (uint32_t) (sByte << 16))
				{
					flag = 1;
					ip_out.push_back(*iii);
				}
				else if (flag) //т.е уже был найден второй байт, значит дальше ничего уже не будет
					break;
			}
		
		}
	}

	return ip_out;
}

std::vector<uint32_t> grepByAnyByte(std::vector<uint32_t> &ip_pool, uint32_t aByte)
{
	std::vector<uint32_t> ip_out;
	for (auto iii=ip_pool.begin(); iii!=ip_pool.end(); ++iii)
	{
		bool fb ((*iii & (255<<24)) == ((uint32_t) (aByte << 24) ) ); 
		bool sb ((*iii & (255<<16)) == ((uint32_t) (aByte << 16) ) ); 
		bool tb ((*iii & (255<<8)) == ((uint32_t) (aByte << 8) ) ); 
		bool qb ((*iii & (255<<0)) == ((uint32_t) (aByte << 0) ) ); 
		if (fb  || sb  || tb  || qb )
			ip_out.push_back(*iii);
		
	}
	return ip_out;
}

void print_ip(std::vector<uint32_t>& pool)
{

	for(int iii = 0; iii < pool.size(); iii++) 
	{
		printf("%u.%u.%u.%u\n", (pool[iii]&(255<<24))>>24, (pool[iii]&(255<<16))>>16, (pool[iii]&(255<<8))>>8, (pool[iii]&(255<<0))>>0);
		
	}
}
