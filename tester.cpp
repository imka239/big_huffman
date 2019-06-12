//
// Created by ASUS on 12.06.2019.
//

#include <unknwn.h>
#include <fstream>
#include "huffman.hpp"
#include "gtest/gtest.h"
bool FileEquals(const std::string &file1, const std::string &file2) {

    std::ifstream input_stream1(file1, std::ios::binary);
    std::ifstream input_stream2(file2, std::ios::binary);
    std::vector<char> first; first.resize(32);
    std::vector<char> second; second.resize(32);
    do {
        input_stream1.read(first.data(), 32);
        first.resize(static_cast<unsigned int>(input_stream1.gcount()));
        input_stream2.read(second.data(), 32);
        second.resize(static_cast<unsigned int>(input_stream2.gcount()));
        if (first.size() != second.size()) {
            input_stream1.close();
            input_stream2.close();
            return false;
        }
        for (size_t i = 0; i < first.size(); i++) {
            if (first[i] != second[i]) {
                input_stream1.close();
                input_stream2.close();
                return false;
            }
        }
    } while (first.size() == 32 && second.size() == 32);
    input_stream1.close();
    input_stream2.close();
    return first.size() == second.size();
}

void code_decode(const std::string &t1, const std::string &t2, const std::string &t3) {
    huffman::file_handler file1(t1);
    file1.code(t2);
    huffman::file_handler file2(t2);
    file2.decode(t3);
}

TEST(just_test, war_and_peace) {
    std::string t1 = "test1/test_in.txt";
    std::string t2 = "test1/test_coded.bin";
    std::string t3 = "test1/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(corner_case, empty) {
    std::string t1 = "test2/test_in.txt";
    std::string t2 = "test2/test_coded.bin";
    std::string t3 = "test2/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(corner_case, only11111111) {
    std::string t1 = "test3/test_in.txt";
    std::string t2 = "test3/test_coded.bin";
    std::string t3 = "test3/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(corner_case, only00000000) {
    std::string t1 = "test4/test_in.txt";
    std::string t2 = "test4/test_coded.bin";
    std::string t3 = "test4/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(music, king_of_the_jungle) {
    std::string t1 = "test5/Ricardo.mp3";
    std::string t2 = "test5/test_coded.bin";
    std::string t3 = "test5/music_uncode.mp3";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}