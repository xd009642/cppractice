cmake_minimum_required(VERSION 3.14)

option(BUILD_BENCHMARKS "Build the benchmarks project" ON)
option(BUILD_TESTS "Build the tests project" ON)

if(BUILD_BENCHMARKS)
    add_subdirectory(benches)
endif()
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
