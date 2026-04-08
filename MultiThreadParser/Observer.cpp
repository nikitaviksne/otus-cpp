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

FileLogger::FileLogger():worker(&FileLogger::process, this){};

FileLogger::~FileLogger()
{
    done = true;
    cv.notify_one();
    if (worker.joinable()) worker.join(); //Ожидание завершения записи последнего файла
}
// Вывод в файл
void FileLogger::update(const std::vector<std::string>& commands, std::time_t time) 
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push({commands, time, counter++});
    }
    cv.notify_one();
}

void FileLogger::process()
{
    while (true)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{return !tasks.empty() || done;});

            if (tasks.empty() && done) break;
            task = std::move(tasks.front());
            tasks.pop();
        }

        /*
        if (!task.id)
        {
            std::string filename = "bulk" + std::to_string(task.time) + "_" + std::to_string(task.id) + ".log";
        }
        else
        {
            std::string filename = "bulk" + std::to_string(task.time) + ".log";
        }
        */
        std::string filename;
        if (!task.id){
            filename = "bulk" + std::to_string(task.time) + "_" + std::to_string(task.id) + ".log";
        }
        else
        {
            filename = "bulk" + std::to_string(task.time) + ".log";
        }
        std::ofstream file(filename);
        if (file.is_open())
        {
            for (const auto& cmd: task.commands)
            {
                file << cmd << "\n";
            }
        }
    }
}
