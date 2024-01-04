// File Name:       day_01/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 1 Part 2 (https://adventofcode.com/2023/day/1)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const vector<string> NUMSTR = {"zero", "one", "two", "three", "four", "five", \
    "six", "seven", "eight", "nine"};

vector<int> find_num(string& line) {
    int start_ind = -1, start_num = 0, end_ind = -1, end_num = 0;
    for (size_t i = 0; i < line.size(); i++) {
        if ('0' <= line[i] && line[i] <= '9') {
            if (start_ind == -1) {
                start_ind = i;
                start_num = int(line[i]) - int('0');
            }
            end_ind = i;
            end_num = int(line[i]) - int('0');
        }
    }
    return {start_ind, start_num, end_ind, end_num};
}

vector<int> find_txt(string& line) {
    int start_ind = -1, start_num = 0, end_ind = -1, end_num = 0;
    for (int num = 1; num <= 9; num++) {
        size_t pos = line.find(NUMSTR[num]);
        if (pos == string::npos) continue;
        size_t curr_start = pos, curr_end = pos;
        while (pos != string::npos) {
            curr_end = pos;
            pos = line.find(NUMSTR[num], pos+1);
        }
        if (start_ind == -1 || curr_start < start_ind) {
            start_ind = curr_start;
            start_num = num;
        }
        if (end_ind == -1 || end_ind < curr_end) {
            end_ind = curr_end;
            end_num = num;
        }
    }
    return {start_ind, start_num, end_ind, end_num};
}

int two_digit_number(string& line) {
    vector<int> single = find_num(line);
    vector<int> multiple = find_txt(line);
    int start, end;
    if (multiple[0] == -1 || single[0] < multiple[0]) start = single[1];
    else start = multiple[1];
    if (multiple[2] == -1 || single[2] > multiple[2]) end = single[3];
    else end = multiple[3];
    return 10 * start + end;
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