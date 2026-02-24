#include <iostream>
#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <algorithm>

template <typename T>
struct is_container {
    //Исключаем строку
    static constexpr bool is_not_string = !std::is_same<T, std::string>::value;

    // проверяем наличие итераторов и методов (специфичных для контейнеров)
    template <typename C>
    static auto test(int) -> decltype(
        std::declval<typename C::value_type>(),
        std::declval<typename C::iterator>(),
        std::declval<C>().begin(), 
        std::declval<C>().end(), 
        std::true_type{}
    );

    template <typename C>
    static std::false_type test(...);

    static constexpr bool value = is_not_string && decltype(test<T>(0))::value;
};

// --- 1. Целочисленные типы ---
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_ip(T val) {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&val);
    for (int i = sizeof(T) - 1; i >= 0; --i) {
        std::cout << static_cast<int>(ptr[i]) << (i == 0 ? "" : ".");
    }
    std::cout << std::endl;
}

// --- 2. Строки ---
template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
print_ip(const T& val) {
    std::cout << val << std::endl;
}

// --- 3. Контейнеры (vector, list) ---
template <typename T>
typename std::enable_if<
    is_container<T>::value, 
    void>::type
print_ip(const T& container) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        if (it != container.begin()) std::cout << ".";
        std::cout << *it;
    }
    std::cout << std::endl;
}

// --- 4. Опционально: Кортежи (Tuple) ---
template <typename T, typename... Ts>
struct all_same : std::true_type {};

template <typename T, typename First, typename... Rest>
struct all_same<T, First, Rest...> 
    : std::integral_constant<bool, std::is_same<T, First>::value && all_same<T, Rest...>::value> {};

// Рекурсивный вывод элементов кортежа
template<size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
print_tuple(const std::tuple<Tp...>&) {}

template<size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
print_tuple(const std::tuple<Tp...>& t) {
    if (I > 0) std::cout << ".";
    std::cout << std::get<I>(t);
    print_tuple<I + 1, Tp...>(t);
}

// Основная функция для tuple
template <typename... Args>
typename std::enable_if<all_same<Args...>::value, void>::type
print_ip(const std::tuple<Args...>& t) {
    print_tuple(t);
    std::cout << std::endl;
}
