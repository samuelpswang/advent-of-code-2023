// TODO:

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using namespace std;

const int LIM = 12; 

enum dir { n, e, s, w };
enum opt { ks, tl, tr };
const vector<int> dr = { -1, 0, 1, 0 };
const vector<int> dc = { 0, 1, 0, -1 };
dir lt(dir p) { return dir((p+3) % 4); }
dir rt(dir p) { return dir((p+1) % 4); }

struct path {
    bool done;
    bool valid;
    int r;
    int c; 
    int cost;
    vector<dir> h;
    set<pair<int,int>> b;
    
    path(int r, int c, dir pd, const vector<vector<int>>& d): valid(true), r(r), c(c), cost(0) {
        done = ((r == LIM) && (c == LIM));
        cost += d[r][c];
        h.push_back(pd);
        b.insert({r,c});
    }

    path(const path& pp, opt no, const vector<vector<int>>& d) {
        dir nd;
        switch (no) {
            case ks:
                nd = pp.h.back();
                break;
            case tl:
                nd = lt(pp.h.back());
                break;
            case tr:
                nd = rt(pp.h.back());
                break;
        }
        r = pp.r + dr[nd];
        c = pp.c + dc[nd];
        valid = (0 <= r && r <= LIM && 0 <= c && c <= LIM && b.count({r,c}) == 0);
        done = (r == LIM && c == LIM);
        h = pp.h;
        h.push_back(nd);
        b = set<pair<int,int>>(pp.b);
        b.insert({r,c});
        if (valid) cost = pp.cost + d[r][c];
        // cout << "> bsize:" << b.size() << endl;
    }

    bool need_turn() { return (h.size() >= 3) && (h.back() == h.at(h.size()-2) && h.back() == h.at(h.size()-3)); }
    bool is_valid() { return valid; }
    bool is_done() { return done; }
};

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

    queue<path> q;
    path p1(0, 0, e, data), p2(0, 0, s, data);
    q.push(p1);
    // q.push(p2);
    
    int min_cost = INT_MAX;
    while (!q.empty()) {
        path cp = q.front();
        cout << "cp: " << cp.r << " " << cp.c << " " << cp.h.back() << endl;
        q.pop();
        // straight
        if (!cp.need_turn()) {
            path np(cp, ks, data);
            if (np.is_done()) {
                min_cost = min(min_cost, np.cost);
                cout << np.cost << endl;
            }
            else if (np.is_valid()) q.push(np);
            cout << "np: " << np.r << " " << np.c << " " << np.h.back() << endl;
        }
        // turn left
        path lp(cp, tl, data);
        if (lp.is_done()) {
            min_cost = min(min_cost, lp.cost);
            cout << lp.cost << endl;
        }
        else if (lp.is_valid()) q.push(lp);
        cout << "lp: " << lp.r << " " << lp.c << " " << lp.h.back() << endl;
        // turn right
        path rp(cp, tr, data);
        if (rp.is_done()) {
            min_cost = min(min_cost, rp.cost);
            cout << rp.cost << endl;
        }
        else if (rp.is_valid()) q.push(rp);
        cout << "rp: " << rp.r << " " << rp.c << " " << rp.h.back() << endl;

        cout << "q: " << q.size() << endl;
        // if (q.size() > 5) return 0;
    }

    // cout << min_cost << endl;
}