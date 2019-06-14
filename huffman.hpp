//
// Created by ASUS on 10.06.2019.
//

#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include <string>
#include <vector>

namespace huffman{

    struct file_handler {
        std::vector<char> read_buffer;
        std::vector<uint64_t> frequency_of_chars;
        std::string name_of_source;
        size_t buffer_max_size = 128;
        unsigned char power_of_two[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    private:
        void read_in_buffer(std::ifstream &);
        void dfs(size_t, size_t);
    public:
        explicit file_handler(std::string);
        void read_coding();
        void code(std::string);
        void decode(std::string);
    };
}

#endif //HUFFMAN_CODE_H
