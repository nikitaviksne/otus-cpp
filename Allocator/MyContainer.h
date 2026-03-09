#include <memory>
#include <iterator>
#include <initializer_list>


template <typename T>
struct Node {
    T value;
    Node* next;
    Node(const T& val) : value(val), next(nullptr) {}
};

template <typename T, typename Alloc = std::allocator<T>>
class MyContainer {
    using NodeT = Node<T>;
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<NodeT>;

public:
    MyContainer() : head_(nullptr), tail_(nullptr) {}

    void push_back(const T& value) {
        NodeT* newNode = std::allocator_traits<NodeAlloc>::allocate(alloc_, 1);
        std::allocator_traits<NodeAlloc>::construct(alloc_, newNode, value);

        if (!head_) {
            head_ = tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
    }

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        NodeT* curr;
        reference operator*() const { return curr->value; }
        Iterator& operator++() { if (curr) curr = curr->next; return *this; }
        bool operator!=(const Iterator& other) const { return curr != other.curr; }
    };

    Iterator begin() { return {head_}; }
    Iterator end() { return {nullptr}; }

private:
    NodeAlloc alloc_;
    NodeT* head_;
    NodeT* tail_;
};

