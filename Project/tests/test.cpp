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

class Receiver {
public:
    int count = 0;
    void handler(int delta) { count += delta; }
};

TEST(SignalTest, MemberFunctionSupport) {
    Signal<int> signal;
    Receiver rx;
    
    // связь с методом класса через лямбду
    auto conn = signal.connect_managed([&rx](int x) { rx.handler(x); });
    
    signal.emit(10);
    EXPECT_EQ(rx.count, 10);
}

TEST(SignalTest, ZeroAllocationIntrusive) {
    Signal<int> signal;
    AllocatorCounter::reset();

    {
        int result = 0;
        // Создаем на стеке 0 аллокаций в куче.
        Connection<int> conn([&](int x) { result = x; });
        
        signal.connect(conn);
        signal.emit(100);

        EXPECT_EQ(result, 100);
        EXPECT_EQ(AllocatorCounter::count.load(), 0); 
    } // conn выходит из области видимости
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

    //Поток "Эмиттер" - постоянно рассылает сигнал
    std::thread emitter([&]() {
        int val = 0;
        while (!stop) {
            signal.emit(val++);
        }
    });

    //Потоки "Слушатели" - постоянно подписываются и тут же отписываются
    auto listener_work = [&]() {
        while (!stop) {
            // Создаем соединение в блоке, чтобы оно тут же разрушалось
            {
                auto conn = signal.connect_managed([&](int x) {
                    call_count++;
                });
                // пауза, чтобы эмиттер успел попасть в этот слот
                std::this_thread::yield(); 
            } 
            // conn разрушился, вызвав unlink()
        }
    };

    std::vector<std::thread> listeners;
    for (int i = 0; i < 4; ++i) {
        listeners.emplace_back(listener_work);
    }

    // поработаем под нагрузкой 2 секунды
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

TEST(SignalMTTest, ConcurrentConnect) {
    Signal<int> signal;
    std::atomic<int> total_received{0};
    const int threads_count = 4;
    const int conns_per_thread = 1000;
    
    std::vector<std::thread> threads;
    // чтобы они не удалились раньше времени
    std::vector<std::vector<std::unique_ptr<Connection<int>>>> all_conns(threads_count);

    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back([&, i]() {
            for (int j = 0; j < conns_per_thread; ++j) {
                all_conns[i].push_back(signal.connect_managed([&](int val) {
                    total_received++;
                }));
            }
        });
    }

    for (auto& t : threads) t.join();

    // Теперь проверяем, что все 4000 соединений работают
    signal.emit(1);
    EXPECT_EQ(total_received.load(), threads_count * conns_per_thread);
}


int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS(); 
}
