#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

const vector<int> dr = {-1, 0, 1, 0};
const vector<int> dc = {0, 1, 0, -1};

int main() {
    // open file and read data
    ifstream input("input.txt");
    if (!input) exit(1);
    string line;
    vector<string> grid;
    while (getline(input, line)) { grid.push_back(line); }
    input.close();
    
    // expand grid - 
    // using trick from Jonathan Paulson
    int rsize = grid.size(), csize = grid[0].size();
    vector<vector<bool>> egrid(rsize*3, vector(csize*3, false));
    for (int r = 0; r < rsize; ++r) {
        for (int c = 0; c < csize; ++c) {
            switch (grid[r][c]) {
                case '-':
                    egrid[3*r+1][3*c] = true;
                    egrid[3*r+1][3*c+1] = true;
                    egrid[3*r+1][3*c+2] = true;
                    break;
                case '|':
                    egrid[3*r][3*c+1] = true;
                    egrid[3*r+1][3*c+1] = true;
                    egrid[3*r+2][3*c+1] = true;
                    break;
                case 'L':
                    egrid[3*r][3*c+1] = true;
                    egrid[3*r+1][3*c+1] = true;
                    egrid[3*r+1][3*c+2] = true;
                    break;
                case 'J':
                    egrid[3*r][3*c+1] = true;
                    egrid[3*r+1][3*c] = true;
                    egrid[3*r+1][3*c+1] = true;
                    break;
                case '7':
                    egrid[3*r+1][3*c] = true;
                    egrid[3*r+1][3*c+1] = true;
                    egrid[3*r+2][3*c+1] = true;
                    break;
                case 'F':
                    egrid[3*r+1][3*c+1] = true;
                    egrid[3*r+1][3*c+2] = true;
                    egrid[3*r+2][3*c+1] = true;
                    break;
                case 'S':
                    egrid[3*r][3*c+1] = true;
                    egrid[3*r+1][3*c] = true;
                    egrid[3*r+1][3*c+1] = true;
                    egrid[3*r+1][3*c+2] = true;
                    egrid[3*r+2][3*c+1] = true;
                    break;
            }
        }
    }

    // prep for search
    queue<pair<int,int>> q;
    set<pair<int,int>> v;
    for (int er = 0; er < rsize*3; ++er) {
        q.push({er, 0});
        q.push({er, csize*3-1});
    }
    for (int ec = 0; ec < csize*3; ++ec) {
        q.push({0, ec});
        q.push({rsize*3-1, ec});
    }

    // bfs
    while (!q.empty()) {
        pair<int,int> curr = q.front();
        q.pop();
        if (v.find(curr) != v.end()){
            continue;
        } else if (egrid[curr.first][curr.second]) {
            v.insert(curr);
            continue;
        } else {
            v.insert(curr);
            for (int d = 0; d < 4; ++d) {
                pair<int,int> np = {curr.first+dr[d], curr.second+dc[d]};
                if (0 <= np.first && np.first < rsize*3 && 0 <= np.second && np.second < csize*3)
                    q.push(np);
            }
        }
    }


    // determining ans
    int num = 0;
    for (int r = 0; r < rsize; ++r) {
        for (int c = 0; c < csize; ++c) {
            bool not_visited = true;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (v.find({3*r+i, 3*c+j}) != v.end()) not_visited = false;
                }
            }
            if (not_visited) ++num;
        }
    }
    cout << num << endl;
}