#ifndef INTRUSIVE_LIST_HPP
#define INTRUSIVE_LIST_HPP
/*
Создаем структуру, которая умеет связываться с соседями, чтобы обеспечить 0 аллокаций, должна быть узлом, значит зранить указатель на следующего и предыдущего
*/

struct ConnectionNode {
    ConnectionNode* next = nullptr;
    ConnectionNode* prev = nullptr;
	
	std::atomic<std::recursive_mutex*> mtx{nullptr}; // <--- Указатель на мьютекс сигнала

    void unlink() {
	auto* current_mtx = mtx.load(std::memory_order_acquire);
        if (current_mtx) {
            std::lock_guard<std::recursive_mutex> lock(*current_mtx);
	    //не занулил ли его signal
	    if (mtx.load(std::memory_order_relaxed) == current_mtx) {
            	perform_unlink();
       	    }
        } else {
            perform_unlink();
        }
    };

private:
	void perform_unlink() {
        if (next && prev) {
            next->prev = prev;
            prev->next = next;
        }
        next = prev = nullptr;
        mtx.store(nullptr, std::memory_order_relaxed); // Больше не связаны с этим мьютексом
    }
};
#endif
