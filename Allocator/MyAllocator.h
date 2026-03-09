#include <memory>
#include <vector>
#include <iostream>

template <typename T, std::size_t N>
class MyAllocator {
public:
    using value_type = T;

    MyAllocator() noexcept = default;

    template <typename U>
    MyAllocator(const MyAllocator<U, N>&) noexcept {}

    T* allocate(std::size_t n) {
        // Проверка: стандартные контейнеры могут запросить n > 1 
        if (used_ + n > N) {
            throw std::bad_alloc();
        }

        if (!buffer_) {
            buffer_ = reinterpret_cast<T*>(std::malloc(N * sizeof(T)));
        }

        T* result = buffer_ + used_;
        used_ += n;
        return result;
    }

    void deallocate(T* p, std::size_t n) noexcept {}

    template <typename U>
    struct rebind {
        using other = MyAllocator<U, N>;
    };

private:
    // Чтобы разные экземпляры MyAllocator<T, N> видели одну память
    static inline T* buffer_ = nullptr;
    static inline std::size_t used_ = 0;
};
