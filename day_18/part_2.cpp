// File Name:       day_18/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 18 Part 2 (https://adventofcode.com/2023/day/18)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

long long area(const vector<vector<long long>>& c) {
    long long sum = 0;
    for (int i = 0; i < c.size()-1; ++i) {
        sum += c[i][0]*c[i+1][1] - c[i+1][0]*c[i][1];
    }
    sum += c[c.size()-1][0]*c[0][1] - c[0][0]*c[c.size()-1][1];
    sum /= 2;
    return abs(sum);
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    long long x = 0;
    long long y = 0;
    long long p = 0;
    vector<vector<long long>> c;
    string line;
    while (getline(input, line)) {
        size_t s = line.find('#')+1;
        size_t e = line.find(')')-1;
        switch (line[e]) {
            case '0':
                x += stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                p += stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                break;
            case '1':
                y += stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                p += stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                break;
            case '2':
                x -= stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                p += stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                break;
            case '3':
                y -= stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                p += stoll(string(line.begin()+s, line.begin()+e), nullptr, 16);
                break;
        }
        c.push_back({x, y});
    }
    input.close();

    // recall pick's theorm:
    // A = (# of points inside) + (# of points on bound)/2 - 1

    // we calculate the area with the shoelace formula and find the # of points on the inside 
    // -> i  = A - b/2 + 1
    // we note that the points on the boundary are also dug out
    // -> total = i + b = A + b/2 + 1
    cout << area(c)+p/2+1 << endl;
}
