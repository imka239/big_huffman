//
// Created by ASUS on 12.06.2019.
//

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

void decode(const std::string& name_of_source, const std::string& to) {
    huffman::file_handler huff = huffman::file_handler();
    std::ifstream input_stream(name_of_source, std::ios::binary);
    std::ofstream output_stream(to, std::ios::binary);
    if (!input_stream.is_open()) {
        throw std::runtime_error("Source test opening failure");
    }
    if (!output_stream.is_open()) {
        throw std::runtime_error("Write test opening failure");
    }
    for (size_t i = 0; i < huff.frequency_of_chars.size(); i++) {
        uint64_t a = 0;
        input_stream.read(reinterpret_cast<char*> (&a), 8);
        huff.read_frequencies(a, i);
        if (input_stream.gcount() != 8) {
            throw std::runtime_error("Not enough frequencies");
        }
    }
    huff.build_tree();
    huff.read_buffer.resize(huff.buffer_max_size);
    do {
        input_stream.read(huff.read_buffer.data(), huff.buffer_max_size);
        huff.read_buffer.resize(static_cast<unsigned int>(input_stream.gcount()));
        std::vector<size_t > ans;
        huff.decode(huff.read_buffer, ans);
        for (auto j : ans) {
            output_stream.write(reinterpret_cast<const char *>(&j), 1);
        }
        ans.clear();
    } while (huff.read_buffer.size() == huff.buffer_max_size);
    for (size_t i = 0; i < huff.frequency_of_chars.size(); i++) {
        if (huff.frequency_of_chars[i] != 0) {
            throw std::runtime_error("Frequencies not equals to symbols in file");
        }
    }
    input_stream.close();
    output_stream.close();
}

void code(const std::string& name_of_source, const std::string& to) {
    huffman::file_handler huff = huffman::file_handler();
    std::ifstream input_stream(name_of_source, std::ios::binary);
    if (!input_stream.is_open()) {
        std::cout << name_of_source << std::endl;
        throw std::runtime_error("Source test opening failure");
    }
    huff.read_buffer.resize(huff.buffer_max_size);
    do {
        input_stream.read(huff.read_buffer.data(), huff.buffer_max_size);
        huff.read_buffer.resize(static_cast<unsigned int>(input_stream.gcount()));
        huff.frequency_update(huff.read_buffer);
    } while (huff.read_buffer.size() == huff.buffer_max_size);
    input_stream.close();
    huff.build_tree();
    std::ifstream input_stream_second(name_of_source, std::ios::binary);
    std::ofstream output_stream(to, std::ios::binary);
    if (!output_stream.is_open()) {
        throw std::runtime_error("Write test opening failure");
    }
    huff.write_frequencies(output_stream);
    if (!input_stream_second.is_open()) {
        throw std::runtime_error("Source test opening failure");
    }
    huff.read_buffer.resize(huff.buffer_max_size);
    do {
        input_stream_second.read(huff.read_buffer.data(), huff.buffer_max_size);
        huff.read_buffer.resize(static_cast<unsigned int>(input_stream_second.gcount()));
        std::vector<char> next_chars;
        huff.code(huff.read_buffer, next_chars);
        for (auto i : next_chars) {
            output_stream.write(reinterpret_cast<const char *>(&i), 1);
        }
        next_chars.clear();
    } while (huff.read_buffer.size() == huff.buffer_max_size);
    if (huff.pos != 0) {
        output_stream.write(reinterpret_cast<const char *>(&huff.next_char), 1);
    }
    input_stream_second.close();
    output_stream.close();
}


void code_decode(const std::string &t1, const std::string &t2, const std::string &t3) {
    code(t1, t2);
    decode(t2, t3);
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
    code(t1, t2);
    code(t2, t4);
    decode(t4, t2);
    decode(t2, t5);
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
    code(t1, t2);
    code(t2, t4);
    code(t4, t2);
    code(t2, t4);
    decode(t4, t2);
    decode(t2, t4);
    decode(t4, t2);
    decode(t2, t5);
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
    EXPECT_ANY_THROW(decode(t1, t2));
}

TEST(exception, no_file_out) {
    std::string t1 = "../test12/test_in.bin";
    std::string t2 = "../test12/test_out.txt";
    EXPECT_ANY_THROW(decode(t1, t2));
}

TEST(exception, no_file_in) {
    std::string t1 = "../test13/test_in.bin";
    std::string t2 = "../test13/test_out.txt";
    EXPECT_ANY_THROW(decode(t1, t2));
}

TEST(exception, empty_file) {
    std::string t1 = "../test14/test_in.bin";
    std::string t2 = "../test14/test_out.txt";
    EXPECT_ANY_THROW(decode(t1, t2));
}

TEST(big_test, only_coding) {
    std::string t1 = "../test8/test_in.txt";
    std::string t2 = "../test8/test_coded.bin";
    EXPECT_NO_THROW(code(t1, t2));
}


TEST(big_test, only_decoding) {
    std::string t1 = "../test8/test_coded.bin";
    std::string t2 = "../test8/test_out.txt";
    EXPECT_NO_THROW(decode(t1, t2));
}

TEST(big_test, enter_only) {
    std::string t1 = "../test15/test_in.txt";
    std::string t2 = "../test15/test_coded.bin";
    std::string t3 = "../test15/test_out.txt";
    code_decode(t1, t2, t3);
    EXPECT_TRUE(FileEquals(t1, t3));
}

TEST(exception, directory) {
    std::string t1 = "../test100";
    std::string t2 = "../test100/test_out.txt";
    EXPECT_ANY_THROW(decode(t1, t2));
}

//TEST(film, Batman) {
//    std::string t1 = "test15/Dk.mkv";
//    std::string t2 = "test15/test_bin.bin";
//    std::string t3 = "test15/Dark_Knight.mkv";
//    code_decode(t1, t2, t3);
//    EXPECT_TRUE(FileEquals(t1, t3));
//}