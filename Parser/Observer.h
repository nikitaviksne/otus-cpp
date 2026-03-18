#ifndef OBSERVER_H
#define OBSERVER_H
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Интерфейс для вывода данных
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::vector<std::string>& commands, std::time_t time) = 0;
};

// Вывод в консоль
class ConsoleLogger : public Observer {
public:
    void update(const std::vector<std::string>& commands, std::time_t time) override ;
};

// Вывод в файл
class FileLogger : public Observer {
public:
    void update(const std::vector<std::string>& commands, std::time_t time) override;
};
#endif
