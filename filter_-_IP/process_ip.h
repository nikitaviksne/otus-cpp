#ifndef PROCESS_IP_H
#define PROCESS_IP_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <type_traits>

class IpFilter
{
public:
	IpFilter();
	IpFilter(const std::vector<uint32_t> &);
	~IpFilter();
	std::tuple<int, uint8_t, uint8_t, uint8_t, uint8_t> readFromStream(std::istream& input);
	void appendItem(int item);

	template<typename... Pack>
	std::vector<uint32_t> grepByFirstsByte(const Pack&... vars)
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
		auto its = std::find_if(arrIP.begin(), arrIP.end(), [etalon, mask](uint32_t ip){return (ip & mask) == etalon;}); // побитовое ИЛИ для соотвествия ip etalon. its -- iterarot start
		//Ищем где начинается первый с конца IP адресс, соответсвующий etalon
		auto ite = std::find_if(arrIP.rbegin(), arrIP.rend(), [etalon, mask](uint32_t ip){return (ip & mask) == etalon;}); // побитовое ИЛИ для соотвествия ip etalon. ite -- iterator end
		std::vector<uint32_t> result;
		if ((its != arrIP.end()) || (ite != arrIP.rend()))
			 result = std::vector<uint32_t>(its, ite.base());
		return result;
	}

	std::vector<uint32_t> grepByAnyByte(uint32_t);
	void print_ip(const std::vector<uint32_t>& );

	std::vector<uint32_t> arrIP;
	//perator >> (std::istream, )
	//friend std::istream& operator>>(std::istream &, IpFilter &);
	//friend std::ostream& operator>>(std::ostream &, const std::vector<uint32_t> &);
};
std::ostream& operator<<(std::ostream &stream, std::vector<uint32_t> vec);
std::istream& operator>>(std::istream& stream,  IpFilter & ip_filter );


#endif// PROCESS_IP_H
