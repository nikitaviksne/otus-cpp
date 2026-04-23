#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include "Slot.h"
#include "signal.hpp"
#include "intrusive_list.hpp"

template<typename R, typename... Args>
class Slot;

template<typename... Args>
class Connection : public ConnectionNode {
    Slot<void, Args...> slot; //слот 

public:
    template<typename F>
    Connection(F&& f) : slot(std::forward<F>(f)) {}

    void emit(Args... args) {
        slot(std::forward<Args>(args)...);
    }
    
   ~Connection() {
    this->unlink(); // Узел сам вычеркнет себя из списка при разрушении
   }
};
#endif
