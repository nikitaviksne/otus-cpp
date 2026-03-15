#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include "CFileState.h"

void print_out(std::vector<std::vector<FileState*>> identicalFiles, std::ostream& out = std::cout);

std::vector<std::vector<FileState*>> process_group(std::vector<FileState>& group, size_t S);
#endif
