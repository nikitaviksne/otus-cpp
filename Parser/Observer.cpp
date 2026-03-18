#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include "Observer.h"

// Вывод в консоль
void ConsoleLogger::update(const std::vector<std::string>& commands, std::time_t time) {
        std::cout << "bulk: ";
        for (size_t i = 0; i < commands.size(); ++i) {
            std::cout << commands[i] << (i == commands.size() - 1 ? "" : ", ");
        }
        std::cout << std::endl;
}

// Вывод в файл
void FileLogger::update(const std::vector<std::string>& commands, std::time_t time) {
        std::string filename = "bulk" + std::to_string(time) + ".log";
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& cmd : commands) file << cmd << "\n";
        }
}
