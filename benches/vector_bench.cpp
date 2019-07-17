#include <benchmark/benchmark.h>
#include "vector.hpp"
#include <vector>


static void xdvec_push_back(benchmark::State& state) {
    xd::vector<int> vec;
    for(auto _ : state) {
        vec.push_back(10);
    }
}


static void stdvec_push_back(benchmark::State& state) {
    std::vector<int> vec;
    for(auto _ : state) {
        vec.push_back(10);
    }
}

BENCHMARK(stdvec_push_back);
BENCHMARK(xdvec_push_back);
BENCHMARK_MAIN();
