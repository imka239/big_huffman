#include "huffman.hpp"
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace huffman;
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

void file_handler::build_tree() {
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
}

void file_handler::write_frequencies(std::ofstream &to) {
    for (size_t i = 0; i < frequency_of_chars.size(); i++) {
        to.write(reinterpret_cast<char*>(&frequency_of_chars[i]), 8);
    }
}

void file_handler::code(std::vector<char> &buffer, std::vector<char> &ans) {
    for (auto i : buffer) {
        auto j_uns = static_cast<unsigned char>(i);
        for (size_t j = 0; j < ans_in_char[j_uns].size(); j++) {
            size_t sz = 8;
            if (j == ans_in_char[j_uns].size() - 1) {
                sz = size[j_uns];
            }
            if (pos + sz >= 8) {
                next_char += (ans_in_char[j_uns][j] >> pos);
                ans.push_back(next_char);
                next_char = (ans_in_char[j_uns][j] << (8 - pos));
                pos = (pos + sz) - 8;
            } else {
                next_char += (ans_in_char[j_uns][j] >> pos);
                pos = (pos + sz);
            }
        }
    }
}

void file_handler::decode(std::vector<char> & buffer, std::vector<size_t> & ans) {
    for (auto j : buffer) {
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
            if (now == 84) {
                now = 84;
            }
            if (now < frequency_of_chars.size()) {
                if (frequency_of_chars[now] == 0) {
                    return;
                }
                frequency_of_chars[now]--;
                ans.push_back(now);
                now = 510;
            }
        }
    }
}

void file_handler::read_frequencies(uint64_t a, size_t i) {
    frequency_of_chars[i] = a;
}

//--decode test2.bin test2_out.txt
//--code test1.txt test1_out.bin