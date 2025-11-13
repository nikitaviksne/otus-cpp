#ifndef PROCESS_IP_H
#define PROCESS_IP_H

#include <vector>
#include <cstdint>
#include <fstream>

std::tuple<int, uint8_t, uint8_t, uint8_t, uint8_t> readFromStream(std::istream& input);
void appendItem(std::vector<uint32_t> &ip_pool, int item);

template<typename... Bytes>
std::vector<uint32_t> grepByFirstsByte(const std::vector<uint32_t> &ip_pool, const Bytes&... vars);

std::vector<uint32_t> grepByFirstByte(std::vector<uint32_t> &, uint32_t);
std::vector<uint32_t> grepByTwoFirstByte(std::vector<uint32_t> &, uint32_t, uint32_t);
std::vector<uint32_t> grepByAnyByte(const std::vector<uint32_t> &, uint32_t);
void print_ip(const std::vector<uint32_t>& );
#endif// PROCESS_IP_H
