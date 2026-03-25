#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "BulkHandler.h"


void BulkHandler::flush() {
        if (commands.empty()) return;
        for (auto& obs : observers) obs->update(commands, first_cmd_time);
        commands.clear();
    }

BulkHandler::BulkHandler(int size) : n(size) {}

void BulkHandler::addObserver(std::shared_ptr<Observer> obs) { observers.push_back(obs); }

void BulkHandler::process(const std::string& line) {
        if (line == "{") { //Начало динамического блка
            if (depth == 0) flush();
            depth++;
        } else if (line == "}") { //Конец динамического блока (может вложенного)
            depth--;
            if (depth == 0) flush();
        } else { // командна внутри динамического блока
            if (commands.empty()) first_cmd_time = std::time(nullptr);
            commands.push_back(line);
            if (depth == 0 && commands.size() == n) flush();
        }
    }

void BulkHandler::forceFlush() { if (depth == 0) flush(); }
