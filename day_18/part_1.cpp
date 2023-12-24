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
    string s;
    vector<pair<char,int>> plan;
    while (getline(input, s, ' ')) {
        string n, c;
        getline(input, n, ' ');
        getline(input, c);
        pair<char,int> dig = {s[0], stoi(n)};
        plan.push_back(dig);
    }
    input.close();

    int r = 0, c = 0;
    int max_r = 0, max_c = 0, min_r = 0, min_c = 0;
    set<pair<int,int>> bound;
    bound.insert({0, 0});
    for (const pair<char,int>& dig: plan) {
        if (dig.first == 'U') {
            for (int i = 0; i < dig.second; ++i) {
                --r;
                bound.insert({r, c});
                max_r = max(max_r, r);
                min_r = min(min_r, r);
                max_c = max(max_c, c);
                min_c = min(min_c, c);
            }
        } else if (dig.first == 'D') {
            for (int i = 0; i < dig.second; ++i) {
                ++r;
                bound.insert({r, c});
                max_r = max(max_r, r);
                min_r = min(min_r, r);
                max_c = max(max_c, c);
                min_c = min(min_c, c);
            }
        } else if (dig.first == 'L') {
            for (int i = 0; i < dig.second; ++i) {
                --c;
                bound.insert({r, c});
                max_r = max(max_r, r);
                min_r = min(min_r, r);
                max_c = max(max_c, c);
                min_c = min(min_c, c);
            }
        } else if (dig.first == 'R') {
            for (int i = 0; i < dig.second; ++i) {
                ++c;
                bound.insert({r, c});
                max_r = max(max_r, r);
                min_r = min(min_r, r);
                max_c = max(max_c, c);
                min_c = min(min_c, c);
            }
        }
    }

    // prep for bfs
    const vector<int> dr = {-1, 1, 0, 0};
    const vector<int> dc = {0, 0, -1, 1};
    queue<pair<int,int>> q;
    for (int r = min_r; r <= max_r; ++r) {
        q.push({r, min_c});
        q.push({r, max_c});
    }
    for (int c = min_c; c <= max_c; ++c) {
        q.push({min_r, c});
        q.push({max_r, c});
    }
    set<pair<int,int>> area;

    // bfs
    while (!q.empty()) {
        pair<int,int> c = q.front();
        q.pop();
        if (area.find(c) != area.end()) {
            continue;
        } else if (bound.find(c) != bound.end()) {
            // area.insert(c);
            continue;
        } else {
            area.insert(c);
            int cr = c.first, cc = c.second;
            for (int i = 0; i < 4; i++) {
                int nr = cr + dr[i], nc = cc + dc[i];
                if (min_r <= nr && nr <= max_r && min_c <= nc && nc <= max_c)
                    q.push({nr, nc});
            }
        }
    }
    
    int inside = (max_r-min_r+1) * (max_c-min_c+1) - area.size();
    cout << inside << endl;
}
