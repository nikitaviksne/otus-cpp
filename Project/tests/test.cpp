#include <iostream>
#include <gtest/gtest.h>  
#include <gmock/gmock.h>  
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>
#include "Slot.h"
#include "signal.hpp"

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

TEST(SignalTest, ZeroAllocationIntrusive) {
    Signal<int> signal;
    AllocatorCounter::reset();

    {
        int result = 0;
        // Создаем связь на стеке! 0 аллокаций в куче.
        Connection<int> conn([&](int x) { result = x; });
        
        signal.connect(conn);
        signal.emit(100);

        EXPECT_EQ(result, 100);
        EXPECT_EQ(AllocatorCounter::count.load(), 0); 
    } // Здесь conn выходит из области видимости
}

TEST(SlotTest, NoAllocationForSmallFuntors)
{
	AllocatorCounter::reset();
	{
		//8 биь (небольшой захват) должен уместьтьбся в SBO_SIZE (32 бит)
		int x = 10;
		auto lambda = [x](int a) {return a+x;};
		
		Slot<int, int> slot(lambda);
		EXPECT_EQ(slot(5), 15);
		//аллокаций быть не должно
		EXPECT_EQ(AllocatorCounter::count.load(),0);
	}
};

TEST(SignalMTTest, MassConnectDisconnect) {
    Signal<int> signal;
    std::atomic<bool> stop{false};
    std::atomic<size_t> call_count{0};

    // 1. Поток "Эмиттер" - постоянно рассылает сигнал
    std::thread emitter([&]() {
        int val = 0;
        while (!stop) {
            signal.emit(val++);
        }
    });

    // 2. Потоки "Слушатели" - постоянно подписываются и тут же отписываются
    auto listener_work = [&]() {
        while (!stop) {
            // Создаем соединение в блоке, чтобы оно тут же разрушалось
            {
                auto conn = signal.connect_managed([&](int x) {
                    call_count++;
                });
                // Небольшая пауза, чтобы эмиттер успел попасть в этот слот
                std::this_thread::yield(); 
            } 
            // Здесь conn разрушился, вызвав unlink()
        }
    };

    std::vector<std::thread> listeners;
    for (int i = 0; i < 4; ++i) {
        listeners.emplace_back(listener_work);
    }

    // Даем системе поработать под нагрузкой 2 секунды
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    stop = true;

    // Ждем завершения всех потоков
    emitter.join();
    for (auto& t : listeners) {
        t.join();
    }

    // Если мы здесь и программа не упала в SegFault — тест пройден!
    SUCCEED() << "No crash detected. Total calls: " << call_count.load();
}

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
