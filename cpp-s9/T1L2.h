#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>

#include "log_duration.h"

using namespace std;


vector<pair<string, int>> GetSortedWordCounts(vector<string> words) {
    unordered_map<string, int> counts_map;

    {
        LOG_DURATION("Filling"s);
        for (auto& word : words) {
            //if (word.size() == 5) {
            ++counts_map[move(word)];
            //}
        }
    }

    {
        LOG_DURATION("Moving & sorting"s);
        vector<pair<string, int>> counts_vector(move_iterator(counts_map.begin()), move_iterator(counts_map.end()));
        sort(counts_vector.begin(), counts_vector.end(), [](const auto& l, const auto& r) {
            return l.second > r.second;
            });

        return counts_vector;
    }
}

void TestCountWords() {
    setlocale(LC_ALL, "ru");
    vector<string> words;
    string word;
    string path = "canterbury.txt";

    ifstream fin;
    fin.open(path);

    if (!fin.is_open()) {
        cout << "Error"s << endl;
    }
    else {
        cout << "OK!"s << endl;
        while (!fin.eof()) {
            word = "";
            fin >> word;
            words.push_back(word);
        }
    }
    fin.close();


    auto counts_vector = GetSortedWordCounts(move(words));

    cout << "Слово - Количество упоминаний в тексте"s << endl;
    // выводим первые 10 слов
    for (auto [iter, i] = tuple(counts_vector.begin(), 0); i < 10 && iter != counts_vector.end(); ++i, ++iter) {
        cout << iter->first << " - "s << iter->second << endl;
    }
}