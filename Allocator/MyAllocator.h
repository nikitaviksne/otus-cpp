#include <memory>
#include <type_traits>
#include <iostream>
#include <vector>

template <typename  T>
class MyAllocator:public std::allocator<T>
{
public:
	template <typename U>
	struct Rebind
	{
		using other = MyAllocator<U>;
	};

	using std::allocator<T>::allocator;
	
	T* allocate(size_t n) 
	{
        
        	return std::allocator<T>::allocate(n);
    	}
	
	void deallocate(T* p, size_t n) 
	{
        	std::allocator<T>::deallocate(p, n);
    	}

	template<typename U, typename... Args>
    	void construct(U* p, Args&&... args) {
        	::new((void*)p) U(std::forward<Args>(args)...);
    	}
    
    	template<typename U>
    	void destroy(U* p) 
	{
        	p->~U();
    	}
};


// Аллокатор с параметром конструктора
template <typename T>
class RuntimeBlockAllocator : public std::allocator<T> {
private:
    size_t block_size_;
    
    struct MemoryBlock {
        T* data;
        size_t used;
        size_t capacity;
        
        MemoryBlock(size_t cap) : capacity(cap), used(0) {
            data = static_cast<T*>(::operator new(sizeof(T) * cap));
            //std::cout << "[Alloc] Allocated block of " << cap << " elements\n";
        }
        
        ~MemoryBlock() {
            ::operator delete(data);
            //std::cout << "[Alloc] Freed block of " << capacity << " elements\n";
        }
        
        bool has_space() const { return used < capacity; }
        T* allocate() { return has_space() ? &data[used++] : nullptr; }
    };
    
    std::vector<std::unique_ptr<MemoryBlock>> blocks;
    
public:
    using value_type = T;
    
    template <typename U>
    struct rebind {
        using other = RuntimeBlockAllocator<U>;
    };
    
    // Конструктор с параметром размера блока
    explicit RuntimeBlockAllocator(size_t block_size = 10) 
        : block_size_(block_size) {
        //std::cout << "[Alloc] Created with block size: " << block_size_ << "\n";
    }
    
    // Конструктор копирования
    RuntimeBlockAllocator(const RuntimeBlockAllocator& other) noexcept 
        : block_size_(other.block_size_) {}
    
    template <typename U>
    RuntimeBlockAllocator(const RuntimeBlockAllocator<U>& other) 
        : block_size_(other.get_block_size()) {}
    
    size_t get_block_size() const { return block_size_; }
    
    T* allocate(size_t n) {
        //std::cout << "[Alloc] Request for " << n << " elements (block size: " << block_size_ << ")\n";
        
        if (n > block_size_) {
            //std::cout << "[Alloc] Large allocation: " << n << " elements\n";
            return static_cast<T*>(::operator new(sizeof(T) * n));
        }
        
        for (auto& block : blocks) {
            if (block->has_space()) {
                return block->allocate();
            }
        }
        
        auto new_block = std::make_unique<MemoryBlock>(block_size_);
        T* ptr = new_block->allocate();
        blocks.push_back(std::move(new_block));
        return ptr;
    }
    
    void deallocate(T* p, size_t n) {
        //std::cout << "[Alloc] Deallocating " << n << " elements\n";
        
        if (n > block_size_) {
            ::operator delete(p);
        }
    }
};

//фабричная функция для создания аллокатора RuntimeBlockAllocator 
template<typename T>
auto make_block_allocator(size_t block_size = 10) {
    return RuntimeBlockAllocator<T>(block_size);
}
