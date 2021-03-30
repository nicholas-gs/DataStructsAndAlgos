cmake_minimum_required(VERSION 3.0)
project(DataStructuresAndAlgorithms VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build tests" ON)

add_subdirectory(include/DataStructures)
add_subdirectory(include/SearchAlgorithms)
add_subdirectory(include/SortingAlgorithms)
add_subdirectory(include/Graphs)
add_subdirectory(include/FlowNetworks)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)