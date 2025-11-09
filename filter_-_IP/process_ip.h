#ifndef PROCESS_IP_H
#define PROCESS_IP_H

#include <vector>
#include <cstdint>
void appendItem(std::vector<uint32_t> &ip_pool, int item);
std::vector<uint32_t> grepByFirstByte(std::vector<uint32_t> &, uint32_t);
std::vector<uint32_t> grepByTwoFirstByte(std::vector<uint32_t> &, uint32_t, uint32_t);
std::vector<uint32_t> grepByAnyByte(std::vector<uint32_t> &, uint32_t);
void print_ip(std::vector<uint32_t>&);
#endif// PROCESS_IP_H
