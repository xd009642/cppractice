cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_BENCHMARKS "Build the benchmarks project" ON)
option(BUILD_TESTS "Build the tests project" ON)
option(BUILD_FUZZING "Include AFL fuzzer runs with build (this requires changing to afl-gcc etc)" OFF)

if(BUILD_BENCHMARKS)
    add_subdirectory(benches)
endif()
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
if(BUILD_FUZZING)
    add_subdirectory(fuzzing)
endif()
