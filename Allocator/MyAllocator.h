#inlcude <memory>
#inlcude <type_traits>


template <typename  T>
class MyAllocator:public std::allocator<T>
{
public:
	template <tupename U>
	struct Rebind
	{
		using other = MyAllocator<U>;
	}

	using std::allocator<T>::allocator;

	void construct(T* _ptr) noexcept(std::is_nothow_default_consrtuctible_v<T>)
	{
		::new(static_cast<void*>(_ptr)) T;
	} 

	template <typename... Args>
	void construct(T* _ptr, Args&&... args ) noexcept(std::is_nothrow_default_constructible_v<T, Args...>)
	{
		::new(static_cast<void*>(_ptr) T(std::forward<Args>(_args)...);
	}
	
};
