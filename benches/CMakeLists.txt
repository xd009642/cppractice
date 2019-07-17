cmake_minimum_required(VERSION 3.14)
project(benches)

find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)

include_directories(../include)
include_directories(${benchmark_INCLUDE_DIRS})

add_executable(benchmarks vector_bench.cpp)
target_link_libraries(benchmarks benchmark ${CMAKE_THREAD_LIBS_INIT})

if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    target_link_libraries(benchmarks Shlwapi)
endif()
