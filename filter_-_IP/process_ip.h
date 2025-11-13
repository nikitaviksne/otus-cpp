#ifndef PROCESS_IP_H
#define PROCESS_IP_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <algorithm>

std::tuple<int, uint8_t, uint8_t, uint8_t, uint8_t> readFromStream(std::istream& input);
void appendItem(std::vector<uint32_t> &ip_pool, int item);

template<typename... Pack>
std::vector<uint32_t> grepByFirstsByte(const std::vector<uint32_t> &ip_pool, const Pack&... vars)
{
	std::vector<uint32_t> ip_out;
	const size_t n = sizeof...(Pack);
	//ищем где начинается первый байт равный fByte
	auto fByte = std::get<0>(std::tie(vars...));
	auto it = std::find_if(ip_pool.begin(), ip_pool.end(), [fByte](uint32_t ip){return (ip & (255<<24)) ==( (uint32_t) fByte << 24);});
	auto it2 = it;
	#if (n>1)
	{
		auto sByte = std::get<1>(std::tie(vars...));
		it2 = std::find_if(it, ip_pool.end(), [sByte](uint32_t ip){return (ip & (255<<16)) == (uint32_t) (sByte << 16);});
	}
	if (it2!=ip_pool.end())
	{
		for (auto iii=it2; iii!=ip_pool.end(); ++iii)
		{
			//Выделяю первую октету IP и сравниваю ее с fByte
			if ((*iii & (255<<24)) == (uint32_t) (fByte << 24))
			{	
				#if (n>1)
				{
					if ( (uint32_t) ( (std::get<1>(std::tie(vars...))) << 16) == (uint32_t) (*iii &(255<<16)  ))
					{
						ip_out.push_back(*iii);
					}
				}
				#else
				{
						ip_out.push_back(*iii);
				}
				#endif
			}
			else //т.к вектор отсортирован, значит дальше ничего уже быть не может
				break;
		}
	}
	return ip_out;

}
std::vector<uint32_t> grepByFirstByte(std::vector<uint32_t> &, uint32_t);
std::vector<uint32_t> grepByTwoFirstByte(std::vector<uint32_t> &, uint32_t, uint32_t);
std::vector<uint32_t> grepByAnyByte(const std::vector<uint32_t> &, uint32_t);
void print_ip(const std::vector<uint32_t>& );
#endif// PROCESS_IP_H
