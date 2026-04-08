#ifndef OBSERVER_H
#define OBSERVER_H
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>

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
    //структура данных для очереди
    struct Task{
        std::vector<std::string> commands;
        std::time_t time;
        uint64_t id; // на всякий пожарный
    } ;
    std::queue<Task> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> done{false};
    std::atomic<uint64_t> counter{0}; // Счетчик задач
    std::thread worker;

    void process(); 
public:
    FileLogger();
    ~FileLogger(); //Дождаться записи всех файлов
    void update(const std::vector<std::string>& commands, std::time_t time) override;
};
#endif
