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

namespace fs = boost::filesystem;
namespace po = boost::program_options;

void process_group(std::vector<FileState>& group, size_t S) {
    if (group.size() < 2) return;

    size_t num_blocks = (group[0].size + S - 1) / S;
    if (group[0].size == 0) num_blocks = 1; // Для пустых файлов

    std::vector<std::vector<FileState*>> current_subgroups;
    std::vector<FileState*> initial;
    for (auto& f : group) initial.push_back(&f);
    current_subgroups.push_back(initial);

    for (size_t b = 0; b < num_blocks; ++b) {
        std::vector<std::vector<FileState*>> next_subgroups;
        for (auto& subgroup : current_subgroups) {
            if (subgroup.size() < 2) continue; // уникальный файл

            std::map<HashValue, std::vector<FileState*>> splitter;
            for (auto* f : subgroup) {
                splitter[f->get_block_hash(b, S)].push_back(f);
            }

            for (auto& pair : splitter) {
                if (pair.second.size() > 1) {
                    next_subgroups.push_back(pair.second);
                } 
            }
        }
        current_subgroups = std::move(next_subgroups);
        if (current_subgroups.empty()) return;
    }

    for (const auto& final_group : current_subgroups) {
        for (auto* f : final_group) std::cout << f->path.string() << std::endl;
        std::cout << std::endl;
    }
}

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
        process_group(pair.second, block_s);
    }

    return 0;
}

