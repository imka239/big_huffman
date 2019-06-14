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
    //bool (*p)(const std::string&, const std::string&) = &FileEquals;
    file2.decode(t3);
}

TEST(corner_case, only_aaaaaaaaa) {
    std::string t1 = "../test1/test_in.txt";
    std::string t2 = "../test1/test_coded.bin";
    std::string t3 = "../test1/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(corner_case, empty) {
    std::string t1 = "../test2/test_in.txt";
    std::string t2 = "../test2/test_coded.bin";
    std::string t3 = "../test2/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(corner_case, only11111111) {
    std::string t1 = "../test3/test_in.txt";
    std::string t2 = "../test3/test_coded.bin";
    std::string t3 = "../test3/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(corner_case, only00000000) {
    std::string t1 = "../test4/test_in.txt";
    std::string t2 = "../test4/test_coded.bin";
    std::string t3 = "../test4/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(corner_case, all_bytes) {
    std::string t1 = "../test5/test_in.txt";
    std::string t2 = "../test5/test_coded.bin";
    std::string t3 = "../test5/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(double_coding, war_and_peace) {
    std::string t1 = "../test6/test_in.txt";
    std::string t2 = "../test6/test_coded.bin";
    std::string t4 = "../test6/test_coded2.bin";
    std::string t5 = "../test6/test_out.txt";
    huffman::file_handler file1(t1);
    file1.code(t2);
    huffman::file_handler file2(t2);
    file2.code(t4);
    huffman::file_handler file3(t4);
    file3.decode(t2);
    huffman::file_handler file4(t2);
    file4.decode(t5);
    EXPECT_TRUE(FileEquals(t1, t5));
}

TEST(big_test, war_and_peace_full) {
    std::string t1 = "../test7/test_in.txt";
    std::string t2 = "../test7/test_coded.bin";
    std::string t3 = "../test7/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(big_test, just_big_text) {
    std::string t1 = "../test8/test_in.txt";
    std::string t2 = "../test8/test_coded.bin";
    std::string t3 = "../test8/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(four_code_decode_test, war_and_peace_full) {
    std::string t1 = "../test9/test_in.txt";
    std::string t2 = "../test9/test_coded.bin";
    std::string t4 = "../test9/test_coded2.bin";
    std::string t5 = "../test9/test_out.txt";
    huffman::file_handler file1(t1);
    file1.code(t2);
    huffman::file_handler file2(t2);
    file2.code(t4);
    huffman::file_handler file3(t4);
    file3.code(t2);
    huffman::file_handler file4(t2);
    file4.code(t4);
    huffman::file_handler file5(t4);
    file5.decode(t2);
    huffman::file_handler file6(t2);
    file6.decode(t4);
    huffman::file_handler file7(t4);
    file7.decode(t2);
    huffman::file_handler file8(t2);
    file8.decode(t5);
    EXPECT_TRUE(FileEquals(t1, t5));
}


TEST(music, king_of_the_jungle) {
    std::string t1 = "../test10/Ricardo.mp3";
    std::string t2 = "../test10/test_coded.bin";
    std::string t3 = "../test10/music_uncode.mp3";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(exception, war_and_peace_without_one_byte) {
    std::string t1 = "../test11/test_in.bin";
    std::string t2 = "../test11/test_out.txt";
    huffman::file_handler file1(t1);
    EXPECT_ANY_THROW(file1.decode(t2));
}

TEST(exception, no_file_out) {
    std::string t1 = "../test12/test_in.bin";
    std::string t2 = "../test12/test_out.txt";
    huffman::file_handler file1(t1);
    EXPECT_ANY_THROW(file1.decode(t2));
}

TEST(exception, no_file_in) {
    std::string t1 = "../test13/test_in.bin";
    std::string t2 = "../test13/test_out.txt";
    huffman::file_handler file1(t1);
    EXPECT_ANY_THROW(file1.decode(t2));
}

TEST(exception, empty_file) {
    std::string t1 = "../test14/test_in.bin";
    std::string t2 = "../test14/test_out.txt";
    huffman::file_handler file1(t1);
    EXPECT_ANY_THROW(file1.decode(t2));
}

//TEST(film, Batman) {
//    std::string t1 = "test15/Dk.mkv";
//    std::string t2 = "test15/test_bin.bin";
//    std::string t3 = "test15/Dark_Knight.mkv";
//    code_decode(t1, t2, t3);
//    EXPECT_TRUE(FileEquals(t1, t3));
//}