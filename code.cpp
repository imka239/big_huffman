#include "huffman.hpp"
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace huffman;
std::vector<unsigned char> coded;
std::vector<std::vector<unsigned char> > ans_in_char;
std::vector<size_t> size;
std::vector<std::vector<size_t > > graph;
void file_handler::dfs(size_t now, size_t val) {
    if (now < frequency_of_chars.size()) {
        for (unsigned char i : coded) {
            ans_in_char[now].push_back(i);
        }
        if (val == 0) {
            size[now] = 8;
        } else {
            size[now] = val;
        }
        return;
    }
    if (val == 0) {
        coded.push_back(0);
        dfs(graph[now][0], 1);
        coded[coded.size() - 1] += power_of_two[0];
        dfs(graph[now][1], 1);
        coded.pop_back();
    } else {
        dfs(graph[now][0], (val + 1) % 8);
        coded[coded.size() - 1] += power_of_two[val];
        dfs(graph[now][1], (val + 1) % 8);
        coded[coded.size() - 1] -= power_of_two[val];
    }
}

void file_handler::code(std::string to) {
    read_coding();
    std::set<std::pair<uint64_t, size_t > > que;
    graph.clear();
    graph.resize(512);
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        que.insert({frequency_of_chars[i], i});
    }
    uint64_t next = frequency_of_chars.size();
    while(que.size() > 1) {
        auto first = *(que.begin());
        que.erase(first);
        auto second = *(que.begin());
        que.erase(second);
        graph[next].push_back(first.second);
        graph[next].push_back(second.second);
        que.insert({first.first + second.first, next});
        next++;
    }
    ans_in_char.clear();
    ans_in_char.resize(frequency_of_chars.size());
    size.resize(frequency_of_chars.size());
    coded.clear();
    dfs(510, 0);
    std::ifstream input_stream(name_of_source, std::ios::binary);
    std::ofstream output_stream(to, std::ios::binary);
    if (!output_stream.is_open()) {
        throw std::runtime_error("Write test opening failure");
    }
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        output_stream.write(reinterpret_cast<char*>(&frequency_of_chars[i]), 8);
    }
    if (!input_stream.is_open()) {
        throw std::runtime_error("Source test opening failure");
    }
    read_buffer.resize(buffer_max_size);
    unsigned char next_char = 0;
    size_t pos = 0;
    do {
        read_in_buffer(input_stream);
        for (auto i : read_buffer) {
            auto j_uns = static_cast<unsigned char>(i);
            for (size_t j = 0; j < ans_in_char[j_uns].size(); j++) {
                size_t sz = 8;
                if (j == ans_in_char[j_uns].size() - 1) {
                    sz = size[j_uns];
                }
                if (pos + sz >= 8) {
                    for (size_t h = pos; h < 8; h++) {
                        if (ans_in_char[j_uns][j] & power_of_two[h - pos]) {
                            next_char += power_of_two[h];
                        }
                    }
                    output_stream.write(reinterpret_cast<const char *>(&next_char), 1);
                    next_char = 0;
                    for (size_t h = 8; h < pos + sz; h++) {
                        if (ans_in_char[j_uns][j] & power_of_two[h - pos]) {
                            next_char += power_of_two[h - 8];
                        }
                    }
                    pos = (pos + sz) % 8;
                } else {
                    for (size_t h = pos; h < pos + sz; h++) {
                        if (ans_in_char[j_uns][j] & power_of_two[h - pos]) {
                            next_char += power_of_two[h];
                        }
                    }
                    pos = (pos + sz);
                }
            }
        }
    } while (read_buffer.size() == buffer_max_size);
    if (pos != 0) {
        output_stream.write(reinterpret_cast<const char *>(&next_char), 1);
    }
    input_stream.close();
    output_stream.close();
}
//for (size_t j = 0; j < ans_in_char[j_uns].size(); j++) {
//                output_stream.write(reinterpret_cast<const char *>(&ans_in_char[j_uns][j]), 1);
//                next_char = 0;
//                pos = 0;
//            }


void file_handler::decode(std::string to) {
    std::ifstream input_stream(name_of_source, std::ios::binary);
    std::ofstream output_stream(to, std::ios::binary);
    if (!input_stream.is_open()) {
        throw std::runtime_error("Source test opening failure");
    }
    if (!output_stream.is_open()) {
        throw std::runtime_error("Write test opening failure");
    }
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        input_stream.read(reinterpret_cast<char*> (&frequency_of_chars[i]), 8);
        if (input_stream.gcount() != 8) {
            throw std::runtime_error("Not enough frequencies");
        }
    }
    std::set<std::pair<uint64_t, size_t > > que;
    graph.clear();
    graph.resize(512);
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        que.insert({frequency_of_chars[i], i});
    }
    uint64_t next = frequency_of_chars.size();
    while(que.size() > 1) {
        auto first = *(que.begin());
        que.erase(first);
        auto second = *(que.begin());
        que.erase(second);
        graph[next].push_back(first.second);
        graph[next].push_back(second.second);
        que.insert({first.first + second.first, next});
        next++;
    }
    ans_in_char.clear();
    ans_in_char.resize(frequency_of_chars.size());
    size.resize(frequency_of_chars.size());
    coded.clear();
    dfs(510, 0);
    size_t now = 510;
    read_buffer.resize(buffer_max_size);
    try {
        do {
            bool exit = false;
            read_in_buffer(input_stream);
            for (auto j : read_buffer) {
                auto j_uns = static_cast<unsigned char>(j);
                auto byte_of_j = 0;
                for (size_t i = 0; i < 8; i++) {
                    size_t ind = 0;
                    if (j_uns >= power_of_two[byte_of_j]) {
                        ind = 1;
                        j_uns -= power_of_two[byte_of_j];
                    }
                    byte_of_j++;
                    now = graph[now][ind];
                    if (now < frequency_of_chars.size()) {
                        if (frequency_of_chars[now] == 0) {
                            throw true;
                        }
                        frequency_of_chars[now]--;
                        output_stream.write(reinterpret_cast<char *>(&now), 1);
                        now = 510;
                    }
                }
            }
        } while (read_buffer.size() == buffer_max_size);
    } catch (bool x) {}
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        if (frequency_of_chars[i] != 0) {
            throw std::runtime_error("Frequencies not equals to symbols in file");
        }
    }
    input_stream.close();
    output_stream.close();
}

//--decode test2.bin test2_out.txt
//--code test1.txt test1_out.bin