//
// Created by ASUS on 10.06.2019.
//

#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include <string>
#include <vector>

namespace huffman{

    struct file_handler {
        std::vector<uint64_t> frequency_of_chars;
        const unsigned char power_of_two[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    private:
        std::vector<unsigned char> coded;
        std::vector<std::vector<unsigned char> > ans_in_char;
        std::vector<size_t> size;
        std::vector<std::vector<size_t > > graph;
    public:
        unsigned char next_char;
        size_t pos;
    private:
        size_t now;

    private:
        void dfs(size_t, size_t);
    public:
        file_handler();
        void build_tree();
        void write_frequencies(std::ofstream& );
        void read_frequencies(uint64_t, size_t);
        void frequency_update(std::vector<char>& );
        void code(std::vector<char>&, std::vector<char>&);
        void decode(std::vector<char>&, std::vector<size_t>&);
    };
}

#endif //HUFFMAN_CODE_H
