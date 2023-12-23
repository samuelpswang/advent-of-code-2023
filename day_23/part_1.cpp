#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <cassert>
using namespace std;

struct path {
    int r;
    int c;
    int length;
    set<pair<int,int>> s;
    path(): r(0), c(1), length(0) { }
    bool is_done(int rlim, int clim) { return r == rlim && c == clim-1; };
    bool is_valid(int rlim, int clim) { return 0 <= r && r <= rlim && 0 <= c && c <= clim && s.count({r,c}) == 0; }
};

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    string line;
    vector<string> forest;
    while (getline(input, line)) {
        forest.push_back(line);
    }
    input.close();

    queue<path> q;
    path start;
    q.push(start);
    const vector<int> dr = { -1, 1, 0, 0 };
    const vector<int> dc = { 0, 0, -1, 1 };
    const int rlim = forest.size()-1;
    const int clim = forest[0].size()-1;
    int max_length = INT_MIN;

    while (!q.empty()) {
        path cp = q.front(), np;
        q.pop();
        switch (forest[cp.r][cp.c]) {
            case '^':
                np = cp;
                np.s.insert({cp.r, cp.c});
                np.r += dr[0];
                np.c += dc[0];
                np.length += 1;
                if (np.is_done(rlim, clim)) {
                    max_length = max(max_length, np.length);
                } else if (np.is_valid(rlim, clim) && forest[np.r][np.c] != '#') {
                    q.push(np);
                }
                break;
            case 'v':
                np = cp;
                np.s.insert({cp.r, cp.c});
                np.r += dr[1];
                np.c += dc[1];
                np.length += 1;
                if (np.is_done(rlim, clim)) {
                    max_length = max(max_length, np.length);
                } else if (np.is_valid(rlim, clim) && forest[np.r][np.c] != '#') {
                    q.push(np);
                }
                break;
            case '<':
                np = cp;
                np.s.insert({cp.r, cp.c});
                np.r += dr[2];
                np.c += dc[2];
                np.length += 1;
                if (np.is_done(rlim, clim)) {
                    max_length = max(max_length, np.length);
                } else if (np.is_valid(rlim, clim) && forest[np.r][np.c] != '#') {
                    q.push(np);
                }
                break;
            case '>':
                np = cp;
                np.s.insert({cp.r, cp.c});
                np.r += dr[3];
                np.c += dc[3];
                np.length += 1;
                if (np.is_done(rlim, clim)) {
                    max_length = max(max_length, np.length);
                } else if (np.is_valid(rlim, clim) && forest[np.r][np.c] != '#') {
                    q.push(np);
                }
                break;
            case '.':
                for (int i = 0; i < 4; ++i) {
                    np = cp;
                    np.s.insert({cp.r, cp.c});
                    np.r += dr[i];
                    np.c += dc[i];
                    np.length += 1;
                    if (np.is_done(rlim, clim)) {
                        max_length = max(max_length, np.length);
                    } else if (np.is_valid(rlim, clim) && forest[np.r][np.c] != '#') {
                        q.push(np);
                    }
                }
                break;
            case '#':
            default:
                break;
        }
    }

    cout << max_length << endl;
}

// NOTE:
// this is a brute force solution, cannot think of anything better now
// on a mac m1 pro takes around 5 minutes 
