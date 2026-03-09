#include <memory>
#include <iterator>
#include <initializer_list>
#include <stdexcept>

template <typename T>
struct Node {
    T value;
    Node* next;
    Node(const T& val, Node* nxt) : value(val), next(nxt) {}
};

template <typename T, typename Alloc = std::allocator<T>>
class MyContainer {
    using NodeT = Node<T>;
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<NodeT>;

public:
    MyContainer() : head_(nullptr), tail_(nullptr) {}

    void push_back(const T& value) {
        // Выделяем и конструируем новый узел
        NodeT* newNode = std::allocator_traits<NodeAlloc>::allocate(alloc_, 1);
        std::allocator_traits<NodeAlloc>::construct(alloc_, newNode, value);

        if (!head_) {
            head_ = tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
    }

    // Итератор для обхода
    struct Iterator {
        NodeT* curr;
        T& operator*() { return curr->value; }
        Iterator& operator++() { curr = curr->next; return *this; }
        bool operator!=(const Iterator& other) const { return curr != other.curr; }
    };

    Iterator begin() { return {head_}; }
    Iterator end() { return {nullptr}; }

private:
    NodeAlloc alloc_;
    NodeT* head_;
    NodeT* tail_;
};
