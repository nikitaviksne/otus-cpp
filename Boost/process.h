#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include "CFileState.h"

void print_out(const std::vector<std::vector<FileState*>> identicalFiles, std::ostream& out = std::cout);

std::vector<std::vector<FileState*>> process_group(const std::vector<FileState>& group, const size_t  S);
#endif
