// File Name:       day_10/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 10 Part 1 (https://adventofcode.com/2023/day/10)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

const int LIM = 139;

struct coor {
    int x;
    int y;
    coor(): x(-1), y(-1) {}
    coor(int x, int y): x(x), y(y) {}
    bool is_valid() { return (x != -1 && y != -1); }

    bool operator==(const coor& rhs) const {return (x == rhs.x && y == rhs.y); }
};

struct dire {
    bool n;
    bool s;
    bool e;
    bool w;
    dire(): n(0), s(0), e(0), w(0) {}
    dire(bool n, bool s, bool e, bool w): n(n), s(s), e(e), w(w) {}
};

struct pipe {
    coor loc;
    dire dir;

    pipe(char c, int x, int y) {
        loc = coor(x, y);
        switch (c) {
            case '-':
                dir = dire(0, 0, 1, 1);
                break;
            case '|':
                dir = dire(1, 1, 0, 0);
                break;
            case 'L':
                dir = dire(1, 0, 1, 0);
                break;
            case 'J':
                dir = dire(1, 0, 0, 1);
                break;
            case '7':
                dir = dire(0, 1, 0, 1);
                break;
            case 'F':
                dir = dire(0, 1, 1, 0);
                break;
            case 'S':
                dir = dire(1, 1, 1, 1);
                break;
            default:
                dir = dire();
                break;
        }
    }

    coor north() { return (loc.y > 0 ? coor(loc.x, loc.y-1) : coor()); }
    coor south() { return (loc.y < LIM ? coor(loc.x, loc.y+1) : coor()); }
    coor east() { return (loc.x < LIM ? coor(loc.x+1, loc.y) : coor()); }
    coor west() { return (loc.x > 0 ? coor(loc.x-1, loc.y) : coor()); }

    bool is_north(const pipe& rhs) { return (loc.x == rhs.loc.x) && (loc.y-1 == rhs.loc.y); }
    bool is_south(const pipe& rhs) { return (loc.x == rhs.loc.x) && (loc.y+1 == rhs.loc.y); }
    bool is_east(const pipe& rhs) { return (loc.x+1 == rhs.loc.x) && (loc.y == rhs.loc.y); }
    bool is_west(const pipe& rhs) { return (loc.x-1 == rhs.loc.x) && (loc.y == rhs.loc.y); }

    bool is_connected(const pipe& rhs) {
        bool ans;
        if (is_north(rhs)) ans = (dir.n && rhs.dir.s);
        else if (is_south(rhs)) ans = (dir.s && rhs.dir.n);
        else if (is_east(rhs)) ans = (dir.e && rhs.dir.w);
        else if (is_west(rhs)) ans = (dir.w && rhs.dir.e);
        else ans = false;
        return ans;
    }
};

struct map {
    vector<vector<pipe>> data;
    coor start;

    map(const string& filename) {
        ifstream input(filename);
        if (!input) exit(1);
        
        int row = 0, col = 0;
        string line;
        while (getline(input, line)) {
            data.push_back({});
            for (char c: line) {
                if (c == 'S') start = coor(col, row);
                data[row].emplace_back(c, col, row);
                ++col;
            }
            col = 0;
            ++row;
        }
    }

    pipe at(coor loc) {
        return data[loc.y][loc.x];
    }
};

struct agent {
    coor loc;
    dire from;
    int steps;
    agent(coor start): loc(start), from(), steps(0) {}
};

int main() {
    map survey("input.txt");
    queue<agent> curr;
    curr.push(agent(survey.start));

    // bfs
    while (!curr.empty()) {
        queue<agent> next;
        while (!curr.empty()) {
            agent x = curr.front();
            curr.pop();
            if (!x.from.n && survey.at(x.loc).north().is_valid() && survey.at(x.loc).is_connected(survey.at(survey.at(x.loc).north()))) {
                agent y(survey.at(x.loc).north());
                if (y.loc.is_valid()) {
                    y.from = dire(0, 1, 0, 0);
                    y.steps = x.steps+1;
                    next.push(y);
                }
            }
            if (!x.from.s && survey.at(x.loc).south().is_valid() && survey.at(x.loc).is_connected(survey.at(survey.at(x.loc).south()))) {
                agent y(survey.at(x.loc).south());
                if (y.loc.is_valid()) {
                    y.from = dire(1, 0, 0, 0);
                    y.steps = x.steps+1;
                    next.push(y);
                }
            }
            if (!x.from.e && survey.at(x.loc).east().is_valid() && survey.at(x.loc).is_connected(survey.at(survey.at(x.loc).east()))) {
                agent y(survey.at(x.loc).east());
                if (y.loc.is_valid()) {
                    y.from = dire(0, 0, 0, 1);
                    y.steps = x.steps+1;
                    next.push(y);
                }
            }
            if (!x.from.w && survey.at(x.loc).west().is_valid() && survey.at(x.loc).is_connected(survey.at(survey.at(x.loc).west()))) {
                agent y(survey.at(x.loc).west());
                if (y.loc.is_valid()) {
                    y.from = dire(0, 0, 1, 0);
                    y.steps = x.steps+1;
                    next.push(y);
                }
            }
        }
        curr = next;
        if (curr.front().loc == curr.back().loc && curr.front().steps == curr.back().steps) break;
    }

    cout << curr.front().steps << endl; 
}

// there has to be much better ways to do this, things to fix:
// 1) bad oop design
// 2) hard coded values
