#ifndef CFILESTATE_H
#define CFILESTATE_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

// Тип для хранения MD5 хеша (в бинарном виде 16 байт)
using HashValue = std::string;

class FileState {
public:
    fs::path path;
    uintmax_t size;
    mutable std::ifstream stream;
    mutable std::vector<HashValue> hashes;

    FileState(fs::path p);

    HashValue get_block_hash(const size_t block_idx, const size_t S) const ;
    
};
#endif
