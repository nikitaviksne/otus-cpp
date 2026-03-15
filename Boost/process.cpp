#include <iostream>
#include <vector>
#include "CFileState.h"


void print_out(std::vector<std::vector<FileState*>> identicalFiles, std::ostream& out)
{
    //if(!identicalFiles.size()) return;//если пусто возвращаемся
    for (const auto& file : identicalFiles) {
        for (auto* f : file) out << f->path.string() << std::endl;
        
        out << std::endl;
    }
}

std::vector<std::vector<FileState*>> process_group(std::vector<FileState>& group, size_t S) {
    if (group.size() < 2) return std::vector<std::vector<FileState*>> ();

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
        if (current_subgroups.empty()) return std::vector<std::vector<FileState*>> ();
    }
    

    return current_subgroups; // std::vector чтобы было удобнее писать тесты

}
