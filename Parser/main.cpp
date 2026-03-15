#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <memory>

// Интерфейс для вывода данных
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::vector<std::string>& commands, std::time_t time) = 0;
};

// Вывод в консоль
class ConsoleLogger : public Observer {
public:
    void update(const std::vector<std::string>& commands, std::time_t time) override {
        std::cout << "bulk: ";
        for (size_t i = 0; i < commands.size(); ++i) {
            std::cout << commands[i] << (i == commands.size() - 1 ? "" : ", ");
        }
        std::cout << std::endl;
    }
};

// Вывод в файл
class FileLogger : public Observer {
public:
    void update(const std::vector<std::string>& commands, std::time_t time) override {
        std::string filename = "bulk" + std::to_string(time) + ".log";
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& cmd : commands) file << cmd << "\n";
        }
    }
};

class BulkHandler {
    std::vector<std::string> commands;
    std::vector<std::shared_ptr<Observer>> observers;
    std::time_t first_cmd_time;
    int n, depth = 0;

    void flush() {
        if (commands.empty()) return;
        for (auto& obs : observers) obs->update(commands, first_cmd_time);
        commands.clear();
    }

public:
    BulkHandler(int size) : n(size) {}

    void addObserver(std::shared_ptr<Observer> obs) { observers.push_back(obs); }

    void process(const std::string& line) {
        if (line == "{") {
            if (depth == 0) flush();
            depth++;
        } else if (line == "}") {
            depth--;
            if (depth == 0) flush();
        } else {
            if (commands.empty()) first_cmd_time = std::time(nullptr);
            commands.push_back(line);
            if (depth == 0 && commands.size() == n) flush();
        }
    }

    void forceFlush() { if (depth == 0) flush(); }
};

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

