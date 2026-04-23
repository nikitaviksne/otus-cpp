#ifndef INTRUSIVE_LIST_HPP
#define INTRUSIVE_LIST_HPP
/*
Создаем структуру, которая умеет связываться с соседями, чтобы обеспечить 0 аллокаций, должна быть узлом, значит зранить указатель на следующего и предыдущего
*/

struct ConnectionNode {
    ConnectionNode* next = nullptr;
    ConnectionNode* prev = nullptr;
	
	std::recursive_mutex* mtx = nullptr; // <--- Указатель на мьютекс сигнала

    void unlink() {
        // Если есть мьютекс — блокируем его перед изменением указателей
        if (mtx) {
            std::lock_guard<std::recursive_mutex> lock(*mtx);
            perform_unlink();
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
        mtx = nullptr; // Больше не связаны с этим мьютексом
    }
};
#endif
