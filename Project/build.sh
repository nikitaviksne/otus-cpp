#!/bin/bash
rm -rf Project/build 
cmake -B Project/build -S ./Project/ && cmake --build Project/build
cmake --build Project/build --target test

# Запускаем бенчмарки
./Project/build/run_benchmarks
./Project/build/bench_qt
