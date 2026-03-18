#include <iostream>
#include <memory>
#include "BulkHandler.h"


int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    int n = std::stoi(argv[1]);

    auto handler = std::make_unique<BulkHandler>(n);
    handler->addObserver(std::make_shared<ConsoleLogger>());
    handler->addObserver(std::make_shared<FileLogger>());

    std::string line;
    while (std::getline(std::cin, line)) {
        handler->process(line);
    }
    handler->forceFlush();

    return 0;
}
