// File Name:       day_21/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 21 Part 2 (https://adventofcode.com/2023/day/21)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <cassert>
using namespace std;

int find(const vector<vector<bool>>& grid, const pair<int,int>& start, int steps) {
    int r = grid.size()-1, c = grid[0].size()-1;
    const vector<int> dr = { 1, -1, 0, 0 };
    const vector<int> dc = { 0, 0, 1, -1 };
    queue<pair<int,int>> q, nq;
    q.push(start);
    for (int sc = 0; sc < steps; ++sc) {
        set<pair<int,int>> s;
        while (!q.empty()) {
            pair<int,int> curr = q.front();
            q.pop();
            for (int i = 0; i < 4; ++i) {
                pair<int,int> next = {curr.first+dr[i], curr.second+dc[i]};
                if (0 <= next.first && next.first <= r &&
                    0 <= next.second && next.second <= c &&
                    grid[next.first][next.second] && s.count(next) == 0) {
                    nq.push(next);
                    s.insert(next);
                }
            }
        }
        q = nq;
        nq = {};
    }
    return q.size();
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    int r = 0;
    int c = 0;
    string line;
    vector<vector<bool>> grid;
    pair<int,int> start;
    while (getline(input, line)) {
        grid.push_back({});
        c = 0;
        for (char ch: line) {
            if (ch != '#') grid[r].push_back(true);
            else grid[r].push_back(false);
            if (ch == 'S') start = {r, c};
            ++c;
        }
        ++r;
    }
    input.close();

    const long long STEPS = 26501365;
    int side = grid.size();

    // validate assumptions with asserts
    // assert that we have a square grid
    assert(grid.size() == grid[0].size());
    // assert that we are starting from center
    assert(start.first == grid.size()/2 && start.second == grid[0].size()/2);
    // assert that we are has highway
    bool empty_col = true, empty_row = true;
    for (int c = 0; c < side; ++c) {
        if (!grid[start.first][c]) empty_col = false;
    }
    for (int r = 0; r < side; ++r) {
        if (!grid[r][start.second]) empty_row = false;
    }
    assert(empty_col && empty_row);
    // assert that total steps to take is (multiple of sides + half a side)
    assert(STEPS % side == side/2);

    // calculate grid area
    // calculate odd and even squares in between
    int os = find(grid, start, side*2+1);
    int es = find(grid, start, side*2);
    // calculate corners
    int tc = find(grid, {side-1, side/2}, side-1);
    int bc = find(grid, {0, side/2}, side-1);
    int lc = find(grid, {side/2, side-1}, side-1);
    int rc = find(grid, {side/2, 0}, side-1);
    // calculate small triangle on edges
    int trt = find(grid, {side-1, 0}, side/2-1);
    int tlt = find(grid, {side-1, side-1}, side/2-1);
    int brt = find(grid, {0, 0}, side/2-1);
    int blt = find(grid, {0, side-1}, side/2-1);
    // calculaute big pentagon on edges
    int trp = find(grid, {side-1, 0}, side*3/2-1);
    int tlp = find(grid, {side-1, side-1}, side*3/2-1);
    int brp = find(grid, {0, 0}, side/2-1);
    int blp = find(grid, {0, side-1}, side*3/2-1);

    // count number of areas
    // grid number
    int n = (STEPS - side/2) / side;
    // calculate number of odd and even squares
    long long os_count = pow(n-1, 2);
    long long es_count = pow(n, 2);
    // calculate number of corners
    long long c_count = 1;
    // calculate number of triangles
    long long t_count = n;
    // calculate number of pentagons
    long long p_count = n-1;


    long long total = 0;
    total += es_count * es; 
    total += os_count * os;
    total += c_count * (tc + bc + lc + rc);
    total += t_count * (trt + tlt + brt + blt);
    total += p_count * (trp + tlp + brp + blp);
    cout << total << endl;
}

// NOTE: 
// Solution taken from @hyper-neutrino, which is to segment the possible
// traversed areas into repeating grids and add them together. One should draw
// out the diamond shape and count the number of various patterns.
