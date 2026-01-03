#include <memory>
#include <iterator>
#include <initializer_list>
#include <stdexcept>

template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
private:
	// Структура узла списка
	struct Node {
		T data;
		Node* next;
		
		template<typename... Args>
		Node(Args&&... args) 
			: data(std::forward<Args>(args)...), next(nullptr) {}
	};

	// Rebind аллокатора для узлов
	using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
	using NodeTraits = std::allocator_traits<NodeAllocator>;

	Node* head = nullptr;
	Node* tail = nullptr;
	size_t size_ = 0;
	NodeAllocator alloc;

public:
	using value_type = T;
	using allocator_type = Allocator;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;

	template <bool IsConst>
	
	class ListIterator {
	private:
		using node_ptr = std::conditional_t<IsConst, const Node*, Node*>;
		using reference_type = std::conditional_t<IsConst, const_reference, reference>;
		
		node_ptr current;

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		
		// Используем уникальные имена для типов итератора
		using iterator_pointer = std::conditional_t<IsConst, const_pointer, pointer>;
		using iterator_reference = reference_type;
		
		ListIterator(node_ptr node = nullptr) : current(node) {}
		
		// Конвертация из неконстантного в константный итератор
		template<bool WasConst, typename = std::enable_if_t<IsConst && !WasConst>>
		ListIterator(const ListIterator<WasConst>& other) : current(other.current) {}
		
		iterator_reference operator*() const { return current->data; }
		iterator_pointer operator->() const { return &current->data; }
		
		ListIterator& operator++() {
			current = current->next;
			return *this;
		}
		
		ListIterator operator++(int) {
			ListIterator temp = *this;
			++(*this);
			return temp;
		}
		
		bool operator==(const ListIterator& other) const { return current == other.current; }
		bool operator!=(const ListIterator& other) const { return current != other.current; }
		
		node_ptr node() const { return current; }
		
		// для доступа к current
		template<bool OtherConst>
		friend class ListIterator;
	};
	using iterator = ListIterator<false>;
	using const_iterator = ListIterator<true>;
	// Можно получить размер блока аллокатора если он есть
    	template<typename A = Allocator>
    	static constexpr auto get_allocator_block_size() {
        	if constexpr (requires { A::BLOCKSIZE; }) {
            		printf("Вызвался метод get_block_size из контейнера\n");
			return A::BLOCKSIZE;
			//return 10;
        	} 
		else {
			printf("Не вызвался метод get_block_size из контейнера, кол-во элементов по умолчанию\n");
	            	return 10; // аллокатор без размера блока (с размером блока по умолчанию)
        	}
    	}

	// Конструктор и деструктор
	MyContainer() : alloc(Allocator()) {
		printf("В конструкторе контейнера, количество элементов в конструкторе аллокатора = %d\n", get_allocator_block_size());
		//MyContainer(get_allocator_block_size(), T, Allocator());
	}
	
	explicit MyContainer(const Allocator& alloc) : alloc(alloc) {}
	
	MyContainer(size_type count, const T& value, const Allocator& alloc = Allocator())
		: alloc(alloc) {
		for (size_type i = 0; i < count; ++i) {
			push_back(value);
		}
	}
	
	explicit MyContainer(size_type count, const Allocator& alloc = Allocator())
		: alloc(alloc) {
		for (size_type i = 0; i < count; ++i) {
			emplace_back();
		}
	}
	
	template<typename InputIterator>
	MyContainer(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
		: alloc(alloc) {
		for (auto it = first; it != last; ++it) {
			push_back(*it);
		}
	}
	
	MyContainer(std::initializer_list<T> init, const Allocator& alloc = Allocator())
		: alloc(alloc) {
		for (const auto& item : init) {
			push_back(item);
		}
	}
	
	// Копирующий конструктор
	MyContainer(const MyContainer& other)
		: alloc(NodeTraits::select_on_container_copy_construction(other.alloc)) {
		for (const auto& item : other) {
			push_back(item);
		}
	}
	
	// Перемещающий конструктор
	MyContainer(MyContainer&& other) noexcept
		: head(other.head), tail(other.tail), size_(other.size_), alloc(std::move(other.alloc)) {
		other.head = other.tail = nullptr;
		other.size_ = 0;
	}
	
	// Деструктор
	~MyContainer() {
		clear();
	}

	//  Операторы присваивания 
	MyContainer& operator=(const MyContainer& other) {
		if (this != &other) {
			clear();
			for (const auto& item : other) {
				push_back(item);
			}
		}
		return *this;
	}
	
	MyContainer& operator=(MyContainer&& other) noexcept {
		if (this != &other) {
			clear();
			head = other.head;
			tail = other.tail;
			size_ = other.size_;
			alloc = std::move(other.alloc);
			other.head = other.tail = nullptr;
			other.size_ = 0;
		}
		return *this;
	}
	
	MyContainer& operator=(std::initializer_list<T> init) {
		clear();
		for (const auto& item : init) {
			push_back(item);
		}
		return *this;
	}

	//  Основные методы 
	void push_back(const T& value) {
		emplace_back(value);
	}
	
	void push_back(T&& value) {
		emplace_back(std::move(value));
	}
	
	template<typename... Args>
	reference emplace_back(Args&&... args) {
		Node* new_node = NodeTraits::allocate(alloc, 1);
		try {
			NodeTraits::construct(alloc, new_node, std::forward<Args>(args)...);
		} catch (...) {
			NodeTraits::deallocate(alloc, new_node, 1);
			throw;
		}
		
		if (tail) {
			tail->next = new_node;
			tail = new_node;
		} else {
			head = tail = new_node;
		}
		++size_;
		return new_node->data;
	}

	//  Обход контейнера 
	iterator begin() noexcept { return iterator(head); }
	const_iterator begin() const noexcept { return const_iterator(head); }
	const_iterator cbegin() const noexcept { return const_iterator(head); }
	
	iterator end() noexcept { return iterator(nullptr); }
	const_iterator end() const noexcept { return const_iterator(nullptr); }
	const_iterator cend() const noexcept { return const_iterator(nullptr); }

	//  Вспомогательные методы 
	bool empty() const noexcept { return size_ == 0; }
	size_type size() const noexcept { return size_; }
	
	reference front() {
		if (empty()) throw std::runtime_error("Container is empty");
		return head->data;
	}
	
	const_reference front() const {
		if (empty()) throw std::runtime_error("Container is empty");
		return head->data;
	}
	
	reference back() {
		if (empty()) throw std::runtime_error("Container is empty");
		return tail->data;
	}
	
	const_reference back() const {
		if (empty()) throw std::runtime_error("Container is empty");
		return tail->data;
	}

	void clear() noexcept {
		Node* current = head;
		while (current) {
			Node* next = current->next;
			NodeTraits::destroy(alloc, current);
			NodeTraits::deallocate(alloc, current, 1);
			current = next;
		}
		head = tail = nullptr;
		size_ = 0;
	}

	allocator_type get_allocator() const noexcept {
		return allocator_type(alloc);
	}

	//  Дополнительные методы для удобства 
	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last) {
		clear();
		for (auto it = first; it != last; ++it) {
			push_back(*it);
		}
	}
	
	void assign(std::initializer_list<T> init) {
		clear();
		for (const auto& item : init) {
			push_back(item);
		}
	}
	
	void assign(size_type count, const T& value) {
		clear();
		for (size_type i = 0; i < count; ++i) {
			push_back(value);
		}
	}
};
