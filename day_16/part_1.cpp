// File Name:       day_16/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 16 Part 1 (https://adventofcode.com/2023/day/16)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set> 
using namespace std;

enum from { from_left = 0, from_up = 1, from_right = 2, from_down = 3 };

int main() {
    // read data
    ifstream input("input.txt");
    if (!input) exit(1);
    string line;
    vector<string> data;
    while (getline(input, line)) { data.push_back(line); }
    input.close();

    // bfs
    const int RLIM = data.size(), CLIM = data[0].size();
    queue<vector<int>> q;
    set<vector<int>> v;
    q.push({0, 0, from_left}); // left: 0, top: 1, right: 2, bottom, 3
    while (!q.empty()) {
        vector<int> curr = q.front();
        q.pop();

        if (!v.count(curr)) v.insert(curr);
        else continue;

        if (curr.at(2) == from_left) {
            switch (data[curr.at(0)][curr.at(1)]) {
                case '\\':
                    if (0 <= curr.at(0)+1 && curr.at(0)+1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)+1, curr.at(1), from_up});
                    }
                    break;
                case '/':
                    if (0 <= curr.at(0)-1 && curr.at(0)-1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)-1, curr.at(1), from_down});
                    }
                    break;
                case '|':
                    if (0 <= curr.at(0)-1 && curr.at(0)-1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)-1, curr.at(1), from_down});
                    }
                    if (0 <= curr.at(0)+1 && curr.at(0)+1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)+1, curr.at(1), from_up});
                    }
                    break;
                case '-':
                default:
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)+1 && curr.at(1)+1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)+1, from_left});
                    }
                    break;
            }
        } else if (curr.at(2) == from_up) {
            switch (data[curr.at(0)][curr.at(1)]) {
                case '\\':
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)+1 && curr.at(1)+1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)+1, from_left});
                    }
                    break;
                case '/':
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)-1 && curr.at(1)-1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)-1, from_right});
                    }
                    break;
                case '-':
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)+1 && curr.at(1)+1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)+1, from_left});
                    }
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)-1 && curr.at(1)-1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)-1, from_right});
                    }
                    break;
                case '|':
                default:
                    if (0 <= curr.at(0)+1 && curr.at(0)+1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)+1, curr.at(1), from_up});
                    }
                    break;
            }
        } else if (curr.at(2) == from_right) {
            switch (data[curr.at(0)][curr.at(1)]) {
                case '\\':
                    if (0 <= curr.at(0)-1 && curr.at(0)-1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)-1, curr.at(1), from_down});
                    }
                    break;
                case '/':
                    if (0 <= curr.at(0)+1 && curr.at(0)+1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)+1, curr.at(1), from_up});
                    }
                    break;
                case '|':
                    if (0 <= curr.at(0)-1 && curr.at(0)-1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)-1, curr.at(1), from_down});
                    }
                    if (0 <= curr.at(0)+1 && curr.at(0)+1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)+1, curr.at(1), from_up});
                    }
                    break;
                case '-':
                default:
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)-1 && curr.at(1)-1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)-1, from_right});
                    }
                    break;
            }
        } else if (curr.at(2) == from_down) {
            switch (data[curr.at(0)][curr.at(1)]) {
                case '\\':
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)-1 && curr.at(1)-1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)-1, from_right});
                    }
                    break;
                case '/':
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)+1 && curr.at(1)+1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)+1, from_left});
                    }
                    break;
                case '-':
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)+1 && curr.at(1)+1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)+1, from_left});
                    }
                    if (0 <= curr.at(0) && curr.at(0) < RLIM && \
                        0 <= curr.at(1)-1 && curr.at(1)-1 < CLIM) {
                        q.push({curr.at(0), curr.at(1)-1, from_right});
                    }
                    break;
                case '|':
                default:
                    if (0 <= curr.at(0)-1 && curr.at(0)-1 < RLIM && \
                        0 <= curr.at(1) && curr.at(1) < CLIM) {
                        q.push({curr.at(0)-1, curr.at(1), from_down});
                    }
                    break;
            }
        }
    }

    set<pair<int,int>> vs;
    for (const vector<int>& vec: v) {
        if (!vs.count({vec.at(0), vec.at(1)})) 
            vs.insert({vec.at(0), vec.at(1)});
    }
    cout << vs.size() << endl;
}