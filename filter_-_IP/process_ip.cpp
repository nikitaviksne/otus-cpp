#include <stdio.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include "process_ip.h"

IpFilter::IpFilter() {};
IpFilter::IpFilter(const std::vector<uint32_t> & vec) {arrIP = vec;};
IpFilter::~IpFilter(){};
std::tuple<int, uint8_t, uint8_t, uint8_t, uint8_t> IpFilter::readFromStream(std::istream& input)
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

void IpFilter::appendItem(int item)
{
	//добавляет item в arrIP сразу в обратном лексикографическом порядке
	int size = arrIP.size();
		static int low; //нижняя граница поиска
		static int up; // верхняя граница поиска
		if (size == 0)
		{
			arrIP.insert(arrIP.begin(), item);
			low = 0;
			up = 1; //arrIP.size();
		}
		else
		{	
			if (item <(uint32_t) arrIP[low]) //если новый элемент меньше последнего, добавленного в массив, то рассматриваем правую часть
			{
				up = arrIP.size() - 1;
			}
			else
			{
				low = 0;
			}
			
			while (low <= up)
			{
				int index = (low+up)/2;
				if ((uint32_t)arrIP[index] < item) //значит надо вставлять в левую часть
				{
					up = index - 1;
					continue;
				}
				if ((uint32_t)arrIP[index] >= item) //значит надо вставлять в правую часть от текущего индекса
				{
					low = index + 1;
					continue;
				}
			}//while
			//после этого цикла, если у меня такого же item не будет найдено в массиве, то возникнет ситуация, low >= up, а значит вставлять надо на позицию up+1 = low
			arrIP.insert(arrIP.begin() + low, item);
		}//else (if (size==0) )
}

std::vector<uint32_t> IpFilter::grepByAnyByte(uint32_t aByte)
{
	std::vector<uint32_t> ip_out;
	for (auto iii=arrIP.begin(); iii!=arrIP.end(); ++iii)
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

void IpFilter::print_ip(const std::vector<uint32_t>& pool)
{

	for(int iii = 0; iii < pool.size(); iii++) 
	{
		std::cout << ((pool[iii]&(255<<24))>>24) << "." << ((pool[iii]&(255<<16))>>16) <<"." << ((pool[iii]&(255<<8))>>8) << "." << ((pool[iii]&(255<<0))>>0) << "\n";
		
	}
}

std::ostream& operator<<(std::ostream &stream, std::vector<uint32_t> vec)
{
	for (auto iii=vec.begin(); iii!=vec.end(); iii++)
	{
		stream << *iii << "\n";
	}
    return stream;
}

std::istream& operator>>(std::istream& stream,  IpFilter & ip_filter )
{
	
	int ip;
	int res;	
//	while (true)
	{
		uint8_t f1,f2,f3,f4;
		//функций чтения из потока
		std::tie(res, f2, f2, f3, f4) = ip_filter.readFromStream(stream);
		std::cout << "Прочитали из потока: f1 = "<< (int) f1 << " f2 = " << (int) f2 << " f3 = " << (int) f3 << " f4 = " << (int) f4 << " ip = " << ip << " res = " << res << "\n";
//		if (res<=0) break;
		ip = (uint32_t) f4 + (f3<<8) + (f2<<16) + (f1<<24);
		//организуем сразу отсортированный массив по принципу бинарного поиска (или сортировка слиянием?)
		ip_filter.appendItem(ip);
	}// while true (input)
    return stream;
}
