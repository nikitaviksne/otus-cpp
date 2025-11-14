#include <cstdint>
#include <vector>
#include <iostream>
#include <tuple>
#include "process_ip.h"
int main()
{		
	int res (10);
	std::vector<uint32_t> arrIp;
	int ip;
	
	while (true)
	{
		uint8_t f1,f2,f3,f4;
		//функций чтения из потока
		std::tie(res, f1, f2, f3, f4) = readFromStream(std::cin);
		if (res<=0) break;
		ip = (uint32_t) f4 + (f3<<8) + (f2<<16) + (f1<<24);
		//организуем сразу отсортированный массив по принципу бинарного поиска (или сортировка слиянием?)
		appendItem(arrIp, ip);
	}// while true (input)
	
	print_ip(arrIp); //вывод отсортированного массива
	std::vector<uint32_t> fb = grepByFirstsByte(arrIp, 1); //фильтрация по первому байту = 1
	print_ip(fb); //вывод в отсортированном порядке адресов с первым байтом 1
	std::vector<uint32_t> tfb = grepByFirstsByte(arrIp, 46, 70); //фильтрация по первому байту 46 и второму байту 70
	print_ip(tfb); //вывод в отсортированном порядке адресов с первым байтом 46 и вторым 70
	std::vector<uint32_t> ab = grepByAnyByte(arrIp, (uint32_t) 46); //фильтрация по любому байту = 46
	print_ip(ab); //вывод в отсортированном порядке адресов с любым байтом 46
	return 0; 
}
