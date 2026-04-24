#include <benchmark/benchmark.h>
#include <signal.hpp>
#include <connection.hpp>
#include <vector>
#include <functional>
#include "boost/signals2.hpp"

static void BM_BoostSignals2(benchmark::State& state) {
    boost::signals2::signal<void(int)> sig;
    int sum = 0;
    
    std::vector<boost::signals2::connection> conns;
    for (int i = 0; i < state.range(0); ++i) {
        conns.push_back(sig.connect([&sum](int val) { sum += val; }));
    }

    for (auto _ : state) {
        sig(1); //вызов сигнала в Boost
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_BoostSignals2)->Arg(1)->Arg(10)->Arg(100);

// «наивная» реализация на std::function
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

// Тестируем Signal-Slot
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
