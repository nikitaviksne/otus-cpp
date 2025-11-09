#include <stdio.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include "process_ip.h"
//#include <algorithm>
int main()
{		
	int res (10);
	std::vector<uint32_t> arrIp;
	int ip;
	while (true)
	{
		uint8_t f1,f2,f3,f4;
		//printf("Введите IP: ");
		res = scanf("%d.%d.%d.%d%*s%*s\n", &f1, &f2, &f3, &f4);
		//res = scanf("%d", &ip); //просто число для отладки
		if (res<=0) break;
		ip = (uint32_t) f4 + (f3<<8) + (f2<<16) + (f1<<24);
		//printf("было считано %d считанный IP-адресс: %d.%d.%d.%d или %u\n", res, f1,f2,f3,f4, ip);
		//организуем сразу отсортированный массив по принципу бинарного поиска (или сортировка слиянием?)
		appendItem(arrIp, ip);
	}// while true (input)
	print_ip(arrIp); //вывод отсортированного массива
	std::vector<uint32_t> fb = grepByFirstByte(arrIp, 1); //фильтрация по первому байту = 1
	print_ip(fb); //вывод в отсортированном порядке адресов с первым байтом 1
	std::vector<uint32_t> tfb = grepByTwoFirstByte(arrIp, 46, 70); //фильтрация по первому байту = 46
	print_ip(tfb); //вывод в отсортированном порядке адресов с первым байтом 46 и вторым 70
	std::vector<uint32_t> ab = grepByAnyByte(arrIp, 46); //фильтрация по любому байту = 46
	print_ip(ab); //вывод в отсортированном порядке адресов с любым байтом 46
	return 0; 
}
