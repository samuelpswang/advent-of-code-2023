// File Name:       day_23/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 23 Part 2 (https://adventofcode.com/2023/day/23)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <set>
#include <map>
#include <cassert>
using namespace std;

struct point {
    int r;
    int c;
    point(): r(0), c(1) {}
    point(int r, int c): r(r), c(c) {}
    bool is_valid(int rlim, int clim) { return 0 <= r && r <= rlim && 0 <= c && c <= clim; }
    bool operator==(const point& rhs) const { return r == rhs.r && c == rhs.c; }
    bool operator<(const point& rhs) const {
        if (r < rhs.r) return true;
        else if (r > rhs.r) return false;
        else return (c < rhs.c);
    }
};

int dfs(const point& start, const point& end, const set<point>& nodeset, const map<point,map<point,int>>& graph, set<point>& taken) {
    if (start == end) return 0;
    int max_length = INT_MIN;
    taken.insert(start);
    for (const pair<point,int>& p: graph.at(start)) {
        point np = p.first;
        if (taken.find(np) == taken.end()) {
            max_length = max(max_length, dfs(np, end, nodeset, graph, taken) + graph.at(start).at(np));
        }
    }
    taken.erase(start);
    return max_length;
}

int main() {
    // read input
    ifstream input("input.txt");
    if (!input) exit(1);
    string line;
    vector<string> forest;
    while (getline(input, line)) {
        forest.push_back(line);
    }
    input.close();

    // find nodes
    const int rlim = forest.size()-1;
    const int clim = forest[0].size()-1;
    const vector<int> dr = { -1, 1, 0, 0 };
    const vector<int> dc = { 0, 0, -1, 1 };
    set<point> nodeset;
    nodeset.emplace(0, 1);
    nodeset.emplace(rlim, clim-1);
    for (int r = 0; r < forest.size(); ++r) {
        for (int c = 0; c < forest[r].size(); ++c) {
            if (forest[r][c] == '#') continue;
            int ncount = 0;
            for (int i = 0; i < 4; ++i) {
                point p(r+dr[i], c+dc[i]);
                if (p.is_valid(rlim, clim) && forest[p.r][p.c] != '#') ncount++;
            }
            if (ncount > 2) {
                nodeset.emplace(r,c);
            }
        }
    }
    
    // find longest path in between decision points
    map<point,map<point,int>> graph;
    for (const point& p: nodeset) {
        stack<pair<point,int>> s; // using stack for depth first search
        set<point> h;
        s.push({p, 0});
        h.insert(p);
        while (!s.empty()) {
            pair<point,int> top = s.top();
            s.pop();
            // if "is new" = not original 0 + is a way to a node
            if (top.second != 0 && nodeset.find(top.first) != nodeset.end()) {
                graph[p][top.first] = top.second;
            } else {
                for (int i = 0; i < 4; ++i) {
                    point np(top.first.r+dr[i], top.first.c+dc[i]);
                    // if "is path" = valid coordinates + does not pass through other points + is not wall
                    if (np.is_valid(rlim, clim) && h.find(np) == h.end() && forest[np.r][np.c] != '#') {
                        s.push({np, top.second+1});
                        h.insert(np);
                    }
                }
            }
        }
    }

    // recursively find the longest path
    set<point> taken;
    cout << dfs({0,1}, {rlim, clim-1}, nodeset, graph, taken) << endl;
}

// NOTE:
// brute force bfs takes longer than an hour - new method tried - which is to
// reconstruct the maze as a graph, and recursively find the maximum length 
// path; inspiration taken from @hyper-neutrino
