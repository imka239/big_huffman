cmake_minimum_required(VERSION 3.13)
project(huffman)

set(CMAKE_CXX_STANDARD 14)

set(HUFFMAN
        huffman.hpp code.cpp reader.cpp)

set(UTILITY
        maûin.cpp)

set(TESTER
        gtest/gtest.h gtest/gtest_main.cc gtest/gtest-all.cc tester.cpp)

add_library(engine STATIC ${HUFFMAN})
add_executable(utility ${UTILITY})
add_executable(tester ${TESTER})

target_link_libraries(utility engine)
target_link_libraries(tester engine)