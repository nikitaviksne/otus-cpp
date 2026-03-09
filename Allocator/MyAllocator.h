#include <memory>
#include <vector>
#include <iostream>

template <typename T, std::size_t N>
class MyAllocator {
    struct Storage {
        std::vector<char> data;
        std::size_t used_bytes = 0; // Считаем байты

        Storage() {
            data.reserve((N + 1) * sizeof(T));
        }
    };

    std::shared_ptr<Storage> pool;

public:
    using value_type = T;

    MyAllocator() : pool(std::make_shared<Storage>()) {}

    template <typename U>
    MyAllocator(const MyAllocator<U, N>& other) noexcept : pool(other.pool) {}

    T* allocate(std::size_t n) {
        std::size_t bytes_needed = n * sizeof(T);
        
        // Проверяем лимит
        if (pool->used_bytes + bytes_needed > (N + 1) * sizeof(T)) {
            throw std::bad_alloc();
        }

        if (pool->data.empty()) {
            pool->data.resize((N + 1) * sizeof(T));
        }

        void* ptr = &pool->data[pool->used_bytes];
        pool->used_bytes += bytes_needed;
        return reinterpret_cast<T*>(ptr);
    }

    void deallocate(T*, std::size_t) noexcept {}

    template <typename U> struct rebind { using other = MyAllocator<U, N>; };
    template <typename U, std::size_t M> friend class MyAllocator;
};
