#include <iostream>
#include "huffman.hpp"
#include <string>
#include <cstring>
#include <fstream>

std::vector<char> buffer;

int main(int32_t argc, char** argv) {
    std::string wr = "\tAvailable commands:\n"
                     "\t\t--code\t\tencode a test\n"
                     "\t\t--decode\tdecode a test\n";

    if (argc != 4) {
        std::cout << "\"Usage: huffman_archiver COMMAND SOURCE FILENAME\n" << wr;
        return 0;
    }
    if (std::strcmp(argv[1], "--code") == 0) {
        huffman::file_handler huff = huffman::file_handler();
        std::ifstream input_stream(argv[2], std::ios::binary);
        if (!input_stream.is_open()) {
            std::cout << argv[2] << std::endl;
            throw std::runtime_error("Source test opening failure");
        }
        buffer.resize(32);
        do {
            input_stream.read(buffer.data(), 32);
            buffer.resize(static_cast<unsigned int>(input_stream.gcount()));
            huff.frequency_update(buffer);
        } while (buffer.size() == 32);
        input_stream.close();
        huff.build_tree();
        std::ofstream output_stream(argv[3], std::ios::binary);
        if (!output_stream.is_open()) {
            throw std::runtime_error("Write test opening failure");
        }
        for (size_t i = 0; i < huff.frequency_of_chars.size(); i++) {
            output_stream.write(reinterpret_cast<char*>(&huff.frequency_of_chars[i]), 8);
        }
        std::ifstream input_stream_second(argv[2], std::ios::binary);
        if (!input_stream_second.is_open()) {
            std::cout << argv[2] << std::endl;
            throw std::runtime_error("Source test opening failure");
        }
        buffer.resize(32);
        do {
            input_stream_second.read(buffer.data(), 32);
            buffer.resize(static_cast<unsigned int>(input_stream_second.gcount()));
            std::vector<char> ans;
            huff.code(buffer, ans);
            for (auto i : ans) {
                output_stream.write(reinterpret_cast<const char *>(&i), 1);
            }
            ans.clear();
        } while (buffer.size() == 32);
        if (huff.pos != 0) {
            output_stream.write(reinterpret_cast<const char *>(&huff.next_char), 1);
        }
        input_stream_second.close();
        output_stream.close();
    } else if (std::strcmp(argv[1], "--decode") == 0) {
        huffman::file_handler huff = huffman::file_handler();
        std::ifstream input_stream(argv[2], std::ios::binary);
        if (!input_stream.is_open()) {
            std::cout << argv[2] << std::endl;
            throw std::runtime_error("Source test opening failure");
        }
        for (size_t i = 0; i < huff.frequency_of_chars.size(); i++) {
            uint64_t a = 0;
            input_stream.read(reinterpret_cast<char *>(&a), 8);
            huff.read_frequencies(a, i);
            if (input_stream.gcount() != 8) {
                throw std::runtime_error("Not enough frequencies");
            }
        }
        std::ofstream output_stream(argv[3], std::ios::binary);
        if (!output_stream.is_open()) {
            throw std::runtime_error("Write test opening failure");
        }
        huff.build_tree();
        buffer.resize(32);
        do {
            input_stream.read(buffer.data(), 32);
            buffer.resize(static_cast<unsigned int>(input_stream.gcount()));
            std::vector<size_t > ans;
            huff.decode(buffer, ans);
            for (auto j : ans) {
                output_stream.write(reinterpret_cast<const char *>(&j), 1);
            }
            ans.clear();
        } while (buffer.size() == 32);
        for (size_t i = 0; i < huff.frequency_of_chars.size(); i++) {
            if (huff.frequency_of_chars[i] != 0) {
                throw std::runtime_error("Frequencies not equals to symbols in file");
            }
        }
        input_stream.close();
        output_stream.close();
    }
    return 0;
}