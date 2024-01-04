// File Name:       day_17/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 17 Part 2 (https://adventofcode.com/2023/day/17)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
using namespace std;

struct gtr {
    bool operator()(const vector<int>& lhs, const vector<int>& rhs) {
        return lhs.at(0) > rhs.at(0);
    }
};

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    vector<vector<int>> data;
    string line;
    while (getline(input, line)) {
        vector<int> row;
        for (char c: line) {
            row.push_back(atoi(&c));
        }
        data.push_back(row);
    }

    int r = data.size()-1;
    int c = data[0].size()-1;
    vector<int> dr = {-1, 0, 1, 0};
    vector<int> dc = {0, 1, 0, -1};
    priority_queue<vector<int>,vector<vector<int>>,gtr> q;
    q.push({0, 0, 0, -1, -1});
    map<vector<int>,int> costs;

    while (!q.empty()) {
        vector<int> curr = q.top();
        int cost = curr[0];
        vector<int> info(curr.begin()+1, curr.end());
        q.pop();

        if (costs.find(info) != costs.end()) continue;
        else costs[info] = cost;

        for (int i = 0; i < 4; ++i) {
            int new_row = info[0] + dr[i];
            int new_col = info[1] + dc[i];
            int new_dir = i;
            int new_count;
            if (new_dir == info[2]) new_count = info[3] + 1;
            else new_count = 1;
            int new_cost;
            if (0 <= new_row && new_row <= r && 0 <= new_col && new_col <= c) new_cost = cost + data[new_row][new_col];
            else continue;
            
            bool is_valid;
            is_valid = 0 <= new_row && new_row <= r && 0 <= new_col && new_col <= c;
            is_valid = is_valid && new_count <= 10 && (info[2] == new_dir || info[3] >= 4 || info[3] == -1);
            bool is_forward = (new_dir+2) % 4 != info[2];
            bool is_unique = costs.find({new_row, new_col, new_dir, new_count}) == costs.end();
            if (is_valid && is_forward && is_unique) q.push({new_cost, new_row, new_col, new_dir, new_count});
        }
    }
    
    int min_cost = INT_MAX;
    for (const pair<vector<int>,int>& p: costs) {
        if (p.first[0] == r && p.first[1] == c) {
            min_cost = min(min_cost, p.second);
        }
    }

    cout << min_cost << endl;
}

// inspiration taken from @jonathanpaulson and @hyper-neutrino
// the solution is essentially a djikstra's algorithm, which I had not 
// previously thought of and was trying various ways of bfs
// solution was translated from python
