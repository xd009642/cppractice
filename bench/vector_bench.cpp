#include <chrono>
#include "vector.hpp"
#include <vector>
#include <utility>
#include <tuple>
#include <iostream>

bool first = true;

std::pair<double, double> push_back_bench() {
    // Create vector of ints then add a number of random values. Measure time
    const size_t INSERT_COUNT = 10'000'000;
    auto start = std::chrono::steady_clock::now();
    xd::vector<int> xdlist;
    for(size_t i=0; i<INSERT_COUNT; i++) {
        xdlist.push_back(rand());
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::nanoseconds xdiff = end - start;
    
    start = std::chrono::steady_clock::now();
    std::vector<int> sdlist;
    for(size_t i=0; i<INSERT_COUNT; i++) {
        sdlist.push_back(rand());
    }
    end = std::chrono::steady_clock::now();

    std::chrono::nanoseconds sdiff = end - start;
    if(first) {
        std::cout<<"Reserved space xd "<< xdlist.capacity()<<std::endl;
        std::cout<<"Reserved space std "<< sdlist.capacity()<<std::endl;
        first = false;
    }
    return std::make_pair(xdiff.count(), sdiff.count());
}


int main() {
    // use steady_clock so that clocks aren't adjusted by system.
    double xdtime = 0.0;
    double stdtime = 0.0;

    const size_t N_TRIALS = 10;
    
    for(int i=0; i<N_TRIALS; i++) {
        double xdtemp;
        double sttemp;
        std::tie(xdtemp, sttemp) = push_back_bench();
        xdtime += xdtemp/(static_cast<double>(N_TRIALS));
        stdtime += sttemp/(static_cast<double>(N_TRIALS));
    }

    std::cout<<"Average std::vector time "<<stdtime<<"ns"<<std::endl;
    std::cout<<"Average xd::vector time "<<xdtime<<"ns"<<std::endl;

}
