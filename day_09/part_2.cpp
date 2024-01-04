// File Name:       day_09/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 9 Part 2 (https://adventofcode.com/2023/day/9)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void parse_data(const string& filename, vector<vector<long>>& data) {
    ifstream input(filename);
    if (!input) exit(1);

    string line;
    while (getline(input, line)) {
        stringstream ss;
        long reading;
        vector<long> series;
        ss << line;
        while (ss >> reading) {
            series.push_back(reading);
        }
        data.push_back(series);
    }

    input.close();
}

vector<long> diff(const vector<long>& f) {
    vector<long> df;
    for (size_t i = 1; i < f.size(); i++) {
        df.push_back(f.at(i) - f.at(i-1));
    }
    return df;
}

bool is_zero(const vector<long>& f) {
    for (long n: f) {
        if (n) return false;
    }
    return true;
}

long predict_previous(const vector<long>& f) {
    vector<vector<long>> dfv = {f};
    vector<long> cdf = f, ndf = diff(f);
    while (!is_zero(ndf)) {
        dfv.push_back(ndf);
        cdf = ndf; 
        ndf = diff(cdf);
    }
    
    long next = 0;
    for (int i = dfv.size()-1; i >= 0; i--) {
        next = dfv[i].front() - next;
    }
    return next;
}

int main() {
    vector<vector<long>> data;
    parse_data("input.txt", data);

    long long sum = 0;
    for (const vector<long>& series: data) {
        sum += predict_previous(series);
    }
    
    cout << sum << endl;
}