#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <mutex>
//#include <utility>
#include "intrusive_list.hpp"
#include "connection.hpp"

//Опережающее объявление шаблона Connection
template<typename... Args>
class Connection; 

template<typename... Args>
class Signal {
    ConnectionNode head; // Фиктивный узел для удобства обхода
	mutable std::recursive_mutex mtx; // Защищает структуру списка
public:
    Signal() {
        head.next = &head;
        head.prev = &head;
    }

	~Signal() {
    // 1. Захватываем мьютекс перед тем, как начать разрушение структур
    std::lock_guard<std::recursive_mutex> lock(mtx);

    // 2. Итерируемся по списку и принудительно отключаем все узлы
    while (head.next != &head) {
        ConnectionNode* node = head.next;
        
        // Важный момент: unlink() должен занулять указатели внутри узла,
        // чтобы деструктор Connection потом не пытался сделать это снова.
        node->unlink(); 
    }
}
	
	template<typename F>
	auto connect_managed(F&& f) {
    		auto conn = std::make_unique<Connection<Args...>>(std::forward<F>(f));
    		connect(*conn);
   	 	return conn; // Пользователь сохраняет этот объект, пока ему нужна связь
}

    // Пользователь сам передает объект Connection, созданный где-то еще
    void connect(Connection<Args...>& conn) {

	std::lock_guard<std::recursive_mutex> lock(mtx);
        conn.unlink();
#if 0
        // Вставляем в конец двусвязного списка
        conn.unlink(); // На случай, если уже где-то подключен
        conn.next = &head;
        conn.prev = head.prev;
        head.prev->next = &conn;
        head.prev = &conn;
#else
	// Привязываем к нашему мьютексу
    	conn.mtx = &this->mtx; 

    	conn.next = &head;
    	conn.prev = head.prev;
    	head.prev->next = &conn;
 	head.prev = &conn;
#endif
	
    }

    void emit(Args... args) {
	std::lock_guard<std::recursive_mutex> lock(mtx);
        ConnectionNode* curr = head.next;
        while (curr != &head) {
            // Магия static_cast: превращаем узел обратно в связь
            auto* conn = static_cast<Connection<Args...>*>(curr);
            curr = curr->next; // Шагаем вперед заранее (на случай отписки внутри emit)
            conn->emit(std::forward<Args>(args)...);
        }
    }
};


#endif
