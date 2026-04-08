#ifndef BULKHANDLER_H
#define BULKHANDLER_H
#include <memory>
#include <vector>
#include "Observer.h"

class BulkHandler {
    std::vector<std::string> commands;
    std::vector<std::shared_ptr<Observer>> observers;
    std::time_t first_cmd_time;
    int n, depth = 0;

    void flush();
public:
    BulkHandler(int size);

    void addObserver(std::shared_ptr<Observer> obs) ;

    void process(const std::string& line) ;

    void forceFlush(); 
};
#endif
