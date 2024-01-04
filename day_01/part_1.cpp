// File Name:       day_01/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 1 Part 1 (https://adventofcode.com/2023/day/1)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int two_digit_number(string& line) {
    char start = ' ', end = ' ';
    for (char c: line) {
        if ('0' <= c && c <= '9') {
            if (start == ' ') start = c;
            end = c;
        }
    }
    string num;
    num += start;
    num += end;
    return stoi(num);
}

int main() {
    // open file
    ifstream ifs;
    ifs.open("input.txt");
    if (!ifs) exit(1);
    
    // parse
    string line;
    long long int sum = 0;
    while (ifs >> line) {
        sum += two_digit_number(line);
    }
    ifs.close();
    
    // output answer
    cout << sum << endl;
}
