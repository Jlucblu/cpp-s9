#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;


template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    string word;
    int collisions = 0;
    unordered_map<size_t, unordered_set<string>> cm;

    while (text >> word) {
        size_t h = hasher(word);
        cm[h].emplace(std::move(word));
        if (cm[h].size() > 1) {
            ++collisions;
        }
    }

    /*for (auto v : cm) {
        if (v.second.size() > 1) collisions += v.second.size() - 1;
    }*/

    std::for_each(cm.begin(), cm.end(), [&](const auto& p) {
        const auto& [_, c] = p;
        collisions += c.size() - 1;
        });

    return collisions;
}

// ��� ������ �����. ��� ����� ������������ ��� ������������.
// ���������, � ��� ��� ����������
struct DummyHash {
    size_t operator()(const string&) const {
        return 42;
    }
};

void TestFindCollisions() {
    DummyHash dummy_hash;
    hash<string> good_hash;

    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(dummy_hash, stream) << endl;
    }
    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(good_hash, stream) << endl;
    }
}