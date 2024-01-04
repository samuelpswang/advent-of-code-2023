// File Name:       day_15/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 15 Part 1 (https://adventofcode.com/2023/day/15)
// Last Updated:    04 Jan 2023

#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int my_hash(const string& s) {
    int curr = 0;
    for (char c: s) {
        curr += c;
        curr *= 17;
        curr %= 256;
    }
    return curr;
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    string line;
    vector<string> seq;
    while (getline(input, line, ',')) {
        seq.push_back(line);
    }

    int sum = 0;
    for (const string& s: seq) {
        sum += my_hash(s);
    }

    cout << sum << endl;
}