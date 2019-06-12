cmake_minimum_required(VERSION 3.13)
project(huffman)

set(CMAKE_CXX_STANDARD 14)

set(HUFFMAN
        huffman.hpp code.cpp reader.cpp)

set(UTILITY
        main.cpp)

add_library(engine STATIC ${HUFFMAN})
add_executable(utility ${UTILITY})

target_link_libraries(utility engine)