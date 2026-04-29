#include <benchmark/benchmark.h>
#include <QObject>

// Класс-отправитель
class QtSender : public QObject {
    Q_OBJECT
signals:
    void sendValue(int v); // Определение сигнала (реализацию сделает MOC)
};

// Класс-получатель
class QtReceiver : public QObject {
    Q_OBJECT
public slots:
    void onValue(int v) {
        benchmark::DoNotOptimize(v);
    }
};

static void BM_QtSignalSlot(benchmark::State& state) {
    QtSender sender;
    // Создаем нужное кол-во приемников
    std::vector<std::unique_ptr<QtReceiver>> receivers;
    for (int i = 0; i < state.range(0); ++i) {
        receivers.push_back(std::make_unique<QtReceiver>());
        QObject::connect(&sender, &QtSender::sendValue, 
                         receivers.back().get(), &QtReceiver::onValue);
    }

    for (auto _ : state) {
        emit sender.sendValue(42);
    }
}
BENCHMARK(BM_QtSignalSlot)->Arg(1)->Arg(10)->Arg(100);

BENCHMARK_MAIN();
#include "bench_qt.moc"
