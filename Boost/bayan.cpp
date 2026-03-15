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
#include "CFileState.h"
#include "process.h"

namespace fs = boost::filesystem;
namespace po = boost::program_options;


int main(int argc, char** argv) {
    std::vector<std::string> inc, exc, masks;
    size_t min_s, block_s;
    int level;

    po::options_description desc("Опции");
    desc.add_options()
        ("include,i", po::value<std::vector<std::string>>(&inc)->multitoken())
        ("exclude,e", po::value<std::vector<std::string>>(&exc)->multitoken())
        ("level,l", po::value<int>(&level)->default_value(0))
        ("min-size,s", po::value<size_t>(&min_s)->default_value(1))
        ("block-size,b", po::value<size_t>(&block_s)->default_value(1024))
        ("mask,m", po::value<std::vector<std::string>>(&masks)->multitoken());

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Подготовка Regex
    std::vector<boost::regex> regex_masks;
    for (auto& m : masks) {
        std::string r = m;
        boost::replace_all(r, ".", "\\.");
        boost::replace_all(r, "*", ".*");
        regex_masks.emplace_back(r, boost::regex::icase);
    }

    std::map<uintmax_t, std::vector<FileState>> files_by_size;

    for (const auto& path_str : inc) {
        if (!fs::exists(path_str)) continue; // если файла нет
        for (fs::recursive_directory_iterator it(path_str), end; it != end; ++it) { 
            if (it.level() > level) { it.no_push(); continue; } // если первышена максимальна глубина
            
            bool is_exc = false;
            for (auto& e : exc) if (fs::equivalent(it->path().parent_path(), e)) is_exc = true; // сравнение относительного/абсолютного пути
            if (is_exc) { it.no_push(); continue; }

            if (fs::is_regular_file(*it) && fs::file_size(*it) >= min_s) {
                bool match = regex_masks.empty();
                for (auto& r : regex_masks) if (boost::regex_match(it->path().filename().string(), r)) match = true;
                
                if (match) {
                    files_by_size[fs::file_size(*it)].emplace_back(it->path());
                }
            }
        }
    }

    for (auto& pair : files_by_size) {
	std::vector<std::vector<FileState*>> groupOfFiles;
        groupOfFiles = process_group(pair.second, block_s);
	print_out(groupOfFiles);
    }

    return 0;
}

