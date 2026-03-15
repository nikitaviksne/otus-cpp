#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "CFileState.h"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

// Тип для хранения MD5 хеша (в бинарном виде 16 байт)
using HashValue = std::string;

FileState::FileState(fs::path p) : path(p), size(fs::file_size(p)) {}

HashValue FileState::get_block_hash(size_t block_idx, size_t S) const {
        if (block_idx < hashes.size()) return hashes[block_idx];

        if (!stream.is_open()) {
            stream.open(path.string(), std::ios::binary);
        }

        std::vector<char> buffer(S, 0);
        stream.seekg(block_idx * S);
        stream.read(buffer.data(), S);

        boost::uuids::detail::md5 hash_algo;
        boost::uuids::detail::md5::digest_type digest;
        hash_algo.process_bytes(buffer.data(), S); // Всегда S байт (дополнено нулями)
        hash_algo.get_digest(digest);

        HashValue res(reinterpret_cast<const char*>(&digest), sizeof(digest));
        hashes.push_back(res);
        return res;
}

