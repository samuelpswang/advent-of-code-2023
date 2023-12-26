// TODO:

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using namespace std;

const int LIM = 12;

enum heading { north, south, east, west };

struct agent {
    int r;
    int c;
    int cost;
    vector<heading> h;

    agent(heading next): r(0), c(0), cost(0), h() { h.push_back(next); }

    agent(const agent& pa, heading next): r(pa.r), c(pa.c), cost(pa.cost), h(pa.h) {
        switch (next) {
            case north:
                --r;
                break;
            case south:
                ++r;
                break;
            case east:
                ++c;
                break;
            case west:
                --c;
                break;
        }
        h.push_back(next);
    }

    bool is_turn() { return (h.size() >= 3) && (h.back() == h.at(h.size()-2) && h.back() == h.at(h.size()-3)); }

    bool is_valid() { return (0 <= r && r <= LIM && 0 <= c && c <= LIM); }

    bool is_end() { return r == LIM && c == LIM;}
};

void print_heading(const vector<heading>& h) {
    for (heading ch: h) {
        if (ch == north) cout << "^";
        if (ch == south) cout << "v";
        if (ch == east) cout << ">";
        if (ch == west) cout << "<";
    }
    cout << endl;
}

int main() {
    ifstream input("test.txt");
    if (!input) exit(1);
    
    string line;
    vector<vector<int>> data;
    while (getline(input, line)) {
        vector<int> row_data;
        for (char c: line) {
            row_data.push_back(atoi(&c));
        }
        data.push_back(row_data);
    }

    queue<agent> q;
    set<pair<int,int>> b;
    agent a1(east), a2(south);
    a1.cost += data[a1.r][a1.c];
    a2.cost += data[a2.r][a2.c];
    q.push(a1);
    q.push(a2);
    b.insert({0, 0});

    int min_path = INT_MAX;
    while (!q.empty()) {
        bool print = false;
        vector<heading> tar = {east, east, east, south};
        if (q.front().h == tar) cout << "======" << endl;
        agent ca = q.front();
        q.pop();
        agent na(ca, north), sa(ca, south), ea(ca, east), wa(ca, east);
        if (na.is_valid()) na.cost += data[na.r][na.c];
        if (sa.is_valid()) sa.cost += data[sa.r][sa.c];
        if (ea.is_valid()) ea.cost += data[ea.r][ea.c];
        if (wa.is_valid()) wa.cost += data[wa.r][wa.c];

        // turn
        // cout << "turn" << endl;
        if (ca.h.back() == north || ca.h.back() == south) {
            if (ea.is_valid()) {
                cout << "te" << endl;
                print_heading(ea.h);
                if (ea.is_end()) {
                    min_path = min(min_path, ea.cost);
                } else if (b.count({ea.r, ea.c}) == 0 || true) {
                    q.push(ea);
                    b.insert({ea.r, ea.c});
                }
            }
            if (wa.is_valid()) {
                // cout << "tw" << endl;
                print_heading(wa.h);
                if (wa.is_end()) {
                    min_path = min(min_path, wa.cost);
                } else if (b.count({wa.r, wa.c}) == 0 || true) {
                    q.push(wa);
                    b.insert({wa.r, wa.c});
                }
            }
        } else if (ca.h.back() == east || ca.h.back() == west) {
            if (na.is_valid()) {
                // cout << "tn" << endl;
                print_heading(na.h);
                if (na.is_end()) {
                    min_path = min(min_path, na.cost);
                } else if (b.count({na.r, na.c}) == 0 || true) {
                    q.push(na);
                    b.insert({na.r, na.c});
                }
            }
            if (sa.is_valid()) {
                print_heading(sa.h);
                // cout << "ts" << endl;
                if (sa.is_end()) {
                    min_path = min(min_path, sa.cost);
                } else if (b.count({sa.r, sa.c}) == 0 || true) {
                    q.push(sa);
                    b.insert({sa.r, sa.c});
                }
            }
        }

        // straight
        // cout << "straight" << endl;
        if (!ca.is_turn()) {
            if (ca.h.back() == north) {
                if (sa.is_valid()) {
                    // cout << "sn" << endl;
                    print_heading(na.h);
                    if (na.is_end()) {
                        min_path = min(min_path, na.cost);
                    } else if (b.count({na.r, na.c}) == 0 || true) {
                        q.push(na);
                        b.insert({na.r, na.c});
                    }
                }
            } else if (ca.h.back() == south) {
                if (sa.is_valid()) {
                    // cout << "ss" << endl;
                    print_heading(sa.h);
                    if (sa.is_end()) {
                        min_path = min(min_path, sa.cost);
                    } else if (b.count({sa.r, sa.c}) == 0 || true) {
                        q.push(sa);
                        b.insert({sa.r, sa.c});
                    }
                }
            } else if (ca.h.back() == east) {
                if (ea.is_valid()) {
                    // cout << "se" << endl;
                    print_heading(ea.h);
                    if (ea.is_end()) {
                        min_path = min(min_path, ea.cost);
                    } else if (b.count({ea.r, ea.c}) == 0 || true) {
                        q.push(ea);
                        b.insert({ea.r, ea.c});
                    }
                }
            } else if (ca.h.back() == west) {
                if (wa.is_valid()) {
                    print_heading(wa.h);
                    // cout << "sw" << endl;
                    if (wa.is_end()) {
                        min_path = min(min_path, wa.cost);
                    } else if (b.count({wa.r, wa.c}) == 0 || true) {
                        q.push(wa);
                        b.insert({wa.r, wa.c});
                    }
                }
            }
        }
        // cout << "q size: " << q.size() << endl;
        if (print) print = false;
    }
    
    cout << min_path << endl;
}