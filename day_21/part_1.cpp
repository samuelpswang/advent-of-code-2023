// File Name:       day_21/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 21 Part 1 (https://adventofcode.com/2023/day/21)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using namespace std;

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    int r = 0;
    int c = 0;
    string line;
    vector<vector<bool>> garden;
    pair<int,int> start;
    while (getline(input, line)) {
        garden.push_back({});
        c = 0;
        for (char ch: line) {
            if (ch != '#') garden[r].push_back(true);
            else garden[r].push_back(false);
            if (ch == 'S') start = {r, c};
            ++c;
        }
        ++r;
    }
    input.close();

    r = garden.size()-1;
    c = garden[0].size()-1;
    const vector<int> dr = { 1, -1, 0, 0 };
    const vector<int> dc = { 0, 0, 1, -1 };

    queue<pair<int,int>> q, nq;
    q.push(start);
    for (int steps = 0; steps < 64; ++steps) {
        set<pair<int,int>> s;
        while (!q.empty()) {
            pair<int,int> curr = q.front();
            q.pop();
            for (int i = 0; i < 4; ++i) {
                pair<int,int> next = {curr.first+dr[i], curr.second+dc[i]};
                if (0 <= next.first && next.first <= r &&
                    0 <= next.second && next.second <= c &&
                    garden[next.first][next.second] && s.count(next) == 0) {
                    nq.push(next);
                    s.insert(next);
                }
            }
        }
        q = nq;
        nq = {};
    }
    
    cout << q.size() << endl;
}