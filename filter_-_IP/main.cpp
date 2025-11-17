#include <cstdint>
#include <vector>
#include <iostream>
#include <tuple>
#include "process_ip.h"
int main()
{		
	std::vector<uint32_t> arrIp;
	IpFilter ip_filter;
	//считывание IP из стандратного потока и формирование отсортированного массива
	std::cin >> ip_filter;
	//вывод отсортированного массива
	std::cout << ip_filter.arrIP;
	std::cout << ip_filter.grepByFirstsByte(1); //фильтрация по первому байту = 1
	std::cout << ip_filter.grepByFirstsByte(46, 70); //фильтрация по первому байту 46 и второму байту 70
	std::cout << ip_filter.grepByAnyByte((uint32_t) 46); //фильтрация по любому байту = 46
	return 0; 
}
