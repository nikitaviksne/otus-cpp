#include <iostream>
#include <gtest/gtest.h>  
#include <gmock/gmock.h>  
#include <atomic>

struct AllocatorCounter 
{
	static inline std::atomic<size_t> count {0};
	static void reset() {count = 0;}
};

void* operator new(std::size_t size)
{
	AllocatorCounter::count++;
	return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
	std::free(ptr);
}

TEST(SlotTest, NoAllocationForSmallFuntors)
{
	AllocationCounter::reset();
	{
		//8 биь (небольшой захват) должен уместьтьбся в SBO_SIZE (32 бит)
		int x = 10;
		auto lambda = [x](int a) {return a+x;};
		
		Slot<int, int> slot(lambda);
		EXPECT_EQ(slot(5), 15);
		//аллокаций быть не должно
		EXCEPT_EQ(AllocationCounter::count.load(),0);
	}
};
int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
