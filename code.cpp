#include "huffman.hpp"
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace huffman;

std::vector<bool> coded;
std::vector<std::vector<bool> > ans;
std::vector<std::vector<unsigned char> > ans_in_char;
std::vector<std::vector<size_t > > graph;
void file_handler::dfs(size_t now) {
    if (now < frequency_of_chars.size()) {
        for (int i = 0; i < coded.size(); i++) {
            ans[now].push_back(coded[i]);
        }
        return;
    }
    coded.push_back(false);
    dfs(graph[now][0]);
    coded.pop_back();
    coded.push_back(true);
    dfs(graph[now][1]);
    coded.pop_back();
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
    ans.clear();
    ans.resize(frequency_of_chars.size());
    dfs(510);
    ans_in_char.clear();
    ans_in_char.resize(frequency_of_chars.size());
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        size_t pos = 0;
        while (pos != ans[i].size()) {
            unsigned char from_bool = 0;
            for (size_t j = pos; j < std::min(pos + 8, ans[i].size()); j++) {
                from_bool *= 2;
                from_bool += ans[i][j];
            }
            while (pos + 8 > ans[i].size()) {
                from_bool *= 2;
                pos--;
            }
            pos = std::min(pos + 8, ans[i].size());
            ans_in_char[i].push_back(from_bool);
        }
    }
    std::ifstream input_stream(name_of_source, std::ios::binary);
    std::ofstream output_stream(to, std::ios::binary);
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        output_stream.write(reinterpret_cast<char*>(&frequency_of_chars[i]), 8);
    }
    if (!input_stream.is_open()) {
        throw std::runtime_error("Source test opening failure");
    }
    read_buffer.resize(buffer_max_size);
    do {
        read_in_buffer(input_stream);
        for (auto i : read_buffer) {
            auto j_uns = static_cast<unsigned char>(i);
            output_stream.write(reinterpret_cast<const char *>(ans_in_char[j_uns].data()), ans_in_char[j_uns].size());
        }
    } while (read_buffer.size() == buffer_max_size);
    input_stream.close();
    output_stream.close();
}

void file_handler::decode(std::string to) {
    std::ifstream input_stream(name_of_source, std::ios::binary);
    std::ofstream output_stream(to, std::ios::binary);
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        input_stream.read(reinterpret_cast<char*> (&frequency_of_chars[i]), 8);
        if (input_stream.gcount() != 8) {
            std::cout << 1 << std::endl;
        }
    }
    std::set<std::pair<uint64_t, size_t > > que;
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
    ans.resize(frequency_of_chars.size());
    dfs(510);
    size_t now = 510;
    read_buffer.resize(buffer_max_size);
    do {
        read_in_buffer(input_stream);
        for (auto j : read_buffer) {
            auto j_uns = static_cast<unsigned char>(j);
            std::vector<bool> buff;
            for (size_t i = 0; i < 8; i++) {
                buff.push_back(j_uns % 2 != 0);
                j_uns /= 2;
            }
            std::reverse(buff.begin(), buff.end());
            for (size_t i = 0; i < 8; i++) {
                now = graph[now][buff[i]];
                if (now < frequency_of_chars.size()) {
                    output_stream.write(reinterpret_cast<char*>(&now), 1);
                    now = 510;
                    break;
                }
            }
        }
    } while (read_buffer.size() == buffer_max_size);
    input_stream.close();
    output_stream.close();
}

//--decode test2.bin test2_out.txt
//--code test1.txt test1_out.bin