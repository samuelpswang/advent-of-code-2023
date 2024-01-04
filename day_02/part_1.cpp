// File Name:       day_02/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 2 Part 1 (https://adventofcode.com/2023/day/2)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

const unordered_map<string,int> LIMIT = {{"red", 12}, {"green", 13}, \
    {"blue", 14}};

vector<string> split(const string& s, const string& sep) {
    vector<string> result;
    size_t prev = 0, next = s.find(sep);
    while (next != string::npos) {
        if (!prev) result.emplace_back(s.begin()+prev, s.begin()+next);
        else result.emplace_back(s.begin()+prev+sep.size(), s.begin()+next);
        prev = next;
        next = s.find(sep, next+1);
    }
    if (!prev) result.emplace_back(s.begin()+prev, s.end());
    else result.emplace_back(s.begin()+prev+sep.size(), s.end());
    return result;
}

bool validate_draw(string& draw) {
    vector<string> colors = split(draw, ", ");
    for (string& c: colors) {
        vector<string> v = split(c, " ");
        if (stoi(v[0]) > LIMIT.at(v[1])) return false;
    }
    return true;
}

bool validate_game(string& game) {
    vector<string> draws = split(game, "; ");
    for (string& d: draws) {
        if (!validate_draw(d)) return false;
    }
    return true;
}

int parse_game(string& line) {
    // find game number
    size_t g_start = 5, g_end = line.find(':');
    int g_num = stoi(string(line.begin()+g_start, line.begin()+g_end));

    // check validity
    line = string(line.begin()+g_end+2, line.end());
    bool valid = validate_game(line);

    // report
    return (valid ? g_num : 0);
}

int main() {
    // open file
    ifstream ifs;
    ifs.open("input.txt");
    if (!ifs) exit(1);
    
    // parse
    string line;
    long long int sum = 0;
    while (getline(ifs, line)) {
        sum += parse_game(line);
    }
    ifs.close();
    
    // output answer
    cout << sum << endl;
}
