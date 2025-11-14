#ifndef PROCESS_IP_H
#define PROCESS_IP_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <type_traits>

std::tuple<int, uint8_t, uint8_t, uint8_t, uint8_t> readFromStream(std::istream& input);
void appendItem(std::vector<uint32_t> &ip_pool, int item);

template<typename... Pack>
std::vector<uint32_t> grepByFirstsByte(const std::vector<uint32_t> &ip_pool, const Pack&... vars)
{
	static_assert(sizeof...(vars)<=4, "Too many arguments. Can not unpack more than 4 arguments");
	int shift = 24;
	uint32_t etalon = 0;
	uint32_t mask = 0; //маска, чтобы знать сколько первых байт брать у входного значения
	([&]{
		//static_assert(std::is_integral<Pack>==, "All arguments must be integral types");
		mask |= static_cast<uint32_t>(0xff) << shift;
	        etalon |= ((static_cast<uint32_t>(vars) ) << shift); //сдвигаем каждый байт из аргументов на соответсвующую позицию и формируем "у" из первых n байт, переданных в функцию
        	shift -= 8;
	}(), ...);
	auto its = std::find_if(ip_pool.begin(), ip_pool.end(), [etalon, mask](uint32_t ip){return (ip & mask) == etalon;}); // побитовое ИЛИ для соотвествия ip etalon. its -- iterarot start
	//Ищем где начинается первый с конца IP адресс, соответсвующий etalon
	auto ite = std::find_if(ip_pool.rbegin(), ip_pool.rend(), [etalon, mask](uint32_t ip){return (ip & mask) == etalon;}); // побитовое ИЛИ для соотвествия ip etalon. ite -- iterator end
	std::vector<uint32_t> result;
	if ((its != ip_pool.end()) || (ite != ip_pool.rend()))
		 result = std::vector<uint32_t>(its, ite.base());
	return result;
}

std::vector<uint32_t> grepByFirstByte(std::vector<uint32_t> &, uint32_t);
std::vector<uint32_t> grepByTwoFirstByte(std::vector<uint32_t> &, uint32_t, uint32_t);
std::vector<uint32_t> grepByAnyByte(const std::vector<uint32_t> &, uint32_t);
void print_ip(const std::vector<uint32_t>& );
#endif// PROCESS_IP_H
