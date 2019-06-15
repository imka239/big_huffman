#include <utility>


#include <fstream>
#include <iostream>
#include "huffman.hpp"

using namespace huffman;

file_handler::file_handler() {
    frequency_of_chars.clear();
    read_buffer.clear();
    frequency_of_chars.resize(256);
    read_buffer.resize(buffer_max_size);
    next_char = 0;
    pos = 0;
    now = 510;
}

void file_handler::frequency_update(std::vector<char> &x) {
    for (auto i : x) {
        auto j_uns = static_cast<unsigned char>(i);
        ++frequency_of_chars[j_uns];
    }
}