template<typename R, typename... Args>
class Slot {
    // Внутренний буфер для Small Object Optimization
    static constexpr size_t SBO_SIZE = 32;
    alignas(max_align_t) std::byte storage[SBO_SIZE];

    // Тип функции-обертки для вызова
    using InvokerSig = R(*)(void*, Args&&...);
    InvokerSig invoker = nullptr;

    // Указатель на функцию очистки (для деструкторов функторов)
    using DeleterSig = void(*)(void*);
    DeleterSig deleter = nullptr;

public:
    // Конструктор для лямбд и функторов
    template<typename F>
    Slot(F&& f) {
        using FunctorType = std::decay_t<F>;
        static_assert(sizeof(FunctorType) <= SBO_SIZE, "Functor too large for SBO");

        // Размещаем объект прямо в буфере (placement new)
        new (storage) FunctorType(std::forward<F>(f));

        // Статический инвокер
        invoker = [](void* s, Args&&... args) -> R {
            return (*reinterpret_cast<FunctorType*>(s))(std::forward<Args>(args)...);
        };

        deleter = [](void* s) {
            reinterpret_cast<FunctorType*>(s)->~FunctorType();
        };
    }

    R operator()(Args... args) {
        return invoker(storage, std::forward<Args>(args)...);
    }

    ~Slot() { if (deleter) deleter(storage); }
};

