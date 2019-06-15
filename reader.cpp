#include <utility>


#include <fstream>
#include <iostream>
#include "huffman.hpp"

using namespace huffman;

file_handler::file_handler(std::string file) : name_of_source(file) {
    frequency_of_chars.clear();
    read_buffer.clear();
    frequency_of_chars.resize(256);
    read_buffer.resize(buffer_max_size);
    next_char = 0;
    pos = 0;
    now = 510;
}
file_handler::file_handler() : name_of_source("DIMA") {
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

void file_handler::read_coding() {
    std::ifstream input_stream(name_of_source, std::ios::binary);
    if (!input_stream.is_open()) {
        std::cout << name_of_source << std::endl;
        throw std::runtime_error("Source test opening failure");
    }
    read_buffer.resize(buffer_max_size);
    do {
        input_stream.read(read_buffer.data(), buffer_max_size);
        read_buffer.resize(static_cast<unsigned int>(input_stream.gcount()));
        frequency_update(read_buffer);
    } while (read_buffer.size() == buffer_max_size);
    input_stream.close();
}