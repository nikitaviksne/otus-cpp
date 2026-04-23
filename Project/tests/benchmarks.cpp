#include <benchmark/benchmark.h>
#include <signal_slot/signal.hpp>
#include <signal_slot/connection.hpp>
#include <vector>
#include <functional>

// 1. Тестируем «наивную» реализацию на std::function
static void BM_StdFunctionVector(benchmark::State& state) {
    std::vector<std::function<void(int)>> listeners;
    int sum = 0;
    for (int i = 0; i < state.range(0); ++i) {
        listeners.push_back([&sum](int val) { sum += val; });
    }

    for (auto _ : state) {
        for (auto& f : listeners) {
            f(1);
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_StdFunctionVector)->Arg(1)->Arg(10)->Arg(100);

// 2. Тестируем твой Signal-Slot
static void BM_IntrusiveSignal(benchmark::State& state) {
    Signal<int> signal;
    int sum = 0;
    
    // Создаем соединения (храним их в векторе, чтобы не удалились)
    std::vector<std::unique_ptr<Connection<int>>> conns;
    for (int i = 0; i < state.range(0); ++i) {
        conns.push_back(std::make_unique<Connection<int>>([&sum](int val) { 
            sum += val; 
        }));
        signal.connect(*conns.back());
    }

    for (auto _ : state) {
        signal.emit(1);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_IntrusiveSignal)->Arg(1)->Arg(10)->Arg(100);

BENCHMARK_MAIN();
