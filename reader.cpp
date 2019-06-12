#include <utility>


#include <fstream>
#include "huffman.hpp"

using namespace huffman;

file_handler::file_handler(std::string file) : name_of_source(file) {
    frequency_of_chars.resize(256);
    read_buffer.resize(buffer_max_size);
}

void file_handler::read_in_buffer(std::ifstream &from) {
    from.read(read_buffer.data(), buffer_max_size);
    read_buffer.resize(static_cast<unsigned int>(from.gcount()));
}

void file_handler::read_coding() {
    std::ifstream input_stream(name_of_source, std::ios::binary);
    if (!input_stream.is_open()) {
        throw std::runtime_error("Source test opening failure");
    }
    read_buffer.resize(buffer_max_size);
    do {
        read_in_buffer(input_stream);
        for (auto i : read_buffer) {
            auto j_uns = static_cast<unsigned char>(i);
            ++frequency_of_chars[j_uns];
        }
    } while (read_buffer.size() == buffer_max_size);
    input_stream.close();
}