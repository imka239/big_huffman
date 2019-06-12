#include <iostream>
#include "huffman.hpp"
#include <string>
#include <cstring>

int main(int32_t argc, char** argv) {
    std::string wr = "\tAvailable commands:\n"
                     "\t\t--code\t\tencode a test\n"
                     "\t\t--decode\tdecode a test\n";

    if (argc != 4) {
        std::cout << "\"Usage: huffman_archiver COMMAND SOURCE FILENAME\n" << wr;
        return 0;
    }
    if (std::strcmp(argv[1], "--code") == 0) {
        huffman::file_handler file(argv[2]);
        try {
            file.code(argv[3]);
        } catch (std::runtime_error &e) {
            std::cout << "Coding error: \n\t" << e.what() << "\n";
            return 1;
        }
    } else if (std::strcmp(argv[1], "--decode") == 0) {
        huffman::file_handler file(argv[2]);
        try {
            file.decode(argv[3]);
        } catch (std::runtime_error &e) {
            std::cout << "Coding error: \n\t" << e.what() << "\n";
            return 1;
        }
    }
    return 0;
}